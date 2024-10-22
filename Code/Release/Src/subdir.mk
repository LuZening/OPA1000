################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/Projects/RADIO/Projects/HF33/MCU/Drivers/ButtonDebouncer.c \
../Src/COM.c \
../Src/COM_task.c \
../Src/FS.c \
../Src/Flash_EEPROM.c \
../Src/I2C_EEPROM.c \
../Src/ICOM_CIV.c \
../Src/Kenwood.c \
../Src/LVGL_GUI.c \
../Src/LVGL_GUI_settings_scene.c \
../Src/LVGL_items.c \
../Src/MultiLinearValueMapper.c \
../Src/NTC.c \
../Src/R61408.c \
../Src/RadioTypes.c \
E:/Projects/RADIO/Projects/HF33/MCU/Drivers/RotEnc.c \
../Src/Sensor_task.c \
../Src/SoftPWMDriver.c \
../Src/SoftUART.c \
../Src/Yaesu.c \
../Src/fan.c \
../Src/freertos.c \
../Src/kfifo.c \
../Src/main.c \
../Src/peak_detector.c \
../Src/persistent.c \
../Src/stm32f4xx_hal_msp.c \
../Src/stm32f4xx_hal_timebase_tim.c \
../Src/stm32f4xx_it.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_stm32f4xx.c \
../Src/touch_HR2046.c 

OBJS += \
./Src/ButtonDebouncer.o \
./Src/COM.o \
./Src/COM_task.o \
./Src/FS.o \
./Src/Flash_EEPROM.o \
./Src/I2C_EEPROM.o \
./Src/ICOM_CIV.o \
./Src/Kenwood.o \
./Src/LVGL_GUI.o \
./Src/LVGL_GUI_settings_scene.o \
./Src/LVGL_items.o \
./Src/MultiLinearValueMapper.o \
./Src/NTC.o \
./Src/R61408.o \
./Src/RadioTypes.o \
./Src/RotEnc.o \
./Src/Sensor_task.o \
./Src/SoftPWMDriver.o \
./Src/SoftUART.o \
./Src/Yaesu.o \
./Src/fan.o \
./Src/freertos.o \
./Src/kfifo.o \
./Src/main.o \
./Src/peak_detector.o \
./Src/persistent.o \
./Src/stm32f4xx_hal_msp.o \
./Src/stm32f4xx_hal_timebase_tim.o \
./Src/stm32f4xx_it.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_stm32f4xx.o \
./Src/touch_HR2046.o 

C_DEPS += \
./Src/ButtonDebouncer.d \
./Src/COM.d \
./Src/COM_task.d \
./Src/FS.d \
./Src/Flash_EEPROM.d \
./Src/I2C_EEPROM.d \
./Src/ICOM_CIV.d \
./Src/Kenwood.d \
./Src/LVGL_GUI.d \
./Src/LVGL_GUI_settings_scene.d \
./Src/LVGL_items.d \
./Src/MultiLinearValueMapper.d \
./Src/NTC.d \
./Src/R61408.d \
./Src/RadioTypes.d \
./Src/RotEnc.d \
./Src/Sensor_task.d \
./Src/SoftPWMDriver.d \
./Src/SoftUART.d \
./Src/Yaesu.d \
./Src/fan.d \
./Src/freertos.d \
./Src/kfifo.d \
./Src/main.d \
./Src/peak_detector.d \
./Src/persistent.d \
./Src/stm32f4xx_hal_msp.d \
./Src/stm32f4xx_hal_timebase_tim.d \
./Src/stm32f4xx_it.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_stm32f4xx.d \
./Src/touch_HR2046.d 


# Each subdirectory must supply rules for building sources it contributes
Src/ButtonDebouncer.o: E:/Projects/RADIO/Projects/HF33/MCU/Drivers/ButtonDebouncer.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/%.o: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/RotEnc.o: E:/Projects/RADIO/Projects/HF33/MCU/Drivers/RotEnc.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

