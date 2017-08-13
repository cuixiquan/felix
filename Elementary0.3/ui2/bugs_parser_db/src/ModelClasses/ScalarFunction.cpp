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

#include "ScalarFunction.hpp"
#include "UnivariateNode.hpp"
#include <iostream>
#include <sstream>
#include <cmath>
#include <cassert>
#include <cfloat>
#include <ctime>
#define MATH_PI 3.1415926
#define MATH_E 2.71828
using namespace std;

ScalarFunction::ScalarFunction() {
	this->isunaryminus = false;
}

ScalarFunction::~ScalarFunction() {
	delete this->parameter1;
	if(this->name == "EQUALS" || this->name == "MAX" || this->name == "MIN" || this->name == "POW"
			|| this->name == "CUMULATIVE")
		delete this->parameter2;
}

string ScalarFunction::toString(){
	string retval = " ";
	retval = retval + "Name = " + this->name + " Parameter= " + this->parameter1->toString();
	if(this->name == "EQUALS" || this->name == "MAX" || this->name == "MIN" || this->name == "POW" || this->name == "CUMULATIVE")
		retval = retval + this->parameter2->toString();
	return retval;
}

inline long ScalarFunction::Factorial(int number) {
	if(number <= 1) return 1;
	return number * Factorial(number - 1);
}

inline double ScalarFunction::Gamma(double n1) {
	if(n1 <= 0)
		return 0;
	return tgamma(n1);
}

inline double ScalarFunction::getReturnValue(double result) {
	if(::isnan(result))
		return -DBL_MAX;
	else if (result <= -DBL_MAX)
		return -DBL_MAX;
	else if (result >= DBL_MAX)
		return DBL_MAX;

	if(this->isunaryminus)
		result = -1 * result;

	return result;
}

inline double ScalarFunction::Power(double n1, double n2) {
	if(n1 < 0 && ceilf(n2)!=n2)
		return -1 * pow(-n1, n2);
	return pow(n1,n2);
}

inline double ScalarFunction::phi(double e){
	double b0 = 0.2316419, b1 = 0.319381530, b2 = -0.356563782, b3 = 1.781477937, b4 = -1.821255978, b5 = 1.330274429;
	double t = (1.0/(b0*e));
	double pdf = (1.0/sqrt(2.0*MATH_PI))* this->Power(MATH_E, -1*(0.5)*(e * e));
	double cdf = 1.0 - pdf * (b1*t + b2 * this->Power(t,2) + b3 * this->Power(t,3) + b4 * this->Power(t,4) + b5 * this->Power(t,5));
	return cdf;
}

void ScalarFunction::createNodeOperands(vector<NodeNameMatrix*> &nodeoperands, IData* data) {
	this->parameter1->createNodeOperands(nodeoperands, data);

	if(this->name == "EQUALS" || this->name == "MAX" || this->name == "MIN" || this->name == "POW"){
		this->parameter2->createNodeOperands(nodeoperands, data);
	}
}

double ScalarFunction::internaleval(vector<double> &inputparams, LogicalNodeExpressionMetadata* metadata,
		int & currentcounter, int &vectorfncounter){
	double evaluatedvalue = this->parameter1->internaleval(inputparams, metadata, currentcounter, vectorfncounter);

	if(this->name == "ABS"){
		return this->getReturnValue(abs(evaluatedvalue));
	}
	else if(this->name == "ARCCOS"){
		assert(evaluatedvalue <= 1);
		assert(evaluatedvalue >= -1);
		return this->getReturnValue(acos(evaluatedvalue));
	}
	else if(this->name == "ARCCOSH"){
		return this->getReturnValue(acosh(evaluatedvalue));
	}
	else if(this->name == "ARCSIN"){
		assert(evaluatedvalue <= 1);
		assert(evaluatedvalue >= -1);
		return this->getReturnValue(asin(evaluatedvalue));
	}
	else if(this->name == "ARCSINH"){
		return this->getReturnValue(asinh(evaluatedvalue));
	}
	else if(this->name == "ARCTAN"){
		return this->getReturnValue(atan(evaluatedvalue));
	}
	else if(this->name == "ARCTANH"){
		return this->getReturnValue(atanh(evaluatedvalue));
	}
	else if(this->name == "CLOGLOG"){
		return this->getReturnValue(log(-log(1.0 - evaluatedvalue)));
	}
	else if(this->name == "COS"){
		return this->getReturnValue(cos(evaluatedvalue));
	}
	else if(this->name == "COSH"){
		return this->getReturnValue(cosh(evaluatedvalue));
	}
	else if(this->name == "CLOGLOG"){
		return this->getReturnValue(log(-log(1.0 - evaluatedvalue)));
	}
	else if(this->name == "EQUALS"){
		double evaluatedvalue2 = this->parameter2->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		return this->getReturnValue((evaluatedvalue == evaluatedvalue2) ? 1 : 0);
	}
	else if(this->name == "EXP"){
		return this->getReturnValue(exp(evaluatedvalue));
	}
	else if(this->name == "ILOGIT"){
		double returnvalue = this->getReturnValue((exp(evaluatedvalue) / (1.0 + exp(evaluatedvalue))));
		if(returnvalue > 1 or returnvalue < 0)
			return -DBL_MAX;
		else
			return returnvalue;
	}
	else if(this->name == "ICLOGLOG"){
		return this->getReturnValue(1.0 - exp(-exp(evaluatedvalue)));
	}
	else if(this->name == "LOG"){
		return this->getReturnValue(log(evaluatedvalue));
	}
	else if(this->name == "LOGFACT"){
		return this->getReturnValue(log(this->Factorial(evaluatedvalue)));
	}
	else if(this->name == "LOGGAM"){
		return this->getReturnValue(log(this->Factorial(evaluatedvalue)));
	}
	else if(this->name == "LOGIT"){
		return this->getReturnValue(log(evaluatedvalue/(1.0 - evaluatedvalue)));
	}
	else if(this->name == "ILOG"){
		return this->getReturnValue(exp(evaluatedvalue));
	}
	else if(this->name == "MAX"){
		double evaluatedvalue2 = this->parameter2->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		double result = this->getReturnValue((evaluatedvalue > evaluatedvalue2) ? evaluatedvalue : evaluatedvalue2);
		return result;
	}
	else if(this->name == "MIN"){
		double evaluatedvalue2 = this->parameter2->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		return this->getReturnValue((evaluatedvalue < evaluatedvalue2) ? evaluatedvalue : evaluatedvalue2);
	}
	else if(this->name == "POW"){
		double evaluatedvalue2 = this->parameter2->internaleval(inputparams, metadata, currentcounter, vectorfncounter);
		return this->getReturnValue(pow(evaluatedvalue, evaluatedvalue2));
	}
	else if(this->name == "ROUND"){
		return this->getReturnValue(evaluatedvalue < 0.0 ? ceil(evaluatedvalue - 0.5) : floor(evaluatedvalue + 0.5));
	}
	else if(this->name == "SIN"){
		return this->getReturnValue(sin(evaluatedvalue));
	}
	else if(this->name == "SINH"){
		return this->getReturnValue(sinh(evaluatedvalue));
	}
	else if(this->name == "SQRT"){
		return this->getReturnValue(sqrt(evaluatedvalue));
	}
	else if(this->name == "STEP"){
		return this->getReturnValue(evaluatedvalue >=0 ? 1 : 0);
	}
	else if(this->name == "TAN"){
		return this->getReturnValue(tan(evaluatedvalue));
	}
	else if(this->name == "TANH"){
		return this->getReturnValue(tanh(evaluatedvalue));
	}
	else if(this->name == "TRUNC"){
		return this->getReturnValue(ceilf(evaluatedvalue));
	}
	else if(this->name == "PHI"){
		double cdf = this->phi(evaluatedvalue);
		return this->getReturnValue(cdf);
	}
	else if(this->name == "PROBIT"){
		double cdf = this->phi(evaluatedvalue);
		return this->getReturnValue(1.0/cdf);
	}
	else {
		cout << "Attention!!! [BUGSError] Function " << this->name << " is not yet implemented." << endl;
		assert(false);
	}
	return 0;
}
