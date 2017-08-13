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
//  Filename: VariableRecordEager.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/17/13.
//

#ifndef Elementary0_3_VariableRecordEager_h
#define Elementary0_3_VariableRecordEager_h

#include "Variable.h"
#include "Factor.h"

namespace hazy{
    

    
class VariableFactorsRecord{
    
public:
    
    int nfactor;
    
    int current_factor;
    
    int current_factor_offset;
    
    int statesize;
    
    Variable variable;
    
    char seconds[1];
    
    VariableFactorsRecord(){
        
        nfactor = 0;
        current_factor = 0;
        current_factor_offset = 0;
        statesize = 0;
    }
    
    void copy(VariableFactorsRecord & to) const{
        
        to.nfactor = nfactor;
        to.variable = variable;
        to.current_factor = current_factor;
        to.current_factor_offset = current_factor_offset;
        to.statesize = statesize;
        
        //for(int i=0;i<to.statesize;i++){}
    }
    
    size_t size() const{
        return sizeof(VariableFactorsRecord) + statesize*sizeof(char);
    }
    
    
    void push(const FullID & fid, const Factor & factor){
        assert(current_factor < nfactor);
        
        if(current_factor != 0 && fid.oid < get_n_factor_fullid(current_factor-1).oid){
            std::cout << "ERROR: Sorry but we want all factors inserted in an ascending order." << std::endl;
            assert(false);
        }
        
        // we do not want to insert duplicated factors.
        if(current_factor != 0 && fid.oid == get_n_factor_fullid(current_factor-1).oid){
            return;
        }
        
        set_n_factor_offset(current_factor, current_factor_offset);
        set_n_factor_content(current_factor, factor);
        set_n_factor_fullid(current_factor, fid);
        
        current_factor ++;
        current_factor_offset += sizeof(FullID) + factor.size();
    }
    
    void set_n_factor_offset(int _nfactor, int _offset){
        reinterpret_cast<int*>(seconds)[_nfactor] = _offset;
    }
    
    inline int get_n_factor_offset(int _nfactor){
        return reinterpret_cast<int*>(seconds)[_nfactor];
    }
    
    void set_n_factor_fullid(int _nfactor, const FullID & _id){
        reinterpret_cast<FullID&>(seconds[get_n_factor_offset(_nfactor)]) = _id;
    }
    
    FullID get_n_factor_fullid(int _nfactor){
        return reinterpret_cast<FullID&>(seconds[get_n_factor_offset(_nfactor)]);
    }
    
    void set_n_factor_content(int _nfactor, const Factor & factor){
        Factor & f = reinterpret_cast<Factor&>(seconds[get_n_factor_offset(_nfactor)
                                                       + sizeof(FullID)]);
        factor.copy(f);
        
        for(int i=0;i<factor.statesize;i++){
            f.state[i] = factor.state[i];
        }
    }
    
    inline Factor & get_n_factor_content(int _nfactor){
        return reinterpret_cast<Factor&>(seconds[get_n_factor_offset(_nfactor)
                                                 + sizeof(FullID)]);
    }
    
    VariableFactorsRecord(const VariableFactorsRecord &source){assert(false);}
    
    VariableFactorsRecord & operator=(const VariableFactorsRecord & source){assert(false);}
    
};
    
}


#endif
