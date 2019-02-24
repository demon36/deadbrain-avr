################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/ctrl.c \
../src/dsp.c \
../src/main.c \
../src/midi.c \
../src/timer.c \
../src/uart.c \
../src/usb.c 

OBJS += \
./src/adc.o \
./src/ctrl.o \
./src/dsp.o \
./src/main.o \
./src/midi.o \
./src/timer.o \
./src/uart.o \
./src/usb.o 

C_DEPS += \
./src/adc.d \
./src/ctrl.d \
./src/dsp.d \
./src/main.d \
./src/midi.d \
./src/timer.d \
./src/uart.d \
./src/usb.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/demon/cpp_projects/deadbrain-avr/vendor" -I/usr/lib/avr/include -I"/home/demon/cpp_projects/deadbrain-avr/include" -Wall -Werror -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=20000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


