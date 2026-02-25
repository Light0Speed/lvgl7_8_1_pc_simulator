################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/src/lv_demo_printer/lv_demo_printer.c \
../lv_examples/src/lv_demo_printer/lv_demo_printer_theme.c 

C_DEPS += \
./lv_examples/src/lv_demo_printer/lv_demo_printer.d \
./lv_examples/src/lv_demo_printer/lv_demo_printer_theme.d 

OBJS += \
./lv_examples/src/lv_demo_printer/lv_demo_printer.o \
./lv_examples/src/lv_demo_printer/lv_demo_printer_theme.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/src/lv_demo_printer/%.o: ../lv_examples/src/lv_demo_printer/%.c lv_examples/src/lv_demo_printer/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-src-2f-lv_demo_printer

clean-lv_examples-2f-src-2f-lv_demo_printer:
	-$(RM) ./lv_examples/src/lv_demo_printer/lv_demo_printer.d ./lv_examples/src/lv_demo_printer/lv_demo_printer.o ./lv_examples/src/lv_demo_printer/lv_demo_printer_theme.d ./lv_examples/src/lv_demo_printer/lv_demo_printer_theme.o

.PHONY: clean-lv_examples-2f-src-2f-lv_demo_printer

