################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/remote/subdir.mk
-include user_drivers/subdir.mk
-include drivers/8231/subdir.mk
-include boot/8231/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
remote.lst \

FLASH_IMAGE += \
remote.bin \

SIZEDUMMY += \
sizedummy \


# All Target
all: telink_blackhawk_lighting_sdk.elf secondary-outputs

# Tool invocations
telink_blackhawk_lighting_sdk.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"D:\Telink\1Firmware\telink_blackhawk_lighting_sdk\drivers\8231" -T ../boot.link -o"telink_blackhawk_lighting_sdk.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

remote.lst: telink_blackhawk_lighting_sdk.elf
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S telink_blackhawk_lighting_sdk.elf  >"remote.lst"
	@echo 'Finished building: $@'
	@echo ' '

remote.bin: telink_blackhawk_lighting_sdk.elf
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy -O binary telink_blackhawk_lighting_sdk.elf  "remote.bin"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: telink_blackhawk_lighting_sdk.elf
	@echo 'Invoking: Print Size'
	tc32-elf-size -t telink_blackhawk_lighting_sdk.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) telink_blackhawk_lighting_sdk.elf
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
