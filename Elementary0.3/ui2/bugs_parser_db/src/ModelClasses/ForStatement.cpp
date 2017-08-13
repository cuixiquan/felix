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


#include "ForStatement.hpp"
#include <sstream>
#include <iostream>
using namespace std;

ForStatement::ForStatement() {
	// TODO Auto-generated constructor stub

}

ForStatement::~ForStatement() {
	delete this->beginexpression;
	delete this->endexpression;

	for (std::list<IStatement*>::iterator stit = this->statements.begin(); stit!=this->statements.end(); stit++){
		delete (*stit);
	}
}

string ForStatement::toString(){
	string retval = "*ForStatementBegin  ";
	retval = retval + "Loopvariable = " + this->loopvariable + " ";
	retval = retval + "Loopbegin is Expression = " + this->beginexpression->toString();
    retval = retval + "Loopend is Expression = " + this->endexpression->toString();
	retval += "\n";

	for (std::list<IStatement*>::iterator stit = this->statements.begin(); stit!=this->statements.end(); stit++)
		retval = retval + (*stit)->toString();

	return retval + "ForStatmentEnd \n";
}

void ForStatement::eval(IData* data){
	int loopbegin = this->beginexpression->evalAsIndex(data);
	int loopend = this->endexpression->evalAsIndex(data);

	data->put(this->loopvariable, loopbegin);
	while(data->getData(loopvariable, vector<int>()) <= loopend) {
		for (std::list<IStatement*>::iterator stit = this->statements.begin(); stit!=this->statements.end(); stit++){
			(*stit)->eval(data);
		}
		data->put(this->loopvariable, data->getData(loopvariable, vector<int>()) + 1);
	}

	data->put(this->loopvariable, data->getData(loopvariable, vector<int>()) - 1);
}
