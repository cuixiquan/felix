################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-all.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-death-test.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-filepath.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-port.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-printers.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-test-part.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest-typed-test.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest.cc \
../ui/bugs_parser/Test/gtest-1.6.0/src/gtest_main.cc 

OBJS += \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-all.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-death-test.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-filepath.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-port.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-printers.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-test-part.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-typed-test.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest.o \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest_main.o 

CC_DEPS += \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-all.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-death-test.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-filepath.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-port.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-printers.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-test-part.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest-typed-test.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest.d \
./ui/bugs_parser/Test/gtest-1.6.0/src/gtest_main.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/gtest-1.6.0/src/%.o: ../ui/bugs_parser/Test/gtest-1.6.0/src/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


