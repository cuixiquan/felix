################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ui/bugs_parser/Test/Test_DataAccess.cpp \
../ui/bugs_parser/Test/Test_Expression.cpp \
../ui/bugs_parser/Test/Test_LogicalNodeExpression.cpp \
../ui/bugs_parser/Test/Test_MultivariateDistribution.cpp \
../ui/bugs_parser/Test/Test_MultivariateNode.cpp \
../ui/bugs_parser/Test/Test_PotentialFunctions.cpp \
../ui/bugs_parser/Test/Test_UnivariateNode.cpp \
../ui/bugs_parser/Test/Test_main.cpp 

OBJS += \
./ui/bugs_parser/Test/Test_DataAccess.o \
./ui/bugs_parser/Test/Test_Expression.o \
./ui/bugs_parser/Test/Test_LogicalNodeExpression.o \
./ui/bugs_parser/Test/Test_MultivariateDistribution.o \
./ui/bugs_parser/Test/Test_MultivariateNode.o \
./ui/bugs_parser/Test/Test_PotentialFunctions.o \
./ui/bugs_parser/Test/Test_UnivariateNode.o \
./ui/bugs_parser/Test/Test_main.o 

CPP_DEPS += \
./ui/bugs_parser/Test/Test_DataAccess.d \
./ui/bugs_parser/Test/Test_Expression.d \
./ui/bugs_parser/Test/Test_LogicalNodeExpression.d \
./ui/bugs_parser/Test/Test_MultivariateDistribution.d \
./ui/bugs_parser/Test/Test_MultivariateNode.d \
./ui/bugs_parser/Test/Test_PotentialFunctions.d \
./ui/bugs_parser/Test/Test_UnivariateNode.d \
./ui/bugs_parser/Test/Test_main.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/%.o: ../ui/bugs_parser/Test/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


