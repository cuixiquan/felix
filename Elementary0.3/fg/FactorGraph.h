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
//  Filename: FactorGraph.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_FactorGraph_h
#define Elementary0_3_FactorGraph_h

#include "VariableRecord.h"
#include "VariableRecordEager.h"

#include "Variable.h"
#include "Factor.h"
#include "../util/Include.h"

#include "Algorithms.h"

namespace hazy{
    
    
    class AbstractFactorGraph{
    public:
        
        virtual void addFactor(const Factor & _f) = 0;
        
        virtual void addVariable(const Variable & _v, int nfactor) = 0;
        
        virtual void addGroup(const OID & f_oid, const int group_id, const Long size, const double aux) = 0;
        
        virtual void addEdge(const OID & v_oid, const OID & f_oid, const int group_id, const int aux, bool is_vg) = 0;
                
        virtual void mat() = 0;
        
        virtual int inference(int nthread) = 0;

        virtual void getVariableStatus(OID vid, VariableStatus & status) = 0;
        
        virtual void print_io() = 0;

        virtual Long get_maxid() = 0;

    };
    
    
    
    template<class STORAGE, class PAGER, VMatType VMAT>
    class FactorGraph : public AbstractFactorGraph{};
    
    template<class STORAGE, class PAGER>
    class FactorGraph<STORAGE, PAGER, V_EAGER> : public AbstractFactorGraph{
    
    public:

        NumericalSystem numSystem;
        
        FMatType FMAT;
        
        VarLenStore<VariableFactorIDsRecord, STORAGE, PAGER> v2fids;
        
        VarLenStore<VariableFactorsRecord, STORAGE, PAGER> v2fids_eager;
        
        VarLenStore<Factor, STORAGE, PAGER> factors;
        
        FixLenStore<Assignment, STORAGE, PAGER> assignments;
        
        FactorGraph(NumericalSystem _numSystem, FMatType _fmat): v2fids(VarLenStore<VariableFactorIDsRecord, STORAGE, PAGER>(SEQ_PAGESIZE, SEQ_BUFFERSIZE)),
                        v2fids_eager(VarLenStore<VariableFactorsRecord, STORAGE, PAGER>(SEQ_PAGESIZE, SEQ_BUFFERSIZE))
        {
            numSystem = _numSystem;
            FMAT = _fmat;
            
            std::cout << ">> Creating Factor Graph:" << std::endl;
            std::cout << "  | V_EAGER, " << (FMAT == F_LAZY ? "F_LAZY" : "F_EAGER") << ", " << (numSystem == hazy::LOG ? "LOG" : "LINEAR") << ", " << STORAGE::name() << ", " << PAGER::name() << std::endl;
            
        }
        
        
        /////////// FUNCTION GROUP 1: INIT A FACTOR GRAPH ////////////
        
        void addFactor(const Factor & _f){
            
            factors.load(_f.oid, _f, dummyrid); // add factor to KV store
            factors.apply_rid(dummyrid, _f.functions->INIT_STATE); // init factor in KV store
            
        }
        
        void addVariable(const Variable & _v, int nfactor){
            
            v2fids_record_not_thread_safe.nfactor = nfactor;
            v2fids_record_not_thread_safe.current = 0;
            v2fids_record_not_thread_safe.variable = _v;
            v2fids.load(_v.oid, v2fids_record_not_thread_safe, dummyrid);
            
            assignment_not_thread_safe.oid = _v.oid;
            assignment_not_thread_safe.cvalue = _v.cvalue;
            assignments.load(_v.oid, assignment_not_thread_safe, dummyrid);
            
        }
        
        void addGroup(const OID & f_oid, const int group_id, const Long size, const double aux){
            factors.apply_oid(f_oid, _add_group_to_factor, group_id, size, aux);
        }
        
        void addEdge(const OID & v_oid, const OID & f_oid, const int group_id, const int aux, bool is_vg){
            
            v2fids.apply_oid(v_oid, _add_variable_to_factor<decltype(factors)>, f_oid, aux, is_vg, group_id, factors);
            
        }
        
        //////////////////////////////////////////////////////////////
        
        ////////////////// FUNCTION GROUP 2: SAMPLE //////////////////
        
        
        void mat(){
            v2fids.batch_apply(1, _mat_v2frecord<decltype(factors), decltype(v2fids_eager)>, factors, v2fids_eager);
        }

        
        int inference(int nthread){
            
            int changed = 0;
            
            v2fids_eager.batch_apply(nthread, sample<VariableFactorsRecord, decltype(*this)>, numSystem, *this, changed);
                
            factors.sigmod_organizePager();
            assignments.sigmod_organizePager();
            
            SIGMOD_START_LOG_IO = true; // for OPT caching, we start from the second iteration.
            
            return changed;
            
        }
        
        /**
         * Lazily materialize factors if necessary.
         **/
        void prepareFactors(VariableFactorsRecord & vrecord){
            
            if(FMAT == hazy::F_LAZY){
                for(int i=0;i<vrecord.nfactor;i++){
                    _mat_factor<decltype(assignments)>(vrecord.get_n_factor_content(i), assignments, vrecord.variable);
                }
            }
            
        }
        
        /**
         * Calculate the potential under the hypothesis to turn vrecord'value to cvalue.
         **/
        double calcPotential(VariableFactorsRecord & vrecord, const double & cvalue){
            
            double potential;
            if(numSystem == LINEAR){
                potential = 1;
            }else{
                potential = 0;
            }
            
                            
            for(int i=0;i<vrecord.nfactor;i++){

                _calculate_potential(vrecord.get_n_factor_content(i), numSystem, vrecord.variable, cvalue, potential);
            }
                
            return potential;
        }
        
        /**
         * Call VG function of factors.
         **/
        void evaluateVGFactor(VariableFactorsRecord & vrecord, VariableGroup & vgroup){
            
            if(vrecord.variable.vgfid.oid != -1){

                for(int i=0;i<vrecord.nfactor;i++){
                    if(vrecord.variable.vgfid.oid == vrecord.get_n_factor_fullid(i).oid){
                        _evaluate_vg_function(vrecord.get_n_factor_content(i), vgroup);
                        break;
                    }
                }
            }else{
                assert(false);
            }
            
        }
        
        /**
         * Given a variable group, update the value of it
         **/
        void updateVariableGroupAssignments(VariableFactorsRecord & vrecord, VariableGroup & vgroup, int & changed){
            
            for(int i=0;i<vgroup.nvar;i++){
                
                assignments.apply_oid(vgroup.hypothesis[i].vid, _update_variable_assignment, vgroup.hypothesis[i].cvalue);
                
                //if(FMAT==F_EAGER){
                v2fids_eager.apply_oid(vgroup.hypothesis[i].vid, _update_variable_assignment_in_factor_eager<decltype(factors), decltype(v2fids_eager)>, FMAT, vgroup.hypothesis[i].cvalue, factors, changed, v2fids_eager);
                //}
                
            }
            
        }
        
        //////////////////////////////////////////////////////////////

        ////////////////// FUNCTION GROUP 3: REPORT //////////////////
        void getVariableStatus(OID vid, VariableStatus & status){
        	v2fids_eager.apply_oid(vid, _fill_in_variable_status<VariableFactorsRecord>, status);
        }

        Long get_maxid()
        {
			return assignments.get_maxid();
		}

        void print_io(){
            size_t total_io = 0;
            
            std::cout << "######## IO ########" << std::endl;
            std::cout << "V2FIDS: " << v2fids.get_nio() << std::endl;
            std::cout << "V2FIDS_EAGER: " << v2fids_eager.get_nio() << std::endl;
            std::cout << "FACTOR: " << factors.get_nio() << std::endl;
            std::cout << "ASSIGN: " << assignments.get_nio() << std::endl;
            std::cout << "####################" << std::endl;
            
            total_io += v2fids_eager.get_nio();
            total_io += v2fids.get_nio();
            total_io += factors.get_nio();
            total_io += assignments.get_nio();
        }
        
        //////////////////////////////////////////////////////////////
        
    };
    
    
    template<class STORAGE, class PAGER>
    class FactorGraph<STORAGE, PAGER, V_LAZY> : public AbstractFactorGraph{
        
    public:
        
        NumericalSystem numSystem;
        
        FMatType FMAT;
        
        VarLenStore<VariableFactorIDsRecord, STORAGE, PAGER> v2fids;
        
        VarLenStore<Factor, STORAGE, PAGER> factors;
        
        FixLenStore<Assignment, STORAGE, PAGER> assignments;
        
        FactorGraph(NumericalSystem _numSystem, FMatType _fmat): v2fids(VarLenStore<VariableFactorIDsRecord, STORAGE, PAGER>(SEQ_PAGESIZE, SEQ_BUFFERSIZE))
        {
            numSystem = _numSystem;
            FMAT = _fmat;
            
            std::cout << ">> Creating Factor Graph:" << std::endl;
            std::cout << "  | V_LAZY, " << (FMAT == F_LAZY ? "F_LAZY" : "F_EAGER") << ", " << (numSystem == hazy::LOG ? "LOG" : "LINEAR") << ", " << STORAGE::name() << ", " << PAGER::name() << std::endl;
        }
        
        /////////// FUNCTION GROUP 1: INIT A FACTOR GRAPH ////////////
        
        void addFactor(const Factor & _f){
            
            factors.load(_f.oid, _f, dummyrid); // add factor to KV store
            factors.apply_rid(dummyrid, _f.functions->INIT_STATE); // init factor in KV store
            
        }
        
        void addVariable(const Variable & _v, int nfactor){
            
            v2fids_record_not_thread_safe.nfactor = nfactor;
            v2fids_record_not_thread_safe.current = 0;
            v2fids_record_not_thread_safe.variable = _v;
            v2fids.load(_v.oid, v2fids_record_not_thread_safe, dummyrid);
            
            assignment_not_thread_safe.oid = _v.oid;
            assignment_not_thread_safe.cvalue = _v.cvalue;
            assignments.load(_v.oid, assignment_not_thread_safe, dummyrid);
            
        }
        
        void addGroup(const OID & f_oid, const int group_id, const Long size, const double aux){
        	factors.apply_oid(f_oid, _add_group_to_factor, group_id, size, aux);
        }
        
        void addEdge(const OID & v_oid, const OID & f_oid, const int group_id, const int aux, bool is_vg){
            
            v2fids.apply_oid(v_oid, _add_variable_to_factor<decltype(factors)>, f_oid, aux, is_vg, group_id, factors);
            
        }
        
        //////////////////////////////////////////////////////////////
        
        ////////////////// FUNCTION GROUP 2: SAMPLE //////////////////
        
        
        void mat(){}
        
        int inference(int nthread){
            
            int changed = 0;
            
            v2fids.batch_apply(nthread, sample<VariableFactorIDsRecord, decltype(*this)>, numSystem, *this, changed);
            
            factors.sigmod_organizePager();
            assignments.sigmod_organizePager();
            
            SIGMOD_START_LOG_IO = true; // for OPT caching, we start from the second iteration.
            
            return changed;
            
        }
        
        /**
         * Lazily materialize factors if necessary.
         **/
        void prepareFactors(VariableFactorIDsRecord & vrecord){
            
            if(FMAT == hazy::F_LAZY){
                for(int i=0;i<vrecord.nfactor;i++){
                    factors.apply_rid(vrecord.get_n_factor_fullid(i).rid, _mat_factor<decltype(assignments)>, assignments, vrecord.variable);
                }
            }
            
        }
        
        /**
         * Calculate the potential under the hypothesis to turn vrecord'value to cvalue.
         **/
        double calcPotential(VariableFactorIDsRecord & vrecord, const double & cvalue){
            
            double potential;
            if(numSystem == LINEAR){
                potential = 1;
            }else{
                potential = 0;
            }
                            
            for(int i=0;i<vrecord.nfactor;i++){
                factors.apply_rid(vrecord.get_n_factor_fullid(i).rid, _calculate_potential, numSystem, vrecord.variable, cvalue, potential);
            }
            
            return potential;
        }
        
        /**
         * Call VG function of factors.
         **/
        void evaluateVGFactor(VariableFactorIDsRecord & vrecord, VariableGroup & vgroup){
            
            if(vrecord.variable.vgfid.oid != -1){
                factors.apply_rid(vrecord.variable.vgfid.rid, _evaluate_vg_function, vgroup);
            }else{
                assert(false);
            }
            
        }
        
        /**
         * Given a variable group, update the value of it
         **/
        void updateVariableGroupAssignments(VariableFactorIDsRecord & vrecord, VariableGroup & vgroup, int & changed){
            
            for(int i=0;i<vgroup.nvar;i++){


                
                assignments.apply_oid(vgroup.hypothesis[i].vid, _update_variable_assignment, vgroup.hypothesis[i].cvalue);
                
                //if(FMAT==F_EAGER){
                v2fids.apply_oid(vgroup.hypothesis[i].vid, _update_variable_assignment_in_factor_lazy<decltype(factors)>, FMAT, vgroup.hypothesis[i].cvalue, factors, changed);
                //}
                
            }
            
        }
        
        //////////////////////////////////////////////////////////////
        
        


        ////////////////// FUNCTION GROUP 3: REPORT //////////////////
         void getVariableStatus(OID vid, VariableStatus & status){
        	 v2fids.apply_oid(vid, _fill_in_variable_status<VariableFactorIDsRecord>, status);
         }
         
         Long get_maxid()
		 {
			return assignments.get_maxid();
		 }

        void print_io(){
            size_t total_io = 0;
            
            std::cout << "######## IO ########" << std::endl;
            std::cout << "V2FIDS: " << v2fids.get_nio() << std::endl;
            std::cout << "FACTOR: " << factors.get_nio() << std::endl;
            std::cout << "ASSIGN: " << assignments.get_nio() << std::endl;
            std::cout << "####################" << std::endl;
            
            total_io += v2fids.get_nio();
            total_io += factors.get_nio();
            total_io += assignments.get_nio();
        }
        
        //////////////////////////////////////////////////////////////
        
    };

}
    
#endif









