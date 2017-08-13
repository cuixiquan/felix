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


#include "LinkFunctionNode.hpp"
#include <iostream>
#include <cassert>

LinkFunctionNode::LinkFunctionNode() {
	this->scalarlinkfunctionmap["LOGIT"] = "ILOGIT";
	this->scalarlinkfunctionmap["CLOGLOG"] = "ICLOGLOG";
	this->scalarlinkfunctionmap["PROBIT"] = "IPROBIT";
	this->scalarlinkfunctionmap["LOG"] = "ILOG";
	this->isunaryminus = false;
}

LinkFunctionNode::~LinkFunctionNode() {
//	delete this->uvnode; [Will be pointed to when eval of Logical Node Statement is called the first time]
}

string LinkFunctionNode::toString(){
	string retval = " ";
	retval = retval + "LinkFunction = " + this->linkfunction + " Node= " + this->uvnode->toString();
	return retval;
}

double LinkFunctionNode::evalAsData(IData* data){
//	return 0; // To Do write eval using linkfunction and univariate node
	cout << " Attention!!! [BugsError] Evaluation function of LinkFunctionNode not implemented " <<endl;
	assert(false);

	return 0;
}

string LinkFunctionNode::getNodeid(IData* data){
	return this->uvnode->getNodeid(data);
}
