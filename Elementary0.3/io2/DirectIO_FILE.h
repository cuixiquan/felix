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
//  Filename: DirectIO_FILE.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_DirectIO_FILE_h
#define Elementary0_3_DirectIO_FILE_h

#include "Page.h"

namespace hazy{
    
    class DirectIO_FILE{
        
    public:
        
        static std::string name(){return "FILE";}
        
        std::string filename;
        
        const size_t frame_size;
        
        int fd;
        
        int current_size;
        
        DirectIO_FILE(size_t pagesize) : frame_size(pagesize) {
            
            filename = getNextTmpFileName();
            
            remove(filename.c_str());
            
#ifdef __MACH__
            fd = open(filename.c_str(), O_RDWR | O_CREAT ,
                      (mode_t) 0600);
#else
            fd = open64(filename.c_str(), O_RDWR | O_CREAT | O_DIRECT,
                        (mode_t) 0600);
#endif
            assert(fd != -1);
            
            char * init;
            int rr = posix_memalign((void**)&init, getpagesize(), frame_size) ;//<< std::endl;
            assert(rr == 0);
            
            current_size = 1;
            
            for(int i=0;i<1;i++){
                
#ifdef __MACH__
                pwrite(fd, init, frame_size, frame_size*i);
#else
                                
                rr = pwrite64(fd, init, frame_size, frame_size*i) ;//<< std::endl;
                assert(rr > 0);
#endif
            }
      
            free(init);
        
        }
        
        /**
         * Not thread-safe!
         **/
        void load(size_t frame_id){
            
            if(frame_id >= current_size){
                
                char * init;
                int rr = posix_memalign((void**)&init, getpagesize(), frame_size) ;//<< std::endl;
                assert(rr == 0);
                
                while(frame_id >= current_size){
                
#ifdef __MACH__
                    pwrite(fd, init, frame_size, frame_size*current_size);
#else
                    
                    rr = pwrite64(fd, init, frame_size, frame_size*current_size) ;//<< std::endl;
                    assert(rr > 0);
#endif
                
                    current_size ++;
                }
                free(init);
            }            
        }
        
        void get(BufferPageHeader & buffer_page, size_t frame_id){
            
#ifdef __MACH__
            pread(fd, buffer_page.content, frame_size, frame_size*frame_id);
#else
            char * init;
            int rr = posix_memalign((void**)&init, getpagesize(), frame_size) ;
            assert(rr==0);
            pread64(fd, init, frame_size, frame_size*frame_id);
            memcpy(buffer_page.content, init, frame_size);
            free(init);
#endif
        }
        
        void put(const BufferPageHeader & buffer_page, size_t frame_id){
            
#ifdef __MACH__
            pwrite(fd, buffer_page.content, frame_size, frame_size*frame_id);
#else
            char * init;
            posix_memalign((void**)&init, getpagesize(), frame_size) ;//;<< std::endl;
            memcpy(init, buffer_page.content, frame_size);
            pwrite64(fd, init, frame_size, frame_size*frame_id);
            free(init);
#endif
        }
    };
    
}



#endif
