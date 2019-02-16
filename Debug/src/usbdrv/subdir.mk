################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../src/usbdrv/usbdrvasm.asm 

C_SRCS += \
../src/usbdrv/oddebug.c \
../src/usbdrv/usbdrv.c 

S_UPPER_SRCS += \
../src/usbdrv/usbdrvasm.S 

OBJS += \
./src/usbdrv/oddebug.o \
./src/usbdrv/usbdrv.o \
./src/usbdrv/usbdrvasm.o 

ASM_DEPS += \
./src/usbdrv/usbdrvasm.d 

S_UPPER_DEPS += \
./src/usbdrv/usbdrvasm.d 

C_DEPS += \
./src/usbdrv/oddebug.d \
./src/usbdrv/usbdrv.d 


# Each subdirectory must supply rules for building sources it contributes
src/usbdrv/%.o: ../src/usbdrv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I/home/demon/workspace/avr-midi05/src/usbdrv -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/usbdrv/%.o: ../src/usbdrv/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/usbdrv/%.o: ../src/usbdrv/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -g2 -gstabs -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


