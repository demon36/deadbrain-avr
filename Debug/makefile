################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/usbdrv/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
deadbrain-avr.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: deadbrain-avr.elf secondary-outputs

# Tool invocations
deadbrain-avr.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,deadbrain-avr.map -mmcu=atmega328p -o "deadbrain-avr.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

deadbrain-avr.lss: deadbrain-avr.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S deadbrain-avr.elf  >"deadbrain-avr.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: deadbrain-avr.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p deadbrain-avr.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) deadbrain-avr.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
