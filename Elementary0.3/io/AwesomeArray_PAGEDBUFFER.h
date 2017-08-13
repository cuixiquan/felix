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
//  Filename: AwesomeArray_PAGEDBUFFER.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//


#ifndef Elementary_0_2_AwesomeArray_PAGEDBUFFER_h
#define Elementary_0_2_AwesomeArray_PAGEDBUFFER_h

#include "../util/Common.h"
#include "AwesomeArray.h"
#include "AwesomeArray_BUFFER.h"
#include "AwesomeArray_PAGEDBUFFER_FIXLEN.h"


namespace hazy{
    
    /**
     * A VarLengthObject is the object that can be stored in a Page.
     * It contains two component, 1) a fixed-length object, and 2) an
     * array of variable-length objects. Only the usage of VarLengthObject
     * in a PagedBuffer is well-defined.
     *
     * \tparam FIX_LEN_OBJECT 
     * \tparam VAR_LEN_OBJECT 
     */
    template<class FIX_LEN_OBJECT, class VAR_LEN_OBJECT>
    struct VarLengthObject{
        
    public:
        
        int len;
        
        int current;
        
        int size_first;
        
        int size_meta_second;
        
        int size_aux;
    
        FIX_LEN_OBJECT first;
        
        VAR_LEN_OBJECT seconds[VAR_LEN_VARIABLE_MAX_NELEMENTS];
        
        VarLengthObject() : len(0), current(0),
                            size_first(sizeof(FIX_LEN_OBJECT)),
                            size_meta_second(sizeof(VAR_LEN_OBJECT)),
                            size_aux(4*sizeof(int)){}
        
    };
    
    
    /**
     * I am not thread-safe!
     **/
    template<class TYPE>
    inline bool push_varlen_obj_to_page(Page & page, const TYPE & object, RID & rid){
        
        int size = object.size_first + object.size_meta_second*object.len + object.size_aux;
        
        if(size > (page._nslots - 1 - sizeof(int))){
            std::cout << "ERROR: Page size is too small." << std::endl;
            assert(false);
        }
        if(page._remain_free_slots <= 0){
            return false;
        }
        if(size > page._remain_free_slots){
            return false;
        }
        
        
        rid.first = page._page_id;
        rid.second = page._current_free_slot;
        
        TYPE& CASTED = reinterpret_cast<TYPE&>(page.content[page._current_free_slot]);
        CASTED.first = object.first;
        CASTED.len = object.len;
        CASTED.current = object.current;
        CASTED.size_first = object.size_first;
        CASTED.size_meta_second = object.size_meta_second;
        CASTED.size_aux = object.size_aux;
        for(int i=0;i<object.len;i++){
            CASTED.seconds[i] = object.seconds[i];
        }
        
        reinterpret_cast<int&>(page.content[page._current_free_pointer_slot]) = page._current_free_slot;
        
        page._current_free_slot += size;
        page._remain_free_slots -= size;
        
        page._current_free_pointer_slot -= sizeof(int);
        page._remain_free_slots -= sizeof(int);
        
        page._nrecord ++;
        
        return true;
    }
    
    template<class VALUE>
    inline bool _func_push_tuple(Page& page, const VALUE& value, RID & rid){
        return push_varlen_obj_to_page(page, value, rid);
    };
    
    template<class VALUE>
    inline bool _func_get_tuple (Page& page, VALUE& value, const RID & rid){
        VALUE& CASTED = reinterpret_cast<VALUE&>(page.content[rid.second]);
        value.first = CASTED.first;
        value.len = CASTED.len;
        value.current = CASTED.current;
        value.size_first = CASTED.size_first;
        value.size_meta_second = CASTED.size_meta_second;
        value.size_aux = CASTED.size_aux;
        for(int i=0;i<CASTED.len;i++){
            value.seconds[i] = CASTED.seconds[i];
        }
        //value = reinterpret_cast<VALUE&>(page.content[rid.second]);
        return true;
    };
    
    template<class VALUE>
    inline bool _func_set_tuple(Page& page, const VALUE& value, const RID & rid){
        //reinterpret_cast<VALUE&>(page.content[rid.second]) = value;
        VALUE& CASTED = reinterpret_cast<VALUE&>(page.content[rid.second]);
        CASTED.first = value.first;
        CASTED.len = value.len;
        CASTED.current = value.current;
        CASTED.size_first = value.size_first;
        CASTED.size_meta_second = value.size_meta_second;
        CASTED.size_aux = value.size_aux;
        for(int i=0;i<value.len;i++){
            CASTED.seconds[i] = value.seconds[i];
        }
        return true;
    };
    
    template<class VALUE, class FUNCTION, class... AUXS>
    inline bool _func_apply_tuple (Page& page, const RID & rid, const FUNCTION & func, AUXS&... aux){
        func(reinterpret_cast<VALUE&>(page.content[rid.second]), aux...);
        return true;
    };
    
    
    /****
     * This is ugly and ad hoc. It is a bug of G++
     ****/
    template<class VALUE, class FUNCTION, class AUX1, class AUX2>
    inline bool _func_batch_apply (Page& page, const int & nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2){
        
        auto batch_func = [&](int step, int start, int stop){
            int _current_free_pointer_slot;
            for(int i=start;i<stop;i+=step){
                _current_free_pointer_slot = PAGESIZE_IN_BYTE - 1 - sizeof(int)- i*sizeof(int);
                func(reinterpret_cast<VALUE&>(page.content[
                        reinterpret_cast<int&>(page.content[_current_free_pointer_slot])]), aux1, aux2);
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
    class AwesomeArray<VALUE, STORAGETYPE, PAGED, BUFFERTYPE, VARLEN>{
        
    private:
        
        std::string name;
        
        AwesomeArray<Page, STORAGETYPE, SIMPLEBUFFER, BUFFERTYPE, FIXLEN> pages;
        
        AwesomeArray<RID, STORAGETYPE, PAGED, BUFFERTYPE, FIXLEN> oid2rid;
        
        Long _current_page;
                
        RID dummyrid;
               
    public:
      
        Long get_maxid(){
            return oid2rid.get_maxid();
        }
        
        /**
         * See AwesomeArray::AwesomeArray()
         */
        AwesomeArray(std::string _name="") : _current_page(0){
            
            name = _name;
            pages.load(0, Page(0), dummyrid);
        }
        
        void load (const OID & oid, const VALUE & value, RID & rid){
            
            // first try to insert to current page
            bool rs = pages.apply_oid(_current_page, _func_push_tuple<VALUE>, value, rid);
            if(rs == true){
                
                //std::cout << oid << ": RID=" << rid.first << ", " << rid.second << std::endl;
                oid2rid.load(oid, rid, dummyrid);
            }
            // we need to insert a new page
            if(rs == false){
                _current_page ++;
                pages.load(_current_page, Page(_current_page), dummyrid);
                rs = pages.apply_oid(_current_page, _func_push_tuple<VALUE>, value, rid);
                
                //std::cout << oid << ": RID=" << rid.first << ", " << rid.second << std::endl;
                oid2rid.load(oid, rid, dummyrid);
            }

            assert(rs==true);
            
        }
        
        void to_rid (const OID & oid, RID & rid){
            oid2rid.get_oid(oid, rid);
        }
        
        void get_oid (const OID & oid, VALUE & value){
            RID rid;
            oid2rid.get_oid(oid, rid);
            get_rid(rid, value);
        }
        
        void get_rid (const RID & rid, VALUE & value){
            
            pages.apply_oid(rid.first, _func_get_tuple<VALUE>, value, rid);
                                           
        }
        
        void set_oid (const OID & oid, const VALUE & value){
            RID rid;
            oid2rid.get_oid(oid, rid);
            set_rid(rid, value);
        }
        
        void set_rid (const RID & rid, const VALUE & value){
            pages.apply_oid(rid.first, _func_set_tuple<VALUE>, value, rid);
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_oid (const OID & oid, const FUNCTION & func, AUXS&... aux){
            RID rid;
            oid2rid.get_oid(oid, rid);
            
            return apply_rid(rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_rid (const RID & rid, const FUNCTION & func, AUXS&... aux){
            return pages.apply_oid(rid.first, _func_apply_tuple<VALUE, FUNCTION, AUXS...>, rid, func, aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        void batch_apply(const int nthread, FUNCTION & func, AUXS&... aux){
            for(Long pid=0; pid<=_current_page; pid++){
                pages.apply_oid(pid, _func_batch_apply<VALUE, FUNCTION, AUXS...>, nthread, func, aux...);
            
            }
        }
        
        Long get_nio (){
            return pages.get_nio() + oid2rid.get_nio();
        }
        
        std::string get_name(){
            return name;
        }
        
        void sigmod_organizePager(){
            pages.sigmod_organizePager();
            oid2rid.sigmod_organizePager();
        }
        
    };
}



#endif
