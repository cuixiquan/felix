/*
* FactorFactory_BUGS.h
*
*  Created on: Mar 18, 2013
*      Author: shriram
*/

#ifndef FACTORFACTORY_BUGS_H_
#define FACTORFACTORY_BUGS_H_
#include "../fg/Factor.h"
#include "../fg/Variable.h"
#include <cmath>
#include <random>
#define MIN_PROB -22 // approximated from e^-700 to fit in double
#define MAX_PROB 22
namespace hazy{

enum FactorOptType {FACTOR_GENERAL=0, FACTOR_DIRICHLETWITHCONSTANTALPHA=1, FACTOR_CATEGORICALWITHEVIDENCERV=2};
const int factorfactory_BUGS_modelsize = 10000;
double FactorFactory_BUGS_MODELS[factorfactory_BUGS_modelsize];

class FactorFactory_BUGS {

public:
	static const int sizeofvariable = sizeof(OID) + sizeof(double) + sizeof(int);
	static const int group_header = sizeof(int) + sizeof(double);

	static void getFunctionGroupParameters(int & _ngroups, int & statesize, Long _totalnvars, FactorOptType _factor_type_id)
	{
		// storing the allowed number of variables, current number of variable + aux in every group
		statesize = statesize + _ngroups * (sizeof (int) + group_header);
		//number of variables * (size of each variable)
		statesize = statesize + _totalnvars * sizeofvariable;

		/*
		 * Optimization: Storing the potential of previous state (double) to calulate the potential for the current state
		 * For FACTOR_DIRICHLETWITHCONSTANTALPHA, I store the current potential and the sum_of_previous_p.
		 * The sum of previous p is required to check if this sample is correct as p should sum to approx. 1
		 * It is stored in the tail of the state.
		 */
		if(_factor_type_id == FACTOR_DIRICHLETWITHCONSTANTALPHA)
		{
			statesize = statesize + sizeof(double) + sizeof(double);
		}
	}

	/*
	 * The generate function creates a factor to be added into the factor graph.
	 * _nvars stores the total number of variables in all the groups
	 */
	static Factor generate(OID _oid, int _ngroups, Long _nvars, void(*const VG)(Factor & factor, VariableGroup & vg),
			double(*const POTENTIAL)(Factor & factor, const Variable & v, const double & cvalue), int _pid, FactorOptType _factor_type_id)
	{
		// Storing the number of groups. Will be used for retrieval
		int statesize = sizeof (int);
		void (*_INIT_STATE_FUNCTION)(Factor & factor) = _INIT_STATE;
		void (*_UPDATE_VAR_FUNCTION)(Factor & factor, const Variable & v, const double & cvalue);

		if(_factor_type_id == FACTOR_DIRICHLETWITHCONSTANTALPHA)
			_UPDATE_VAR_FUNCTION = _UPDATE_VAR_DIRICHLETWITHCONSTANTALPHA;
		else
			_UPDATE_VAR_FUNCTION = _UPDATE_VAR;

		getFunctionGroupParameters(_ngroups, statesize, _nvars, _factor_type_id);

		return Factor(_oid,
					  statesize,
					  new Factor::FunctionGroup(_INIT_STATE_FUNCTION, VG, POTENTIAL, _INIT_GROUP, _INIT_VAR, NULL, _UPDATE_VAR_FUNCTION, NULL, NULL),
					  _nvars,
					  _pid,
					  _factor_type_id,
					  _ngroups);
	}

	static void _INIT_STATE(Factor & factor){
		_set_number_of_current_groups(factor, 0);

		for(int i=0; i< factor.ngroup; i++){
			reinterpret_cast<int&>(factor.state[(i+1)*sizeof(int)]) = 0;
		}

		if(factor.faux == FACTOR_DIRICHLETWITHCONSTANTALPHA)
		{
			reinterpret_cast<double&>(factor.state[factor.statesize - sizeof(double)]) = -1;
			reinterpret_cast<double&>(factor.state[factor.statesize - 2 * sizeof(double)]) = 0;
		}
	}

	static void _clear_group_location(Factor & factor, int & group_id)
	{
		int no_of_variables_in_group = _get_number_of_total_variables_in_group(factor, group_id);
		int group_start_loc = _get_group_start_location(factor, group_id);

		reinterpret_cast<int& >(factor.state[group_start_loc]) = 0;
		reinterpret_cast<double& >(factor.state[group_start_loc + sizeof (int)]) = 0;

		for(int i = group_start_loc + group_header;i < no_of_variables_in_group * (sizeofvariable);i += sizeofvariable){
			reinterpret_cast<OID& >(factor.state[i]) = 0;
			reinterpret_cast<double& >(factor.state[i + sizeof (OID)]) = 0;
			reinterpret_cast<double& >(factor.state[i + sizeof (OID) + sizeof (double)]) = 0;
		}
	}

	static void _INIT_GROUP(Factor & factor, int group_id, int noofvar, double _aux)
	{
		int current_group = _get_number_of_current_groups(factor);

		_set_number_of_total_variables_in_group(factor, group_id, noofvar);

		_clear_group_location(factor, group_id);

		_set_aux_for_group(factor, group_id, _aux);

		_set_number_of_current_groups(factor, current_group+1);
	}

	static void _INIT_VAR(Factor & factor, int group_id, const OID vid, const double value, const int aux){

		//std::cout << "INIT V" << vid << "  <---  " << value << std::endl;

		int current_nvar =_get_number_of_current_var_in_group(factor, group_id);

		_set_oid_for_variable_in_group(factor, group_id, current_nvar, vid);
		_set_value_for_variable_in_group(factor, group_id, current_nvar, value);
		_set_aux_for_variable_in_group(factor, group_id, current_nvar, aux);

		_set_number_of_current_var_in_group(factor, group_id, current_nvar + 1);

		//std::cout << "REAL = " <<  _get_value_for_variable_in_group(factor, group_id, current_nvar) << std::endl;

	}

	static void _UPDATE_VAR(Factor & factor, const Variable & v, const double & cvalue){
	   int current_ngroups =_get_number_of_current_groups(factor);
	   for(int i=0;i<current_ngroups;i++){
		   int current_group_nvar = _get_number_of_current_var_in_group(factor, i);
		   for(int j=0;j<current_group_nvar;j++){
			   if(v.oid == _get_oid_of_variable_in_group(factor, i, j)) {
				   _set_value_for_variable_in_group(factor, i, j, cvalue);
			   }
		   }
	   }
	}

	static void _UPDATE_VAR_DIRICHLETWITHCONSTANTALPHA(Factor & factor, const Variable & v, const double & cvalue){
	   assert(factor.faux == FACTOR_DIRICHLETWITHCONSTANTALPHA);

	   double sum_p = 0.0;
	   int current_group_nvar = _get_number_of_current_var_in_group(factor, 0);

	   double previouspotential = _get_previous_potential(factor, FACTOR_DIRICHLETWITHCONSTANTALPHA);
	   double currentpotential = 0.0;

	   for(int j=0;j<current_group_nvar;j++) {
		   int aux = _get_aux_for_variable_in_group(factor, 0, j);
		   double alpha = FactorFactory_BUGS_MODELS[aux];
		   double oldvalue = _get_value_for_variable_in_group(factor, 0, j);

		   if(v.oid == _get_oid_of_variable_in_group(factor, 0, j)) {
			   if(previouspotential == -1)
				   currentpotential = currentpotential + (alpha - 1.0) * log(cvalue);
			   else
				   currentpotential = previouspotential + (alpha - 1.0) * (log(cvalue) - log(oldvalue));

			   _set_value_for_variable_in_group(factor, 0, j, cvalue);
			   sum_p = sum_p + cvalue;
		   }
		   else {
			   double varvalue = _get_value_for_variable_in_group(factor, 0, j);

			   if(previouspotential == -1)
				  currentpotential = currentpotential + (alpha - 1.0) * log(varvalue);

			   sum_p = sum_p + varvalue;
		   }
	   }

//		   if(sum_p >= 0.96 && sum_p <= 1.0000001) -> Not checking this condition because if it got accepted, then sum_p should be proper
	   _set_current_potential(factor, getReturnValue(currentpotential), FACTOR_DIRICHLETWITHCONSTANTALPHA);
	   _set_current_sum_of_prob(factor, sum_p);
	}

	inline static double getReturnValue(double result) {
		if (::isnan(result)) // happens when x is Nan
			return MIN_PROB; // since e ^ -inf  = 0 and using hazy::LOG
		else if (result <= -DBL_MAX)
			return MIN_PROB;
		else if (result >= DBL_MAX)
			return MAX_PROB;
		return result;
	}

	/*
	 * Logical Node Expression:
	 * 1. Aux of group indicates expressionid.
	 * 2. _get_vloc_given_aux is not used as the logical node expression itself handles input location params.
	 * 	This is special case because the node params of the logical node is handled as one group.
	 */
	inline static double getEvaluatedResultForLogicalNodeExpression(Factor & factor, int &group_id, const OID varoid, const double cvalue
			, int expressionid)
	{
		double mu = 0;
		int variablecount = 0;
		int totalNoOfVar = _get_number_of_current_var_in_group(factor, group_id);
		double variables[totalNoOfVar], nestedindexvariablesvalue = 0;
		bool hasNestedIndex = false;

		for(int i = 0; i < totalNoOfVar; i++)
		{
			if(_get_aux_for_variable_in_group(factor, group_id, i) == 0) // Nested Index
			{
				hasNestedIndex = true;
				int noofindices = _get_value_for_variable_in_group(factor, group_id, i);
				assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing
				int indexvalues[noofindices], sizeofindices[noofindices], indextogetvariablefor = 0;
				int dim = 0;

				//get index values.
				for(int i = 1; i < noofindices + 1; i++){
					indexvalues[dim++] = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));
				}

				//Get dimensions of the indices
				dim = 0;
				for(int i = noofindices + 1; i < 2 * noofindices + 1; i++){
					sizeofindices[dim++] = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));
				}

				for(int i = 0; i < noofindices - 1; i++){
					int sizetomultiply = 1;

					for(int j = i+1; j < noofindices; j++)
						sizetomultiply = sizetomultiply * sizeofindices[j];

					if(indexvalues[i] != 0) // This is a hotfix for sampling problem
						indextogetvariablefor += (indexvalues[i] - 1) * sizetomultiply;
				}

				assert(2 * noofindices + 1 + indextogetvariablefor < totalNoOfVar);
				if(_get_oid_of_variable_in_group(factor, group_id,
						_get_vloc_given_aux(factor, group_id, 2 * noofindices + 1 + indextogetvariablefor)) == varoid)
					nestedindexvariablesvalue = cvalue;
				else
					nestedindexvariablesvalue = _get_value_for_variable_in_group(factor, group_id,
						_get_vloc_given_aux(factor, group_id, 2 * noofindices + 1 + indextogetvariablefor));
			}
			else if(_get_aux_for_variable_in_group(factor, group_id, i) < 0) { // Non Nested Index Variables
				if(_get_oid_of_variable_in_group(factor, group_id, i) == varoid)
					variables[variablecount++] = cvalue;
				else
					variables[variablecount++] = _get_value_for_variable_in_group(factor, group_id, i);
			}
		}

		if(hasNestedIndex)
			variables[variablecount] = nestedindexvariablesvalue;

		double evalresult = inmemorylogicalnodes[expressionid]->logicalnodeexpression->eval(variables, inmemorylogicalnodes[expressionid]->metadata);
		return getReturnValue(evalresult);
	}

	//group_id's are by reference. May be incremented here
	inline static void getRequiredParametersForNestedIndexArray(Factor & factor, int &group_id, const OID varoid, const double cvalue,
			double allvariables[], int sizeofindices[], int &indextogetvariablefor) {

		int inputgroupid = group_id, totalnumberofvar = 1;

		int noofindices = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, 0));
		assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing
		/*
		 * No Of Indices,
		 * Size of Index1, Size of Index2, ...
		 * Data...
		 *
		 * Subsequent Groups:
		 * Index1, Index2...,
		 * Indexing Algo: var1= (Index1 - 1) * [size of index 2 * size of Index 3....] +
		 * (Index2 - 1) * [size of Index 3 * size of index 4 ....]
		 */

		int indexvalues[noofindices];
		int dim = 0;

		//Get dimensions of the indices
		for(int i = 1; i < noofindices + 1; i++){
			sizeofindices[dim] = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));
			totalnumberofvar = totalnumberofvar * sizeofindices[dim];
			dim++;
		}

		//get index values. The Indices are in subsequent groups. They may also be Logical Node Expressions.
		dim = 0;
		for(int i = 0;i < noofindices;i++){
			group_id++;
			if(_get_aux_for_group(factor, group_id) >= 0) // >=0 indicates the aux is a Logical Expression
				indexvalues[dim++]= getEvaluatedResultForLogicalNodeExpression(factor, group_id, varoid, cvalue, _get_aux_for_group(factor, group_id));
			else { // The group is a constant or Nodename
				assert(_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY);
				if(_get_oid_of_variable_in_group(factor, group_id, 0) == varoid){
					indexvalues[dim++] = cvalue;
				}else{
					indexvalues[dim++] = _get_value_for_variable_in_group(factor, group_id, 0);
				}
			}
		}

		for(int i = 0; i < noofindices - 1; i++){
			int sizetomultiply = 1;

			for(int j = i+1; j < noofindices; j++)
				sizetomultiply = sizetomultiply * sizeofindices[j];

			if(indexvalues[i] != 0) // This is a hotfix for sampling problem
				indextogetvariablefor += (indexvalues[i] - 1) * sizetomultiply;
		}

//		assert(_get_number_of_current_var_in_group(factor, inputgroupid) - (noofindices + 1) > 0);
		int sizeofallvar = 0;
		for(int i = 0;i < totalnumberofvar; i++, sizeofallvar++)
		{
			group_id++;
			if(_get_aux_for_group(factor, group_id) >= 0) // >=0 indicates the aux is a Logical Expression
				allvariables[sizeofallvar]= getEvaluatedResultForLogicalNodeExpression(factor, group_id, varoid, cvalue, _get_aux_for_group(factor, group_id));
			else
			{ // The group is a constant or Nodename
				assert(_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY);
				if(_get_oid_of_variable_in_group(factor, group_id, 0) == varoid){
					allvariables[sizeofallvar] = cvalue;
				}else{
					allvariables[sizeofallvar] = _get_value_for_variable_in_group(factor, group_id, 0);
				}
			}
		}
	}

	/*
	 * Protocol: Handling Only one Nested Index. Nested Index with list of variables are not handled.
	 * Nested Index Variable needs to go as the first variable for evaluation.
	 * 1. Sweep the variables in the group for aux = 0 to find if there is a nested index.
	 * 	If present, then handle nested index and add the variables to the array.
	 * 2. Scan all other variables sequentially and if their aux is not >= 0 add them to the list of variables.
	 */
	inline static void vg_logicalnode_assign(Factor & factor, VariableGroup& vgroup)
	{
		int groupid = 0;

		double evaluatedvalue = getReturnValue(getEvaluatedResultForLogicalNodeExpression(factor, groupid, -1, 0, factor.parameterID));
		if(evaluatedvalue <= MIN_PROB)
			return;

		vgroup.nvar = 1;
		vgroup.hypothesis[0].vid = _get_oid_of_variable_in_group(factor, 1, 0); // last group
		vgroup.hypothesis[0].cvalue = evaluatedvalue;
		return;
	}

	inline static void vg_inverse_logicalnode_assign(Factor& factor, VariableGroup& vgroup){
		assert(_get_aux_for_group(factor, 0) == LISTOFVARIABLE);
		assert(_get_aux_for_group(factor, 1) == LISTOFVARIABLE);
		assert(_get_number_of_current_var_in_group(factor, 0) == _get_number_of_current_var_in_group(factor, 1));

		matrix mattofindinverse;
		mattofindinverse.n = sqrt(_get_number_of_current_var_in_group(factor, 0));
		mattofindinverse.col = mattofindinverse.n;
		for (int i = 1; i <= mattofindinverse.n; i++) {
			for (int j = 1; j <= mattofindinverse.n; j++) {
				mattofindinverse.a[i][j] = _get_value_for_variable_in_group(factor, 0, mattofindinverse.n *(i - 1) + (j - 1));
//				std::cout << " ARRAY [" << i << "," << j << "] = " << mattofindinverse.a[i][j] << std::endl;
			}
		}

		vgroup.nvar = _get_number_of_current_var_in_group(factor, 1);
		matrix inverse_mat = inverse(mattofindinverse);
		for (int i = 1; i <= mattofindinverse.n; i++) {
			for (int j = 1; j <= mattofindinverse.n; j++) {
				vgroup.hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].vid = _get_oid_of_variable_in_group(factor, 1, mattofindinverse.n *(i - 1) + (j - 1));
				vgroup.hypothesis[mattofindinverse.n *(i - 1) + (j - 1)].cvalue = inverse_mat.a[i][j];
			}
		}
	}

	inline static double eval_variable_group(Factor & factor, int &group_id, const Variable & v, const double & cvalue){

		/*
		 * Handling only the first variable for LISTOFVARIABLE too.
		 * Just returning the first value in group if it is a list of variable.
		 * LISTOFVARIABLE will be called by categorical and eval list of variables (for every group)
		 */
		if (_get_aux_for_group(factor, group_id) == SINGLETON || _get_aux_for_group(factor, group_id) == LISTOFVARIABLE) {
			if (_get_oid_of_variable_in_group(factor, group_id, 0) == v.oid) {
				return cvalue;
			} else {
				return _get_value_for_variable_in_group(factor, group_id, 0);
			}
		}
		else if(_get_aux_for_group(factor, group_id) >= 0) // >=0 indicates Logical Expression
		{
			return getEvaluatedResultForLogicalNodeExpression(factor, group_id, v.oid, cvalue, _get_aux_for_group(factor, group_id));
		}
		else if (_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY) {
			int noofindices = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, 0));
			assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing
			int sizeofindices[noofindices], indextogetvariablefor = 0, sizeofallvariables = 1;

			for(int i = 1; i < noofindices + 1; i++)
				sizeofallvariables *= _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));

			double allvariables[sizeofallvariables];
			getRequiredParametersForNestedIndexArray(factor, group_id, v.oid, cvalue, allvariables, sizeofindices, indextogetvariablefor);

			assert(indextogetvariablefor < sizeofallvariables);
			return allvariables[indextogetvariablefor];
		}
		assert(false);
		return 0;
	}

	inline static double evaluate_categorical(Factor & factor, const Variable & v, const double & cvalue) {
		//returns p[r-1]
		int group_id = 0;
		int r;

		if(_get_aux_for_group(factor, group_id) == LISTOFVARIABLE || _get_aux_for_group(factor, group_id) > 0) {
			int ncatgroups = _get_number_of_current_groups(factor) - 1;
			if(factor.faux == FACTOR_CATEGORICALWITHEVIDENCERV)
				r = 1;
			else
				r = eval_variable_group(factor, ncatgroups, v, cvalue);

			group_id = r - 1;
			return eval_variable_group(factor, group_id, v, cvalue); // eval variable group handles both Logical Expression and listofvariable
		}
		else if(_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY) {
			int noofindices = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, 0));
			int indextogetvariablefor = 0, sizeofallvariables = 1;
			int sizeofindices[noofindices];

			for(int i = 1; i < noofindices + 1; i++)
				sizeofallvariables *= _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));

			double allvariables[sizeofallvariables];
			getRequiredParametersForNestedIndexArray(factor, group_id, v.oid, cvalue, allvariables,	sizeofindices, indextogetvariablefor);

			group_id++;
			assert(_get_aux_for_group(factor, group_id) == SINGLETON);
			if(factor.faux == FACTOR_CATEGORICALWITHEVIDENCERV)
				r = 1;
			else
				r = eval_variable_group(factor, group_id, v, cvalue);

			assert(indextogetvariablefor + r - 1 < sizeofallvariables);
			return allvariables[indextogetvariablefor + r - 1];
		}
//		else if(_get_aux_for_group(factor, group_id) > 0) // Logical Expression
//		{
//			cout << " Logical Expression " << endl;
//			cout << " Number of groups " << _get_number_of_current_groups(factor) << endl;
//			int curvar = 0;
//			double allvariables[_get_number_of_current_groups(factor) - 1];
//			for(int i = 0; i < _get_number_of_current_groups(factor) - 1; i++, curvar++)
//			{
//				allvariables[curvar] = getEvaluatedResultForLogicalNodeExpression(factor, group_id, v.oid, cvalue,
//						_get_aux_for_group(factor, group_id));
//				group_id++;
//			}
//
//			if(factor.faux == FACTOR_CATEGORICALWITHEVIDENCERV)
//				r = 1;
//			else
//				r = eval_variable_group(factor, group_id, v, cvalue);
//
//			cout << "r is " << r << " cur " << curvar << endl;
//			assert(r - 1 < curvar);
//			return allvariables[r - 1];
//		}
		assert(false);
		return 0;
	}

	inline static void evaluate_list_of_variables(Factor & factor, int &group_id, const Variable & v, const double & cvalue,
			double arraytofill[], int num_groups_to_lookahead = 1) {
		if(_get_aux_for_group(factor, group_id) == LISTOFVARIABLE || _get_aux_for_group(factor, group_id) > 0)
		{
			/*
			 * calling eval group for both list of variable and logicalexpression as it handles it efficiently.
			 * Only consuming as much group id as necessary for the evaluation
			 */
			for(int i = 0 ;i < num_groups_to_lookahead; i++) {
				arraytofill[i] = eval_variable_group(factor, group_id, v, cvalue);
				if(i != num_groups_to_lookahead - 1) group_id++;
			}
		}
		else if(_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY) {

			int noofindices = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, 0));
			assert(noofindices <= 4); // This is because BUGS Supports upto 4 Nested Indexing

			int indextogetvariablefor = 0, dim = 0, sizeofallvariables = 1;
			int sizeofindices[noofindices];

			for(int i = 1; i < noofindices + 1; i++)
				sizeofallvariables *= _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, i));
			double allvariables[sizeofallvariables];

			getRequiredParametersForNestedIndexArray(factor, group_id, v.oid, cvalue, allvariables,	sizeofindices, indextogetvariablefor);

			for(int i=indextogetvariablefor; i < indextogetvariablefor + sizeofindices[noofindices-1];i++) {
				assert(i < sizeofallvariables);
				arraytofill[dim++] = allvariables[i];
			}
		}
		else
			assert(false);
	}

	inline static double potential_null(Factor & factor, const Variable & v, const double & cvalue){
		return 1;
	}

	inline static double bugs_bernoulli(Factor & factor, const Variable & v, const double & cvalue) {
		int group_id = 0;
		double p = eval_variable_group(factor, group_id, v, cvalue);group_id++;
		int R = eval_variable_group(factor, group_id, v, cvalue);group_id++;
		if(p <= 0 || R < 0)
			return MIN_PROB;
		else if (R == 1)
			return getReturnValue(log(p));
		else if (R == 0)
			return getReturnValue(log(1 - p));

		return 0;
	}

	inline static double bugs_binomial(Factor & factor, const Variable & v, const double & cvalue) {
		//get p, N, R
		int group_id = 0;
		double p = eval_variable_group(factor, group_id, v, cvalue);group_id++;
		int N = eval_variable_group(factor, group_id, v, cvalue);group_id++;
		int R = eval_variable_group(factor, group_id, v, cvalue);group_id++;

//		cout << " p = " << p << " R = " << R << endl;
		if(R == 0 && p == 0)
			return 0;
		else if (p <= 0 || R <= 0 || p >= 1 || N <= 0)
			return MIN_PROB;
		else if (R > N)
			return MIN_PROB;

		double logcomb = LogCombinations(N, R);
		double result = logcomb + R * log(p) + (N - R) * log(1-p);
//		cout << " result " << result << " ret val " << getReturnValue(result) << endl;
		return getReturnValue(result);
	}

	inline static double bugs_negativebinomial(Factor & factor, const Variable & v, const double & cvalue) {
		int groupid = 0;
		double p = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		int R = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		int X = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double logcomb = LogCombinations(X + R - 1, X);

		double result = logcomb + R * log(p) + X * log(1 - p);
		return getReturnValue(result);
	}


   inline static double bugs_poisson(Factor & factor, const Variable & v, const double & cvalue) {
		int groupid = 0;
		double lambda = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		int R = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = -lambda + R * log(lambda) - LogFactorial(R);
//		cout << " R " << R << " lam " << lambda << " res " << resuendl;
		return getReturnValue(result);
   }

   /*
	* Implemented Fisher's Non Central Hypergeometric distribution.
	* http://en.wikipedia.org/wiki/Fisher's_noncentral_hypergeometric_distribution
	*/

   inline static double bugs_noncentralhypergeometric(Factor & factor, const Variable & v, const double & cvalue) {
		int groupid = 0;
		//get m1
		int m1 = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		//get m2
		int m2 = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		//get n
		int n = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		//get psi
		double w = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		//get x in range xmin, xmax. xmin = max(0, n- m2); xmax = min(n,m1)
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

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


   inline static double bugs_beta(Factor & factor, const Variable & v, const double & cvalue) {
	   int groupid = 0;
		double a = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double b = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double p = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		//	std::cout << "a = " << a << " b =" << b << " p = " << p << " aans " << Gamma(a+b) << " " << Power(p, a - 1.0) << " " << Power(1.0 - p, b - 1.0) << " " << Gamma(a) << " " << Gamma(b) <<std::endl;

		double result = log(Gamma(a + b)) + (a - 1.0) * log(p) + (b	- 1.0) * log(1.0 - p)  - log(Gamma(a)) - log(Gamma(b));
		return getReturnValue(result);
   }


   inline static double bugs_chisquared(Factor & factor, const Variable & v, const double & cvalue) {
	   int groupid = 0;

		double k = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = ((k / 2.0) - 1.0) * log(x) + (-1.0 * x / 2.0) -  (k / 2.0) * log(2) -log(Gamma(k / 2.0));
		return getReturnValue(result);
   }


   inline static double bugs_doubleexponential(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double tau = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = log(tau / 2) + (-1.0 * fabs(x - mu) * tau);
		return getReturnValue(result);
   }


   inline static double bugs_gamma(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double r = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = r * log(mu) + (r - 1.0) * log(x) + (-1.0 * mu * x) - log(Gamma(r));
		return getReturnValue(result);
   }


   inline static double bugs_exponential(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double lambda = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

	   //	std::cout << " Lambda = " << lambda << " x = " << x << std::endl;
		double result = log(lambda) - lambda * x;
		return getReturnValue(result);
   }

   /*
	* Flat Distribution is approximated to a Normal with Mean 0 and high variance
	*/

   inline static double bugs_flat(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;
		double mu = 0;
		double sigma = 10000000;
		double value = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = -log(sigma) -log(sqrt(2 * MATH_PI)) + (-0.5 * ((value - mu) / sigma) * ((value - mu) / sigma));
	   //	cout << "sigma " << sigma << " value " << value << " mu " << mu << " result " << result << " Return = " << exp(getReturnValue(result)) << endl;
		return getReturnValue(result);
   }


   inline static double bugs_generalizedextremevalue(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double sigma = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double eta = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = -log(sigma) + (-1.0 - 1.0 / eta) * log(1.0 + (eta * ((x - mu) / sigma)))
				 + (-1 * Power((1.0 + eta * ((x	- mu) / sigma)), (-1.0 / eta)));

		return getReturnValue(result);
   }


   inline static double bugs_generalizedgamma(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double r = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double beta = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = log(beta) - log(Gamma(r)) + (beta * r) * log(mu) + (beta * r - 1.0) * log(x) + (-1 * Power(mu * x, beta));
	   //	std::cout << "r " << r << " mu " << mu << " beta  " << beta << " x " << x << " ans " << result << std::endl;
		return getReturnValue(result);
   }

   inline static double bugs_generalizedpareto(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double sigma = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double eta = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = -log(sigma) + (-1.0 - 1.0 / eta) * log(1.0 + eta * (x - mu) / sigma) + Power(-(1.0 + (eta/sigma)*(x - mu)), (-1.0 / eta));
		return getReturnValue(result);
   }

   inline static double bugs_logistic(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double tau = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double value = Power(MATH_E, tau * (x - mu));
		double result = log(tau) + (tau * (x - mu)) - 2 * log((1.0 + value));
		return getReturnValue(result);
   }


   inline static double bugs_genericloglikelihood(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;
		double lambda = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double result = lambda;
		return getReturnValue(result);
   }


   inline static double bugs_lognormal(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double tau = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		 // Lognormal x should not be less than zero
		assert(x >= 0);
		double result = -log(x) + log(sqrt(tau / 2 * MATH_PI)) + (-1.0 * ((log(x) - mu) * (log(x) - mu)) * tau / 2);

	   //	std::cout << " x = " << x << " Mu = " << mu << " tau= " << tau << " Result = " << result << " Return value = " << exp(getReturnValue(result)) << std::endl;
		return getReturnValue(result);
   }


   inline static double bugs_normal(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double tau = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double value = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double sigma = 1.0 / sqrt(tau);

		double result = -log(sigma) + (-0.5 * ((value - mu) / sigma) * ((value - mu) / sigma));
//       	cout << " value " << value << " return val " << exp(getReturnValue(result)) << endl;
		return getReturnValue(result);
   }

   inline static double bugs_pareto(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double alpha = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double c = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		if(x < c)
			return -10;
		double result = log(alpha) + alpha * log(c) - (alpha + 1) * log(x);
		return getReturnValue(result);
   }


   inline static double bugs_studentt(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double mu = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double tau = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double k = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double p = (k + 1.0) / 2.0;
		double q = (x - mu) * (x - mu);

		//	std::cout << " Mu= " << mu << " tau= " << tau << " k= " << k << " x= " << x << " ans= " <<
		//			(Gamma(p)/Gamma(k/2.0)) * sqrt(tau/(k*MATH_PI)) * Power((1.0 + (tau/k)*q), -1*p) << std::endl;
		double result = log(Gamma(p)) - log(Gamma(k / 2.0)) + log(sqrt(tau / (k * MATH_PI))) + (-1 * p) * log(1.0 + (tau / k) * q);
		return getReturnValue(result);
   }


   inline static double bugs_uniform(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double a = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double b = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = -log(b - a);
		return getReturnValue(result);
   }


   inline static double bugs_weibull(Factor & factor, const Variable & v, const double & cvalue) {
	    int groupid = 0;

		double v1 = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double lambda = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		double x = eval_variable_group(factor, groupid, v, cvalue);groupid++;

		double result = log(v1) + log(lambda) + (v1 - 1.0) * log(x) + -1 * lambda * Power(x, v1);
		//	std::cout << " v1 = " << v1 << " lamba " << lambda << " x = " << x << " result " << result << std::endl;
		return getReturnValue(result);
   }

   /*
	* Bounds [1, size of p]. Sum of all p = 1
	*/
   inline static double bugs_categorical(Factor & factor, const Variable & v, const double & cvalue) {
		double result = evaluate_categorical(factor, v, cvalue);
		return getReturnValue(log(result));
   }

   /*
	* http://en.wikipedia.org/wiki/Multinomial_distribution
	* Last Evidence variable is the number of Random Variables
	*/

   inline static int get_number_of_variables(Factor & factor, int group_id){
	   if(_get_aux_for_group(factor, group_id) == NESTED_INDEX_ARRAY) {
			int noofindices = _get_value_for_variable_in_group(factor, group_id, _get_vloc_given_aux(factor, group_id, 0));
			return _get_number_of_current_var_in_group(factor, group_id) - (noofindices + 1);
	   }
	   else
		   return _get_number_of_current_var_in_group(factor, group_id);
   }

   /*
    * TODO fix number of groups where one group is a nested index array
    */
   inline static double bugs_multinomial(Factor & factor, const Variable & v, const double & cvalue) {
	   /*
	    * Using the symmetric property of the distribution to indicate how many groups to look ahead
	    * while calling evaluate list of variables.
	    */
	    int ngroups = _get_number_of_current_groups(factor);

		int groupid = 0;
		int count_x = 0;
		int num_x = (ngroups - 1) / 2;
		double p[num_x];
		double x[num_x];
		evaluate_list_of_variables(factor, groupid, v, cvalue, p, num_x);groupid++;
		int N = eval_variable_group(factor, groupid, v, cvalue);groupid++;
		evaluate_list_of_variables(factor, groupid, v, cvalue, x, num_x);groupid++;

		for(int i = 0 ; i < num_x; i++)
			count_x += x[i];

		if(count_x != N)
			return -10; // Because e^-10 ~ 0. Or else will cause underflow


		double logfactorial_xs = 0;
		for(int i = 0 ; i < num_x; i++) {
			logfactorial_xs = logfactorial_xs + LogFactorial(x[i]);
		}

		double logproduct_p = 0;
		for(int i = 0 ; i < num_x; i++) {
			logproduct_p = logproduct_p + x[i] * log(p[i]);
		}

		//	std::cout << " Num x = " << num_x << " Num_p = " << num_p << std::endl;

		double result = LogFactorial(N) + logproduct_p - logfactorial_xs;
		return getReturnValue(result);
   }

   /*
	* Optimized dirichlet distribution to speed up BUGS
	*/
   inline static double bugs_dirichlet_with_constant_alpha(Factor & factor, const Variable & v, const double & cvalue) {
	   double previouspotential = _get_previous_potential(factor, FACTOR_DIRICHLETWITHCONSTANTALPHA);
	   if(previouspotential == -1)
	   {
		   assert(_get_aux_for_group(factor, 0) == LISTOFVARIABLE);
		   double currentpotential = 0;
		   double sum_p = 0.0;

			for(int i = 0 ;i < _get_number_of_current_var_in_group(factor, 0); i++) {
				int aux = _get_aux_for_variable_in_group(factor, 0, i);
				double alpha = FactorFactory_BUGS_MODELS[aux];

				if (_get_oid_of_variable_in_group(factor, 0, i) == v.oid) {
					currentpotential = currentpotential + (alpha - 1.0) * log(cvalue);
					sum_p += cvalue;
				} else {
					double varvalue = _get_value_for_variable_in_group(factor, 0, i);

					currentpotential = currentpotential + (alpha - 1.0) * log(varvalue);
					sum_p += varvalue;
				}
			}

			// If the sampled value's sum != 1, then it is not a dirichlet distribution
			if(!(sum_p >= 0.96 && sum_p <= 1.0000001))
				return -100; // e^-100 => 0

		   return currentpotential;
	   }
	   else {
		   int current_group_nvar = _get_number_of_current_var_in_group(factor, 0);
		   for(int j = 0;j < current_group_nvar; j++) {
			   if(v.oid == _get_oid_of_variable_in_group(factor, 0, j)) {

				   int aux = _get_aux_for_variable_in_group(factor, 0, j);

				   //aux indexes into alpha in the models
				   double alpha = FactorFactory_BUGS_MODELS[aux];
				   double oldvalue = _get_value_for_variable_in_group(factor, 0, j);

				   double sum_p = _get_previous_sum_of_prob(factor) + cvalue - oldvalue;

				   if(!(sum_p >= 0.96 && sum_p <= 1.0000001))
						return -100; // e^-100 => 0

				   // new potential = previouspotential * Power(newvalue/oldvalue, alpha - 1)
				   // The previous potential was set in the log scale only
//					   cout << " Previous potential " << previouspotential <<endl;
				   double currentpotential = previouspotential +  (alpha - 1.0) * (log(cvalue) - log(oldvalue));

				   currentpotential = getReturnValue(currentpotential);

				   return currentpotential;
			   }
		   }
	   }

	   assert(false);
	   return 0;
   }

   /*
	* http://en.wikipedia.org/wiki/Dirichlet_distribution
	* Last Evidence variable is the number of Random Variables
	* Last - 1 = > Parameter 2
	* Last - 2 = > Parameter 1
	* * TODO fix number of groups where one group is a nested index array
	*/
   inline static double bugs_dirichlet(Factor & factor, const Variable & v, const double & cvalue) {
		int groupid = 0;
		double alpha_sum = 0.0;

		int ngroups = _get_number_of_current_groups(factor);
		int num_alpha = ngroups/2;
		double alpha[num_alpha];
		evaluate_list_of_variables(factor, groupid, v, cvalue, alpha, num_alpha);groupid++;

		for(int i = 0 ; i < num_alpha; i++)
			alpha_sum += alpha[i];

		double sum_p = 0.0;
		double p[num_alpha];
		evaluate_list_of_variables(factor, groupid, v, cvalue, p, num_alpha);groupid++;
		for(int i = 0 ;i < num_alpha; i++)
			sum_p += p[i];
	   //		std::cout << " p[" << i << "] = " << p[i] <<endl;

		if(!(sum_p >= 0.96 && sum_p <= 1.0000001))
		{
//				cout << " Returning -10 in sum_p " << sum_p << endl;
			return -10; // Dirichlet is used as a prior dist
		}

		double loggamma_alpha = 0.0;
		for(int i = 0 ; i < num_alpha; i++) {
			loggamma_alpha = loggamma_alpha + log(Gamma(alpha[i]));
		}

		double logbetafunction = loggamma_alpha - log(Gamma(alpha_sum));

		double logproduct_p = 0;
		for(int i = 0 ; i < num_alpha; i++) {
			logproduct_p = logproduct_p  + (alpha[i] - 1) * log(p[i]);
		}

		double result = logproduct_p - logbetafunction;

//		   	std::cout << " ***************Result************** " << result << endl;
		return getReturnValue(result);
   }


   /*
    * TODO fix number of groups where one group is a nested index array
    */
   inline static double bugs_mvnormal(Factor & factor, const Variable & v, const double & cvalue) {
	int groupid = 0;

	int ngroups = _get_number_of_current_groups(factor);
	// using formula x^2 + 2 * x = k where k is ngroups
//	int ngroupsmapping[21] = {3, 8, 15, 24, 33, 48, 63, 80, 99, 120, 143, 165, 195, 224, 255, 288, 323, 360, 399, 440, 483};
	int ngroupsmapping[10] = {3, 8, 15, 24, 33, 48, 63, 80, 99, 120};

	int num_mu = -1;
	for(int i = 0; i < 10; i++)
		if(ngroups == ngroupsmapping[i])
		{
			num_mu = i + 1;
			break;
		}

	if(num_mu == -1)
	{
		cout << "[BUGSError] This scenario is not yet handled. This might be because\n"
				"1. You might be using nested indexes in Multivariate Nodes. \n"
				"2. You might be using matrices whose size is very large.\n"
				"If not, Please send a note "
				"to the developer with your model and data. \nIf you really need this feature, then too please send a note." << endl;
		assert(false);
	}

	double mu[num_mu];
	double t[num_mu][num_mu];
	evaluate_list_of_variables(factor, groupid, v, cvalue, mu, num_mu);

	for(int i = 0 ;i < num_mu; i++) {
		for(int j = 0 ; j < num_mu; j++) {
			groupid++;
			t[i][j] = eval_variable_group(factor, groupid, v, cvalue);
		}
	}

	groupid++;
	double x[num_mu];
	evaluate_list_of_variables(factor, groupid, v, cvalue, x, num_mu);groupid++;

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
	}
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


   inline static double bugs_mvstudentt(Factor & factor, const Variable & v, const double & cvalue) {
	int groupid = 0;

	int ngroups = _get_number_of_current_groups(factor);

	// using formula x^2 + 2 * x = k where k is ngroups
	int ngroupsmapping[10] = {4, 9, 16, 25, 34, 49, 64, 81, 100, 121};

	int num_mu = -1;
	for(int i = 0; i < 10; i++)
		if(ngroups == ngroupsmapping[i])
		{
			num_mu = i + 1;
			break;
		}

	if(num_mu == -1)
	{
		cout << "[BUGSError] This scenario is not yet handled. This might be because\n"
						"1. You might be using nested indexes in Multivariate Nodes. \n"
						"2. You might be using matrices whose size is very large.\n"
						"If not, Please send a note "
						"to the developer with your model and data. If you really need this feature, then too please send a note." << endl;
		assert(false);
	}

	double mu[num_mu];
	evaluate_list_of_variables(factor, groupid, v, cvalue, mu, num_mu);

	double t[num_mu][num_mu];
	for(int i = 0 ;i < num_mu; i++) {
			for(int j = 0 ; j < num_mu; j++) {
				groupid++;
				t[i][j] = eval_variable_group(factor, groupid, v, cvalue);
			}
		}


	++groupid;
	int k = eval_variable_group(factor, groupid, v, cvalue);groupid++;

	double x[num_mu];
	evaluate_list_of_variables(factor, groupid, v, cvalue, x, num_mu);groupid++;

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


   inline static double bugs_wishart(Factor & factor, const Variable & v, const double & cvalue) {

	int groupid = 0;

	int num_r = get_number_of_variables(factor, groupid);
	int n = sqrt(num_r);
	double r[n][n];
	double x[n][n];
   //	std::cout << " Num_x = " << num_x << " n= " << n << std::endl;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if(v.oid == _get_oid_of_variable_in_group(factor, groupid, n * i + j))
				r[i][j] = cvalue;
			else
				r[i][j] = _get_value_for_variable_in_group(factor, groupid, n * i + j);
//    			std::cout << " r[" << i << "," << j << "] = " << r[i][j] << std::endl;
		}
	}

	++groupid;
	int k = eval_variable_group(factor, groupid, v, cvalue);groupid++;

//    	std::cout << " k = " << k << " " << state.vgroups[1].hypothesis[0].cvalue << " " << state.vgroups[1].nvar << std::endl;

	int num_x = get_number_of_variables(factor, groupid);

	if(num_r != num_x)
	{
		std::cout << "size of r should be size of x " << std::endl;
		assert(false);
	}

	for(int i = 0 ;i < n; i++)
	{
		for(int j = 0 ; j < n; j++)
		{
			if(v.oid == _get_oid_of_variable_in_group(factor, groupid, n * i + j))
				x[i][j] = cvalue;
			else
				x[i][j] = _get_value_for_variable_in_group(factor, groupid, n * i + j);
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


private:
	inline static double _get_previous_potential(const Factor &factor, FactorOptType type) {
		if(type == FACTOR_DIRICHLETWITHCONSTANTALPHA)
			return reinterpret_cast<const double&>(factor.state[factor.statesize - sizeof(double)]);
		assert(false);
	}

	inline static double _get_previous_sum_of_prob(const Factor &factor){
		return reinterpret_cast<const double&>(factor.state[factor.statesize - 2 * sizeof(double)]);
	}

	inline static double _set_current_sum_of_prob(Factor &factor, double sum_p){
		reinterpret_cast<double&>(factor.state[factor.statesize - 2 * sizeof(double)]) = sum_p;
	}

	inline static void _set_current_potential(Factor &factor, double potential, FactorOptType type) {
		if(type == FACTOR_DIRICHLETWITHCONSTANTALPHA)
			reinterpret_cast<double&>(factor.state[factor.statesize - sizeof(double)]) = potential;
		else
			assert(false);
	}

	inline static int _get_number_of_current_groups(const Factor & factor){
		return reinterpret_cast<const int&>(factor.state[0]);
	}

	inline static void _set_number_of_current_groups(Factor & factor, int current_ngroups){
		reinterpret_cast<int&>(factor.state[0]) = current_ngroups;
	}

	inline static int _get_number_of_total_variables_in_group(const Factor &factor, int group_id){
		return reinterpret_cast<const int&>(factor.state[sizeof(int) + group_id * sizeof(int)]);
	}

	inline static void _set_number_of_total_variables_in_group(Factor &factor, int group_id, int noofvar) {
		reinterpret_cast<int&>(factor.state[sizeof(int) + group_id * sizeof(int)]) = noofvar;
	}

	/*
	 * The first line calculates the static portion of the state.
	 */
	inline static int _get_group_start_location(const Factor &factor, int group_id){
		int start = sizeof(int) + factor.ngroup * sizeof(int);
		for(int i=0; i < group_id; i++){
			start = start + sizeof(int) + sizeof(double) + _get_number_of_total_variables_in_group(factor, i) * (sizeof(OID) + sizeof(double) + sizeof(int));
		}
		return start;
	}

	inline static int _get_number_of_current_var_in_group(const Factor & factor, int group_id)
	{
		int nvarlocation = _get_group_start_location(factor, group_id);
		return reinterpret_cast<const int&>(factor.state[nvarlocation]);
	}

	inline static void _set_number_of_current_var_in_group(Factor & factor, int group_id, int current_nvar){
		int nvarlocation = _get_group_start_location(factor, group_id);
		reinterpret_cast<int&>(factor.state[nvarlocation]) = current_nvar;
	}

	inline static void _set_aux_for_group(Factor & factor, int group_id, double aux) {
		int auxstart = _get_group_start_location(factor, group_id) + sizeof(int);
		reinterpret_cast<double&>(factor.state[auxstart]) = aux;
	}

	inline static double _get_aux_for_group(const Factor & factor, int group_id) {
		int auxstart = _get_group_start_location(factor, group_id) + sizeof(int);
		return reinterpret_cast<const double&>(factor.state[auxstart]);
	}

	inline static void _set_oid_for_variable_in_group(Factor & factor, int group_id, int nvar, OID oid){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double); // nvar + aux
		reinterpret_cast<OID&>(factor.state[variablelocstart + nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]) = oid;
	}

	inline static OID _get_oid_of_variable_in_group(const Factor & factor, int group_id, int nvar){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		return reinterpret_cast<const OID&>(factor.state[variablelocstart + nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]);
	}

	inline static void _set_value_for_variable_in_group(Factor & factor, int group_id, int nvar, double value){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		reinterpret_cast<double&>(factor.state[variablelocstart + sizeof(OID)+nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]) = value;
	}

	inline static double _get_value_for_variable_in_group(const Factor & factor, int group_id, int nvar){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		return reinterpret_cast<const double&>(factor.state[variablelocstart + sizeof(OID)+nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]);
	}

	inline static void _set_aux_for_variable_in_group(Factor & factor, int group_id, int nvar, int aux){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		reinterpret_cast<int&>(factor.state[variablelocstart +sizeof(OID)+sizeof(double)+nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]) = aux;
	}

	inline static int _get_aux_for_variable_in_group(const Factor & factor, int group_id, int nvar){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		return reinterpret_cast<const int&>(factor.state[variablelocstart +sizeof(OID)+sizeof(double)+nvar*(sizeof(OID)+sizeof(double)+sizeof(int))]);
	}

	inline static int _get_vloc_given_aux(const Factor & factor, int group_id, int aux){
		int variablelocstart = _get_group_start_location(factor, group_id) + sizeof(int) + sizeof(double);
		int novaringroup = _get_number_of_current_var_in_group(factor, group_id);
		for(int i = 0; i < novaringroup; i++)
		{
			if(_get_aux_for_variable_in_group(factor, group_id, i) == aux)
				return i;
		}

		return 0;
	}
};
}

#endif /* FACTORFACTORY_BUGS_H_ */
