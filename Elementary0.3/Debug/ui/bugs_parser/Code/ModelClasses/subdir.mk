################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../ui/bugs_parser/Code/ModelClasses/Expression.o \
../ui/bugs_parser/Code/ModelClasses/ForStatement.o \
../ui/bugs_parser/Code/ModelClasses/LinkFunctionNode.o \
../ui/bugs_parser/Code/ModelClasses/LogicalNodeExpression.o \
../ui/bugs_parser/Code/ModelClasses/LogicalNodeStatement.o \
../ui/bugs_parser/Code/ModelClasses/MVDParameters.o \
../ui/bugs_parser/Code/ModelClasses/MultiDimExpression.o \
../ui/bugs_parser/Code/ModelClasses/MultivariateDistribution.o \
../ui/bugs_parser/Code/ModelClasses/MultivariateNode.o \
../ui/bugs_parser/Code/ModelClasses/Program.o \
../ui/bugs_parser/Code/ModelClasses/ScalarFunction.o \
../ui/bugs_parser/Code/ModelClasses/StochasticNodeLimitation.o \
../ui/bugs_parser/Code/ModelClasses/StochasticNodeStatement.o \
../ui/bugs_parser/Code/ModelClasses/UnivariateDistribution.o \
../ui/bugs_parser/Code/ModelClasses/UnivariateNode.o \
../ui/bugs_parser/Code/ModelClasses/VectorFunction.o 

CPP_SRCS += \
../ui/bugs_parser/Code/ModelClasses/Expression.cpp \
../ui/bugs_parser/Code/ModelClasses/ForStatement.cpp \
../ui/bugs_parser/Code/ModelClasses/LinkFunctionNode.cpp \
../ui/bugs_parser/Code/ModelClasses/LogicalNodeExpression.cpp \
../ui/bugs_parser/Code/ModelClasses/LogicalNodeStatement.cpp \
../ui/bugs_parser/Code/ModelClasses/MVDParameters.cpp \
../ui/bugs_parser/Code/ModelClasses/MultiDimExpression.cpp \
../ui/bugs_parser/Code/ModelClasses/MultivariateDistribution.cpp \
../ui/bugs_parser/Code/ModelClasses/MultivariateNode.cpp \
../ui/bugs_parser/Code/ModelClasses/Program.cpp \
../ui/bugs_parser/Code/ModelClasses/ScalarFunction.cpp \
../ui/bugs_parser/Code/ModelClasses/StochasticNodeLimitation.cpp \
../ui/bugs_parser/Code/ModelClasses/StochasticNodeStatement.cpp \
../ui/bugs_parser/Code/ModelClasses/UnivariateDistribution.cpp \
../ui/bugs_parser/Code/ModelClasses/UnivariateNode.cpp \
../ui/bugs_parser/Code/ModelClasses/VectorFunction.cpp 

OBJS += \
./ui/bugs_parser/Code/ModelClasses/Expression.o \
./ui/bugs_parser/Code/ModelClasses/ForStatement.o \
./ui/bugs_parser/Code/ModelClasses/LinkFunctionNode.o \
./ui/bugs_parser/Code/ModelClasses/LogicalNodeExpression.o \
./ui/bugs_parser/Code/ModelClasses/LogicalNodeStatement.o \
./ui/bugs_parser/Code/ModelClasses/MVDParameters.o \
./ui/bugs_parser/Code/ModelClasses/MultiDimExpression.o \
./ui/bugs_parser/Code/ModelClasses/MultivariateDistribution.o \
./ui/bugs_parser/Code/ModelClasses/MultivariateNode.o \
./ui/bugs_parser/Code/ModelClasses/Program.o \
./ui/bugs_parser/Code/ModelClasses/ScalarFunction.o \
./ui/bugs_parser/Code/ModelClasses/StochasticNodeLimitation.o \
./ui/bugs_parser/Code/ModelClasses/StochasticNodeStatement.o \
./ui/bugs_parser/Code/ModelClasses/UnivariateDistribution.o \
./ui/bugs_parser/Code/ModelClasses/UnivariateNode.o \
./ui/bugs_parser/Code/ModelClasses/VectorFunction.o 

CPP_DEPS += \
./ui/bugs_parser/Code/ModelClasses/Expression.d \
./ui/bugs_parser/Code/ModelClasses/ForStatement.d \
./ui/bugs_parser/Code/ModelClasses/LinkFunctionNode.d \
./ui/bugs_parser/Code/ModelClasses/LogicalNodeExpression.d \
./ui/bugs_parser/Code/ModelClasses/LogicalNodeStatement.d \
./ui/bugs_parser/Code/ModelClasses/MVDParameters.d \
./ui/bugs_parser/Code/ModelClasses/MultiDimExpression.d \
./ui/bugs_parser/Code/ModelClasses/MultivariateDistribution.d \
./ui/bugs_parser/Code/ModelClasses/MultivariateNode.d \
./ui/bugs_parser/Code/ModelClasses/Program.d \
./ui/bugs_parser/Code/ModelClasses/ScalarFunction.d \
./ui/bugs_parser/Code/ModelClasses/StochasticNodeLimitation.d \
./ui/bugs_parser/Code/ModelClasses/StochasticNodeStatement.d \
./ui/bugs_parser/Code/ModelClasses/UnivariateDistribution.d \
./ui/bugs_parser/Code/ModelClasses/UnivariateNode.d \
./ui/bugs_parser/Code/ModelClasses/VectorFunction.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Code/ModelClasses/%.o: ../ui/bugs_parser/Code/ModelClasses/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


