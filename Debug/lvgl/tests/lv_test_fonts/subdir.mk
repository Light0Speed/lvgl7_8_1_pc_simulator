################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/tests/lv_test_fonts/font_1.c \
../lvgl/tests/lv_test_fonts/font_2.c \
../lvgl/tests/lv_test_fonts/font_3.c 

C_DEPS += \
./lvgl/tests/lv_test_fonts/font_1.d \
./lvgl/tests/lv_test_fonts/font_2.d \
./lvgl/tests/lv_test_fonts/font_3.d 

OBJS += \
./lvgl/tests/lv_test_fonts/font_1.o \
./lvgl/tests/lv_test_fonts/font_2.o \
./lvgl/tests/lv_test_fonts/font_3.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/tests/lv_test_fonts/%.o: ../lvgl/tests/lv_test_fonts/%.c lvgl/tests/lv_test_fonts/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-tests-2f-lv_test_fonts

clean-lvgl-2f-tests-2f-lv_test_fonts:
	-$(RM) ./lvgl/tests/lv_test_fonts/font_1.d ./lvgl/tests/lv_test_fonts/font_1.o ./lvgl/tests/lv_test_fonts/font_2.d ./lvgl/tests/lv_test_fonts/font_2.o ./lvgl/tests/lv_test_fonts/font_3.d ./lvgl/tests/lv_test_fonts/font_3.o

.PHONY: clean-lvgl-2f-tests-2f-lv_test_fonts

