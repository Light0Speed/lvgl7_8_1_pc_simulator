################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include lvgl/tests/lv_test_widgets/subdir.mk
-include lvgl/tests/lv_test_fonts/subdir.mk
-include lvgl/tests/lv_test_core/subdir.mk
-include lvgl/tests/subdir.mk
-include lvgl/src/lv_widgets/subdir.mk
-include lvgl/src/lv_themes/subdir.mk
-include lvgl/src/lv_misc/subdir.mk
-include lvgl/src/lv_hal/subdir.mk
-include lvgl/src/lv_gpu/subdir.mk
-include lvgl/src/lv_font/subdir.mk
-include lvgl/src/lv_draw/subdir.mk
-include lvgl/src/lv_core/subdir.mk
-include lvgl/examples/porting/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_win/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_tileview/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_textarea/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_tabview/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_table/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_switch/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_spinner/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_spinbox/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_slider/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_roller/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_page/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_objmask/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_obj/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_msgbox/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_list/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_linemeter/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_line/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_led/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_label/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_keyboard/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_imgbtn/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_img/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_gauge/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_dropdown/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_cpicker/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_cont/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_checkbox/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_chart/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_canvas/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_calendar/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_btnmatrix/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_btn/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_bar/subdir.mk
-include lv_examples/src/lv_ex_widgets/lv_ex_arc/subdir.mk
-include lv_examples/src/lv_ex_style/subdir.mk
-include lv_examples/src/lv_ex_get_started/subdir.mk
-include lv_examples/src/lv_demo_widgets/subdir.mk
-include lv_examples/src/lv_demo_stress/subdir.mk
-include lv_examples/src/lv_demo_printer/images/subdir.mk
-include lv_examples/src/lv_demo_printer/subdir.mk
-include lv_examples/src/lv_demo_keypad_encoder/subdir.mk
-include lv_examples/src/lv_demo_benchmark/subdir.mk
-include lv_examples/assets/subdir.mk
-include lv_drivers/indev/subdir.mk
-include lv_drivers/gtkdrv/subdir.mk
-include lv_drivers/display/subdir.mk
-include lv_drivers/subdir.mk
-include subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := lv_sim_eclipse_sdl
BUILD_ARTIFACT_EXTENSION :=
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: main-build

# Main-build Target
main-build: lv_sim_eclipse_sdl

# Tool invocations
lv_sim_eclipse_sdl: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	gcc  -o "lv_sim_eclipse_sdl" $(OBJS) $(USER_OBJS) $(LIBS) -lSDL2main -lSDL2
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) lv_sim_eclipse_sdl
	-@echo ' '

.PHONY: all clean dependents main-build

-include ../makefile.targets
