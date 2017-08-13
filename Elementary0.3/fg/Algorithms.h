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
//  Filename: Algorithms.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/16/13.
//

#ifndef Elementary0_3_Algorithms_h
#define Elementary0_3_Algorithms_h

#include "Factor.h"

namespace hazy{
    
    VariableFactorsRecord _v2frecord_object_pool;
    
    VariableFactorIDsRecord v2fids_record_not_thread_safe;  // a simple object-pool
    
    Assignment assignment_not_thread_safe;  // a simple object-pool
    
    RID dummyrid;   // a simple object-pool
       
    void _push_factor_to_eager_record2(VariableFactorsRecord & v2frecord, Factor & factor, FullID & fullID){
        v2frecord.push(fullID, factor);
    }
    
    template<class EAGERKV>
    void _push_factor_to_eager_record(Factor & factor, RID & eager_rid, FullID & fullID, EAGERKV & _v2fids_eager){
        
        _v2fids_eager.apply_rid(eager_rid, _push_factor_to_eager_record2, factor, fullID);
        
    }
    
    template<class FACTORS, class EAGERKV>
    void _mat_v2frecord(VariableFactorIDsRecord & lazy_record, FACTORS & _factors, EAGERKV & _v2fids_eager){
        _v2frecord_object_pool.nfactor = lazy_record.nfactor;
        _v2frecord_object_pool.statesize = lazy_record.mat_factor_size;
        _v2frecord_object_pool.current_factor = 0;
        _v2frecord_object_pool.current_factor_offset = sizeof(int)*_v2frecord_object_pool.nfactor;
        _v2frecord_object_pool.variable = lazy_record.variable;
        
        //std::cout << lazy_record.size() << "  -->  " << _v2frecord_object_pool.size() << std::endl;
        
        _v2fids_eager.load(lazy_record.variable.oid, _v2frecord_object_pool, dummyrid);
        
        for(int i=0;i<lazy_record.nfactor;i++){
            
            //std::cout << lazy_record.variable.oid << "    f=" << lazy_record.seconds[i].oid << std::endl;
            
            _factors.apply_rid(lazy_record.seconds[i].rid, _push_factor_to_eager_record<EAGERKV>, dummyrid, lazy_record.seconds[i], _v2fids_eager);
        }
    }
    
    template<class TUPLECLASS>
    void _fill_in_variable_status(TUPLECLASS & vrecord, VariableStatus & status){
            	  status.vid = vrecord.variable.oid;
            	  status.last_sample_cvalue = vrecord.variable.cvalue;
            	  status.expectation = 1.0*vrecord.variable.exp_sum/vrecord.variable.exp_count;

            	  status.vtype = vrecord.variable.vtype;

            	  status.standard_derivation = sqrt((1.0*
            	         status.expectation*status.expectation*vrecord.variable.exp_count
            	         -2*status.expectation*vrecord.variable.exp_sum
            	         +vrecord.variable.exp_sum_v2
            	         )/vrecord.variable.exp_count);
    }

    inline void _add_group_to_factor(Factor & factor, const int group_id, const int _size, const double aux){
        factor.functions->INIT_GROUP(factor, group_id, _size, aux);
    }
    
    inline void _init_var(Factor & factor, const OID & v_oid, const double & value, const int & group_id, const int & aux, VariableFactorIDsRecord & v2f_record){
        factor.functions->INIT_VAR(factor, group_id, v_oid, value, aux);
        v2f_record.mat_factor_size += sizeof(FullID) + factor.size() + sizeof(int);
    }
    
    template<class FACTORS>
    inline void _add_variable_to_factor(VariableFactorIDsRecord & v2f_record, const OID & f_oid, const int aux, bool is_vg, const int & group_id, FACTORS & _factors){
        FullID full_fid;
        full_fid.oid = f_oid;
        
        _factors.to_rid(f_oid, full_fid.rid);
        
        v2f_record.push(full_fid);
        
        if(is_vg){
            v2f_record.variable.vgfid = full_fid;
        }
        
        _factors.apply_rid(full_fid.rid, _init_var, v2f_record.variable.oid, v2f_record.variable.cvalue, group_id, aux, v2f_record);
    }
    
    
    inline bool _calculate_potential(Factor & factor,
                                          const NumericalSystem & numSystem,
                                          const Variable & v,
                                          const double & cvalue,
                                          double & potential){
        
        if(numSystem == LINEAR){
            potential *= factor.functions->POTENTIAL(factor, v, cvalue);
        }else{
            potential += factor.functions->POTENTIAL(factor, v, cvalue);
        }

        return true;
        
    }
    
    template<class ASSIGNMENTS>
    inline bool _mat_factor(Factor & factor,
                            ASSIGNMENTS & assignments, const Variable & v){
        
        factor.functions->MAT(factor, &assignments, v);
        return true;
    }
    
    inline bool _evaluate_vg_function(Factor & factor, VariableGroup & vgroup){
        
        factor.functions->VG(factor, vgroup);
        return true;
    }
    
    
    inline bool _update_factor_state(Factor & factor, Variable & v, const double & cvalue){
        factor.functions->UPDATE_VAR(factor, v, cvalue);
        return true;
    }
        
    
    template<class FACTORS>
    inline void _update_variable_assignment_in_factor_lazy(VariableFactorIDsRecord & vrecord, const FMatType & FMAT, const double & cvalue, FACTORS & _factors, int& changed){

        if(FMAT == hazy::F_EAGER){

                for(int i=0;i<vrecord.nfactor;i++){
                    _factors.apply_rid(vrecord.seconds[i].rid, _update_factor_state, vrecord.variable, cvalue);
                }

        }
        
        if(vrecord.variable.cvalue != cvalue){
   	            changed ++;
   	    }

        vrecord.variable.cvalue = cvalue;
        vrecord.variable.exp_sum += cvalue;
        vrecord.variable.exp_sum_v2 += cvalue*cvalue;
        vrecord.variable.exp_count ++;

    }
    
    
    inline void _update_variable_assignment_in_factor_eager_other_factor(VariableFactorsRecord & vrecord, const double & cvalue){
        for(int i=0;i<vrecord.nfactor;i++){
            Factor & f = vrecord.get_n_factor_content(i);
            f.functions->UPDATE_VAR(f, vrecord.variable, cvalue);
        }
    }
    
    template<class FACTORS, class V2FEAGER>
    inline void _update_variable_assignment_in_factor_eager(VariableFactorsRecord & vrecord, const FMatType & FMAT, const double & cvalue, FACTORS & _factors, int& changed, V2FEAGER & v2fids_eager){
        
        if(FMAT == hazy::F_EAGER){
            
            for(int i=0;i<vrecord.nfactor;i++){
                //_update_factor_state(vrecord.get_n_factor_content(i), vrecord.variable, cvalue);
                
                Factor & f = vrecord.get_n_factor_content(i);
                f.functions->UPDATE_VAR(f, vrecord.variable, cvalue);
                
                OID oidv;
                int nvar = f.functions->GET_NVAR(f);
                
                for(int i=0;i<nvar;i++){
                    oidv = f.functions->GET_VAR_OID(f, i);
                    if(oidv != vrecord.variable.oid){
                        v2fids_eager.apply_oid(oidv, _update_variable_assignment_in_factor_eager_other_factor, cvalue);
                    }
                }
                
            }
            
        }
        
        if(vrecord.variable.cvalue != cvalue){
            changed ++;
        }
        
        vrecord.variable.cvalue = cvalue;
        vrecord.variable.exp_sum += cvalue;
        vrecord.variable.exp_sum_v2 += cvalue*cvalue;
        vrecord.variable.exp_count ++;
    }

        
    inline bool _update_variable_assignment(Assignment & assign, const double & cvalue){
        assign.cvalue = cvalue;
        return true;
    }
    
    
    
    template<class TUPLECLASS, class FACTORGRAPHPOINTER>
    inline bool sample(TUPLECLASS & vrecord, const NumericalSystem & numSystem, FACTORGRAPHPOINTER & fg, int & changed){
        
        // inference result
        VariableGroup vgroup;
        vgroup.nvar = 1;
        vgroup.hypothesis[0].vid = vrecord.variable.oid;

        if(vrecord.variable.vtype == EVID){
            vgroup.hypothesis[0].cvalue = vrecord.variable.cvalue;
            // if there is a VG function associated with this variable
        }else if(vrecord.variable.vgfid.oid != -1){
            fg.prepareFactors(vrecord);
            fg.evaluateVGFactor(vrecord, vgroup);
        }else if(vrecord.variable.dtype == REAL or vrecord.variable.ubound > 100){

            fg.prepareFactors(vrecord);
            
            // Metropolis-Hastling
            
            double very_ori_value = vrecord.variable.cvalue;
            double ori_value = vrecord.variable.cvalue;
            double old_potential = fg.calcPotential(vrecord, ori_value);
            
            for(int i=0;i<MH_STEP_PER_GIBBS_STEP;i++){
                double new_value;
                if(vrecord.variable.dtype == REAL){
                    new_value = fRand() * (vrecord.variable.ubound - vrecord.variable.lbound) + vrecord.variable.lbound;
                }else{
                    new_value = int(fRand() * (vrecord.variable.ubound - vrecord.variable.lbound + 1)) + vrecord.variable.lbound;
                }
                
                double random = fRand();
                
                double new_potential = fg.calcPotential(vrecord, new_value);
                
                bool accept = false;
                if(numSystem == LINEAR){
                    accept = (random < new_potential/old_potential);
                }else{
                    accept = (random < exp(new_potential - old_potential));
                }
                
                if(accept){
                    old_potential = new_potential;
                    ori_value = new_value;
                    vrecord.variable.cvalue = new_value;
                    vgroup.hypothesis[0].cvalue = new_value;
                }else{
                    vgroup.hypothesis[0].cvalue = ori_value;
                }
            }
            vrecord.variable.cvalue = very_ori_value;
            
        }else{
            
            fg.prepareFactors(vrecord);
            
            double potential[100];  //TODO: This is slow, make a better object pool
            
            for(int propose=vrecord.variable.lbound; propose<=vrecord.variable.ubound; propose++){
                potential[propose] = fg.calcPotential(vrecord, propose);
            }
            
            double random = fRand();
            
            double pfunc;
            
            if(numSystem == LOG){
                pfunc = -10000000;
            }else{
                pfunc = 0;
            }
            
            for(int value=vrecord.variable.lbound; value <= vrecord.variable.ubound; value ++){
                
                if(numSystem == LOG){
                    pfunc = logadd(potential[value], pfunc);
                }else{
                    pfunc += potential[value];
                }
            }
            
            double accum = 0;
            double pmeta = 0;
            int targetValue = -1;
            for(int value=vrecord.variable.lbound; value <= vrecord.variable.ubound; value ++){
                
                if(numSystem == LOG){
                    pmeta = exp( potential[value] - pfunc );
                }else{
                    pmeta = potential[value]/pfunc;
                }
                
                accum += pmeta;
                
                if(random < accum){
                    targetValue = value;
                    break;
                }
                
            }
            if(targetValue == -1){
            	cout << " failed for " << vrecord.variable.oid << " lb= " << vrecord.variable.lbound << " ub= " << vrecord.variable.ubound << endl;
            }
            assert(targetValue != -1);
            
            vgroup.hypothesis[0].cvalue = targetValue;
        }
        
        fg.updateVariableGroupAssignments(vrecord, vgroup, changed);
        
        return true;
    }
    
    
    
}


#endif
