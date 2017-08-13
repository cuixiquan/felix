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

#ifndef NODE_HPP_
#define NODE_HPP_
using namespace std;

#include <vector>
#include <string>
#include "Expression.hpp"
#include "INode.hpp"
#include "../ElementaryAdapter/StochasticNode.hpp"

class UnivariateNode : public INode {
public:
	UnivariateNode();
	UnivariateNode(UnivariateNode* uvnode);

	virtual ~UnivariateNode();
	string nodeid;
	vector<Expression* > indices;

	string getNodeid(IData* data);
	double evalAsData(IData* data);
	int evalAsIndex(IData* data);
	bool canEvaluate(IData* data);

	NodeNameMatrix* evalAsNodeNameMatrix(IData* data);
	double getLowerBound(IData* data);
	double getUpperBound(IData* data);
	bool containsData(IData* data);
	string toString();
};

#endif /* NODE_HPP_ */
