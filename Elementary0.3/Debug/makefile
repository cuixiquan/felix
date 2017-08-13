################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/xcode/Samples/FrameworkSample/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/test/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/src/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/samples/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/make/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/fused-src/gtest/subdir.mk
-include ui/bugs_parser/Test/gtest-1.6.0/codegear/subdir.mk
-include ui/bugs_parser/Test/subdir.mk
-include ui/bugs_parser/Test/Builders/subdir.mk
-include ui/bugs_parser/Parser/output/subdir.mk
-include ui/bugs_parser/Code/ModelClasses/subdir.mk
-include ui/bugs_parser/Code/ElementaryAdapter/subdir.mk
-include ui/bugs_parser/Code/DataClasses/subdir.mk
-include ui/bugs_parser/Code/subdir.mk
-include third/tclap-src/examples/subdir.mk
-include lib/userspace-rcu-0.7.6/subdir.mk
-include lib/userspace-rcu-0.7.6/tests/subdir.mk
-include lib/userspace-rcu-0.7.6/.libs/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Elementary_0.3

# Tool invocations
Elementary_0.3: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o"Elementary_0.3" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) Elementary_0.3
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
