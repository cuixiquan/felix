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


#include "Expression.hpp"
#include "UnivariateNode.hpp"
#include <string>
#include <typeinfo>
#include <sstream>
#include <cassert>
#include <iostream>
using namespace std;

Expression::Expression() {
	// TODO Auto-generated constructor stub
}

Expression::Expression(double expvalue) {
	this->expvalue = expvalue;
	this->type = CONSTANT;
}

Expression::Expression(Expression* expression) {
	this->type = expression->type;
	if(this->type == CONSTANT)
		this->expvalue = expression->expvalue;
	else if(this->type == NODE)
	{
		if(typeid(*expression->uvnode) == typeid(UnivariateNode))
			this->uvnode = new UnivariateNode((UnivariateNode*)expression->uvnode);
		else
		{
			cout << " Attention!!! [BUGSError] Multivariate Node in Expression" <<endl;
			assert(false);
		}
	}
	else if(this->type == EXPRESSION)
		this->exp = new Expression(expression->exp);
	else if (this->type == OPERATOR){
		this->op = expression->op;
		this->leftchild = new Expression(expression->leftchild);
		this->rightchild = new Expression(expression->rightchild);
	}
}

Expression::~Expression() {
	if(this->type == NODE)
		delete this->uvnode;
	else if(this->type == EXPRESSION)
		delete this->exp;
	else if(this->type == OPERATOR)
	{
		delete this->leftchild;
		delete this->rightchild;
	}
}

string Expression::toString() {
	string retval = "";
	if(this->type == CONSTANT)
	{
		stringstream ss (stringstream::in | stringstream::out);
        ss << this->expvalue;
		retval = retval + "Constant " + ss.str() + " ";
	}
	else if(this->type == NODE)
	{
		retval = retval + "Node " ;
		if(typeid(*this->uvnode) == typeid(UnivariateNode)){
			retval = retval + ((UnivariateNode*)this->uvnode)->toString();
		}
	}
	else if(this->type == EXPRESSION)
	{
		retval = retval + "Expression " ;
		retval = retval + this->exp->toString();
	}

	else if(this->type == OPERATOR)
	{
		retval = retval + "Operator " +  this->op + " " ;
		retval = retval + "Left Child = " + this->leftchild->toString() + " ";
		retval = retval + "Right Child = " + this->rightchild->toString() + " ";
	}
	else {
		retval = retval + "Empty expression" + " ";
	}
	return retval;
}

bool Expression::canEvaluate(IData* data) {
	if(this->type == CONSTANT)
		return true;
	else if(this->type == NODE){
		if(typeid(*this->uvnode) == typeid(UnivariateNode))
			return ((UnivariateNode*)this->uvnode)->canEvaluate(data);
		else {
			cout << "Attention!!! [BUGSError] Not Evaluating MultivariateNode Expression" <<endl;
			assert(false);
		}
	}
	else if(this->type == EXPRESSION)
		return this->exp->canEvaluate(data);
	else if(this->type == OPERATOR)
		return (this->leftchild->canEvaluate(data) && this->rightchild->canEvaluate(data));
	else
		return false;
}

double Expression::evalAsData(IData* data) {
	double value;
	if(this->type == CONSTANT)
	{
		value = this->expvalue;
	}
	else if(this->type == NODE){
		if(typeid(*this->uvnode) == typeid(UnivariateNode))
			value = ((UnivariateNode*)this->uvnode)->evalAsData(data);
		else {
			cout << "Attention!!! [BUGSError] Not Evaluating MultivariateNode Expression" <<endl;
			assert(false);
		}
	}
	else if(this->type == EXPRESSION){
		value = this->exp->evalAsData(data);
	}
	else if(this->type == OPERATOR){
		value = operate(this->leftchild->evalAsData(data), this->op, this->rightchild->evalAsData(data));
	}
	else {
		cout << "Attention!!! [BUGSError] Expression is Empty. Cannot Evaluate" <<endl;
		assert(false);
	}
	return value;
}

int Expression::evalAsIndex(IData* data) {
	int value;
	if(this->type == CONSTANT)
	{
		value = this->expvalue;
	}
	else if(this->type == NODE){
		if(typeid(*this->uvnode) == typeid(UnivariateNode))
			value = ((UnivariateNode*)this->uvnode)->evalAsIndex(data);
		else {
			cout << "Attention!!! [BUGSError] Not Evaluating MultivariateNode Expression" <<endl;
			assert(false);
		}
	}
	else if(this->type == EXPRESSION){
		value = this->exp->evalAsIndex(data);
	}
	else if(this->type == OPERATOR){
		value = operate(this->leftchild->evalAsIndex(data), this->op, this->rightchild->evalAsIndex(data));
	}
	else {
		cout << "Attention!!! [BUGSError] Expression is Empty. Cannot Evaluate" <<endl;
		assert(false);
	}
	return value;
}

double Expression::operate(double left, string op, double right){
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

NodeNameMatrix* Expression::evalDistParams(IData* data){
	NodeNameMatrix* nodenamematrix;

	if(this->type == CONSTANT) {
		nodenamematrix = new NodeNameMatrix();
		nodenamematrix->value = this->expvalue;
		nodenamematrix->type = NNM_UNIVARIATECONSTANT; // Handle as NNM_UNC
	}
	else if(this->type == NODE){
		if(typeid(*this->uvnode) == typeid(UnivariateNode))
		{
			nodenamematrix = ((UnivariateNode*)this->uvnode)->evalAsNodeNameMatrix(data);
		}
		else
		{
			cout << " Attention!!! [BUGSError] Cannot EvalDistParams of Multivariate Node in Expression" <<endl;
			assert(false);
		}
	}
	else if(this->type == EXPRESSION){
		nodenamematrix = this->exp->evalDistParams(data);
	}
	else {
//		This should not be possible as distribution parameters are not allowed to contain expressions with operators
		cout << " Attention!!! [BUGSError] Distribution Parameter contains either Invalid Operator or is Empty" <<endl;
		assert(false);
	}
	return nodenamematrix;
}
