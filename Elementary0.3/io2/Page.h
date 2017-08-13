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
//  Filename: Page.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 2/20/13.
//

#ifndef Elementary0_3_Page_h
#define Elementary0_3_Page_h

namespace hazy{

    class BufferPageHeader{
        
    public:
        
        size_t page_id;
        
        bool is_dirty;
        
        char * content;
        
        size_t frame_size;
        
        //BufferPageHeader(size_t pagesize) : frame_size(pagesize){
        //    content = new char[frame_size];
        //    page_id = -1;
        //}
        
        BufferPageHeader() : frame_size(PAGESIZE){
            content = new char[frame_size];
            page_id = -1;
        }
        
    };
    
    
    class SimplePageHeader{
        
    public:
        
        size_t frame_id;
        
        size_t _current_free_slot = 0;
        
        size_t _current_free_pointer_slot = PAGESIZE - 1 - sizeof(size_t);
        
        size_t _nrecord;
        
        size_t _remain_free_slots = PAGESIZE - 1 - 1 - sizeof(size_t);
        
        size_t _nslots = PAGESIZE;
        
        size_t frame_size;
        
        size_t buf_id;
        
        BufferPageHeader * p_buffer_page;
        
        //SimplePageHeader(size_t pagesize) : frame_size(pagesize){
        //    p_buffer_page = NULL;
        //}
        
        SimplePageHeader() : frame_size(PAGESIZE){
            p_buffer_page = NULL;
        }
        
    };
    
}

/*
void fill_in_int(char* const content, hazy::SimplePageHeader & page, const int & value){
    for(size_t i =0; i < page._nslots; i += sizeof(int)){
        reinterpret_cast<int&>(content[i]) = value;
    }
};

void print_page(char* const content, hazy::SimplePageHeader & page){
    std::cout << "PAGE " << page.frame_id << ": ";
    for(size_t i=0; i < page._nslots; i++){
        std::cout << ((int)(content[i])) << " ";
    }
    std::cout << std::endl;
}


void test_page(){
    
    hazy::VarLenStore<hazy::ExampleVarLenObject, hazy::DirectIO_MM, hazy::Pager<hazy::LRU>> store;
    
    hazy::RID dummyrid;
    hazy::ExampleVarLenObject header;
    for(int i=0;i<10000;i++){
        header.id = i;
        header.ndouble = i;
        store.load(i, header, dummyrid);
    }
    
    auto print_item = [](const hazy::ExampleVarLenObject & obj){
        std::cout << "OBJID=" << obj.id << " LEN=" << obj.ndouble << std::endl;
        std::cout << "   VALUES=";
        for(int i=0;i<obj.ndouble;i++){
            std::cout << obj.aaa[i] << " ";
        }
        std::cout << std::endl;
    };
    
    auto fill_double = [](hazy::ExampleVarLenObject & obj){
        for(int i=0;i<obj.ndouble;i++){
            obj.aaa[i] = i;
        }
    };
    
    store.batch_apply(1, fill_double);
    
    //for(int i=0;i<10000;i++){
    //    store.apply_oid(i, fill_double);
    //}
    
    for(int i=0;i<10000;i++){
        store.apply_oid(i, print_item);
    }
    
    
    //hazy::FixLenStore<double> store;
    
    hazy::Timer t;
    
    hazy::RID dummyrid;
    for(int i=0;i<100000000;i++){
        store.load(i, i, dummyrid);
    }
    
    std::cout << t.elapsed() << std::endl;
    
    auto pp = [](double & toplus, const double & value){toplus += value;};
    double toadd = 1;
    for(int i=0;i<100000000;i++){
        store.apply_oid(i, pp, toadd);
    }
    
    
    t.restart();
    double value;
    double sum = 0;
    for(int i=0;i<100000000;i++){
        store.get_oid(i, value);
        sum += value;
    }
    
    std::cout << t.elapsed() << std::endl;
    
    
    
    
    
    //DirectIO_MM dio;
    
    //BufferPageHeader buffer_page;
    
    //Buffer<DirectIO_MM> buffer;

    
    for(size_t i=0;i<100;i++){
        buffer.load(i);
    }
    
    
    int a = 1;
    for(size_t i=0;i<100;i++){
        buffer.apply(i, fill_in_int, a);
    }
    
    //for(size_t i=0;i<100;i++){
    //    buffer.apply(i, print_page);
    //}
    
    
}
    */


#endif










