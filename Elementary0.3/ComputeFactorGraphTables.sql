CREATE OR REPLACE FUNCTION assignvariableid() RETURNS void AS $$
DECLARE
	namecursor CURSOR FOR select min(tvariableid), variablename from tvariables group by variablename order by min(tvariableid);
BEGIN
       RAISE NOTICE 'Creating VariableIds in tvariables';
      Drop table if exists tvariables_temp;
      Drop table if exists tvariables_mat;
      Drop sequence if exists tvariables_seq;
      create sequence tvariables_seq minvalue 0 start 0;

      create table tvariables_temp (vid bigint default(nextval('tvariables_seq')), vname varchar);
       insert into tvariables_temp (vname) (select variablename from tvariables group by variablename order by min(tvariableid));
       analyse tvariables_temp;
      create table tvariables_mat as (select T1.tvariableid, T1.variablename, T2.vid as variableid from tvariables T1 join tvariables_temp T2 on T1.variablename = T2.vname);	

      create index tv_mat_vid on tvariables_mat (variableid);
      create index tv_mat_tvid on tvariables_mat (tvariableid);

      drop table if exists tvariables;
      drop table if exists tvariables_temp;

       Analyse tvariables_mat;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION updatevariableid() RETURNS void AS $$
BEGIN
       RAISE NOTICE 'Updating tvariableids in Variables';
      create index var_tvariableid on variables(tvariableid);
       analyse variables;
      
      drop table if exists variables_temp;
      create table variables_temp as (select TVM.variableid as tvariableid, V.type, V.dtype, V.cvalue, V.lowerbound, V.upperbound, V.nfactors from tvariables_mat TVM join variables V on V.tvariableid = TVM.tvariableid);
      drop table variables;
      alter table variables_temp rename to variables;
	
       RAISE NOTICE 'Updating tvariableids in Edges';
      create index edges_tvariableid on edges (tvariableid);
       analyse edges;
      drop table if exists edges_temp;
       create table edges_temp as (select TVM.variableid as tvariableid, E.factorid, E.groupid, E.aux, E.is_vg from tvariables_mat TVM join Edges E on E.tvariableid = TVM.tvariableid);
      drop table edges;
      alter table edges_temp rename to edges;
	
       RAISE NOTICE 'Updating tvariableids in Bounds';
      create index bounds_tvariableid on bounds(tvariableid);
       analyse bounds;
      drop table if exists bounds_temp;
      create table bounds_temp with (fillfactor = 70) as (select TVM.variableid as tvariableid, B.lowerbound, B.upperbound, B.lbtvariableid, B.ubtvariableid from tvariables_mat TVM join Bounds B on B.tvariableid = TVM.tvariableid);
      drop table bounds;
      alter table bounds_temp rename to bounds;

       RAISE NOTICE 'Updating tvariableids for Upper and Lower Bounds';
      create index bounds_lbtvariableid on bounds(lbtvariableid);
      create index bounds_ubtvariableid on bounds(ubtvariableid);
       update bounds as B set lbtvariableid = TV.variableid from (select tvariableid, variableid from tvariables_mat) as TV where B.lbtvariableid = TV.tvariableid;
       update bounds as B set ubtvariableid = TV.variableid from (select tvariableid, variableid from tvariables_mat) as TV where B.ubtvariableid = TV.tvariableid;

       RAISE NOTICE 'Creating Required Indices';
      create index variable_tvariableid on variables(tvariableid);
      create index bounds_tvariableid on bounds(tvariableid);
      create index edges_factorid on edges (factorid);
      create index edges_tvariableid on edges (tvariableid);
      create index factors_factorid on factors(factorid);

       Analyse;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION updateStochasticNodesWithLogicalNodeDistributionParameters() RETURNS void AS $$
BEGIN
       RAISE NOTICE 'Updating StochasticNodes With LogicalNode As Distribution Parameters';

      Drop table if exists VariableToModify;
      Drop table if exists NewEdges;

       RAISE NOTICE 'Creating Variables To Modify';
      create table VariableToModify as (With X as (select F.factorid as Ffid, E.tvariableid from (Factors F Join Edges E on F.distname = 'LOGICAL' and F.factorid = E.factorid and E.groupid = F.ngroups - 1)) select X.Ffid, E1.tvariableid, E1.factorid as efid, E1.groupid, E1.aux, E1.is_vg from X, Edges E1, Factors F1 where E1.factorid = F1.factorid and F1.distname <> 'LOGICAL' and E1.factorid <> X.Ffid and X.tvariableid = E1.tvariableid);

       RAISE NOTICE 'Creating NewEdges';
      create index vtm_ffid on variabletomodify (ffid);
      create table NewEdges as (select V.efid, V.groupid, E.tvariableid, F.pid, V.tvariableid as oldtvariableid, V.aux as oldaux, V.is_vg as oldvg, E.aux as newaux from (Factors F Join Edges E on F.distname = 'LOGICAL' and F.factorid = E.factorid and E.groupid <> F.ngroups - 1) JOIN VariableToModify V on F.factorid = V.Ffid);

	Analyse;

       RAISE NOTICE 'Creating New Edges For Logical Node Expressions';
      drop table if exists edges_temp;
      create table edges_temp as (((select E.tvariableid, E.factorid, E.groupid, E.aux, E.is_vg from edges E) EXCEPT ALL (select N.oldtvariableid, N.efid, N.groupid, N.oldaux, N.oldvg from NewEdges N)) UNION ALL (select N.tvariableid, N.efid, N.groupid, N.newaux, 'f' from newedges N));
      drop table edges;
      alter table edges_temp rename to edges;
       
       RAISE NOTICE 'Creating Required Indices';
      create index edge_multicolumn on edges (factorid, groupid);
      create index edges_tvariableid on edges (tvariableid);

       RAISE NOTICE 'Updating Group aux';
      drop table if exists group_newaux_temp;
      create table group_newaux_temp as (select G.factorid, G.groupid, N.pid, G.nvar, G.aux from groups G join NewEdges N on G.factorid = N.efid and G.groupid = N.groupid group by G.factorid, G.groupid, N.pid, G.aux, G.nvar);

      drop table if exists groups_temp;
      create table groups_temp as (select * from groups G1) except all (select GA.factorid, GA.groupid, GA.aux, GA.nvar from group_newaux_temp GA) union all (select GA.factorid, GA.groupid, GA.pid, GA.nvar from group_newaux_temp GA);
      drop table groups;
      alter table groups_temp rename to groups;

      create index group_multicolumn on groups (factorid, groupid, aux);

      drop table if exists NewEdges;
      drop table if exists VariableToModify;
      drop table if exists group_newaux_temp;
       Analyse;	
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION updatenvars() RETURNS void AS $$
BEGIN
       RAISE NOTICE 'Updating nvars in groups';
      drop table if exists groups_temp;
      create table groups_temp as (select G.factorid, G.groupid, G.aux, count(G.factorid) as nvar from Edges E join Groups G on E.factorid = G.factorid and E.groupid = G.groupid group by G.factorid, G.groupid, G.aux);
      drop table groups;
      alter table groups_temp rename to groups;

       RAISE NOTICE 'Creating indices/tmp table for updating nvars';
      drop table if exists fac_count_temp;
      create table fac_count_temp as (select E.factorid, count(E.factorid) as nvars from Edges E Join Factors F on E.factorid = F.factorid group by E.factorid);
      create index f_c_t_fid on fac_count_temp (factorid);

       RAISE NOTICE 'Updating nvars in factors';
      drop table if exists factors_temp;
      create table factors_temp as (select F.factorid, F.pid, F.factoroptimisationtype, F.distname, F.ngroups, FCT.nvars from factors F join fac_count_temp FCT on F.factorid = FCT.factorid);
      drop table factors;
      alter table factors_temp rename to factors;

      drop table if exists fac_count_temp;
       Analyse;
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION assignnfactors() RETURNS void AS $$
BEGIN
       RAISE NOTICE 'Updating NFactors';
      drop table if exists tv_f_count_temp;
      create table tv_f_count_temp as (select tvariableid, count(distinct(factorid)) from edges group by tvariableid);
      create index tv_f_t_tvid on tv_f_count_temp (tvariableid);
      drop table if exists variables_temp;
      create table variables_temp with (fillfactor = 50) as (select V.tvariableid, V.type, V.dtype, V.cvalue, V.lowerbound, V.upperbound, TVFC.count as nfactors from tv_f_count_temp TVFC join variables V on V.tvariableid = TVFC.tvariableid);
      drop table variables;
      alter table variables_temp rename to variables;
      
      drop table if exists tv_f_count_temp;
      create index var_tvariableid on variables(tvariableid);
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION computebounds() RETURNS void AS $$
DECLARE
	boundrecord RECORD;
	ubcursor CURSOR FOR select B1.tvariableid, B2.upperbound from bounds B1 join bounds B2 on B2.tvariableid = B1.ubtvariableid and B2.upperbound is not NULL and B1.upperbound is NULL;
	lbcursor CURSOR FOR select B1.tvariableid, B2.lowerbound from bounds B1 join bounds B2 on B2.tvariableid = B1.lbtvariableid and B2.lowerbound is not NULL and B1.lowerbound is NULL;
BEGIN
	RAISE NOTICE 'Computing UpperBound for all Variables';
	loop
		open ubcursor;
		fetch ubcursor into boundrecord;
		if boundrecord is NULL then
			close ubcursor;
			exit;
		end if;

		close ubcursor;
		for currentubound in ubcursor loop
			UPDATE BOUNDS SET UPPERBOUND = currentubound.upperbound, UBTVARIABLEID = NULL WHERE TVARIABLEID = currentubound.tvariableid;
		end loop;
	end loop;

	RAISE NOTICE 'Computing Lowerbound for all Variables';
	loop
		open lbcursor;
		fetch lbcursor into boundrecord;
		if boundrecord is NULL then
			close lbcursor;
			exit;
		end if;

		close lbcursor;
		for currentlbound in lbcursor loop
			UPDATE BOUNDS SET LOWERBOUND = currentlbound.lowerbound, LBTVARIABLEID = NULL WHERE TVARIABLEID = currentlbound.tvariableid;
		end loop;
	end loop;
	
	RAISE NOTICE 'Setting the Bounds in Variables Table';
       drop table if exists variables_temp;
       create table variables_temp as (select * from variables where type = 0) UNION ALL (select B.tvariableid, V.type, V.dtype, V.cvalue, B.lowerbound, B.upperbound, V.nfactors from bounds B join variables V on V.tvariableid = B.tvariableid);
       drop table variables;
       alter table variables_temp rename to variables;
	
	Analyse;
	
END;
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE FUNCTION cleanseFactorGraphTables() RETURNS void AS $$
BEGIN
	PERFORM assignvariableid();
	PERFORM updatevariableid();
	PERFORM updateStochasticNodesWithLogicalNodeDistributionParameters();
	PERFORM updatenvars();
	PERFORM assignnfactors();
	PERFORM computebounds();
END
$$ LANGUAGE 'plpgsql';

