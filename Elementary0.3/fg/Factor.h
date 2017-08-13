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
//  Filename: Factor.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_Factor_h
#define Elementary0_3_Factor_h

#include "VariableGroup.h"
#include "Variable.h"

#include "../io2/AbstractKV.h"

namespace hazy{
    
    class Factor{
        
    public:
        
        class FunctionGroup{
            
        public:
            
            void (* const INIT_STATE)(Factor & factor);
            
            void (* const VG)(Factor & factor, VariableGroup & vg);
            
            double (* const POTENTIAL)(Factor & factor, const Variable & v, const double & cvalue);
            
            void (* const INIT_GROUP)(Factor & factor, int group_id, int _size, double _aux);
            
            void (* const INIT_VAR)(Factor & factor, int group_id, const OID vid, const double value, const int aux);
        
            void (* const MAT)(Factor & factor, AbstractKV<Assignment> * assignments_kv, const Variable & v);
            
            void (* const UPDATE_VAR)(Factor & factor, const Variable & v, const double & cvalue);

            int (* const GET_NVAR)(const Factor & factor);
            
            OID (* const GET_VAR_OID)(const Factor & factor, int nvar);
            
            FunctionGroup(void (*_INIT_STATE)(Factor & factor),
                          void (*_VG)(Factor & factor, VariableGroup & vg),
                          double (*_POTENTIAL)(Factor & factor, const Variable & v, const double & cvalue),
                          void (*_INIT_GROUP)(Factor & factor, int group_id, int _size, double _aux),
                          void (*_INIT_VAR)(Factor & factor, int group_id, const OID vid, const double value, const int aux),
                          void (*_MAT)(Factor & factor, AbstractKV<Assignment> * assignments_kv, const Variable & v),
                          void (*_UPDATE_VAR)(Factor & factor, const Variable & v, const double & cvalue),
                          int (*_GET_NVAR)(const Factor & factor),
                          OID (*_GET_VAR_OID)(const Factor & factor, int nvar))
            : INIT_STATE(_INIT_STATE),
              VG(_VG),
              POTENTIAL(_POTENTIAL),
              INIT_GROUP(_INIT_GROUP),
              INIT_VAR(_INIT_VAR),
              MAT(_MAT),
              UPDATE_VAR(_UPDATE_VAR),
              GET_NVAR(_GET_NVAR),
              GET_VAR_OID(_GET_VAR_OID)
            {}
            
            FunctionGroup()
            :   INIT_STATE(NULL),
                VG(NULL),
                POTENTIAL(NULL),
                INIT_GROUP(NULL),
                INIT_VAR(NULL),
                MAT(NULL),
                UPDATE_VAR(NULL),
                GET_NVAR(NULL),
                GET_VAR_OID(NULL){}
            
        };
        
        OID oid;
        
        int parameterID;
        
        int nvar;
        
        int ngroup;

        int statesize;
        
        int faux;
        
        FunctionGroup * functions;
                
        char state[1];
        
        void copy(Factor & to) const{
            
            to.oid = oid;
            to.functions = functions;
            to.statesize = statesize;
            
            to.parameterID = parameterID;
            to.faux = faux;
            to.nvar = nvar;
            to.ngroup = ngroup;
            
            for(int i=0;i<to.statesize;i++){
                to.state[i] = 0;
            }
        }
        
        size_t size() const {
            return sizeof(Factor) + statesize*sizeof(char) ;
        }
        
        Factor(   const OID & _oid, int _statesize,
                  FunctionGroup * _functions,
                  const int _nvar,
                  const int _pid,
                  const int _faux,
                  int _ngroup = -1)
        {
            functions = _functions;
            oid = _oid;
            nvar = _nvar;
            statesize = _statesize;
            parameterID = _pid;
            faux = _faux;
            ngroup = _ngroup;
        }
        
    };

}


#endif





