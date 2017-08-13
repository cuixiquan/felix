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
//  Filename: AwesomeArray_PAGEDBUFFER_FIXLEN.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/19/13.
//

#ifndef Elementary0_3_AwesomeArray_PAGEDBUFFER_FIXLEN_h
#define Elementary0_3_AwesomeArray_PAGEDBUFFER_FIXLEN_h

#include "../util/Common.h"
#include "AwesomeArray.h"
#include "AwesomeArray_BUFFER.h"

#include <set>

namespace hazy{
    
    class Page{
    public:
        
        Long _page_id;
        
        Long        _current_free_slot = 0;
        
        Long        _current_free_pointer_slot = PAGESIZE_IN_BYTE - 1 - sizeof(int);
        
        int        _nrecord = 0;
        
        Long        _remain_free_slots = PAGESIZE_IN_BYTE - 1 - sizeof(int);
        
        Long        _nslots = PAGESIZE_IN_BYTE;
        
        char       content[PAGESIZE_IN_BYTE];
        
        Page(Long __pageid){
            _page_id = __pageid;
        }
        
        Page(){}
        
    };

    
    /**
     * I am not thread-safe!
     **/
    template<class TYPE>
    inline bool push_fixlen_obj_to_page(Page & page, const TYPE & object, const RID & rid){
        
        (reinterpret_cast<TYPE*>(page.content))[rid.second] = object;
                
        page._nrecord ++;
        
        return true;
    }
    
    template<class VALUE>
    inline bool _func_push_fixlen_tuple(Page& page, const VALUE& value, const RID & rid){
        return push_fixlen_obj_to_page(page, value, rid);
    };
    
    template<class VALUE>
    inline bool _func_get_fixlen_tuple (Page& page, VALUE& value, const RID & rid){
        value= (reinterpret_cast<VALUE*>(page.content))[rid.second];
        return true;
    };
    
    template<class VALUE>
    inline bool _func_set_fixlen_tuple(Page& page, const VALUE& value, const RID & rid){
        (reinterpret_cast<VALUE*>(page.content))[rid.second] = value;
        return true;
    };
    
    template<class VALUE, class FUNCTION, class... AUXS>
    inline bool _func_apply_fixlen_tuple (Page& page, const RID & rid, const FUNCTION & func, AUXS&... aux){
        func((reinterpret_cast<VALUE*>(page.content))[rid.second], aux...);
        return true;
    }
    
    
    /****
     * This is ugly and ad hoc. It is a bug of G++
     ****/
    template<class VALUE, class FUNCTION, class AUX1, class AUX2>
    inline bool _func_batch_fixlen_apply (Page& page, const int & nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2){
        
        auto batch_func = [&](int step, int start, int stop){
            for(int i=start;i<stop;i+=step){
                func((reinterpret_cast<VALUE*>(page.content))[i], aux1, aux2);
            }
        };
        
        std::thread* threads = new std::thread[nthread];
        
        for(int i=0; i<nthread; i++){
            threads[i] = std::thread(batch_func, nthread, i, page._nrecord);
        }
        
        for (int i = 0; i < nthread; ++i) {
            threads[i].join();
        }
        
        delete [] threads;
        
        return true;
    };
    
    
    template<class VALUE, StorageType STORAGETYPE, BufferType BUFFERTYPE>
    class AwesomeArray<VALUE, STORAGETYPE, PAGED, BUFFERTYPE, FIXLEN>{
        
    private:
        
        std::string name;
        
        AwesomeArray<Page, STORAGETYPE, SIMPLEBUFFER, BUFFERTYPE, FIXLEN> pages;
        
        Long _current_page;
        
        RID dummyrid;
        
        const int NOBJ_PER_PAGE = PAGESIZE_IN_BYTE/(sizeof(VALUE) + sizeof(int) + 1);
        
        OID _max_id = -1;
        
        std::set<int> pagesets; /*<  <b>Design Decision:</b> We believe that the page table should fit in memory. For pagesize=1M, we only need 1M pages for 1TB. 1M integers are just 4MB. */
        
    public:
        
        Long get_maxid(){
            return _max_id;
        }
        
        /**
         * See AwesomeArray::AwesomeArray()
         */
        AwesomeArray(std::string _name="") : _current_page(0){
            
            name = _name;
            
            if(NOBJ_PER_PAGE == 0){
                std::cout << "ERROR: Page size is too small." << std::endl;
                assert(false);
            }
        }
        
        void load (const OID & oid, const VALUE & value, RID & rid){
            if(oid > _max_id){
                _max_id = oid;
            }
            
            rid.first = oid/NOBJ_PER_PAGE;
            rid.second = oid%NOBJ_PER_PAGE;
            
            // if page does not exist
            if(pagesets.find(rid.first) == pagesets.end()){
                pages.load(rid.first, Page(rid.first), dummyrid);
                pagesets.insert(rid.first);
            }
            bool rs = pages.apply_oid(rid.first, _func_push_fixlen_tuple<VALUE>, value, rid);
                        
            assert(rs==true);
            
        }
        
        void to_rid (const OID & oid, RID & rid){
            rid.first = oid/NOBJ_PER_PAGE;
            rid.second = oid%NOBJ_PER_PAGE;
        }
        
        void get_oid (const OID & oid, VALUE & value){
            RID rid;
            this->to_rid(oid, rid);
            get_rid(rid, value);
        }
        
        void get_rid (const RID & rid, VALUE & value){
            
            pages.apply_oid(rid.first, _func_get_fixlen_tuple<VALUE>, value, rid);
            
        }
        
        void set_oid (const OID & oid, const VALUE & value){
            RID rid;
            this->to_rid(oid, rid);
            set_rid(rid, value);
        }
        
        void set_rid (const RID & rid, const VALUE & value){
            pages.apply_oid(rid.first, _func_set_fixlen_tuple<VALUE>, value, rid);
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_oid (const OID & oid, const FUNCTION & func, AUXS&... aux){
            RID rid;
            this->to_rid(oid, rid);
            return apply_rid(rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_rid (const RID & rid, const FUNCTION & func, AUXS&... aux){
            return pages.apply_oid(rid.first, _func_apply_fixlen_tuple<VALUE, FUNCTION, AUXS...>, rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        void batch_apply(const int nthread, FUNCTION & func, AUXS&... aux){
            for(Long pid=0; pid<=_current_page; pid++){
                pages.apply_oid(pid, _func_batch_fixlen_apply<VALUE, FUNCTION, AUXS...>, nthread, func, aux...);
            }
        }
        
        Long get_nio (){
            return pages.get_nio();
        }
        
        std::string get_name(){
            return name;
        }
        
        void sigmod_organizePager(){
            pages.sigmod_organizePager();
        }
        
    };
    
    
}

#endif
