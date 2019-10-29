################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/dev/Dropbox/PROJECTS/LIBRARY/MPU6050/lib/MPU6050/Source/mpu6050.c 

OBJS += \
./Source/mpu6050.o 

C_DEPS += \
./Source/mpu6050.d 


# Each subdirectory must supply rules for building sources it contributes
Source/mpu6050.o: D:/dev/Dropbox/PROJECTS/LIBRARY/MPU6050/lib/MPU6050/Source/mpu6050.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"D:/dev/Dropbox/PROJECTS/LIBRARY/MPU6050/lib/MPU6050/Include" -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Source/mpu6050.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

