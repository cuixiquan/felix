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
//  Filename: Direct_MM.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_Direct_MM_h
#define Elementary0_3_Direct_MM_h

#include <vector>

#include "Page.h"

namespace hazy{

    class DirectIO_MM{
  
    public:
    
        static std::string name(){return "MM";}
        
        const size_t frame_size;
    
        std::vector<char*> frames;
    
        DirectIO_MM(size_t pagesize) : frame_size(pagesize) {}

        /**
         * Not thread-safe!
         **/
        void load(size_t frame_id){
            for(size_t i=frames.size();i<=frame_id;i++){
                frames.push_back(NULL);
            }
            frames[frame_id] = new char[frame_size];
        }
    
        void get(BufferPageHeader & buffer_page, size_t frame_id){
            buffer_page.content = frames[frame_id];
        }
    
        void put(const BufferPageHeader & buffer_page, size_t frame_id){
        }
    };

}
    
#endif





