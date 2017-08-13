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
//  Filename: Timer.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/12/13.
//

#ifndef Elementary_0_2_Timer_h
#define Elementary_0_2_Timer_h


#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#ifdef __MACH__
#include <sys/time.h>
//clock_gettime is not implemented on OSX
int clock_gettime(int /*clk_id*/, struct timespec* t) {
    struct timeval now;
    int rv = gettimeofday(&now, NULL);
    if (rv) return rv;
    t->tv_sec  = now.tv_sec;
    t->tv_nsec = now.tv_usec * 1000;
    return 0;
}

#define CLOCK_MONOTONIC 0
#endif


#include <time.h>

namespace hazy{
            
            class Timer {
            public:
                
                struct timespec _start;
                struct timespec _end;
                
                Timer(){
                    clock_gettime(CLOCK_MONOTONIC, &_start);
                }
                
                virtual ~Timer(){}
                
                inline void restart(){
                    clock_gettime(CLOCK_MONOTONIC, &_start);
                }
                
                inline float elapsed(){
                    clock_gettime(CLOCK_MONOTONIC, &_end);
                    return (_end.tv_sec - _start.tv_sec) + (_end.tv_nsec - _start.tv_nsec) / 1000000000.0;
                }
                
                
            };
            
}



#endif
