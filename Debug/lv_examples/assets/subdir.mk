################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lv_examples/assets/img_cogwheel_alpha16.c \
../lv_examples/assets/img_cogwheel_argb.c \
../lv_examples/assets/img_cogwheel_chroma_keyed.c \
../lv_examples/assets/img_cogwheel_indexed16.c \
../lv_examples/assets/img_cogwheel_rgb.c \
../lv_examples/assets/img_hand.c \
../lv_examples/assets/lv_font_montserrat_12_compr_az.c \
../lv_examples/assets/lv_font_montserrat_16_compr_az.c \
../lv_examples/assets/lv_font_montserrat_28_compr_az.c 

C_DEPS += \
./lv_examples/assets/img_cogwheel_alpha16.d \
./lv_examples/assets/img_cogwheel_argb.d \
./lv_examples/assets/img_cogwheel_chroma_keyed.d \
./lv_examples/assets/img_cogwheel_indexed16.d \
./lv_examples/assets/img_cogwheel_rgb.d \
./lv_examples/assets/img_hand.d \
./lv_examples/assets/lv_font_montserrat_12_compr_az.d \
./lv_examples/assets/lv_font_montserrat_16_compr_az.d \
./lv_examples/assets/lv_font_montserrat_28_compr_az.d 

OBJS += \
./lv_examples/assets/img_cogwheel_alpha16.o \
./lv_examples/assets/img_cogwheel_argb.o \
./lv_examples/assets/img_cogwheel_chroma_keyed.o \
./lv_examples/assets/img_cogwheel_indexed16.o \
./lv_examples/assets/img_cogwheel_rgb.o \
./lv_examples/assets/img_hand.o \
./lv_examples/assets/lv_font_montserrat_12_compr_az.o \
./lv_examples/assets/lv_font_montserrat_16_compr_az.o \
./lv_examples/assets/lv_font_montserrat_28_compr_az.o 


# Each subdirectory must supply rules for building sources it contributes
lv_examples/assets/%.o: ../lv_examples/assets/%.c lv_examples/assets/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lv_examples-2f-assets

clean-lv_examples-2f-assets:
	-$(RM) ./lv_examples/assets/img_cogwheel_alpha16.d ./lv_examples/assets/img_cogwheel_alpha16.o ./lv_examples/assets/img_cogwheel_argb.d ./lv_examples/assets/img_cogwheel_argb.o ./lv_examples/assets/img_cogwheel_chroma_keyed.d ./lv_examples/assets/img_cogwheel_chroma_keyed.o ./lv_examples/assets/img_cogwheel_indexed16.d ./lv_examples/assets/img_cogwheel_indexed16.o ./lv_examples/assets/img_cogwheel_rgb.d ./lv_examples/assets/img_cogwheel_rgb.o ./lv_examples/assets/img_hand.d ./lv_examples/assets/img_hand.o ./lv_examples/assets/lv_font_montserrat_12_compr_az.d ./lv_examples/assets/lv_font_montserrat_12_compr_az.o ./lv_examples/assets/lv_font_montserrat_16_compr_az.d ./lv_examples/assets/lv_font_montserrat_16_compr_az.o ./lv_examples/assets/lv_font_montserrat_28_compr_az.d ./lv_examples/assets/lv_font_montserrat_28_compr_az.o

.PHONY: clean-lv_examples-2f-assets

