################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/S32K144_LIN/HAL/timer_hal/src/timer_hal.c 

OBJS += \
./Sources/S32K144_LIN/HAL/timer_hal/src/timer_hal.o 

C_DEPS += \
./Sources/S32K144_LIN/HAL/timer_hal/src/timer_hal.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/S32K144_LIN/HAL/timer_hal/src/%.o: ../Sources/S32K144_LIN/HAL/timer_hal/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/S32K144_LIN/HAL/timer_hal/src/timer_hal.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


