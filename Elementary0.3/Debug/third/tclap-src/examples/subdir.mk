################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../third/tclap-src/examples/test1.cpp \
../third/tclap-src/examples/test10.cpp \
../third/tclap-src/examples/test11.cpp \
../third/tclap-src/examples/test12.cpp \
../third/tclap-src/examples/test13.cpp \
../third/tclap-src/examples/test14.cpp \
../third/tclap-src/examples/test15.cpp \
../third/tclap-src/examples/test16.cpp \
../third/tclap-src/examples/test17-a.cpp \
../third/tclap-src/examples/test17.cpp \
../third/tclap-src/examples/test18.cpp \
../third/tclap-src/examples/test19.cpp \
../third/tclap-src/examples/test2.cpp \
../third/tclap-src/examples/test20.cpp \
../third/tclap-src/examples/test21.cpp \
../third/tclap-src/examples/test3.cpp \
../third/tclap-src/examples/test4.cpp \
../third/tclap-src/examples/test5.cpp \
../third/tclap-src/examples/test6.cpp \
../third/tclap-src/examples/test7.cpp \
../third/tclap-src/examples/test8.cpp \
../third/tclap-src/examples/test9.cpp 

OBJS += \
./third/tclap-src/examples/test1.o \
./third/tclap-src/examples/test10.o \
./third/tclap-src/examples/test11.o \
./third/tclap-src/examples/test12.o \
./third/tclap-src/examples/test13.o \
./third/tclap-src/examples/test14.o \
./third/tclap-src/examples/test15.o \
./third/tclap-src/examples/test16.o \
./third/tclap-src/examples/test17-a.o \
./third/tclap-src/examples/test17.o \
./third/tclap-src/examples/test18.o \
./third/tclap-src/examples/test19.o \
./third/tclap-src/examples/test2.o \
./third/tclap-src/examples/test20.o \
./third/tclap-src/examples/test21.o \
./third/tclap-src/examples/test3.o \
./third/tclap-src/examples/test4.o \
./third/tclap-src/examples/test5.o \
./third/tclap-src/examples/test6.o \
./third/tclap-src/examples/test7.o \
./third/tclap-src/examples/test8.o \
./third/tclap-src/examples/test9.o 

CPP_DEPS += \
./third/tclap-src/examples/test1.d \
./third/tclap-src/examples/test10.d \
./third/tclap-src/examples/test11.d \
./third/tclap-src/examples/test12.d \
./third/tclap-src/examples/test13.d \
./third/tclap-src/examples/test14.d \
./third/tclap-src/examples/test15.d \
./third/tclap-src/examples/test16.d \
./third/tclap-src/examples/test17-a.d \
./third/tclap-src/examples/test17.d \
./third/tclap-src/examples/test18.d \
./third/tclap-src/examples/test19.d \
./third/tclap-src/examples/test2.d \
./third/tclap-src/examples/test20.d \
./third/tclap-src/examples/test21.d \
./third/tclap-src/examples/test3.d \
./third/tclap-src/examples/test4.d \
./third/tclap-src/examples/test5.d \
./third/tclap-src/examples/test6.d \
./third/tclap-src/examples/test7.d \
./third/tclap-src/examples/test8.d \
./third/tclap-src/examples/test9.d 


# Each subdirectory must supply rules for building sources it contributes
third/tclap-src/examples/%.o: ../third/tclap-src/examples/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


