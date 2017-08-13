################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest-all.cc \
../ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest_main.cc 

OBJS += \
./ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest-all.o \
./ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest_main.o 

CC_DEPS += \
./ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest-all.d \
./ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/gtest_main.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/%.o: ../ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


