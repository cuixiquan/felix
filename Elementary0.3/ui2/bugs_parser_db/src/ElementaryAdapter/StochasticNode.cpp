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


#include "StochasticNode.hpp"

StochasticNode::StochasticNode() {
	limitation = new Limitation();
	nodenamematrix = new NodeNameMatrix();
}

StochasticNode::~StochasticNode() {
	delete nodenamematrix;
	delete limitation;

	for(std::vector<NodeNameMatrix* >::iterator it = this->distributionparameters.begin(); it!=this->distributionparameters.end(); ++it)
		delete (*it);

	this->distributionparameters.clear();
	this->distributionparameters.shrink_to_fit();
}

Long StochasticNode::serialize(ParserFileHandler * fd) {

	Long stochasticnodennmatid = nodenamematrix->serialize(fd, false);
	Long limitationid = limitation->serialize(fd);
	vector<Long> distparamsids;
	for(auto it = distributionparameters.begin(); it != distributionparameters.end(); it++)
		distparamsids.push_back((*it)->serialize(fd, false));

	// Create Stochastic Node
	fd->handleNodeFile(stochasticnodennmatid, limitationid, distparamsids, 1, distributionname);
	return stochasticnodennmatid;
}
