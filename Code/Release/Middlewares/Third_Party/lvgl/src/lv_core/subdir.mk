################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_debug.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_disp.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_group.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_indev.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_obj.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_refr.c \
../Middlewares/Third_Party/lvgl/src/lv_core/lv_style.c 

OBJS += \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_debug.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_disp.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_group.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_indev.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_obj.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_refr.o \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_style.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_debug.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_disp.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_group.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_indev.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_obj.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_refr.d \
./Middlewares/Third_Party/lvgl/src/lv_core/lv_style.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/src/lv_core/%.o: ../Middlewares/Third_Party/lvgl/src/lv_core/%.c Middlewares/Third_Party/lvgl/src/lv_core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

