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
//  Filename: VariableRecord.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/4/13.
//

#ifndef Elementary0_3_VariableRecord_h
#define Elementary0_3_VariableRecord_h

#include "Variable.h"
#include "../util/Common.h"

namespace hazy{

    class VariableFactorIDsRecord{
    
    public:
    
        int nfactor;
    
        int current;
        
        int mat_factor_size = 0;
    
        Variable variable;
    
        FullID seconds[1];
    
        void copy(VariableFactorIDsRecord & to) const{
            to.nfactor = nfactor; to.current = current; to.variable = variable; to.mat_factor_size=mat_factor_size;
            for(int i=0;i<to.nfactor;i++){}
        }
    
        size_t size() const{
            return sizeof(VariableFactorIDsRecord) + nfactor*sizeof(FullID);
        }
    
        VariableFactorIDsRecord(){
            nfactor = 0; current = 0; mat_factor_size=0;
        }
    
        VariableFactorIDsRecord(const VariableFactorIDsRecord &source){assert(false);}
    
        VariableFactorIDsRecord & operator=(const VariableFactorIDsRecord & source){assert(false);}
    
        void push(const FullID & fid){
        	//std::cout << this->variable.oid << "   fid=" << fid.oid << "     "<< "Current " << current << " nfactor " << nfactor << std::endl;

            if(current != 0 && fid.oid < seconds[current-1].oid){
                std::cout << "ERROR: Sorry but we want all factors inserted in an ascending order." << std::endl;
                assert(false);
            }
            
            // we do not want to insert duplicated factors.
            if(current != 0 && fid.oid == seconds[current-1].oid){
                return;
            }

            if(current >= nfactor)
            {
            	std::cout << this->variable.oid << "   fid=" << fid.oid << "     "<< "Current " << current << " nfactor " << nfactor << std::endl;
            	assert(current < nfactor);
            }
        
            seconds[current] = fid;
            current ++;
        }
        
        void set_n_factor_offset(int _nfactor, int _offset){assert(false);}
        
        int get_n_factor_offset(int _nfactor){assert(false);}
        
        void set_n_factor_fullid(int _nfactor, const FullID & _id){assert(false);}
        
        FullID get_n_factor_fullid(int _nfactor){return seconds[_nfactor];}
        
        void set_n_factor_content(int _nfactor, const Factor & factor){assert(false);}
        
        Factor & get_n_factor_content(int _nfactor){assert(false);}    
    };
    
    
    
    
}

#endif
