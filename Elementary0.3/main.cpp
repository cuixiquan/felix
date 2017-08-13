//
//  main.cpp
//  Elementary0.3
//
//  Created by Ce Zhang on 2/13/13.
//  Copyright (c) 2013 Hazy Research. All rights reserved.
//

#include <iostream>

#include "util/Common.h"
#include "util/Include.h"
#include "io/AwesomeArray_DIRECT_MM.h"
//#include "io/AwesomeArray_PAGEDBUFFER.h"

#include "fg/FactorGraph.h"

#include "ui/WebLDA.h"
#include "ui/FG.h"

//#include "ui/play.h"
//#include "ui/BUGS.h"
#include "ui2/bugs_parser_db/Bugs_Main.h"

#include "util/CMDParser.h"

//#include <tclap/CmdLine.h>

void * testtt(void * arg){
    
    /*
    rcu_register_thread();
    
    hazy::AwesomeArray<double, hazy::MM, hazy::PAGED, hazy::LRU> * aa
    = (hazy::AwesomeArray<double, hazy::MM, hazy::PAGED, hazy::LRU>*) arg;
    
    double value;
    for(int i=0;i<10000;i++){
        int num = rand() % 10000;
        aa->get_oid(num, value);
        
        //if(3.14*num != value){
        //    std::cout << 3.14*num << " ~~ " << value << std::endl;
        //    assert(false);
        //}
        assert(3.14*num + 1 == value);
        
    }
    */
    
    rcu_unregister_thread();
    
}

void test1(){
    /*
    hazy::AwesomeArray<double, hazy::MM, hazy::DIRECT, hazy::NONE> aa;
    
    hazy::RID rid;
    for(hazy::Long i=0;i<10000;i++){
        aa.load(i, 3.14*i, rid);
    }
    
    aa.batch_apply(10, [&](double & a){a = a + 1;});
    
    std::cout << "BEGIN PAPRALLEL!" << std::endl;
    
    
    LOG_LOCK = true;
    
    
    hazy::Timer t;
    
    int nthreads = 4;
    
    std::thread* threads = new std::thread[nthreads];
    
    for(int w=0;w<nthreads;w++){
        
        threads[w] = std::thread(testtt, &aa);
        
    }
    
    for (int i = 0; i < nthreads; ++i) {
        threads[i].join();
    }
    
    std::cout << t.elapsed() << std::endl;
    
    std::cout << "nio = " << aa.get_nio() << std::endl;
    */
}

void test2(){
    /*
    hazy::AwesomeArray<hazy::VarLengthObject<double, int>, hazy::MM, hazy::PAGED, hazy::RANDOM> aa;
    hazy::AwesomeArray<int, hazy::MM, hazy::DIRECT, hazy::NONE> bb;
    
    hazy::VarLengthObject<double, int> obj;
    hazy::RID rid;
    for(int i=1;i<1000;i++){
        obj.first = 3.14*i;
        obj.len = i;
        for(int w=0;w<i;w++){
            obj.seconds[w] = w;
        }
        aa.load(i, obj, rid);
        bb.load(i, i-1, rid);
    }
    
    hazy::Timer t;
    int sum_last = 0;
    for(int i=1;i<1000;i++){
        aa.get_oid(i, obj);
        sum_last += obj.seconds[obj.len - 1];
        //std::cout << "OBJ_" << i << ": FIRST=" << obj.first << ", LEN=" << obj.len << ", LAST=" << obj.seconds[obj.len - 1] << std::endl;
    }
    std::cout << t.elapsed() << " seconds to calc value=" << sum_last << std::endl;
    
    sum_last = 0;
    int ii;
    for(int i=1;i<1000;i++){
        bb.get_oid(i, ii);
        sum_last += ii;
        //std::cout << "OBJ_" << i << ": FIRST=" << obj.first << ", LEN=" << obj.len << ", LAST=" << obj.seconds[obj.len - 1] << std::endl;
    }
    std::cout << "[DIRECT] " << t.elapsed() << " seconds to calc value=" << sum_last << std::endl;
    
    t.restart();
    sum_last = 0;
    for(int i=1;i<1000;i++){
        aa.apply_oid(i, [&](hazy::VarLengthObject<double, int> & a){sum_last += a.seconds[a.len - 1]; return true;});
        //sum_last += obj.seconds[obj.len - 1];
        //std::cout << "OBJ_" << i << ": FIRST=" << obj.first << ", LEN=" << obj.len << ", LAST=" << obj.seconds[obj.len - 1] << std::endl;
    }
    std::cout << t.elapsed() << " seconds to calc value=" << sum_last << std::endl;
    
    t.restart();
    sum_last = 0;
    for(int i=1;i<1000;i++){
        bb.apply_oid(i, [&](int & a){sum_last += a; return true;});
        //sum_last += obj.seconds[obj.len - 1];
        //std::cout << "OBJ_" << i << ": FIRST=" << obj.first << ", LEN=" << obj.len << ", LAST=" << obj.seconds[obj.len - 1] << std::endl;
    }
    std::cout << "[DIRECT] " << t.elapsed() << " seconds to calc value=" << sum_last << std::endl;
    */
}

void test3(){
    /*
     hazy::FactorGraph<hazy::STATE_GENERAL, hazy::MM, hazy::PAGED, hazy::RANDOM, hazy::LOG> fg;
          
     fg.addFactor(hazy::Factor<hazy::STATE_GENERAL>(1, NULL, hazy::test_potential, -1));
     fg.addFactor(hazy::Factor<hazy::STATE_GENERAL>(2, hazy::test_vg, hazy::test_potential, -1));
     fg.addFactor(hazy::Factor<hazy::STATE_GENERAL>(30, NULL, hazy::test_potential, -1));
     
     fg.addVariable(hazy::Variable(1, hazy::QUERY, hazy::BOOLEAN, -1, -10, 10), 500);
     fg.addVariable(hazy::Variable(2, hazy::QUERY, hazy::BOOLEAN, 3.14, -10, 10), 1);
     fg.addVariable(hazy::Variable(3, hazy::QUERY, hazy::BOOLEAN, 2.71, -10, 10), 1);
     fg.addVariable(hazy::Variable(4, hazy::QUERY, hazy::BOOLEAN, 5.5, -10, 10), 1);
     
     fg.addGroup(1, 0, -1);
     fg.addGroup(2, 0, -2);
     fg.addGroup(2, 1, -2);
     fg.addGroup(30, 0, -3);
     
     fg.addEdge(1, 1, 0, 1, false);
     fg.addEdge(1, 2, 0, 1, false);
     fg.addEdge(1, 2, 1, 1, false);
     fg.addEdge(1, 30, 0, 1, false);
     
     fg.addEdge(2, 2, 0, 1, true);
     fg.addEdge(3, 2, 1, 1, true);
     fg.addEdge(4, 2, 1, 1, false);
     
     hazy::VariableFactorIDsRecord vrecord;
     fg.v2fids.get_oid(3, vrecord);
     fg.updateVariableAssignment(vrecord, 1000000);
     
     fg.inference(1);
     
     */
}

int main(int argc, const char * argv[])
{
    
    hazy::parse_cmd(argc, argv);
    
    //test_page();
    
    //hazy::NTHREAD = 4;
    
    //WebLDA("/Users/czhang/Desktop/eric_data", 50, 0.1, 0.1, "/tmp/out", true);
    
    //WebLDA("/Users/czhang/Desktop/Infrastructure/Elementary_0.2/TestData/WebLDA/ap", 50, 0.1, 0.1, "/tmp/out", true);

    /*
    std::string path_v = "/Users/czhang/Desktop/Infrastructure/Elementary0.3/Test/FG/vf.tsv";
    std::vector<std::string> path_fs;
    path_fs.push_back("/Users/czhang/Desktop/Infrastructure/Elementary0.3/Test/FG/unigram.tsv");
    path_fs.push_back("/Users/czhang/Desktop/Infrastructure/Elementary0.3/Test/FG/bigram.tsv");    
    
    std::vector<std::string> path_models;
    path_models.push_back("/Users/czhang/Desktop/Infrastructure/Elementary0.3/Test/FG/lrmodels");
    path_models.push_back("/Users/czhang/Desktop/Infrastructure/Elementary0.3/Test/FG/crfmodels");
    
    FG(path_v, path_fs, path_models, "", true);
    */
    
	#ifdef __MACH__
//		WebLDA("/Users/czhang/Desktop/Infrastructure/Elementary_0.2/TestData/WebLDA/ap", 50, 0.1, 0.1, "/tmp/out", true);
//		hazy::play();
//		parse_cmd(argc, argv);
//		hazy::runBUGS("ui/bugs_parser/Data/lda-ce.model", "ui/bugs_parser/Data/lda-ce.data", "ui/bugs_parser/Data/test.inits", hazy::NEPOCH);
//	#else
//		parse_cmd(argc, argv);
	#endif

	return 0;
}

