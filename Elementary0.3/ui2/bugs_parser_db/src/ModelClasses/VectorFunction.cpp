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

#include "VectorFunction.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <cfloat>
#include <cmath>
using namespace std;

VectorFunction::VectorFunction() {
	this->isunaryminus = false;
}

VectorFunction::~VectorFunction() {
	delete this->mvnode;
	if(this->name == "INPROD")
		delete this->mvnode2;
}

string VectorFunction::toString(){
	string retval = " ";
	retval = retval + "Name = " + this->name + " Parameter= " + this->mvnode->toString();
	if(this->name == "INPROD")
		retval = retval + " Parameter = " + this->mvnode2->toString();
	return retval;
}

/*
 * TODO Should we return 0 when result is Nan or -10 since it is in hazy::LOG scale
 */
inline double VectorFunction::getReturnValue(double result) {
	if (result != result) // happens when result is Nan
		return 0;
	else if (result <= -DBL_MAX)
		return -DBL_MAX;
	else if (result >= DBL_MAX)
		return DBL_MAX;

	if(this->isunaryminus)
		result = -1 * result;

	return result;
}

void VectorFunction::createNodeOperands(vector<NodeNameMatrix*> &nodeoperands, IData* data) {

	if(this->name == "MEAN" || this->name == "SD" || this->name == "SUM" || this->name == "INVERSE"){
		NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
		nodenamematrix->nodename = this->mvnode->getNodeid(data);
		nodenamematrix->type = NNM_MULTIVARIATE;
		mvnode->createIndices(nodenamematrix->indices, data);
		nodeoperands.push_back(nodenamematrix);
	}
	else if(this->name == "INPROD"){
		NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
		nodenamematrix->nodename = this->mvnode->getNodeid(data);
		nodenamematrix->type = NNM_MULTIVARIATE;
		mvnode->createIndices(nodenamematrix->indices, data);
		nodeoperands.push_back(nodenamematrix);

		NodeNameMatrix* nodenamematrix2 = new NodeNameMatrix();
		nodenamematrix2->nodename = this->mvnode2->getNodeid(data);
		nodenamematrix2->type = NNM_MULTIVARIATE;
		mvnode->createIndices(nodenamematrix2->indices, data);
		nodeoperands.push_back(nodenamematrix2);
	}
	else
	{
		cout << "Attention!!! [BUGSError] Vector Function " << this->name << " is not yet supported. " <<endl;
		assert(false);
	}
}

double VectorFunction::internaleval(vector<double> &inputparams, LogicalNodeExpressionMetadata* metadata, int & currentcounter,
		int &vectorfncounter){

	if(this->name == "MEAN"){
		int n = 0;
		double sum = 0.0;

		assert(vectorfncounter < metadata->vectorfunctionnoofnodes.size());
		for(int i=0; i < metadata->vectorfunctionnoofnodes[vectorfncounter]; i++, n++)
		{
			assert(currentcounter < metadata->inputlocationparams.size());
			sum = sum + inputparams[metadata->inputlocationparams[currentcounter++]];
		}

		vectorfncounter++;
		return getReturnValue(sum/n);
	}
	else if(this->name == "SD"){
		int n = 0;
		double sum = 0.0;
		vector<double> params;

		assert(vectorfncounter < metadata->vectorfunctionnoofnodes.size());
		int numberofnodes = metadata->vectorfunctionnoofnodes[vectorfncounter];
		for(int i=0; i < numberofnodes; i++, n++)
		{
			assert(currentcounter < metadata->inputlocationparams.size());
			params.push_back(inputparams[metadata->inputlocationparams[currentcounter]]);
			sum = sum + inputparams[metadata->inputlocationparams[currentcounter]];
			currentcounter++;
		}

		double mean = sum/n;
		double numvariance = 0.0;

		for(int i=0; i < numberofnodes; i++)
			numvariance = numvariance + (params[i] - mean) * (params[i] - mean);

		vectorfncounter++;
		return getReturnValue(sqrt(numvariance/(n - 1)));
	}
	else if(this->name == "SUM"){
		double sum = 0.0;

		assert(vectorfncounter < metadata->vectorfunctionnoofnodes.size());
		int numberofnodes = metadata->vectorfunctionnoofnodes[vectorfncounter];

		for(int i=0; i < numberofnodes; i++)
			sum = sum + inputparams[metadata->inputlocationparams[currentcounter++]];

		vectorfncounter++;
		return getReturnValue(sum);
	}
	else if(this->name == "INPROD"){
		assert(vectorfncounter < metadata->vectorfunctionnoofnodes.size());
		int numberofnodes1 = metadata->vectorfunctionnoofnodes[vectorfncounter++];
		int numberofnodes2 = metadata->vectorfunctionnoofnodes[vectorfncounter++];

		assert(numberofnodes1 == numberofnodes2);

		double sum = 0.0;

		vector<double> params1;
		vector<double> params2;

		for(int i=0; i < numberofnodes1; i++) {
			assert(currentcounter < metadata->inputlocationparams.size());
			params1.push_back(inputparams[metadata->inputlocationparams[currentcounter++]]);
		}

		for(int i=0; i < numberofnodes2; i++) {
			assert(currentcounter < metadata->inputlocationparams.size());
			params2.push_back(inputparams[metadata->inputlocationparams[currentcounter++]]);
		}

		for(int i=0; i < params1.size(); i++)
			sum = sum + params1[i] * params2[i];

		return getReturnValue(sum);
	}
	else if(this->name == "INVERSE"){
		cout << "Attention!!! [BUGSError] INVERSE FUNCTION IS CURRENTLY HANDLED BY A NEW POTENTIAL FUNCTION " <<endl;
		assert(false);
	}
	else {
		cout << "Attention!!! [BUGSError] Function " << this->name << " is not yet implemented." << endl;
		assert(false);
	}
	return 0;
}

