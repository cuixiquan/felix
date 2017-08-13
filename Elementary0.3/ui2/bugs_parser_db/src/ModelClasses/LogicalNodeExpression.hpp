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

#ifndef LOGICALNODEEXPRESSION_HPP_
#define LOGICALNODEEXPRESSION_HPP_
#include "INode.hpp"
#include "IFunction.hpp"
#include "UnivariateNode.hpp"
#include "LogicalNodeExpressionMetadata.h"
#include "../ElementaryAdapter/NodeNameMatrix.hpp"
#include <vector>
#include <map>
#include <list>

enum LogicalNodeExpressionType {LCONSTANT, LNODE, LFUNCTION, LEXPRESSION, LOPERATOR};

class LogicalNodeExpression {
private:
	double operate(double left, string op, double right);
	void handleNodeInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data);
	void handleExpressionInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data);
	void handleOperatorInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data);
	void handleFunctionInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data);
public:
    LogicalNodeExpression();
    LogicalNodeExpression(LogicalNodeExpression *value, IData *data);
    virtual ~LogicalNodeExpression();
    double expvalue;
    INode *uvnode;
    IFunction *function;
    LogicalNodeExpression *exp;
    LogicalNodeExpressionType type;
    string op;
    LogicalNodeExpression *leftchild;
    LogicalNodeExpression *rightchild;
    bool isunaryminus;

    void createNodeOperands(vector<NodeNameMatrix*> &nodeoperands, IData* data);
    string toString();
	double eval(double params[], LogicalNodeExpressionMetadata* metadata);
	double internaleval(vector<double>& inputparams, LogicalNodeExpressionMetadata* metadata, int &currentcounter,
				int &vectorfncounter);
};

#endif /* LOGICALNODEEXPRESSION_HPP_ */
