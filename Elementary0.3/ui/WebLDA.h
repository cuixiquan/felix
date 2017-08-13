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
//  Filename: WebLDA.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/18/13.
//

#ifndef Elementary0_3_WebLDA_h
#define Elementary0_3_WebLDA_h

#include "../util/Common.h"
#include "../fg/Variable.h"
#include "../fg/Factor.h"
#include "../fg/FactorGraph.h"

#include "../udf/FactorFactory_LDA.h"

#include "../util/Include.h"



#include <glob.h>
#include <fstream>
#include <map>
#include <vector>

// we assume the vocabulary fit in memory
// we assume the document ID fit in memory

typedef std::pair<std::string, int> mypair;

struct IntCmp {
    bool operator()(const mypair &lhs, const mypair &rhs) {
        return lhs.second < rhs.second;
    }
};
        
int WebLDA(std::string dir, int ntopic, float alpha, float beta, std::string output, bool verbose_output)
{
    
    hazy::REUSEMODE = false;
    
    hazy::model_fixmask[0] = false;
    hazy::model_fixmask[1] = false;
    hazy::model_fixmask[2] = false;
    
    glob_t data;
    
    switch( glob((dir + "/*").c_str(), 0, NULL, &data ) )
    {
        case 0:
            break;
        case GLOB_NOSPACE:
            printf( "Out of memory\n" );
            break;
        case GLOB_ABORTED:
            printf( "Reading error\n" );
            break;
        case GLOB_NOMATCH:
            printf( "No files found\n" );
            break;
        default:
            break;
    }
    
    hazy::FactorGraph<hazy::DirectIO_FILE, hazy::Pager<hazy::RANDOM>, hazy::V_LAZY> fg(hazy::LINEAR, hazy::F_EAGER);
        
    std::map<std::string, hazy::Long> docid2factor;
    std::map<std::string, hazy::Long> wordid2factor;
    std::map<hazy::Long, std::string> factor2wordid;
    std::map<hazy::Long, std::string> factor2docid;
    
    // every variable is connected to factor 0
    hazy::Factor f = hazy::FactorFactory_LDA::generate(0, ntopic, 0, true);
    fg.addFactor(f);
    
    int i;
    hazy::Long factorid = 1;
    hazy::Long wordid = 0;
    for(i=0; i<data.gl_pathc; i++)
    {
        
        hazy::Long docfactor = -1;
        
        std::string filename = data.gl_pathv[i];
        std::cout << ">> Processing File " << i << " @ " << filename << std::endl;
        
        if(docid2factor.find(filename) == docid2factor.end()){
            
            docid2factor[filename] = factorid;
            
            hazy::Factor f = hazy::FactorFactory_LDA::generate(factorid, ntopic, 1, false);
                        
            fg.addFactor(f);
            
            factor2docid[factorid] = filename;
            
            factorid ++;
 
        }
        
        docfactor = docid2factor.find(filename)->second;
        
        std::ifstream fin(filename.c_str());
        std::string word;
        while(fin >> word){
            
            hazy::Long wordfactor = -1;
            if(wordid2factor.find(word) == wordid2factor.end()){
                
                wordid2factor[word] = factorid;
                
                hazy::Factor f = hazy::FactorFactory_LDA::generate(factorid, ntopic, 2, false);
                
                fg.addFactor(f);
                factor2wordid[factorid] = word;
                                
                factorid ++;
            }
            
            wordfactor = wordid2factor.find(word)->second;
            
            hazy::Variable v(wordid, hazy::QUERY, hazy::MULTINOMIAL, rand()%ntopic, 0, ntopic-1);
            fg.addVariable(v, 3);
            
            fg.addEdge(wordid, 0, 0, 0, false);
            if(docfactor > wordfactor){
                fg.addEdge(wordid, wordfactor, 0, 0, false);
                fg.addEdge(wordid, docfactor, 0, 0, false);
            }else{
                fg.addEdge(wordid, docfactor, 0, 0, false);
                fg.addEdge(wordid, wordfactor, 0, 0, false);
            }
            
            wordid = wordid + 1;
            
        }
        
    }
    
    globfree( &data );
    
    hazy::Timer timer;
    
    hazy::model[0] = beta * wordid2factor.size();   //W*beta
    hazy::model[1] = alpha;
    hazy::model[2] = beta;
        
    hazy::Timer t;
    hazy::SIGMOD_START_OPTIMAL_CACHE = true;
    for(int i=0; i<=hazy::NEPOCH;i++){
        hazy::Long changed = fg.inference(hazy::NTHREAD);
        std::cout << "EPOCH " << i << " CHANGED=" << changed << " VARS [" << t.elapsed() << "seconds]" << std::endl;
        
    }
    
    fg.print_io();
    
    /*
    hazy::VariableFactorIDsRecord v2f_record;
    hazy::VariableStatus vs;
    
    if(output != ""){
        
        std::ofstream fout_meta((output + ".meta").c_str());
        std::ofstream fout_doctopic((output + ".doc_topic").c_str());
        std::ofstream fout_topicword((output + ".topic_word").c_str());
        std::ofstream fout_all((output + ".all").c_str());
        
        std::cout << "Dumping output to " << output << std::endl;
        for(auto it=docid2factor.begin(); it!=docid2factor.end(); it++){
            fout_meta << it->first << "\t" << it->second << std::endl;
        }
        fout_meta.close();
        
        std::map<std::string, hazy::Long> doc_topic_count;
        std::map<std::string, hazy::Long> topic_word_count;
        
        hazy::Long maxoid = fg.v2fids.get_maxid();
        
        std::string word;
        std::string doc;
        
        for(hazy::Long ovid=0;ovid<maxoid; ovid++){
            
            word = "";
            doc = "";
            
                        fg.v2fids.get_oid(ovid, v2f_record);
            
            hazy::Long sample = (int) v2f_record.first.cvalue;
            
            for(int i=0;i<3;i++){
                hazy::Long fid = v2f_record.seconds[i].oid;
                
                if(factor2wordid.find(fid) != factor2wordid.end()){
                    word = factor2wordid.find(fid)->second;
                    std::string key = std::to_string(sample) + "\t" + word;
                    if(topic_word_count.find(key) == topic_word_count.end()){
                        topic_word_count[key] = 0;
                    }
                    topic_word_count[key] = topic_word_count[key] + 1;
                }
                
                if(factor2docid.find(fid) != factor2docid.end()){
                    doc = factor2docid.find(fid)->second;
                    std::string key = std::to_string(fid) + "\t" + std::to_string(sample);
                    if(doc_topic_count.find(key) == doc_topic_count.end()){
                        doc_topic_count[key] = 0;
                    }
                    doc_topic_count[key] = doc_topic_count[key] + 1;
                }
            }
            
            assert(doc != "");
            assert(word != "");
            
            fout_all << doc << "\t" << word << "\t" << sample << std::endl;
            
        }
        
        for(auto it=topic_word_count.begin();it!=topic_word_count.end();it++){
            fout_topicword << it->first << "\t" << it->second << std::endl;
        }
        
        
        for(auto it=doc_topic_count.begin();it!=doc_topic_count.end();it++){
            fout_doctopic << it->first << "\t" << it->second << std::endl;
        }
        
        fout_all.close();
    }else{
        
        std::cout << "Ignore Dumping output. To dump output, use --output." << std::endl;
    }
    
    if(verbose_output){
        std::cout << "Print more output for each topic..." << std::endl;
        
        for(int topic=0;topic<ntopic;topic++){
            
            std::map<std::string, int> wordcount;
            
            hazy::Long maxoid = fg.v2fids.get_maxid();
            
            for(hazy::Long ovid=0;ovid<maxoid; ovid++){
                
                fg.v2fids.get_oid(ovid, v2f_record);
                
                hazy::Long sample = (int) v2f_record.first.cvalue;
                if(sample != topic){
                    continue;
                }
                
                for(int nf=0;nf<3;nf++){
                    hazy::Long fid = v2f_record.seconds[nf].oid;
                    
                    if(factor2wordid.find(fid) != factor2wordid.end()){
                        std::string word = factor2wordid.find(fid)->second;
                        
                        if(wordcount.find(word) == wordcount.end()){
                            wordcount[word] = 0;
                        }
                        
                        wordcount[word] --;
                        
                    }
                }
                
            }
            
            std::vector<mypair> myvec(wordcount.begin(), wordcount.end());
            //assert(myvec.size() >= 10);
            int print_nword = myvec.size() < 10 ? myvec.size() : 10;
	    std::partial_sort(myvec.begin(), myvec.begin() + print_nword, myvec.end(), IntCmp());
            
            std::cout << "TOPIC " << topic << std::endl;
            for (int i = 0; i < print_nword; ++i) {
                std::cout << "    " << i << ": " << myvec[i].first
                << " -> " << -myvec[i].second << std::endl;
            }
        }
        
    }else{
        std::cout << "Ignore print topic. To print more output for each tpic, use --verbose_output." << std::endl;
        
    }
     */
    
    return 0;
}


#endif
