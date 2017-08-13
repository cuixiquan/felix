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


#include "NodeNameMatrix.hpp"
#include <iostream>

NodeNameMatrix::NodeNameMatrix() {
	this->nodename = "";
}

NodeNameMatrix::~NodeNameMatrix() {
	for(auto it = this->indices.begin(); it != this->indices.end(); it++)
		delete (*it);
}

string NodeNameMatrix::getNodeid(){
	string nodename = "";
	if(type == NNM_CONSTANT)
	{
		stringstream ss;
		ss << value;
		nodename += ss.str();
	}
	else
	 nodename += this->nodename;

	if(this->indices.size()>0){
		nodename += "[";
		for(auto it=this->indices.begin(); it!=this->indices.end(); ++it){
			nodename = nodename + (*it)->getNodeid();
			if((it+1)!=this->indices.end())
				nodename += ",";
		}
		nodename += "]";
	}
	return nodename;
}

/*
 * Using only the first character while serializing to save space.
 */
//('M','U', 'UC', 'E','C','N', 'R') = (1,2,3,4,5,6,7)
Long NodeNameMatrix::serialize(ParserFileHandler *fd, bool isindex) {
	Long stochasticnodennmatid = -1;
	if(type == NNM_UNIVARIATE) {
		vector<Long> indexids;
		for(auto it = indices.begin(); it != indices.end(); it++) {
			indexids.push_back((*it)->serialize(fd, true));
		}
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFileWithIndexIds(fd->nodenamematrixid, nodename, indexids, 2);
	}
	else if(type == NNM_MULTIVARIATE){
		vector<Long> indexids;
		for(auto it = indices.begin(); it != indices.end(); it++) {
			indexids.push_back((*it)->serialize(fd, true));
		}
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFileWithIndexIds(fd->nodenamematrixid, nodename, indexids, 1);
	}
	else if(type == NNM_UNIVARIATECONSTANT){
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFileWithValue(fd->nodenamematrixid, nodename, value, 3, isindex);
	}
	else if(type == NNM_CONSTANT){
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFileWithValue(fd->nodenamematrixid, nodename, value, 5, isindex);
	}
	else if(type == NNM_RANGE){
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFile(fd->nodenamematrixid, nodename, 7);
	}
	else if(type == NNM_EMPTY){
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFile(fd->nodenamematrixid, nodename, 4);
	}
	else {
		// TODO Assuming Node will not have further indices
		assert(indices.size() == 0);
		stochasticnodennmatid = fd->nodenamematrixid;
		fd->handleNodeNameMatrixFile(fd->nodenamematrixid, nodename, 6);
	}
	return stochasticnodennmatid;
}
