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

#include "MultivariateDistribution.hpp"
#include <iostream>
using namespace std;

MultivariateDistribution::MultivariateDistribution() {
	// TODO Auto-generated constructor stub

}

MultivariateDistribution::~MultivariateDistribution() {
	for(vector<MVDParameters*>::iterator it=this->distributionParameters.begin(); it!=this->distributionParameters.end(); ++it)
		delete (*it);
}

string MultivariateDistribution::toString() {
	string retval = " ";
	retval += this->name + " ";
	for(vector<MVDParameters*>::iterator it=this->distributionParameters.begin(); it!=this->distributionParameters.end(); ++it){
		if((*it)->type == MVDNODE)
			retval = retval + (*it)->mvnode->toString() ;
		else
			retval = retval + (*it)->exp->toString();
	}
	return retval;
}

void MultivariateDistribution::evaluateParameters(vector<NodeNameMatrix* > &distparams, IData* data){
	for(auto it=this->distributionParameters.begin(); it!=this->distributionParameters.end(); ++it)
		(*it)->evalDistParams(distparams, data); /* Pass by reference here */
}

