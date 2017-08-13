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
//  Filename: Pager_LRU.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//

#ifndef Elementary_0_2_Pager_LRU_h
#define Elementary_0_2_Pager_LRU_h

#include "Pager.h"

namespace hazy{
    
    template<>
    class Pager<LRU>{
    public:

        static std::string name(){return "LRU";}
        
        const Long NBUFFER;
        
        double * LRUs;
        double current_time;
        
        Pager(Long _NBUFFER) : NBUFFER(_NBUFFER){
            
            LRUs = new double[NBUFFER];
            
            for(int i=0;i<NBUFFER;i++){
                LRUs[i] = 0;
            }
            current_time = 0;
        }
        
        ~Pager(){
            delete [] LRUs;
        }
        
        Long get_toevict(){
            
            double min = LRUs[0];
            int nmin = 0;
            for(int i=0;i<NBUFFER;i++){
                if(min > LRUs[i]){
                    min = LRUs[i];
                    nmin = i;
                }
            }
            
            LRUs[nmin] = current_time ++;
            return nmin;
            
        }
        
        void update(Long objid, Long bufid){
            LRUs[bufid] = current_time++;
        }
        
        void organize(){

        }
    };
    
}


#endif
