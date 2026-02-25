################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_drivers/display/GC9A01.c \
../lv_drivers/display/ILI9341.c \
../lv_drivers/display/R61581.c \
../lv_drivers/display/SHARP_MIP.c \
../lv_drivers/display/SSD1963.c \
../lv_drivers/display/ST7565.c \
../lv_drivers/display/UC1610.c \
../lv_drivers/display/drm.c \
../lv_drivers/display/fbdev.c \
../lv_drivers/display/monitor.c 

C_DEPS += \
./lv_drivers/display/GC9A01.d \
./lv_drivers/display/ILI9341.d \
./lv_drivers/display/R61581.d \
./lv_drivers/display/SHARP_MIP.d \
./lv_drivers/display/SSD1963.d \
./lv_drivers/display/ST7565.d \
./lv_drivers/display/UC1610.d \
./lv_drivers/display/drm.d \
./lv_drivers/display/fbdev.d \
./lv_drivers/display/monitor.d 

OBJS += \
./lv_drivers/display/GC9A01.o \
./lv_drivers/display/ILI9341.o \
./lv_drivers/display/R61581.o \
./lv_drivers/display/SHARP_MIP.o \
./lv_drivers/display/SSD1963.o \
./lv_drivers/display/ST7565.o \
./lv_drivers/display/UC1610.o \
./lv_drivers/display/drm.o \
./lv_drivers/display/fbdev.o \
./lv_drivers/display/monitor.o 


# Each subdirectory must supply rules for building sources it contributes
lv_drivers/display/%.o: ../lv_drivers/display/%.c lv_drivers/display/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_drivers-2f-display

clean-lv_drivers-2f-display:
	-$(RM) ./lv_drivers/display/GC9A01.d ./lv_drivers/display/GC9A01.o ./lv_drivers/display/ILI9341.d ./lv_drivers/display/ILI9341.o ./lv_drivers/display/R61581.d ./lv_drivers/display/R61581.o ./lv_drivers/display/SHARP_MIP.d ./lv_drivers/display/SHARP_MIP.o ./lv_drivers/display/SSD1963.d ./lv_drivers/display/SSD1963.o ./lv_drivers/display/ST7565.d ./lv_drivers/display/ST7565.o ./lv_drivers/display/UC1610.d ./lv_drivers/display/UC1610.o ./lv_drivers/display/drm.d ./lv_drivers/display/drm.o ./lv_drivers/display/fbdev.d ./lv_drivers/display/fbdev.o ./lv_drivers/display/monitor.d ./lv_drivers/display/monitor.o

.PHONY: clean-lv_drivers-2f-display

