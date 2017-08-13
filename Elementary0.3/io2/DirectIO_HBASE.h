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
//  Filename: DirectIO_HBASE.h
//  Project:  Elementary0.3
//
//  Created by Ce Zhang on 3/22/13.
//

#ifndef Elementary0_3_DirectIO_HBASE_h
#define Elementary0_3_DirectIO_HBASE_h

/*
#include <time.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <poll.h>

#include <iostream>

#include <boost/lexical_cast.hpp>
#include <protocol/TBinaryProtocol.h>
#include <transport/TSocket.h>
#include <transport/TTransportUtils.h>

#include "gen-cpp/Hbase.h"

namespace hazy{

    int HBASEID = 0;
    
    class DirectIO_HBASE{
    
    private:
    
        pthread_mutex_t* mutex;
        
        boost::shared_ptr<apache::thrift::transport::TTransport> socket;
        boost::shared_ptr<apache::thrift::transport::TTransport> transport;
        boost::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
        apache::hadoop::hbase::thrift::HbaseClient * client;
        
        std::map<apache::hadoop::hbase::thrift::Text, apache::hadoop::hbase::thrift::Text> empty_attrs;
        
        std::string t;
        
        std::vector<std::string> columnNames;
        
        apache::hadoop::hbase::thrift::Mutation mutation;
        
        
    public:
    
        static std::string name(){return "HBASE";}
    
        const size_t frame_size;
    
        DirectIO_HBASE(size_t pagesize) : frame_size(pagesize) {
        
            HBASEID ++;
            
            char tablename[1000];
            sprintf(tablename, "elly_testtable_%d_%d", hbaseid, time(NULL) );
                        
            columnNames.push_back("key:");
            
            mutation.column = "key:key";
            
            mutex = new pthread_mutex_t;
            pthread_mutex_init(mutex, NULL);
            
            socket = boost::shared_ptr<apache::thrift::transport::TTransport>(new apache::thrift::transport::TSocket(rt_hbase_thrift_server.c_str(), rt_hbase_thrift_port));
            transport = boost::shared_ptr<apache::thrift::transport::TTransport>(new apache::thrift::transport::TBufferedTransport(socket));
            protocol = boost::shared_ptr<apache::thrift::protocol::TProtocol>(new apache::thrift::protocol::TBinaryProtocol(transport));
            
            client = new apache::hadoop::hbase::thrift::HbaseClient(protocol);
            
            transport->open();
            
            t = std::string(tablename);
            std::vector<apache::hadoop::hbase::thrift::ColumnDescriptor> columns;
            columns.push_back(apache::hadoop::hbase::thrift::ColumnDescriptor());
            columns.back().name = "key:";
            columns.back().maxVersions = 1;
            
            std::cout << "droping table: " << t << std::endl;
            try {
                client->disableTable(t);
                client->deleteTable(t);
            }catch (const apache::hadoop::hbase::thrift::IOError &ioe){
                std::cerr << "WARN: " << ioe.message << std::endl;
            }
            
            std::cout << "creating table: " << t << std::endl;
            try{
                client->createTable(t, columns);
                //client->enableTable(t);
            } catch (const apache::hadoop::hbase::thrift::AlreadyExists &ae) {
                std::cerr << "WARN: " << ae.message << std::endl;
            }
        }
    
    void load(size_t frame_id){
        for(size_t i=frames.size();i<=frame_id;i++){
            frames.push_back(NULL);
        }
        frames[frame_id] = new char[frame_size];
    }
    
    void get(BufferPageHeader & buffer_page, size_t frame_id){
        buffer_page.content = frames[frame_id];
    }
    
    void put(const BufferPageHeader & buffer_page, size_t frame_id){
    }
};
    
    
}*/

#endif
