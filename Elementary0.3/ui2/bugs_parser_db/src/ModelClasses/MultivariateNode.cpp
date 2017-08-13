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

#include "MultivariateNode.hpp"
#include <iostream>
#include <cassert>
#include <typeinfo>
#include "../ElementaryAdapter/StochasticNode.hpp"
#include "../ElementaryAdapter/LogicalNode.hpp"

using namespace std;

MultivariateNode::MultivariateNode() {
	this->isunaryminus = false;
}

MultivariateNode::~MultivariateNode() {
	for(std::vector<MultiDimExpression* >::iterator it=this->indices.begin(); it!=this->indices.end();++it)
		delete (*it);
}

string MultivariateNode::toString(){
	string retval = " ";
	retval = retval + this->nodeid + " ";
	for(vector<MultiDimExpression* >::iterator it=this->indices.begin();it!=this->indices.end(); ++it)
		retval = retval + (*it)->toString();

	return retval;
}

void MultivariateNode::createIndices(vector<NodeNameMatrix*> &nodenamematrixindices, IData* data) {
	for(std::vector<MultiDimExpression* >::iterator it = this->indices.begin(); it != this->indices.end(); ++it)
	{
		NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
		if((*it)->empty)
			nodenamematrix->type = NNM_EMPTY;
		else if((*it)->colon)
		{
			nodenamematrix->type = NNM_RANGE;
			// TODO Assuming the loopbegin and loopend can be evaluated to integers.
		    int loopbegin = (*it)->expbeforecolon->evalAsIndex(data);
		    int loopend = (*it)->expaftercolon->evalAsIndex(data);
		    std::ostringstream buff;
		    buff << loopbegin << ":" << loopend;
			nodenamematrix->nodename = buff.str();
		}
		else
		{
			if((*it)->expbeforecolon->canEvaluate(data)) {
				nodenamematrix->type = NNM_CONSTANT;
				nodenamematrix->value = (*it)->expbeforecolon->evalAsIndex(data);
			}
			else {
				if((*it)->expbeforecolon->type != NODE && typeid(*(*it)->expbeforecolon->uvnode) != typeid(UnivariateNode))
					assert(false);

				// TODO Assuming that this can only be a Univariate Node. So all we need is just the node's id.
				UnivariateNode* uvnode = new UnivariateNode((UnivariateNode*)((*it)->expbeforecolon->uvnode));
				nodenamematrix->type = NNM_NODE;
				nodenamematrix->nodename = uvnode->getNodeid(data);
				delete uvnode;
			}
		}
		nodenamematrixindices.push_back(nodenamematrix);
	}
}

double MultivariateNode::evalAsData(IData* data){
	cout << " Attention!!! [BUGSError] Eval Function should not be called in MultivariateNode [Name: " << this->nodeid << "]" <<endl;
	return 0;
}

string MultivariateNode::getNodeid(IData* data){
	return nodeid;
}

bool MultivariateNode::canEvaluate(IData* data) {
	for(vector<MultiDimExpression*>::iterator it = this->indices.begin(); it!=this->indices.end(); it++) {
		if(!(*it)->canEvaluate(data))
			return false;
	}
	return true;
}

string MultivariateNode::getCategoricalDistributionNodeName(IData* data) {
	string nodename = this->nodeid;
	if(this->indices.size()>0){
		nodename += "[";
		for(vector<MultiDimExpression*>::iterator it = this->indices.begin(); it!=this->indices.end(); it++){
			stringstream ss (stringstream::in | stringstream::out);
			if((*it)->empty || (*it)->colon)
				assert(false);
			else
				ss << (*it)->expbeforecolon->evalAsIndex(data);
			nodename = nodename + ss.str();
			if((it+1)!=this->indices.end())
				nodename += ",";
		}
		nodename += "]";
	}
	return nodename;
}
