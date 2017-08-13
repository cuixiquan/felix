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

#include "UnivariateNode.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <cassert>
using namespace std;

UnivariateNode::UnivariateNode() {
	this->isunaryminus = false;
}

UnivariateNode::UnivariateNode(UnivariateNode* uvnode) {
	this->nodeid = uvnode->nodeid;
	this->isunaryminus = uvnode->isunaryminus;
	for(vector<Expression* >::iterator it= uvnode->indices.begin(); it!= uvnode->indices.end(); ++it)
		this->indices.push_back(new Expression(*it));
}

UnivariateNode::~UnivariateNode() {
	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		delete *it;
	}
}

string UnivariateNode::toString(){
	string retval = "";
	retval = retval + this->nodeid + " ";
	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		retval = retval + (*it)->toString() ;
	}
	return retval;
}

bool UnivariateNode::canEvaluate(IData* data) {

	string nodename = this->nodeid;
	vector<int> parameters;

	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		if((*it)->canEvaluate(data) == false)
			return false;
		parameters.push_back((*it)->evalAsIndex(data));
	}

	bool datacontains = data->contains(nodename, parameters);
	vector<int>().swap(parameters);

	if(datacontains)
		return true;

	return false;
}

double UnivariateNode::evalAsData(IData* data){
	string nodename = this->nodeid;
	vector<int> parameters;
	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		parameters.push_back((*it)->evalAsIndex(data));
	}

	double datatoreturn = 0;
	if(this->isunaryminus)
		datatoreturn =  -1 * data->getData(nodename, parameters);
	else
		datatoreturn = data->getData(nodename, parameters);

	vector<int>().swap(parameters);
	return datatoreturn;
}

int UnivariateNode::evalAsIndex(IData* data){
	string nodename = this->nodeid;
	vector<int> parameters;
	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		parameters.push_back((*it)->evalAsIndex(data));
	}

	int datatoreturn = 0;
	if(this->isunaryminus)
		datatoreturn =  -1 * data->getData(nodename, parameters);
	else
		datatoreturn = data->getData(nodename, parameters);

	vector<int>().swap(parameters);
	return datatoreturn;
}

string UnivariateNode::getNodeid(IData* data){
	string nodename = this->nodeid;
	if(this->indices.size()>0){
		nodename += "[";
		for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
			stringstream ss (stringstream::in | stringstream::out);
			if((*it)->type == EMPTY)
				nodename = nodename + " ";
			else
				ss << (*it)->evalAsIndex(data);
			nodename = nodename + ss.str();
			if((it+1)!=this->indices.end())
				nodename += ",";
		}
		nodename += "]";
	}
	return nodename;
}

double UnivariateNode::getLowerBound(IData* data){
	string nodename = this->nodeid;
	return data->getLowerBound(nodename);
}

double UnivariateNode::getUpperBound(IData* data){
	string nodename = this->nodeid;
	return data->getUpperBound(nodename);
}

bool UnivariateNode::containsData(IData* data){
	if(!this->canEvaluate(data))
		return false;

	vector<int> parameters;
	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it){
		if((*it)->type == EMPTY)
		{
			cout << " Attention!!! [BugsError] Univariate Node [Name: " << this->nodeid << "] contains an empty index " << endl;
			assert(false);
		}
		parameters.push_back((*it)->evalAsIndex(data));
	}
	bool contains = data->contains(this->nodeid, parameters);
	vector<int>().swap(parameters);
	return contains;
}

NodeNameMatrix* UnivariateNode::evalAsNodeNameMatrix(IData* data){
	NodeNameMatrix* nodenamematrix = new NodeNameMatrix();
	nodenamematrix->nodename = this->nodeid;
	nodenamematrix->isunaryminus = this->isunaryminus;
	nodenamematrix->type = NNM_UNIVARIATE;

	// Evaluating nodes if present in data.
	if(this->indices.empty() && data->contains(nodenamematrix->nodename, vector<int>()))
	{
		if(data->getVariableType(nodenamematrix->nodename) == TEMPORARY)
			nodenamematrix->value = data->getTemporaryVariableData(nodenamematrix->nodename);
		else
			nodenamematrix->value = data->getData(nodenamematrix->nodename, vector<int>());

		cout << " NNM va " << nodenamematrix->value << endl;
		if(nodenamematrix->isunaryminus)
			nodenamematrix->value *= -1;
		nodenamematrix->type = NNM_UNIVARIATECONSTANT; // Handle as NNM_UNC
		return nodenamematrix;
	}

	for(vector<Expression*>::iterator it=this->indices.begin(); it!=this->indices.end(); ++it)
	{
		NodeNameMatrix* nodenamematrixindex = new NodeNameMatrix();
		if((*it)->type == EMPTY)
			nodenamematrixindex->type = NNM_EMPTY;
		else if((*it)->canEvaluate(data)) {
			nodenamematrixindex->type = NNM_CONSTANT;
			nodenamematrixindex->value = (*it)->evalAsIndex(data);
		}
		else {
			NodeNameMatrix* nnm = (*it)->evalDistParams(data);
			// TODO Handling only the scenario where the index is a node. Using the nodeid because the assumption is that
			// there will be no node index of the node.
			nodenamematrixindex->nodename = nnm->getNodeid();
			nodenamematrixindex->type = NNM_NODE;
			delete nnm;
		}
		nodenamematrix->indices.push_back(nodenamematrixindex);
	}

	return nodenamematrix;
}

