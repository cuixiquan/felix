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


#ifndef PARSERFILEHANDLER_HPP_
#define PARSERFILEHANDLER_HPP_

#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <iostream>
using namespace std;

typedef long long int Long;

class ParserFileHandler {
private:
	ofstream createfile;
	fstream nodefile;
	fstream nodenamematrixfile;
	fstream limitationfile;

public:
	Long nodenamematrixid;
	Long limitationid;

	ParserFileHandler(string tempfolder) {
		if(tempfolder == "")
			return;

		createfile.open(tempfolder + "node.csv");
		createfile.close();

		nodefile.open(tempfolder + "node.csv");
		if(!nodefile.is_open())
			assert(false);

		createfile.open(tempfolder + "nodenamematrix.csv");
		createfile.close();

		nodenamematrixfile.open(tempfolder + "nodenamematrix.csv");
		if(!nodenamematrixfile.is_open())
			assert(false);

		createfile.open(tempfolder + "limitation.csv");
		createfile.close();

		limitationfile.open(tempfolder + "limitation.csv");
		if(!limitationfile.is_open())
			assert(false);

		nodenamematrixid = 0;
		limitationid = 0;
	}

	~ParserFileHandler() {
		flushAndCloseFiles();
	}

	void flushAndCloseFiles(){
		if(nodefile.is_open())
			nodefile.close();

		if(nodenamematrixfile.is_open())
			nodenamematrixfile.close();

		if(limitationfile.is_open())
			limitationfile.close();
	}

	void deleteFiles(string tempfolder){
		remove((tempfolder + "node.csv").c_str());
		remove((tempfolder + "nodenamematrix.csv").c_str());
		remove((tempfolder + "limitation.csv").c_str());
	}

	void handleNodeFile(Long nodeid, Long e_l_id, vector<Long> &distparamsid, int nodetype, string distname){
		nodefile << nodeid << ";";
		if(e_l_id != -1)
			nodefile << e_l_id << ";";
		else
			nodefile << ";";

		string distparampostgresarray = "";
		for(int i = 0; i < distparamsid.size(); i++)
		{
			std::ostringstream buff;
			buff << distparamsid[i];
			if(i != distparamsid.size() - 1)
				buff << ",";
			distparampostgresarray.append(buff.str());
		}

		if(distparampostgresarray.size() > 0)
			nodefile << "{" << distparampostgresarray << "};";
		else
			nodefile << ";";
		nodefile << nodetype << ";" ;
		nodefile << distname << "\n" ;
	}

	void handleLimitationFile(Long id, string limitationtype, string lowerbound, string upperbound){
		limitationfile << id << ";";
		limitationfile << limitationtype << ";";
		limitationfile << lowerbound << ";" ;
		limitationfile << upperbound << "\n" ;
		limitationid++;
	}

	void handleNodeNameMatrixFile(Long id, string nodename, int variabletype) {
		nodenamematrixfile << id << ";";
		nodenamematrixfile << nodename << ";";
		nodenamematrixfile << ";;false;";
		nodenamematrixfile << variabletype << "\n";
		nodenamematrixid++;
	}

	void handleNodeNameMatrixFileWithValue(Long id, string nodename, double value, int variabletype, bool isindex){
		nodenamematrixfile << id << ";";
		nodenamematrixfile << nodename << ";";
		nodenamematrixfile << ";";
		if(isindex)
			nodenamematrixfile << (int)(value) << ";false;";
		else
			nodenamematrixfile << value << ";false;";
		nodenamematrixfile << variabletype << "\n";
		nodenamematrixid++;
	}

	void handleNodeNameMatrixFileWithIndexIds(Long id, string nodename, vector<Long> &indexids, int variabletype){
		nodenamematrixfile << id << ";";
		nodenamematrixfile << nodename << ";";

		string indexidspostgresarray = "";
		for(int i = 0; i < indexids.size(); i++)
		{
			std::ostringstream buff;
			buff << indexids[i];
			if(i != indexids.size() - 1)
				buff << ",";
			indexidspostgresarray.append(buff.str());
		}

		if(indexids.size() > 0)
			nodenamematrixfile << "{" << indexidspostgresarray << "};";
		else
			nodenamematrixfile << ";";
		nodenamematrixfile << ";false;";
		nodenamematrixfile << variabletype << "\n";
		nodenamematrixid++;
	}
};

#endif /* PARSERFILEHANDLER_HPP_ */
