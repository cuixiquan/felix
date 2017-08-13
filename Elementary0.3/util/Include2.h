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
//  Filename: Include2.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/22/13.
//

#ifndef Elementary0_3_Include2_h
#define Elementary0_3_Include2_h

#include "Common.h"
#include "../fg/FactorGraph.h"

namespace hazy{
    AbstractFactorGraph * get_factorgraph_from_config(){
        // this function looks stupid, but it is what we can do.
        
        // std::string EXP_MAT = "LAZY";        [EAGER, LAZY, VCOC, FCOC]
        // std::string EXP_PAGING = "LRU";      [LRU, RANDOM, OPTIMAL]
        // std::string EXP_STORAGE = "FILE";    [MM, FILE, HBASE]
        
        if(EXP_MAT != "LAZY" && EXP_MAT != "EAGER" && EXP_MAT != "VCOC" && EXP_MAT != "FCOC"){
            std::cout << "ERROR: " << EXP_MAT << " is not supported as --mat" << std::endl;
            assert(false);
        }
        
        if(EXP_PAGING != "LRU" && EXP_PAGING != "RANDOM" && EXP_PAGING != "OPTIMAL"){
            std::cout << "ERROR: " << EXP_PAGING << " is not supported as --paging" << std::endl;
            assert(false);
        }
        
        if(EXP_STORAGE != "MM" && EXP_STORAGE != "FILE" && EXP_STORAGE != "HBASE"){
            std::cout << "ERROR: " << EXP_STORAGE << " is not supported as --storage" << std::endl;
            assert(false);
        }
        
        FMatType FMATTYPE = F_LAZY;
        if(EXP_MAT == "EAGER" || EXP_MAT == "FCOC"){
            FMATTYPE = F_EAGER;
        }
        
        if(EXP_PAGING == "LRU" and EXP_STORAGE == "MM"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_MM, Pager<LRU>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_MM, Pager<LRU>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "RANDOM" and EXP_STORAGE == "MM"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_MM, Pager<RANDOM>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_MM, Pager<RANDOM>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "OPTIMAL" and EXP_STORAGE == "MM"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_MM, Pager<OPTIMAL>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_MM, Pager<OPTIMAL>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "LRU" and EXP_STORAGE == "FILE"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_FILE, Pager<LRU>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_FILE, Pager<LRU>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "RANDOM" and EXP_STORAGE == "FILE"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_FILE, Pager<RANDOM>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_FILE, Pager<RANDOM>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "OPTIMAL" and EXP_STORAGE == "FILE"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_FILE, Pager<OPTIMAL>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_FILE, Pager<OPTIMAL>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        if(EXP_PAGING == "LRU" and EXP_STORAGE == "MM"){
            if(EXP_MAT == "LAZY" || EXP_MAT == "FCOC"){
                return new FactorGraph<DirectIO_MM, Pager<LRU>, V_LAZY>(EXP_NUMSYS, FMATTYPE);
            }else{
                return new FactorGraph<DirectIO_MM, Pager<LRU>, V_EAGER>(EXP_NUMSYS, FMATTYPE);
            }
        }
        
        assert(false);
        
        
    }
}

#endif
