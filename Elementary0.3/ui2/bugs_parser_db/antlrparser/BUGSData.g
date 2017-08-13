grammar BUGSData;

options {language = Cpp;}

tokens {
	LIST = 'list(';
	EQUALTO = '=';
	CLOSEBRACKET =')';
	COMMA = ',';
	VECTORBEGIN = 'c(';
	STRUCTUREBEGIN = 'structure(';
	DOT='.';
	DATA ='Data';
	DIM = 'Dim';
	NA ='NA';
}

@lexer::namespace {BUGSData}
@parser::namespace {BUGSData}
@lexer::traits {

     class BUGSDataLexer; class BUGSDataParser;

     typedef antlr3::Traits< BUGSDataLexer, BUGSDataParser > BUGSDataLexerTraits;

     typedef BUGSDataLexerTraits BUGSDataParserTraits;

}
@lexer::includes{
#include <iostream>
using namespace std;
}
@parser::includes{
#include "BUGSDataLexer.hpp"
#include "../DataClasses/IData.hpp"
#include "../DataClasses/MainMemoryData.hpp"
}

data[MainMemoryData* mmdata]
	: LIST datatype[mmdata] (COMMA datatype[mmdata])* CLOSEBRACKET 
	;
	
datatype[MainMemoryData* mmdata] 	
	: scalar {mmdata->scalars[$scalar.nodename] = $scalar.value;}
	| vector {mmdata->NAlocations[$vector.nodename] = $vector.NAlocations; mmdata->vectors[$vector.nodename] = $vector.value;}
	| array  
	{ BugsArray* bugsarray = new BugsArray();
	bugsarray->data=$array.value;
	bugsarray->NAlocations =$array.NAlocations;
	bugsarray->dimensions=$array.dimensions;
	mmdata->array[$array.nodename]= bugsarray; }
	;
	
array	returns [std::string nodename, std::deque<double> value, std::unordered_set<int> NAlocations, std::deque<double> dimensions]
	: NODENAME EQUALTO STRUCTUREBEGIN DOT DATA EQUALTO v1=vectordata COMMA 
	DOT DIM EQUALTO v2=vectordata CLOSEBRACKET
	{$nodename = $NODENAME.text; $value=$v1.value; $NAlocations=$v1.NAlocations; $dimensions=$v2.value;}
	;

vector	returns [std::string nodename, std::deque<double> value, std::unordered_set<int> NAlocations]
	: NODENAME EQUALTO vectordata {$nodename = $NODENAME.text; $value = $vectordata.value; $NAlocations = $vectordata.NAlocations;}
	;

vectordata returns [std::deque<double> value, std::unordered_set<int> NAlocations]
scope {
int variablecount;
}
	: VECTORBEGIN {$vectordata::variablecount = 0; }
		(cv1=NA 
			{ $NAlocations.insert($vectordata::variablecount); 
			$value.push_back(0); }
		 | cv1=CONSTANTVALUE 
		 	{$value.push_back(::atof($cv1.text.c_str())); }) 
	(COMMA {$vectordata::variablecount++; } 
		(cv2=NA
			 { $NAlocations.insert($vectordata::variablecount); 
			 $value.push_back(0); } 
		| cv2=CONSTANTVALUE 
			{$value.push_back(::atof($cv2.text.c_str())); })
	)*  CLOSEBRACKET
	;
	

scalar	returns [std::string nodename, double value]
	: NODENAME EQUALTO 
		(cv1=NA {std::cout << " Scalars cannot have NA as value" << std::endl; assert(false);}
		| cv1=CONSTANTVALUE {$nodename = $NODENAME.text; $value=::atof($cv1.text.c_str());}) 
	;

CONSTANTVALUE 
	:  ('+'|'-')? ('0'..'9')+(DOT('0'..'9')+)?('E'('+'|'-')?('0'..'9')+)? 
	|  ('+'|'-')? (DOT('0'..'9')+)('E'('+'|'-')?('0'..'9')+)? 
	;
	
NODENAME 
	:('a'..'z'| 'A'..'Z') ('a'..'z'| 'A'..'Z'| '0'..'9'| '.')* 
	;

WHITESPACE : ( '\t' | ' ' | '\r' | '\n'| ';' | '\u000C' )+    { $channel = HIDDEN; } ;

