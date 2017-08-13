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
//  Filename: STATE_GENERAL.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_STATE_GENERAL_h
#define Elementary0_3_STATE_GENERAL_h


#include "../util/Common.h"

#include "../fg/VariableGroup.h"
#include "../fg/Variable.h"

namespace hazy{

    class STATE_GENERAL{
  
    public:
        
        int parameterID = -1;
        
        int ngroup = MAX_GROUP_PER_STATE;
                
        VariableGroup vgroups[MAX_GROUP_PER_STATE];
        
        void init_group(int group_id, double _aux){
            assert(group_id < MAX_GROUP_PER_STATE);
            vgroups[group_id].aux = _aux;
        }
        
        void init_var(int group_id, const Variable & v, const int aux){
            assert(group_id < MAX_GROUP_PER_STATE);
            vgroups[group_id].push(v.oid, v.cvalue, aux);
        }
        
        template<class ASSIGNMENT_KV>
        void mat(ASSIGNMENT_KV & assignments){

	  //TODO
	  /*
	  Assignment assign;
            for(int i=0;i<ngroup;i++){
                for(int j=0;j<vgroups[i].nvar;j++){
		  assignments.get_oid(vgroups[i].hypothesis[j].vid, assign);
		  vgroups[i].hypothesis[j].cvalue = assign.cvalue;
                }
            }
	  */
        }
        
        void update_var(const Variable & v, const double cvalue){
            for(int i=0;i<ngroup;i++){
                for(int j=0;j<vgroups[i].nvar;j++){
                    if(vgroups[i].hypothesis[j].vid == v.oid){
                        vgroups[i].hypothesis[j].cvalue = cvalue;
                    }
                }
            }
        }
        
        void print(){
            std::cout << " NGROUP=" << ngroup << std::endl;
            for(int i=0;i<ngroup;i++){
                std::cout << "        GROUP " << i << " ";
                for(int j=0;j<vgroups[i].nvar;j++){
                    std::cout << " [ID=" << vgroups[i].hypothesis[j].vid << ";VALUE=" << vgroups[i].hypothesis[j].cvalue << "]";
                }
                std::cout << std::endl;
            }
        }
        
        STATE_GENERAL(){}
    
    };
    
    double test_potential(const STATE_GENERAL& state, const Variable & v, const double & cvalue){
        return cvalue;
    }
    
    void test_vg(const STATE_GENERAL& state, VariableGroup& vgroup){
        
        vgroup.nvar = 2;
        vgroup.hypothesis[0].vid = 3;
        vgroup.hypothesis[0].cvalue = 1.45;
        
        vgroup.hypothesis[1].vid = 2;
        vgroup.hypothesis[1].cvalue = 2.45;
        
        return;
    }

    double potential_null(const STATE_GENERAL& state, const Variable & v, const double & cvalue){
      return 1;
    }

    void vg_assign(const STATE_GENERAL& state, VariableGroup& vgroup){

      assert(state.vgroups[0].aux == SINGLETON);
      assert(state.vgroups[1].aux == SINGLETON);

      vgroup.nvar = 1;
      vgroup.hypothesis[0].vid = state.vgroups[0].hypothesis[0].vid;
      vgroup.hypothesis[0].cvalue = state.vgroups[1].hypothesis[0].cvalue;

      return;
    }


    inline double getReturnValue(double result) {
    	if (result != result) // happens when x is Nan
    		return -DBL_MAX; // since e ^ -inf  = 0 and using hazy::LOG
    	else if (result <= -DBL_MAX)
    		return -DBL_MAX;
    	else if (result >= DBL_MAX)
    		return DBL_MAX;
    	return result;
    }

    void processNestedIndexArray(int & noofindices, const VariableGroup & vgroup, const Variable & v,
    		int indexvalues[], const double & cvalue,
    		int sizeofindices[], vector<float> & allvariables)
    {
        int dim = 0;
        for(int i = 1;i < noofindices + 1;i++){
            if(vgroup.hypothesis[i].vid == v.oid){
                indexvalues[dim++] = cvalue;
            }else{
                indexvalues[dim++] = vgroup.hypothesis[i].cvalue;
            }
//           cout << " Index value " << indexvalues[dim-1] <<endl;
        }

        //get dimensions of all indices
        dim = 0;
        for(int i = noofindices + 1;i < 2 * noofindices + 1;i++){
            sizeofindices[dim++] = vgroup.hypothesis[i].cvalue;
//            cout << " sizeofindices " << sizeofindices[dim - 1] <<endl;
        }
        //get all variables
        for(int i = 2 * noofindices + 1;i < vgroup.nvar && vgroup.hypothesis[i].aux == NESTED_INDEX_ARRAY;i++){
            if(vgroup.hypothesis[i].vid == v.oid){
                allvariables.push_back(cvalue);
            }else{
//                cout << " pushing back here " << vgroup.hypothesis[i].cvalue << endl;
                allvariables.push_back(vgroup.hypothesis[i].cvalue);
            }
        }
    }


	inline double eval_variable_group(const VariableGroup & vgroup, const Variable & v, const double & cvalue){

		if (vgroup.aux == SINGLETON) {
			if (vgroup.hypothesis[0].vid == v.oid) {
				return cvalue;
			} else {
				return vgroup.hypothesis[0].cvalue;
			}
		}

		else if (vgroup.aux == NESTED_INDEX_ARRAY) {
			int noofindices = vgroup.hypothesis[0].cvalue;
			assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing

			int indexvalues[noofindices], sizeofindices[noofindices];
			vector<float> allvariables;
			processNestedIndexArray(noofindices, vgroup, v, indexvalues, cvalue, sizeofindices, allvariables);

    		int indextogetvariablefor = 0;
    		for(int i = 0; i < noofindices; i++){
    			int sizetomultiply = 1;
    			for(int j = i+1; j < noofindices; j++)
    				sizetomultiply = sizetomultiply * sizeofindices[j];

    			if(indexvalues[i] != 0) // This is a hotfix for sampling problem
    				indextogetvariablefor += (indexvalues[i] - 1) * sizetomultiply;
    		}

    		assert(indextogetvariablefor < allvariables.size());
    		return allvariables[indextogetvariablefor];
		}

		assert(false);
		return 0;
	}

    inline void evaluate_list_of_variables(const VariableGroup & vgroup, const Variable & v, const double & cvalue, double arraytofill[]) {
    	if(vgroup.aux == LISTOFVARIABLE) {
			for(int i = 0 ;i < vgroup.nvar; i++) {
				if (vgroup.hypothesis[i].vid == v.oid) {
					arraytofill[i] = cvalue;
				} else {
					arraytofill[i] = vgroup.hypothesis[i].cvalue;
				}
			}
    	}

    	else if(vgroup.aux == NESTED_INDEX_ARRAY) {
    		int noofindices = vgroup.hypothesis[0].cvalue;
    		assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing
    		/*
    		 * No Of Indices, Index1, Index2...,
    		 * Size of Index1, Size of Index2, ...
    		 * Data...
    		 * Indexing Algo: var1= (Index1 - 1) * [size of index 2 * size of Index 3....] +
    		 * (Index2 - 1) * [size of Index 3 * size of index 4 ....]
    		 */

    		//get index values
    		int indexvalues[noofindices], sizeofindices[noofindices];
    		vector<float> allvariables;
    		processNestedIndexArray(noofindices, vgroup, v,indexvalues, cvalue, sizeofindices, allvariables);

    		int indextogetvariablefor = 0;
    		for(int i = 0; i < noofindices - 1; i++){
    			int sizetomultiply = 1;

    			for(int j = i+1; j < noofindices; j++)
    				sizetomultiply = sizetomultiply * sizeofindices[j];

    			if(indexvalues[i] != 0) // This is a hotfix for sampling problem
    				indextogetvariablefor += (indexvalues[i] - 1) * sizetomultiply;
    		}

    		int dim = 0;
    		for(int i=indextogetvariablefor; i < indextogetvariablefor + sizeofindices[noofindices-1];i++) {
    			assert(i < allvariables.size());
    			arraytofill[dim++] = allvariables[i];
    		}
    	}
    	else
    		assert(false);
    }

    void vg_logicalnode_assign(const STATE_GENERAL& state, VariableGroup& vgroup){

        assert(state.vgroups[0].aux == SINGLETON);
        assert(state.vgroups[1].aux == SINGLETON || state.vgroups[1].aux == SINGLETONEVID);

        /*
         * when the logical node itself is evidence (calculated in the parser stage itself), then there is no need to evaluate the logical node expression.
         * Just assign and return the value
         */
        if(state.vgroups[1].aux == SINGLETONEVID)
        {
        	vgroup.nvar = 1;
			vgroup.hypothesis[0].vid = state.vgroups[1].hypothesis[0].vid;
			vgroup.hypothesis[0].cvalue = state.vgroups[1].hypothesis[0].cvalue;
			return;
        }

        double mu = 0;
        int noOfVar = state.vgroups[0].nvar;
        int variablecount = 0;
        double variables[noOfVar];
//        cout << " No of var " << noOfVar << endl;

        for (int i = 0; i < noOfVar; i++){ // taking all the variables to be sent to eval function of Logical Node Expression

			assert(state.vgroups[0].hypothesis[i].aux != SPECIAL_SUM);
			assert(state.vgroups[0].hypothesis[i].aux != SPECIAL_COUNT);

			if(state.vgroups[0].hypothesis[i].aux == SPECIAL_NODE_WITH_NODE_INDEX){
				int noofindices = state.vgroups[0].hypothesis[i].cvalue;
//				cout << " Node with node index: No of Indices " << noofindices <<endl;
				assert(noofindices <= 1); //TODO Support more indices here. Only 1 index in logical node is now supported

				int index = noofindices;
				for (int j = i + 1; j <= i + noofindices && j < noOfVar; j++)
				{
					assert(state.vgroups[0].hypothesis[j].aux == SPECIAL_NODE_WITH_NODE_INDEX);
					index = index + state.vgroups[0].hypothesis[j].cvalue;
				}

				i += noofindices;
//				cout << " Incrementing index " << i << endl;
				for (int k = i + 1; k < noOfVar && state.vgroups[0].hypothesis[k].aux == SPECIAL_NODE_WITH_NODE_INDEX; k++)
				{
					i += 1;
//					cout << " Incrementing index " << i << endl;
				}

				assert(index <= i);
//				cout << "INdex is " << index << " value is " << state.vgroups[0].hypothesis[index].cvalue << endl;
				variables[variablecount++] =  state.vgroups[0].hypothesis[index].cvalue;
			}

			else if(state.vgroups[0].hypothesis[i].aux == SPECIAL_DATA)
				variables[variablecount++] = state.vgroups[0].hypothesis[i].cvalue;
			else
				variables[variablecount++] = state.vgroups[0].hypothesis[i].cvalue;
//			std::cout << " variables [" << i << "] = " << variables[i] << "ID= " << state.vgroups[0].hypothesis[i].vid << std::endl;
		}

		double evalresult = hazy::logicalnodeexpressions[state.parameterID]->evalAsData(variables);
     	mu = evalresult;

     	vgroup.nvar = 1;
		vgroup.hypothesis[0].vid = state.vgroups[1].hypothesis[0].vid;
		vgroup.hypothesis[0].cvalue = mu;

//		std::cout << "VG_ASSIGN: ASSIGN VAR " << vgroup.hypothesis[0].vid << " " << mu << std::endl;
        return;
       }

    void vg_inverse_logicalnode_assign(const STATE_GENERAL& state, VariableGroup& vgroup){
    	assert(state.vgroups[0].aux == LISTOFVARIABLE);
    	assert(state.vgroups[1].aux == LISTOFVARIABLE);
    	assert(state.vgroups[0].nvar == state.vgroups[1].nvar);

    	matrix mattofindinverse;
		mattofindinverse.n = sqrt(state.vgroups[0].nvar);
		mattofindinverse.col = mattofindinverse.n;
		for (int i = 1; i <= mattofindinverse.n; i++) {
			for (int j = 1; j <= mattofindinverse.n; j++) {
				mattofindinverse.a[i][j] = state.vgroups[0].hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].cvalue;
//				std::cout << " ARRAY [" << i << "," << j << "] = " << mattofindinverse.a[i][j] << std::endl;
			}
		}

		vgroup.nvar = state.vgroups[1].nvar;
		matrix inverse_mat = inverse(mattofindinverse);
		for (int i = 1; i <= mattofindinverse.n; i++) {
			for (int j = 1; j <= mattofindinverse.n; j++) {
				vgroup.hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].vid = state.vgroups[1].hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].vid;
				vgroup.hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].cvalue = inverse_mat.a[i][j];
			}
		}
    }

    double potential_norm(const STATE_GENERAL& state, const Variable & v, const double & cvalue){

      double N = eval_variable_group(state.vgroups[0], v, cvalue);
      double mu = eval_variable_group(state.vgroups[1], v, cvalue);
      double sigma = eval_variable_group(state.vgroups[2], v, cvalue);

      return 1.0/sigma*exp(-(N-mu)*(N-mu)/2/sigma/sigma);
    }

    inline double bugs_bernoulli(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double p = eval_variable_group(state.vgroups[0], v, cvalue);
    	int R = eval_variable_group(state.vgroups[1], v, cvalue);

    	if (R == 1)
    		return getReturnValue(log(p));
    	else if (R == 0)
    		return getReturnValue(log(1 - p));

    	return 0;
    }


    inline double bugs_binomial(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	//get p, N, R
    	double p = eval_variable_group(state.vgroups[0], v, cvalue);
    	int N = eval_variable_group(state.vgroups[1], v, cvalue);
    	int R = eval_variable_group(state.vgroups[2], v, cvalue);

    	double logcomb = LogCombinations(N, R);

    	if(R == 0 && p == 0)
    		return 0;
    	else if (p==0)
    		return -10;

    	double result = logcomb + R * log(p) + (N - R) * log(1-p);
    	return getReturnValue(result);
    }

    inline double bugs_negativebinomial(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	double p = eval_variable_group(state.vgroups[0], v, cvalue);
		int R = eval_variable_group(state.vgroups[1], v, cvalue);
		int X = eval_variable_group(state.vgroups[2], v, cvalue);

    	double logcomb = LogCombinations(X + R - 1, X);

    	double result = logcomb + R * log(p) + X * log(1 - p);
    	return getReturnValue(result);
    }


    inline double bugs_poisson(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	double lambda = eval_variable_group(state.vgroups[0], v, cvalue);
    	int R = eval_variable_group(state.vgroups[1], v, cvalue);

    	double result = -lambda + R * log(lambda) - LogFactorial(R);
    	return getReturnValue(result);
    }

    /*
     * Implemented Fisher's Non Central Hypergeometric distribution.
     * http://en.wikipedia.org/wiki/Fisher's_noncentral_hypergeometric_distribution
     */

    inline double bugs_noncentralhypergeometric(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	//get m1
    	int m1 = eval_variable_group(state.vgroups[0], v, cvalue);

    	//get m2
    	int m2 = eval_variable_group(state.vgroups[1], v, cvalue);

    	//get n
    	int n = eval_variable_group(state.vgroups[2], v, cvalue);

    	//get psi
    	double w = eval_variable_group(state.vgroups[3], v, cvalue);

    	//get x in range xmin, xmax. xmin = max(0, n- m2); xmax = min(n,m1)
    	double x = eval_variable_group(state.vgroups[4], v, cvalue);

    	double logcomb1 = LogCombinations(m1, x);
    	double logcomb2 = LogCombinations(m2, n - x);
    	double lognumerator = logcomb1 + logcomb2 + x * log(w);

    	int xmin = std::max(0, n - m2);
    	int xmax = std::min(n, m1);
    	double denominator = 0;
    	for (int i = xmin; i <= xmax; i++) {
    		denominator = denominator + Combinations(m1, i) * Combinations(m2, n - i) * Power(w, i);
    	}

    	//	  std::cout << " num = " << numerator << " den " << denominator << std::endl;
    	double result = lognumerator - log(denominator);
    	return getReturnValue(result);
    }


    inline double bugs_beta(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double a = eval_variable_group(state.vgroups[0], v, cvalue);
    	double b = eval_variable_group(state.vgroups[1], v, cvalue);
    	double p = eval_variable_group(state.vgroups[2], v, cvalue);
    	//	std::cout << "a = " << a << " b =" << b << " p = " << p << " aans " << Gamma(a+b) << " " << Power(p, a - 1.0) << " " << Power(1.0 - p, b - 1.0) << " " << Gamma(a) << " " << Gamma(b) <<std::endl;

    	double result = log(Gamma(a + b)) + (a - 1.0) * log(p) + (b	- 1.0) * log(1.0 - p)  - log(Gamma(a)) - log(Gamma(b));
    	return getReturnValue(result);
    }


    inline double bugs_chisquared(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double k = eval_variable_group(state.vgroups[0], v, cvalue);
    	double x = eval_variable_group(state.vgroups[1], v, cvalue);

    	double result = ((k / 2.0) - 1.0) * log(x) + (-1.0 * x / 2.0) -  (k / 2.0) * log(2) -log(Gamma(k / 2.0));
    	return getReturnValue(result);
    }


    inline double bugs_doubleexponential(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double tau = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	double result = log(tau / 2) + (-1.0 * fabs(x - mu) * tau);
    	return getReturnValue(result);
    }


    inline double bugs_gamma(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double r = eval_variable_group(state.vgroups[0], v, cvalue);
    	double mu = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	double result = r * log(mu) + (r - 1.0) * log(x) + (-1.0 * mu * x) - log(Gamma(r));
//    	cout << " result = " << result << " return value " << getReturnValue(result) << endl;
    	return getReturnValue(result);
    }


    inline double bugs_exponential(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double lambda = eval_variable_group(state.vgroups[0], v, cvalue);
    	double x = eval_variable_group(state.vgroups[1], v, cvalue);

    //	std::cout << " Lambda = " << lambda << " x = " << x << std::endl;
    	double result = log(lambda) - lambda * x;
    	return getReturnValue(result);
    }

    /*
     * Flat Distribution is approximated to a Normal with Mean 0 and high variance
     */

    inline double bugs_flat(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	double mu = 0;
    	double sigma = 10000000;
    	double value = eval_variable_group(state.vgroups[0], v, cvalue);

    	double result = -log(sigma) -log(sqrt(2 * MATH_PI)) + (-0.5 * ((value - mu) / sigma) * ((value - mu) / sigma));
    //	cout << "sigma " << sigma << " value " << value << " mu " << mu << " result " << result << " Return = " << exp(getReturnValue(result)) << endl;
    	return getReturnValue(result);
    }


    inline double bugs_generalizedextremevalue(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double sigma = eval_variable_group(state.vgroups[1], v, cvalue);
    	double eta = eval_variable_group(state.vgroups[2], v, cvalue);
    	double x = eval_variable_group(state.vgroups[3], v, cvalue);

    	double result = -log(sigma) + (-1.0 - 1.0 / eta) * log(1.0 + (eta * ((x - mu) / sigma)))
    			 + (-1 * Power((1.0 + eta * ((x	- mu) / sigma)), (-1.0 / eta)));

    	return getReturnValue(result);
    }


    inline double bugs_generalizedgamma(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double r = eval_variable_group(state.vgroups[0], v, cvalue);
    	double mu = eval_variable_group(state.vgroups[1], v, cvalue);
    	double beta = eval_variable_group(state.vgroups[2], v, cvalue);
    	double x = eval_variable_group(state.vgroups[3], v, cvalue);

    	double result = log(beta) - log(Gamma(r)) + (beta * r) * log(mu) + (beta * r - 1.0) * log(x) + (-1 * Power(mu * x, beta));
    //	std::cout << "r " << r << " mu " << mu << " beta  " << beta << " x " << x << " ans " << result << std::endl;
    	return getReturnValue(result);
    }

    inline double bugs_generalizedpareto(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double sigma = eval_variable_group(state.vgroups[1], v, cvalue);
    	double eta = eval_variable_group(state.vgroups[2], v, cvalue);
    	double x = eval_variable_group(state.vgroups[3], v, cvalue);

    	double result = -log(sigma) + (-1.0 - 1.0 / eta) * log(1.0 + eta * (x - mu) / sigma) + Power(-(1.0 + (eta/sigma)*(x - mu)), (-1.0 / eta));
    	return getReturnValue(result);
    }

    inline double bugs_logistic(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double tau = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	double value = Power(MATH_E, tau * (x - mu));
    	double result = log(tau) + (tau * (x - mu)) - 2 * log((1.0 + value));
    	return getReturnValue(result);
    }


    inline double bugs_genericloglikelihood(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double lambda = eval_variable_group(state.vgroups[0], v, cvalue);

    	double result = lambda;
    	return getReturnValue(result);
    }


    inline double bugs_lognormal(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double tau = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	 // Lognormal x should not be less than zero
    	assert(x >= 0);
    	double result = -log(x) + log(sqrt(tau / 2 * MATH_PI)) + (-1.0 * ((log(x) - mu) * (log(x) - mu)) * tau / 2);

    //	std::cout << " x = " << x << " Mu = " << mu << " tau= " << tau << " Result = " << result << " Return value = " << exp(getReturnValue(result)) << std::endl;
    	return getReturnValue(result);
    }


    inline double bugs_normal(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double tau = eval_variable_group(state.vgroups[1], v, cvalue);
    	double value = eval_variable_group(state.vgroups[2], v, cvalue);
    	double sigma = 1.0 / sqrt(tau);

    	double result = -log(sigma) -log(sqrt(2 * MATH_PI)) + (-0.5 * ((value - mu) / sigma) * ((value - mu) / sigma));
//    	cout << "sigma " << sigma << " value " << value << " mu " << mu << " result " << result << endl;
    	return getReturnValue(result);
    }


    inline double bugs_pareto(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double alpha = eval_variable_group(state.vgroups[0], v, cvalue);
    	double c = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	if(x < c)
    		return -10;
    	double result = log(alpha) + alpha * log(c) - (alpha + 1) * log(x);
    	return getReturnValue(result);
    }


    inline double bugs_studentt(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double mu = eval_variable_group(state.vgroups[0], v, cvalue);
    	double tau = eval_variable_group(state.vgroups[1], v, cvalue);
    	double k = eval_variable_group(state.vgroups[2], v, cvalue);
    	double x = eval_variable_group(state.vgroups[3], v, cvalue);

    	double p = (k + 1.0) / 2.0;
    	double q = (x - mu) * (x - mu);

    	//	std::cout << " Mu= " << mu << " tau= " << tau << " k= " << k << " x= " << x << " ans= " <<
    	//			(Gamma(p)/Gamma(k/2.0)) * sqrt(tau/(k*MATH_PI)) * Power((1.0 + (tau/k)*q), -1*p) << std::endl;
    	double result = log(Gamma(p)) - log(Gamma(k / 2.0)) + log(sqrt(tau / (k * MATH_PI))) + (-1 * p) * log(1.0 + (tau / k) * q);
    	return getReturnValue(result);
    }


    inline double bugs_uniform(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double a = eval_variable_group(state.vgroups[0], v, cvalue);
    	double b = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	double result = -log(b - a);
    	return getReturnValue(result);
    }


    inline double bugs_weibull(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	double v1 = eval_variable_group(state.vgroups[0], v, cvalue);
    	double lambda = eval_variable_group(state.vgroups[1], v, cvalue);
    	double x = eval_variable_group(state.vgroups[2], v, cvalue);

    	double result = log(v1) + log(lambda) + (v1 - 1.0) * log(x) + -1 * lambda * Power(x, v1);
    	//	std::cout << " v1 = " << v1 << " lamba " << lambda << " x = " << x << " result " << result << std::endl;
    	return getReturnValue(result);
    }

    /*
     * Bounds [1, size of p]. Sum of all p = 1
     */

    inline double bugs_categorical(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	int num_p = state.vgroups[0].nvar;
    	double p[num_p];

    	evaluate_list_of_variables(state.vgroups[0], v, cvalue, p);
    	int r = eval_variable_group(state.vgroups[1], v, cvalue);

//    	std::cout << " r = " << r << std::endl;
    	double result = log(p[r - 1]);
//    	std::cout << " result = " << p[r-1] << std::endl;
    	return getReturnValue(result);
    }

    /*
     * http://en.wikipedia.org/wiki/Multinomial_distribution
     * Last Evidence variable is the number of Random Variables
     */

    inline double bugs_multinomial(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
//    	int num_p = state.vgroups[0].nvar;
    	int num_x = state.vgroups[2].nvar;
    	int num_p = num_x; // due to the fact that there can be index within index.

//    	if(num_p != num_x)
//    	{
//    		std::cout << "Num of Probability values is not equal to number of random variables" <<std::endl;
//    		assert(false);
//    	}

    	double p[num_p];
    	double x[num_x];


    	evaluate_list_of_variables(state.vgroups[0], v, cvalue, p);
    	int N = eval_variable_group(state.vgroups[1], v, cvalue);

//    	std::cout << "N =" << N << std::endl;
    	int count_x = 0;
    	evaluate_list_of_variables(state.vgroups[2], v, cvalue, x);

    	for(int i = 0 ; i < num_x; i++)
    		count_x += x[i];

    	if(count_x != N)
    		return -10; // Because e^-10 ~ 0. Or else will cause underflow


    	double logfactorial_xs = 0;
    	for(int i = 0 ; i < num_x; i++) {
    		logfactorial_xs = logfactorial_xs + LogFactorial(x[i]);
    	}

    	double logproduct_p = 0;
    	for(int i = 0 ; i < num_p; i++) {
    		logproduct_p = logproduct_p + x[i] * log(p[i]);
    	}

    //	std::cout << " Num x = " << num_x << " Num_p = " << num_p << std::endl;

    	double result = LogFactorial(N) + logproduct_p - logfactorial_xs;
    	return getReturnValue(result);
    }

    /*
     * http://en.wikipedia.org/wiki/Dirichlet_distribution
     * Last Evidence variable is the number of Random Variables
     * Last - 1 = > Parameter 2
     * Last - 2 = > Parameter 1
     */

    inline double bugs_dirichlet(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	int num_p = state.vgroups[1].nvar;
    	int num_alpha = state.vgroups[0].nvar;

    	if(num_p != num_alpha)
    	{
    		std::cout << "Num of Probability values is not equal to number of random variables" <<std::endl;
    		assert(false);
    	}

    	double p[num_p];
    	double alpha[num_alpha];

    //	std::cout << " Num alpha  = " << num_alpha << " Num_p = " << num_p << std::endl;

    	double alpha_sum = 0.0;
    	evaluate_list_of_variables(state.vgroups[0], v, cvalue, alpha);
    	for(int i = 0 ; i < num_alpha; i++)
    		alpha_sum += alpha[i];
    //		std::cout << " alpha[" << i << "] = " << alpha[i] <<endl;

    	double sum_p = 0.0;
    	evaluate_list_of_variables(state.vgroups[1], v, cvalue, p);
    	for(int i = 0 ;i < num_p; i++)
    		sum_p += p[i];
    //		std::cout << " p[" << i << "] = " << p[i] <<endl;

    	if(!(sum_p >= 0.96 && sum_p <= 1.0000001))
    		return -10; // Dirichlet is used as a prior dist

    	double loggamma_alpha = 1.0;
    	for(int i = 0 ; i < num_alpha; i++) {
    		loggamma_alpha = loggamma_alpha + log(Gamma(alpha[i]));
    	}

    	double logbetafunction = loggamma_alpha - log(Gamma(alpha_sum));

    	double logproduct_p = 1;
    	for(int i = 0 ; i < num_p; i++) {
    		logproduct_p = logproduct_p  + (alpha[i] - 1) * log(p[i]);
    	}

    	double result = logproduct_p - logbetafunction;

    //	std::cout << " ***************Result************** " << result << endl;
    	return getReturnValue(result);
    }


    inline double bugs_mvnormal(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	int num_mu = state.vgroups[0].nvar;
    	int num_t = state.vgroups[1].nvar;
    	int num_x = state.vgroups[2].nvar;

    	if(num_t != Power(num_mu, 2)  || num_mu != num_x)
    	{
    		std::cout << "MVNORMAL Size of T should be square of size of mu. Also size of mu should be size of x " << std::endl;
    		assert(false);
    	}

    	double mu[num_mu];
    	double t[num_mu][num_mu];
    	double x[num_x];

    //	std::cout << " Num mu  = " << num_mu << " Num_t = " << num_t << " Num_x = " << num_x << std::endl;

    	evaluate_list_of_variables(state.vgroups[0], v, cvalue, mu);

    	assert(state.vgroups[1].aux == LISTOFVARIABLE);
    	for(int i = 0 ;i < num_mu; i++) {
    		for(int j = 0 ; j < num_mu; j++) {
				if(v.oid == state.vgroups[1].hypothesis[num_mu * i + j].vid)
					t[i][j] = cvalue;
				else
					t[i][j] = state.vgroups[1].hypothesis[num_mu * i + j].cvalue;
//				std::cout << " ARRAY[" << i << "," << j << "] = " << t[i][j] << std::endl;
			}
    	}

    	evaluate_list_of_variables(state.vgroups[2], v, cvalue, x);

    	/*
    	 * calculate det of t, inverse of t.
    	 * calculate x-mu and x-mu transpose
    	 */

    	matrix det;
    	det.n = num_mu;
    	det.col = num_mu;
    	for (int i = 1; i <= num_mu; i++) {
    		for (int j = 1; j <= num_mu; j++) {
    			det.a[i][j] = t[i - 1][j - 1];
    		}
    	}
    	double determinant_t = determinant(det);
    //	std::cout << " Determinant = " << determinant_t <<endl;

    	matrix inverse_t = inverse(det);

    	matrix x_mu, x_mu_transpose;
    	x_mu.n = 1;
    	x_mu.col = num_mu;

    	x_mu_transpose.n = num_mu;
    	x_mu_transpose.col = 1;

    	inverse_t.n = num_mu;
    	inverse_t.col = num_mu;

    	for (int i = 1; i <= num_mu; i++) {
    		x_mu.a[1][i] = x[i - 1] - mu[i - 1];
    		x_mu_transpose.a[i][1] = x[i - 1] - mu[i - 1];
    //		std::cout << " x - mu [1," << i << "] = " << x_mu.a[1][i] << " ";
    	}
    //	std::cout << std::endl;
    	matrix product_x_mu_inverse_t = matmultiply(x_mu, inverse_t);
    	matrix matresult = matmultiply(product_x_mu_inverse_t, x_mu_transpose);

    	if(matresult.n != 1 && matresult.col != 1)
    	{
    		std::cout << " Something went wrong while multiplying matrices in mvnormal " << std::endl;
    		assert(false);
    	}

    	double logdenominator = (num_mu/2) * log(2*MATH_PI) + 0.5 * log(determinant_t);
    	double lognumerator =  -0.5 * matresult.a[1][1];
    	double result = lognumerator - logdenominator;

    //	std::cout << " ***************Result************** " << result << " numerator " << lognumerator << " denominator " << logdenominator << endl;
    	return getReturnValue(result);
    }


    inline double bugs_mvstudentt(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {
    	int num_mu = state.vgroups[0].nvar;
		int num_t = state.vgroups[1].nvar;
		int num_x = state.vgroups[3].nvar;

    	if(num_t != Power(num_mu, 2)  || num_mu != num_x)
    	{
    		std::cout << "Size of T should be square of size of mu. Also size of mu should be size of x " << std::endl;
    		assert(false);
    	}

    	double mu[num_mu];
    	double t[num_mu][num_mu];
    	double x[num_x];

    //	std::cout << " Num mu  = " << num_mu << " Num_t = " << num_t << " Num_x = " << num_x << std::endl;
    	evaluate_list_of_variables(state.vgroups[0], v, cvalue, mu);

    	for(int i = 0 ;i < num_mu; i++)
    	{
    		for(int j = 0 ; j < num_mu; j++)
    		{
    			if(v.oid == state.vgroups[1].hypothesis[num_mu * i + j].vid)
					t[i][j] = cvalue;
				else
					t[i][j] = state.vgroups[1].hypothesis[num_mu * i + j].cvalue;
    //			std::cout << " t[" << i << "," << j << "] = " << t[i][j] <<endl;
    		}
    	}

    	int k = eval_variable_group(state.vgroups[2], v, cvalue);
    	evaluate_list_of_variables(state.vgroups[3], v, cvalue, x);

    	/*
    	 * calculate det of t, inverse of t.
    	 * calculate x-mu and x-mu transpose
    	 */

    	matrix det;
    	det.n = num_mu;
    	for (int i = 1; i <= num_mu; i++) {
    		for (int j = 1; j <= num_mu; j++) {
    			det.a[i][j] = t[i - 1][j - 1];
    		}
    	}
    	double determinant_t = determinant(det);
    //	std::cout << " Determinant = " << determinant_t <<endl;

    	matrix inverse_t = inverse(det);

    	matrix x_mu, x_mu_transpose;
    	x_mu.n = 1;
    	x_mu.col = num_mu;

    	x_mu_transpose.n = num_mu;
    	x_mu_transpose.col = 1;

    	inverse_t.n = num_mu;
    	inverse_t.col = num_mu;

    	for (int i = 1; i <= num_mu; i++) {
    		x_mu.a[1][i] = x[i - 1] - mu[i - 1];
    		x_mu_transpose.a[i][1] = x[i - 1] - mu[i - 1];
    	}

    	matrix product_x_mu_inverse_t = matmultiply(x_mu, inverse_t);
    	matrix matresult = matmultiply(product_x_mu_inverse_t, x_mu_transpose);
    	if(matresult.n != 1 && matresult.col != 1)
    	{
    		std::cout << " Something went wrong while multiplying matrices in mvnormal " << std::endl;
    		assert(false);
    	}

    	double logdenominator = log(Gamma(k/2.0)) + (num_mu/2) * log(k) + (num_mu/2) * log(MATH_PI) + 0.5 * log(determinant_t) + ((k + num_mu)/2.0) * log(1 + matresult.a[1][1]/k);
    	double lognumerator = log(Gamma((k + num_mu)/2.0));
    	double result = lognumerator- logdenominator;

    	return getReturnValue(result);
    }


    inline double bugs_wishart(const STATE_GENERAL& state, const Variable & v, const double & cvalue) {

    	int num_r = state.vgroups[0].nvar;
		int num_x = state.vgroups[2].nvar;

    	if(num_r != num_x)
    	{
    		std::cout << "size of r should be size of x " << std::endl;
    		assert(false);
    	}

    	int n = sqrt(num_x);
    	double r[n][n];
    	double x[n][n];

    //	std::cout << " Num_x = " << num_x << " n= " << n << std::endl;

    	for (int i = 0; i < n; i++) {
    		for (int j = 0; j < n; j++) {
    			if(v.oid == state.vgroups[0].hypothesis[n * i + j].vid)
					r[i][j] = cvalue;
				else
					r[i][j] = state.vgroups[0].hypothesis[n * i + j].cvalue;
//    			std::cout << " r[" << i << "," << j << "] = " << r[i][j] << std::endl;
    		}
    	}

    	int k = eval_variable_group(state.vgroups[1], v, cvalue);

//    	std::cout << " k = " << k << " " << state.vgroups[1].hypothesis[0].cvalue << " " << state.vgroups[1].nvar << std::endl;

    	for(int i = 0 ;i < n; i++)
    	{
    		for(int j = 0 ; j < n; j++)
    		{
    			if(v.oid == state.vgroups[2].hypothesis[n * i + j].vid)
					x[i][j] = cvalue;
				else
					x[i][j] = state.vgroups[2].hypothesis[n * i + j].cvalue;
//    			std::cout << " x[" << i << "," << j << "] = " << x[i][j] <<std::endl;
    		}
    	}

    	matrix det_r;
    	det_r.n = n;
    	det_r.col = n;
    	for (int i = 1; i <= n; i++) {
    		for (int j = 1; j <= n; j++) {
    			det_r.a[i][j] = r[i - 1][j - 1];
    		}
    	}

//    	matrix inverse_r = inverse(det_r);
    	double determinant_r = determinant(det_r);
    //	std::cout << " Determinant r= " << determinant_r <<endl;

    	matrix det_x;
    	det_x.n = n;
    	det_x.col = n;
    	for (int i = 1; i <= n; i++) {
    		for (int j = 1; j <= n; j++) {
    			det_x.a[i][j] = x[i - 1][j - 1];
    		}
    	}

    	double determinant_x = determinant(det_x);

    	/*
    	 * X Should be a symmetric Positive definite matrix.
    	 */
    	if(!isSymmetric(det_x))
    		return -10;

    	bool ispd = ispositivedefinite(det_x, det_x.n -1);
    	if(!ispd || determinant_x < 0)
    		return -10;

    	matrix product_r_x = matmultiply(det_r, det_x);
    	float trace_result = trace(product_r_x);

    	double result = (k/2.0) * log(determinant_r) + ((k-n-1)/2.0) * log(determinant_x)  - 0.5*trace_result;


//    	std::cout << " ***************Result************** " << exp(getReturnValue(result)) << endl;
    	return getReturnValue(result);
    }
}

#endif








