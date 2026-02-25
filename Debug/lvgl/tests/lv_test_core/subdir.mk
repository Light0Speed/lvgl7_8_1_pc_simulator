################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/tests/lv_test_core/lv_test_core.c \
../lvgl/tests/lv_test_core/lv_test_font_loader.c \
../lvgl/tests/lv_test_core/lv_test_obj.c \
../lvgl/tests/lv_test_core/lv_test_style.c 

C_DEPS += \
./lvgl/tests/lv_test_core/lv_test_core.d \
./lvgl/tests/lv_test_core/lv_test_font_loader.d \
./lvgl/tests/lv_test_core/lv_test_obj.d \
./lvgl/tests/lv_test_core/lv_test_style.d 

OBJS += \
./lvgl/tests/lv_test_core/lv_test_core.o \
./lvgl/tests/lv_test_core/lv_test_font_loader.o \
./lvgl/tests/lv_test_core/lv_test_obj.o \
./lvgl/tests/lv_test_core/lv_test_style.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/tests/lv_test_core/%.o: ../lvgl/tests/lv_test_core/%.c lvgl/tests/lv_test_core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-tests-2f-lv_test_core

clean-lvgl-2f-tests-2f-lv_test_core:
	-$(RM) ./lvgl/tests/lv_test_core/lv_test_core.d ./lvgl/tests/lv_test_core/lv_test_core.o ./lvgl/tests/lv_test_core/lv_test_font_loader.d ./lvgl/tests/lv_test_core/lv_test_font_loader.o ./lvgl/tests/lv_test_core/lv_test_obj.d ./lvgl/tests/lv_test_core/lv_test_obj.o ./lvgl/tests/lv_test_core/lv_test_style.d ./lvgl/tests/lv_test_core/lv_test_style.o

.PHONY: clean-lvgl-2f-tests-2f-lv_test_core

