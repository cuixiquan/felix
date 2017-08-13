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
//  Filename: AbstractKV.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/16/13.
//

#ifndef Elementary0_3_AbstractKV_h
#define Elementary0_3_AbstractKV_h


namespace hazy{
    
    template<class VALUE>
    class AbstractKV{
        
    public:
                
        virtual void load (const OID & oid, const VALUE & value, RID & rid) = 0;
            
        virtual void to_rid (const OID & oid, RID & rid) = 0;
        
        //template<class FUNCTION, class... AUXS>
        //void apply_rid (const RID & rid, const FUNCTION & func, AUXS&... aux) {assert(false);}
        
        //template<class FUNCTION, class... AUXS>
        //void apply_oid (const OID & oid, const FUNCTION & func, AUXS&... aux) {assert(false);};
        
        //template<class FUNCTION, class... AUXS>
        //void batch_apply(const int nthread, FUNCTION & func, AUXS&... aux) {assert(false);}
        
        virtual void apply_oid_speical_for_assignments (const OID & oid, double & assignment) = 0;
        
        virtual Long get_nio () = 0;
        
        virtual std::string get_name() = 0;
        
        virtual void sigmod_organizePager() = 0;
        
    };
    
}


#endif
