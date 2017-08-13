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

#include "MainMemoryData.hpp"
#include <limits>
#include <cassert>

MainMemoryData::MainMemoryData(string filedetailsfolder) : IData(filedetailsfolder) {

}

MainMemoryData::~MainMemoryData() {
	for(std::map<std::string, BugsArray*>::iterator it=array.begin(); it!=array.end(); it++)
		delete it->second;
}

double MainMemoryData::getData(std::string variablename, const std::vector<int> &indices){
	if(getVariableType(variablename) == NOTFOUND)
	{
		cout << " Attention!!! [BUGSError] Data Not Found for " << variablename << endl ;
		assert(false);
	}
	if(indices.size() == 0){
		if(scalars.find(variablename)!=scalars.end())
			return scalars[variablename];
		else if(temporaryvalues.find(variablename) != temporaryvalues.end())
			return temporaryvalues[variablename];
		else
		{
			cout << "Attention!!! [BUGSError] Variable " << variablename << " not found in data " << endl;
			assert(false);
		}
	}

	if(indices.size() == 1)
	{
		if(indices[0] > vectors[variablename].size())
		{
			cout << "Attention!!! [BUGSError] Invalid dimension exception for variable name " << variablename << " dimension " << indices[0] << endl;
			assert(false);
		}
		return vectors[variablename][indices[0]-1];
	}

	return array[variablename]->getData(indices);
}

int MainMemoryData::getTemporaryVariableData(std::string variablename) {
	assert(getVariableType(variablename) == TEMPORARY);
	return temporaryvalues[variablename];
}

bool MainMemoryData::contains(std::string variablename, const vector<int> &indices){
	if(scalars.find(variablename)!=scalars.end())
		return true;
	else if(temporaryvalues.find(variablename)!=temporaryvalues.end())
		return true;
	else if(vectors.find(variablename)!=vectors.end() && indices.size() > 0)
	{
		auto naloc = NAlocations.find(variablename);
		if(naloc == NAlocations.end() || (naloc->second).find(indices[0]-1) == (naloc->second).end()) // variable name itself not present or index not present
			return true;
	}
	else if(array.find(variablename)!=array.end())
		return array[variablename]->contains(indices);
	return false;
}

DataType MainMemoryData::getVariableType(std::string variablename){
	if(scalars.find(variablename)!=scalars.end())
		return SCALAR;
	else if(temporaryvalues.find(variablename)!=temporaryvalues.end())
		return TEMPORARY;
	else if(vectors.find(variablename)!=vectors.end())
		return VECTOR;
	else if(array.find(variablename)!=array.end())
		return ARRAY;
	return NOTFOUND;
}

int MainMemoryData::getDimension(std::string variablename, int index) {
	if(vectors.find(variablename)!=vectors.end() && index == 0)
		return vectors[variablename].size();
	else if(array.find(variablename)!=array.end())
		return array[variablename]->getDimension(index);
	return 0;
}

void MainMemoryData::put(std::string variablename, int value){
	cout << "Incrementing " << variablename << " " << value << endl;
	temporaryvalues[variablename] = value;
}

double MainMemoryData::getLowerBound(std::string variablename){
	DataType type = this->getVariableType(variablename);
	if(type == SCALAR)
		return scalars[variablename];
	else if(type == TEMPORARY)
		return temporaryvalues[variablename];
	else if(type==VECTOR){
		double min = std::numeric_limits<double>::max();
		for (int i = 0; i < vectors[variablename].size() ; i++){
			auto naloc = NAlocations.find(variablename);
			bool isnotNA = (naloc == NAlocations.end() || (naloc->second).find(i) == (naloc->second).end());
			if(isnotNA && vectors[variablename][i] < min)
				min = vectors[variablename][i];
		}
	return min;
	}
	else if(type==ARRAY)
		return array[variablename]->getMin();
	else
	{
		cout << "Attention!!! [BUGSError] LowerBound Not Found for Variable [Name: " << variablename << "]" << endl;
		assert(false);
	}
}

double MainMemoryData::getUpperBound(std::string variablename){
	if(upperboundcache.find(variablename) != upperboundcache.end())
		return upperboundcache[variablename];

	DataType type = this->getVariableType(variablename);

	if(type == SCALAR)
	{
		double max = scalars[variablename];
		upperboundcache[variablename] = max;
		return max;
	}
	else if(type == TEMPORARY)
	{
		double max = temporaryvalues[variablename];
		upperboundcache[variablename] = max;
		return max;
	}
	else if(type==VECTOR){
		double max = std::numeric_limits<double>::min();

		for (int i = 0; i < vectors[variablename].size() ; i++){
			auto naloc = NAlocations.find(variablename);
			bool isnotNA = (naloc == NAlocations.end() || (naloc->second).find(i) == (naloc->second).end());
			if(isnotNA && vectors[variablename][i] > max)
				max = vectors[variablename][i];
		}

		upperboundcache[variablename] = max;
	return max;
	}
	else if(type==ARRAY)
	{
		double max = array[variablename]->getMax();
		upperboundcache[variablename] = max;
		return max;
	}
	else
	{
		cout << "Attention!!! [BUGSError] UpperBound Not Found for Variable [Name: " << variablename << "]" << endl;
		assert(false);
	}
}
