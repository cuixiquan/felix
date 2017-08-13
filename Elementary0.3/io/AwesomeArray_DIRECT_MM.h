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
//  Filename: AwesomeArray_DIRECT_MM.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//

#ifndef Elementary_0_2_AwesomeArray_DIRECT_MM_h
#define Elementary_0_2_AwesomeArray_DIRECT_MM_h

#include "../util/Common.h"
#include "AwesomeArray.h"
namespace hazy{
    
    
    template<class FUNCTION, class VALUE, class... AUXS>
    inline void batch_func(const int step, const int start, const int stop, const FUNCTION & func, VALUE* content, AUXS &... aux){
        for(int i=start;i<stop;i+=step){
            func(content[i], aux...);
        }
    }
    
    /**
     * A specification of AwesomeArray that stores in main memory, without any buffer
     * and pager. 
     *
     * See AwesomeArray for details.
     */
    template<class VALUE>
    class AwesomeArray<VALUE, MM, DIRECT, NONE, FIXLEN>{
        
    private:
        
        Lock lock; /**< Mutex while expanding content by expand(). */
        
        VALUE INITVALUE; /**< Default value for an un-loaded key.*/
        
        const bool hasInitValue; /**< Whether INITVALUE is provided.*/
        
        Long nio; /**< Count for number of IOs.*/
        
        Long size; /**< Current size of the content array.*/
        
        OID maxid; /**< Max key ever seen. */
        
        /**
         * If the key load() into the array is larger than size, expand the array
         * by 2x. Filling empty values with INITVALUE if provided. This function is
         * thread safe.
         *
         * <br/><b>Design Decision:</b> we do not do any fancier locking because
         * this function is only called while load(), and will not be used in Gibbs sampling.
         */
        void expand(){
            
            lock.lock();
            
            VALUE * oldcontent = content;
            VALUE * newcontent = new VALUE[size*2];
            memcpy(newcontent, content, size*sizeof(VALUE));
            
            if(hasInitValue){
                for(OID i=size;i<size*2;i++){
                    newcontent[i] = INITVALUE;
                }
            }
            
            content = newcontent;
            size = size * 2;
            
            delete [] oldcontent;
            
            lock.unlock();
        }
        
    public:
        
        VALUE * content; /**< An in-memory array that holds the data. */
        
        std::string name; /**< Name of this AwesomeArray. **/
        
        Long get_maxid(){
            return maxid;
        }
        
        /**
         * See AwesomeArray::AwesomeArray()
         */
        AwesomeArray(std::string _name="") : 
                        size(INIT_ARRAY_SIZE), hasInitValue(false){
            content = new VALUE[INIT_ARRAY_SIZE];
            maxid = -1;
            nio = 0;
        }
        
        
        /**
         * Constructor with INITVALUE.
         */
        AwesomeArray(
                     VALUE initvalue, /**<  Value of INITVALUE. */
                     std::string _name=""
                     ) : size(INIT_ARRAY_SIZE), INITVALUE(initvalue), hasInitValue(true){
            content = new VALUE[INIT_ARRAY_SIZE];
            maxid = -1;
            nio = 0;
        }
        
        void load (const OID & oid, const VALUE & value, RID & rid){
            
            while (oid >= size){
                expand();
            }
            
            if(oid >= maxid){
                maxid = oid;
            }
            
            content[oid] = value;
            
            // for MM, RID=OID
            rid.first = 0;
            rid.second = oid;
            
        }
        
        void to_rid (const OID & oid, RID & rid){
            rid.first = 0;
            rid.second = oid;
        }
        
        void get_oid (const OID & oid, VALUE & value){
            value = content[oid];
        }
        
        void get_rid (const RID & rid, VALUE & value){
            value = content[rid.second];
        }
        
        void set_oid (const OID & oid, const VALUE & value){
            content[oid] = value;
        }
        
        void set_rid (const RID & rid, const VALUE & value){
            content[rid.second] = value;
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_oid (const OID & oid, const FUNCTION & func, AUXS & ... aux){
            return func(content[oid], aux...);
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_rid (const RID & rid, const FUNCTION & func, AUXS & ... aux){
            return func(content[rid.second], aux...);
        }
        
        template<class FUNCTION, class AUX1, class AUX2>
        void batch_apply(const int nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2){
        
            // GCC is not happy with this... Clang is OK.
            //auto batch_func = [&](int step, int start, int stop){
            //    for(int i=start;i<stop;i+=step){
            //        func(content[i], aux...);
            //    }
            //};
            
            std::thread* threads = new std::thread[nthread];
            
            for(int i=0; i<nthread; i++){
                threads[i] = std::thread(batch_func<FUNCTION, VALUE>, nthread, i, maxid+1, func, content, aux1, aux2);
            }
            
            for (int i = 0; i < nthread; ++i) {
                threads[i].join();
            }
            
        }
        
        Long get_nio (){
            return 0;
        }
        
        std::string get_name(){
            return name;
        }
        
        void sigmod_organizePager(){
            
        }
        
    };
    
}


#endif
