################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample1.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample10_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample1_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample2.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample2_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample3_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample4.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample4_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample5_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample6_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample7_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample8_unittest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/samples/sample9_unittest.cc 

OBJS += \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample1.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample10_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample1_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample2.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample2_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample3_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample4.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample4_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample5_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample6_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample7_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample8_unittest.o \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample9_unittest.o 

CC_DEPS += \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample1.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample10_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample1_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample2.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample2_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample3_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample4.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample4_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample5_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample6_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample7_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample8_unittest.d \
./ui/bugs_parser/Test/gtest-1.6.0/samples/sample9_unittest.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/gtest-1.6.0/samples/%.o: ../ui/bugs_parser/Test/gtest-1.6.0/samples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


