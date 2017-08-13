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

#ifndef ADDLOGICALNODE_H_
#define ADDLOGICALNODE_H_

#include <map>
#include <string>
#include "Bugs_Common.h"

#define LOGICAL_LB -10000
#define LOGICAL_UB 10000

using namespace std;

/*
 * Algorithm for handling LogicalNodeExpression Metadata
 * 1. Add GlobalVariableids into a vector and store in Metadata O(n)
 * 2. Before calling inference, go through all the inmemoryLNE and do the following O(n)
 * 3. Get the actual vids corresponding to globalvids and create a new temp vector O(n)
 * 4. Sort and eliminate duplicates in a copy of the temp vector. Use set O(nlogn)
 * 5. Hash the set. O(n)
 * 6. Go through the temp vector and create inputparamloc O(n)
 */

namespace hazy {

void Bugs_AddNodes::handleLogicalNodeConstantExpression(NodeInput &nodeinput, int expressionid)
{
	//evaluate the result here
	NodeNameMatrixEvaluationResult *rvevalresult = evaluateNNM(conn, data, nodeinput.nnmeiRV);
	assert(rvevalresult->nodenames.size() == 1);
	assert(rvevalresult->indexnodenames.size() == 0);

	Long gvid = data->fgfiledetails->handletvariables(rvevalresult->nodenames[0]);

	double evalparams[1] = {0};
	inmemorylogicalnodes[expressionid]->metadata->numberofnodes = 0;
	inmemorylogicalnodes[expressionid]->evaluated = true;
	double constant = inmemorylogicalnodes[expressionid]->logicalnodeexpression->eval(evalparams, inmemorylogicalnodes[expressionid]->metadata);

//	data->fgfiledetails->handleGroup(factorid, 0, SINGLETONEVID);
	data->fgfiledetails->handleConstantVariable(gvid, EVID, REAL, constant);
//	data->fgfiledetails->handleEdge(factorid, gvid, 0, -1, true);
//	data->fgfiledetails->handleFactor(factorid, expressionid, FACTOR_GENERAL, "LOGICAL", 1);
	delete rvevalresult;
}

/*
 * If inmemoryLNE is notevaluated
 * 	If nodename is already found in map, then add the location to inputlocationparams
 * 	else if not found, add to map, add to inputlocationparams, increment number of nodes, insert into tvariables,  add edge
 * Else if inmemoryLNE is evaluated
 * 	If nodename is already found in map, do nothing [Everyone is happy :)]
 * 	else if not found, add to map, insert into tvariables, add edge
 *
 */
void Bugs_AddNodes::manipulateMapAndMetadata(int & expressionid, map<string,Long> &expressionnodenames, string variable)
{
	if(!inmemorylogicalnodes[expressionid]->evaluated){
		if(expressionnodenames.find(variable) != expressionnodenames.end()){ // if found
			inmemorylogicalnodes[expressionid]->metadata->inputlocationparams.push_back(expressionnodenames[variable]);
		}else{
			Long gvid = data->fgfiledetails->handletvariables(variable);
			expressionnodenames[variable] = gvid;
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, false);
			inmemorylogicalnodes[expressionid]->metadata->inputlocationparams.push_back(gvid);
			inmemorylogicalnodes[expressionid]->metadata->numberofnodes++;
		}
	}
	else{
		if(expressionnodenames.find(variable) == expressionnodenames.end()){ // if not found
			Long gvid = data->fgfiledetails->handletvariables(variable);
			expressionnodenames[variable] = gvid;
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, false);
		}
	}
}

//TODO duplicate addition of constant here ??
void Bugs_AddNodes::handleLogicalNodeConstantValueInExpression(double value, int & expressionid, map<string,Long> & expressionnodenames)
{
	stringstream constantvariable;
	constantvariable << value;
	manipulateMapAndMetadata(expressionid, expressionnodenames, constantvariable.str());
	Long gvid = data->fgfiledetails->handletvariables(constantvariable.str());
	data->fgfiledetails->handleConstantVariable(gvid, EVID, REAL, value);
}

void Bugs_AddNodes::handleLogicalNodeParameter(NodeNameMatrixEvaluationResult *& expressionnodes, int & expressionid,
		map<string,Long> & expressionnodenames)
{
	if(expressionnodes->type == UnivariateNode)
		assert(expressionnodes->nodenames.size() == 1);
	else if(expressionnodes->type == MultivariateNode)
	{
		if(!inmemorylogicalnodes[expressionid]->evaluated)
			inmemorylogicalnodes[expressionid]->metadata->vectorfunctionnoofnodes.push_back(expressionnodes->nodenames.size());
	}

	for(auto nodenameit = expressionnodes->nodenames.begin();nodenameit != expressionnodes->nodenames.end();nodenameit++)
	{
		string barenodename = "";
		vector<int> indices;
		getBareNodeNameAndIndices((*nodenameit), barenodename, indices);
		if(data->contains(barenodename, indices)){
			double value = data->getData(barenodename, indices);
			handleLogicalNodeConstantValueInExpression(value, expressionid, expressionnodenames);
		}else{
			manipulateMapAndMetadata(expressionid, expressionnodenames, (*nodenameit));
		}
	}
}

void Bugs_AddNodes::handleLogicalRandomVariable(NodeInput &nodeinput)
{
    /* Evaluate RV
	 * Check for data.
	 * Check for inits.
	 */

	NodeNameMatrixEvaluationResult *rvevalresult = evaluateNNM(conn, data, nodeinput.nnmeiRV);
	assert(rvevalresult->indexnodenames.size() == 0);
	assert(rvevalresult->type != Constant);

	// Add Group
	data->fgfiledetails->handleGroup(factorid, groupid, SINGLETON);

	/*
	 * For all nodenames, check if present in data. If yes, handle
	 * Else, handle bounds, add variable, bounds, edge
	 */
	for(auto nodenameit = rvevalresult->nodenames.begin();nodenameit != rvevalresult->nodenames.end();nodenameit++)
	{
		string barenodename = "";
		vector<int> indices;
		getBareNodeNameAndIndices((*nodenameit), barenodename, indices);

		if(data->contains(barenodename, indices)){
			stringstream constantvariable;
			double value = data->getData(barenodename, indices);
			constantvariable << value;

			Long gvid = data->fgfiledetails->handletvariables(constantvariable.str());
			data->fgfiledetails->handleConstantVariable(gvid, EVID, REAL, value);
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, true);
		}
		else{
			double init = getInit((*nodenameit));

			Long gvid = data->fgfiledetails->handletvariables((*nodenameit));
			data->fgfiledetails->handleNodeVariable(gvid, QUERY, REAL, init, LOGICAL_LB, LOGICAL_UB);
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, true);
			data->fgfiledetails->handleBounds(gvid, LOGICAL_LB, LOGICAL_UB, -1, -1); //??
		}
	}
	delete rvevalresult;
}

void Bugs_AddNodes::handleLogicalNodeWithNodeIndex(NodeNameMatrixEvaluationResult *& expressionnodes, int & expressionid)
{
	// Handle Node (s) with index parameter
	/*
	 * Protocol:
	 * Variable's aux will indicate that it is a NESTED_INDEX_ARRAY. This is the number of indices [aux = 0]
	 * Since the variableids will not be in order, we are using aux to indicate the order of the remaining variables [with aux > 0]
	 * The order in which variables are added are :
	 * Number of Indices[aux = 0]. size of the vector<set>.
	 * Index 1, Index 2 ,.....
	 * Dimensions of indices
	 * Variable 1, Variable 2, ......
	 *
	 * The inmemorylogicalnodes are indicated that there is a Nested Index (by setting a -1 as the variablename).
	 * The nested index's values are sent as the last value into the variable's array for evaluation
	 */

	// Indicating that there is a Nested Index.
	if(!inmemorylogicalnodes[expressionid]->evaluated)
	{
		inmemorylogicalnodes[expressionid]->metadata->inputlocationparams.push_back(-1);
		inmemorylogicalnodes[expressionid]->metadata->numberofnodes++;
	}

	// add number of indices
	int aux = 0;
	assert(expressionnodes->indices.size() > 0);
	handleConstantEvidVariable(expressionnodes->indices.size(), aux++);

	// add Indices.
	for(int ind = 0;ind < expressionnodes->indices.size();ind++)
	{
		if(expressionnodes->indexnodenames.find(ind) != expressionnodes->indexnodenames.end()){
			Long gvid = data->fgfiledetails->handletvariables(expressionnodes->indexnodenames[ind]);
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, aux++, false);
		}else{
			handleConstantEvidVariable(1, aux++);
		}
	}

	// Adding dimensions of indices.
	for(vector<set<int> >::iterator uit = expressionnodes->indices.begin();uit != expressionnodes->indices.end();uit++)
		handleConstantEvidVariable(uit->size(), aux++);

	// all variables.
	for(auto nodenameit = expressionnodes->nodenames.begin();nodenameit != expressionnodes->nodenames.end();nodenameit++)
	{
		string barenodename = "";
		vector<int> indices;
		getBareNodeNameAndIndices((*nodenameit), barenodename, indices);
		if(data->contains(barenodename, indices))
			handleConstantEvidVariable(data->getData(barenodename, indices), aux++);
		else
		{
			Long gvid = data->fgfiledetails->handletvariables((*nodenameit));
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, aux++, false);
		}
	}
}

void Bugs_AddNodes::addLogicalNodes(NodeInput &nodeinput)
{
	factorid = data->fgfiledetails->globalfactorid;
	int expressionid = nodeinput.e_l_id;

	/*
	 * Check if Metadata for the logical node expression has been created. If not, handle that too.
	 * The following data structures are used for filling the metadata.
	 */
	map<string, Long> expressionnodenames; // nodename, Globaltvids

	assert(inmemorylogicalnodes.find(expressionid) != inmemorylogicalnodes.end());

	if(nodeinput.nnmeiNodeParams.size() == 0) { // Logical Node is just an assignment
		handleLogicalNodeConstantExpression(nodeinput, expressionid);
	}
	else
	{
		bool hasNestedIndexNodeParams = false;
		groupid = 0;
		for(auto it = nodeinput.nnmeiNodeParams.begin();it != nodeinput.nnmeiNodeParams.end();it++)
		{
			NodeNameMatrixEvaluationResult *expressionnodes = evaluateNNM(conn, data, (*it));
			if(expressionnodes->type == Constant) {
				double value = expressionnodes->value;
				handleLogicalNodeConstantValueInExpression(value, expressionid, expressionnodenames);
			}
			else if(expressionnodes->indexnodenames.empty())
			{
				handleLogicalNodeParameter(expressionnodes, expressionid, expressionnodenames);
			}
			else
			{
				/*
				 * Handling only one Nested Index in Logical Nodes.
				 * Also, the nested index cannot be a list of variables.
				 * TODO:: Need to extend the implementation for these two scenarios!
				 */
				if(!hasNestedIndexNodeParams) {
					hasNestedIndexNodeParams = true;
					handleLogicalNodeWithNodeIndex(expressionnodes, expressionid);
				}
				else {
					cout << "[BUGSError!!] We as of now handle only one Nested Index in Logical Nodes. If you really want this feature, "
							"please send a note to the developer " << endl;

					assert(false);
				}
			}
			delete expressionnodes;
		}

		data->fgfiledetails->handleGroup(factorid, groupid, SINGLETON);
		inmemorylogicalnodes[expressionid]->evaluated = true;

		++groupid;
		// Add Random Variable. Group id would have been incremented
		handleLogicalRandomVariable(nodeinput);

		// Add Factor
		data->fgfiledetails->handleFactor(factorid, expressionid, FACTOR_GENERAL, "LOGICAL", groupid + 1);
	}
}

}

#endif /* ADDLOGICALNODE_H_ */
