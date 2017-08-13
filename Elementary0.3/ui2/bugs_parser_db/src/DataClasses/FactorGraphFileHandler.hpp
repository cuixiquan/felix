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

#ifndef FACTORGRAPHFILEHANDLER_HPP_
#define FACTORGRAPHFILEHANDLER_HPP_

#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <float.h>
#include <cstdio>
using namespace std;

typedef long long int Long;

class FactorGraphFileHandler {
private:
	ofstream createfile;
	fstream factorfile;
	fstream variablefile;
	fstream groupfile;
	fstream edgefile;
	fstream boundsfile;
	fstream tvariablesfile;
	Long globaltvariableid;

public:
	Long globalfactorid;

	FactorGraphFileHandler(string tempfolder) {
		if(tempfolder == "")
			return;

		createfile.open(tempfolder + "factors.csv");
		createfile.close();

		factorfile.open(tempfolder + "factors.csv");
		if(!factorfile.is_open())
			assert(false);

		createfile.open(tempfolder + "variables.csv");
		createfile.close();

		variablefile.open(tempfolder + "variables.csv");
		if(!variablefile.is_open())
			assert(false);

		createfile.open(tempfolder + "groups.csv");
		createfile.close();

		groupfile.open(tempfolder + "groups.csv");
		if(!groupfile.is_open())
			assert(false);

		createfile.open(tempfolder + "edges.csv");
		createfile.close();

		edgefile.open(tempfolder + "edges.csv");
		if(!edgefile.is_open())
			assert(false);

		createfile.open(tempfolder + "bounds.csv");
		createfile.close();

		boundsfile.open(tempfolder + "bounds.csv");
		if(!boundsfile.is_open())
			assert(false);

		createfile.open(tempfolder + "tvariables.csv");
		createfile.close();

		tvariablesfile.open(tempfolder + "tvariables.csv");
		if(!tvariablesfile.is_open())
			assert(false);

		globalfactorid = 0;
		globaltvariableid = 0;
	}

	void deleteFiles(string tempfolder){
		remove((tempfolder + "factors.csv").c_str());
		remove((tempfolder + "variables.csv").c_str());
		remove((tempfolder + "groups.csv").c_str());
		remove((tempfolder + "edges.csv").c_str());
		remove((tempfolder + "bounds.csv").c_str());
		remove((tempfolder + "tvariables.csv").c_str());
	}

	~FactorGraphFileHandler() {
		flushAndCloseFiles();
	}

	Long handletvariables(string variablename) {
		Long returnvariableid = globaltvariableid;
		tvariablesfile << globaltvariableid << ";" << variablename << ";\n";
		globaltvariableid++;
		return returnvariableid;
	}

	//nvars is not inserted. Will be handled in Stored Proc after Replacing Stochastic Node dist param with Logical Node (if necessary)
	void handleFactor(Long factorid, int pid, int factoropttype, string distname, int ngroups) {
		factorfile << factorid << ";" << pid << ";" << factoropttype << ";";
		factorfile << distname << ";" << ngroups << ";\n";
		globalfactorid++;
	}

	void handleConstantVariable(Long gvid, int type, int dtype, double cvalue){
		variablefile << gvid << ";" << type << ";" << dtype << ";" << cvalue << ";" << cvalue << ";" << cvalue << ";\n";
	}

	void handleNodeVariable(Long gvid, int type, int dtype, double cvalue, double lowerbound, double upperbound){
		variablefile << gvid << ";" << type << ";" << dtype << ";" << cvalue << ";" ;

		if(lowerbound == -DBL_MAX)
			variablefile << ";";
		else
			variablefile << lowerbound << ";" ;

		if(upperbound == DBL_MAX)
			variablefile << ";\n";
		else
			variablefile << upperbound << ";\n";
	}

	//nvars is not inserted.
	void handleGroup(Long factorid, int groupid, double aux) {
		groupfile << factorid << ";" << groupid << ";" << aux << ";\n";
	}

	void handleEdge(Long factorid, Long gvid, int groupid, int aux, bool is_vg) {
		edgefile << gvid << ";" << factorid << ";" << groupid << ";" << aux << ";" << is_vg << "\n";
	}

	/*
	 * Bounds funda is as follows:
	 * 1. Add all variable names whose upper,lowerbound, lbvarname, ubvarname into bounds table.
	 * 2. If min(UB) is null, then check recursively for the min(UB) for the ubvarname
	 * 3. If max(LB) is null, then check recursively for the max(LB) for the lbvarname
	 */
	void handleBounds(Long gvid, double lowerbound, double upperbound, Long lgvid, Long ugvid) {
		boundsfile << gvid << ";";

		if(lowerbound == -DBL_MAX)
			boundsfile << ";";
		else
			boundsfile << lowerbound << ";" ;

		if(upperbound == DBL_MAX)
			boundsfile << ";";
		else
			boundsfile << upperbound << ";" ;

		if(lgvid != -1)
			boundsfile << lgvid << ";";
		else
			boundsfile << ";";

		if(ugvid != -1)
			boundsfile << ugvid << "\n";
		else
			boundsfile << "\n";
	}

	void flushAndCloseFiles(){
		if(factorfile.is_open())
			factorfile.close();

		if(variablefile.is_open())
			variablefile.close();

		if(groupfile.is_open())
			groupfile.close();

		if(edgefile.is_open())
			edgefile.close();

		if(boundsfile.is_open())
			boundsfile.close();

		if(tvariablesfile.is_open())
			tvariablesfile.close();
	}
};
#endif /* FACTORGRAPHFILEHANDLER_HPP_ */
