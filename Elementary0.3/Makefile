
UNAME := $(shell uname)

## WE NEED TO SET BUILD_PREFIX=--build=x86_64-apple-darwin10.0.0 ON MAC
APP ?= ""
BUILD_PREFIX ?= ""
CC ?= gcc
CPP ?= g++
LIBS  = -lrt
PG_PATH ?= /usr/local/pgsql
CFLAGS = -g -I./ui2/bugs_parser_db/src/ParserClasses/include -I${PG_PATH}/include -O2 -I./third/ -c

# Should be equivalent to your list of C files, if you don't build selectively
BUGSOBJECTSSRC=$(wildcard ui2/bugs_parser_db/src/*/*.cpp)
BUGSOBJECTFILES := $(patsubst %.cpp, %.o, ${BUGSOBJECTSSRC})

all:    $(BUGSOBJECTFILES)
	${CPP} ${CFLAGS} -L./lib/urcu/lib/ -I./lib/urcu/include/ -lurcu-qsbr -lrt -std=c++0x -lpthread main.cpp
	${CPP} -o ele main.o ui2/bugs_parser_db/src/*/*.o -lrt -lpthread -L./lib/urcu/lib/ -lurcu-qsbr -L${PG_PATH}/lib -lpq

%.o: %.cpp
	${CPP} ${CFLAGS} -L./lib/urcu/lib/ -I./lib/urcu/include/ -O2 -lurcu-qsbr -lrt -c -std=c++0x $< -o $@

dep:	
	mkdir -p lib/urcu
	mkdir -p lib/thrift

	#cd lib/thrift-0.3.0 ;\
	#CC=${CC} CPP=${CPP} ./configure --prefix=$(CURDIR)/lib/thrift/;\
	#make ;\
	#make install
	
	cd lib/userspace-rcu-0.7.6; \
	CFLAGS="-m64 -O2" CC=${CC} ./configure --prefix=$(CURDIR)/lib/urcu/ ${BUILD_PREFIX}; \
	make; \
	make install ;

cleanparser:
	rm -f *.o
	rm -f ui2/bugs_parser_db/src/*/*.o

clean:	
	rm -f *.o

	rm -f ui2/bugs_parser_db/src/*/*.o

	rm -rf lib/urcu

	cd lib/userspace-rcu-0.7.6 ;\
	make clean ;\

	#cd lib/thrift-0.3.0 ;\
	#make clean ;\



