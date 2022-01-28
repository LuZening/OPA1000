################################################################################
# Automatically-generated file. Do not edit!
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
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
OPA1000_controller.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
OPA1000_controller.list \

OBJCOPY_HEX += \
OPA1000_controller.hex \


# All Target
all: OPA1000_controller.elf secondary-outputs

# Tool invocations
OPA1000_controller.elf: $(OBJS) $(USER_OBJS) C:\Users\Zening\OneDrive\RADIO\Projects\OPA1000\Code\STM32F407VETX_FLASH.ld
	arm-none-eabi-gcc -o "OPA1000_controller.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Zening\OneDrive\RADIO\Projects\OPA1000\Code\STM32F407VETX_FLASH.ld" --specs=nosys.specs -Wl,-Map="OPA1000_controller.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

OPA1000_controller.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "OPA1000_controller.list"
	@echo 'Finished building: $@'
	@echo ' '

OPA1000_controller.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "OPA1000_controller.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
