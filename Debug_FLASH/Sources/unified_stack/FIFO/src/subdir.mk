################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sources/unified_stack/FIFO/src/multi_cyc_fifo.c 

OBJS += \
./Sources/unified_stack/FIFO/src/multi_cyc_fifo.o 

C_DEPS += \
./Sources/unified_stack/FIFO/src/multi_cyc_fifo.d 


# Each subdirectory must supply rules for building sources it contributes
Sources/unified_stack/FIFO/src/%.o: ../Sources/unified_stack/FIFO/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@Sources/unified_stack/FIFO/src/multi_cyc_fifo.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


