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

#ifndef MVDPARAMETERS_HPP_
#define MVDPARAMETERS_HPP_
#include "MultivariateNode.hpp"
#include "Expression.hpp"
#include "../ElementaryAdapter/NodeNameMatrix.hpp"
enum MVDParamType {MVDNODE, MVDEXPRESSION};

class MVDParameters {
public:
	MVDParameters();
	virtual ~MVDParameters();
	MultivariateNode* mvnode;
	Expression* exp;
	MVDParamType type;

	void evalDistParams(vector<NodeNameMatrix*>& distparams, IData* data);
};


#endif /* MVDPARAMETERS_HPP_ */
