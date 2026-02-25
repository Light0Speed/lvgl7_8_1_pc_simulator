################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_drivers/win_drv.c 

C_DEPS += \
./lv_drivers/win_drv.d 

OBJS += \
./lv_drivers/win_drv.o 


# Each subdirectory must supply rules for building sources it contributes
lv_drivers/%.o: ../lv_drivers/%.c lv_drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_drivers

clean-lv_drivers:
	-$(RM) ./lv_drivers/win_drv.d ./lv_drivers/win_drv.o

.PHONY: clean-lv_drivers

