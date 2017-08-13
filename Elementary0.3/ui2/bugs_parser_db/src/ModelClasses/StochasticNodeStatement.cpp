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

#include "StochasticNodeStatement.hpp"
#include <iostream>
#include <typeinfo>
#include <cassert>
#include <sstream>
#include <algorithm>
using namespace std;

StochasticNodeStatement::StochasticNodeStatement() {
	// TODO Auto-generated constructor stub
}

StochasticNodeStatement::~StochasticNodeStatement() {
	if((typeid(*this->node) == typeid(UnivariateNode)) && this->limitationType != NOLIMITATION)
		delete this->limitation;

	delete this->node;
	delete this->distribution;
}

string StochasticNodeStatement::toString(){
	string retval = "*StochasticNode : ";
	retval = retval + "Node = " + this->node->toString();
	if((typeid(*this->node) == typeid(UnivariateNode)) && this->limitationType == CENSOR){
		retval = retval + " CENSOR ";
		retval = retval + this->limitation->toString();
	}
	else if((typeid(*this->node) == typeid(UnivariateNode)) && this->limitationType == TRUNCATION){
		retval = retval + " TRUNCATION ";
		retval = retval + this->limitation->toString();
	}
	retval = retval + "\n Distribution= " + this->distribution->toString();
	return retval + "\n";
}

void StochasticNodeStatement::handleUnivariateNode(IData *data)
{
    UnivariateNode* uvnode = (UnivariateNode*)((this->node));
    StochasticNode *stochasticNode = new StochasticNode();

    // handle node
    stochasticNode->nodenamematrix = uvnode->evalAsNodeNameMatrix(data);

    //handle censor & truncation
    if(this->limitationType == NOLIMITATION)
        stochasticNode->limitation->limitationType = S_NOLIMITATION;
    else if(this->limitationType == CENSOR) {
		stochasticNode->limitation->limitationType = S_CENSOR;
		if(this->limitation->optional != LOWER){
			stochasticNode->limitation->optional = S_UPPER;
			stochasticNode->limitation->lowerbound = this->limitation->findlowerbound(data);
		}
		if(this->limitation->optional != UPPER) {
			stochasticNode->limitation->optional = (stochasticNode->limitation->optional == S_UPPER) ? S_NOOPTIONAL : S_LOWER;
			stochasticNode->limitation->upperbound = this->limitation->findupperbound(data);
		}
	}
    else if(this->limitationType == TRUNCATION)
    {
    	cout << " Attention!!! [BUGSError] Truncation is not yet supported " << endl;
    	assert(false);
    }

    // handle distribution
    stochasticNode->distributionname = this->distribution->name;
    this->distribution->evaluateParameters(stochasticNode->distributionparameters, data);

    // serialize into file
    stochasticNode->serialize(data->parserfiledetails);
    delete stochasticNode;
}

void StochasticNodeStatement::handleMultivariateNode(IData *data)
{
	MultivariateNode* mvnode = (MultivariateNode*)((this->node));
	StochasticNode *stochasticNode = new StochasticNode();

	// handle Node
   	stochasticNode->nodenamematrix->nodename = mvnode->getNodeid(data);
   	stochasticNode->distributionname = this->distribution->name;
    stochasticNode->nodenamematrix->type = stochasticNode->distributionname == "CATEGORICAL" ? NNM_UNIVARIATE : NNM_MULTIVARIATE;

    // handle indices
    mvnode->createIndices(stochasticNode->nodenamematrix->indices, data);

    //handle distribution
	this->distribution->evaluateParameters(stochasticNode->distributionparameters, data);

	//serialize
	stochasticNode->serialize(data->parserfiledetails);
	delete stochasticNode;
}

void StochasticNodeStatement::eval(IData* data){
	if(typeid(*this->node) == typeid(UnivariateNode))
	    handleUnivariateNode(data);
	else
		handleMultivariateNode(data);
}
