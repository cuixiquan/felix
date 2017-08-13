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

#include "BugsArray.hpp"
#include <limits>
#include <cassert>

BugsArray::BugsArray() {
}

BugsArray::~BugsArray() {
}

bool BugsArray::checkDataAvailability(){
	int total = 1;
	for(int i=0; i< this->dimensions.size();i++) {
		total = total * this->dimensions[i];
	}

	if(data.size() == size_t(total))
		return true;

	return false;
}

bool BugsArray::contains(const std::vector<int> &indices){
	auto dt = this->dimensions.begin();
	auto it=indices.begin();
	int dataloc = 0;

	for(;it!=indices.end();++it,++dt){
		int loc = (*it);
		int dim = (*dt);
		if(loc > dim)
			throw "Invalid Dimension Exception";
		if(it+1 != indices.end())
		{
			// Index Manipulation
			std::deque<double>::iterator interdt = dt + 1;
			int otherindicesprod = 1;
			while(interdt != this->dimensions.end()) {
				otherindicesprod = otherindicesprod * (*interdt);
				interdt++;
			}
			dataloc += (loc - 1) * otherindicesprod;
		}
		else
			dataloc += loc - 1;
	}

	if(NAlocations.find(dataloc) != NAlocations.end())
		return false;
	else
		return true;
}

int BugsArray::getDimension(int index){
	return this->dimensions[index];
}

double BugsArray::getData(const std::vector<int> &location) {

	if(!checkDataAvailability())
		throw "Data from Model Incorrect";

	if(location.size()!=this->dimensions.size())
		throw "Invalid Dimension Exception";

	auto dt=this->dimensions.begin();
	auto it=location.begin();
	int dataloc = 0;
	for(;it!=location.end();++it,++dt){
		int loc = (*it);
		int dim = (*dt);
		if(loc > dim)
			throw "Invalid Dimension Exception";
		if(it+1 != location.end())
		{
			// Index Manipulation
			std::deque<double>::iterator interdt = dt + 1;
			int otherindicesprod = 1;
			while(interdt != this->dimensions.end()) {
				otherindicesprod = otherindicesprod * (*interdt);
				interdt++;
			}
			dataloc += (loc - 1) * otherindicesprod;
		}
		else
			dataloc += loc - 1;
	}

	return data[dataloc];
}

double BugsArray::getMin(){
	double min = std::numeric_limits<double>::max();
	for (int i = 0; i < data.size() ; i++){
		if(NAlocations.find(i) != NAlocations.end() && data[i] < min)
			min = data[i];
	}
	return min;
}

double BugsArray::getMax(){
	double max = std::numeric_limits<double>::min();
	for (int i = 0; i < data.size() ; i++){
		if(NAlocations.find(i) != NAlocations.end() && data[i] > max)
			max = data[i];
	}
	return max;
}
