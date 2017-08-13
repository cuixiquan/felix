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
//  Filename: Lock.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/12/13.
//

#ifndef Elementary_0_2_Lock_h
#define Elementary_0_2_Lock_h

#include <pthread.h>

class Lock{
    
public:
    
    Lock(){
    }
    
    inline int trywlock(){
        return pthread_mutex_trylock (&mutex);
    }
    
    inline void lock(){
        pthread_mutex_lock (&mutex);
    }
    
    inline void unlock(){
        pthread_mutex_unlock (&mutex);
    }
    
private:
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
};



#endif
