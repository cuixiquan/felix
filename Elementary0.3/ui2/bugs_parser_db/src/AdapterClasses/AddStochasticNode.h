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

#ifndef ADDSTOCHASTICNODE_H_
#define ADDSTOCHASTICNODE_H_

#include "Bugs_Common.h"
/*
 * TODO:
 * 1. Optimizations are not handled.
 * 2. Handle Bounds for Gamma with very low values properly
 * 3. Bounds if Variable is a Logical Node will not be handled correctly. Eg: A ~ dbin(X,Y) and Y <- F+G. Then A gets bounds of Y, which is wrong
 */
namespace hazy {

void Bugs_AddNodes::handleStochasticConstantDistributionParameter(double value, Bounds & bounds, int indexid)
{
	if(bounds.ubparam == indexid)
		bounds.upperbound = value;

	else if(bounds.lbparam == indexid)
		bounds.lowerbound = value;

	bounds.distributionparametervalues[indexid] = value;
	handleConstantEvidVariable(value, -1);
}

void Bugs_AddNodes::handleStochasticDistributionNodeParameter(NodeNameMatrixEvaluationResult *& evalresult, Bounds & bounds, int indexid)
{
	// Handle Node (s) distribution parameter
	for(auto nodenameit = evalresult->nodenames.begin();nodenameit != evalresult->nodenames.end();nodenameit++){
	/*
	* Variable is added into file only if it is a constant.
	* Else, only tvariables array and edge are populated.
	* This is based on the assumption that a variable name can appear only in distribution parameter only if it a constant
	*/

	string barenodename = "";
	vector<int> indices;
	getBareNodeNameAndIndices((*nodenameit), barenodename, indices);
	if(data->contains(barenodename, indices))
	{
		addGroupToFile(evalresult);
		handleStochasticConstantDistributionParameter(data->getData(barenodename, indices), bounds, indexid);
	}
	else
	{
		addGroupToFile(evalresult);
		Long gvid = data->fgfiledetails->handletvariables((*nodenameit));
		data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, false);

		if(bounds.ubparam == indexid)
			bounds.ubvarname = (*nodenameit);

		else if(bounds.lbparam == indexid)
			bounds.lbvarname = (*nodenameit);
	}
	}
}

void Bugs_AddNodes::addGroupToFile(NodeNameMatrixEvaluationResult *& distparamevalresult)
{
	++groupid;
	double aux = SINGLETON;
	if(distparamevalresult->nodenames.size() > 1 && distparamevalresult->indexnodenames.size() == 0)
		aux = LISTOFVARIABLE; // Multivariate Variable

	else if(distparamevalresult->indexnodenames.size() > 0) // Node Index
		aux = NESTED_INDEX_ARRAY;

	data->fgfiledetails->handleGroup(factorid, groupid, aux);
}

// returns whether a vg_function can handle this factor
void Bugs_AddNodes::handleStochasticRVBounds(NodeInput & nodeinput, double & rvlowerbound, double & rvupperbound, ParameterType varptype, Bounds & bounds, string & nodename)
{
	if(varptype == ZEROTOONE || varptype == ZEROTON || varptype == ZEROTOINF)
		rvlowerbound = 0;

	else if(varptype == MINUSINFTOINF)
		rvlowerbound = distributionMap[factorname].lowerbound;

	if(varptype == ZEROTOONE)
		rvupperbound = 1;

	else if(varptype == ZEROTOINF || varptype == MINUSINFTOINF || varptype == NTOINF)
		rvupperbound = distributionMap[factorname].upperbound;

	if(nodeinput.limitationtype != "")
	{
		if(nodeinput.lowerbound != "")
			rvlowerbound = atof(nodeinput.lowerbound.c_str());

		if(nodeinput.upperbound != "")
			rvupperbound = atof(nodeinput.upperbound.c_str());
	}

	rvupperbound = std::min(rvupperbound, bounds.upperbound);
	rvlowerbound = std::max(rvlowerbound, bounds.lowerbound);

	Long gvid = data->fgfiledetails->handletvariables(nodename);
	Long lgvid = -1, ugvid = -1;

	// 95% interval for normal
	if(factorname == "NORMAL" and bounds.distributionparametervalues[0] != -DBL_MAX and bounds.distributionparametervalues[1] != -DBL_MAX)
	{
		double sigma = 1/sqrt(bounds.distributionparametervalues[1]);
		rvlowerbound = bounds.distributionparametervalues[0] - 3 * sigma;
		rvupperbound = bounds.distributionparametervalues[0] + 3 * sigma;
	}
	else if(factorname == "GAMMA" and bounds.distributionparametervalues[0] != -DBL_MAX)
	{
		//setting lower bound to a parameter very close to 0 as Gamma has infinite prob at 0
		if(bounds.distributionparametervalues[0] < GAMMA_LOWER_BOUND)
			rvlowerbound = bounds.distributionparametervalues[0];
		else
			rvlowerbound = GAMMA_LOWER_BOUND;
	}

	if(bounds.lbvarname != "")
		lgvid = data->fgfiledetails->handletvariables(bounds.lbvarname);
	if(bounds.ubvarname != "")
		ugvid = data->fgfiledetails->handletvariables(bounds.ubvarname);

	data->fgfiledetails->handleBounds(gvid, rvlowerbound, rvupperbound, lgvid, ugvid);
}

void Bugs_AddNodes::handleStochasticRandomVariable(NodeInput &nodeinput, Bounds &bounds,  ParameterType varptype)
{
	/* Evaluate RV
	 * Check for data.
	 * Check for inits.
	 * Modify LB/UB for censor and truncation
	 * Look up distparammap and add Random Variable
	 * Add Bounds
	 */

	NodeNameMatrixEvaluationResult *rvevalresult = evaluateNNM(conn, data, nodeinput.nnmeiRV);
	assert(rvevalresult->indexnodenames.size() == 0);
    assert(rvevalresult->type != Constant);

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
			addGroupToFile(rvevalresult);
			handleConstantEvidVariable(data->getData(barenodename, indices), -1);
		}
		else{
			addGroupToFile(rvevalresult);
			Long gvid = data->fgfiledetails->handletvariables((*nodenameit));
			double init = getInit((*nodenameit));

			//Handling Bounds -> Censor/Truncation and Bounds found from Distribution Parameter
			double rvupperbound = DBL_MAX, rvlowerbound = -DBL_MAX;
			handleStochasticRVBounds(nodeinput, rvlowerbound, rvupperbound, varptype, bounds, (*nodenameit));

			data->fgfiledetails->handleNodeVariable(gvid, QUERY, distributionMap[factorname].variabledtype,
					init, rvlowerbound, rvupperbound);
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, -1, false);
		}
	}

    delete rvevalresult;
}

void Bugs_AddNodes::handleBoundsForCategoricalDistribution(Bounds & bounds, int nvars)
{
	if(factorname == "CATEGORICAL"){
		bounds.lowerbound = 1;
		bounds.upperbound = nvars;
	}
}

void Bugs_AddNodes::handleStochasticNodeWithNodeIndexDistributionParameter(NodeNameMatrixEvaluationResult *& distparamevalresult, Bounds & bounds)
{
	// Handle Node (s) with index distribution parameter
	/*
	 * Since the variableids will not be in order, we are using aux to indicate the order of the variables.
	 * The order in which variables are added are :
	 *
	 * First group will contain:
	 * Number of Indices. size of the vector<set>.
	 * Dimensions of indices
	 *
	 * Every index will be in a separate group.
	 * This is because, we need to handle if the index's group becomes a Logical Node Expression. [Eg: Asia]
	 *  Index 1, Index 2 ,..... (Only those indices that are stochastic. Remaining indices will be 1).
	 *
	 * Every Variable will be in a separate group.
	 * Variable 1, Variable 2, ......
	 * This is because, we need to handle if the variable's group becomes a Logical Node Expression. [Eg: Hearts]
	 */

	// add number of indices
	int aux = 0;
	assert(distparamevalresult->indices.size() > 0);

	addGroupToFile(distparamevalresult);

	handleConstantEvidVariable(distparamevalresult->indices.size(), aux++);
	// Adding dimensions of indices.
	for(vector<set<int> >::iterator uit = distparamevalresult->indices.begin();uit != distparamevalresult->indices.end();uit++)
		handleConstantEvidVariable(uit->size(), aux++);

	// add Indices. Each Index goes into a separate group, since it can be a Logical Node and will get replaced later by the stored proc
	for(int ind = 0;ind < distparamevalresult->indices.size();ind++)
	{
		addGroupToFile(distparamevalresult);

		if(distparamevalresult->indexnodenames.find(ind) != distparamevalresult->indexnodenames.end()){
			Long gvid = data->fgfiledetails->handletvariables(distparamevalresult->indexnodenames[ind]);
			data->fgfiledetails->handleEdge(factorid, gvid, groupid, aux++, false);
		}else{
			/*
			 * Handling the bounds of the categorical distribution as the size of the indices which does not contain a nodename.
			 * Based on the assumption that only one index in dcat will have this property.
			 * TODO Check this implementation.
			 */
			if(distparamevalresult->indices[ind].size() > 1)
				handleBoundsForCategoricalDistribution(bounds, distparamevalresult->indices[ind].size());

			handleConstantEvidVariable(1, aux++);
		}
	}

	// all variables.
	for(auto nodenameit = distparamevalresult->nodenames.begin();nodenameit != distparamevalresult->nodenames.end();nodenameit++)
	{
		addGroupToFile(distparamevalresult);

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

void Bugs_AddNodes::addStochasticNode(NodeInput & nodeinput)
{
	factorname = nodeinput.distname;
	factorid = data->fgfiledetails->globalfactorid;
	// Handling bounds of the RV.
	Bounds bounds;
	ParameterType varptype = distributionMap[factorname].variableptype;
	if(varptype == ZEROTON || varptype == NTON)
		bounds.ubparam = distributionMap[factorname].upperboundparameter;

	else
		if(varptype == NTOINF || varptype == NTON)
			bounds.lbparam = distributionMap[factorname].lowerboundparameter;


	// Add Distribution Parameters [Into tvariables and/or into variable table]
	/*
	 * groups are handled individually for each distribution parameter.
	 * If the distparam contains multiple variables, each of them is put in its own group as it might get
	 * replaced by a Logical Node Expression.
	 *
	 * When adding group to file, the group id is incremented.
	 */
	groupid = -1;
	int indexid = 0;
	for(auto it = nodeinput.nnmeiNodeParams.begin();it != nodeinput.nnmeiNodeParams.end();it++, ++indexid){
		NodeNameMatrixEvaluationResult *distparamevalresult = evaluateNNM(conn, data, (*it));
		if(distparamevalresult->type == Constant){
			// Handle Constant Distribution Parameter
			addGroupToFile(distparamevalresult);
			handleStochasticConstantDistributionParameter(distparamevalresult->value, bounds, indexid);
		}else
			if(distparamevalresult->indexnodenames.empty()){
				// Handle Node (s) distribution parameter
				handleStochasticDistributionNodeParameter(distparamevalresult, bounds, indexid);
				handleBoundsForCategoricalDistribution(bounds, distparamevalresult->nodenames.size());
			}else{
				// Handle Node (s) with index distribution parameter. The following function will change group ids
				handleStochasticNodeWithNodeIndexDistributionParameter(distparamevalresult, bounds);
			}

		delete distparamevalresult;
	}

	// Add Random Variable
	handleStochasticRandomVariable(nodeinput, bounds, varptype);

	//Add Factor
	data->fgfiledetails->handleFactor(factorid, -1, FACTOR_GENERAL, factorname, groupid + 1);
}
}

#endif /* ADDSTOCHASTICNODE_CPP_ */
