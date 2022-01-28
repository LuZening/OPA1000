################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.c \
../Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.c \
../Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.c 

OBJS += \
./Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.o \
./Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.o \
./Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.d \
./Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.d \
./Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.o: ../Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/porting/lv_port_disp_template.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.o: ../Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/porting/lv_port_fs_template.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.o: ../Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/porting/lv_port_indev_template.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

