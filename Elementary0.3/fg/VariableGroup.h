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
//  Filename: VariableGroup.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_VariableGroup_h
#define Elementary0_3_VariableGroup_h

namespace hazy{
    
    class VariableHypothesis{
    public:
        OID vid;
        double cvalue;
        int aux;
    };

    class VariableGroup{
    
    public:
        
        int nvar = 0;
        
        double aux = 0;
        
        VariableHypothesis hypothesis[MAX_VAR_PER_GROUP];
        
        void push(const OID & vid, const double cvalue, const int aux){
            if(nvar >= MAX_VAR_PER_GROUP){
            	std::cout << " Number of variables is " << nvar << " Maximum allowed variables is " << MAX_VAR_PER_GROUP << std::endl;
                assert(false);
            }
            hypothesis[nvar].vid = vid;
            hypothesis[nvar].cvalue = cvalue;
            hypothesis[nvar].aux = aux;
            nvar ++;
        }
    };
    
}


#endif
