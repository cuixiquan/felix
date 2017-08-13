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

#ifndef MYBUGSDATAPARSER_HPP_
#define MYBUGSDATAPARSER_HPP_
#include <map>
#include <string>
#include <deque>
#include <iostream>
#include <unordered_set>

#include "MyBUGSDataLexer.hpp"
#include "../DataClasses/BugsArray.hpp"
#include "../DataClasses/MainMemoryData.hpp"

using namespace std;

class MyBUGSDataParser{
public:
	MainMemoryData* mmdata;
	deque<TokenType> tokens;
    deque<string> nodenames;
    deque<double> constants;
	int tokencounter;
	int nodenamecounter;
	int constantcounter;

	MyBUGSDataParser(MainMemoryData* mmdata, deque<TokenType> &tokens, deque<string> &nodenames, deque<double> &constants)
	{
		this->mmdata = mmdata;
		this->tokens = tokens;
		this->nodenames = nodenames;
		this->constants = constants;
		this->tokencounter = 0;
		this->nodenamecounter = 0;
		this->constantcounter = 0;
	}

	~MyBUGSDataParser()
	{
		deque<string>().swap(nodenames);
		deque<TokenType>().swap(tokens);
		deque<double>().swap(constants);
	}

	void handleScalar(string currentnodename)
    {
        mmdata->scalars[currentnodename] = constants[constantcounter];
        constantcounter++;
        tokencounter++;
        assert(tokens[tokencounter] == PARSER_COMMA || tokens[tokencounter] == PARSER_CLOSEBRACKET);
        tokencounter++;
        cout << " Scalar: " << currentnodename << " " <<  mmdata->scalars[currentnodename] << endl;
    }

    void handleVector(string currentnodename, deque<double> &vectordata, unordered_set<int> &nalocations)
    {
        cout << " Vector " << currentnodename << endl;
        int naloc = 0;
        tokencounter++;
        while(tokens[tokencounter] != PARSER_CLOSEBRACKET && tokencounter < tokens.size())
		{
			if(tokens[tokencounter] == PARSER_NA)
			{
				vectordata.push_back(0);
				nalocations.insert(naloc);
				naloc++;
				tokencounter++;
			}
			else if(tokens[tokencounter] == PARSER_CONSTANT)
			{
				vectordata.push_back(constants[constantcounter]);
				constantcounter++;
				naloc++;
				tokencounter++;
			}
			else if(tokens[tokencounter] == PARSER_COMMA && tokencounter < tokens.size()
					&& tokens[tokencounter+1]!=PARSER_CLOSEBRACKET)
			{
				tokencounter++;
			}
			else
			{
				cout << " Unexpected Token in Vector: " << currentnodename << endl;
				assert(false);
			}
		}

        assert(tokens[tokencounter] == PARSER_CLOSEBRACKET);
        tokencounter++;

        if(tokencounter < tokens.size() && tokens[tokencounter] == PARSER_COMMA)
        	tokencounter++;

    }

    void handleStructure(string currentnodename)
    {
        cout << " Structure " << currentnodename << endl;
        assert(tokens[++tokencounter] == PARSER_DOT);
        assert(tokens[++tokencounter] == PARSER_DATA);
        assert(tokens[++tokencounter] == PARSER_EQUALTO);
        assert(tokens[++tokencounter] == PARSER_VECTORBEGIN);

        BugsArray *bugsarray = new BugsArray();
        mmdata->array[currentnodename] = bugsarray;
        handleVector(currentnodename, bugsarray->data, bugsarray->NAlocations);

        assert(tokens[tokencounter] == PARSER_DOT);
        assert(tokens[++tokencounter] == PARSER_DIM);
        assert(tokens[++tokencounter] == PARSER_EQUALTO);
        assert(tokens[++tokencounter] == PARSER_VECTORBEGIN);

        unordered_set<int> dummy;
        handleVector(currentnodename, bugsarray->dimensions, dummy);
        assert(tokens[tokencounter] == PARSER_CLOSEBRACKET);
        tokencounter++;

        if(tokencounter < tokens.size() && tokens[tokencounter] == PARSER_COMMA)
            tokencounter++;
    }

    void parse()
    {
        assert(tokens[tokencounter++] == PARSER_LIST);
        while(tokencounter < tokens.size()) {
			string currentnodename = "";
			if(tokens[tokencounter] == PARSER_NODENAME) {
				assert(nodenamecounter < nodenames.size());
				assert(constantcounter < constants.size());

				currentnodename = nodenames[nodenamecounter];
				nodenamecounter++;
				tokencounter++;
			}
			else if(tokencounter == tokens.size() - 1 && tokens[tokencounter] == PARSER_CLOSEBRACKET)
			{
				cout << " Successfully Parsed Data " << endl;
				break;
			}
			else {
				cout << " Expected Nodename but was " << tokens[tokencounter] << endl;
				assert(false);
			}

			assert(tokens[tokencounter++] == PARSER_EQUALTO);

			if(tokens[tokencounter] == PARSER_CONSTANT)
				handleScalar(currentnodename);
			else if(tokens[tokencounter] == PARSER_VECTORBEGIN)
				handleVector(currentnodename, mmdata->vectors[currentnodename], mmdata->NAlocations[currentnodename]);
			else if(tokens[tokencounter] == PARSER_STRUCTUREBEGIN)
				handleStructure(currentnodename);
			else {
				cout << " Unexpected Token after EQUALTO " << endl;
				assert(false);
			}
		}
	}
};

#endif
