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
//  Filename: FactorFactory_LDA.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/16/13.
//

#ifndef Elementary0_3_FactorFactory_LDA_h
#define Elementary0_3_FactorFactory_LDA_h

#include "../fg/Factor.h"
#include "../fg/VariableGroup.h"
#include "../fg/Variable.h"

namespace hazy{
    
    class FactorFactory_LDA{
        
    public:
        
        static Factor::FunctionGroup _factorgroup;
        
        static Factor::FunctionGroup _factorgroup_inverse;
        
        static Factor generate(OID _oid, int _ntopics, int _pid, bool is_inverse){
            
            if(is_inverse){
                return Factor(_oid,
                              _ntopics*sizeof(int),  // for each topic, we maintain an int
                              &FactorFactory_LDA::_factorgroup_inverse,
                              -1, // nvar
                              _pid,
                              -1  // no aux info for LDA
                              );
            }else{
                return Factor(_oid,
                              _ntopics*sizeof(int),  // for each topic, we maintain an int
                              &FactorFactory_LDA::_factorgroup,
                              -1, // nvar
                              _pid, 
                              -1  // no aux info for LDA
                              );
            }
            
        }
        
        static void _INIT_STATE(Factor & factor){
            for(int i=0;i<factor.statesize;i+=sizeof(int)){
                reinterpret_cast<int&>(factor.state[i]) = 0;
            }
        }
        
        static double _POTENTIAL(Factor & factor, const Variable & v, const double & cvalue){
            return (model[factor.parameterID] + reinterpret_cast<const int*>(factor.state)[(int)cvalue]);
        }
        
        static double _POTENTIAL_INVERSE(Factor & factor, const Variable & v, const double & cvalue){
            return 1.0/(model[factor.parameterID] + reinterpret_cast<const int*>(factor.state)[(int)cvalue]);
        }
        
        static void _INIT_GROUP(Factor & factor, int group_id, int _size, double _aux){}
        
        static void _INIT_VAR(Factor & factor, int group_id, const OID vid, const double value, const int aux){
            reinterpret_cast<int*>(factor.state)[(int)value] ++;
        }
        
        static void _UPDATE_VAR(Factor & factor, const Variable & v, const double & cvalue){
            reinterpret_cast<int*>(factor.state)[(int)cvalue] ++;
            reinterpret_cast<int*>(factor.state)[(int)v.cvalue] --;
        }
              
        static int _get_n_current_var(const Factor & factor){
            assert(false);
            return 0;
        }
        
        static OID _get_n_oid(const Factor & factor, int nv){
            assert(false);
            return 0;
        }
        
    };
    
    
    
}

#endif










