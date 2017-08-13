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
//  Filename: Buffer.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_Buffer_h
#define Elementary0_3_Buffer_h

#include <new>
#include "Page.h"

namespace hazy{
    
    template<class DIRECTIO, class PAGER>
    class SimpleBuffer{
        
    public:
        
        size_t n_rio, n_wio;
        
        size_t nmiss;
        
        DIRECTIO direct_io;
        
        PAGER pager;
        
        BufferPageHeader * bufs;
        
        SimplePageHeader * page_table;
        
        size_t _current_n_page_table;
        
        const size_t buffer_size;
        
        const size_t nbuffer;
        
        const size_t frame_size;
        
        SimpleBuffer(size_t pagesize, size_t buffersize):
        frame_size(pagesize),
        buffer_size(buffersize),
        nbuffer(buffersize/pagesize),
        pager(buffersize/pagesize),
        direct_io(DIRECTIO(pagesize))
        {
            assert(nbuffer > 0);
            
            bufs = new BufferPageHeader[nbuffer];
            
            for(int i=0;i<nbuffer;i++){
                bufs[i].frame_size = pagesize;
                bufs[i].content = new char[bufs[i].frame_size];
            }
            
            _current_n_page_table = 250000;
            page_table = new SimplePageHeader[_current_n_page_table];
            
            for(int i=0;i<_current_n_page_table;i++){
                page_table[i].frame_size = pagesize;
                page_table[i].p_buffer_page = NULL;
                
                page_table[i]._current_free_slot = 0;
                page_table[i]._current_free_pointer_slot = pagesize - 1 - sizeof(size_t);
                page_table[i]._remain_free_slots = pagesize - 1 - 1 - sizeof(size_t);
                page_table[i]._nslots = pagesize;
            }
            
            n_rio = 0;
            n_wio = 0;
            nmiss = 0;
        }
        
        size_t get_nio(){
            return n_rio + n_wio;
            //return nmiss;
        }
        
        template<class FUNCTION, class ... ARGS>
        void apply(size_t frame_id, FUNCTION func, ARGS&... args){
            
            assert(page_table[frame_id].frame_id == frame_id);
            
            if(page_table[frame_id].p_buffer_page == NULL){
                
                nmiss ++;
                
                size_t bufid;
                
                bufid = pager.get_toevict();
                
                if(bufs[bufid].page_id != -1){
                    direct_io.put(bufs[bufid], bufs[bufid].page_id);
                    n_wio++;
                    
                    page_table[bufs[bufid].page_id].p_buffer_page = NULL;
                    bufs[bufid].page_id = -1;
                }
                
                assert(bufs[bufid].page_id = -1);
                bufs[bufid].page_id = frame_id;
                direct_io.get(bufs[bufid], bufs[bufid].page_id);
                n_rio++;
                
                page_table[frame_id].p_buffer_page = &bufs[bufid];
                page_table[frame_id].buf_id = bufid;
            }
            
            assert(page_table[frame_id].p_buffer_page != NULL);
            func(page_table[frame_id].p_buffer_page->content, page_table[frame_id], args...);
            pager.update(frame_id, page_table[frame_id].buf_id);
            
        }
        
        void load(size_t frame_id){
            
            if(frame_id >= _current_n_page_table){
                std::cout << "[INFO] Expanding page header table!" << std::endl;
                assert(false);
            }
            
            direct_io.load(frame_id);
            n_wio ++;
            
            page_table[frame_id].frame_id = frame_id;
            page_table[frame_id].p_buffer_page = NULL;
        }
        
        void sigmod_organizePager(){
            pager.organize();
        }
        
    };
    
}

#endif









