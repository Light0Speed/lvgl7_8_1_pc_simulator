################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/lv_themes/lv_theme.c \
../lvgl/src/lv_themes/lv_theme_empty.c \
../lvgl/src/lv_themes/lv_theme_material.c \
../lvgl/src/lv_themes/lv_theme_mono.c \
../lvgl/src/lv_themes/lv_theme_template.c 

C_DEPS += \
./lvgl/src/lv_themes/lv_theme.d \
./lvgl/src/lv_themes/lv_theme_empty.d \
./lvgl/src/lv_themes/lv_theme_material.d \
./lvgl/src/lv_themes/lv_theme_mono.d \
./lvgl/src/lv_themes/lv_theme_template.d 

OBJS += \
./lvgl/src/lv_themes/lv_theme.o \
./lvgl/src/lv_themes/lv_theme_empty.o \
./lvgl/src/lv_themes/lv_theme_material.o \
./lvgl/src/lv_themes/lv_theme_mono.o \
./lvgl/src/lv_themes/lv_theme_template.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/lv_themes/%.o: ../lvgl/src/lv_themes/%.c lvgl/src/lv_themes/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-src-2f-lv_themes

clean-lvgl-2f-src-2f-lv_themes:
	-$(RM) ./lvgl/src/lv_themes/lv_theme.d ./lvgl/src/lv_themes/lv_theme.o ./lvgl/src/lv_themes/lv_theme_empty.d ./lvgl/src/lv_themes/lv_theme_empty.o ./lvgl/src/lv_themes/lv_theme_material.d ./lvgl/src/lv_themes/lv_theme_material.o ./lvgl/src/lv_themes/lv_theme_mono.d ./lvgl/src/lv_themes/lv_theme_mono.o ./lvgl/src/lv_themes/lv_theme_template.d ./lvgl/src/lv_themes/lv_theme_template.o

.PHONY: clean-lvgl-2f-src-2f-lv_themes

