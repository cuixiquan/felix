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

#ifndef JOINEVALUATOR_H_
#define JOINEVALUATOR_H_

#include "Bugs_Common.h"

//Pass by reference is very important

namespace hazy {

bool Bugs_AddNodes::tryAndFetchFromJoinCursor(int & currenttuple, int &notuplesincursor, Long noRowsToFetch, PGresult *& res)
{
	//Fetch Next noRowsToFetch
	if(res != NULL)
		PQclear(res);

	stringstream ss;
	ss << noRowsToFetch;
	res = executeSQLQuery(conn, "FETCH " + ss.str() + " from joincursor"); // Prepared Statement??
	char *result = PQcmdStatus(res); // FETCH count
	string fetchresult(result);
	cout << " fetch count " << atoi(fetchresult.substr(6, fetchresult.size()).c_str()) << endl;
	if(atoi(fetchresult.substr(6, fetchresult.size()).c_str()) == 0) // Break if fetch count = 0
		return false;

	currenttuple = 0;
	notuplesincursor = PQntuples(res);
	return true;
}

void Bugs_AddNodes::constructNNMEI(PGresult *& res, int &currenttuple, NodeNameMatrixEvaluationInput &nnmeitoconstruct,
		int &notuplesincursor, Long noRowsToFetch)
{
	Long n1id = atol(PQgetvalue(res, currenttuple, 8));
	nnmeitoconstruct.node.name = PQgetvalue(res, currenttuple, 9);
	nnmeitoconstruct.node.value = atof(PQgetvalue(res, currenttuple, 10));
	nnmeitoconstruct.node.variabletype = ::atoi(PQgetvalue(res, currenttuple, 11));
	string n1indexid = PQgetvalue(res, currenttuple, 12);
	vector<Long> indices;
	getIndexIds(indices, n1indexid);
	if(indices.size() != 0)
	{
		assert(indices[0] == ::atol(PQgetvalue(res, currenttuple, 13))); // Assert that n2id is same as that n1indexid for 0th index
		MaterializedViewInput matindex;
		matindex.name = PQgetvalue(res, currenttuple, 14);
		matindex.value = ::atol(PQgetvalue(res, currenttuple, 15));
		matindex.variabletype = ::atoi(PQgetvalue(res, currenttuple, 16));
		nnmeitoconstruct.nodeindices.push_back(matindex);
	}

	for(int index = 1; index < indices.size(); index++) // 0 index has already been processed
	{
		currenttuple++; // need next tuple from res
		if(currenttuple >= notuplesincursor) { // There is a half baked NodeInput
			if(!tryAndFetchFromJoinCursor(currenttuple, notuplesincursor, noRowsToFetch, res))
			{
				cout << "[BUGS Error] Ill constructed Join while processing NNM's indices. Please report this error with your model and data!" << endl;
				assert(false);
			}
		}

		// Assert that n2id is same as that of n1indexid for this index
		assert(indices[index] == ::atol(PQgetvalue(res, currenttuple, 13)));
		MaterializedViewInput matindex;
		matindex.name = PQgetvalue(res, currenttuple, 14);
		matindex.value = ::atol(PQgetvalue(res, currenttuple, 15));
		matindex.variabletype = ::atoi(PQgetvalue(res, currenttuple, 16));
		nnmeitoconstruct.nodeindices.push_back(matindex);
	}
}

/*
Join Evaluator: N depends on amount of memory available.
Input : Join cursor

while(1):
	Fetch N Rows from cursor.
	If cursor returns 0
		exit;

	Get Nodeid, nodeparamid, n1id, n1indexid.
	Populate NNMEI for node. If n1indexid is not empty, populate MaterializedViewInput of first index
	for numberofn1indexid - 1:
		Fetch Row from cursor
		Populate MaterializedViewInput for other indices

	============= NNMEI for Node =========
	for i in distparams
		Fetch Row from cursor [Get next row]
		assert n1id is nodeparams[i]
		Get n1id, n1indexid.
		Populate NNMEI for node. If n1indexid is not empty, populate MaterializedViewInput of first index
		for numberofn1indexid - 1:
			Fetch Row from cursor [Get next row]
			Populate MaterializedViewInput for other indices
	============= NNMEI for NodeParams =========
	Output: NodeInput {distname, e_l_id, limitationtype, lowerbound, upperbound, NNMEI for RV, vector<NNMEI> for nodeparams}
	Call StochasticNode or LogicalNode
 */
void Bugs_AddNodes::joinAndAddNodes(Long noRowsToFetch)
{
	cout << "******************** Begin Processing Nodes ****************** " << endl;
	PGresult *res;
	Timer timer;
	timer.restart();
	executeSQLCommand(conn, res, "BEGIN", "", "");
	executeSQLCommand(conn, res, "DECLARE joincursor CURSOR FOR select nodetype, nodeid, e_l_id, nodeparams, distname, "
			"type, lowerbound, upperbound, n1id, n1name, n1value, n1type, n1indexid, n2id, n2name, n2value, n2type "
			"from (node N join MaterializedNNM M on (N.nodeid = M.n1id) or (M.n1id = ANY(N.nodeparams))) "
			"left join limitation L on L.id = N.e_l_id order by n1id, n2id;", "Cannot Set Cursor for join", "");

	int currenttuple = 0;
	int notuplesincursor = 0;

	/*
	* This loop creates the NodeInput required for stochastic and logical nodes and calls them.
	* If at any point the the currenttuple exceeds the notuplesincursor, then again fetch is done
	*/

	while(true)
	{
		if(currenttuple >= notuplesincursor) { // indicates we have exploited all the fetched tuples and all there are no half baked NodeInputs.
			if(!tryAndFetchFromJoinCursor(currenttuple, notuplesincursor, noRowsToFetch, res))
				break;
		}

		NodeInput nodeinput;

		int nodetype = atoi(PQgetvalue(res, currenttuple, 0));
		Long nodeid = atol(PQgetvalue(res, currenttuple, 1));
		nodeinput.e_l_id = -1;
		if(nodetype == 2) // Logical Node
			nodeinput.e_l_id = atol(PQgetvalue(res, currenttuple, 2));

		string nodeparams = PQgetvalue(res, currenttuple, 3);
		vector<Long> nodeparamid;
		getIndexIds(nodeparamid, nodeparams);

		nodeinput.distname = PQgetvalue(res, currenttuple, 4);
		nodeinput.limitationtype = PQgetvalue(res, currenttuple, 5);
		nodeinput.lowerbound = PQgetvalue(res, currenttuple, 6);
		nodeinput.upperbound = PQgetvalue(res, currenttuple, 7);

		//populate Random Variable. Attention!! the code below modifies the currenttuple.
		constructNNMEI(res, currenttuple, nodeinput.nnmeiRV, notuplesincursor, noRowsToFetch);

		//Populate Nodeparams
		int nodeparamindexno = 0;
		for(auto it = nodeparamid.begin(); it != nodeparamid.end(); it++, nodeparamindexno++)
		{
			currenttuple++; // need next tuple from res
			if(currenttuple >= notuplesincursor) { // There is a half baked NodeInput
				if(!tryAndFetchFromJoinCursor(currenttuple, notuplesincursor, noRowsToFetch, res))
				{
					cout << "[BUGS Error] Ill constructed Join while processing Nodeparams. Please report this error with your model and data!" << endl;
					assert(false);
				}
			}
			assert((*it) == atol(PQgetvalue(res, currenttuple, 8))); // Assert that the current tuple has the same n1id as nodeparamid
			nodeinput.nnmeiNodeParams.push_back(NodeNameMatrixEvaluationInput()); // creating the current nnmeinodeparam
			constructNNMEI(res, currenttuple, nodeinput.nnmeiNodeParams[nodeparamindexno], notuplesincursor, noRowsToFetch);
		}

		//Process current node
		if(nodetype == 1)
			addStochasticNode(nodeinput);
		else
			addLogicalNodes(nodeinput);

		currenttuple++; // Process Next Node
	}

	PQclear(res);
	executeSQLCommand(conn, res, "CLOSE joincursor", "" , "");
	executeSQLCommand(conn, res, "END", "", "");
	PQclear(res);

	cout << "Time to process nodes " << timer.elapsed() << endl;
	cout << "******************** End Processing Nodes ****************** " << endl;
}
}
#endif /* JOINEVALUATOR_H_ */
