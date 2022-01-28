################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/tests/lv_test_assert.c \
../Middlewares/Third_Party/lvgl/tests/lv_test_main.c 

OBJS += \
./Middlewares/Third_Party/lvgl/tests/lv_test_assert.o \
./Middlewares/Third_Party/lvgl/tests/lv_test_main.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/tests/lv_test_assert.d \
./Middlewares/Third_Party/lvgl/tests/lv_test_main.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/tests/lv_test_assert.o: ../Middlewares/Third_Party/lvgl/tests/lv_test_assert.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/tests/lv_test_assert.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/tests/lv_test_main.o: ../Middlewares/Third_Party/lvgl/tests/lv_test_main.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/tests/lv_test_main.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

