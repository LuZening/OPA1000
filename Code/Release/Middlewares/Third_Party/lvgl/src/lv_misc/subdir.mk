################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.c \
../Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.c 

OBJS += \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.o \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.o 

C_DEPS += \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.d \
./Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_anim.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_area.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_async.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_bidi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_color.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_fs.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_gc.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_ll.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_log.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_math.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_mem.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_printf.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_task.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_templ.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_txt_ap.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.o: ../Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 '-D__FPU_USED=1' -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/Third_Party/lvgl/src/lv_misc/lv_utils.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

