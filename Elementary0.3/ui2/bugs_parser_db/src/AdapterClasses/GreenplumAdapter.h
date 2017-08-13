// Copyright 2013 Hazy Research [http://hazy.cs.wisc.edu]
//   University of Wisconsin-Madison [http://www.wisc.edu]
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//  Filename: BUGS.h
//  Project:  Elementary_0.2
//
//  Created by Shriram Sridharan on 05/13/13.

#ifndef GREENPLUMADAPTER_H_
#define GREENPLUMADAPTER_H_

#include <libpq-fe.h>
#include <string>
#include <iostream>
#include "DBCommon.h"
using namespace std;

PGconn* connect_Greenplum(string user, string password, string host, string port, string dbname) {
	PGconn* conn;
	stringstream connectionstring;
	if(!user.empty())
		connectionstring << "user = " << user;
	if(!password.empty())
		connectionstring << " password = " << password;
	if(!host.empty())
		connectionstring << " host = " << host;
	if(!port.empty())
		connectionstring << " port = " << port;
	if(!dbname.empty())
		connectionstring << " dbname = " << dbname;

	conn = PQconnectdb(connectionstring.str().c_str());
	cout << "Connecting to Greenplum " << connectionstring.str() << endl;

	if (PQstatus(conn) != CONNECTION_OK)
	{
		cout << "Connection to Greenplum failed: " << 	PQerrorMessage(conn) << endl;
		cout << "Please check whether the username, password, host and dbname information have been correctly specified." << endl;
		cout << "Defaults are user=greenplum password=<empty> host=localhost port=5432 dbname=bugs" << endl;
		exit_nicely(conn);
	}
	cout << "Successfully Connected to Greenplum" << endl;
	return conn;
}

void disconnect_Greenplum(PGconn *conn) {
	PQfinish(conn);
}

//void copyIntoTable(PGconn *conn, string templocation) {
//	cout << "******************** Begin Loading Data Into Postgres ****************** " << endl;
//
//	PGresult   *res;
//
//	//('C','T') //('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
//	executeSQLCommand(conn, res, "create  table nodenamematrix (id bigint, name varchar NULL, indexid bigint[] NULL, value double precision NULL, isunaryminus boolean, variabletype smallint) with (appendonly = true) distributed by (id);", "create  table nodenamematrix failed", "Created nodenamematrix Table");
//	executeSQLCommand(conn, res, "create  table node (nodeid bigint, e_l_id bigint NULL, nodeparams bigint[], nodetype int, distname varchar NULL) with (appendonly = true) distributed by (nodeid);", "create  table node failed", "Created node Table");
//	executeSQLCommand(conn, res, "create  table LIMITATION (id bigint, type varchar, lowerbound double precision NULL, upperbound double precision NULL) with (appendonly = true) distributed by (id);", "create  table Limitation failed", "Created Limitation Table");
//
//	string copynodenamematrixquery = "COPY NODENAMEMATRIX FROM '" + templocation + "nodenamematrix.csv' with delimiter ';' NULL as '';";
//	string copynodequery = "COPY NODE FROM '" + templocation + "node.csv' with delimiter ';' NULL as '';";
//	string copylimitationquery = "COPY LIMITATION FROM '" + templocation + "limitation.csv' with delimiter ';' NULL as '';";
//	executeSQLCommand(conn, res, copynodenamematrixquery, "Copy Nodenamematrix failed", "Successfully copied Nodenamenamematrix");
//	executeSQLCommand(conn, res, copynodequery, "Copy Node failed", "Successfully copied Node");
//	executeSQLCommand(conn, res, copylimitationquery, "Copy Limitation failed", "Successfully copied Limitation");
//
////	executeSQLCommand(conn, res, "create index node_nodeid on node(nodeid);", "node_nodeid Index creation failed", "Successfully created node_nodeid Index on Node" );
////	executeSQLCommand(conn, res, "create index limitation_id on limitation(id);", "limitation_id Index creation failed", "Successfully created limitation_id Index on Limitation" );
////	executeSQLCommand(conn, res, "CREATE INDEX nnm_id on Nodenamematrix (id)", "nnm_id Index creation failed", "Successfully created nnm_id Index on NodenameMatrix" );
////	executeSQLCommand(conn, res, "cluster nnm_id on Nodenamematrix;", "clustering NNM failed", "Successfully clustered NodenameMatrix" );
////	executeSQLCommand(conn, res, "Analyse;", "Analyse failed", "Analyse Successful");
//
//	PQclear(res);
//
//	cout << "******************** End Loading Data Into Postgres ****************** " << endl;
//}

void LoadFGdataIntoTables_Greenplum(PGconn* conn, string templocation) {

	cout << "******************** Begin Loading FactorGraph data Into Postgres ****************** " << endl;

	PGresult   *res;
	executeSQLCommand(conn, res, "create  table factors (factorid bigint, pid int, factoroptimisationtype int, "
			"distname varchar, ngroups int, nvars bigint NULL) with (appendonly = true) distributed by (factorid);",
			"create  table factor failed", "Created factor Table");
	executeSQLCommand(conn, res, "create  table variables (tvariableid bigint, type int, "
			"dtype int, cvalue double precision, lowerbound double precision NULL, upperbound double precision NULL, nfactors bigint NULL) with (appendonly = true) distributed by (tvariableid);",
			"create  table variable failed", "Created variable Table");
	executeSQLCommand(conn, res, "create  table groups (factorid bigint, groupid int, aux double precision, nvar bigint NULL) with (appendonly = true) distributed by (factorid);", "create  table groups failed", "Created groups table");
	executeSQLCommand(conn, res, "create  table edges (tvariableid bigint, factorid bigint, groupid int, aux int, is_vg boolean) with (appendonly = true) distributed by (tvariableid);", "create  table edges failed", "Created edges table");
	executeSQLCommand(conn, res, "create  table bounds (tvariableid bigint, lowerbound double precision NULL, upperbound double precision NULL, "
			"lbtvariableid bigint NULL, ubtvariableid bigint NULL) distributed by (tvariableid);", "create  table bounds failed", "Created bounds table");
	executeSQLCommand(conn, res, "create  table tvariables (tvariableid bigint, variablename varchar, variableid bigint NULL) with (appendonly = true) distributed by (tvariableid);"
				, "create  table tvariables failed", "Created tvariables table");

	string copyfactorsquery = "COPY factors FROM '" + templocation + "factors.csv' with delimiter ';' NULL as '';";
	string copyvariablesquery = "COPY variables FROM '" + templocation + "variables.csv' with delimiter ';' NULL as '';";
	string copygroupsquery = "COPY groups FROM '" + templocation + "groups.csv' with delimiter ';' NULL as '';";
	string copyedgesquery = "COPY edges FROM '" + templocation + "edges.csv' with delimiter ';' NULL as '';";
	string copyboundsquery = "COPY bounds FROM '" + templocation + "bounds.csv' with delimiter ';' NULL as '';";
	string copytvariablesquery = "COPY tvariables FROM '" + templocation + "tvariables.csv' with delimiter ';' NULL as '';";
	executeSQLCommand(conn, res, copyfactorsquery , "Copy factors failed", "Successfully copied factors");
	executeSQLCommand(conn, res, copyvariablesquery, "Copy variables failed", "Successfully copied variables");
	executeSQLCommand(conn, res, copygroupsquery, "Copy groups failed", "Successfully copied groups");
	executeSQLCommand(conn, res, copyedgesquery, "Copy edges failed", "Successfully copied edges");
	executeSQLCommand(conn, res, copyboundsquery, "Copy bounds failed", "Successfully copied bounds");
	executeSQLCommand(conn, res, copytvariablesquery, "Copy tvariables failed", "Successfully copied tvariables");

	PQclear(res);

	cout << "******************** End Loading FactorGraph data Into Postgres ****************** " << endl;
}

void dropexistingtables_Greenplum(PGconn* conn) {
	PGresult   *res;
	executeSQLCommand(conn, res, "DROP TABLE if exists factors", "", "Dropped Table factors");
	executeSQLCommand(conn, res, "DROP TABLE if exists variables", "", "Dropped Table variables");
	executeSQLCommand(conn, res, "DROP TABLE if exists groups", "", "Dropped Table groups");
	executeSQLCommand(conn, res, "DROP TABLE if exists edges", "", "Dropped Table edges");
	executeSQLCommand(conn, res, "DROP TABLE if exists bounds", "", "Dropped Table bounds");
	executeSQLCommand(conn, res, "DROP TABLE if exists tvariables", "", "Dropped Table tvariables");
	executeSQLCommand(conn, res, "DROP TABLE if exists tvariables_mat", "", "Dropped Table tvariables_mat");
	PQclear(res);
}

void dropFGVE(PGconn *conn){
	PGresult   *res;
	executeSQLCommand(conn, res, "DROP TABLE if exists factors", "", "Dropped Table factors");
	executeSQLCommand(conn, res, "DROP TABLE if exists variables", "", "Dropped Table variables");
	executeSQLCommand(conn, res, "DROP TABLE if exists groups", "", "Dropped Table groups");
	executeSQLCommand(conn, res, "DROP TABLE if exists edges", "", "Dropped Table edges");
	executeSQLCommand(conn, res, "DROP TABLE if exists bounds", "", "Dropped Table bounds");
	executeSQLCommand(conn, res, "DROP TABLE if exists tvariables", "", "Dropped Table tvariables");
	executeSQLCommand(conn, res, "DROP TABLE if exists newedges", "", "Dropped Table newedges");
	executeSQLCommand(conn, res, "DROP TABLE if exists VariableToModify", "", "Dropped Table VariableToModify");
	PQclear(res);
	cout << "******************** Dropped Unwanted Tables ****************** " << endl;
}

void droptvmat(PGconn *conn){
	PGresult   *res;
	executeSQLCommand(conn, res, "DROP TABLE if exists tvariables_mat", "", "Dropped Table tvariables_mat");
	PQclear(res);
}

//void createMaterializedView(PGconn* conn){
//	cout << "******************** Creating Materialized View  ****************** " << endl;
//	PGresult *res;
//
//	hazy::Timer timer;
//	timer.restart();
//
//	//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
//	string command = "create  table MaterializedNNM with (appendonly = true) as (select N1.id as n1id, N1.name as n1name, N1.value as n1value, "
//			"N1.variabletype as n1type, N1.indexid as n1indexid, N2.id as n2id, N2.name as n2name, N2.value as n2value, "
//			"N2.variabletype as n2type from nodenamematrix N1 left join nodenamematrix N2 on N2.id = ANY(N1.indexid) where "
//			"(N1.variabletype <> 4 and N1.variabletype <> 5 and N1.variabletype <> 6 and N1.variabletype <> 7) order by N1.indexid, N2.id) distributed by (n1id);";
//
//	executeSQLCommand(conn, res, command, "Failed to Create Materialized View", "Successfully Created Materialized View MaterializedNNM");
//	PQclear(res);
//	cout << "******************** End Creating Materialized View ****************** " << endl;
//}

void assignvariableid(PGconn *conn){
	PGresult *res;
	executeSQLCommand(conn, res, "Drop table if exists tvariables_temp;", "" , "");
	executeSQLCommand(conn, res, "Drop table if exists tvariables_mat;", "" , "");
	executeSQLCommand(conn, res, "Drop sequence if exists tvariables_seq;", "" , "");

	executeSQLCommand(conn, res, "create sequence tvariables_seq minvalue 0 start 0;", "Create Sequence Failed" , "Create Sequence Succeeded");
	executeSQLCommand(conn, res, "create table tvariables_temp (vid bigint default(nextval('tvariables_seq')), vname varchar) with (appendonly=true) distributed by (vid);", "Create tvariables_temp failed" , "Create tvariables_temp Succeeded");
	executeSQLCommand(conn, res, "insert into tvariables_temp (vname) (select variablename from tvariables group by variablename order by min(tvariableid));", "Insert tvariables_temp failed" , "Insert tvariables_temp succeeded");
	executeSQLCommand(conn, res, "create table tvariables_mat with (appendonly=true) as (select T1.tvariableid, T1.variablename, T2.vid as variableid from tvariables T1 join tvariables_temp T2 on T1.variablename = T2.vname) distributed by (tvariableid);", "Create tvariables_mat Failed" , "Create tvariables_mat Succeeded");

	executeSQLCommand(conn, res, "drop table if exists tvariables;", "" , "");
	executeSQLCommand(conn, res, "drop table if exists tvariables_temp;", "" , "");
	PQclear(res);
}

void updatevariableid(PGconn *conn){

	PGresult *res;
	executeSQLCommand(conn, res, "drop table if exists variables_temp;", "" , "");
	executeSQLCommand(conn, res, "create table variables_temp with (appendonly = true) as (select TVM.variableid as tvariableid, V.type, V.dtype, V.cvalue, V.lowerbound, V.upperbound, V.nfactors from tvariables_mat TVM join variables V on V.tvariableid = TVM.tvariableid) distributed by (tvariableid);", "Create variables_temp failed" , "Create variables_temp Succeeded");
	executeSQLCommand(conn, res, "drop table variables;", "" , "");
	executeSQLCommand(conn, res, "alter table variables_temp rename to variables;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists edges_temp;", "" , "");
	executeSQLCommand(conn, res, "create table edges_temp with (appendonly = true) as (select TVM.variableid as tvariableid, E.factorid, E.groupid, E.aux, E.is_vg from tvariables_mat TVM join Edges E on E.tvariableid = TVM.tvariableid) distributed by (tvariableid);", "Create edges_temp failed" , "Create edges_temp Succeeded");
	executeSQLCommand(conn, res, "drop table edges;", "" , "");
	executeSQLCommand(conn, res, "alter table edges_temp rename to edges;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists bounds_temp;", "" , "");
	executeSQLCommand(conn, res, "create table bounds_temp as (select TVM.variableid as tvariableid, B.lowerbound, B.upperbound, B.lbtvariableid, B.ubtvariableid from tvariables_mat TVM join Bounds B on B.tvariableid = TVM.tvariableid) distributed by (tvariableid);", "Create bounds_temp failed" , "Create bounds_temp Succeeded");
	executeSQLCommand(conn, res, "drop table bounds;", "" , "");
	executeSQLCommand(conn, res, "alter table bounds_temp rename to bounds;", "" , "");

	executeSQLCommand(conn, res, "update bounds as B set lbtvariableid = TV.variableid from (select tvariableid, variableid from tvariables_mat) as TV where B.lbtvariableid = TV.tvariableid;", "Update tvid in lowerbound failed" , "Update tvid in lowerbound Succeeded");
	executeSQLCommand(conn, res, "update bounds as B set ubtvariableid = TV.variableid from (select tvariableid, variableid from tvariables_mat) as TV where B.ubtvariableid = TV.tvariableid;", "Update tvid in upperbound failed" , "Update tvid in upperbound Succeeded");
	PQclear(res);
}

void updateStochasticNodesWithLogicalNodeDistributionParameters(PGconn *conn) {
	PGresult *res;
	executeSQLCommand(conn, res, "Drop table if exists VariableToModify;", "" , "");
	executeSQLCommand(conn, res, "Drop table if exists NewEdges;", "" , "");
	executeSQLCommand(conn, res, "create table VariableToModify with (appendonly = true) as (With X as (select F.factorid as Ffid, E.tvariableid from (Factors F Join Edges E on F.distname = 'LOGICAL' and F.factorid = E.factorid and E.groupid = F.ngroups - 1)) select X.Ffid, E1.tvariableid, E1.factorid as efid, E1.groupid, E1.aux, E1.is_vg from X, Edges E1, Factors F1 where E1.factorid = F1.factorid and F1.distname <> 'LOGICAL' and E1.factorid <> X.Ffid and X.tvariableid = E1.tvariableid) distributed by (efid);", "Create VariableToModify failed" , "Create VariableToModify Succeeded");
	executeSQLCommand(conn, res, "create table NewEdges with (appendonly = true) as (select V.efid, V.groupid, E.tvariableid, F.pid, V.tvariableid as oldtvariableid, V.aux as oldaux, V.is_vg as oldvg, E.aux as newaux from (Factors F Join Edges E on F.distname = 'LOGICAL' and F.factorid = E.factorid and E.groupid <> F.ngroups - 1) JOIN VariableToModify V on F.factorid = V.Ffid) distributed by (efid);", "Create NewEdges failed" , "Create NewEdges Succeeded");

	executeSQLCommand(conn, res, "drop table if exists edges_temp;", "" , "");
	executeSQLCommand(conn, res, "create table edges_temp with (appendonly = true) as (((select E.tvariableid, E.factorid, E.groupid, E.aux, E.is_vg from edges E) EXCEPT ALL (select N.oldtvariableid, N.efid, N.groupid, N.oldaux, N.oldvg from NewEdges N)) UNION ALL (select N.tvariableid, N.efid, N.groupid, N.newaux, 'f' from newedges N)) distributed by (tvariableid);", "Create Edges_Temp failed" , "Create Edges_Temp Succeeded");
	executeSQLCommand(conn, res, "drop table edges;", "" , "");
	executeSQLCommand(conn, res, "alter table edges_temp rename to edges;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists group_newaux_temp;", "" , "");
	executeSQLCommand(conn, res, "create table group_newaux_temp with (appendonly = true) as (select G.factorid, G.groupid, N.pid, G.nvar, G.aux from groups G join NewEdges N on G.factorid = N.efid and G.groupid = N.groupid group by G.factorid, G.groupid, N.pid, G.aux, G.nvar) distributed by (factorid);", "Create group_newaux_temp failed" , "Create group_newaux_temp Succeeded");
	executeSQLCommand(conn, res, "drop table if exists groups_temp;", "" , "");
	executeSQLCommand(conn, res, "create table groups_temp with (appendonly = true) as (select * from groups G1) except all (select GA.factorid, GA.groupid, GA.aux, GA.nvar from group_newaux_temp GA) union all (select GA.factorid, GA.groupid, GA.pid, GA.nvar from group_newaux_temp GA) distributed by (factorid);", "Create groups_temp failed" , "Create groups_temp Succeeded");
	executeSQLCommand(conn, res, "drop table groups;", "" , "");
	executeSQLCommand(conn, res, "alter table groups_temp rename to groups;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists NewEdges;", "" , "");
	executeSQLCommand(conn, res, "drop table if exists VariableToModify;", "" , "");
	executeSQLCommand(conn, res, "drop table if exists group_newaux_temp;", "" , "");

	PQclear(res);
}

void updatenvars(PGconn *conn){
	PGresult *res;
	executeSQLCommand(conn, res, "drop table if exists groups_temp;", "" , "");
	executeSQLCommand(conn, res, "create table groups_temp with (appendonly = true) as (select G.factorid, G.groupid, G.aux, count(G.factorid) as nvar from Edges E join Groups G on E.factorid = G.factorid and E.groupid = G.groupid group by G.factorid, G.groupid, G.aux) distributed by (factorid);", "Create groups_temp failed" , "Create groups_temp Succeeded");
	executeSQLCommand(conn, res, "drop table groups;", "" , "");
	executeSQLCommand(conn, res, "alter table groups_temp rename to groups;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists fac_count_temp;", "" , "");
	executeSQLCommand(conn, res, "create table fac_count_temp with (appendonly = true) as (select E.factorid, count(E.factorid) as nvars from Edges E Join Factors F on E.factorid = F.factorid group by E.factorid) distributed by (factorid);", "Create fac_count_temp failed" , "Create fac_count_temp Succeeded");

	executeSQLCommand(conn, res, "drop table if exists factors_temp;", "" , "");
	executeSQLCommand(conn, res, "create table factors_temp with (appendonly = true) as (select F.factorid, F.pid, F.factoroptimisationtype, F.distname, F.ngroups, FCT.nvars from factors F join fac_count_temp FCT on F.factorid = FCT.factorid) distributed by (factorid);", "Create factors_temp failed" , "Create factors_temp Succeeded");
	executeSQLCommand(conn, res, "drop table factors;", "" , "");
	executeSQLCommand(conn, res, "alter table factors_temp rename to factors;", "" , "");

	executeSQLCommand(conn, res, "drop table if exists fac_count_temp;", "" , "");
	PQclear(res);
}

void assignnfactors(PGconn *conn){
	PGresult *res;
	executeSQLCommand(conn, res, "drop table if exists tv_f_count_temp;", "" , "");
	executeSQLCommand(conn, res, "create table tv_f_count_temp with (appendonly = true) as (select tvariableid, count(distinct(factorid)) from edges group by tvariableid) distributed by (tvariableid);", "Create tv_f_count_temp failed" , "Create tv_f_count_temp Succeeded");
	executeSQLCommand(conn, res, "drop table if exists variables_temp;", "" , "");
	executeSQLCommand(conn, res, "create table variables_temp with (appendonly = true) as (select V.tvariableid, V.type, V.dtype, V.cvalue, V.lowerbound, V.upperbound, TVFC.count as nfactors from tv_f_count_temp TVFC join variables V on V.tvariableid = TVFC.tvariableid) distributed by (tvariableid);", "Create variables_temp for updating nfactors failed" , "Create variables_temp for updating nfactors Succeeded");
	executeSQLCommand(conn, res, "drop table variables;", "" , "");
	executeSQLCommand(conn, res, "alter table variables_temp rename to variables;", "" , "");
	executeSQLCommand(conn, res, "drop table if exists tv_f_count_temp;", "" , "");
	PQclear(res);
}

void computebounds(PGconn *conn, string storedprocname){
	string computeboundsfunction = "";
	fstream storedprocfile(storedprocname);
	PGresult *res;

	stringstream storedproc;
	while(storedprocfile.good()) {
		char c = storedprocfile.get();
		if(storedprocfile.good())
			storedproc << c;
	}

	storedprocfile.close();
	executeSQLCommand(conn, res, storedproc.str(), "", "");
	executeSQLCommand(conn, res, "SELECT computebounds()", "", "Successfully Computed Bounds");

	executeSQLCommand(conn, res, "drop table if exists variables_temp;", "" , "");
	executeSQLCommand(conn, res, "create table variables_temp with (appendonly = true) as (select * from variables where type = 0) "
			"UNION ALL "
			"(select B.tvariableid, V.type, V.dtype, V.cvalue, B.lowerbound, B.upperbound, V.nfactors from bounds B join variables V on V.tvariableid = B.tvariableid) distributed by (tvariableid);",
			"Create variables_temp for updating bounds failed" , "Create variables_temp for updating bounds Succeeded");
	executeSQLCommand(conn, res, "drop table variables;", "" , "");
	executeSQLCommand(conn, res, "alter table variables_temp rename to variables;", "" , "");

	PQclear(res);
}

void cleanseFactorGraphTables_Greenplum(PGconn* conn, string computeboundsstoredprocname){
	cout << "******************** Begin Cleansing FactorGraph tables ****************** " << endl;

	assignvariableid(conn);
	updatevariableid(conn);
	updateStochasticNodesWithLogicalNodeDistributionParameters(conn);
	updatenvars(conn);
	assignnfactors(conn);
	computebounds(conn, computeboundsstoredprocname);

	cout << "******************** End Cleansing FactorGraph tables ****************** " << endl;
}

#endif /* GREENPLUMADAPTER_H_ */
