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
//  Filename: VarLenStore.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_VarLenStore_h
#define Elementary0_3_VarLenStore_h

#include "Buffer.h"
#include "FixLenStore.h"
#include "../util/Common.h"

#include "AbstractKV.h"

namespace hazy{

    /**
     * Add one variable-length object into a page and returns its record ID.
     *
     * \param content A buffer page that contains the target page.
     * \param page The page header of the target page.
     * \param rid Record ID (will be updated with the actual record ID).
     * \param object The object that we want to add.
     *
     * \tparam VALUE The type of object (in a schema similar to ExampleVarLenObject) stored in VarLenStore.
     **/
    template<class VALUE>
    void _func_push_object_to_page(char* const content, SimplePageHeader & page, RID & rid, const VALUE & object){
        
        size_t size = object.size();
        
        if(size > (page._nslots - 1 - sizeof(size_t))){
        	cout << " Object Size " << size << ". No of page slots " << page._nslots << " sizeof size_t " << sizeof(size_t) <<endl;
            std::cout << "ERROR: Page size is too small." << std::endl;
            assert(false);
        }
        
        if(page._remain_free_slots <= sizeof(size_t) ){
            return;
        }
        if(size >= page._remain_free_slots - sizeof(size_t) ){
            return;
        }
        
        // here, let me just call the copy constructor of VALUE.
        object.copy(reinterpret_cast<VALUE&>(content[page._current_free_slot]));
        
        reinterpret_cast<size_t&>(content[page._current_free_pointer_slot]) = page._current_free_slot;
        
        rid.second = page._current_free_slot;
        
        //if(LOGDEBUG){
        //    std::cout << "PUSH OBJ TO " << page._current_free_slot << " POS POINTER=" << page._current_free_pointer_slot << "  REMIND=" << page._remain_free_slots << std::endl;
        //}
        
        
        page._current_free_slot += size;
        page._remain_free_slots -= size;
        
        page._current_free_pointer_slot -= sizeof(size_t);
        page._remain_free_slots -= sizeof(size_t);
        
        page._nrecord ++;
    };
    
    
    template<class VALUE, class FUNCTION, class... AUXS>
    void _func_apply_var_object_from_page(char* const content, SimplePageHeader & page, const RID & rid, const FUNCTION & func, AUXS&... auxs){
        func(reinterpret_cast<VALUE&>(content[rid.second]), auxs...);
    }
    
    double compt_time;
    
    /****
     * This is ugly and ad hoc. It is a bug of G++. AUX1 and AUX2 should be AUX..., but this only works in LLVM for now.
     ****/
    template<class VALUE, class FUNCTION, class AUX1, class AUX2>
    inline bool _func_batch_apply (char* const content, SimplePageHeader & page, const int & nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2){
        
    
        auto batch_func = [&](int step, int start, int stop){
            Timer t;
            size_t _current_free_pointer_slot;
            for(int i=start;i<stop;i+=step){
                _current_free_pointer_slot = page._nslots - 1 - sizeof(size_t)- i*sizeof(size_t);
                func(reinterpret_cast<VALUE&>(content[reinterpret_cast<int&>(content[_current_free_pointer_slot])]), aux1, aux2);
            }
            compt_time += t.elapsed();
        };
        
        std::thread* threads = new std::thread[nthread];
        
        for(int i=0; i<nthread; i++){
            
            if(nthread == 1){
                batch_func(nthread, i, page._nrecord);
            }else{
                threads[i] = std::thread(batch_func, nthread, i, page._nrecord);
            }
        }
        
        for (int i = 0; i < nthread; ++i) {
             if(nthread == 1){
             }else{
                 threads[i].join();
             }
        }
        
        delete [] threads;
        
        return true;
    };
    
    
    /****
     * This is ugly and ad hoc. It is a bug of G++. AUX1 and AUX2 should be AUX..., but this only works in LLVM for now.
     ****/
    template<class VALUE, class FUNCTION, class AUX1, class AUX2, class AUX3>
    inline bool _func_batch_apply (char* const content, SimplePageHeader & page, const int & nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2, AUX3 & aux3){
        
        
        auto batch_func = [&](int step, int start, int stop){
            Timer t;
            size_t _current_free_pointer_slot;
            for(int i=start;i<stop;i+=step){
                _current_free_pointer_slot = page._nslots - 1 - sizeof(size_t)- i*sizeof(size_t);
                func(reinterpret_cast<VALUE&>(content[reinterpret_cast<int&>(content[_current_free_pointer_slot])]), aux1, aux2, aux3);
            }
            compt_time += t.elapsed();
        };
        
        std::thread* threads = new std::thread[nthread];
        
        for(int i=0; i<nthread; i++){
            
            if(nthread == 1){
                batch_func(nthread, i, page._nrecord);
            }else{
                threads[i] = std::thread(batch_func, nthread, i, page._nrecord);
            }
        }
        
        for (int i = 0; i < nthread; ++i) {
            if(nthread == 1){
            }else{
                threads[i].join();
            }
        }
        
        delete [] threads;
        
        return true;
    };
  
    
    template<class VALUE, class STORAGE, class PAGER>
    class VarLenStore{
        
    public:
        
        SimpleBuffer<STORAGE, PAGER> pages;
        
        FixLenStore<RID, STORAGE, PAGER> oid2rid;
        
        VarLenStore() : pages(SimpleBuffer<STORAGE, PAGER>(PAGESIZE, BUFFERSIZE)){
            _current_page = 0;
            pages.load(_current_page);
        }
        
        VarLenStore(size_t pagesize, size_t buffersize) : pages(SimpleBuffer<STORAGE, PAGER>(pagesize, buffersize)),
                                                        oid2rid(FixLenStore<RID, STORAGE, PAGER>(pagesize, buffersize)){
            _current_page = 0;
            pages.load(_current_page);
        }

        
        void load (const OID & oid, const VALUE & value, RID & rid){
            
            if(oid > _max_id){
                _max_id = oid;
            }
            
            rid.first = _current_page;
            rid.second = -1;
            
            //pages.template apply2<decltype(_func_push_object_to_page<VALUE>),
            //        _func_push_object_to_page<VALUE>>(rid.first, rid, value);
            
            pages.apply(rid.first, _func_push_object_to_page<VALUE>, rid, value);
            
            if(rid.second == -1){
                _current_page ++;
                pages.load(_current_page);
            
                rid.first = _current_page;
                rid.second = -1;
                
                //pages.template apply2<decltype(_func_push_object_to_page<VALUE>), _func_push_object_to_page<VALUE>>
                //    (rid.first, rid, value);
            
                pages.apply(rid.first, _func_push_object_to_page<VALUE>, rid, value);
                
            }

            oid2rid.load(oid, rid, dummyrid);
            
            assert(rid.second != -1);
            
        }
        
        void to_rid (const OID & oid, RID & rid){
            oid2rid.get_oid(oid, rid);
        }
        
        template<class FUNCTION, class... AUXS>
        void apply_rid (const RID & rid, const FUNCTION & func, AUXS&... aux){
            
            pages.apply(rid.first, _func_apply_var_object_from_page<VALUE, FUNCTION, AUXS...>, rid, func, aux...);
            
            //pages.template apply2<decltype(_func_apply_var_object_from_page<VALUE, FUNCTION, AUXS...>),
            //                               _func_apply_var_object_from_page<VALUE, FUNCTION, AUXS...>>
            //(rid.first, rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        void apply_oid (const OID & oid, const FUNCTION & func, AUXS&... aux){
            RID rid;
            to_rid(oid, rid);
            apply_rid(rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        void batch_apply(const int nthread, FUNCTION & func, AUXS&... aux){
            compt_time = 0;
            for(Long pid=0; pid<=_current_page; pid++){
                
                pages.apply(pid, _func_batch_apply<VALUE, FUNCTION, AUXS...>, nthread, func, aux...);

                //pages.template apply2<decltype(_func_batch_apply<VALUE, FUNCTION, AUXS...>),
                //                               _func_batch_apply<VALUE, FUNCTION, AUXS...>>
                //(pid, nthread, func, aux...);
            }
//            std::cout << "Comput Time = " << compt_time << std::endl;
        }
        
        Long get_nio (){
            return pages.get_nio();
        }
        
        std::string get_name(){
            return name;
        }
        
        void sigmod_organizePager(){
            pages.sigmod_organizePager();
            oid2rid.sigmod_organizePager();
        }
        
        Long get_maxid(){
            return _max_id;
        }
        
    private:
        
        std::string name;
        
        RID dummyrid;
        
        Long _current_page;
        
        OID _max_id = -1;
        
    };
    
    
}


#endif
