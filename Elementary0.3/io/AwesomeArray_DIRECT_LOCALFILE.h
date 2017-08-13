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
//  Filename: AwesomeArray_DIRECT_LOCALFILE.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/19/13.
//

#ifndef Elementary0_3_AwesomeArray_DIRECT_LOCALFILE_h
#define Elementary0_3_AwesomeArray_DIRECT_LOCALFILE_h

#include "../util/Common.h"
#include "AwesomeArray.h"
namespace hazy{

    /**
     * A specification of AwesomeArray that stores in local files, without any buffer
     * and pager.
     *
     * See AwesomeArray for details.
     */
    template<class VALUE>
    class AwesomeArray<VALUE, LOCALFILE, DIRECT, NONE, FIXLEN>{

    private:

        Lock lock; /**< Mutex while expanding content by expand(). */

        VALUE INITVALUE; /**< Default value for an un-loaded key.*/

        const bool hasInitValue; /**< Whether INITVALUE is provided.*/

        Long nio; /**< Count for number of IOs.*/

        Long size; /**< Current size of the content array.*/

        OID maxid; /**< Max key ever seen. */

        std::string filename;

        int fd;

        int objsize;

        void do_init_job(){

            filename = getNextTmpFileName();
            filename = filename + "_" + name;

            if(!REUSEMODE){
                remove(filename.c_str());
            }

#ifdef __MACH__
            fd = open(filename.c_str(), O_RDWR | O_CREAT ,
                      (mode_t) 0600);
#else
            fd = open64(filename.c_str(), O_RDWR | O_CREAT, (mode_t) 0600);
#endif

            assert(fd != -1);

            if(!REUSEMODE){
                objsize = sizeof(VALUE);

                for(int i=0;i<INIT_ARRAY_SIZE;i++){

#ifdef __MACH__
                    pwrite(fd, &INITVALUE, objsize, objsize*i);
#else
                    VALUE * init;

                    int rr = posix_memalign((void**)&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl;

                    assert(rr == 0);

                    *init = INITVALUE;

                    rr = pwrite64(fd, init, objsize, objsize*i) ;//<< std::endl;

                    assert(rr > 0);
                    free(init);
#endif

                }
            }

        }

    public:

        std::string name; /**< Name of this AwesomeArray. **/

        Long get_maxid(){
            return maxid;
        }

        /**
         * See AwesomeArray::AwesomeArray()
         */
        AwesomeArray(std::string _name="") :
        size(INIT_ARRAY_SIZE), hasInitValue(false){
            name = _name;

            do_init_job();

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
            name = _name;

            do_init_job();

            maxid = -1;
            nio = 0;
        }

        void load (const OID & key, const VALUE & value, RID & rid){

            if(key >= maxid){
                maxid = key;
            }

            if (key >= size) {
                size_t newsize = size;
                while (key >= newsize) {
                    newsize *= 2;
                }

                for(size_t i=size; i<newsize; i++){
#ifdef __MACH__
                    pwrite(fd, &INITVALUE, objsize, objsize*i);
#else
                    VALUE * init;
                    //*init = INITVALUE;
                    //std::cout << "###" <<
                    posix_memalign((void**)&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl;
                    free(init);
#endif
                }

                size = newsize;

            }
#ifdef __MACH__
            pwrite(fd, &value, objsize, objsize*key);
#else


            VALUE * init;
            //std::cout << "###" <<
            posix_memalign((void**)&init, getpagesize(), sizeof(VALUE)) ;//<< std::endl;
            *init = value;
            pwrite64(fd, init, objsize, objsize*key);
            free(init);
#endif

            // for LOCALFILE, RID=OID
            rid.first = 0;
            rid.second = key;

        }

        void to_rid (const OID & oid, RID & rid){
            rid.first = 0;
            rid.second = oid;
        }

        void get_oid (const OID & key, VALUE & value){
            #ifdef __MACH__
                pread(fd, &value, objsize, objsize*key);
            #else
                VALUE * init;
                posix_memalign((void**)&init, getpagesize(), sizeof(VALUE)) ;// << std::endl;
                pread64(fd, init, objsize, objsize*key);
                value = *init;
                free(init);
            #endif

             nio ++;
        }

        void get_rid (const RID & rid, VALUE & value){
            get_oid(rid.second, value);
        }

        void set_oid (const OID & key, const VALUE & value){
            #ifdef __MACH__
                pwrite(fd, &value, objsize, objsize*key);
            #else
                VALUE * init;
                posix_memalign((void**)&init, getpagesize(), sizeof(VALUE)) ;//;<< std::endl;
                *init = value;
                pwrite64(fd, init, objsize, objsize*key);
                free(init);
            #endif

            nio ++;
        }

        void set_rid (const RID & rid, const VALUE & value){
            set_oid(rid.second, value);
        }

        template<class FUNCTION, class... AUXS>
        bool apply_oid (const OID & key, const FUNCTION & func, AUXS & ... aux){
            VALUE v;
            this->get_oid(key, v);
            func(v, aux...);
            this->set_oid(key, v);
        }

        template<class FUNCTION, class... AUXS>
        bool apply_rid (const RID & rid, const FUNCTION & func, AUXS & ... aux){
            VALUE v;
            this->get_rid(rid, v);
            func(v, aux...);
            this->set_rid(rid, v);
        }

        template<class FUNCTION, class AUX1, class AUX2>
        void batch_apply(const int nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2){

            // This function is not useful... Use a Paged version would be better!
            assert(false);
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
