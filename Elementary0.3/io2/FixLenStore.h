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
//  Filename: FixLenStore.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_FixLenStore_h
#define Elementary0_3_FixLenStore_h

#include "AbstractKV.h"
#include "Buffer.h"
#include "../util/Common.h"


namespace hazy{
    
    template<class VALUE>
    void _fetch_assignment(VALUE & assignment, double & value){
        assert(false);
    }
    
    template<>
    void _fetch_assignment(Assignment & assignment, double & value){
        value = assignment.cvalue;
    }

    
    
    
    
    template<class VALUE>
    void _func_set_object_to_page(char* const content, SimplePageHeader & page, const RID & rid, const VALUE & object){
        (reinterpret_cast<VALUE* const>(content))[rid.second] = object;
    };
    
    template<class VALUE>
    void _func_get_object_from_page(char* const content, SimplePageHeader & page, const RID & rid, VALUE & object){
        object = (reinterpret_cast<VALUE* const>(content))[rid.second];
    }
    
    template<class VALUE, class FUNCTION, class... AUXS>
    void _func_apply_object_from_page(char* const content, SimplePageHeader & page, const RID & rid, const FUNCTION & func, AUXS&... auxs){
        func((reinterpret_cast<VALUE* const>(content))[rid.second], auxs...);
    }
    
    template<class VALUE, class STORAGE, class PAGER>
    class FixLenStore : public AbstractKV<VALUE>{
        
    public:
        
        SimpleBuffer<STORAGE, PAGER> pages;
        
        const size_t NOBJ_PER_PAGE;
        
        FixLenStore() : pages(SimpleBuffer<STORAGE, PAGER>(PAGESIZE, BUFFERSIZE)), NOBJ_PER_PAGE(PAGESIZE/sizeof(VALUE)){
            _current_page = -1;
        }
        
        FixLenStore(size_t pagesize, size_t buffersize) : pages(SimpleBuffer<STORAGE, PAGER>(pagesize, buffersize)), NOBJ_PER_PAGE(pagesize/sizeof(VALUE)){
            _current_page = -1;
        }
        
        void load (const OID & oid, const VALUE & value, RID & rid){
            
            if(oid > _max_id){
                _max_id = oid;
            }
            
            rid.first = oid/NOBJ_PER_PAGE;
            rid.second = oid%NOBJ_PER_PAGE;
            
            for(; _current_page <= rid.first; _current_page++){
                pages.load(rid.first);
            }
            
            pages.apply(rid.first, _func_set_object_to_page<VALUE>, rid, value);
            
        }
                
        void get_rid (const RID & rid, VALUE & value){
            pages.apply(rid.first, _func_get_object_from_page<VALUE>, rid, value);
        }
        
        template<class FUNCTION, class... AUXS>
        void apply_rid (const RID & rid, const FUNCTION & func, AUXS&... aux){
            pages.apply(rid.first, _func_apply_object_from_page<VALUE, FUNCTION, AUXS...>, rid, func, aux...);
        }
        
        Long get_nio (){
            return pages.get_nio();
        }
        
        std::string get_name(){
            return name;
        }
        
        void to_rid (const OID & oid, RID & rid){
            rid.first = oid/NOBJ_PER_PAGE;
            rid.second = oid%NOBJ_PER_PAGE;
        }
        
        void get_oid (const OID & oid, VALUE & value){
            RID rid;
            to_rid(oid, rid);
            get_rid(rid, value);
        }
        
        template<class FUNCTION, class... AUXS>
        void apply_oid (const OID & oid, const FUNCTION & func, AUXS&... aux){
            RID rid;
            to_rid(oid, rid);
            pages.apply(rid.first, _func_apply_object_from_page<VALUE, FUNCTION, AUXS...>, rid, func, aux...);
        }
        
        void sigmod_organizePager(){
            pages.sigmod_organizePager();
        }
        
        Long get_maxid(){
            return _max_id;
        }
        
        void apply_oid_speical_for_assignments (const OID & oid, double & assignment){
            apply_oid(oid, _fetch_assignment<VALUE>, assignment);
            
        }
        
    private:
        
        std::string name;
        
        Long _current_page;
        
        OID _max_id = -1;
        
    };

    
}

#endif
