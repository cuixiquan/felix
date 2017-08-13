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
//  Filename: Init.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/16/13.
//

#ifndef Elementary0_3_Init_h
#define Elementary0_3_Init_h

#include "../fg/Factor.h"
#include "FactorFactory_LDA.h"
#include "FactorFactory_FG.h"


/*
 FunctionGroup(void (*_INIT_STATE)(const Factor & factor),
 void (*_VG)(const Factor & factor, VariableGroup & vg),
 double (*_POTENTIAL)(const Factor & factor, const Variable & v, const double & cvalue),
 void (*_INIT_GROUP)(Factor & factor, int group_id, double _aux),
 void (*_INIT_VAR)(Factor & factor, int group_id, const OID vid, const double value, const int aux),
 void (*_MAT)(Factor & factor, void * assignments_kv),
 void (*_UPDATE_VAR)(Factor & factor, const Variable & v, const double & cvalue)){
 */

namespace hazy{

    Factor::FunctionGroup FactorFactory_LDA::_factorgroup
                                                = Factor::FunctionGroup(
                                                    FactorFactory_LDA::_INIT_STATE,
                                                    NULL,
                                                    FactorFactory_LDA::_POTENTIAL,
                                                    FactorFactory_LDA::_INIT_GROUP,
                                                    FactorFactory_LDA::_INIT_VAR,
                                                    NULL,
                                                    FactorFactory_LDA::_UPDATE_VAR,
                                                    FactorFactory_LDA::_get_n_current_var,
                                                    FactorFactory_LDA::_get_n_oid
                                                );
    
    Factor::FunctionGroup FactorFactory_LDA::_factorgroup_inverse
                                                = Factor::FunctionGroup(
                                                    FactorFactory_LDA::_INIT_STATE,
                                                    NULL,
                                                    FactorFactory_LDA::_POTENTIAL_INVERSE,
                                                    FactorFactory_LDA::_INIT_GROUP,
                                                    FactorFactory_LDA::_INIT_VAR,
                                                    NULL,
                                                    FactorFactory_LDA::_UPDATE_VAR,
                                                    FactorFactory_LDA::_get_n_current_var,
                                                    FactorFactory_LDA::_get_n_oid
                                                    );
    
}

#endif



