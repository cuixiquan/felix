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
//  Filename: FactorFactory_FG.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/16/13.
//

#ifndef Elementary0_3_FactorFactory_FG_h
#define Elementary0_3_FactorFactory_FG_h

#include "../fg/Factor.h"
#include "../fg/VariableGroup.h"
#include "../fg/Variable.h"

namespace hazy{
    
    Factor::FunctionGroup * FactorFactory_FG_FGS[1000];
    double *** FactorFactory_FG_MODELS;
    
    class FactorFactory_FG{
        
    public:
        
        static void init(){
        
            FactorFactory_FG_FGS[0] =   //LR Factor
                new Factor::FunctionGroup(_INIT_STATE, NULL, _LR_POTENTIAL, _INIT_GROUP,_INIT_VAR, _MAT,_UPDATE_VAR, _get_n_current_var, _get_n_oid);
            
            FactorFactory_FG_FGS[1] =   //CRF Factor
            new Factor::FunctionGroup(_INIT_STATE, NULL, _CRF_POTENTIAL, _INIT_GROUP,_INIT_VAR, _MAT,_UPDATE_VAR, _get_n_current_var, _get_n_oid);
            
            FactorFactory_FG_MODELS = new double**[2];
        
        }
        
        static Factor generate(OID _oid, int _function_id, int _nvars, int _pid, int _aux){
            
            return Factor(_oid,
                          sizeof(int) + _nvars*(sizeof(OID) + sizeof(double)+sizeof(int)),  // for each variable, we maintain an OID and a double
                          FactorFactory_FG_FGS[_function_id],
                          _nvars,
                          _pid,
                          _aux);
            
        }
        
        static void _INIT_STATE(Factor & factor){
            
            _set_n_current_var(factor, 0);
            
            for(int i=sizeof(int);i<factor.statesize;i+=sizeof(OID) + sizeof(double) + sizeof(int)){
                reinterpret_cast<OID&>(factor.state[i]) = 0;
                reinterpret_cast<double&>(factor.state[i+sizeof(OID)]) = 0;
                reinterpret_cast<double&>(factor.state[i+sizeof(OID)+sizeof(double)]) = 0;
            }
        }
        
        static void _INIT_GROUP(Factor & factor, int group_id, int _size, double _aux){}
        
        static void _INIT_VAR(Factor & factor, int group_id, const OID vid, const double value, const int aux){
            
            int current_nvar =_get_n_current_var(factor);
            
            _set_n_oid(factor, current_nvar, vid);
            _set_n_value(factor, current_nvar, value);
            _set_n_aux(factor, current_nvar, aux);

            _set_n_current_var(factor, current_nvar+1);
            
        }
        
        static void _UPDATE_VAR(Factor & factor, const Variable & v, const double & cvalue){
            int current_nvar =_get_n_current_var(factor); 
            for(int i=0;i<current_nvar;i++){
                if(v.oid == _get_n_oid(factor, i)){
                    _set_n_value(factor, i, cvalue);
                }
            }
        }
        
        static void _UPDATE_VAR_EAGERV(Factor & factor, const Variable & v, const double & cvalue, void * v2fids_eager){
            
            int current_nvar =_get_n_current_var(factor);
            for(int i=0;i<current_nvar;i++){
                if(v.oid == _get_n_oid(factor, i)){
                    _set_n_value(factor, i, cvalue);
                }
            }
            
        }
        
        static void _MAT(Factor & factor, AbstractKV<Assignment> * assignments_kv, const Variable & v){
          
            int current_nvar =_get_n_current_var(factor);
            OID oid;
            double cvalue;
            
            for(int i=0;i<current_nvar;i++){
                oid = _get_n_oid(factor, i);
                if(oid == v.oid){
                    _set_n_value(factor, i, v.cvalue);
                }else{
                    assignments_kv->apply_oid_speical_for_assignments(oid, cvalue);
                    _set_n_value(factor, i, cvalue);
                }
            }
             
        }
        

        static double _LR_POTENTIAL(Factor & factor, const Variable & v, const double & cvalue){
            
            int current_nvar =_get_n_current_var(factor);
            for(int i=0;i<current_nvar;i++){
                if(v.oid == _get_n_oid(factor, i)){
                    //std::cout << "VALUE=" << cvalue << ": POTENTIAL=" << cvalue * model[_get_n_aux(factor, i)] << std::endl;
                    return FactorFactory_FG_MODELS[0][factor.parameterID][(int)cvalue];
                }
            }
            assert(false);
            return 0;
            
        }
        
        static double _CRF_POTENTIAL(Factor & factor, const Variable & v, const double & cvalue){
            
            int current_nvar =_get_n_current_var(factor);
            double cv1 = _get_n_value(factor, 0);
            double cv2 = _get_n_value(factor, 1);
            
            for(int i=0;i<current_nvar;i++){
                if(v.oid == _get_n_oid(factor, i)){
                    if(i==0){
                        cv1 = cvalue;
                    }else{
                        cv2 = cvalue;
                    }
                }
            }
            return FactorFactory_FG_MODELS[1][int(cv1)][int(cv2)];            
        }
        
        static int _get_n_current_var(const Factor & factor){
            return reinterpret_cast<const int&>(factor.state[0]);
        }

        static OID _get_n_oid(const Factor & factor, int nv){
            return reinterpret_cast<const OID&>(factor.state[0+sizeof(int)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]);
        }
                
    private:
        
        
        static void _set_n_current_var(Factor & factor, int current_nvar){
            reinterpret_cast<int&>(factor.state[0]) = current_nvar;
        }
        
        
        static void _set_n_oid(Factor & factor, int nv, OID oid){
            reinterpret_cast<OID&>(factor.state[0+sizeof(int)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]) = oid;
        }
        
        static double _get_n_value(const Factor & factor, int nv){
            return reinterpret_cast<const double&>(factor.state[0+sizeof(int)+sizeof(OID)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]);
        }
        
        static void _set_n_value(Factor & factor, int nv, double value){
            reinterpret_cast<double&>(factor.state[0+sizeof(int)+sizeof(OID)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]) = value;
        }
        
        static int _get_n_aux(const Factor & factor, int nv){
            return reinterpret_cast<const double&>(factor.state[0+sizeof(int)+sizeof(OID)+sizeof(double)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]);
        }
        
        static void _set_n_aux(Factor & factor, int nv, int value){
            reinterpret_cast<double&>(factor.state[0+sizeof(int)+sizeof(OID)+sizeof(double)+nv*(sizeof(OID)+sizeof(double)+sizeof(int))]) = value;
        }
        
    };
    
    
    
}

#endif

