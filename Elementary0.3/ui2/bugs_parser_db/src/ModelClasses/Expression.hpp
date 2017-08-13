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


#ifndef EXPRESSION_HPP_
#define EXPRESSION_HPP_

#include "INode.hpp"
#include <string>
#include <vector>
#include "../DataClasses/IData.hpp"
#include "../ElementaryAdapter/NodeNameMatrix.hpp"
using namespace std;

enum ExpressionType { CONSTANT, NODE, EXPRESSION, OPERATOR, EMPTY};
/*
 * Expression can be a double value or a node or an expression or operator
 */
class Expression {
private:
	double operate(double previousvalue, string op, double newvalue);
public:
	Expression();
	Expression(double expvalue);
	Expression(Expression* exp);
	virtual ~Expression();
	double expvalue;
	INode* uvnode;
	Expression* exp;
	ExpressionType type;

	string op;
	Expression* leftchild;
	Expression* rightchild;

	NodeNameMatrix* evalDistParams(IData* data);
	double evalAsData(IData* data);
	int evalAsIndex(IData* data); // This is because after 6 significant digits, double/float converts into exponent.
	bool canEvaluate(IData* data);
	string toString();
};

#endif /* EXPRESSION_HPP_ */
