################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.c \
../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.c 

OBJS += \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.o \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.d \
./Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_arc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_blend.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_img.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_label.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_line.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_mask.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_rect.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_draw_triangle.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_buf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_cache.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_draw/lv_img_decoder.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

