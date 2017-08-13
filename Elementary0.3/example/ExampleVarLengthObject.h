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
//  Filename: ExampleVarLengthObject.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_ExampleVarLengthObject_h
#define Elementary0_3_ExampleVarLengthObject_h

namespace hazy{

    /**
     * This class is an example about how to create a variable-length object
     * that can be put in VarLenStore. VarLenStore will assume all objects implement
     * copy() and size() function.
     **/
    class ExampleVarLenObject{
        
    public:
        
        int id; /*< You can put whatever fix-length objects before the variable-length part.*/
        
        int ndouble; /*< If you want, you can save the length of the variable-length part,
                      But this is not required as long as you are happy with it. That is, VarLenStore
                      will not use this variable.*/
        
        double aaa[]; /*< The variable-length part is in the form of an array.*/
        
        /**
         * We need a copy function to copy the current object to another object.
         * This copy function should not copy the var-len part, instead it should init them.
         */
        void copy(ExampleVarLenObject & to) const{
            to.id = id;
            to.ndouble = ndouble;
            for(int i=0;i<to.ndouble;i++){
                to.aaa[0] = 0;
            }
        }
        
        /**
         * We need a function to tell us the size of this object. The size is the size of
         * fix-length part and variable-length part.
         **/
        size_t size() const{
            return sizeof(ExampleVarLenObject) + ndouble*sizeof(double);
        }
        
        /**
         * Constructor of the fix-length part. The variable-length part will be intilized
         * by copy(), which will be called everytime you put an object into VarLenStore.
         **/
        ExampleVarLenObject(){
            ndouble = 0;
        }
        
        /**
         * We don't allow this object to be copied by other worker other than VarLenStore.
         **/
        ExampleVarLenObject(const ExampleVarLenObject &source)
        {
            assert(false);
        }
        
        /**
         * We don't allow this object to be assigned by other worker other than VarLenStore.
         **/
        ExampleVarLenObject & operator=(const ExampleVarLenObject & source){
            assert(false);
        }
        
    };

    
    
}

#endif
