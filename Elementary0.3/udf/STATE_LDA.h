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
//  Filename: STATE_LDA.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_STATE_LDA_h
#define Elementary0_3_STATE_LDA_h

#include "../fg/VariableGroup.h"
#include "../fg/Variable.h"

#define MAX_N_TOPICS 200

namespace hazy{
    
    class STATE_LDA{
        
    public:
        
        int count[MAX_N_TOPICS]; // for this toy example, we want < 200 topics.
        int nvar;
        
        int parameterID = -1;
        
        void init_group(int group_id, double _aux){
            assert(false);
        }
        
        void init_var(int group_id, const Variable & v, const int aux){
            nvar ++;
            count[(int)v.cvalue] ++;
        }
        
        template<class ASSIGNMENT_KV>
        void mat(const ASSIGNMENT_KV & assignments){
            std::cout << "ERROR: [STATE_LDA] STATE_LDA does not support Lazy-Mat on Factor Side." << std::endl;
            assert(false);
        }
        
        //void update_var(const Variable & v, const double & cvalue){
        void update_var(const Variable & v, const double & cvalue){
            count[(int)v.cvalue] --;
            count[(int)cvalue] ++;
        }
        
        void print(){
            
        }
        
        STATE_LDA(){
            nvar = 0;
            for(int i=0;i<MAX_N_TOPICS;i++){
                count[i] = 0;
            }
        }
        
    };
    
    inline double lda_potential(const STATE_LDA & state, const Variable & v, const double & cvalue){
        return (hazy::model[state.parameterID] + state.count[(int)cvalue]);
    }
    
    inline double lda_potential_inverse(const STATE_LDA & state, const Variable & v, const double & cvalue){
        return 1.0/(hazy::model[state.parameterID] + state.count[(int)cvalue]);
    }
    
    
}


#endif
