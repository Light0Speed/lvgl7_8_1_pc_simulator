################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/lv_core/lv_disp.c \
../lvgl/src/lv_core/lv_group.c \
../lvgl/src/lv_core/lv_indev.c \
../lvgl/src/lv_core/lv_obj.c \
../lvgl/src/lv_core/lv_refr.c \
../lvgl/src/lv_core/lv_style.c 

C_DEPS += \
./lvgl/src/lv_core/lv_disp.d \
./lvgl/src/lv_core/lv_group.d \
./lvgl/src/lv_core/lv_indev.d \
./lvgl/src/lv_core/lv_obj.d \
./lvgl/src/lv_core/lv_refr.d \
./lvgl/src/lv_core/lv_style.d 

OBJS += \
./lvgl/src/lv_core/lv_disp.o \
./lvgl/src/lv_core/lv_group.o \
./lvgl/src/lv_core/lv_indev.o \
./lvgl/src/lv_core/lv_obj.o \
./lvgl/src/lv_core/lv_refr.o \
./lvgl/src/lv_core/lv_style.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/lv_core/%.o: ../lvgl/src/lv_core/%.c lvgl/src/lv_core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-src-2f-lv_core

clean-lvgl-2f-src-2f-lv_core:
	-$(RM) ./lvgl/src/lv_core/lv_disp.d ./lvgl/src/lv_core/lv_disp.o ./lvgl/src/lv_core/lv_group.d ./lvgl/src/lv_core/lv_group.o ./lvgl/src/lv_core/lv_indev.d ./lvgl/src/lv_core/lv_indev.o ./lvgl/src/lv_core/lv_obj.d ./lvgl/src/lv_core/lv_obj.o ./lvgl/src/lv_core/lv_refr.d ./lvgl/src/lv_core/lv_refr.o ./lvgl/src/lv_core/lv_style.d ./lvgl/src/lv_core/lv_style.o

.PHONY: clean-lvgl-2f-src-2f-lv_core

