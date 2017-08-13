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


#ifndef FORSTATEMENT_HPP_
#define FORSTATEMENT_HPP_
#include "IStatement.hpp"
#include "UnivariateNode.hpp"
#include <list>

class ForStatement : public IStatement {
public:
	ForStatement();
	virtual ~ForStatement();
	string loopvariable;
	Expression* beginexpression;
	Expression* endexpression;

	list<IStatement* > statements;
	void eval(IData* data);
	string toString();
};

#endif /* FORSTATEMENT_HPP_ */
