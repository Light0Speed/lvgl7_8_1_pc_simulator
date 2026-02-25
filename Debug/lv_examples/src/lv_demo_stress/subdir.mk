################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/src/lv_demo_stress/lv_demo_stress.c 

C_DEPS += \
./lv_examples/src/lv_demo_stress/lv_demo_stress.d 

OBJS += \
./lv_examples/src/lv_demo_stress/lv_demo_stress.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/src/lv_demo_stress/%.o: ../lv_examples/src/lv_demo_stress/%.c lv_examples/src/lv_demo_stress/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-src-2f-lv_demo_stress

clean-lv_examples-2f-src-2f-lv_demo_stress:
	-$(RM) ./lv_examples/src/lv_demo_stress/lv_demo_stress.d ./lv_examples/src/lv_demo_stress/lv_demo_stress.o

.PHONY: clean-lv_examples-2f-src-2f-lv_demo_stress

