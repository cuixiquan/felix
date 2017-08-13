################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget.cc \
../ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget_test.cc 

OBJS += \
./ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget.o \
./ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget_test.o 

CC_DEPS += \
./ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget.d \
./ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/widget_test.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/%.o: ../ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


