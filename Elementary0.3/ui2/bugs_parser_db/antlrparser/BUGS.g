grammar BUGS ;

options {language = Cpp;}

tokens {
	MODEL = 'model';
	
	/* Scalar Functions*/
	LOGOPENBRACKET = 'log(';
	LOGITOPENBRACKET = 'logit(';
	CLOGLOGOPENBRACKET = 'cloglog(';
	PROBITOPENBRACKET = 'probit(';
	
	ABSOPENBRACKET = 'abs(';
	ARCCOSOPENBRACKET='arccos(';
	ARCCOSHOPENBRACKET='arccosh(';
	ARCSINOPENBRACKET='arcsin(';
	ARCSINHOPENBRACKET='arcsinh(';
	ARCTANOPENBRACKET='arctan(';
	ARCTANHOPENBRACKET='arctanh(';
	COSOPENBRACKET='cos(';
	COSHOPENBRACKET='cosh(';
	CUMULATIVEOPENBRACKET='cumulative(';
	CUTOPENBRACKET='cut(';
	DENSITYOPENBRACKET='density(';
	DEVIANCEOPENBRACKET='deviance(';
	EQUALSOPENBRACKET='equals(';
	EXPOPENBRACKET='exp(';
	GAMMAPOPENBRACKET='gammap(';
	ILOGITOPENBRACKET='ilogit(';
	ICLOGLOGOPENBRACKET='icloglog(';
	INTEGRALOPENBRACKET='integral(';
	LOGFACTOPENBRACKET='logfact(';
	LOGGAMOPENBRACKET='loggam(';
	MAXOPENBRACKET='max(';
	MINOPENBRACKET='min(';
	PHIOPENBRACKET='phi(';
	POSTPVALUEOPENBRACKET='post.p.value(';
	POWOPENBRACKET='pow(';
	PRIORPVALUEOPENBRACKET='prior.p.value(';
	REPLICATEPOSTOPENBRACKET='replicate.post(';
	REPLICATEPRIOROPENBRACKET='replicate.prior(';
	ROUNDOPENBRACKET='round(';
	SINOPENBRACKET='sin(';
	SINHOPENBRACKET='sinh(';
	SOLUTIONOPENBRACKET='solution(';
	SQRTOPENBRACKET='sqrt(';
	STEPOPENBRACKET='step(';
	TANOPENBRACKET='tan(';
	TANHOPENBRACKET='tanh(';
	TRUNCOPENBRACKET='trunc(';

	/* Vector Functions*/
	INVERSEOPENBRACKET = 'inverse(';
	MEANOPENBRACKET='mean(';
	SDOPENBRACKET='sd(';
	SUMOPENBRACKET='sum(';
	INPRODOPENBRACKET='inprod(';
	
	BERNOULLIOPENBRACKET = 'dbern(';
	BINOMIALOPENBRACKET='dbin(';
	CATEGORICALOPENBRACKET='dcat(';
	NEGATIVEBINOMIALOPENBRACKET='dnegbin(';
	POISSONOPENBRACKET='dpois(';
	NCHGOPENBRACKET='dhyper(';
	
	BETAOPENBRACKET = 'dbeta(';
	CHISQUAREDOPENBRACKET='dchisqr(';
	DOUBLEEXPONENTIALOPENBRACKET='ddexp(';
	EXPONENTIALOPENBRACKET='dexp(';
	FLATOPENBRACKET='dflat(';
	GAMMAOPENBRACKET='dgamma(';
	GEVOPENBRACKET='dgev(';
	GENFOPENBRACKET='df(';
	GENGAMMAOPENBRACKET='dggamma(';
	GENPARETOOPENBRACKET='dgpar(';
	GENLOGLIKOPENBRACKET='dloglik(';
	LOGNORMALOPENBRACKET='dlnorm(';
	LOGISTICOPENBRACKET='dlogis(';
	NORMALOPENBRACKET='dnorm(';
	PARETOOPENBRACKET='dpar(';
	STUDENTTOPENBRACKET='dt(';
	UNIFORMOPENBRACKET='dunif(';
	WEIBULLOPENBRACKET='dweib(';
	
	MUTLTINOMIALOPENBRACKET = 'dmulti(';
	DIRICHLETOPENBRACKET = 'ddirich(';
	DIRICHLET2OPENBRACKET = 'ddirch(';
	MVNORMALOPENBRACKET='dmnorm(';
	MVSTUDENTTOPENBRACKET='dmt(';
	WISHARTOPENBRACKET='dwish(';
	
	FORSTART = 'for';
	IN = 'in';

	OPENBRACE = '{';
	CLOSEBRACE = '}';
	TILDE = '~';
	CENSORBEGINWITHC = 'C(';
	CENSORBEGINWITHI = 'I(';
	TRUNCATIONBEGIN = 'T(';
	OPENBRACKET = '(';
	CLOSEBRACKET = ')';
	COMMA =  ',';
	OPENSQUAREBRACKET = '[';
	CLOSESQUAREBRACKET = ']';
	LEFTPOINTER = '<-';
	PLUS = '+';
	MINUS = '-';
	MULT = '*';
	DIV = '/';
	COLON = ':';	
	DOT='.';
	EQUALTO='=';
	HASH='#';
}

@lexer::namespace {BUGS}
@parser::namespace {BUGS}
@lexer::traits {

     class BUGSLexer; class BUGSParser;

     typedef antlr3::Traits< BUGSLexer, BUGSParser > BUGSLexerTraits;

     typedef BUGSLexerTraits BUGSParserTraits;

}
@lexer::includes{
#include <iostream>
using namespace std;
}
@parser::includes{
#include "BUGSLexer.hpp"
#include "../ModelClasses/Program.hpp"
#include "../ModelClasses/StochasticNodeStatement.hpp"
#include "../ModelClasses/StochasticNodeLimitation.hpp"
#include "../ModelClasses/LogicalNodeStatement.hpp"
#include "../ModelClasses/ForStatement.hpp"

#include "../ModelClasses/UnivariateNode.hpp"
#include "../ModelClasses/MultivariateNode.hpp"
#include "../ModelClasses/LinkFunctionNode.hpp"

#include "../ModelClasses/UnivariateDistribution.hpp"
#include "../ModelClasses/MultivariateDistribution.hpp"
#include "../ModelClasses/MVDParameters.hpp"

#include "../ModelClasses/Expression.hpp"
#include "../ModelClasses/MultiDimExpression.hpp"
#include "../ModelClasses/LogicalNodeExpression.hpp"

#include "../ModelClasses/ScalarFunction.hpp"
#include "../ModelClasses/VectorFunction.hpp"
}

/* Parser Rules*/
prog returns [Program* program = new Program()]
	: MODEL OPENBRACE statements CLOSEBRACE {$program->statements = $statements.stat;}
	;

statements returns [list<IStatement* > stat]:  
	((uvNode TILDE) => ste1=stochasticNodeExpr {$stat.push_back($ste1.stochasticNodeStatement);}
	| (mvNode TILDE) => ste2=stochasticNodeExpr {$stat.push_back($ste2.stochasticNodeStatement);}
	| (uvNode LEFTPOINTER) => lne1=logicalNodeExpr {$stat.push_back($lne1.logicalNodeStatement);}
	| (mvNode LEFTPOINTER) => lne2=logicalNodeExpr {$stat.push_back($lne2.logicalNodeStatement);}
	| (linkFunction LEFTPOINTER) =>  lne3=logicalNodeExpr  {$stat.push_back($lne3.logicalNodeStatement);}
	| sf=startFor stats=statements endFor
	{
	ForStatement* forstat = new ForStatement();
	forstat->loopvariable = $sf.loopvariable;
	forstat->beginexpression = $sf.beginexpression;
	forstat->endexpression = $sf.endexpression;
	forstat->statements = $stats.stat;
	$stat.push_back(forstat);
	}
	)+
	;

stochasticNodeExpr returns [StochasticNodeStatement* stochasticNodeStatement = new StochasticNodeStatement()]
	:
	(uvNode TILDE categoricaldistribution) => mvsne1=mvStochasticNodeExpr  
	{$stochasticNodeStatement->node = $mvsne1.mvnode; $stochasticNodeStatement->distribution = $mvsne1.mvdistribution; $stochasticNodeStatement->limitationType=NOLIMITATION;}
	| (uvNode) => uvsne=uvStochasticNodeExpr 
	{$stochasticNodeStatement->node = $uvsne.uvnode; 
	$stochasticNodeStatement->distribution = $uvsne.uvdistribution;
	$stochasticNodeStatement->limitationType=NOLIMITATION;}
	 (censor {$stochasticNodeStatement->limitationType=CENSOR; $stochasticNodeStatement->limitation=$censor.limitation;}
	 | truncation {$stochasticNodeStatement->limitationType=TRUNCATION; $stochasticNodeStatement->limitation=$truncation.limitation;}
	 )? 
	 | mvsne=mvStochasticNodeExpr  
	{$stochasticNodeStatement->node = $mvsne.mvnode; $stochasticNodeStatement->distribution = $mvsne.mvdistribution;}
	;

uvStochasticNodeExpr returns[UnivariateNode* uvnode, UnivariateDistribution* uvdistribution]
	:uvNode {$uvnode = $uvNode.uvnode;} 
	TILDE 
	uvDistribution  {$uvdistribution = $uvDistribution.distribution;}
	;
mvStochasticNodeExpr returns[MultivariateNode* mvnode, MultivariateDistribution* mvdistribution]
	:
	(uvNode TILDE) => uv1=uvNode 
	{$mvnode = new MultivariateNode(); 
	$mvnode->nodeid = $uv1.uvnode->nodeid; 
	
	for(std::vector<Expression* >::iterator it =$uv1.uvnode->indices.begin(); it!=$uv1.uvnode->indices.end(); it++) {
		MultiDimExpression* mdexp = new MultiDimExpression();
		mdexp->expbeforecolon = new Expression(*it);
		mdexp->colon = false;
		mdexp->empty = false;
		$mvnode->indices.push_back(mdexp);
	}

	delete $uv1.uvnode; // deleting the allocated memory		
	} 
	TILDE 
	categoricaldistribution {$mvdistribution = $categoricaldistribution.distribution;}
	| mvNode {$mvnode = $mvNode.mvnode;} 
	TILDE 
	mvDistribution {$mvdistribution = $mvDistribution.distribution;}
	;

categoricaldistribution  returns [MultivariateDistribution* distribution = new MultivariateDistribution()]
	: CATEGORICALOPENBRACKET dp1=mvNode CLOSEBRACKET
	{vector<MVDParameters* > parameters;
	MVDParameters* param1 = new MVDParameters(); 
	param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; 
	parameters.push_back(param1); 
	$distribution->name="CATEGORICAL"; 
	$distribution->distributionParameters=parameters;}
	;
	
censor returns [StochasticNodeLimitation* limitation = new StochasticNodeLimitation()]
	 : (CENSORBEGINWITHC|CENSORBEGINWITHI) 
	 ( (distributionParameter COMMA) =>
	 l=lowerWithOptionalUpper  
	 {$limitation->optional=$l.optional; 
	 $limitation->lowerlimit=$l.lowerlimit;
	 $limitation->upperlimit=$l.upperlimit;}
	 | u=upperWithOptionalLower
	 {$limitation->optional=$u.optional; 
	 $limitation->lowerlimit=$u.lowerlimit;
	 $limitation->upperlimit=$u.upperlimit;} )  CLOSEBRACKET
	 ;
truncation returns [StochasticNodeLimitation* limitation = new StochasticNodeLimitation()]
	: TRUNCATIONBEGIN (  (uvNode|CONSTANTVALUE) =>
	 l=lowerWithOptionalUpper 
	 {$limitation->optional=$l.optional; 
	 $limitation->lowerlimit=$l.lowerlimit;
	 $limitation->upperlimit=$l.upperlimit;}
	 | u=upperWithOptionalLower 
	 {$limitation->optional=$u.optional; 
	 $limitation->lowerlimit=$u.lowerlimit;
	 $limitation->upperlimit=$u.upperlimit;})  CLOSEBRACKET
	;
	
lowerWithOptionalUpper returns[OptionalComponent optional = UPPER, Expression* lowerlimit, Expression* upperlimit]
	:  (dp1=distributionParameter {$lowerlimit = $dp1.exp;}) 
	COMMA 
	 ( dp2=distributionParameter {$upperlimit = $dp2.exp;$optional = NOOPTIONAL;})?
	;
	
upperWithOptionalLower returns[OptionalComponent optional = LOWER, Expression* lowerlimit, Expression* upperlimit]
	:  (dp1=distributionParameter {$lowerlimit = $dp1.exp; $optional = NOOPTIONAL;})? 
	COMMA 
	(dp2=distributionParameter {$upperlimit = $dp2.exp;})
	;

logicalNodeExpr returns [LogicalNodeStatement* logicalNodeStatement = new LogicalNodeStatement()]
	: (uvNode | linkFunction) => 
	(uvNode {$logicalNodeStatement->logicalNode = $uvNode.uvnode;}
	| linkFunction {$logicalNodeStatement->logicalNode = $linkFunction.linkfunction;}) 
	LEFTPOINTER ex1=exprWithNodesFunctions {$logicalNodeStatement->logicalnodeexp= $ex1.exp;}
	|  mvNode {$logicalNodeStatement->logicalNode = $mvNode.mvnode;}
	LEFTPOINTER 
	ex2=exprWithNodesFunctions  {$logicalNodeStatement->logicalnodeexp= $ex2.exp;}
	;

linkFunction returns [LinkFunctionNode* linkfunction = new LinkFunctionNode()]
	: LOGOPENBRACKET uvNode CLOSEBRACKET  {$linkfunction->linkfunction = "LOG"; $linkfunction->uvnode=$uvNode.uvnode;}
	| LOGITOPENBRACKET uvNode CLOSEBRACKET {$linkfunction->linkfunction = "LOGIT"; $linkfunction->uvnode=$uvNode.uvnode;}
	| CLOGLOGOPENBRACKET uvNode CLOSEBRACKET {$linkfunction->linkfunction = "CLOGLOG"; $linkfunction->uvnode=$uvNode.uvnode;}
	| PROBITOPENBRACKET uvNode CLOSEBRACKET {$linkfunction->linkfunction = "PROBIT"; $linkfunction->uvnode=$uvNode.uvnode;}
	;

exprWithNodesFunctions returns [LogicalNodeExpression* exp]
	: logicalnodeadditiveExpression {$exp = $logicalnodeadditiveExpression.exp;}
	;
logicalnodeadditiveExpression returns [LogicalNodeExpression* exp]
@init{
LogicalNodeExpression* left;
LogicalNodeExpression* root;
LogicalNodeExpression* right;
bool rootinit = false;
LogicalNodeExpression* oldroot;
}
	: m1=logicalnodemultExpression {left = $m1.exp; $exp=left;}
	((PLUS { root = new LogicalNodeExpression(); root->op ='+'; root->type=LOPERATOR;}
	|MINUS { root = new LogicalNodeExpression(); root->op ='-'; root->type=LOPERATOR;}) 
	m2=logicalnodemultExpression 
	{right = $m2.exp;
	 if(rootinit) 
	{ root->leftchild = oldroot;  oldroot = root; }
	else 
	{root->leftchild = left; oldroot = root; rootinit = true;} 
	root->rightchild = right; $exp = root;} )*
	;

logicalnodemultExpression returns [LogicalNodeExpression* exp]
@init{
LogicalNodeExpression* left;
LogicalNodeExpression* root;
LogicalNodeExpression* right;
bool rootinit = false;
LogicalNodeExpression* oldroot;
}
	: e1=logicalnodeexprAtom {left = $e1.exp; $exp=left;}
	((MULT { root = new LogicalNodeExpression(); root->op ='*'; root->type=LOPERATOR;}
	| DIV { root = new LogicalNodeExpression(); root->op ='/'; root->type=LOPERATOR;}) 
	e2=logicalnodeexprAtom 
	{right = $e2.exp;
	if(rootinit) 
	{ root->leftchild = oldroot;  oldroot = root; }
	else 
	{root->leftchild = left; oldroot = root; rootinit = true;}
	 root->rightchild = right; $exp = root;})*
	;

logicalnodeexprAtom returns [LogicalNodeExpression* exp = new LogicalNodeExpression()]
	: unaryExpression {$exp->expvalue=$unaryExpression.uexpvalue; $exp->type=LCONSTANT; }
	| uvNode {$exp->uvnode=$uvNode.uvnode; $exp->type=LNODE; }
	| MINUS uvNode {$exp->uvnode=$uvNode.uvnode; $exp->uvnode->isunaryminus=true; $exp->type=LNODE; }
	| OPENBRACKET e1=exprWithNodesFunctions CLOSEBRACKET {$exp->exp = $e1.exp; $exp->type=LEXPRESSION;}
	| MINUS OPENBRACKET e1=exprWithNodesFunctions CLOSEBRACKET {$exp->exp = $e1.exp; $exp->isunaryminus = true; $exp->type=LEXPRESSION;}
	| scalarFunctions {$exp->function = $scalarFunctions.function;  $exp->type=LFUNCTION;}
	| vectorFunctions {$exp->function = $vectorFunctions.function;  $exp->type=LFUNCTION;}
	| MINUS scalarFunctions {$scalarFunctions.function->isunaryminus=true; $exp->function = $scalarFunctions.function;  $exp->type=LFUNCTION;}
	| MINUS vectorFunctions {$vectorFunctions.function->isunaryminus=true; $exp->function = $vectorFunctions.function;  $exp->type=LFUNCTION;}
	;
	
uvNode returns [UnivariateNode* uvnode = new UnivariateNode()]
	: NODENAME {$uvnode->nodeid = $NODENAME.text;}
	(
	OPENSQUAREBRACKET exp1=expression {$uvnode->indices.push_back($exp1.exp);} 
	(
	COMMA exp2=expression {$uvnode->indices.push_back($exp2.exp);}
	)* 
	 CLOSESQUAREBRACKET
	 )?
	;
mvNode returns [MultivariateNode* mvnode = new MultivariateNode()]
	: NODENAME OPENSQUAREBRACKET mi=multiIndices CLOSESQUAREBRACKET
	{$mvnode->nodeid = $NODENAME.text; $mvnode->indices=$mi.indices;}
	;

expression returns [Expression* exp]
	: additiveExpression {$exp = $additiveExpression.exp;}
	;

additiveExpression returns [Expression* exp]
@init{
Expression* left;
Expression* root;
Expression* right;
bool rootinit = false;
Expression* oldroot;
}
	: m1=multExpression {left = $m1.exp; $exp=left;}
	((PLUS {root = new Expression(); root->op ='+'; root->type=OPERATOR;}
	|MINUS {root = new Expression(); root->op ='-'; root->type=OPERATOR;}) 
	m2=multExpression
	{right = $m2.exp; 
	if(rootinit) 
	{ root->leftchild = oldroot;  oldroot = root; }
	else 
	{root->leftchild = left; oldroot = root; rootinit = true;}
	root->rightchild = right; $exp = root;} )*
	;

multExpression returns [Expression* exp]
@init{
Expression* left;
Expression* root;
Expression* right;
bool rootinit = false;
Expression* oldroot;
}
	: e1=exprAtom {left = $e1.exp; $exp=left;}
	((MULT {root = new Expression(); root->op ='*'; root->type=OPERATOR;}
	| DIV {root = new Expression(); root->op ='/'; root->type=OPERATOR;}) 
	e2=exprAtom {right = $e2.exp; 
	if(rootinit) 
	{ root->leftchild = oldroot;  oldroot = root; }
	else 
	{root->leftchild = left; oldroot = root; rootinit = true;}
	root->rightchild = right; $exp = root;} )*
	;

exprAtom returns [Expression* exp = new Expression()]
	: unaryExpression {$exp->expvalue=$unaryExpression.uexpvalue; $exp->type=CONSTANT; }
	| uvNode {$exp->uvnode=$uvNode.uvnode; $exp->type=NODE; }
	| OPENBRACKET e1=expression CLOSEBRACKET {$exp->exp = $e1.exp; $exp->type=EXPRESSION;}
	;
	
unaryExpression returns [double uexpvalue]
	: MINUS CONSTANTVALUE {$uexpvalue = -1 * ::atof($CONSTANTVALUE.text.c_str());}
	| CONSTANTINT  {$uexpvalue = ::atoi($CONSTANTINT.text.c_str());}
	| MINUS CONSTANTINT {$uexpvalue = -1 * ::atoi($CONSTANTINT.text.c_str());}
	| CONSTANTVALUE {$uexpvalue = ::atof($CONSTANTVALUE.text.c_str());}
	;

multiIndices returns [vector<MultiDimExpression* > indices]
	: me1=multiDimExpression {$indices.push_back($me1.exp);}
	 (COMMA me2=multiDimExpression {$indices.push_back($me2.exp);})*
	;
	
multiDimExpression returns[MultiDimExpression* exp = new MultiDimExpression()]
	: 
	 /*empty*/ {$exp->empty=true;}
	| (OPENBRACKET expression COLON) => OPENBRACKET ex1=expression COLON ex2=expression CLOSEBRACKET
	{$exp->colon=true; $exp->expbeforecolon=$ex1.exp; $exp->expaftercolon=$ex2.exp; $exp->empty=false;}
	| ex3=expression {$exp->colon=false; $exp->expbeforecolon=$ex3.exp; $exp->empty=false;}
	(COLON ex4=expression {$exp->colon=true; $exp->expaftercolon=$ex4.exp;})? 
	;

startFor returns [string loopvariable, Expression* beginexpression, Expression* endexpression]
	: FORSTART OPENBRACKET loopVariable IN loopBegin COLON loopEnd CLOSEBRACKET OPENBRACE 
	{$loopvariable = $loopVariable.text; 
	$beginexpression = $loopBegin.expr;
	$endexpression = $loopEnd.expr;}
	;

endFor 
	: CLOSEBRACE
	;

loopVariable 
	: NODENAME
	;
/*uvNode cannot be stochastic. It should be from the data only*/
loopBegin returns [Expression* expr]
	: expression {$expr = $expression.exp;}
	;

loopEnd  returns [Expression* expr]
	: expression  {$expr = $expression.exp; }
	;

CONSTANTINT
	: ('0'..'9')+ 
	;
	
NODENAME 
	:('a'..'z'| 'A'..'Z') ('a'..'z'| 'A'..'Z'| '0'..'9'| '.')* 
	;

CONSTANTVALUE 
	: ('0'..'9')+(DOT('0'..'9')+)?('E'(PLUS|MINUS)?('0'..'9')+)? 
	| (DOT('0'..'9')+)('E'(PLUS|MINUS)?('0'..'9')+)?
	| OPENBRACKET (PLUS|MINUS) ('0'..'9')+ (DOT('0'..'9')+)?('E'(PLUS|MINUS)?('0'..'9')+)? CLOSEBRACKET
	;

WHITESPACE : ( '\t' | ' ' | ';' | '\u000C' )+    { $channel = HIDDEN; skip();}
	;
	  
NEWLINE 	: ('\r' | '\n')+ {$channel = HIDDEN; skip();}
	;

scalarFunctions returns [ScalarFunction* function = new ScalarFunction()]
	:
	 ABSOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ABS"; $function->parameter1=$ex1.exp;}
	| ARCCOSOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ARCCOS"; $function->parameter1=$ex1.exp;}
	| ARCCOSHOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ARCCOSH"; $function->parameter1=$ex1.exp;}
	| ARCSINOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ARCSIN"; $function->parameter1=$ex1.exp;}
	| ARCSINHOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ARCSINH"; $function->parameter1=$ex1.exp;}
	| ARCTANOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="ARCTAN"; $function->parameter1=$ex1.exp;}
	| CLOGLOGOPENBRACKET  ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="CLOGLOG"; $function->parameter1=$ex1.exp;}
	| COSOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="COS"; $function->parameter1=$ex1.exp;}
	| COSHOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="COSH"; $function->parameter1=$ex1.exp;}
	| CUMULATIVEOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="CUMULATIVE"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| CUTOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET {$function->name="CUT"; $function->parameter1=$ex1.exp;}
	| DENSITYOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="DENSITY"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| DEVIANCEOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="DEVIANCE"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| EQUALSOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="EQUALS"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| EXPOPENBRACKET ex1=exprWithNodesFunctions  CLOSEBRACKET 
	{$function->name="EXP"; $function->parameter1=$ex1.exp;}
	| GAMMAPOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions  CLOSEBRACKET 
	{$function->name="GAMMAP"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| ILOGITOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="ILOGIT"; $function->parameter1=$ex1.exp;}
	| ICLOGLOGOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="ICLOGLOG"; $function->parameter1=$ex1.exp;}
	| INTEGRALOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions 
	COMMA ex3=exprWithNodesFunctions COMMA ex4=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="INTEGRAL"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp; 
	$function->parameter3=$ex3.exp; $function->parameter4=$ex4.exp;}
	| LOGOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="LOG"; $function->parameter1=$ex1.exp;}
	| LOGFACTOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="LOGFACT"; $function->parameter1=$ex1.exp;}
	| LOGGAMOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="LOGGAM"; $function->parameter1=$ex1.exp;}
	| LOGITOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="LOGIT"; $function->parameter1=$ex1.exp;}
	| MAXOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="MAX"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| MINOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="MIN"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp;}
	| PHIOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="PHI"; $function->parameter1=$ex1.exp;}
	| POSTPVALUEOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="POSTPVALUE"; $function->parameter1=$ex1.exp;}
	| POWOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="POW"; $function->parameter1=$ex1.exp;  $function->parameter2=$ex2.exp;}
	| PRIORPVALUEOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="PRIORPVALUE"; $function->parameter1=$ex1.exp;}
	| PROBITOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="PROBIT"; $function->parameter1=$ex1.exp;}
	| REPLICATEPOSTOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="REPLICATEPOST"; $function->parameter1=$ex1.exp;}
	| REPLICATEPRIOROPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="REPLICATEPRIOR"; $function->parameter1=$ex1.exp;}
	| ROUNDOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="ROUND"; $function->parameter1=$ex1.exp;}
	| SINOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="SIN"; $function->parameter1=$ex1.exp;}
	| SINHOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="SINH"; $function->parameter1=$ex1.exp;}
	| SOLUTIONOPENBRACKET ex1=exprWithNodesFunctions COMMA ex2=exprWithNodesFunctions 
	COMMA ex3=exprWithNodesFunctions COMMA ex4=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="SOLUTION"; $function->parameter1=$ex1.exp; $function->parameter2=$ex2.exp; 
	$function->parameter3=$ex3.exp; $function->parameter4=$ex4.exp;}
	| SQRTOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="SQRT"; $function->parameter1=$ex1.exp;}
	| STEPOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="STEP"; $function->parameter1=$ex1.exp;}
	| TANOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="TAN"; $function->parameter1=$ex1.exp;}
	| TANHOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="TANH"; $function->parameter1=$ex1.exp;}
	| TRUNCOPENBRACKET ex1=exprWithNodesFunctions CLOSEBRACKET 
	{$function->name="TRUNC"; $function->parameter1=$ex1.exp;}
	;

vectorFunctions returns [VectorFunction* function = new VectorFunction()]
	: 
	  MEANOPENBRACKET mvNode CLOSEBRACKET {$function->name="MEAN"; $function->mvnode=$mvNode.mvnode;}
	| INVERSEOPENBRACKET mvNode CLOSEBRACKET {$function->name="INVERSE"; $function->mvnode=$mvNode.mvnode;}
	| SDOPENBRACKET mvNode CLOSEBRACKET {$function->name="SD"; $function->mvnode=$mvNode.mvnode;}
	| SUMOPENBRACKET mvNode CLOSEBRACKET {$function->name="SUM"; $function->mvnode=$mvNode.mvnode;}
	| INPRODOPENBRACKET mv1=mvNode COMMA mv2=mvNode CLOSEBRACKET {$function->name="INPROD"; $function->mvnode=$mv1.mvnode; $function->mvnode2=$mv2.mvnode;}
	;
	
uvDistribution returns [UnivariateDistribution* distribution = new UnivariateDistribution()]
	: discreteUnivariate {$distribution->name = $discreteUnivariate.name; $distribution->distributionParameters = $discreteUnivariate.parameters;}
	| continuousUnivariate {$distribution->name = $continuousUnivariate.name; $distribution->distributionParameters = $continuousUnivariate.parameters;}
	;

mvDistribution returns [MultivariateDistribution* distribution = new MultivariateDistribution()]
	: discreteMultivariate {$distribution->name = $discreteMultivariate.name; $distribution->distributionParameters = $discreteMultivariate.parameters;}
	| continuousMultivariate {$distribution->name = $continuousMultivariate.name; $distribution->distributionParameters = $continuousMultivariate.parameters;}
	;

discreteUnivariate returns [string name, vector<Expression* > parameters]
	: bernoulli {$name="BERNOULLI"; $parameters=$bernoulli.parameters;}
	| binomial {$name="BINOMIAL"; $parameters=$binomial.parameters;}
	| negativebinomial {$name="NEGATIVEBINOMIAL"; $parameters=$negativebinomial.parameters;}
	| poisson {$name="POISSON"; $parameters=$poisson.parameters;}
	| nchg {$name="NCHG"; $parameters=$nchg.parameters;}
	;

continuousUnivariate returns [std::string name, vector<Expression* > parameters]
	: beta {$name="BETA"; $parameters=$beta.parameters;}
	| chisquared {$name="CHISQUARED"; $parameters=$chisquared.parameters;}
	| ddexp {$name="DDEXP"; $parameters=$ddexp.parameters;}
	| exp {$name="EXP"; $parameters=$exp.parameters;}
	| flat {$name="FLAT"; $parameters=$flat.parameters;}
	| gamma {$name="GAMMA"; $parameters=$gamma.parameters;}
	| gev {$name="GEV"; $parameters=$gev.parameters;}
	| genf {$name="GENF"; $parameters=$genf.parameters;}
	| gengamma {$name="GENGAMMA"; $parameters=$gengamma.parameters;}
	| genpareto {$name="GENPARETO"; $parameters=$genpareto.parameters;}
	| genloglik {$name="GENLOGLIK"; $parameters=$genloglik.parameters;}
	| lognormal {$name="LOGNORMAL"; $parameters=$lognormal.parameters;}
	| logistic {$name="LOGISTIC"; $parameters=$logistic.parameters;}
	| normal {$name="NORMAL"; $parameters=$normal.parameters;}
	| pareto {$name="PARETO"; $parameters=$pareto.parameters;}
	| studentt {$name="STUDENTT"; $parameters=$studentt.parameters;}
	| uniform {$name="UNIFORM"; $parameters=$uniform.parameters;}
	| weibull {$name="WEIBULL"; $parameters=$weibull.parameters;}
	;

discreteMultivariate returns [string name, vector<MVDParameters* > parameters]
	: multinomial {$name="MULTINOMIAL"; $parameters=$multinomial.parameters;}
	;

continuousMultivariate returns [string name, vector<MVDParameters* > parameters]
	: dirichlet {$name="DIRICHLET"; $parameters=$dirichlet.parameters;}
	| mvnormal {$name="MVNORMAL"; $parameters=$mvnormal.parameters;}
	| mvstudentt {$name="MVSTUDENTT"; $parameters=$mvstudentt.parameters;}
	| wishart {$name="WISHART"; $parameters=$wishart.parameters;}
	;
	
distributionParameter returns [Expression* exp = new Expression()]
	: uvNode {$exp->uvnode=$uvNode.uvnode; $exp->type=NODE;}
	| unaryExpression {$exp->expvalue=$unaryExpression.uexpvalue; $exp->type=CONSTANT;}
	;
	
bernoulli returns [vector<Expression* > parameters]
	: BERNOULLIOPENBRACKET distributionParameter CLOSEBRACKET {$parameters.push_back($distributionParameter.exp);}
	;

binomial returns [vector<Expression* > parameters]
	: BINOMIALOPENBRACKET dp1=distributionParameter COMMA dp2=distributionParameter CLOSEBRACKET {$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);}
	;
	
negativebinomial returns [vector<Expression* > parameters]
	: NEGATIVEBINOMIALOPENBRACKET dp1=distributionParameter COMMA dp2=distributionParameter CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);}
	;

poisson returns [vector<Expression* > parameters]
	: POISSONOPENBRACKET distributionParameter CLOSEBRACKET {$parameters.push_back($distributionParameter.exp);}
	;

nchg returns [vector<Expression* > parameters]
	: NCHGOPENBRACKET dp1=distributionParameter  
	COMMA dp2=distributionParameter 
	COMMA dp3=distributionParameter 
	COMMA dp4=distributionParameter 
	CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);
	$parameters.push_back($dp3.exp);$parameters.push_back($dp4.exp);}
	;
	
beta returns [vector<Expression* > parameters]
	: BETAOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter CLOSEBRACKET {$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);}
	;

chisquared returns [vector<Expression* > parameters]
	: CHISQUAREDOPENBRACKET dp1=distributionParameter CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); }
	;

ddexp returns [vector<Expression* > parameters]
	: DOUBLEEXPONENTIALOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);}
	;

exp returns [vector<Expression* > parameters]
	: EXPONENTIALOPENBRACKET dp1=distributionParameter  CLOSEBRACKET
	 {$parameters.push_back($dp1.exp); }
	;

flat returns [vector<Expression* > parameters]
	:FLATOPENBRACKET  CLOSEBRACKET {}
	;
	
gamma returns [vector<Expression* > parameters]
	: GAMMAOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);}
	;

gev returns [vector<Expression* > parameters]
	: GEVOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter COMMA 
	dp3=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);  $parameters.push_back($dp3.exp);}
	;

genf returns [vector<Expression* > parameters]
	: GENFOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter COMMA 
	dp3=distributionParameter  COMMA dp4=distributionParameter CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);  $parameters.push_back($dp3.exp); $parameters.push_back($dp4.exp);}
	;

gengamma returns [vector<Expression* > parameters]
	: GENGAMMAOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter COMMA 
	dp3=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);  $parameters.push_back($dp3.exp);}
	;

genpareto returns [vector<Expression* > parameters]
	: GENPARETOOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter COMMA 
	dp3=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);  $parameters.push_back($dp3.exp);}
	;
	
genloglik returns [vector<Expression* > parameters]
	: GENLOGLIKOPENBRACKET dp1=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp);}
	;

lognormal returns [vector<Expression* > parameters]
	: LOGNORMALOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;

logistic returns [vector<Expression* > parameters]
	: LOGISTICOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;
	
normal returns [vector<Expression* > parameters]
	: NORMALOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;

pareto returns [vector<Expression* > parameters]
	: PARETOOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;

studentt returns [vector<Expression* > parameters]
	: STUDENTTOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter COMMA 
	dp3=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp);  $parameters.push_back($dp3.exp);}
	;

uniform returns [vector<Expression* > parameters]
	: UNIFORMOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;

weibull returns [vector<Expression* > parameters]
	: WEIBULLOPENBRACKET dp1=distributionParameter  COMMA dp2=distributionParameter  CLOSEBRACKET 
	{$parameters.push_back($dp1.exp); $parameters.push_back($dp2.exp); }
	;
	
multinomial returns [vector<MVDParameters* > parameters]
	: MUTLTINOMIALOPENBRACKET dp1=mvNode COMMA 
	dp2=distributionParameter CLOSEBRACKET
	{MVDParameters* param1 = new MVDParameters(); param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; $parameters.push_back(param1);
	MVDParameters* param2  = new MVDParameters(); param2->type=MVDEXPRESSION; param2->exp = $dp2.exp; $parameters.push_back(param2);}
	;
	
dirichlet returns [vector<MVDParameters* > parameters]
	: (DIRICHLETOPENBRACKET|DIRICHLET2OPENBRACKET) dp1=mvNode CLOSEBRACKET 
	{MVDParameters* param1 = new MVDParameters(); param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; $parameters.push_back(param1);}
	;

mvnormal returns [vector<MVDParameters* > parameters]
	: MVNORMALOPENBRACKET dp1=mvNode COMMA dp2=mvNode CLOSEBRACKET 
	{MVDParameters* param1 = new MVDParameters(); param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; $parameters.push_back(param1);
	MVDParameters* param2 = new MVDParameters(); param2->type=MVDNODE; param2->mvnode = $dp2.mvnode; $parameters.push_back(param2);}
	;
	
mvstudentt returns [vector<MVDParameters* > parameters]
	: MVSTUDENTTOPENBRACKET dp1=mvNode COMMA dp2=mvNode COMMA dp3=distributionParameter CLOSEBRACKET 
	{MVDParameters* param1 = new MVDParameters(); param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; $parameters.push_back(param1);
	MVDParameters* param2 = new MVDParameters(); param2->type=MVDNODE; param2->mvnode = $dp2.mvnode; $parameters.push_back(param2);
	MVDParameters* param3  = new MVDParameters(); param3->type=MVDEXPRESSION; param3->exp = $dp3.exp; $parameters.push_back(param3);}
	;
	
wishart returns [vector<MVDParameters* > parameters]
	: WISHARTOPENBRACKET dp1=mvNode COMMA dp2=distributionParameter CLOSEBRACKET 
	{MVDParameters* param1 = new MVDParameters(); param1->type=MVDNODE; param1->mvnode = $dp1.mvnode; $parameters.push_back(param1);
	MVDParameters* param2  = new MVDParameters(); param2->type=MVDEXPRESSION; param2->exp = $dp2.exp; $parameters.push_back(param2);}
	;