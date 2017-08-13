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
//  Filename: Variable.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_Variable_h
#define Elementary0_3_Variable_h

#include "../util/Common.h"
namespace hazy{
    
    class Variable{
    public:
        
        OID oid;
        
        RID rid;
        
        FullID vgfid;
        
        VariableType vtype; /**< {QUERY, EVID, EMPTY} */
        
        DomainType dtype; /**< {BOOLEAN, MULTINOMIAL, REAL} */
        
        double cvalue; /**< continous value.*/
        
        double lbound; /**< lower bound of value (include). */
        double ubound; /**< upper bound of value (include). */
        
        double exp_sum;    /**< Sum of sample values to calculate expectation. */
        int exp_count;  /**< Count of sample values to calculate expectation. */
        
        double exp_sum_v2;  /**< Sum of sample square. */
        
        /**
         * Default constructor -- VariableType vtype = EMPTY;
         */
        Variable(){
            vtype = EMPTY;
            exp_sum = 0;
            exp_count = 0;
            exp_sum_v2 = 0;
        }
        
        /**
         * Constructor.
         */
        Variable(OID _oid,
                 VariableType _vtype,
                 DomainType _dtype,
                 double _cvalue,
                 double _lbound,
                 double _ubound){
            
            oid = _oid;
            
            vtype = _vtype;
            dtype = _dtype;
            
            cvalue = _cvalue;
            
            lbound = _lbound;
            ubound = _ubound;
            
            exp_sum = 0;
            exp_count = 0;
            
        }
        
    };
    
    
}



#endif
