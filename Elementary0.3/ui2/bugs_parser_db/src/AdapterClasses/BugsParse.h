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
//  Filename: BUGSParse.h
//  Project:  Elementary_0.2
//
//  Created by Shriram Sridharan on 1/17/13.

#include "../ParserClasses/BUGSParser.hpp"
#include "../ParserClasses/MyBUGSDataParser.hpp"
#include "../ParserClasses/MyBUGSDataLexer.hpp"
#include "../DataClasses/MainMemoryData.hpp"

namespace hazy {

bool _BUGSDEBUG_ = false;

Program* parseProgram(std::string model_file) {
	BUGS::BUGSLexerTraits::InputStreamType input(
			(ANTLR_UINT8*) model_file.c_str(), ANTLR_ENC_8BIT);
	BUGS::BUGSLexer lxr(&input);
	BUGS::BUGSParserTraits::TokenStreamType tstream(ANTLR_SIZE_HINT, lxr.get_tokSource());
	BUGS::BUGSParser psr(&tstream);
	cout << "******************** Begin Parsing Program ****************** " << endl;
	Program* program = psr.prog();
	std::list<IStatement*>::iterator stit;

	for (stit = program->statements.begin(); stit != program->statements.end(); stit++) {
		cout << (*stit)->toString();
	}

	cout << "******************** End Parsing Program ****************** " << endl << endl;
	return program;
}

void testData(IData *mmdata) {
	map<std::string, double> scalars = ((MainMemoryData*) (mmdata))->scalars;
	vector<int> indices;
	for (std::map<std::string,double>::iterator it = scalars.begin(); it != scalars.end(); ++it) {
		cout << "value of " << (*it).first << "=>" << (*it).second << "=>" 	<< mmdata->getData((*it).first, indices) << endl;
	}
//	std::map<std::string, deque<double> > vectors = ((MainMemoryData*) (mmdata))->vectors;
//	for (std::map<std::string, deque<double> >::iterator it = vectors.begin(); it != vectors.end(); ++it) {
//		cout << "value of " << (*it).first;
//		for (std::deque<double>::iterator f = (*it).second.begin(); f!= (*it).second.end(); ++f) {
//			cout << "=>" << *f;
//		}
//	}
//
//	std::map<std::string, unordered_set<int> > NAlocations = ((MainMemoryData*) (mmdata))->NAlocations;
//		for (std::map<std::string, unordered_set<int> >::iterator it = NAlocations.begin(); it != NAlocations.end(); ++it) {
//			cout << "NAlocations of " << (*it).first;
//			for (std::unordered_set<int>::iterator f = (*it).second.begin(); f!= (*it).second.end(); ++f) {
//				cout << "=>" << *f;
//			}
//			cout << endl;
//		}
//
//
//	std::map<std::string, BugsArray*> array = ((MainMemoryData*) (mmdata))->array;
//	for (std::map<std::string, BugsArray*>::iterator it = array.begin(); it	!= array.end(); ++it) {
//		cout << "value of " << (*it).first << endl;
//		for(std::deque<double>::iterator at = (*it).second->data.begin(); at!=(*it).second->data.end(); at++)
//			cout << (*at) << "=>";
//		cout << endl << " Dimensions " ;
//		for(std::deque<double>::iterator at = (*it).second->dimensions.begin(); at!=(*it).second->dimensions.end(); at++)
//			cout << (*at) << "=>";
//	}
}

void parseData(MainMemoryData* mmdata, std::string data_file) {
	cout << "******************** Begin Parsing Data ****************** " << endl;
	MyBUGSDataLexer lexer(data_file);
	lexer.lex();

	MyBUGSDataParser parser(mmdata, lexer.tokens, lexer.nodenames, lexer.constants);
	parser.parse();

//	testData(mmdata);

	cout << "******************** End Parsing Data ****************** " << endl << endl;
}
}
