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

#ifndef LOGICALNODEEXPRESSIONMETADATA_H_
#define LOGICALNODEEXPRESSIONMETADATA_H_

#include <vector>
using namespace std;

class LogicalNodeExpressionMetadata {
public:
	/* This stores the indexes within the double array for every node in the expression.
	 * It is also used to temporarily store the gvids. It will be changed into indexes later on.
	 */
    vector<int> inputlocationparams;
    vector<int> vectorfunctionnoofnodes; // This stores the number of nodes needed by a vector function [sequentially]
    int numberofnodes; // Number of nodenames added to elementary while creating the factor graph for this LNE

	LogicalNodeExpressionMetadata() {
		this->numberofnodes = 0;
	}

	~LogicalNodeExpressionMetadata(){

	}
};

#endif /* LOGICALNODEEXPRESSIONMETADATA_H_ */
