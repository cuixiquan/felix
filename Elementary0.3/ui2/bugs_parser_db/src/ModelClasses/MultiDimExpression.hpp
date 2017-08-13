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

#ifndef MULTIDIMEXPRESSION_HPP_
#define MULTIDIMEXPRESSION_HPP_

#include "Expression.hpp"
/*
 * Multidimensional expression can contain expression:expression or only expression
 * However a multivariate node should contain atleast one Multidimensional expression with a colon
 */

class MultiDimExpression {
public:
	MultiDimExpression();
	MultiDimExpression(MultiDimExpression* mdexp);
	virtual ~MultiDimExpression();
	bool empty;

	Expression* expbeforecolon;
	Expression* expaftercolon;
	bool colon;
	bool canEvaluate(IData* data);
	string toString();
};

#endif /* MULTIDIMEXPRESSION_HPP_ */
