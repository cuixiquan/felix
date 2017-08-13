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
//  Filename: CMDParser.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_CMDParser_h
#define Elementary0_3_CMDParser_h

#include "../fg/FactorGraph.h"
#include <tclap/CmdLine.h>

namespace hazy{

    
    
    

    void parse_cmd(int argc, const char * argv[]){
        try{
            
            TCLAP::CmdLine cmd("", ' ', "0.3");
            
            TCLAP::ValueArg<std::string> app("", "app", "Application. [lda, bugs, fg]",true,"","string");
            
            TCLAP::ValueArg<std::string> work_dir("", "work_dir", "Folder to store temporary files",true,"/tmp/","string");
            
            TCLAP::ValueArg<std::string> data("", "data", "Data folder to use",false,"","string");
            
            TCLAP::ValueArg<std::string> output("", "output", "Output file",false,"","string");
            
            TCLAP::SwitchArg verbose_output("","verbose_output","Print more output on command line.", cmd, false);
            
            TCLAP::ValueArg<int> nthread("","nthread","Number of threads to use",false,1,"int");
            
            TCLAP::ValueArg<int> nepoch("","nepoch","Number of epochs to run",false,20,"int");
            
            ////////////EXPERIMENT///////////
            
            TCLAP::ValueArg<std::string> exp_mat("","mat","[EXP] Materialization strategy to use. [EAGER, LAZY, VCOC, FCOC]",false,"","string");
            
            TCLAP::ValueArg<std::string> exp_storage("","storage","[EXP] Storage to use. [MM, FILE, HBASE]",false,"","string");
            
            TCLAP::ValueArg<std::string> exp_paging("","paging","[EXP] Paging replacement strategy to use. [LRU, RANDOM, OPTIMAL]",false,"","string");
            
            
            ////////////LDA////////////////
            
            TCLAP::ValueArg<float> lda_alpha("","lda_alpha","[LDA] Alpha parameter. http://www.pnas.org/content/101/suppl.1/5228.full.pdf",false,0.1,"float");
            
            TCLAP::ValueArg<float> lda_beta("","lda_beta","[LDA] Beta parameter. http://www.pnas.org/content/101/suppl.1/5228.full.pdf",false,0.1,"float");
            
            TCLAP::ValueArg<float> lda_ntopic("","lda_ntopic","[LDA] Number of topics (<200. If you need more, set MAX_N_TOPICS and recompile.). ",false,0.1,"float");
            
            
            ////////////FG////////////////
                        
            TCLAP::ValueArg<std::string> fg_vrel("", "fg_vrel", "[LR] Variable Relation of Factor Graph",false,"","string");
            TCLAP::MultiArg<std::string> fg_factors("", "fg_factors", "[LR] Factor Relations of Factor Graph", false,"string");
            TCLAP::MultiArg<std::string> fg_models("", "fg_models", "[LR] Model Relations of Factor Graph", false,"string");
            
            ////////////BUGS////////////////
            
            TCLAP::SwitchArg bugs_hybrid("","bugs_hybrid","Run Bugs Using both PostgreSQL and GreenPlum", false);
            
            cmd.add(app);
            cmd.add(work_dir);
            cmd.add(data);
            cmd.add(output);
            
            cmd.add(nthread);
            cmd.add(nepoch);
            
            cmd.add(exp_mat);
            cmd.add(exp_storage);
            cmd.add(exp_paging);
            
            cmd.add(lda_alpha);
            cmd.add(lda_beta);
            cmd.add(lda_ntopic);
            
            cmd.add(fg_vrel);
            cmd.add(fg_factors);
            cmd.add(fg_models);
            
            cmd.add(bugs_hybrid);
            cmd.parse( argc, argv );
            
            hazy::NTHREAD = nthread.getValue();
            hazy::NEPOCH = nepoch.getValue();

            if(exp_mat.getValue() != ""){
                hazy::EXP_MAT = exp_mat.getValue();
            }
            if(exp_paging.getValue() != ""){
                hazy::EXP_PAGING = exp_paging.getValue();
            }
            if(exp_storage.getValue() != ""){
                hazy::EXP_STORAGE = exp_storage.getValue();
            }
            
            config_tmp_folder = work_dir.getValue();
            
            if(app.getValue() == "lda"){
                
                WebLDA(data.getValue(), lda_ntopic.getValue(), lda_alpha.getValue(), lda_beta.getValue(), output.getValue(), verbose_output.getValue());
                
            }else if(app.getValue() == "fg"){
                
                FG(fg_vrel.getValue(), fg_factors.getValue(), fg_models.getValue(), output.getValue(), verbose_output.getValue());
                
            }else if(app.getValue() == "bugs"){
                
				if(bugs_hybrid.getValue())
				{
					hazy::runBUGSHybrid(hazy::NEPOCH);
				}
				else
				{
					hazy::runBUGSPostgreSQL(hazy::NEPOCH);
				}
            }else{
                std::cout << "PARSE ERROR:" << std::endl;
                std::cout << "              Unknown value for argument: app" << std::endl;
            }
            
            return;
            
        }catch (TCLAP::ArgException &e){
            std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
            return;
        }
        
    }
    
    
}

#endif
