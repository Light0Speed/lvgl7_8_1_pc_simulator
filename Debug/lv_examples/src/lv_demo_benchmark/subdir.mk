################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.c 

C_DEPS += \
./lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.d 

OBJS += \
./lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/src/lv_demo_benchmark/%.o: ../lv_examples/src/lv_demo_benchmark/%.c lv_examples/src/lv_demo_benchmark/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-src-2f-lv_demo_benchmark

clean-lv_examples-2f-src-2f-lv_demo_benchmark:
	-$(RM) ./lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.d ./lv_examples/src/lv_demo_benchmark/lv_demo_benchmark.o

.PHONY: clean-lv_examples-2f-src-2f-lv_demo_benchmark

