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


#include "MultiDimExpression.hpp"
#include <iostream>
using namespace std;

MultiDimExpression::MultiDimExpression() {
	this->empty = false;
	this->colon = false;
	// TODO Auto-generated constructor stub

}

MultiDimExpression::~MultiDimExpression() {
	if(this->empty)
	{
	}
	else if(this->colon) {
		delete this->expbeforecolon;
		delete this->expaftercolon;
	}
	else
	{
		delete this->expbeforecolon;
	}
}

MultiDimExpression::MultiDimExpression(MultiDimExpression* mdexp) {
	if(mdexp->empty)
	{
		this->empty = true;
		this->colon = false;
	}
	else if(mdexp->colon) {
		this->colon = true;
		this->empty = false;
		this->expbeforecolon = new Expression(mdexp->expbeforecolon);
		this->expaftercolon = new Expression(mdexp->expaftercolon);
	}
	else
	{
		this->colon = false;
		this->empty = false;
		this->expbeforecolon = new Expression(mdexp->expbeforecolon);
	}
}

bool MultiDimExpression::canEvaluate(IData* data) {
	if(this->empty)
		return false;
	else if(this->colon)
		return this->expbeforecolon->canEvaluate(data) && this->expaftercolon->canEvaluate(data);
	else
		return this->expbeforecolon->canEvaluate(data);
}

string MultiDimExpression::toString(){
	string retval="";
	if(this->empty)
		retval = retval + " Empty Dimension ";
	else if(this->colon)
		retval = " Multidimension " + this->expbeforecolon->toString() + " : " + this->expaftercolon->toString();
	else
		retval = " Singledimension " + this->expbeforecolon->toString();

	return retval;
}
