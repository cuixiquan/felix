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

#include <glob.h>
#include <fstream>
#include <map>
#include <vector>
#include <iterator>
#include <typeinfo>
#include "src/AdapterClasses/BugsParse.h"
#include "src/AdapterClasses/NodeNameMatrixEvaluator.h"
#include "src/AdapterClasses/JoinEvaluator.h"
#include "src/AdapterClasses/AddStochasticNode.h"
#include "src/AdapterClasses/AddLogicalNode.h"
#include "src/AdapterClasses/Bugs_Inference.h"
#define BUGS_CONFIG "bugs_config"
#define COMPUTE_FGVE "ComputeFactorGraphTables.sql"
#define COMPUTE_BOUNDS "ComputeBoundsStoredProc.sql"

namespace hazy {

void parseConfigFile(map<string, string> &bugs_config_values, string config_file){
	fstream configfilestream(config_file);

	while(configfilestream.good()) {
		string config;
		std::getline(configfilestream, config);
		if(config.empty())
			continue;
		vector<string> configvec;
		getConfigs(configvec, config);
		if(configvec.size() < 1 or configvec.size() > 2)
		{
			cout << "Error In Configuration File. Please check bugs_config" << endl;
			exit(1);
		}
		if(configvec.size() == 1)
			bugs_config_values[configvec[0]] = "";
		else
			bugs_config_values[configvec[0]] = configvec[1];
	}

	configfilestream.close();
}

void runBUGSHybrid(long epochs) {
	Timer timer;
	timer.restart();

	map<string, string> bugs_config_values;
	parseConfigFile(bugs_config_values, BUGS_CONFIG);
	int rowstofetch = ::atol(bugs_config_values["Rows_per_fetch"].c_str());
	string model_file = bugs_config_values["Bugs_Model"];
	string data_file = bugs_config_values["Bugs_Data"];
	string inits_file = bugs_config_values["Bugs_Inits"];
	std::vector<std::string> bugs_paramstomonitor;
	getMonitors(bugs_paramstomonitor, bugs_config_values["Bugs_Monitors"]);

	MainMemoryData* data = new MainMemoryData(config_tmp_folder);
	parseData(data, data_file);
	MainMemoryData* initdata = new MainMemoryData("");
	parseData(initdata, inits_file);
	Program* prog = parseProgram(model_file);
	prog->eval(data);
	data->parserfiledetails->flushAndCloseFiles();

	PGconn* conn = connect_Postgres(bugs_config_values["PostgreSQL_uname"], bugs_config_values["PostgreSQL_password"],
				bugs_config_values["PostgreSQL_host"], bugs_config_values["PostgreSQL_port"], bugs_config_values["PostgreSQL_dbname"]);
	dropGroundingPhaseTables_Postgres(conn);
	dropFGVETables_Postgres(conn);

	copyGroundingPhaseTables_Postgres(conn, config_tmp_folder);
	data->parserfiledetails->deleteFiles(config_tmp_folder);

	createMaterializedView_Postgres(conn);

	Bugs_AddNodes addnodes(conn, data, initdata);
	addnodes.joinAndAddNodes(rowstofetch);
	data->fgfiledetails->flushAndCloseFiles();
	dropGroundingPhaseTables_Postgres(conn);
	disconnect_Postgres(conn);

	conn = connect_Greenplum(bugs_config_values["Greenplum_uname"], bugs_config_values["Greenplum_password"],
			bugs_config_values["Greenplum_host"], bugs_config_values["Greenplum_port"], bugs_config_values["Greenplum_dbname"]);
	dropexistingtables_Greenplum(conn);
	LoadFGdataIntoTables_Greenplum(conn, config_tmp_folder);
	data->fgfiledetails->deleteFiles(config_tmp_folder);
	cleanseFactorGraphTables_Greenplum(conn, COMPUTE_BOUNDS);
	cout << "Time taken in Bugs_Parser: " << timer.elapsed() << " seconds" << endl;
	doInference(conn, bugs_config_values["EXP_STORAGE"], epochs, rowstofetch, bugs_paramstomonitor);
	cout << "Total Time taken : " << timer.elapsed() << " sec" << endl;
	disconnect_Greenplum(conn);

	delete prog;
	delete initdata;
	delete data;
}

void runBUGSPostgreSQL(long epochs) {
	Timer timer;
	timer.restart();

	map<string, string> bugs_config_values;
	parseConfigFile(bugs_config_values, BUGS_CONFIG);
	int rowstofetch = ::atol(bugs_config_values["Rows_per_fetch"].c_str());
	string model_file = bugs_config_values["Bugs_Model"];
	string data_file = bugs_config_values["Bugs_Data"];
	string inits_file = bugs_config_values["Bugs_Inits"];
	std::vector<std::string> bugs_paramstomonitor;
	getMonitors(bugs_paramstomonitor, bugs_config_values["Bugs_Monitors"]);


	MainMemoryData* data = new MainMemoryData(config_tmp_folder);
	parseData(data, data_file);
	MainMemoryData* initdata = new MainMemoryData("");
	parseData(initdata, inits_file);
	Program* prog = parseProgram(model_file);
	prog->eval(data);
	data->parserfiledetails->flushAndCloseFiles();

	PGconn* conn = connect_Postgres(bugs_config_values["PostgreSQL_uname"], bugs_config_values["PostgreSQL_password"],
			bugs_config_values["PostgreSQL_host"], bugs_config_values["PostgreSQL_port"], bugs_config_values["PostgreSQL_dbname"]);
	dropGroundingPhaseTables_Postgres(conn);
	dropFGVETables_Postgres(conn);

	copyGroundingPhaseTables_Postgres(conn, config_tmp_folder);
	data->parserfiledetails->deleteFiles(config_tmp_folder);

	createMaterializedView_Postgres(conn);

	Bugs_AddNodes addnodes(conn, data, initdata);
	addnodes.joinAndAddNodes(rowstofetch);
	data->fgfiledetails->flushAndCloseFiles();
	dropGroundingPhaseTables_Postgres(conn);

	LoadFGVEIntoTables_Postgres(conn, config_tmp_folder);
	data->fgfiledetails->deleteFiles(config_tmp_folder);
	cleanseFGVETables_Postgres(conn, COMPUTE_FGVE);

	cout << "Time taken in Bugs_Parser: " << timer.elapsed() << " seconds" << endl;
	doInference(conn, bugs_config_values["EXP_STORAGE"], epochs, rowstofetch, bugs_paramstomonitor);
	cout << "Total Time taken : " << timer.elapsed() << " seconds" << endl;

	disconnect_Postgres(conn);

	delete prog;
	delete initdata;
	delete data;
}
}
