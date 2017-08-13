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
//  Filename: Common.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/12/13.
//

#ifndef Elementary_0_2_Common_h
#define Elementary_0_2_Common_h

#define _LGPL_SOURCE

#include <urcu-qsbr.h>

#include <thread>

#include <vector>
#include <iostream>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <fcntl.h>
#include <float.h>

#include <math.h>
#include <map>

#include "Lock.h"
#include "RWLock.h"
#include "Timer.h"

//#include "../third/ezRateProgressBar.hpp"
#include "../ui2/bugs_parser_db/src/AdapterClasses/LogicalNodeComposite.hpp"

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif


#define LOG_2   0.693147180559945
#define MINUS_LOG_THRESHOLD   -18.42

#define MATH_PI 3.1415926
#define MATH_E 2.71828
#define MAX_MATRIX_SIZE 50

bool LOG_LOCK = false;

inline bool fast_exact_is_equal(double a, double b){
    return (a <= b && b <= a);
}

inline double logadd(double log_a, double log_b){
    
    if (log_a < log_b)
    { // swap them
        double tmp = log_a;
        log_a = log_b;
        log_b = tmp;
    } else if (fast_exact_is_equal(log_a, log_b)) {
        // Special case when log_a == log_b. In particular this works when both
        // log_a and log_b are (+-) INFINITY: it will return (+-) INFINITY
        // instead of NaN.
        return LOG_2 + log_a;
    }
    double negative_absolute_difference = log_b - log_a;
    if (negative_absolute_difference < MINUS_LOG_THRESHOLD)
        return (log_a);
    return (log_a + log1p(exp(negative_absolute_difference)));
    
}


int ntmp = 0;
std::string config_tmp_folder = "/tmp/";
std::string getNextTmpFileName(){
    char tmp[1000];
    sprintf(tmp, "%s/%d", config_tmp_folder.c_str(), (++ntmp));
    return std::string(tmp);
}


inline double fRand()
{
    //return (double)rand() / RAND_MAX;
    return drand48();
}

namespace hazy{


  typedef long Long;

  /**
     * Object ID.
     */
    typedef Long OID;
    
    /**
     * Record ID.
     */
    class RID{
    public:

      Long first = -1;
      Long second = -1;
                
      RID(): first(-1), second(-1) {}
    };
    
    class FullID{
    public:
      
      OID oid = -1;
      RID rid;
        
      FullID(){}
        
    };

    class Assignment{
    public:
      OID oid;
      double cvalue;
    };
    
    bool LOGDEBUG = false;
    
    bool _DEBUG_;
    
    bool REUSEMODE = false;
    
    //A logical expression is indicated by sending in a Positive Logical ExpressionId
    enum VGROUPTYPE {LISTOFVARIABLE = -1000, NESTED_INDEX_ARRAY = -1001, SINGLETON = -1002};
        
    enum NumericalSystem {LINEAR, LOG};
    
    enum VariableType {EVID, QUERY, EMPTY};

    enum DomainType {BOOLEAN, MULTINOMIAL, REAL};

    class VariableStatus{
    public:
      OID vid;
      VariableType vtype;
      double last_sample_cvalue;
      double expectation;
      double standard_derivation;

    };

    std::string EXP_MAT = "LAZY";
    std::string EXP_PAGING = "LRU";
    std::string EXP_STORAGE = "FILE";
    
    NumericalSystem EXP_NUMSYS = LOG;
    
    int NEPOCH = 1;
    
    int NTHREAD = 1;
    
    size_t SEQ_PAGESIZE = 128L*1024*1024;
    
    size_t SEQ_BUFFERSIZE = 256L*1024*1024;
    
    size_t PAGESIZE = 10L * 1024L * 1024L;
    
    size_t BUFFERSIZE = 1024L * 1024L * 1024L;
    
    const int MAX_VAR_PER_GROUP = 50;
    
    const int MAX_GROUP_PER_STATE = 5;
    
    const long INIT_ARRAY_SIZE = 64;
    
    const long MAX_PARAMETER_IN_MODEL = 1024;
    
    const double SGD_STEP_SIZE = 0.01;
    
    //const int VAR_LEN_VARIABLE_MAX_NELEMENTS = 10240;
    
        // the model shared by every class
    double model[MAX_PARAMETER_IN_MODEL];
    
    bool model_fixmask[MAX_PARAMETER_IN_MODEL];
        
    enum ObjectType {FIXLEN, VARLEN};
    
    enum StorageType {MM, LOCALFILE, ACCU, HBASE};
    
    enum LayoutType {DIRECT, PAGED, SIMPLEBUFFER};
    
    enum BufferType {RANDOM, LRU, OPTIMAL, NONE, MRU};
    
    enum VMatType {V_LAZY, V_EAGER};
    
    enum FMatType {F_LAZY, F_EAGER};
    
    bool SIGMOD_START_OPTIMAL_CACHE = false;
    
    bool SIGMOD_START_LOG_IO = false;
    
    Long MH_STEP_PER_GIBBS_STEP = 10;
    
//    std::vector<std::string> bugs_paramstomonitor;

}
hazy::Long Combinations(int n, int k)
{
  assert(k>=0);
  assert(n>=0);

  if (k > n){
    return 0;
  }

  hazy::Long r = 1;
  for (unsigned int d = 1; d <= k; ++d)
  {
      r *= n--;
      r /= d;
  }

  return r;
}

double LogCombinations(int n, int k)
{
  assert(k>=0);
  assert(n>=0);
  assert(n>=k);

  double r = 0;
  for (unsigned int d = 1; d <= k; ++d)
  {
      r += log(n--);
      r -= log(d);
  }

  return r;
}

hazy::Long Factorial(int number) {
	if(number <= 1) return 1;
	return number * Factorial(number - 1);
}

double LogFactorial(int number) {
	if(number <= 1) return 0;
	return log(number) + LogFactorial(number - 1);
}

double Power(double n1, double n2) {
	if(n1 < 0 && ceilf(n2)!=n2)
		return -1 * pow(-n1, n2);
	return pow(n1,n2);
}

double Gamma(double n1) {
	if(n1 <= 0)
		return 0;
	return tgamma(n1);
}

double MultivariateGamma(double p, double n) {
	double result = Power(MATH_PI, p * (p-1) /4);
	for(int j =1; j <=p; j++){
		result = result * Gamma(n/2 + (1-j)/2.0);
	}
	return result;
}

// Matrix Operations

struct matrix {
	float a[MAX_MATRIX_SIZE][MAX_MATRIX_SIZE];
	int n;
	int col;
};

matrix reduced(matrix &obj, int i, int j) {
	// This function reduces the matrix passed as input to it
	// The 'reduction' requirement is like this:
	// Eliminate the row i and column j from the given matrix to get the reduced matrix

	// This is done by the following logic:
	// a is given matrix. c_obj is desired reduced matrix
	// i. Using two for loops (iterating with p and q here) as usual, we scan the given matrix.
	// row and col represent the current location pointer of row and column of the required reduced matrix.
	// ii. All elements from given matrix are copied to reduced matrix except for those corresponding to
	// row i and column j
	// iii. The reduced matrix has its dimensions one less than that of given matrix
	matrix c_obj;
	int row = 1, col = 1;
	for (int p = 1; p <= obj.n; p++) // outer loop traverses through rows as usual
	{
		for (int q = 1; q <= obj.n; q++) // inner loop traverses through columns as usual
		{
			if ((p != i) && (q != j)) // Skip the elements corresponding to row i OR column j of the given matrix
			{
				c_obj.a[row][col] = obj.a[p][q];
				col = col + 1;
			}
			if (col >= obj.n) // When column 'col' of reduced matrix reaches (or exceeds n), reset it to 1
			{ // and increment 'row'. This means current row of reduced matrix got filled and
			// we need to begin filling a new row.
				col = 1;
				row = row + 1;

				if (row >= obj.n) //This represents the case when both 'col' and 'row of reduce matrix reach (or
				// exceed) n. This means the reduced matrix has been filled up.Break out of the loops.
					break;
			}
		}

	}
	c_obj.n = obj.n - 1; // Fix the dimension of the reduced matrix one less than the given input matrix

	return c_obj; // Return the reduced matrix to the calling function.
}

float determinant(matrix obj) {
	float det = 0;
	if (obj.n == 1) {
		return obj.a[1][1];
	} else {
		for (int scan = 1; scan <= obj.n; scan++) {
			det = det + obj.a[1][scan] * (int)(scan%2 + scan%2 - 1)
					* determinant(reduced(obj, 1, scan));
		}
		return det;
	}
}
/* det(Ak) where 1 <= k <= n is positive => positive definiteness of a matrix
 * Need to find a better way to implement this. This is slower in case of big matrices
 */
bool ispositivedefinite(matrix obj, int ntocheck){
	int n = obj.n;
	for(int i = 1; i <= ntocheck; i++){
		obj.n=i;
		obj.col=i;
		if(determinant(obj) < 0)
			return false;
	}
	return true;
}

bool isSymmetric(matrix obj) {
	//should be a square matrix
	if(obj.col != obj.n)
		return false;

	for (int i = 1; i <= obj.n; i++) {
		for (int j = 1; j <= obj.n; j++) {
			if(fabs(obj.a[i][j] - obj.a[j][i]) != 0) // since they are two float values
				return false;
		}
	}
	return true;
}

matrix transpose(matrix obj) {
	matrix trans_obj;
	trans_obj.n = obj.n;
	for (int i = 1; i <= obj.n; i++) {
		for (int j = 1; j <= obj.n; j++) {
			trans_obj.a[i][j] = obj.a[j][i];
		}
	}
	return trans_obj;
}

float cofactor(matrix obj, int i, int j) {
	float cofact;
	if (obj.n == 1) {
		return obj.a[1][1];
	} else {
		cofact = int(pow(-1, (i + j))) * determinant(reduced(obj, i, j));
	}
	return cofact;
}

matrix adjoint(matrix obj) {
	matrix obj_adj, obj_cof;
	obj_adj.n = obj.n;
	obj_cof.n = obj.n;

	for (int i = 1; i <= obj.n; i++) {
		for (int j = 1; j <= obj.n; j++) {
			obj_cof.a[i][j] = cofactor(obj, i, j);
		}
	}
	obj_adj = transpose(obj_cof);
	return obj_adj;
}

float trace(matrix obj) {
	float result = 0.0;
	for (int i = 1; i <= obj.n; i++) {
		result += obj.a[i][i];
	}

	return result;
}

matrix inverse(matrix obj) {
	matrix obj_inv;
	float d = determinant(obj); // First find determinant of the given matrix

	matrix obj_null;
	obj_null.n = 0;
	// Display error message if determinant is 0

	if (d == 0) {
//		std::cout << "Inverse can be found only if the determninant of the matrix is non-zero" << std::endl;
		return obj_null;
	}

	// Determinant is non-zero - Proceed finding inverse using the above formula
	else {
		matrix obj_adj1 = adjoint(obj);
		obj_adj1.n = obj.n;
		obj_inv.n = obj.n;
		obj_inv.col = obj.n;
		for (int i = 1; i <= obj.n; i++) {
			for (int j = 1; j <= obj.n; j++) {
				obj_inv.a[i][j] = (obj_adj1.a[i][j]) / d;
			}
		}
		return obj_inv;
	}
}

matrix matmultiply(matrix aMatrix, matrix bMatrix) {

	if(aMatrix.col != bMatrix.n)
	{
		std::cout << " Matrices cannot be multiplied " << aMatrix.col << " " << bMatrix.n << std::endl;
		assert(false);
	}

	matrix product;
	product.n = aMatrix.n;
	product.col = bMatrix.col;

	for(int row = 1; row <= aMatrix.n; row++) {
        for (int col = 1; col <= bMatrix.col; col++) {
        	product.a[row][col] = 0;
        }
    }

    for (int row = 1; row <= aMatrix.n; row++) {
        for (int col = 1; col <= bMatrix.col; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 1; inner <= aMatrix.col; inner++) {
                product.a[row][col] += aMatrix.a[row][inner] * bMatrix.a[inner][col];
            }
//            std::std::cout << product.a[row][col] << "  ";
        }
//        std::std::cout << "\n";
    }

    return product;
}




#endif
