################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
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
Middlewares/Third_Party/lvgl/src/lv_draw/%.o: ../Middlewares/Third_Party/lvgl/src/lv_draw/%.c Middlewares/Third_Party/lvgl/src/lv_draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -DOPA1000 -DLV_LVGL_H_INCLUDE_SIMPLE -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/lvgl -I"E:/Projects/RADIO/Projects/HF33/MCU/Drivers" -O2 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

