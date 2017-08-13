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
//  Filename: RWLock.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/22/13.
//

#ifndef Elementary_0_2_RWLock_h
#define Elementary_0_2_RWLock_h

#include <pthread.h>

long TOTAL_RLOCK = 0;
long TOTAL_WLOCK = 0;

class RWLock{
    
public:
    
    pthread_rwlock_t mutex;
    
    RWLock(){
        mutex = PTHREAD_RWLOCK_INITIALIZER;
    }
    
    void rlock(){
        TOTAL_RLOCK++;
        pthread_rwlock_rdlock (&mutex);
    }
    
    void wlock(){
        TOTAL_WLOCK++;
        
        pthread_rwlock_wrlock (&mutex);
    }
    
    int trywlock(){
        return pthread_rwlock_trywrlock(&mutex);
    }
    
    void unlock(){
        pthread_rwlock_unlock(&mutex);
    }
    
private:
    
};


#endif
