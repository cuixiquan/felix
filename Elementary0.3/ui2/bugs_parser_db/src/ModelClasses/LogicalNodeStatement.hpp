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

#ifndef LOGICALNODESTATEMENT_H_
#define LOGICALNODESTATEMENT_H_

#include "INode.hpp"
#include "IStatement.hpp"
#include "LogicalNodeExpression.hpp"
#include "../ElementaryAdapter/LogicalNode.hpp"
#include "../AdapterClasses/LogicalNodeComposite.hpp"
#include <vector>

class LogicalNodeStatement : public IStatement{
public:
	LogicalNodeStatement();
	virtual ~LogicalNodeStatement();
	INode* logicalNode;
	LogicalNodeExpression* logicalnodeexp;
	Long logicalnodeexpressionid;
	void eval(IData* data);
	string toString();
private:
    void handleLinkFunctionNode(IData* data);
    void handleLogicalMultivariateNode(IData* data);
    void handleLogicalStochasticNode(IData* data);
};

#endif /* LOGICALNODESTATEMENT_H_ */
