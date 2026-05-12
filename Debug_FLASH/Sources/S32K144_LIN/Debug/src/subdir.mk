################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/S32K144_LIN/Debug/src/bootloader_debug.c \
../Sources/S32K144_LIN/Debug/src/debug_IO.c \
../Sources/S32K144_LIN/Debug/src/debug_timer.c 

OBJS += \
./Sources/S32K144_LIN/Debug/src/bootloader_debug.o \
./Sources/S32K144_LIN/Debug/src/debug_IO.o \
./Sources/S32K144_LIN/Debug/src/debug_timer.o 

C_DEPS += \
./Sources/S32K144_LIN/Debug/src/bootloader_debug.d \
./Sources/S32K144_LIN/Debug/src/debug_IO.d \
./Sources/S32K144_LIN/Debug/src/debug_timer.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/S32K144_LIN/Debug/src/%.o: ../Sources/S32K144_LIN/Debug/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/S32K144_LIN/Debug/src/bootloader_debug.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


