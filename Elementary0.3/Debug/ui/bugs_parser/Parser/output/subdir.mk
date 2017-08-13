################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ui/bugs_parser/Parser/output/BUGSLexer.cpp \
../ui/bugs_parser/Parser/output/BUGSParser.cpp 

C_SRCS += \
../ui/bugs_parser/Parser/output/BUGSLexer.c \
../ui/bugs_parser/Parser/output/BUGSParser.c 

OBJS += \
./ui/bugs_parser/Parser/output/BUGSLexer.o \
./ui/bugs_parser/Parser/output/BUGSParser.o 

C_DEPS += \
./ui/bugs_parser/Parser/output/BUGSLexer.d \
./ui/bugs_parser/Parser/output/BUGSParser.d 

CPP_DEPS += \
./ui/bugs_parser/Parser/output/BUGSLexer.d \
./ui/bugs_parser/Parser/output/BUGSParser.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Parser/output/%.o: ../ui/bugs_parser/Parser/output/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ui/bugs_parser/Parser/output/%.o: ../ui/bugs_parser/Parser/output/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


