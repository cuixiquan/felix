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
//  Filename: AwesomeArray_BUFFER.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/15/13.
//

#ifndef Elementary_0_2_AwesomeArray_Buffer_h
#define Elementary_0_2_AwesomeArray_Buffer_h

#define MACRO_RCU_CHECK_BEGIN() while(1){ \
rcu_read_lock();\
BufferObject<VALUE> **ppbuf = rcu_dereference(pageheaders[key].pptobuf);\
BufferObject<VALUE> *pbuf = *ppbuf;\
if(pbuf != NULL){

#define MACRO_RCU_CHECK_END() }else{\
rcu_read_unlock();\
rcu_quiescent_state();\
i_want_key_in_buf(key);\
continue;\
}\
rcu_read_unlock();\
rcu_quiescent_state();\
break;\
}
 

#include "../util/Common.h"
#include "AwesomeArray.h"

#include "Pager_RANDOM.h"
#include "Pager_LRU.h"

#include "AwesomeArray_DIRECT_MM.h"
#include "AwesomeArray_DIRECT_LOCALFILE.h"

const bool VERBOSE = false;

namespace hazy{
    
    /**
     * An object in the buffer. Each BufferObject contains the content of the actual
     * buffer'ed object, and some auxilary information, including object ID, whether
     * the buffer'ed object is dirty.
     *
     * \tparam VALUE Type of object that is buffer'ed
     */
    template<class VALUE>
    class BufferObject{
    public:
        OID objid; /**< Object ID buffered in this BufferObject.*/
        
        VALUE obj; /**< Content of the buffered object.*/
        
        bool dirty; /**< Whether this BufferObject is dirty. */
        
        /**
         * Constructor.
         */
        BufferObject(){
            dirty = false;
            objid = -1;
        }
    };
    
    template<class VALUE>
    class PageHeader{
    public:
        OID objid;
        
        int current;
        
        Lock lock;
        
        BufferObject<VALUE>** (_pool_pptobuf)[2];
        
        BufferObject<VALUE>** pptobuf;
        
        PageHeader() {
            current = 0;
            _pool_pptobuf[0] = new BufferObject<VALUE>*;
            _pool_pptobuf[1] = new BufferObject<VALUE>*;
            
            (*_pool_pptobuf[0]) = NULL;
            (*_pool_pptobuf[1]) = NULL;
            
            pptobuf = _pool_pptobuf[current];
        }
        
    };
    
    
    /**
     * This is a specification of a <i>single-threaded</i> AwesomeArray which contains
     * a buffer in the object level:
     *   <ul>
     *        <li> Use arbitary storage backend.  </li>
     *        <li> Use arbitary pager. </li>
     *   </ul>
     *
     * The size of the buffer is defined by hazy::BufferSizeInByte.
     *
     * See AwesomeArray for details.
     */
    template<class VALUE, StorageType STORAGETYPE, BufferType BUFFERTYPE, ObjectType OBJECTTYPE>
    class AwesomeArray<VALUE, STORAGETYPE, SIMPLEBUFFER, BUFFERTYPE, OBJECTTYPE>{

    public:
        
        const Long NBUFFER; /**< Number of buffered object in the buffer. */
        
        BufferObject<VALUE> * buffers; /**< The buffer: a list of BufferObject. */
        
        Long free_buf_id; /**< ID for the first free buffer. */
        
        std::string name;
        
        /**
         * See AwesomeArray::AwesomeArray().
         */
        AwesomeArray(std::string _name="") : NBUFFER(BUFFERSIZE_IN_BYTE/sizeof(BufferObject<VALUE>)),
                            pager(Pager<BUFFERTYPE>(BUFFERSIZE_IN_BYTE/sizeof(BufferObject<VALUE>)))
        {
            
            name = _name;
            buffers = new BufferObject<VALUE>[NBUFFER];
            maxid = -1;
            nio = 0;
            free_buf_id = 0;
            
            pageheaders = new PageHeader<VALUE>[INIT_ARRAY_SIZE];
        }
        
        Long get_nio(){
            return nio;
        }
        
        //~AwesomeArray(){
//            delete [] pageheaders;
//            delete [] buffers;
        //}
        
        void i_want_key_in_buf(OID key){
                        
            int trylock = pageheaders[key].lock.trywlock();
            if(trylock != 0){
                
                return;
                
            }else{
                
                // only I can change ppbuf
                OID candbuf, candpage;
                
                bool newbuf = false;
                
                do{
                    
                    candbuf = pager.get_toevict();
                    
                    candpage = buffers[candbuf].objid;
                    
                    if(candpage != -1){
                        trylock = pageheaders[candpage].lock.trywlock();
                    }
                    
                }while(trylock != 0);
                
                if(candpage != -1){
                    
                    assert(buffers[candbuf].objid == candpage);
                    
                    // now I lock two pages
                    pageheaders[candpage].current = 1 - pageheaders[candpage].current;   // change a new buf obj
                    
                    // swap the candpage, so that later threads will see NULL
                    (*pageheaders[candpage]._pool_pptobuf[pageheaders[candpage].current]) = NULL;
                    
                    assert(pageheaders[candpage]._pool_pptobuf[0] != NULL);
                    assert(pageheaders[candpage]._pool_pptobuf[1] != NULL);
                    
                    rcu_assign_pointer(pageheaders[candpage].pptobuf, pageheaders[candpage]._pool_pptobuf[pageheaders[candpage].current]);
                    
                    assert(pageheaders[candpage]._pool_pptobuf[pageheaders[candpage].current] != NULL);
                    
                    synchronize_rcu();
                    
                    assert((*pageheaders[candpage].pptobuf) == NULL);
                    
                    // now we are free to change candbuf because no one else can see it
                    if(buffers[candbuf].dirty){   //flush if dirty
                        nio ++;
                        directarray.set_oid(candpage, buffers[candbuf].obj);
                        
                    }
                }
                
                nio ++;
                buffers[candbuf].dirty = false;
                directarray.get_oid(key, buffers[candbuf].obj);
                
                buffers[candbuf].objid = key;
                pager.update(key, candbuf);
                
                // then we are free to change key's page because we loaded the buffer
                pageheaders[key].current = 1 - pageheaders[key].current;   // change a new buf obj
                
                (*pageheaders[key]._pool_pptobuf[pageheaders[key].current]) = &buffers[candbuf];
                
                rcu_assign_pointer(pageheaders[key].pptobuf, pageheaders[key]._pool_pptobuf[pageheaders[key].current]);
                
                synchronize_rcu();
                            
                if(candpage != -1){
                    pageheaders[candpage].lock.unlock();
                }
                
                pageheaders[key].lock.unlock();
                
            }
            
        }
        
        void to_rid (const OID & oid, RID & rid){
            rid.first = 0;
            rid.second = oid;
        }
              
        void get_oid (const OID & key, VALUE & value){
            
            
            
            /*
            MACRO_RCU_CHECK_BEGIN();
            
            value = pbuf->obj;
                    
            MACRO_RCU_CHECK_END();*/
            
        }
        
        void get_rid (const RID & rid, VALUE & value){
            
            OID key = rid.second;
            
            MACRO_RCU_CHECK_BEGIN();
            
            value = pbuf->obj;
            
            MACRO_RCU_CHECK_END();
        }
        
        void set_oid (const OID & key, const VALUE & value){
            
            MACRO_RCU_CHECK_BEGIN();
            
            pbuf->obj = value;
            pbuf->dirty = true;
            
            MACRO_RCU_CHECK_END();
            
        }
        
        void set_rid (const RID & rid, const VALUE & value){
            
            OID key = rid.second;
            
            MACRO_RCU_CHECK_BEGIN();
            
            pbuf->obj = value;
            pbuf->dirty = true;
            
            MACRO_RCU_CHECK_END();
            
        }

        template<class FUNCTION, class... AUXS>
        bool apply_oid (const OID & key, const FUNCTION & func, AUXS & ... aux){
                
            while(1){
                rcu_read_lock();
                BufferObject<VALUE> **ppbuf = rcu_dereference(pageheaders[key].pptobuf);
                BufferObject<VALUE> *pbuf = *ppbuf;
                if(pbuf != NULL){
                    
                    pbuf->dirty = true;
                    
                    if(func(pbuf->obj, aux...)){
                        
                        rcu_read_unlock();
                        rcu_quiescent_state();
                        
                        return true;
                    }else{
                        
                        rcu_read_unlock();
                        rcu_quiescent_state();
                        
                        return false;
                    }
                    
            
                }else{
                    rcu_read_unlock();
                    rcu_quiescent_state();
                    i_want_key_in_buf(key);
                    continue;
                }
                rcu_read_unlock();
                rcu_quiescent_state();
                break;
            }
            
            assert(false);
            return false;
        }
        
        template<class FUNCTION, class... AUXS>
        bool apply_rid (const RID & rid, const FUNCTION & func, AUXS & ... aux){
            
            OID key = rid.second;
            
            apply_oid(key, func, aux...);

        }
        
        Lock lock;
        Long _current_n_pageheader = INIT_ARRAY_SIZE;
        
        /**
         * I am not thread safe.
         **/
        void load (const OID & key, const VALUE & value, RID & rid){
            
            directarray.load(key, value, dummyrid);
            
            if(key >= _current_n_pageheader){
                lock.lock();
                Long ideal_size = _current_n_pageheader;
                
                while(key >= ideal_size){
                    ideal_size *= 2;
                }
                    
                PageHeader<VALUE> * oldcontent = pageheaders;
                
                pageheaders = new PageHeader<VALUE>[ideal_size];
                
                memcpy(pageheaders, oldcontent, _current_n_pageheader*sizeof(PageHeader<VALUE>));
                
                _current_n_pageheader = ideal_size;
                    
                delete [] oldcontent;
                    
                lock.unlock();
            }
            
            // when loading, we eagerly put value into the buffer
            // this simplies locking for the actual executing phase
            if(free_buf_id < NBUFFER){
                buffers[free_buf_id].dirty = false;
                buffers[free_buf_id].obj = value;
                buffers[free_buf_id].objid = key;
                (*pageheaders[key]._pool_pptobuf[pageheaders[key].current]) = &buffers[free_buf_id];
                pageheaders[key].pptobuf = pageheaders[key]._pool_pptobuf[pageheaders[key].current];
                
                free_buf_id ++;
            }
            
            if(key > maxid){
                maxid = key;
            }
            
            rid.first = 0;
            rid.second = key;
            
        }
        
        OID get_maxid(){
            return maxid;
        }
       
        void sigmod_organizePager(){
            pager.organize();
        }
        
        
    private:
        
        Long nio; /**< Count for number of IOs.*/
        
        PageHeader<VALUE>* pageheaders;   //!!!!!!!!
       
        AwesomeArray<VALUE, STORAGETYPE, DIRECT, NONE, FIXLEN> directarray; /**< An AwesomeArray without any buffer to store the data.*/
        
        Pager<BUFFERTYPE> pager; /**< An Pager to select which page to evict when the buffer is full.*/
        
        OID maxid; /**< Max key ever seen. */
        
        RID dummyrid;
        
    };
    
}

#endif
