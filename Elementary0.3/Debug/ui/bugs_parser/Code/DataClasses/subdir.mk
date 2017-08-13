################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../ui/bugs_parser/Code/DataClasses/BugsArray.o \
../ui/bugs_parser/Code/DataClasses/MainMemoryData.o 

CPP_SRCS += \
../ui/bugs_parser/Code/DataClasses/BugsArray.cpp \
../ui/bugs_parser/Code/DataClasses/MainMemoryData.cpp 

OBJS += \
./ui/bugs_parser/Code/DataClasses/BugsArray.o \
./ui/bugs_parser/Code/DataClasses/MainMemoryData.o 

CPP_DEPS += \
./ui/bugs_parser/Code/DataClasses/BugsArray.d \
./ui/bugs_parser/Code/DataClasses/MainMemoryData.d 


# Each subdirectory must supply rules for building sources it contributes
ui/bugs_parser/Code/DataClasses/%.o: ../ui/bugs_parser/Code/DataClasses/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


