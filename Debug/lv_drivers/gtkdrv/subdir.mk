################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_drivers/gtkdrv/gtkdrv.c 

C_DEPS += \
./lv_drivers/gtkdrv/gtkdrv.d 

OBJS += \
./lv_drivers/gtkdrv/gtkdrv.o 


# Each subdirectory must supply rules for building sources it contributes
lv_drivers/gtkdrv/%.o: ../lv_drivers/gtkdrv/%.c lv_drivers/gtkdrv/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_drivers-2f-gtkdrv

clean-lv_drivers-2f-gtkdrv:
	-$(RM) ./lv_drivers/gtkdrv/gtkdrv.d ./lv_drivers/gtkdrv/gtkdrv.o

.PHONY: clean-lv_drivers-2f-gtkdrv

