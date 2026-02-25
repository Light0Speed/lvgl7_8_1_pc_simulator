################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lvgl/src/lv_gpu/lv_gpu_nxp_pxp.c \
../lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.c \
../lvgl/src/lv_gpu/lv_gpu_nxp_vglite.c \
../lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.c 

C_DEPS += \
./lvgl/src/lv_gpu/lv_gpu_nxp_pxp.d \
./lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.d \
./lvgl/src/lv_gpu/lv_gpu_nxp_vglite.d \
./lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.d 

OBJS += \
./lvgl/src/lv_gpu/lv_gpu_nxp_pxp.o \
./lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.o \
./lvgl/src/lv_gpu/lv_gpu_nxp_vglite.o \
./lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.o 


# Each subdirectory must supply rules for building sources it contributes
lvgl/src/lv_gpu/%.o: ../lvgl/src/lv_gpu/%.c lvgl/src/lv_gpu/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -I"E:\simware\lvgl_simluator\lv_port_pc_eclipse-7.8.1" -O0 -g3 -Wall -c -fmessage-length=0 -Wall -Wmaybe-uninitialized -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-lvgl-2f-src-2f-lv_gpu

clean-lvgl-2f-src-2f-lv_gpu:
	-$(RM) ./lvgl/src/lv_gpu/lv_gpu_nxp_pxp.d ./lvgl/src/lv_gpu/lv_gpu_nxp_pxp.o ./lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.d ./lvgl/src/lv_gpu/lv_gpu_nxp_pxp_osa.o ./lvgl/src/lv_gpu/lv_gpu_nxp_vglite.d ./lvgl/src/lv_gpu/lv_gpu_nxp_vglite.o ./lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.d ./lvgl/src/lv_gpu/lv_gpu_stm32_dma2d.o

.PHONY: clean-lvgl-2f-src-2f-lv_gpu

