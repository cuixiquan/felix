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

#include "StochasticNodeLimitation.hpp"
#include <sstream>
#include <iostream>

StochasticNodeLimitation::StochasticNodeLimitation() {
	// TODO Auto-generated constructor stub

}

StochasticNodeLimitation::~StochasticNodeLimitation() {
	if(this->optional!=LOWER)
		delete this->lowerlimit;
	if(this->optional!=UPPER)
		delete this->upperlimit;
}

string StochasticNodeLimitation::toString(){
	string retval = "";
	if(this->optional!=LOWER)
		retval = retval + " LowerLimit= " + this->lowerlimit->toString();
	if(this->optional!=UPPER)
		retval = retval + " UpperLimit= " + this->upperlimit->toString();
	return retval;
}

double StochasticNodeLimitation::findlowerbound(IData* data){
	if(this->optional==LOWER)
		return 0;
	return this->lowerlimit->evalAsData(data);
}

double StochasticNodeLimitation::findupperbound(IData* data){
	if(this->optional==UPPER)
		return 0;
	return this->upperlimit->evalAsData(data);
}
