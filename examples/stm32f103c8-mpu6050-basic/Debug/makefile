################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/subdir.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32f103c8-mpu6050-basic.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32f103c8-mpu6050-basic.list \


# All Target
all: stm32f103c8-mpu6050-basic.elf secondary-outputs

# Tool invocations
stm32f103c8-mpu6050-basic.elf: $(OBJS) $(USER_OBJS) D:\dev\Dropbox\PROJECTS\LIBRARY\MPU6050\examples\stm32f103c8-mpu6050-basic\STM32F103C8TX_FLASH.ld
	arm-none-eabi-gcc -o "stm32f103c8-mpu6050-basic.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\dev\Dropbox\PROJECTS\LIBRARY\MPU6050\examples\stm32f103c8-mpu6050-basic\STM32F103C8TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32f103c8-mpu6050-basic.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32f103c8-mpu6050-basic.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32f103c8-mpu6050-basic.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
