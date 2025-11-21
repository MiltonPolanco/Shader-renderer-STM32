################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SolarSystem/camera.c \
../SolarSystem/celestial_body.c \
../SolarSystem/planet_shader.c \
../SolarSystem/solar_system.c 

C_DEPS += \
./SolarSystem/camera.d \
./SolarSystem/celestial_body.d \
./SolarSystem/planet_shader.d \
./SolarSystem/solar_system.d 

OBJS += \
./SolarSystem/camera.o \
./SolarSystem/celestial_body.o \
./SolarSystem/planet_shader.o \
./SolarSystem/solar_system.o 


# Each subdirectory must supply rules for building sources it contributes
SolarSystem/%.o SolarSystem/%.su SolarSystem/%.cyclo: ../SolarSystem/%.c SolarSystem/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-SolarSystem

clean-SolarSystem:
	-$(RM) ./SolarSystem/camera.cyclo ./SolarSystem/camera.d ./SolarSystem/camera.o ./SolarSystem/camera.su ./SolarSystem/celestial_body.cyclo ./SolarSystem/celestial_body.d ./SolarSystem/celestial_body.o ./SolarSystem/celestial_body.su ./SolarSystem/planet_shader.cyclo ./SolarSystem/planet_shader.d ./SolarSystem/planet_shader.o ./SolarSystem/planet_shader.su ./SolarSystem/solar_system.cyclo ./SolarSystem/solar_system.d ./SolarSystem/solar_system.o ./SolarSystem/solar_system.su

.PHONY: clean-SolarSystem

