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


#ifndef MAINMEMORYDATA_HPP_
#define MAINMEMORYDATA_HPP_
#include "IData.hpp"
#include "BugsArray.hpp"
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <deque>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class MainMemoryData : public IData {
public:
	MainMemoryData(string filedetailsfolder);
	virtual ~MainMemoryData();

	/*
	 * Semantic: [Done to handle scale]
	 * 1. scalars vector contain all the scalars. Assuming Scalars will not have "NA" as their value
	 * 2. vectors vector contains all the vectors. If any value is "NA", then vectors contains 0 as the value
	 * 3. NAlocations contains the locations where the particular vector has "NA" as the value
	 * 4. Similar semantic as vectors is used for array assuming that the dimensions will never be "NA"
	 */
	std::map<std::string, double> scalars;
	std::map<std::string, deque<double> > vectors;
	std::map<std::string, unordered_set<int> > NAlocations;
	std::map<std::string, BugsArray* > array;
	std::map<std::string, int > temporaryvalues;
	std::unordered_map<std::string, double> upperboundcache;

	double getData(std::string variablename, const vector<int> &indices);
	bool contains(std::string variablename, const vector<int> &indices);
	DataType getVariableType(std::string variablename);
	int getDimension(std::string variablename, int index);
	void put(std::string variablename, int value);
	double getLowerBound(std::string variablename);
	double getUpperBound(std::string variablename);
	int getTemporaryVariableData(std::string variablename);
};

#endif /* MAINMEMORYDATA_HPP_ */
