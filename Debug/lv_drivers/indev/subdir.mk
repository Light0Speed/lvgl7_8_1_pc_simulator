################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_drivers/indev/AD_touch.c \
../lv_drivers/indev/FT5406EE8.c \
../lv_drivers/indev/XPT2046.c \
../lv_drivers/indev/evdev.c \
../lv_drivers/indev/keyboard.c \
../lv_drivers/indev/libinput.c \
../lv_drivers/indev/mouse.c \
../lv_drivers/indev/mousewheel.c 

C_DEPS += \
./lv_drivers/indev/AD_touch.d \
./lv_drivers/indev/FT5406EE8.d \
./lv_drivers/indev/XPT2046.d \
./lv_drivers/indev/evdev.d \
./lv_drivers/indev/keyboard.d \
./lv_drivers/indev/libinput.d \
./lv_drivers/indev/mouse.d \
./lv_drivers/indev/mousewheel.d 

OBJS += \
./lv_drivers/indev/AD_touch.o \
./lv_drivers/indev/FT5406EE8.o \
./lv_drivers/indev/XPT2046.o \
./lv_drivers/indev/evdev.o \
./lv_drivers/indev/keyboard.o \
./lv_drivers/indev/libinput.o \
./lv_drivers/indev/mouse.o \
./lv_drivers/indev/mousewheel.o 


# Each subdirectory must supply rules for building sources it contributes
lv_drivers/indev/%.o: ../lv_drivers/indev/%.c lv_drivers/indev/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_drivers-2f-indev

clean-lv_drivers-2f-indev:
	-$(RM) ./lv_drivers/indev/AD_touch.d ./lv_drivers/indev/AD_touch.o ./lv_drivers/indev/FT5406EE8.d ./lv_drivers/indev/FT5406EE8.o ./lv_drivers/indev/XPT2046.d ./lv_drivers/indev/XPT2046.o ./lv_drivers/indev/evdev.d ./lv_drivers/indev/evdev.o ./lv_drivers/indev/keyboard.d ./lv_drivers/indev/keyboard.o ./lv_drivers/indev/libinput.d ./lv_drivers/indev/libinput.o ./lv_drivers/indev/mouse.d ./lv_drivers/indev/mouse.o ./lv_drivers/indev/mousewheel.d ./lv_drivers/indev/mousewheel.o

.PHONY: clean-lv_drivers-2f-indev

