################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/src/lv_ex_style/lv_ex_style_1.c \
../lv_examples/src/lv_ex_style/lv_ex_style_10.c \
../lv_examples/src/lv_ex_style/lv_ex_style_11.c \
../lv_examples/src/lv_ex_style/lv_ex_style_2.c \
../lv_examples/src/lv_ex_style/lv_ex_style_3.c \
../lv_examples/src/lv_ex_style/lv_ex_style_4.c \
../lv_examples/src/lv_ex_style/lv_ex_style_5.c \
../lv_examples/src/lv_ex_style/lv_ex_style_6.c \
../lv_examples/src/lv_ex_style/lv_ex_style_7.c \
../lv_examples/src/lv_ex_style/lv_ex_style_8.c \
../lv_examples/src/lv_ex_style/lv_ex_style_9.c 

C_DEPS += \
./lv_examples/src/lv_ex_style/lv_ex_style_1.d \
./lv_examples/src/lv_ex_style/lv_ex_style_10.d \
./lv_examples/src/lv_ex_style/lv_ex_style_11.d \
./lv_examples/src/lv_ex_style/lv_ex_style_2.d \
./lv_examples/src/lv_ex_style/lv_ex_style_3.d \
./lv_examples/src/lv_ex_style/lv_ex_style_4.d \
./lv_examples/src/lv_ex_style/lv_ex_style_5.d \
./lv_examples/src/lv_ex_style/lv_ex_style_6.d \
./lv_examples/src/lv_ex_style/lv_ex_style_7.d \
./lv_examples/src/lv_ex_style/lv_ex_style_8.d \
./lv_examples/src/lv_ex_style/lv_ex_style_9.d 

OBJS += \
./lv_examples/src/lv_ex_style/lv_ex_style_1.o \
./lv_examples/src/lv_ex_style/lv_ex_style_10.o \
./lv_examples/src/lv_ex_style/lv_ex_style_11.o \
./lv_examples/src/lv_ex_style/lv_ex_style_2.o \
./lv_examples/src/lv_ex_style/lv_ex_style_3.o \
./lv_examples/src/lv_ex_style/lv_ex_style_4.o \
./lv_examples/src/lv_ex_style/lv_ex_style_5.o \
./lv_examples/src/lv_ex_style/lv_ex_style_6.o \
./lv_examples/src/lv_ex_style/lv_ex_style_7.o \
./lv_examples/src/lv_ex_style/lv_ex_style_8.o \
./lv_examples/src/lv_ex_style/lv_ex_style_9.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/src/lv_ex_style/%.o: ../lv_examples/src/lv_ex_style/%.c lv_examples/src/lv_ex_style/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-src-2f-lv_ex_style

clean-lv_examples-2f-src-2f-lv_ex_style:
	-$(RM) ./lv_examples/src/lv_ex_style/lv_ex_style_1.d ./lv_examples/src/lv_ex_style/lv_ex_style_1.o ./lv_examples/src/lv_ex_style/lv_ex_style_10.d ./lv_examples/src/lv_ex_style/lv_ex_style_10.o ./lv_examples/src/lv_ex_style/lv_ex_style_11.d ./lv_examples/src/lv_ex_style/lv_ex_style_11.o ./lv_examples/src/lv_ex_style/lv_ex_style_2.d ./lv_examples/src/lv_ex_style/lv_ex_style_2.o ./lv_examples/src/lv_ex_style/lv_ex_style_3.d ./lv_examples/src/lv_ex_style/lv_ex_style_3.o ./lv_examples/src/lv_ex_style/lv_ex_style_4.d ./lv_examples/src/lv_ex_style/lv_ex_style_4.o ./lv_examples/src/lv_ex_style/lv_ex_style_5.d ./lv_examples/src/lv_ex_style/lv_ex_style_5.o ./lv_examples/src/lv_ex_style/lv_ex_style_6.d ./lv_examples/src/lv_ex_style/lv_ex_style_6.o ./lv_examples/src/lv_ex_style/lv_ex_style_7.d ./lv_examples/src/lv_ex_style/lv_ex_style_7.o ./lv_examples/src/lv_ex_style/lv_ex_style_8.d ./lv_examples/src/lv_ex_style/lv_ex_style_8.o ./lv_examples/src/lv_ex_style/lv_ex_style_9.d ./lv_examples/src/lv_ex_style/lv_ex_style_9.o

.PHONY: clean-lv_examples-2f-src-2f-lv_ex_style

