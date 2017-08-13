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

#include "MVDParameters.hpp"
#include "UnivariateNode.hpp"
#include <iostream>
#include <cassert>

using namespace std;

MVDParameters::MVDParameters() {
	// TODO Auto-generated constructor stub

}

MVDParameters::~MVDParameters() {
	if(this->type == MVDNODE)
		delete this->mvnode;
	else
		delete this->exp;
}

/*
 * TODO Handle MVNode with indices as another node which does not have data
 */
void MVDParameters::evalDistParams(vector<NodeNameMatrix*> &distparams, IData* data){
	if(this->type == MVDEXPRESSION)
	{
		NodeNameMatrix* nodenamematrix = this->exp->evalDistParams(data);
		distparams.push_back(nodenamematrix);
	}
	else {
		NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
		nodenamematrix->nodename = this->mvnode->getNodeid(data);
		nodenamematrix->type = NNM_MULTIVARIATE;
		this->mvnode->createIndices(nodenamematrix->indices, data);
		distparams.push_back(nodenamematrix);
	}
}
