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

#ifndef POSTGRESADAPTER_H_
#define POSTGRESADAPTER_H_
#include <libpq-fe.h>
#include <string>
#include <iostream>
#include "DBCommon.h"
using namespace std;

PGconn* connect_Postgres(string user, string password, string host, string port, string dbname) {
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
	cout << "Connecting to Postgres " << connectionstring.str() << endl;

	if (PQstatus(conn) != CONNECTION_OK)
	{
		cout << "Connection to database failed: " << 	PQerrorMessage(conn) << endl;
		cout << "Please check whether the username, password, host and dbname information have been correctly specified." << endl;
		cout << "Defaults are user=postgres password=<empty> host=localhost port=5433 dbname=bugs" << endl;
		exit_nicely(conn);
	}
	cout << "Successfully Connected to Postgres" << endl;
	return conn;
}

void disconnect_Postgres(PGconn *conn) {
	PQfinish(conn);
}

void copyGroundingPhaseTables_Postgres(PGconn *conn, string templocation) {
	cout << "******************** Begin Loading Data Into Postgres ****************** " << endl;

	PGresult   *res;

	//('C','T') //('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
	executeSQLCommand(conn, res, "create  table nodenamematrix (id bigint, name varchar NULL, indexid bigint[] NULL, value double precision NULL, isunaryminus boolean, variabletype smallint);", "create  table nodenamematrix failed", "Created nodenamematrix Table");
	executeSQLCommand(conn, res, "create  table node (nodeid bigint, e_l_id bigint NULL, nodeparams bigint[], nodetype int, distname varchar NULL);", "create  table node failed", "Created node Table");
	executeSQLCommand(conn, res, "create  table LIMITATION (id bigint, type varchar, lowerbound double precision NULL, upperbound double precision NULL);", "create  table Limitation failed", "Created Limitation Table");

	string copynodenamematrixquery = "COPY NODENAMEMATRIX FROM '" + templocation + "nodenamematrix.csv' with delimiter ';' NULL as '';";
	string copynodequery = "COPY NODE FROM '" + templocation + "node.csv' with delimiter ';' NULL as '';";
	string copylimitationquery = "COPY LIMITATION FROM '" + templocation + "limitation.csv' with delimiter ';' NULL as '';";
	executeSQLCommand(conn, res, copynodenamematrixquery, "Copy Nodenamematrix failed", "Successfully copied Nodenamenamematrix");
	executeSQLCommand(conn, res, copynodequery, "Copy Node failed", "Successfully copied Node");
	executeSQLCommand(conn, res, copylimitationquery, "Copy Limitation failed", "Successfully copied Limitation");

	executeSQLCommand(conn, res, "create index node_nodeid on node(nodeid);", "node_nodeid Index creation failed", "Successfully created node_nodeid Index on Node" );
	executeSQLCommand(conn, res, "create index limitation_id on limitation(id);", "limitation_id Index creation failed", "Successfully created limitation_id Index on Limitation" );
	executeSQLCommand(conn, res, "CREATE INDEX nnm_id on Nodenamematrix (id)", "nnm_id Index creation failed", "Successfully created nnm_id Index on NodenameMatrix" );
	executeSQLCommand(conn, res, "cluster Nodenamematrix using nnm_id", "clustering NNM failed", "Successfully clustered NodenameMatrix" );
	executeSQLCommand(conn, res, "Analyse;", "Analyse failed", "Analyse Successful");
	PQclear(res);

	cout << "******************** End Loading Data Into Postgres ****************** " << endl;
}

void LoadFGVEIntoTables_Postgres(PGconn* conn, string templocation) {

	cout << "******************** Begin Loading FactorGraph data Into Postgres ****************** " << endl;

	PGresult   *res;
	executeSQLCommand(conn, res, "create  table factors (factorid bigint, pid int, factoroptimisationtype int, "
			"distname varchar, ngroups int, nvars bigint NULL) with (fillfactor = 50);",
			"create  table factor failed", "Created factor Table");
	executeSQLCommand(conn, res, "create  table variables (tvariableid bigint, type int, "
			"dtype int, cvalue double precision, lowerbound double precision NULL, upperbound double precision NULL, nfactors bigint NULL) with (fillfactor = 50);",
			"create  table variable failed", "Created variable Table");
	executeSQLCommand(conn, res, "create  table groups (factorid bigint, groupid int, aux double precision, nvar bigint NULL) with (fillfactor = 50);", "create  table groups failed", "Created groups table");
	executeSQLCommand(conn, res, "create  table edges (tvariableid bigint, factorid bigint, groupid int, aux int, is_vg boolean) with (fillfactor = 50);", "create  table edges failed", "Created edges table");
	executeSQLCommand(conn, res, "create  table bounds (tvariableid bigint, lowerbound double precision NULL, upperbound double precision NULL, "
			"lbtvariableid bigint NULL, ubtvariableid bigint NULL) with (fillfactor = 50);", "create  table bounds failed", "Created bounds table");
	executeSQLCommand(conn, res, "create  table tvariables (tvariableid bigint, variablename varchar, variableid bigint NULL) with (fillfactor = 50);"
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

	// create indices
	executeSQLCommand(conn, res, "Analyse;", "Analyse failed", "Analyse Successful");

	PQclear(res);

	cout << "******************** End Loading FactorGraph data Into Postgres ****************** " << endl;
}

void dropGroundingPhaseTables_Postgres(PGconn* conn) {
	PGresult   *res;
	executeSQLCommand(conn, res, "DROP TABLE if exists MaterializedNNM", "", "Dropped Table MaterializedNNM");
	executeSQLCommand(conn, res, "DROP TABLE if exists NODENAMEMATRIX", "", "Dropped Table Nodenamematrix");
	executeSQLCommand(conn, res, "DROP TABLE if exists Node", "", "Dropped Table Node");
	executeSQLCommand(conn, res, "DROP TABLE if exists Limitation", "", "Dropped Table Limitation");

	PQclear(res);
}

void dropFGVETables_Postgres(PGconn *conn){
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

void droptvariablesmat_Postgres(PGconn *conn){
	PGresult   *res;
	executeSQLCommand(conn, res, "DROP TABLE if exists tvariables_mat", "", "Dropped Table tvariables_mat");
	PQclear(res);
}

void createMaterializedView_Postgres(PGconn* conn){
	cout << "******************** Creating Materialized View  ****************** " << endl;
	PGresult *res;

	hazy::Timer timer;
	timer.restart();

	//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
	string command = "create  table MaterializedNNM as (select N1.id as n1id, N1.name as n1name, N1.value as n1value, "
			"N1.variabletype as n1type, N1.indexid as n1indexid, N2.id as n2id, N2.name as n2name, N2.value as n2value, "
			"N2.variabletype as n2type from nodenamematrix N1 left join nodenamematrix N2 on N2.id = ANY(N1.indexid) where "
			"(N1.variabletype = 1 or N1.variabletype = 2 or N1.variabletype = 3) order by N1.indexid, N2.id);";

	executeSQLCommand(conn, res, command, "Failed to Create Materialized View", "Successfully Created Materialized View MaterializedNNM");
	executeSQLCommand(conn, res, "create index matnnm_n1id on MaterializedNNM(n1id);", "matnnm_n1id Index Creation failed", "Successfully Created Index on MaterializedNNM");
	executeSQLCommand(conn, res, "Cluster MaterializedNNM using matnnm_n1id", "Failed to Cluster MaterializedNNM", "Successfully Clustered MaterializedNNM");
	executeSQLCommand(conn, res, "Analyse MaterializedNNM;", "Analyse MaterializedNNM failed", "Analyse MaterializedNNM Successful");
	PQclear(res);
	cout << "******************** End Creating Materialized View ****************** " << endl;
}

void cleanseFGVETables_Postgres(PGconn* conn, string storedprocname){
	cout << "******************** Begin Cleansing FactorGraph tables ****************** " << endl;
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
	executeSQLCommand(conn, res, "SELECT cleanseFactorGraphTables()", "", "Successfully Cleansed Factor Tables");

	PQclear(res);
	cout << "******************** End Cleansing FactorGraph tables ****************** " << endl;
}

#endif /* POSTGRESADAPTER_H_ */
