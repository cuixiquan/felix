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
//  Filename: AwesomeArray.h
//  Project:  Elementary_0.2
//
//  Created by Ce Zhang on 1/12/13.
//

#ifndef Elementary_0_2_AwesomeArray_h
#define Elementary_0_2_AwesomeArray_h

#include "../util/Common.h"

/**
 * \brief Hazy Research [http://hazy.cs.wisc.edu]
 * University of Wisconsin-Madison [http://www.wisc.edu]
 */
namespace hazy{
    
    
    
    /**
     * An AwesomeArray is an array that is awesome. AwesomeArray has the following features:
     *   <ul> 
     *       <li> <b>Hetergenous Storage:</b> Data can be stored in different storages, 
     *                                    e.g., Main memory, file, HBase, Accumulo etc.  </li>
     *       <li> <b>Buffer and Paging:</b> For those AwesomeArray stored in secondary backends,
     *                                    different buffers are supported, including LRU, MRU, RANDOM,
     *                                    and OPTIMAL. Also, objects can be oganized into pages
     *                                    to minimize I/O operations. </li>
     *       <li> <b>Fast:</b> Operations on an AwesomeArray in Main memory is almost as fast as 
     *                                    on an in-memory array. </li>
     *       <li> <b>Thread Safe</b> We support concurrent visits with different locking
     *                                    protocols.
     *   </ul>
     *
     * One needs to keep in mind that, to make things faster, AwesomeArray does not do range checking
     * for get() and set(). It is the developers' responsitiblity to make sure these entries are load()
     * before using them.
     * <br/>
     *
     * Different kinds of AwesomeArray is specified by using template for specification. These
     * specifications are implemented in files AwesomeArray_*.h
     * <br/>
     *
     * This interface shows those methods that are shared by all AwesomeArray's. Different
     * AwesomeArray make have more methods that can be used (mainly by other AwesomeArray).
     *
     * \tparam VALUE Object type stored in this AwesomeArray.
     * \tparam STORAGESTYPE StorageBackend: {MM, FILE, HBASE, ACCUMULO}.
     * \tparam BUFFERTYPE Type of Buffer: {BUFFER, PAGEDBUFFER, DIRECT}.
     * \tparam RANDOM Type of Pager if necessary: {LRU, MRU, RANDOM, OPTIMAL, NONE}. NONE can be used only if BUFFERTYPE is DIRECT.
     *
     */
    template<class VALUE, StorageType STORAGESTYPE, LayoutType LAYOUTTYPE, BufferType BUFFERTYPE, ObjectType OBJECTYPE>
    class AwesomeArray{
        
    public:
        
         void        load       (const OID & oid, const VALUE & value, RID & rid) ;
        
         void        get_oid    (const OID & oid, VALUE & value) ;
        
         void        get_rid    (const RID & rid, VALUE & value) ;
        
         void        set_oid    (const OID & oid, const VALUE & value) ;
        
         void        set_rid    (const RID & rid, const VALUE & value) ;
    
         template<class FUNCTION, class... AUXS>
         bool        apply_oid  (const OID & oid, const FUNCTION & func, AUXS & ... aux);

         template<class FUNCTION, class... AUXS>
         bool        apply_rid  (const RID & rid, const FUNCTION & func, AUXS & ... aux);
       
         /**
          * <b>Note: You may wonder why there are AUX1/AUX2 instead of AUXS... similar to apply_oid and apply_rid.
          * This is unfortunately caused by a bug in GCC to expand variadic template in Lambda. The AUXS... version
          * works well for LLVM (in Mac). We may want to change it back to the more general version after GCC
          * fixes the bug.</b>
          **/
         template<class FUNCTION, class AUX1, class AUX2>
         void        batch_apply(const int nthread, const FUNCTION & func, AUX1 & aux1, AUX2 & aux2);
        
         void        to_rid    (const OID & oid, RID & rid);
        
         Long        get_nio    () ;
        
         std::string get_name   () ;
        
         Long        get_maxid  () ;
        
         void        sigmod_organizePager () ;
        
    };
    
}

#endif
