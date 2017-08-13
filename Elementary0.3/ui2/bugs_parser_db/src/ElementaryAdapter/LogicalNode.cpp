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


#include "LogicalNode.hpp"

LogicalNode::LogicalNode() {
	nodenamematrix = new NodeNameMatrix();
}

LogicalNode::~LogicalNode() {
	delete this->nodenamematrix;

	for(std::vector<NodeNameMatrix* >::iterator it = this->nodeoperands.begin(); it!=this->nodeoperands.end(); ++it)
		delete (*it);

	this->nodeoperands.clear();
	this->nodeoperands.shrink_to_fit();
}

Long LogicalNode::serialize(ParserFileHandler * fd) {

	Long logicalnnmatid = nodenamematrix->serialize(fd, false);
	vector<Long> nodeoperandsids;
	for(auto it = nodeoperands.begin(); it != nodeoperands.end(); it++)
		nodeoperandsids.push_back((*it)->serialize(fd, false));

	// Create Logical Node
	fd->handleNodeFile(logicalnnmatid, this->expressionid, nodeoperandsids, 2, "");
	return logicalnnmatid;
}
