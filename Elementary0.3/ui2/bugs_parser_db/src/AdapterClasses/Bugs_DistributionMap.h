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

#include "../../../../util/Common.h"
#include "../../../../fg/Variable.h"
#include "../../../../fg/Factor.h"
#include "../../../../fg/FactorGraph.h"
#include "../../../../udf/FactorFactory_BUGS.h"
#include <float.h>

#define NEGBIN_UPPER_BOUND 10000;
#define POISSON_UPPER_BOUND 10000
#define NCHG_UPPER_BOUND 10000
#define BETA_UPPER_BOUND 10000
#define CHISQR_UPPER_BOUND 10000
#define DDEXP_UPPER_BOUND 10000
#define DDEXP_LOWER_BOUND -10000
#define EXP_UPPER_BOUND 10000
#define GAMMA_LOWER_BOUND 1E-4
#define GAMMA_UPPER_BOUND 20
#define GEV_LOWER_BOUND -10000
#define GEV_UPPER_BOUND 10000
#define GENPARETO_LOWER_BOUND -10000
#define GENPARETO_UPPER_BOUND 10000
#define GENGAMMA_UPPER_BOUND 10000
#define GENLOGLIK_UPPER_BOUND 10000
#define LOGNORMAL_LOWER_BOUND -10000
#define LOGNORMAL_UPPER_BOUND 10000
#define LOGISTIC_LOWER_BOUND -10000
#define LOGISTIC_UPPER_BOUND 10000
#define NORMAL_LOWER_BOUND -10000
#define NORMAL_UPPER_BOUND 10000
#define PARETO_UPPER_BOUND 10000
#define STUDENTT_LOWER_BOUND -10000
#define STUDENTT_UPPER_BOUND 10000
#define UNIFORM_UPPER_BOUND 10000
#define WEIBULL_UPPER_BOUND 10000
#define DIRICHLET_UPPER_BOUND 10000
#define MVNORMAL_LOWER_BOUND -10000
#define MVNORMAL_UPPER_BOUND 10000
#define MVSTUDENTT_LOWER_BOUND -10000
#define MVSTUDENTT_UPPER_BOUND 10000
#define WISHART_LOWER_BOUND -10000
#define WISHART_UPPER_BOUND 10000
#define FLAT_LOWER_BOUND -10000
#define FLAT_UPPER_BOUND 10000
#define ZEROTOINF_LOWER_BOUND 1E-10000

namespace hazy {
enum ParameterType {ZEROTOONE, ZEROTON, ZEROTOINF, MINUSINFTOINF, NTOINF, NTON};
class DistributionParametersAdapter{
public:
	DomainType type;
	ParameterType ptype;
	float upperbound; // only for ZEROTOINF
	float lowerbound; // both ZERTOTOINF and MINUSINFTOINF
	VGROUPTYPE variablegrouptype; // only for MV nodes
};

class DistributionAdapter {
public:
	DistributionAdapter() {};
	virtual ~DistributionAdapter() {};
	double (*potential)(Factor &, const Variable &, const double &);
	void (*vg)(Factor &factor, VariableGroup & vg);
	DomainType variabledtype;
	ParameterType variableptype;
	float upperbound; // only for ZEROTOINF
	float lowerbound; // both ZERTOTOINF and MINUSINFTOINF
	int lowerboundparameter;
	int upperboundparameter;
	std::vector<DistributionParametersAdapter> parametertype;
};

std::map<std::string, DistributionAdapter > getDistributionMap(){
std::map<std::string, DistributionAdapter > distributionMap;

	DistributionAdapter logical;
	logical.potential = &FactorFactory_BUGS::potential_null;
	logical.vg = &FactorFactory_BUGS::vg_logicalnode_assign;
	distributionMap["LOGICAL"] = logical;

//	DistributionAdapter normal_vg;
//	normal_vg.vg = &FactorFactory_BUGS::vg_normal_assign;
//	normal_vg.potential = &FactorFactory_BUGS::potential_null;
//	distributionMap["NORMAL_VG"] = normal_vg;

	DistributionAdapter bernoulli;
	bernoulli.potential = &FactorFactory_BUGS::bugs_bernoulli;
	bernoulli.vg = NULL;
	bernoulli.variabledtype = BOOLEAN;
	bernoulli.variableptype = ZEROTOONE;
	DistributionParametersAdapter bernoulliparams;
	bernoulliparams.type = REAL;
	bernoulliparams.ptype = ZEROTOONE;
	bernoulli.parametertype.push_back(bernoulliparams);

	distributionMap["BERNOULLI"] = bernoulli;

	DistributionAdapter binomial;
	binomial.potential = &FactorFactory_BUGS::bugs_binomial;
	binomial.vg = NULL;
	binomial.variabledtype = hazy::MULTINOMIAL;
	binomial.variableptype = ZEROTON;
	binomial.upperboundparameter = 1; // 2nd parameter is the upperbound
	DistributionParametersAdapter binparam1;
	binparam1.type = hazy::REAL;
	binparam1.ptype = ZEROTOONE;
	binomial.parametertype.push_back(binparam1);
	DistributionParametersAdapter binparam2;
	binparam2.type = hazy::MULTINOMIAL;
	binparam2.ptype = ZEROTON;
	binomial.parametertype.push_back(binparam2);

	distributionMap["BINOMIAL"] = binomial;


	DistributionAdapter negbin;
	negbin.vg = NULL;
	negbin.potential = &FactorFactory_BUGS::bugs_negativebinomial;
	negbin.variabledtype = hazy::MULTINOMIAL;
	negbin.variableptype = ZEROTOINF;
	negbin.upperbound = NEGBIN_UPPER_BOUND;
	DistributionParametersAdapter negbinparams1;
	negbinparams1.type = hazy::REAL;
	negbinparams1.ptype = ZEROTOONE;
	negbin.parametertype.push_back(negbinparams1);
	DistributionParametersAdapter negbinparams2;
	negbinparams2.type = hazy::MULTINOMIAL;
	negbinparams2.ptype = ZEROTOINF;
	negbinparams2.upperbound = NEGBIN_UPPER_BOUND;
	negbin.parametertype.push_back(negbinparams2);

	distributionMap["NEGATIVEBINOMIAL"] = negbin;


	DistributionAdapter poisson;
	poisson.vg = NULL;
	poisson.potential = &FactorFactory_BUGS::bugs_poisson;
	poisson.variabledtype = hazy::MULTINOMIAL;
	poisson.variableptype = ZEROTOINF;
	poisson.upperbound = POISSON_UPPER_BOUND;
	DistributionParametersAdapter poissonparams;
	poissonparams.type = hazy::REAL;
	poissonparams.ptype = ZEROTOINF;
	poissonparams.upperbound = POISSON_UPPER_BOUND;
	poisson.parametertype.push_back(poissonparams);

	distributionMap["POISSON"] = poisson;

	DistributionAdapter nchg;
	nchg.vg = NULL;
	nchg.potential = &FactorFactory_BUGS::bugs_noncentralhypergeometric;
	nchg.variabledtype = hazy::MULTINOMIAL;
	nchg.variableptype = NTON;
	DistributionParametersAdapter nchgparams1;
	nchgparams1.type = hazy::MULTINOMIAL;
	nchgparams1.ptype = ZEROTOINF;
	nchgparams1.upperbound = NCHG_UPPER_BOUND;
	nchg.parametertype.push_back(nchgparams1);
	DistributionParametersAdapter nchgparams2;
	nchgparams2.type = hazy::MULTINOMIAL;
	nchgparams2.ptype = ZEROTOINF;
	nchgparams2.upperbound = NCHG_UPPER_BOUND;
	nchg.parametertype.push_back(nchgparams2);
	DistributionParametersAdapter nchgparams3;
	nchgparams3.type = hazy::MULTINOMIAL;
	nchgparams3.ptype = ZEROTON;
	nchgparams3.upperbound = NCHG_UPPER_BOUND;
	nchg.parametertype.push_back(nchgparams3);
	DistributionParametersAdapter nchgparams4;
	nchgparams4.type = hazy::REAL;
	nchgparams4.ptype = ZEROTOINF;
	nchgparams4.upperbound = NCHG_UPPER_BOUND;
	nchg.parametertype.push_back(nchgparams4);

	distributionMap["NCHG"] = nchg;

	DistributionAdapter beta;
	beta.vg = NULL;
	beta.potential = &FactorFactory_BUGS::bugs_beta;
	beta.variabledtype = hazy::REAL;
	beta.variableptype = ZEROTOONE;
	beta.upperbound = BETA_UPPER_BOUND;
	DistributionParametersAdapter betaparams1;
	betaparams1.type = hazy::REAL;
	betaparams1.ptype = ZEROTOINF;
	betaparams1.upperbound = BETA_UPPER_BOUND;
	beta.parametertype.push_back(betaparams1);
	DistributionParametersAdapter betaparams2;
	betaparams2.type = hazy::REAL;
	betaparams2.ptype = ZEROTOINF;
	betaparams2.upperbound = BETA_UPPER_BOUND;
	beta.parametertype.push_back(betaparams2);

	distributionMap["BETA"] = beta;

	DistributionAdapter chisqr;
	chisqr.vg = NULL;
	chisqr.potential = &FactorFactory_BUGS::bugs_chisquared;
	chisqr.variabledtype = hazy::REAL;
	chisqr.variableptype = ZEROTOINF;
	chisqr.upperbound = CHISQR_UPPER_BOUND;
	DistributionParametersAdapter chisqrparams1;
	chisqrparams1.type = hazy::REAL;
	chisqrparams1.ptype = ZEROTOINF;
	chisqrparams1.upperbound = CHISQR_UPPER_BOUND;
	chisqr.parametertype.push_back(chisqrparams1);

	distributionMap["CHISQUARED"] = chisqr;

	DistributionAdapter ddexp;
	ddexp.vg = NULL;
	ddexp.potential = &FactorFactory_BUGS::bugs_doubleexponential;
	ddexp.variabledtype = hazy::REAL;
	ddexp.variableptype = MINUSINFTOINF;
	ddexp.upperbound = DDEXP_UPPER_BOUND;
	ddexp.lowerbound = DDEXP_LOWER_BOUND;
	DistributionParametersAdapter ddexpparams1;
	ddexpparams1.type = hazy::REAL;
	ddexpparams1.ptype = MINUSINFTOINF;
	ddexpparams1.upperbound = DDEXP_UPPER_BOUND;
	ddexpparams1.lowerbound = DDEXP_LOWER_BOUND;
	ddexp.parametertype.push_back(ddexpparams1);
	DistributionParametersAdapter ddexpparams2;
	ddexpparams2.type = hazy::REAL;
	ddexpparams2.ptype = ZEROTOINF;
	ddexpparams2.upperbound = DDEXP_UPPER_BOUND;
	ddexp.parametertype.push_back(ddexpparams2);

	distributionMap["DDEXP"] = ddexp;

	DistributionAdapter exp;
	exp.vg = NULL;
	exp.potential = &FactorFactory_BUGS::bugs_exponential;
	exp.variabledtype = hazy::REAL;
	exp.variableptype = ZEROTOINF;
	exp.upperbound = EXP_UPPER_BOUND;
	DistributionParametersAdapter expparams1;
	expparams1.type = hazy::REAL;
	expparams1.ptype = ZEROTOINF;
	expparams1.upperbound = EXP_UPPER_BOUND;
	exp.parametertype.push_back(expparams1);

	distributionMap["EXP"] = exp;

	DistributionAdapter dgamma;
	dgamma.vg = NULL;
	dgamma.potential = &FactorFactory_BUGS::bugs_gamma;
	dgamma.variabledtype = hazy::REAL;
	dgamma.variableptype = ZEROTOINF;
	dgamma.upperbound = GAMMA_UPPER_BOUND;
	DistributionParametersAdapter dgammaparams1;
	dgammaparams1.type = hazy::REAL;
	dgammaparams1.ptype = ZEROTOINF;
	dgammaparams1.upperbound = GAMMA_UPPER_BOUND;
	dgamma.parametertype.push_back(dgammaparams1);
	DistributionParametersAdapter dgammaparams2;
	dgammaparams2.type = hazy::REAL;
	dgammaparams2.ptype = ZEROTOINF;
	dgammaparams2.upperbound = GAMMA_UPPER_BOUND;
	dgamma.parametertype.push_back(dgammaparams2);

	distributionMap["GAMMA"] = dgamma;

	DistributionAdapter gev;
	gev.vg = NULL;
	gev.potential = &FactorFactory_BUGS::bugs_generalizedextremevalue;
	gev.variabledtype = hazy::REAL;
	gev.variableptype = MINUSINFTOINF;
	gev.lowerbound = GEV_LOWER_BOUND;
	gev.upperbound = GEV_UPPER_BOUND;
	DistributionParametersAdapter gevparams1;
	gevparams1.type = hazy::REAL;
	gevparams1.ptype = MINUSINFTOINF;
	gevparams1.lowerbound = GEV_LOWER_BOUND;
	gevparams1.upperbound = GEV_UPPER_BOUND;
	gev.parametertype.push_back(gevparams1);
	DistributionParametersAdapter gevparams2;
	gevparams2.type = hazy::REAL;
	gevparams2.ptype = ZEROTOINF;
	gevparams2.upperbound = GEV_UPPER_BOUND;
	gev.parametertype.push_back(gevparams2);
	DistributionParametersAdapter gevparams3;
	gevparams3.type = hazy::REAL;
	gevparams3.ptype = MINUSINFTOINF;
	gevparams3.lowerbound = GEV_LOWER_BOUND;
	gevparams3.upperbound = GEV_UPPER_BOUND;
	gev.parametertype.push_back(gevparams3);

	distributionMap["GEV"] = gev;

	DistributionAdapter genpareto;
	genpareto.vg = NULL;
	genpareto.potential = &FactorFactory_BUGS::bugs_generalizedpareto;
	genpareto.variabledtype = hazy::REAL;
	genpareto.variableptype = MINUSINFTOINF;
	genpareto.lowerbound = GENPARETO_LOWER_BOUND;
	genpareto.upperbound = GENPARETO_UPPER_BOUND;
	DistributionParametersAdapter genparetoparams1;
	genparetoparams1.type = hazy::REAL;
	genparetoparams1.ptype = MINUSINFTOINF;
	genparetoparams1.lowerbound = GENPARETO_LOWER_BOUND;
	genparetoparams1.upperbound = GENPARETO_UPPER_BOUND;
	genpareto.parametertype.push_back(genparetoparams1);
	DistributionParametersAdapter genparetoparams2;
	genparetoparams2.type = hazy::REAL;
	genparetoparams2.ptype = ZEROTOINF;
	genparetoparams2.upperbound = GENPARETO_UPPER_BOUND;
	genpareto.parametertype.push_back(genparetoparams2);
	DistributionParametersAdapter genparetoparams3;
	genparetoparams3.type = hazy::REAL;
	genparetoparams3.ptype = MINUSINFTOINF;
	genparetoparams3.lowerbound = GENPARETO_LOWER_BOUND;
	genparetoparams3.upperbound = GENPARETO_UPPER_BOUND;
	genpareto.parametertype.push_back(genparetoparams3);

	distributionMap["GENPARETO"] = genpareto;

	DistributionAdapter gengamma;
	gengamma.vg = NULL;
	gengamma.potential = &FactorFactory_BUGS::bugs_generalizedgamma;
	gengamma.variabledtype = hazy::REAL;
	gengamma.variableptype = ZEROTOINF;
	gengamma.upperbound = GENGAMMA_UPPER_BOUND;
	DistributionParametersAdapter gengammaparams1;
	gengammaparams1.type = hazy::REAL;
	gengammaparams1.ptype = ZEROTOINF;
	gengammaparams1.upperbound = GENGAMMA_UPPER_BOUND;
	gengamma.parametertype.push_back(gengammaparams1);
	DistributionParametersAdapter gengammaparams2;
	gengammaparams2.type = hazy::REAL;
	gengammaparams2.ptype = ZEROTOINF;
	gengammaparams2.upperbound = GENGAMMA_UPPER_BOUND;
	gengamma.parametertype.push_back(gengammaparams2);
	DistributionParametersAdapter gengammaparams3;
	gengammaparams3.type = hazy::REAL;
	gengammaparams3.ptype = ZEROTOINF;
	gengammaparams3.upperbound = GENGAMMA_UPPER_BOUND;
	gengamma.parametertype.push_back(gengammaparams3);

	distributionMap["GENGAMMA"] = gengamma;

	DistributionAdapter genloglik;
	genloglik.vg = NULL;
	genloglik.potential = &FactorFactory_BUGS::bugs_genericloglikelihood;
	genloglik.variabledtype = hazy::REAL;
	genloglik.variableptype = ZEROTOINF;
	genloglik.upperbound = GENLOGLIK_UPPER_BOUND;
	DistributionParametersAdapter genloglikparams1;
	genloglikparams1.type = hazy::REAL;
	genloglikparams1.ptype = ZEROTOINF;
	genloglikparams1.upperbound = GENLOGLIK_UPPER_BOUND;
	genloglik.parametertype.push_back(genloglikparams1);

	distributionMap["GENLOGLIK"] = genloglik;

	DistributionAdapter lognormal;
	lognormal.vg = NULL;
	lognormal.potential = &FactorFactory_BUGS::bugs_lognormal;
	lognormal.variabledtype = hazy::REAL;
	lognormal.variableptype = ZEROTOINF;
	lognormal.upperbound = LOGNORMAL_UPPER_BOUND;
	DistributionParametersAdapter lognormalparams1;
	lognormalparams1.type = hazy::REAL;
	lognormalparams1.ptype = MINUSINFTOINF;
	lognormalparams1.lowerbound = LOGNORMAL_LOWER_BOUND;
	lognormalparams1.upperbound = LOGNORMAL_UPPER_BOUND;
	lognormal.parametertype.push_back(lognormalparams1);
	DistributionParametersAdapter lognormalparams2;
	lognormalparams2.type = hazy::REAL;
	lognormalparams2.ptype = ZEROTOINF;
	lognormalparams2.upperbound = LOGNORMAL_UPPER_BOUND;
	lognormal.parametertype.push_back(lognormalparams2);

	distributionMap["LOGNORMAL"] = lognormal;

	DistributionAdapter logistic;
	logistic.vg = NULL;
	logistic.potential = &FactorFactory_BUGS::bugs_logistic;
	logistic.variabledtype = hazy::REAL;
	logistic.variableptype = MINUSINFTOINF;
	logistic.lowerbound = LOGISTIC_LOWER_BOUND;
	logistic.upperbound = LOGISTIC_UPPER_BOUND;
	DistributionParametersAdapter logisticparams1;
	logisticparams1.type = hazy::REAL;
	logisticparams1.ptype = MINUSINFTOINF;
	logisticparams1.lowerbound = LOGISTIC_LOWER_BOUND;
	logisticparams1.upperbound = LOGISTIC_UPPER_BOUND;
	logistic.parametertype.push_back(logisticparams1);
	DistributionParametersAdapter logisticparams2;
	logisticparams2.type = hazy::REAL;
	logisticparams2.ptype = ZEROTOINF;
	logisticparams2.upperbound = LOGISTIC_UPPER_BOUND;
	logistic.parametertype.push_back(logisticparams2);

	distributionMap["LOGISTIC"] = logistic;

	DistributionAdapter normal;
	normal.vg = &FactorFactory_BUGS::vg_logicalnode_assign;
	normal.potential = &FactorFactory_BUGS::bugs_normal;
	normal.variabledtype = hazy::REAL;
	normal.variableptype = MINUSINFTOINF;
	normal.lowerbound = NORMAL_LOWER_BOUND;
	normal.upperbound = NORMAL_UPPER_BOUND;
	DistributionParametersAdapter normalparams1;
	normalparams1.type = hazy::REAL;
	normalparams1.ptype = MINUSINFTOINF;
	normalparams1.lowerbound = NORMAL_LOWER_BOUND;
	normalparams1.upperbound = NORMAL_UPPER_BOUND;
	normal.parametertype.push_back(normalparams1);
	DistributionParametersAdapter normalparams2;
	normalparams2.type = hazy::REAL;
	normalparams2.ptype = ZEROTOINF;
	normalparams2.upperbound = NORMAL_UPPER_BOUND;
	normal.parametertype.push_back(normalparams2);

	distributionMap["NORMAL"] = normal;

	DistributionAdapter pareto;
	pareto.vg = NULL;
	pareto.potential = &FactorFactory_BUGS::bugs_pareto;
	pareto.variabledtype = hazy::REAL;
	pareto.variableptype = NTOINF;
	pareto.lowerboundparameter = 1; //2nd parameter
	pareto.upperbound = PARETO_UPPER_BOUND;
	DistributionParametersAdapter paretoparams1;
	paretoparams1.type = hazy::REAL;
	paretoparams1.ptype = ZEROTOINF;
	paretoparams1.upperbound = PARETO_UPPER_BOUND;
	pareto.parametertype.push_back(paretoparams1);
	DistributionParametersAdapter paretoparams2;
	paretoparams2.type = hazy::REAL;
	paretoparams2.ptype = ZEROTOINF;
	paretoparams2.upperbound = PARETO_UPPER_BOUND;
	pareto.parametertype.push_back(paretoparams2);

	distributionMap["PARETO"] = pareto;

	DistributionAdapter studentt;
	studentt.vg = NULL;
	studentt.potential = &FactorFactory_BUGS::bugs_studentt;
	studentt.variabledtype = hazy::REAL;
	studentt.variableptype = MINUSINFTOINF;
	studentt.lowerbound = STUDENTT_LOWER_BOUND;
	studentt.upperbound = STUDENTT_UPPER_BOUND;
	DistributionParametersAdapter studenttparams1;
	studenttparams1.type = hazy::REAL;
	studenttparams1.ptype = ZEROTOINF;
	studenttparams1.upperbound = STUDENTT_UPPER_BOUND;
	studentt.parametertype.push_back(studenttparams1);
	DistributionParametersAdapter studenttparams2;
	studenttparams2.type = hazy::REAL;
	studenttparams2.ptype = ZEROTOINF;
	studenttparams2.upperbound = STUDENTT_UPPER_BOUND;
	studentt.parametertype.push_back(studenttparams2);
	DistributionParametersAdapter studenttparams3;
	studenttparams3.type = hazy::REAL;
	studenttparams3.ptype = NTOINF;
	studenttparams3.lowerbound = 2;
	studenttparams3.upperbound = STUDENTT_UPPER_BOUND;
	studentt.parametertype.push_back(studenttparams3);

	distributionMap["STUDENTT"] = studentt;

	DistributionAdapter uniform;
	uniform.vg = NULL;
	uniform.potential = &FactorFactory_BUGS::bugs_uniform;
	uniform.variabledtype = hazy::REAL;
	uniform.variableptype = NTON;
	uniform.lowerboundparameter = 0;
	uniform.upperboundparameter = 1;
	DistributionParametersAdapter uniformparams1;
	uniformparams1.type = hazy::REAL;
	uniformparams1.ptype = ZEROTOINF;
	uniformparams1.upperbound = UNIFORM_UPPER_BOUND;
	uniform.parametertype.push_back(uniformparams1);
	DistributionParametersAdapter uniformparams2;
	uniformparams2.type = hazy::REAL;
	uniformparams2.ptype = ZEROTOINF;
	uniformparams2.upperbound = UNIFORM_UPPER_BOUND;
	uniform.parametertype.push_back(uniformparams2);

	distributionMap["UNIFORM"] = uniform;

	DistributionAdapter weibull;
	weibull.vg = NULL;
	weibull.potential = &FactorFactory_BUGS::bugs_weibull;
	weibull.variabledtype = hazy::REAL;
	weibull.variableptype = ZEROTOINF;
	weibull.upperbound = WEIBULL_UPPER_BOUND;
	DistributionParametersAdapter weibullparams1;
	weibullparams1.type = hazy::REAL;
	weibullparams1.ptype = ZEROTOINF;
	weibullparams1.upperbound = WEIBULL_UPPER_BOUND;
	weibull.parametertype.push_back(weibullparams1);
	DistributionParametersAdapter weibullparams2;
	weibullparams2.type = hazy::REAL;
	weibullparams2.ptype = ZEROTOINF;
	weibullparams2.upperbound = WEIBULL_UPPER_BOUND;
	weibull.parametertype.push_back(weibullparams2);

	distributionMap["WEIBULL"] = weibull;


	DistributionAdapter flat;
	flat.vg = NULL;
	flat.potential = &FactorFactory_BUGS::bugs_flat;
	flat.variabledtype = hazy::REAL;
	flat.variableptype = MINUSINFTOINF;
	flat.upperbound = FLAT_UPPER_BOUND;
	flat.lowerbound = FLAT_LOWER_BOUND;
	distributionMap["FLAT"] = flat;

	DistributionAdapter categorical;
	categorical.potential = &FactorFactory_BUGS::bugs_categorical;
	categorical.variabledtype = hazy::MULTINOMIAL;
	categorical.variableptype = ZEROTON;
	categorical.upperboundparameter = -1; // Is Handled as a special case
	DistributionParametersAdapter categoricalparams;
	categoricalparams.type = hazy::REAL;
	categoricalparams.ptype = ZEROTOONE;
	categoricalparams.variablegrouptype = LISTOFVARIABLE;
	categorical.parametertype.push_back(categoricalparams);

	distributionMap["CATEGORICAL"] = categorical;

	DistributionAdapter multinomial;
	multinomial.potential = &FactorFactory_BUGS::bugs_multinomial;
	multinomial.variabledtype = hazy::MULTINOMIAL;
	multinomial.variableptype = ZEROTON;
	multinomial.upperboundparameter = 1; // 2nd parameter is the upperbound
	DistributionParametersAdapter multinomialparams1;
	multinomialparams1.type = hazy::REAL;
	multinomialparams1.ptype = ZEROTOONE;
	multinomialparams1.variablegrouptype = LISTOFVARIABLE;
	multinomial.parametertype.push_back(multinomialparams1);
	DistributionParametersAdapter multinomialparams2;
	multinomialparams2.type = hazy::MULTINOMIAL;
	multinomialparams2.ptype = ZEROTON;
	multinomialparams2.variablegrouptype = SINGLETON;
	multinomial.parametertype.push_back(multinomialparams2);

	distributionMap["MULTINOMIAL"] = multinomial;

	DistributionAdapter dirichlet;
	dirichlet.potential = &FactorFactory_BUGS::bugs_dirichlet;
	dirichlet.variabledtype = hazy::REAL;
	dirichlet.variableptype = ZEROTOONE;
	DistributionParametersAdapter dirichletparams1;
	dirichletparams1.type = hazy::REAL;
	dirichletparams1.ptype = ZEROTOINF;
	dirichletparams1.variablegrouptype = LISTOFVARIABLE;
	dirichletparams1.upperbound = DIRICHLET_UPPER_BOUND;
	dirichlet.parametertype.push_back(dirichletparams1);

	distributionMap["DIRICHLET"] = dirichlet;

	DistributionAdapter dirichlet_with_constant_alpha;
	dirichlet_with_constant_alpha.potential = &FactorFactory_BUGS::bugs_dirichlet_with_constant_alpha;
	dirichlet_with_constant_alpha.variabledtype = hazy::REAL;
	dirichlet_with_constant_alpha.variableptype = ZEROTOONE;
	DistributionParametersAdapter dirichlet_with_constant_alphaparams1;
	dirichlet_with_constant_alphaparams1.type = hazy::REAL;
	dirichlet_with_constant_alphaparams1.ptype = ZEROTOINF;
	dirichlet_with_constant_alphaparams1.variablegrouptype = LISTOFVARIABLE;
	dirichlet_with_constant_alphaparams1.upperbound = DIRICHLET_UPPER_BOUND;
	dirichlet_with_constant_alpha.parametertype.push_back(dirichlet_with_constant_alphaparams1);

	distributionMap["DIRICHLET_WITH_CONSTANT_ALPHA"] = dirichlet_with_constant_alpha;

	DistributionAdapter mvnormal;
	mvnormal.potential = &FactorFactory_BUGS::bugs_mvnormal;
	mvnormal.variabledtype = hazy::REAL;
	mvnormal.variableptype = MINUSINFTOINF;
	mvnormal.upperbound = MVNORMAL_UPPER_BOUND;
	mvnormal.lowerbound = MVNORMAL_LOWER_BOUND;
	DistributionParametersAdapter mvnormalparams1;
	mvnormalparams1.type = hazy::REAL;
	mvnormalparams1.ptype = MINUSINFTOINF;
	mvnormalparams1.upperbound = MVNORMAL_UPPER_BOUND;
	mvnormalparams1.lowerbound = MVNORMAL_LOWER_BOUND;
	mvnormalparams1.variablegrouptype = LISTOFVARIABLE;
	mvnormal.parametertype.push_back(mvnormalparams1);
	DistributionParametersAdapter mvnormalparams2;
	mvnormalparams2.type = hazy::REAL;
	mvnormalparams2.ptype = MINUSINFTOINF;
	mvnormalparams2.upperbound = MVNORMAL_UPPER_BOUND;
	mvnormalparams2.lowerbound = MVNORMAL_LOWER_BOUND;
	mvnormalparams1.variablegrouptype = LISTOFVARIABLE;
	mvnormal.parametertype.push_back(mvnormalparams2);

	distributionMap["MVNORMAL"] = mvnormal;

	DistributionAdapter mvstudentt;
	mvstudentt.potential = &FactorFactory_BUGS::bugs_mvstudentt;
	mvstudentt.variabledtype = hazy::REAL;
	mvstudentt.variableptype = MINUSINFTOINF;
	mvstudentt.upperbound = MVSTUDENTT_UPPER_BOUND;
	mvstudentt.lowerbound = MVSTUDENTT_LOWER_BOUND;
	DistributionParametersAdapter mvstudenttparams1;
	mvstudenttparams1.type = hazy::REAL;
	mvstudenttparams1.ptype = MINUSINFTOINF;
	mvstudenttparams1.upperbound = MVSTUDENTT_UPPER_BOUND;
	mvstudenttparams1.lowerbound = MVSTUDENTT_LOWER_BOUND;
	mvstudenttparams1.variablegrouptype = LISTOFVARIABLE;
	mvstudentt.parametertype.push_back(mvstudenttparams1);
	DistributionParametersAdapter mvstudenttparams2;
	mvstudenttparams2.type = hazy::REAL;
	mvstudenttparams2.ptype = MINUSINFTOINF;
	mvstudenttparams2.upperbound = MVSTUDENTT_UPPER_BOUND;
	mvstudenttparams2.lowerbound = MVSTUDENTT_LOWER_BOUND;
	mvstudenttparams2.variablegrouptype = LISTOFVARIABLE;
	mvstudentt.parametertype.push_back(mvstudenttparams2);
	DistributionParametersAdapter mvstudenttparams3;
	mvstudenttparams3.type = hazy::REAL;
	mvstudenttparams3.ptype = NTOINF;
	mvstudenttparams3.upperbound = MVSTUDENTT_UPPER_BOUND;
	mvstudenttparams3.lowerbound = 2;
	mvstudenttparams3.variablegrouptype = SINGLETON;
	mvstudentt.parametertype.push_back(mvstudenttparams3);

	distributionMap["MVSTUDENTT"] = mvstudentt;

//	DistributionAdapter wishart;
//	wishart.potential = &FactorFactory_BUGS::bugs_wishart;
//	wishart.variabledtype = hazy::REAL;
//	wishart.variableptype = MINUSINFTOINF;
//	wishart.upperbound = WISHART_UPPER_BOUND;
//	wishart.lowerbound = WISHART_LOWER_BOUND;
//	DistributionParametersAdapter wishartparams1;
//	wishartparams1.type = hazy::REAL;
//	wishartparams1.ptype = MINUSINFTOINF;
//	wishartparams1.upperbound = WISHART_UPPER_BOUND;
//	wishartparams1.lowerbound = WISHART_LOWER_BOUND;
//	wishartparams1.variablegrouptype = LISTOFVARIABLE;
//	wishart.parametertype.push_back(wishartparams1);
//	DistributionParametersAdapter wishartparams2;
//	wishartparams2.type = hazy::REAL;
//	wishartparams2.ptype = ZEROTOINF;
//	wishartparams2.upperbound = WISHART_UPPER_BOUND;
//	wishartparams2.variablegrouptype = SINGLETON;
//	wishart.parametertype.push_back(wishartparams2);
//
//	distributionMap["WISHART"] = wishart;

	return distributionMap;
}

}
