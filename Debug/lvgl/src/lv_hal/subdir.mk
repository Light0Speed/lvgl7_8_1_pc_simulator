################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/lv_hal/lv_hal_disp.c \
../lvgl/src/lv_hal/lv_hal_indev.c \
../lvgl/src/lv_hal/lv_hal_tick.c 

C_DEPS += \
./lvgl/src/lv_hal/lv_hal_disp.d \
./lvgl/src/lv_hal/lv_hal_indev.d \
./lvgl/src/lv_hal/lv_hal_tick.d 

OBJS += \
./lvgl/src/lv_hal/lv_hal_disp.o \
./lvgl/src/lv_hal/lv_hal_indev.o \
./lvgl/src/lv_hal/lv_hal_tick.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/lv_hal/%.o: ../lvgl/src/lv_hal/%.c lvgl/src/lv_hal/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-src-2f-lv_hal

clean-lvgl-2f-src-2f-lv_hal:
	-$(RM) ./lvgl/src/lv_hal/lv_hal_disp.d ./lvgl/src/lv_hal/lv_hal_disp.o ./lvgl/src/lv_hal/lv_hal_indev.d ./lvgl/src/lv_hal/lv_hal_indev.o ./lvgl/src/lv_hal/lv_hal_tick.d ./lvgl/src/lv_hal/lv_hal_tick.o

.PHONY: clean-lvgl-2f-src-2f-lv_hal

