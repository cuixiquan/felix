################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../lib/userspace-rcu-0.7.6/tests/rcutorture_urcu-urcutorture.o \
../lib/userspace-rcu-0.7.6/tests/rcutorture_urcu_bp-urcutorture.o \
../lib/userspace-rcu-0.7.6/tests/rcutorture_urcu_mb-urcutorture.o \
../lib/userspace-rcu-0.7.6/tests/rcutorture_urcu_qsbr-urcutorture.o \
../lib/userspace-rcu-0.7.6/tests/rcutorture_urcu_signal-urcutorture.o \
../lib/userspace-rcu-0.7.6/tests/test_cycles_per_loop.o \
../lib/userspace-rcu-0.7.6/tests/test_looplen.o \
../lib/userspace-rcu-0.7.6/tests/test_mutex.o \
../lib/userspace-rcu-0.7.6/tests/test_perthreadlock.o \
../lib/userspace-rcu-0.7.6/tests/test_perthreadlock_timing.o \
../lib/userspace-rcu-0.7.6/tests/test_rwlock.o \
../lib/userspace-rcu-0.7.6/tests/test_rwlock_timing.o \
../lib/userspace-rcu-0.7.6/tests/test_uatomic.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign_dynamic_link-test_urcu_assign.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign_dynamic_link-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign_dynamic_link-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign_dynamic_link-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp_dynamic_link-test_urcu_bp.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp_dynamic_link-urcu-bp.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp_dynamic_link-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp_dynamic_link-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_defer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_dynamic_link-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_dynamic_link-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_dynamic_link-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_dynamic_link-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_fork.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash-test_urcu_hash.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash-test_urcu_hash_rw.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash-test_urcu_hash_unique.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq_dynlink-test_urcu_lfq.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq_dynlink-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq_dynlink-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq_dynlink-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs_dynlink-test_urcu_lfs.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs_dynlink-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs_dynlink-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs_dynlink-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lgc-test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lgc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lgc-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lgc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_gc-test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_gc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_gc-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_gc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_lgc-test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_lgc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_lgc-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_mb_lgc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_dynamic_link-test_urcu_qsbr.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_dynamic_link-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_dynamic_link-urcu-qsbr.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_dynamic_link-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_lgc-test_urcu_qsbr_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_lgc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_lgc-urcu-qsbr.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_lgc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_timing.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_dynamic_link-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_dynamic_link-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_dynamic_link-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_dynamic_link-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_gc-test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_gc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_gc-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_gc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_lgc-test_urcu_gc.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_lgc-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_lgc-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_lgc-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_timing-test_urcu_timing.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_timing-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_timing-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_timing-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_yield-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_yield-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_yield-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_signal_yield-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_timing.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfq.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfq_dynlink-test_urcu_wfq.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfs.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfs_dynlink-test_urcu_wfs.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_yield-test_urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_yield-urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_yield-urcu.o \
../lib/userspace-rcu-0.7.6/tests/test_urcu_yield-wfqueue.o \
../lib/userspace-rcu-0.7.6/tests/urcu-bp.o \
../lib/userspace-rcu-0.7.6/tests/urcu-pointer.o \
../lib/userspace-rcu-0.7.6/tests/urcu-qsbr.o \
../lib/userspace-rcu-0.7.6/tests/urcu.o \
../lib/userspace-rcu-0.7.6/tests/wfqueue.o 

C_SRCS += \
../lib/userspace-rcu-0.7.6/tests/test_cycles_per_loop.c \
../lib/userspace-rcu-0.7.6/tests/test_looplen.c \
../lib/userspace-rcu-0.7.6/tests/test_mutex.c \
../lib/userspace-rcu-0.7.6/tests/test_perthreadlock.c \
../lib/userspace-rcu-0.7.6/tests/test_perthreadlock_timing.c \
../lib/userspace-rcu-0.7.6/tests/test_rwlock.c \
../lib/userspace-rcu-0.7.6/tests/test_rwlock_timing.c \
../lib/userspace-rcu-0.7.6/tests/test_uatomic.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_assign.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_bp.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_defer.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_fork.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_gc.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash_rw.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_hash_unique.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfq.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_lfs.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_gc.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_timing.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_timing.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfq.c \
../lib/userspace-rcu-0.7.6/tests/test_urcu_wfs.c \
../lib/userspace-rcu-0.7.6/tests/urcutorture.c 

OBJS += \
./lib/userspace-rcu-0.7.6/tests/test_cycles_per_loop.o \
./lib/userspace-rcu-0.7.6/tests/test_looplen.o \
./lib/userspace-rcu-0.7.6/tests/test_mutex.o \
./lib/userspace-rcu-0.7.6/tests/test_perthreadlock.o \
./lib/userspace-rcu-0.7.6/tests/test_perthreadlock_timing.o \
./lib/userspace-rcu-0.7.6/tests/test_rwlock.o \
./lib/userspace-rcu-0.7.6/tests/test_rwlock_timing.o \
./lib/userspace-rcu-0.7.6/tests/test_uatomic.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_assign.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_bp.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_defer.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_fork.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_gc.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash_rw.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash_unique.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_lfq.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_lfs.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_gc.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_timing.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_timing.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_wfq.o \
./lib/userspace-rcu-0.7.6/tests/test_urcu_wfs.o \
./lib/userspace-rcu-0.7.6/tests/urcutorture.o 

C_DEPS += \
./lib/userspace-rcu-0.7.6/tests/test_cycles_per_loop.d \
./lib/userspace-rcu-0.7.6/tests/test_looplen.d \
./lib/userspace-rcu-0.7.6/tests/test_mutex.d \
./lib/userspace-rcu-0.7.6/tests/test_perthreadlock.d \
./lib/userspace-rcu-0.7.6/tests/test_perthreadlock_timing.d \
./lib/userspace-rcu-0.7.6/tests/test_rwlock.d \
./lib/userspace-rcu-0.7.6/tests/test_rwlock_timing.d \
./lib/userspace-rcu-0.7.6/tests/test_uatomic.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_assign.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_bp.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_defer.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_fork.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_gc.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash_rw.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_hash_unique.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_lfq.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_lfs.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_gc.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_qsbr_timing.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_timing.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_wfq.d \
./lib/userspace-rcu-0.7.6/tests/test_urcu_wfs.d \
./lib/userspace-rcu-0.7.6/tests/urcutorture.d 


# Each subdirectory must supply rules for building sources it contributes
lib/userspace-rcu-0.7.6/tests/%.o: ../lib/userspace-rcu-0.7.6/tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


