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

#ifndef SCALARFUNCTION_HPP_
#define SCALARFUNCTION_HPP_

#include "IFunction.hpp"
#include "LogicalNodeExpression.hpp"

class ScalarFunction : public IFunction{
private:
	long Factorial(int number);
	double Gamma(double number);
	double getReturnValue(double result);
	double Power(double n1, double n2);
public:
	ScalarFunction();
	virtual ~ScalarFunction();

	LogicalNodeExpression* parameter1;
	LogicalNodeExpression* parameter2;
	LogicalNodeExpression* parameter3;
	LogicalNodeExpression* parameter4;

	string toString();
	double internaleval(vector<double> &inputparams, LogicalNodeExpressionMetadata* metadata, int & currentcounter, int &vectorfncounter);
	void createNodeOperands(vector<NodeNameMatrix*> &nodeoperands, IData* data);
	double phi(double e);
};

#endif /* SCALARFUNCTION_HPP_ */
