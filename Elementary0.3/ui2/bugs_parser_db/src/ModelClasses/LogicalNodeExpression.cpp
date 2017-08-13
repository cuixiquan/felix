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

#include "LogicalNodeExpression.hpp"
#include "UnivariateNode.hpp"
#include "ScalarFunction.hpp"
#include "VectorFunction.hpp"
#include <typeinfo>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cassert>
using namespace std;

LogicalNodeExpression::LogicalNodeExpression() {
	this->isunaryminus = false;
}


LogicalNodeExpression::~LogicalNodeExpression() {
	if(this->type == LEXPRESSION)
		delete this->exp;
	else if(this->type == LOPERATOR){
		delete this->leftchild;
		delete this->rightchild;
	}
	else if(this->type == LFUNCTION){
		delete this->function;
	}
	else if(this->type == LNODE){
		delete this->uvnode;
	}
}

string LogicalNodeExpression::toString(){
	string retval = "";
	if(this->type == LCONSTANT)
	{
		stringstream ss (stringstream::in | stringstream::out);
		ss << this->expvalue;
		retval = retval + "Constant " + ss.str() + " ";
	}
	else if(this->type == LNODE)
		retval = retval + "Node " + this->uvnode->toString();
	else if(this->type == LFUNCTION)
		retval = retval + "Function " + this->function->toString();
	else if(this->type == LEXPRESSION)
		retval = retval + "Expression " + this->exp->toString() ;
	else{
		retval = retval + "Operator "  + this->op + " " ;
		retval = retval + "Left Child " + this->leftchild->toString() + " ";
		retval = retval + "Right Child " + this->rightchild->toString() + " ";
	}
	return retval;
}

double LogicalNodeExpression::internaleval(vector<double> &inputparams, LogicalNodeExpressionMetadata* metadata,
		int & currentcounter, int &vectorfncounter){

	if(this->type == LCONSTANT)
		return this->expvalue;
	else if(this->type == LNODE)
	{
		assert(currentcounter < metadata->inputlocationparams.size());
		if(this->uvnode->isunaryminus)
			return -1 * inputparams[metadata->inputlocationparams[currentcounter++]];
		else
			return inputparams[metadata->inputlocationparams[currentcounter++]];
	}
	else if(this->type == LEXPRESSION)
	{
		if(this->exp->isunaryminus)
			return -1 * this->exp->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		else
			return this->exp->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
	}
	else if (this->type == LFUNCTION)
		return (this->function)->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
	else
	{
		double lefteval = this->leftchild->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		double righteval = this->rightchild->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		return this->operate(lefteval, op, righteval);
	}
}

double LogicalNodeExpression::operate(double left, string op, double right){
	if(op=="+")
		return left+right;
	if(op=="-")
		return left-right;
	if(op=="*")
		return left*right;
	if(op=="/")
		return left/right;
	return right;
}

double LogicalNodeExpression::eval(double params[], LogicalNodeExpressionMetadata* metadata){
	vector<double> inputparams;
	for(int i=0; i < metadata->numberofnodes; i++){
		inputparams.push_back(params[i]);
	}

	int currentcounter = 0;
	int vectorfncounter = 0;
	double result = this->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
	return result;
}

void LogicalNodeExpression::createNodeOperands(vector<NodeNameMatrix*> &nodeoperands, IData* data) {
	if(this->type == LCONSTANT)
		return;
	else if(this->type == LNODE)
		handleNodeInConstructingNodeOperands(nodeoperands,data);
	else if(this->type == LEXPRESSION)
		handleExpressionInConstructingNodeOperands(nodeoperands,data);
	else if(this->type == LFUNCTION)
		handleFunctionInConstructingNodeOperands(nodeoperands,data);
	else if(this->type == LOPERATOR)
		handleOperatorInConstructingNodeOperands(nodeoperands,data);
}

void LogicalNodeExpression::handleNodeInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data){
	assert(typeid(*this->uvnode) == typeid(UnivariateNode));

	UnivariateNode* uvnode = (UnivariateNode*)(this->uvnode);
	/*
	 * Need to check for data since we are not using the constructor anymore
	 */
	if(uvnode->containsData(data)){
		NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
		nodenamematrix->nodename = uvnode->getNodeid(data);
		nodenamematrix->value = uvnode->evalAsData(data);
		nodenamematrix->type = NNM_UNIVARIATECONSTANT; // Handle as NNM_UNC
		nodeoperands.push_back(nodenamematrix);
	}
	else {
		nodeoperands.push_back(uvnode->evalAsNodeNameMatrix(data));
	}
}

void LogicalNodeExpression::handleExpressionInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data)
{
    this->exp->createNodeOperands(nodeoperands, data);
}


void LogicalNodeExpression::handleOperatorInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data) {
	this->leftchild->createNodeOperands(nodeoperands, data);
	this->rightchild->createNodeOperands(nodeoperands, data);
}

void LogicalNodeExpression::handleFunctionInConstructingNodeOperands(vector<NodeNameMatrix*> &nodeoperands,IData* data) {
	this->function->createNodeOperands(nodeoperands, data);
}

