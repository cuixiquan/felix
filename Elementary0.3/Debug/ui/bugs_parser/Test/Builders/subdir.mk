################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ui/bugs_parser/Test/Builders/Builder_Data.cpp \
../ui/bugs_parser/Test/Builders/Builder_Expression.cpp \
../ui/bugs_parser/Test/Builders/Builder_IAdapterNodes.cpp \
../ui/bugs_parser/Test/Builders/Builder_LogicalNodeExpression.cpp \
../ui/bugs_parser/Test/Builders/Builder_MultivariateNode.cpp 

OBJS += \
./ui/bugs_parser/Test/Builders/Builder_Data.o \
./ui/bugs_parser/Test/Builders/Builder_Expression.o \
./ui/bugs_parser/Test/Builders/Builder_IAdapterNodes.o \
./ui/bugs_parser/Test/Builders/Builder_LogicalNodeExpression.o \
./ui/bugs_parser/Test/Builders/Builder_MultivariateNode.o 

CPP_DEPS += \
./ui/bugs_parser/Test/Builders/Builder_Data.d \
./ui/bugs_parser/Test/Builders/Builder_Expression.d \
./ui/bugs_parser/Test/Builders/Builder_IAdapterNodes.d \
./ui/bugs_parser/Test/Builders/Builder_LogicalNodeExpression.d \
./ui/bugs_parser/Test/Builders/Builder_MultivariateNode.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Test/Builders/%.o: ../ui/bugs_parser/Test/Builders/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


