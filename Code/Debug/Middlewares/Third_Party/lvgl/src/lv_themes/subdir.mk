################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme.c \
../Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_empty.c \
../Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_material.c \
../Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_mono.c \
../Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_template.c 

OBJS += \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme.o \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_empty.o \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_material.o \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_mono.o \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_template.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme.d \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_empty.d \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_material.d \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_mono.d \
./Middlewares/Third_Party/lvgl/src/lv_themes/lv_theme_template.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/src/lv_themes/%.o: ../Middlewares/Third_Party/lvgl/src/lv_themes/%.c Middlewares/Third_Party/lvgl/src/lv_themes/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

