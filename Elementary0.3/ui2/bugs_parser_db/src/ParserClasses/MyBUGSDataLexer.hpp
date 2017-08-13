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

#ifndef MYBUGSDATALEXER_HPP_
#define MYBUGSDATALEXER_HPP_
#include <iostream>
#include <string>
#include <deque>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#define CHARBUFFER 30
using namespace std;

enum TokenType {PARSER_LIST, PARSER_EQUALTO, PARSER_CLOSEBRACKET, PARSER_COMMA, PARSER_VECTORBEGIN, PARSER_STRUCTUREBEGIN,
	PARSER_DOT, PARSER_DATA, PARSER_DIM, PARSER_NA, PARSER_NODENAME, PARSER_CONSTANT};

class MyBUGSDataLexer {
public:
	string filename;
	int currentlocation;
	int temploc;
	ifstream file;
    deque<TokenType> tokens;

    deque<string> nodenames;
    deque<double> constants;

    deque<char> charstream;

	MyBUGSDataLexer(string filename){
		this->filename = filename;
		this->currentlocation = 0;
		this->temploc = currentlocation;
	}

	~MyBUGSDataLexer(){

	}

	bool reserve(int reserve){
//		cout << " reserve " << reserve + currentlocation << " " << charstream.size() << " " << reserve << endl;
		if(reserve + currentlocation <= charstream.size())
			return true;

    	if(reserve + currentlocation > CHARBUFFER - 1)
			return fillcharstream(file, charstream);

    	return true;
    }

    bool canHandleAsWhiteSpace()
    {
    	if(!reserve(1))
			return false;

    	char c = charstream[currentlocation];

        if(c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == ';' || c == '\f')
        {
		// 	cout << " WHITESPACE " <<endl;
			currentlocation = currentlocation + 1; // do nothing. Whitespace
			temploc = temploc + 1;
			return true;
        }
        return false;
    }

    bool canHandleAsEqualTo()
    {
    	if(!reserve(1))
			return false;

    	char c = charstream[currentlocation];
        if(c == '=') {
			//            	 cout << " EQUALTO " <<endl;
			tokens.push_back(PARSER_EQUALTO);
			currentlocation = currentlocation + 1;
			temploc = temploc + 1;
			return true;
        }

        return false;
    }

    bool canHandleAsCloseBracket()
    {
    	if(!reserve(1))
			return false;

    	char c = charstream[currentlocation];
        if(c == ')') {
			//            	 cout << " CLOSEBRACKET " <<endl;
			tokens.push_back(PARSER_CLOSEBRACKET);
			currentlocation = currentlocation + 1;
			temploc = temploc + 1;
			return true;
        }
        return false;
    }

    bool canHandleAsComma()
    {
    	if(!reserve(1))
			return false;

    	char c = charstream[currentlocation];
        if(c ==',') {
			//            	 cout << " COMMA " <<endl;
			tokens.push_back(PARSER_COMMA);
			currentlocation = currentlocation + 1;
			temploc = temploc + 1;
			return true;
        }

        return false;
    }

    bool canHandleAsDot()
    {
    	if(!reserve(1))
			return false;

    	char c = charstream[currentlocation];
        if(c == '.') {
			//            	 cout << " DOT " <<endl;
			tokens.push_back(PARSER_DOT);
			currentlocation = currentlocation + 1;
			temploc = temploc + 1;
			return true;
        }

        return false;
    }

    bool fillcharstream(ifstream & file, deque<char> & charstream)
    {
    	if(file.eof())
    		return false;

    	int i = 0;
    	while(i < currentlocation) {
    		charstream.pop_front();
    		i++;
    	}

    	i = 0;
        while(i < CHARBUFFER) {
        	char c = file.get();
        	if(file.eof())
        		break;
			charstream.push_back(c);
			i++;
        }

        currentlocation = 0;
        return true;
    }

    bool canHandleAsList()
    {
    	if(!reserve(5))
    		return false;

        if(charstream[currentlocation] == 'l' && charstream[currentlocation + 1] == 'i' &&
        		charstream[currentlocation + 2] == 's' && charstream[currentlocation + 3] == 't'
        				&& charstream[currentlocation + 4] == '(') {

	//				cout << " LIST " <<endl;
			tokens.push_back(PARSER_LIST);
			currentlocation = currentlocation + 5;
			temploc = temploc + 5;
			return true;
        }

        return false;
    }

    bool canHandleAsStructure()
    {
    	if(!reserve(10))
    		return false;

        if( charstream[currentlocation] == 's' && charstream[currentlocation + 1] == 't' &&
        	   charstream[currentlocation + 2] == 'r' && charstream[currentlocation + 3] == 'u' &&
        	   charstream[currentlocation + 4] == 'c' && charstream[currentlocation + 5] == 't' &&
        	   charstream[currentlocation + 6] == 'u' && charstream[currentlocation + 7] == 'r' &&
        	   charstream[currentlocation + 8] == 'e' && charstream[currentlocation + 9] == '('
        	   ) {
			//				cout << " STRUCTURE " <<endl;
			tokens.push_back(PARSER_STRUCTUREBEGIN);
			currentlocation = currentlocation + 10;
			temploc = temploc + 10;
			return true;
        }

        return false;
    }

    bool canHandleAsVector()
    {
    	if(!reserve(2))
    		return false;

        if(charstream[currentlocation] == 'c' && charstream[currentlocation + 1] == '(') {
			//				cout << " VECTOR " <<endl;
			tokens.push_back(PARSER_VECTORBEGIN);
			currentlocation = currentlocation + 2;
			temploc = temploc + 2;
			return true;
        }

        return false;
    }

    bool canHandleAsData()
    {
    	if(!reserve(4))
    		return false;

        if(charstream[currentlocation] == 'D' && charstream[currentlocation + 1] == 'a' &&
        	   charstream[currentlocation + 2] == 't' && charstream[currentlocation + 3] == 'a') {
//			cout << " DATA " <<endl;
			tokens.push_back(PARSER_DATA);
			currentlocation = currentlocation + 4;
			temploc = temploc + 4;
			return true;
        }

        return false;
    }

    bool canHandleAsDim()
    {
    	if(!reserve(3))
    		return false;

        if(charstream[currentlocation] == 'D' && charstream[currentlocation + 1] == 'i' &&
        	   charstream[currentlocation + 2] == 'm'){
//        	cout << " DIM " <<endl;
			tokens.push_back(PARSER_DIM);
			currentlocation = currentlocation + 3;
			temploc = temploc + 3;
			return true;
        }

        return false;
    }

    bool canHandleAsNA()
    {
    	if(!reserve(2))
    		return false;

        if(charstream[currentlocation] == 'N' && charstream[currentlocation + 1] == 'A') {
			//				cout << " NA " <<endl;
			tokens.push_back(PARSER_NA);
			currentlocation = currentlocation + 2;
			temploc = temploc + 2;
			return true;
        }

        return false;
    }

    bool canHandleAsNodeName()
    {
    	string nodename = "";
    	bool nomatch = false;
//    	cout << " Nodename " << currentlocation << " " << charstream[currentlocation] << " is eof " << file.eof() << endl;

    	while(!file.eof() || currentlocation < charstream.size()) {
    		if(!reserve(1) || nomatch)
    			break;
			switch (charstream[currentlocation]) {
			case '.':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
				{
					nodename = nodename + string(1, charstream[currentlocation]);
					currentlocation = currentlocation + 1;
					break;
				}
			default:
				{
					nomatch = true;
					break;
				}
			}
    	}

    	if(nodename == "")
    		return false;

    	nodenames.push_back(nodename);
    	tokens.push_back(PARSER_NODENAME);
    	temploc = temploc + nodename.size();

    	return true;
    }

    bool canHandleAsConstant()
	{
		string nodename = "";
		bool nomatch = false;
		bool potentialconstant = false;
//    	cout << " Nodename " << currentlocation << " " << charstream[currentlocation] << " is eof " << file.eof() << endl;

		if(!reserve(1))
			return false;

		switch (charstream[currentlocation]) {
		case '.':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '+':
		case '-':
			{
				potentialconstant = true;
				break;
			}
		default:
			break;
		}

		if(!potentialconstant)
			return false;

		while(!file.eof() || currentlocation < charstream.size()) {
			if(!reserve(1) || nomatch)
				break;
			switch (charstream[currentlocation]) {
			case '.':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'E':
			case '+':
			case '-':
				{
					nodename = nodename + string(1, charstream[currentlocation]);
					currentlocation = currentlocation + 1;
					break;
				}
			default:
				{
					nomatch = true;
					break;
				}
			}
		}

		if(nodename == "")
			return false;

		constants.push_back(::atof(nodename.c_str()));
		tokens.push_back(PARSER_CONSTANT);
		temploc = temploc + nodename.size();

		return true;
	}

    void lex()
    {
		clock_t begin = clock();

        file.open(filename.c_str());
        fillcharstream(file, charstream);
        do{
            if(canHandleAsWhiteSpace());
            else if(canHandleAsEqualTo());
            else if(canHandleAsCloseBracket());
            else if(canHandleAsComma());
            else if(canHandleAsDot());
			else if(canHandleAsList());
			else if(canHandleAsStructure());
			else if(canHandleAsVector());
			else if(canHandleAsData());
			else if(canHandleAsDim());
			else if(canHandleAsNA());
			else if(canHandleAsConstant());
			else if(canHandleAsNodeName());
			else if(!(charstream.size() == 0))
			{
				cout << " Error in Parsing Data at Location " << temploc + 1 << endl;
				cout << " Please check if you have named the variables as Data, Dim or NA as these are keywords " << endl;
				assert(false);
			}
		}while(!file.eof() || currentlocation < charstream.size());

//        cout << " Nodenames are " <<endl;
//        for(deque<string>::iterator it = nodenames.begin(); it != nodenames.end(); it++)
//        	cout << *it << endl;
//
//        cout << " Constants are " << endl;
//        for(deque<double>::iterator it = constants.begin(); it != constants.end(); it++)
//            cout << *it << endl;

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		cout << " Elapsed Secs " << elapsed_secs << endl;

		file.close();
	}
};

#endif
