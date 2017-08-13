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


#ifndef BUGS_INFERENCE_H_
#define BUGS_INFERENCE_H_

#include "Bugs_Common.h"

namespace hazy {

void addEdges(PGconn *& conn, PGresult *& res, AbstractFactorGraph *fg, std::map<std::string,DistributionAdapter> &distributionMap,long norowstofetch)
{
	cout << " Adding Edges Into FactorGraph" << endl;
	executeSQLCommand(conn, res, "DECLARE edgecursor CURSOR FOR Select factorid, tvariableid, groupid, aux, is_vg from edges order by tvariableid, factorid, groupid",
			"Cannot Set Cursor for Edge", "");
	PQclear(res);

	while(true)
	{
		stringstream ss;
		ss << norowstofetch;
		res = executeSQLQuery(conn, "FETCH " + ss.str() + " from edgecursor");
		char *result = PQcmdStatus(res); // FETCH count
		string fetchresult(result);

		// Break if fetch count = 0
		if(atoi(fetchresult.substr(6, fetchresult.size()).c_str()) == 0)
			break;

		for(int currenttuple = 0; currenttuple < PQntuples(res); currenttuple++)
		{
			OID fid = ::atol(PQgetvalue(res, currenttuple, 0));
			OID vid = ::atol(PQgetvalue(res, currenttuple, 1));
			int groupid = ::atoi(PQgetvalue(res,currenttuple, 2));
			int aux = ::atoi(PQgetvalue(res, currenttuple, 3));
			string is_vg = PQgetvalue(res,currenttuple, 4);
			bool b = (is_vg == "t");
			fg->addEdge(vid, fid, groupid, aux, b);
		}

		PQclear(res);
	}

	executeSQLCommand(conn, res, "CLOSE edgecursor", "", "");
}

void addGroups(PGconn *& conn, PGresult *& res, AbstractFactorGraph *fg, std::map<std::string,DistributionAdapter> &distributionMap, long norowstofetch)
{
	cout << " Adding Groups Into FactorGraph" << endl;
	executeSQLCommand(conn, res, "DECLARE groupcursor CURSOR FOR Select factorid, groupid, nvar, aux from groups order by factorid, groupid",
			"Cannot Set Cursor for Groups", "");
	PQclear(res);

	while(true)
	{
		stringstream ss;
		ss << norowstofetch;
		res = executeSQLQuery(conn, "FETCH " + ss.str() + " from groupcursor");
		char *result = PQcmdStatus(res); // FETCH count
		string fetchresult(result);

		// Break if fetch count = 0
		if(atoi(fetchresult.substr(6, fetchresult.size()).c_str()) == 0)
			break;

		for(int currenttuple = 0; currenttuple < PQntuples(res); currenttuple++)
		{
			OID fid = ::atol(PQgetvalue(res, currenttuple, 0));
			int groupid = ::atoi(PQgetvalue(res, currenttuple, 1));
			Long nvar = ::atol(PQgetvalue(res, currenttuple, 2));
			double aux = ::atof(PQgetvalue(res, currenttuple, 3));
			fg->addGroup(fid, groupid, nvar, aux);
		}

		PQclear(res);
	}

	executeSQLCommand(conn, res, "CLOSE groupcursor", "", "");
}

void addVariables(PGconn *& conn, PGresult *& res, AbstractFactorGraph *fg, std::map<std::string,DistributionAdapter> &distributionMap, long norowstofetch)
{
	cout << " Adding Variables Into FactorGraph" << endl;
	executeSQLCommand(conn, res, "DECLARE varcursor CURSOR FOR select tvariableid, type, dtype, cvalue, lowerbound, upperbound, nfactors from variables "
			"group by tvariableid, type, dtype, cvalue, lowerbound, upperbound, nfactors order by tvariableid", "Cannot Set Cursor for Variable", "");
	PQclear(res);

	int i = 0;
	while(true)
	{
		stringstream ss;
		ss << norowstofetch;
		res = executeSQLQuery(conn, "FETCH " + ss.str() + " from varcursor");
		char *result = PQcmdStatus(res); // FETCH count
		string fetchresult(result);

		// Break if fetch count = 0
		if(atoi(fetchresult.substr(6, fetchresult.size()).c_str()) == 0)
			break;

		for(int currenttuple = 0; currenttuple < PQntuples(res); currenttuple++)
		{
			i++;
			OID oid = ::atol(PQgetvalue(res, currenttuple, 0));
			VariableType type = (VariableType)(::atoi(PQgetvalue(res, currenttuple, 1)));
			DomainType dtype = (DomainType)(::atoi(PQgetvalue(res, currenttuple, 2)));
			double cvalue = ::atof(PQgetvalue(res, currenttuple, 3));
			double lowerbound = ::atof(PQgetvalue(res, currenttuple, 4));
			double upperbound = ::atof(PQgetvalue(res, currenttuple, 5));
			int nfactors = ::atol(PQgetvalue(res, currenttuple, 6));
			fg->addVariable(Variable(oid, type, dtype, cvalue, lowerbound, upperbound), nfactors);
		}

		PQclear(res);
	}

	cout << " Total Number of Variables In Factor Graph : " << i << endl;
	executeSQLCommand(conn, res, "CLOSE varcursor", "", "");
}

void addFactors(PGconn *& conn, PGresult *& res, AbstractFactorGraph *fg, std::map<std::string,DistributionAdapter> &distributionMap, long norowstofetch)
{
	cout << " Adding Factors Into FactorGraph" << endl;
	executeSQLCommand(conn, res, "DECLARE factorcursor CURSOR FOR Select factorid, ngroups, nvars, distname, pid, "
			"factoroptimisationtype from factors order by factorid", "Cannot Set Cursor for Factor", "");
	PQclear(res);

	while(true)
	{
		//Fetch Next row
		stringstream ss;
		ss << norowstofetch;
		res = executeSQLQuery(conn, "FETCH " + ss.str() + " from factorcursor");
		char *result = PQcmdStatus(res); // FETCH count
		string fetchresult(result);

		// Break if fetch count = 0
		if(atoi(fetchresult.substr(6, fetchresult.size()).c_str()) == 0)
			break;

		for(int currenttuple = 0; currenttuple < PQntuples(res); currenttuple++)
		{
			OID oid = ::atol(PQgetvalue(res, currenttuple, 0));
			int ngroups = ::atoi(PQgetvalue(res, currenttuple, 1));
			Long nvars = ::atol(PQgetvalue(res, currenttuple, 2));
			string factorname = PQgetvalue(res, currenttuple, 3);
			int pid = ::atoi(PQgetvalue(res, currenttuple, 4));
			FactorOptType factoropttype = (FactorOptType)(::atoi(PQgetvalue(res, currenttuple, 5)));

			fg->addFactor(FactorFactory_BUGS::generate(oid, ngroups, nvars, distributionMap[factorname].vg, distributionMap[factorname].potential, pid,
					factoropttype));
		}
		PQclear(res);
	}

	executeSQLCommand(conn, res, "CLOSE factorcursor", "", "");
}

void modifyinmemoryLNEmetadata(PGconn *conn) {
	PGresult *res;
	cout << " Modifying Logical Node Expressions" << endl;
	for(auto it = inmemorylogicalnodes.begin(); it != inmemorylogicalnodes.end(); it++) {
		vector<Long> originalvariableids;
		set<Long> sortanddupeliminatedvariableids; //set will sort the ids
		map<Long, Long> originalvidlocation; //hash it
		bool hasNestedIndex = false;
		for(auto gvidit = (*it).second->metadata->inputlocationparams.begin(); gvidit != (*it).second->metadata->inputlocationparams.end(); gvidit++)
		{
			if((*gvidit) == -1) // Indicates there is a nested index array. Will be sent as the last parameter
				hasNestedIndex = true;
			else {
				stringstream ss;
				ss << "select variableid from tvariables_mat where tvariableid = " << (*gvidit);
				res = executeSQLQuery(conn, ss.str());
				originalvariableids.push_back(::atol(PQgetvalue(res, 0, 0)));
				PQclear(res);
			}
		}

		for(auto ovid=originalvariableids.begin(); ovid != originalvariableids.end(); ovid++)
			sortanddupeliminatedvariableids.insert((*ovid));

		Long i = 0;
		for(auto sdevid = sortanddupeliminatedvariableids.begin(); sdevid != sortanddupeliminatedvariableids.end(); sdevid++, i++)
			originalvidlocation[(*sdevid)] = i;

		(*it).second->metadata->inputlocationparams.clear();
		for(auto ovid=originalvariableids.begin(); ovid != originalvariableids.end(); ovid++)
			(*it).second->metadata->inputlocationparams.push_back(originalvidlocation[(*ovid)]);

		if(hasNestedIndex)
			(*it).second->metadata->inputlocationparams.push_back(i); // last parameter
	}
}

void constructFactorGraph(PGconn *conn, AbstractFactorGraph *fg, long norowstofetch)
{
	cout << "******************** Begin Constructing FactorGraph ****************** " << endl;
	std::map<std::string,DistributionAdapter> distributionMap = getDistributionMap();
	PGresult *res;

	executeSQLCommand(conn, res, "BEGIN", "", "");
	modifyinmemoryLNEmetadata(conn);
	addFactors(conn, res, fg, distributionMap, norowstofetch);
	addVariables(conn, res, fg, distributionMap, norowstofetch);
	addGroups(conn, res, fg, distributionMap, norowstofetch);
	addEdges(conn, res, fg, distributionMap, norowstofetch);
	executeSQLCommand(conn, res, "END", "", "");

	PQclear(res);
	cout << "******************** End Constructing FactorGraph ****************** " << endl;
}

void callElementaryInference(PGconn *conn, AbstractFactorGraph *fg, long epochs, std::vector<std::string> &bugs_paramstomonitor)
{
	Timer timer;
	for(int i = 0;i < epochs;i++){
		timer.restart();
		int changed = fg->inference(1);
		float elapsed = timer.elapsed();
		std::cout << "EPOCH " << i << " CHANGED = " << changed << " [ " << elapsed << " seconds]" << std::endl;
	}

	std::cout << "******************** Results ************************" << endl;
	std::cout << "VarName\tE(X)\t  Standard Deviation" << endl;

	std::map<std::string, VariableStatus> results;
	PGresult *res = NULL;
	for(OID ovid = 0;ovid <= fg->get_maxid();ovid++){
		  VariableStatus status;
		  fg->getVariableStatus(ovid, status);
		  if(status.vtype != QUERY)
			  continue;

		  stringstream query;
		  query << "Select distinct(variablename) from tvariables_mat where variableid = " << ovid << endl;
		  res = executeSQLQuery(conn, query.str());

		  results[PQgetvalue(res, 0, 0)] = status;
	}

	if(results.size() == 0)
			cout << "Attention!!! [BUGSWarning] NO QUERY VARIABLE IN THE MODEL " << endl;
	else
		for(std::map<std::string,hazy::VariableStatus>::iterator it = results.begin();it != results.end();++it)
		{
			string barenodename = it->first;
			if(barenodename.find('[') != -1)
				barenodename = barenodename.substr(0, barenodename.find('['));
			if(bugs_paramstomonitor.size() !=0 && std::find(bugs_paramstomonitor.begin(), bugs_paramstomonitor.end(), barenodename) == bugs_paramstomonitor.end())
				continue;
			std::cout << (*it).first << "\t" << (*it).second.expectation << "\t\t" << (*it).second.standard_derivation << std::endl;
		}

	if(res != NULL)
		PQclear(res);
}

void doInference(PGconn *conn, string exp_storage, long epochs, long norowstofetch, std::vector<std::string> &bugs_paramstomonitor){
	EXP_STORAGE = exp_storage;
	EXP_MAT = "FCOC";
	AbstractFactorGraph *fg = get_factorgraph_from_config();

	constructFactorGraph(conn, fg, norowstofetch);
	dropFGVETables_Postgres(conn);

	if(epochs != 0)
		callElementaryInference(conn, fg, epochs, bugs_paramstomonitor);

	droptvariablesmat_Postgres(conn);
}

}
#endif /* BUGS_INFERENCE_H_ */
