################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f407vetx.s 

OBJS += \
./Startup/startup_stm32f407vetx.o 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -c -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Middlewares/Third_Party/lvgl" -I"C:/Users/Zening/OneDrive/RADIO/Projects/OPA1000/Code/Inc" -x assembler-with-cpp --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

