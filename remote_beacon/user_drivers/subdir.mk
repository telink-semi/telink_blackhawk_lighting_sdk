################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user_drivers/fm24c02_drv.c \
../user_drivers/i2c_drv.c 

OBJS += \
./user_drivers/fm24c02_drv.o \
./user_drivers/i2c_drv.o 


# Each subdirectory must supply rules for building sources it contributes
user_drivers/%.o: ../user_drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"D:\Telink\1Firmware\telink_blackhawk_lighting_sdk" -D__PROJECT_REMOTE_BEACON__=1 -DCHIP_TYPE=CHIP_TYPE_8231 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


