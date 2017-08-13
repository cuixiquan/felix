################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../ui/bugs_parser/Code/BUGSDataLexer.o \
../ui/bugs_parser/Code/BUGSDataParser.o \
../ui/bugs_parser/Code/BUGSLexer.o \
../ui/bugs_parser/Code/BUGSParser.o 

CPP_SRCS += \
../ui/bugs_parser/Code/BUGSDataLexer.cpp \
../ui/bugs_parser/Code/BUGSDataParser.cpp \
../ui/bugs_parser/Code/BUGSLexer.cpp \
../ui/bugs_parser/Code/BUGSParser.cpp 

OBJS += \
./ui/bugs_parser/Code/BUGSDataLexer.o \
./ui/bugs_parser/Code/BUGSDataParser.o \
./ui/bugs_parser/Code/BUGSLexer.o \
./ui/bugs_parser/Code/BUGSParser.o 

CPP_DEPS += \
./ui/bugs_parser/Code/BUGSDataLexer.d \
./ui/bugs_parser/Code/BUGSDataParser.d \
./ui/bugs_parser/Code/BUGSLexer.d \
./ui/bugs_parser/Code/BUGSParser.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Code/%.o: ../ui/bugs_parser/Code/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


