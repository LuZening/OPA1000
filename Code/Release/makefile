################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/lvgl/tests/lv_test_objx/subdir.mk
-include Middlewares/Third_Party/lvgl/tests/lv_test_core/subdir.mk
-include Middlewares/Third_Party/lvgl/tests/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_widgets/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_themes/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_misc/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_hal/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_gpu/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_font/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_draw/subdir.mk
-include Middlewares/Third_Party/lvgl/src/lv_core/subdir.mk
-include Middlewares/Third_Party/lvgl/porting/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := OPA2000_controller
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
OPA2000_controller.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
OPA2000_controller.list \

OBJCOPY_HEX += \
OPA2000_controller.hex \

OBJCOPY_BIN += \
OPA2000_controller.bin \


# All Target
all: main-build

# Main-build Target
main-build: OPA2000_controller.elf secondary-outputs

# Tool invocations
OPA2000_controller.elf: $(OBJS) $(USER_OBJS) E:\Projects\RADIO\Projects\OPA1000\Code\STM32F407VETX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "OPA2000_controller.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"E:\Projects\RADIO\Projects\OPA1000\Code\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="OPA2000_controller.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

OPA2000_controller.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "OPA2000_controller.list"
	@echo 'Finished building: $@'
	@echo ' '

OPA2000_controller.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "OPA2000_controller.hex"
	@echo 'Finished building: $@'
	@echo ' '

OPA2000_controller.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "OPA2000_controller.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJCOPY_HEX)$(SIZE_OUTPUT)$(OBJDUMP_LIST)$(EXECUTABLES)$(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(OBJCOPY_BIN) OPA2000_controller.elf
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
