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
//  Filename: Pager_OPTIMAL.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//

#ifndef Elementary_0_2_Pager_OPTIMAL_h
#define Elementary_0_2_Pager_OPTIMAL_h

#include "Pager.h"
#include "../util/Common.h"

namespace hazy{
    
    class IDPair{
        
    public:
        Long id;
        Long nextPos;
        
        IDPair(Long _id){
            id = _id;
            nextPos = -1;   // never used
        }
    };
    
    template<>
    class Pager<OPTIMAL>{
    public:
        
        static std::string name(){return "OPTIMAL";}
        
        int ref_counter;
        
        bool organized;
        
        const Long NBUFFER;
        
        double * priorities;
        
        std::vector<IDPair> ref_streams;
        
        Pager(Long _NBUFFER) : NBUFFER(_NBUFFER){
            
            priorities = new double[NBUFFER];
            
            for(int i=0;i<NBUFFER;i++){
                priorities[i] = 0;
            }
            
            organized = false;
            
        }
        
        ~Pager(){
            delete [] priorities;
        }
        
        Long get_toevict(){
            
            if(organized == false || SIGMOD_START_OPTIMAL_CACHE == false){
                return rand() % NBUFFER;
            }else{
                double min = priorities[0];
                double nmin = 0;
                for(int i=0;i<NBUFFER;i++){
                    
                    if(priorities[i] >= 0){   //never seen again
                        nmin = i;
                        break;
                    }
                    
                    if(min > priorities[i]){
                        min = priorities[i];
                        nmin = i;
                    }
                }
                
                return nmin;
            }
        }
        
        void update(Long objid, Long bufid){
            
            if(SIGMOD_START_OPTIMAL_CACHE == true){
                
                if(organized == false){
                    ref_streams.push_back(objid);
                }else{
                    
                    priorities[bufid] = -ref_streams[ref_counter].nextPos;
                    ref_counter++;
                    
                    if(ref_counter > ref_streams.size()){   // ERROR
                        ref_counter %= ref_streams.size();
                    }
                }
            }else{
                
            }
            
        }
        
        void organize(){
            
            ref_counter = 0;
            
            if(organized == false){
                
                AwesomeArray<Long, MM, DIRECT, NONE, FIXLEN> lastseen(-1);
                
                RID dummyrid;
                Long id;
                Long lastSeen;
                for(Long i=0;i<ref_streams.size();i++){
                    
                    id = ref_streams[i].id;
                    
                    if(id > lastseen.get_maxid()){
                        lastseen.load(id, -1, dummyrid);
                    }
                    
                    lastseen.get_oid(id, lastSeen);
                    
                    if(lastSeen == -1){
                        
                    }else{
                        ref_streams[lastSeen].nextPos = i;
                    }
                    lastseen.set_oid(id, i);
                    
                }
                
            }
            
            organized = true;
        }
    };
    
}

#endif
