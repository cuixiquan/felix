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

#ifndef IDATA_HPP_
#define IDATA_HPP_
#include <string>
#include <vector>
#include "ParserFileHandler.hpp"
#include "FactorGraphFileHandler.hpp"

enum DataType {NOTFOUND, SCALAR, TEMPORARY, VECTOR, ARRAY};

class IData {
	public:
		ParserFileHandler* parserfiledetails;
		FactorGraphFileHandler* fgfiledetails;
		IData(string filedetailsfolder) {
			this->parserfiledetails = new ParserFileHandler(filedetailsfolder);
			this->fgfiledetails = new FactorGraphFileHandler(filedetailsfolder);
		}

		~IData() {
			delete parserfiledetails;
			delete fgfiledetails;
		};

		virtual double getData(std::string variablename, const std::vector<int> &indices) = 0;
		virtual bool contains(std::string variablename, const std::vector<int> &indices) = 0;
		virtual DataType getVariableType(std::string variablename) = 0;
		virtual int getDimension(std::string variablename, int index) = 0;
		virtual void put(std::string variablename, int value) = 0;
		virtual double getLowerBound(std::string variablename) = 0;
		virtual double getUpperBound(std::string variablename) = 0;
		virtual int getTemporaryVariableData(std::string variablename) = 0;
};

#endif /* IDATA_HPP_ */
