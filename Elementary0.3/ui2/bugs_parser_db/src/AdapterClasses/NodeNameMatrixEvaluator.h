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

#ifndef NODENAMEMATRIXEVALUATOR_H_
#define NODENAMEMATRIXEVALUATOR_H_
#include <vector>
#include <set>
#include <string>
#include <map>
#include "PostgresAdapter.h"
#include "GreenplumAdapter.h"
#include "../DataClasses/IData.hpp"
using namespace std;

namespace hazy {
enum NNMER_TYPE {UnivariateNode, MultivariateNode, Constant};

class NodeNameMatrixEvaluationResult {
public:
	vector<string> nodenames; // Encompasses the indices too.

	// Sent so that it would be easy to handle Node index
	vector<set<int> > indices;
	map<int, string> indexnodenames;

	double value;
	NNMER_TYPE type;
};

class MaterializedViewInput {
public:
	string name;
	double value;
	int variabletype;
};

class NodeNameMatrixEvaluationInput {
public:
	NodeNameMatrixEvaluationInput() {}
	~NodeNameMatrixEvaluationInput() {
		nodeindices.clear();
		nodeindices.shrink_to_fit();
	}
	MaterializedViewInput node;
	vector<MaterializedViewInput> nodeindices;
};

class IndexRange {
public:
	int rangebegin;
	int rangeend;
	IndexRange(){}
	IndexRange(int rangebegin, int rangeend) {
		this->rangebegin = rangebegin;
		this->rangeend = rangeend;
	}
};

/*
 * The following datastructure is used to store the index ranges of multidimensional nodes that are accessed in some other place
 * in the program using an empty or node index.
 * This is populated the first time an empty/node index is encountered (a sql query is fired).
 * This data structure should fit in memory if the program fits in memory.
 *
 * Read as: map<Nodename, map<indexno, IndexRange>>
 */
map<string, map<int, IndexRange>> inmemoryHashOfIndexRanges;


void testevalresult(NodeNameMatrixEvaluationResult* &evalresult){
	if(evalresult->type == UnivariateNode || evalresult->type == MultivariateNode) {
		cout << endl << "Nodenames : " << endl;
		for(auto it = evalresult->nodenames.begin(); it != evalresult->nodenames.end(); it++)
			cout << (*it) << endl;

		cout << "Indices :" << endl;
		int indexno = 0;
		for(auto it = evalresult->indices.begin(); it != evalresult->indices.end(); it++, indexno++)
		{
			cout << "Index No " << indexno << ":" ;
			for(auto setit = (*it).begin(); setit != (*it).end(); setit++)
				cout << (*setit) << ",";
			cout << endl;
		}

		cout << "Index Nodenames : " << endl;
		for(auto it = evalresult->indexnodenames.begin(); it != evalresult->indexnodenames.end(); it++, indexno++)
		{
			cout << (*it).first << "=>" << (*it).second << ",";
		}

		cout << endl;
	}
	else if (evalresult->type == Constant)
	{
		cout << endl << " Constant : " << endl;
		cout << evalresult->value <<endl;
	}

}

void getRangeLowerAndUpperBound(string &rangefield, int &low, int &high) {
	vector<Long> indices;
	std::stringstream ss(rangefield);
	std::string item;

	while(std::getline(ss, item, ':')) {
		 indices.push_back(::atol(item.c_str()));
	}

	assert(indices.size() == 2);

	low = indices[0];
	high = indices[1];
}

void handleRangeIndex(string &range, NodeNameMatrixEvaluationResult *& evalresult, int indexno, string nodename, bool shouldaddtohash)
{
    int lowerbound = 0, upperbound = 0;
    getRangeLowerAndUpperBound(range, lowerbound, upperbound);
    if(evalresult->indices.size() <= indexno)
        evalresult->indices.push_back(set<int>());

    for(int i = lowerbound;i <= upperbound;i++)
        evalresult->indices[indexno].insert(i);

    // populate the inmemoryhash here so that this function will not be called the next time for the same nodename
    if(shouldaddtohash) {
    	if(inmemoryHashOfIndexRanges[nodename].find(indexno) == inmemoryHashOfIndexRanges[nodename].end())
    	{
    		inmemoryHashOfIndexRanges[nodename][indexno] = IndexRange(lowerbound, upperbound);
    	}
    	else
    	{
    		if(inmemoryHashOfIndexRanges[nodename][indexno].rangebegin > lowerbound)
    			inmemoryHashOfIndexRanges[nodename][indexno].rangebegin = lowerbound;
    		if(inmemoryHashOfIndexRanges[nodename][indexno].rangeend < upperbound)
    			inmemoryHashOfIndexRanges[nodename][indexno].rangeend = upperbound;
    	}
    }
}

void handleRangeIndexGivenHighAndLowIndex(IndexRange &range, NodeNameMatrixEvaluationResult *& evalresult, int indexno)
{
    if(evalresult->indices.size() <= indexno)
        evalresult->indices.push_back(set<int>());

    for(int i = range.rangebegin;i <= range.rangeend;i++)
        evalresult->indices[indexno].insert(i);
}

/*
 * Check in Data.
 * Else, Check in inmemoryHashOfIndexRanges.
 * Else, fire a SQL query.
 */
void handleDimesionOfParentNodeForIndex(PGconn *conn, NodeNameMatrixEvaluationInput &nnmei, int indexno,
		NodeNameMatrixEvaluationResult *& evalresult, IData* data)
{
    /*
	* Getting all the nodes' indices that have the same name as the original nodename.
	* The nodes' indices can be Range or Constant.
	* Get the dimensions of the parent Univariate Node.
	*/

	string nodename = nnmei.node.name;

	//Check in MM Data
	DataType datatype = data->getVariableType(nodename);
	if(datatype == VECTOR || datatype == ARRAY)
	{
		int dimension = data->getDimension(nodename, indexno);
		if(evalresult->indices.size() <= indexno)
			evalresult->indices.push_back(set<int>());
		for(int i=1; i<= dimension; i++)
			evalresult->indices[indexno].insert(i);

		return;
	}


	// check in inmemoryHashOfIndexRanges
	if(inmemoryHashOfIndexRanges.find(nodename) != inmemoryHashOfIndexRanges.end() &&
			inmemoryHashOfIndexRanges[nodename].find(indexno) != inmemoryHashOfIndexRanges[nodename].end())
	{
		handleRangeIndexGivenHighAndLowIndex(inmemoryHashOfIndexRanges[nodename][indexno], evalresult, indexno);
		return;
	}

	// fire a SQL query
    stringstream ss;
    ss << (indexno + 1);
    string originalnodeindicescorrespondingtothecurrentindex = "select n2type, n2name, n2value from MaterializedNNM "
    		"where n1name = '"+ nodename + "' and n2id = n1indexid["+ ss.str() +"] and (n2type = 7 or n2type = 5);";

    PGresult *noderes = executeSQLQuery(conn, originalnodeindicescorrespondingtothecurrentindex);
    for(int noderesno = 0;noderesno < PQntuples(noderes);noderesno++)
    {
        int noderesvariabletype = ::atoi(PQgetvalue(noderes, noderesno, 0));
        if(noderesvariabletype == 5){//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
            if(evalresult->indices.size() <= indexno)
                evalresult->indices.push_back(set<int>());

            evalresult->indices[indexno].insert(::atoi(PQgetvalue(noderes, noderesno, 2)));
        }
        else// Believe on the query.
        {
            string range = PQgetvalue(noderes, noderesno, 1);
            handleRangeIndex(range, evalresult, indexno, nodename, true); // will store the range index
        }
    }

    PQclear(noderes);
}

void handleNodeIndex(PGconn *conn, NodeNameMatrixEvaluationInput &nnmei, int indexno, NodeNameMatrixEvaluationResult *& evalresult, IData* data)
{
    // Put the indexname in the map
    evalresult->indexnodenames[indexno] = nnmei.nodeindices[indexno].name;
    handleDimesionOfParentNodeForIndex(conn, nnmei, indexno, evalresult, data);
}

void handleIndices(PGconn *conn, NodeNameMatrixEvaluationInput &nnmei, NodeNameMatrixEvaluationResult* &evalresult, IData* data)
{
    // retrieving indices
    // each row in the resultset is an index of the original node
	//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
    for (int indexno = 0; indexno < nnmei.nodeindices.size(); indexno++) {
		 int indexvariabletype = nnmei.nodeindices[indexno].variabletype;
		 if(indexvariabletype == 5) { // Handle Constant Index
			 if(evalresult->indices.size() <= indexno)
				 evalresult->indices.push_back(set<int>());
			 evalresult->indices[indexno].insert(nnmei.nodeindices[indexno].value);
		 }
		 else if(indexvariabletype == 6){ // Handle Node Index
			 handleNodeIndex(conn, nnmei, indexno, evalresult, data);
		 }
		 else if(indexvariabletype == 7 && nnmei.node.variabletype == 1) { // HandleRangeIndices Only For MultivariateNode
			 string range = nnmei.nodeindices[indexno].name;
			 handleRangeIndex(range, evalresult, indexno, "", false); // should not add index
		 }
		 else if(indexvariabletype == 4 && nnmei.node.variabletype == 1) { // Handle Empty Indices only for Multivariate Nodes
			 handleDimesionOfParentNodeForIndex(conn, nnmei, indexno, evalresult, data);
		 }
		 else {
			 cout << "[Error] Index can only be Node or Constant in a Univariate Node" <<endl;
			 assert(false);
		 }
	}
}

void createNodenamesWithIndices(string &nodename, NodeNameMatrixEvaluationResult* &evalresult) {

	vector<string> finals;
	for(auto it = evalresult->indices.begin(); it != evalresult->indices.end(); it++){
		vector<string> tempresults;
		if(finals.empty()){
			for(auto setit = (*it).begin(); setit != (*it).end(); setit++){
				stringstream ss;
				ss << (*setit);
				if((it+1) != evalresult->indices.end())
					ss << ",";
				finals.push_back(ss.str());
			}
		}
		else {
			for(auto finalsit = finals.begin(); finalsit != finals.end(); finalsit++) {
				for(auto setit = (*it).begin(); setit != (*it).end(); setit++){
					stringstream ss;
					ss << (*finalsit) << (*setit);
					if((it+1) != evalresult->indices.end())
						 ss << ",";
					tempresults.push_back(ss.str());
				}
			}
			finals.clear();
			finals = tempresults;
		}
	}

	if(finals.size() == 0)
		evalresult->nodenames.push_back(nodename);

	for(auto finalsit = finals.begin(); finalsit != finals.end(); finalsit++) {
		evalresult->nodenames.push_back(nodename + "[" + (*finalsit) + "]");
	}
}

/*
 MaterializedViewInput {Name, value, type}
	Input:  NNMEI { MaterializedViewInput , vector<MaterializedViewInput> }
	Output: NNMER
	Change: New Query for handleDimesionOfParentNodeForIndex. [Caching Strategy]
		Get all the tuples from them and handle them in a different way.
 */

NodeNameMatrixEvaluationResult* evaluateNNM(PGconn* conn, IData* data, NodeNameMatrixEvaluationInput &nnmei)
{
	NodeNameMatrixEvaluationResult* evalresult = new NodeNameMatrixEvaluationResult();
	int variabletype = nnmei.node.variabletype;

	if(variabletype == 1 || variabletype == 2) {//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
		evalresult->type = (variabletype == 2) ? UnivariateNode : MultivariateNode;
		string nodename = nnmei.node.name;

		if(nnmei.nodeindices.size() == 0 && variabletype == 1) //No Indices and Multivariate
			assert(false);
		else if(nnmei.nodeindices.size() == 0 && variabletype == 2); //No Indices and Univariate
		else
		    handleIndices(conn, nnmei, evalresult, data);

		createNodenamesWithIndices(nodename, evalresult);
	}
	else if(variabletype == 3) {
		evalresult->type = Constant;
		evalresult->value = nnmei.node.value;
	}
	else
		assert(false);

	return evalresult;
}
}
#endif
