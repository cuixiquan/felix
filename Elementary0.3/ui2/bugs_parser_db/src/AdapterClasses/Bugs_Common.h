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

#ifndef BUGS_COMMON_H_
#define BUGS_COMMON_H_

#include <map>
#include "../AdapterClasses/LogicalNodeComposite.hpp"
#include "NodeNameMatrixEvaluator.h"
#include "Bugs_DistributionMap.h"

using namespace std;

namespace hazy {

class Bounds {
public:
	string ubvarname, lbvarname;
	double lowerbound, upperbound;
	int ubparam, lbparam;

	double distributionparametervalues[4] = {-DBL_MAX, -DBL_MAX, -DBL_MAX, -DBL_MAX}; //as NCHG is maximum with 4 parameters

	Bounds() {
		ubvarname = "";
		lbvarname = "";
		lowerbound = -DBL_MAX;
		upperbound = DBL_MAX;
		ubparam = -1;
		lbparam = -1;
	}
};

class NodeInput {
public:
	string distname;
	Long e_l_id;
	string limitationtype;
	string lowerbound;
	string upperbound;

	NodeNameMatrixEvaluationInput nnmeiRV; // NNMEI for the Random Variable
	vector<NodeNameMatrixEvaluationInput> nnmeiNodeParams;

	NodeInput() {}
	~NodeInput() {
		nnmeiNodeParams.clear();
		nnmeiNodeParams.shrink_to_fit();
	}
};

class Bugs_AddNodes {
public:
	PGconn *conn;
	IData* data;
	IData* inits;
	std::map<std::string,DistributionAdapter> distributionMap;
	Long factorid;
	int groupid;
	string factorname;
    int normal_distribution_vg = 0;
    Bugs_AddNodes(PGconn *conn, IData *data, IData *inits)
    {
        this->conn = conn;
        this->data = data;
        this->inits = inits;
        this->distributionMap = getDistributionMap();
    }

    void joinAndAddNodes(Long N);
private:
    inline double getInit(string nodename);
    void addStochasticNode(NodeInput & nodeinput);
    void addLogicalNodes(NodeInput & nodeinput);
    void getBareNodeNameAndIndices(string nodename, string & barenodename, vector<int> & indices);
    void handleStochasticConstantDistributionParameter(double value, Bounds & bounds, int indexid);
    void handleStochasticDistributionNodeParameter(NodeNameMatrixEvaluationResult *& evalresult, Bounds & bounds, int indexid);
    void addGroupToFile(NodeNameMatrixEvaluationResult *& distparamevalresult);
    void handleStochasticRVBounds(NodeInput & nodeinput, double & rvlowerbound, double & rvupperbound, ParameterType varptype, Bounds & bounds, string & nodename);
    void handleStochasticRandomVariable(NodeInput & nodeinput, Bounds & bounds, ParameterType varptype);
    void handleBoundsForCategoricalDistribution(Bounds & bounds, int numberofvariables);
    void handleStochasticNodeWithNodeIndexDistributionParameter(NodeNameMatrixEvaluationResult *& distparamevalresult, Bounds & bounds);
    void handleLogicalNodeConstantExpression(NodeInput & nodeinput, int expressionid);
    void manipulateMapAndMetadata(int & expressionid, map<string,Long> & expressionnodenames, string constantvariable);
    void handleLogicalNodeParameter(NodeNameMatrixEvaluationResult *& expressionnodes, int & expressionid, map<string,Long> & expressionnodenames);
    void handleLogicalRandomVariable(NodeInput & nodeinput);
    void handleLogicalNodeConstantValueInExpression(double value, int & expressionid, map<string,Long> & expressionnodenames);
    void handleLogicalNodeWithNodeIndex(NodeNameMatrixEvaluationResult *& expressionnodes, int & expressionid);
    bool tryAndFetchFromJoinCursor(int & currenttuple, int & notuplesincursor, Long noRowsToFetch, PGresult *& res);
    void constructNNMEI(PGresult *& res, int & currenttuple, NodeNameMatrixEvaluationInput & nnmeitoconstruct, int & notuplesincursor, Long noRowsToFetch);
    void handleConstantEvidVariable(double value, int aux);
};

void Bugs_AddNodes::handleConstantEvidVariable(double value, int aux)
{
	stringstream constantvariable;
	constantvariable << value;
	Long gvid = data->fgfiledetails->handletvariables(constantvariable.str());
	data->fgfiledetails->handleConstantVariable(gvid, EVID, REAL, value);
	data->fgfiledetails->handleEdge(factorid, gvid, groupid, aux, false);
}

void getIndexIds(vector<Long> &indices, string indexfield) {
	if(indexfield == "")
		return;

	string indiceswithcomma = indexfield.substr(1, indexfield.size() - 2);
	std::stringstream ss(indiceswithcomma);
	std::string item;

	while(std::getline(ss, item, ',')) {
		 indices.push_back(::atol(item.c_str()));
	}
}

void getConfigs(vector<string> &configs, string indexfield) {
	if(indexfield == "")
		return;

	std::stringstream ss(indexfield);
	std::string item;

	while(std::getline(ss, item, '=')) {
		 configs.push_back(item);
	}
}

void getMonitors(vector<string> &monitors, string indexfield) {
	if(indexfield == "")
		return;

	std::stringstream ss(indexfield);
	std::string item;

	while(std::getline(ss, item, ',')) {
		monitors.push_back(item);
	}
}

void Bugs_AddNodes::getBareNodeNameAndIndices(string nodename, string &barenodename, vector<int> &indices){
    int openbracketpos = nodename.find('[');
    barenodename = nodename;
    if(openbracketpos != -1)
    {
    	barenodename = nodename.substr(0, openbracketpos);
    	string indiceswithcomma = nodename.substr(openbracketpos + 1, nodename.size() - openbracketpos - 2);
		std::stringstream ss(indiceswithcomma);
		std::string item;
		while(std::getline(ss, item, ',')) {
			 indices.push_back(atoi(item.c_str()));
            }
        }
}

inline double Bugs_AddNodes::getInit(string nodename)
{
	float init = 1.0;
	string initnodename = nodename;
	vector<int> indices;
	getBareNodeNameAndIndices(nodename, initnodename, indices);
	if(inits->contains(initnodename, indices))
		init = inits->getData(initnodename, indices);

	return init;
}

}

#endif
