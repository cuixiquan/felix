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

#include "LogicalNodeStatement.hpp"
#include "LinkFunctionNode.hpp"
#include "ScalarFunction.hpp"
#include "MultivariateNode.hpp"

#include <iostream>
#include <typeinfo>
#include <cassert>
using namespace std;

map<Long, LogicalNodeComposite*> inmemorylogicalnodes; // definition

LogicalNodeStatement::LogicalNodeStatement() {
	logicalnodeexpressionid = -1;
}

LogicalNodeStatement::~LogicalNodeStatement() {
	delete this->logicalNode;
	// Logical Node Expressions are not deleted. Needed for processing in elementary.
}

string LogicalNodeStatement::toString(){
	string retval = "*LogicalNode :  ";
	retval = retval + " Name = " + this->logicalNode->toString() + "\n Expression = " + this->logicalnodeexp->toString();
	return retval+"\n";
}

/*
 * The first time this is invoked, the logical node expression as per the link function node and
 * the logicalNode becomes a univariatenode => This will be called only once during eval.
 */
void LogicalNodeStatement::handleLinkFunctionNode(IData* data)
{
    LinkFunctionNode *linkfnnode = (LinkFunctionNode*)((this->logicalNode));

    // Do some nasty pointer operations
    LogicalNodeExpression *exp = new LogicalNodeExpression();
    exp->type = LFUNCTION;
    ScalarFunction *scalarfunction = new ScalarFunction();
    scalarfunction->name = linkfnnode->scalarlinkfunctionmap[linkfnnode->linkfunction];
    scalarfunction->parameter1 = this->logicalnodeexp;
    exp->function = scalarfunction;
    this->logicalnodeexp = exp;
    this->logicalNode = linkfnnode->uvnode;
    delete linkfnnode;

    handleLogicalStochasticNode(data);
}

void LogicalNodeStatement::handleLogicalMultivariateNode(IData* data)
{
	LogicalNode* logicalnode = new LogicalNode();
	MultivariateNode *mvnode = (MultivariateNode*)(this->logicalNode);

	logicalnode->nodenamematrix->nodename = mvnode->getNodeid(data);
	logicalnode->nodenamematrix->type = NNM_MULTIVARIATE;
	mvnode->createIndices(logicalnode->nodenamematrix->indices, data);

	this->logicalnodeexp->createNodeOperands(logicalnode->nodeoperands, data);

	if(logicalnodeexpressionid == -1)
	{
		logicalnodeexpressionid = data->getData("expressionid", vector<int>());
		data->put("expressionid", logicalnodeexpressionid + 1);
		LogicalNodeComposite* composite = new LogicalNodeComposite(this->logicalnodeexp, new LogicalNodeExpressionMetadata());
		inmemorylogicalnodes[logicalnodeexpressionid] = composite;
	}
	logicalnode->expressionid = logicalnodeexpressionid;

	logicalnode->serialize(data->parserfiledetails);

	delete logicalnode;
}

void LogicalNodeStatement::handleLogicalStochasticNode(IData* data)
{
	LogicalNode* logicalnode = new LogicalNode();
	UnivariateNode *uvnode = (UnivariateNode*)(this->logicalNode);
	logicalnode->nodenamematrix = uvnode->evalAsNodeNameMatrix(data);

	this->logicalnodeexp->createNodeOperands(logicalnode->nodeoperands, data);

	if(logicalnodeexpressionid == -1)
	{
		logicalnodeexpressionid = data->getData("expressionid", vector<int>());
		data->put("expressionid", logicalnodeexpressionid + 1);
		LogicalNodeComposite* composite = new LogicalNodeComposite(this->logicalnodeexp, new LogicalNodeExpressionMetadata());
		inmemorylogicalnodes[logicalnodeexpressionid] = composite;
	}
	logicalnode->expressionid = logicalnodeexpressionid;

	logicalnode->serialize(data->parserfiledetails);

	delete logicalnode;
}

void LogicalNodeStatement::eval(IData* data){
	if(typeid(*this->logicalNode) == typeid(LinkFunctionNode))
	    handleLinkFunctionNode(data);
	else if(typeid(*this->logicalNode) == typeid(MultivariateNode))
		handleLogicalMultivariateNode(data);
	else
		handleLogicalStochasticNode(data);
}
