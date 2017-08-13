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
	Analyse;
END;
$$ LANGUAGE 'plpgsql';
