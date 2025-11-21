################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Touch/touch_xpt2046.c 

C_DEPS += \
./Drivers/Touch/touch_xpt2046.d 

OBJS += \
./Drivers/Touch/touch_xpt2046.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Touch/%.o Drivers/Touch/%.su Drivers/Touch/%.cyclo: ../Drivers/Touch/%.c Drivers/Touch/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Touch

clean-Drivers-2f-Touch:
	-$(RM) ./Drivers/Touch/touch_xpt2046.cyclo ./Drivers/Touch/touch_xpt2046.d ./Drivers/Touch/touch_xpt2046.o ./Drivers/Touch/touch_xpt2046.su

.PHONY: clean-Drivers-2f-Touch

