################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../lib/userspace-rcu-0.7.6/liburcu_mb_la-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/liburcu_mb_la-urcu.o \
../lib/userspace-rcu-0.7.6/liburcu_signal_la-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/liburcu_signal_la-urcu.o \
../lib/userspace-rcu-0.7.6/rculfhash-mm-chunk.o \
../lib/userspace-rcu-0.7.6/rculfhash-mm-mmap.o \
../lib/userspace-rcu-0.7.6/rculfhash-mm-order.o \
../lib/userspace-rcu-0.7.6/rculfhash.o \
../lib/userspace-rcu-0.7.6/rculfqueue.o \
../lib/userspace-rcu-0.7.6/rculfstack.o \
../lib/userspace-rcu-0.7.6/urcu-bp.o \
../lib/userspace-rcu-0.7.6/urcu-pointer.o \
../lib/userspace-rcu-0.7.6/urcu-qsbr.o \
../lib/userspace-rcu-0.7.6/urcu.o \
../lib/userspace-rcu-0.7.6/wfqueue.o \
../lib/userspace-rcu-0.7.6/wfstack.o 

C_SRCS += \
../lib/userspace-rcu-0.7.6/compat_arch_x86.c \
../lib/userspace-rcu-0.7.6/compat_futex.c \
../lib/userspace-rcu-0.7.6/rculfhash-mm-chunk.c \
../lib/userspace-rcu-0.7.6/rculfhash-mm-mmap.c \
../lib/userspace-rcu-0.7.6/rculfhash-mm-order.c \
../lib/userspace-rcu-0.7.6/rculfhash.c \
../lib/userspace-rcu-0.7.6/rculfqueue.c \
../lib/userspace-rcu-0.7.6/rculfstack.c \
../lib/userspace-rcu-0.7.6/urcu-bp.c \
../lib/userspace-rcu-0.7.6/urcu-pointer.c \
../lib/userspace-rcu-0.7.6/urcu-qsbr.c \
../lib/userspace-rcu-0.7.6/urcu.c \
../lib/userspace-rcu-0.7.6/wfqueue.c \
../lib/userspace-rcu-0.7.6/wfstack.c 

OBJS += \
./lib/userspace-rcu-0.7.6/compat_arch_x86.o \
./lib/userspace-rcu-0.7.6/compat_futex.o \
./lib/userspace-rcu-0.7.6/rculfhash-mm-chunk.o \
./lib/userspace-rcu-0.7.6/rculfhash-mm-mmap.o \
./lib/userspace-rcu-0.7.6/rculfhash-mm-order.o \
./lib/userspace-rcu-0.7.6/rculfhash.o \
./lib/userspace-rcu-0.7.6/rculfqueue.o \
./lib/userspace-rcu-0.7.6/rculfstack.o \
./lib/userspace-rcu-0.7.6/urcu-bp.o \
./lib/userspace-rcu-0.7.6/urcu-pointer.o \
./lib/userspace-rcu-0.7.6/urcu-qsbr.o \
./lib/userspace-rcu-0.7.6/urcu.o \
./lib/userspace-rcu-0.7.6/wfqueue.o \
./lib/userspace-rcu-0.7.6/wfstack.o 

C_DEPS += \
./lib/userspace-rcu-0.7.6/compat_arch_x86.d \
./lib/userspace-rcu-0.7.6/compat_futex.d \
./lib/userspace-rcu-0.7.6/rculfhash-mm-chunk.d \
./lib/userspace-rcu-0.7.6/rculfhash-mm-mmap.d \
./lib/userspace-rcu-0.7.6/rculfhash-mm-order.d \
./lib/userspace-rcu-0.7.6/rculfhash.d \
./lib/userspace-rcu-0.7.6/rculfqueue.d \
./lib/userspace-rcu-0.7.6/rculfstack.d \
./lib/userspace-rcu-0.7.6/urcu-bp.d \
./lib/userspace-rcu-0.7.6/urcu-pointer.d \
./lib/userspace-rcu-0.7.6/urcu-qsbr.d \
./lib/userspace-rcu-0.7.6/urcu.d \
./lib/userspace-rcu-0.7.6/wfqueue.d \
./lib/userspace-rcu-0.7.6/wfstack.d 


# Each subdirectory must supply rules for building sources it contributes
lib/userspace-rcu-0.7.6/%.o: ../lib/userspace-rcu-0.7.6/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


