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
//  Filename: Pager.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//

#ifndef Elementary_0_2_Pager_h
#define Elementary_0_2_Pager_h

#include "../util/Common.h"

namespace hazy{

    template<BufferType BUFFERTYPE>
    class Pager{
    public:
                
        Pager(Long _NBUFFER){
            std::cout << "ERROR: You must select a un-NONE BUFFERTYPE!" << std::endl;
            assert(false);
        }
        
        Long get_toevict(){
            return -1;
        }
        
        void update(Long objid, Long bufid){
            
        }
        
        void organize(){}

    };
    
}


#endif
