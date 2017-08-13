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
//  Filename: FG.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/17/13.
//

#ifndef Elementary0_3_FG_h
#define Elementary0_3_FG_h

#include "../util/Common.h"
#include "../fg/Variable.h"
#include "../fg/Factor.h"
#include "../fg/FactorGraph.h"

#include "../udf/FactorFactory_FG.h"

#include "../util/Include.h"

#include "../util/Include2.h"


void FG(std::string _file_vf, std::vector<std::string> _file_fvs, std::vector<std::string> _file_models, std::string output, bool verbose_output){

    hazy::EXP_NUMSYS = hazy::LOG;
        
    hazy::AbstractFactorGraph * fg = hazy::get_factorgraph_from_config();
    
    hazy::FactorFactory_FG::init();
    
    // parse models
    std::cout << ">> Parse Models..." << std::endl;
    for(auto it=_file_models.begin();it!=_file_models.end();it++){
        
        std::cout << "   | " << it->data() << std::endl;
        
        ifstream fin(it->data());
        int mid, width, height;
        double value;
        
        fin >> mid >> width >> height;
        hazy::FactorFactory_FG_MODELS[mid] = new double*[width];
        for(int i=0;i<width;i++){
            hazy::FactorFactory_FG_MODELS[mid][i] = new double[height];
        }
        
        int npara = 0;
        while(fin >> width >> height >> value){
            npara++;
            hazy::FactorFactory_FG_MODELS[mid][width][height] = value;
        }
        
        std::cout << "      # nparam = " << npara << std::endl;
        
    }
    
    // parse factors
    std::cout << ">> Parsing Factors..." << std::endl;
    for(auto it=_file_fvs.begin();it!=_file_fvs.end();it++){
        std::cout << "   | " << it->data() << std::endl;
        
        // the first integer is function id
        ifstream fin(it->data());
        int func_id;
        fin >> func_id;
        std::cout << "      # func_id = " << func_id << std::endl;
        
        hazy::OID fid;
        int nvar;
        int aux;
        int pid;
        
        hazy::OID nfactor = 0;
        
        while(!fin.eof()){
            
            fin >> fid >> nvar >> aux >> pid;

            if(fin.eof()){
                break;
            }
            
            //std::cout << fid << "   @" << nvar << std::endl;
            hazy::Factor f = hazy::FactorFactory_FG::generate(fid,  func_id, nvar, pid, aux);
            fg->addFactor(f);
            
            nfactor ++;
        }
        
        fin.close();
        
        std::cout << "      # nfactor = " << nfactor << std::endl;
    }
    
    std::cout << ">> Parsing Variables..." << std::endl;
    ifstream fin(_file_vf.c_str());
    
    hazy::OID vid;
    int upper;
    int nfactor;
    hazy::OID fid;
    
    hazy::OID nvars = 0;
    
    while(!fin.eof()){
            
        fin >> vid >> upper >> nfactor;
        
        if(fin.eof()){
            break;
        }
        
        hazy::Variable v(vid, hazy::QUERY, hazy::MULTINOMIAL, rand()%(upper-2)+1, 1, upper-1);
        
        //std::cout << "#" << vid << std::endl;
        fg->addVariable(v, nfactor);
        
        for(int i=0;i<nfactor;i++){
            fin >> fid;
            
            //std::cout << "#" << vid << "    f=" << fid << "    nf=" << nfactor << std::endl;
            
            fg->addEdge(vid, fid, 0, -1, false);
                        
        }
        nvars++;
    }
    
    std::cout << "      # nvars = " << nvars << std::endl;
    
    fin.close();
    
    fg->mat();
    
    fg->print_io();
    hazy::Timer timer;
    hazy::Timer t;
    hazy::SIGMOD_START_OPTIMAL_CACHE = true;
    for(int i=0; i<=hazy::NEPOCH;i++){
        hazy::Long changed = fg->inference(hazy::NTHREAD);
        std::cout << "EPOCH " << i << " CHANGED=" << changed << " VARS [" << t.elapsed() << "seconds]" << std::endl;
        //fg.print_io();
    }
    fg->print_io();
    
}







void test(){
    
    hazy::FactorGraph<hazy::DirectIO_FILE, hazy::Pager<hazy::LRU>, hazy::V_EAGER> fg(hazy::LOG, hazy::F_LAZY);
    
    typedef decltype(fg.assignments) ASSIGNMENT_TYPE;
    typedef hazy::FactorFactory_FG FACTOR_FACTORY;
    
    FACTOR_FACTORY::init();
    
    hazy::model[0] = 2;

    int NUM = 4900000;
    
    for(hazy::OID i=0;i<NUM;i++){
        
        hazy::Variable v(i, hazy::QUERY, hazy::MULTINOMIAL, 1, 0, 1);
        
        if(i==0){
            fg.addVariable(v, 2);
        }else{
            fg.addVariable(v, 3);
        }
        
    }
    
    for(hazy::OID i=0;i<NUM;i++){
        
        /*
        hazy::Factor f = FACTOR_FACTORY::generate(i, 0, 1, 0, 0);
        fg.addFactor(f);
        fg.addEdge(i, i, 0, 0, false);
        */
        
        
        hazy::Factor f = FACTOR_FACTORY::generate(i*2, 0, 1, 0, 0);
        fg.addFactor(f);
        
        hazy::Factor f2 = FACTOR_FACTORY::generate(i*2+1, 1, 2, 0, 0);
        fg.addFactor(f2);
        
        
        fg.addEdge(i, i*2, 0, 0, false);
        
        
        fg.addEdge(i, i*2+1, 0, 0, false);
        
        if(i+1 >= NUM){
            continue;
        }
        
        fg.addEdge(i+1, i*2+1, 0, 0, false);
                
    }
    
    
    fg.mat();
    
    fg.print_io();
    
    hazy::Timer timer;
    hazy::Timer t;
    hazy::SIGMOD_START_OPTIMAL_CACHE = true;
    for(int i=0; i<=hazy::NEPOCH;i++){
        hazy::Long changed = fg.inference(hazy::NTHREAD);
        std::cout << "EPOCH " << i << " CHANGED=" << changed << " VARS [" << t.elapsed() << "seconds]" << std::endl;
        fg.print_io();
        
    }
    
}


#endif
