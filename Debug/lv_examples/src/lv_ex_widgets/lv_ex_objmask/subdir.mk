################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_1.c \
../lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_2.c 

C_DEPS += \
./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_1.d \
./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_2.d 

OBJS += \
./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_1.o \
./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_2.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/src/lv_ex_widgets/lv_ex_objmask/%.o: ../lv_examples/src/lv_ex_widgets/lv_ex_objmask/%.c lv_examples/src/lv_ex_widgets/lv_ex_objmask/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-src-2f-lv_ex_widgets-2f-lv_ex_objmask

clean-lv_examples-2f-src-2f-lv_ex_widgets-2f-lv_ex_objmask:
	-$(RM) ./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_1.d ./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_1.o ./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_2.d ./lv_examples/src/lv_ex_widgets/lv_ex_objmask/lv_ex_objmask_2.o

.PHONY: clean-lv_examples-2f-src-2f-lv_ex_widgets-2f-lv_ex_objmask

