################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UserCode/Src/animation.cpp \
../UserCode/Src/event_handler.cpp \
../UserCode/Src/generator.cpp \
../UserCode/Src/pcb_status_led.cpp \
../UserCode/Src/rng_handler.cpp \
../UserCode/Src/transmitter.cpp 

OBJS += \
./UserCode/Src/animation.o \
./UserCode/Src/event_handler.o \
./UserCode/Src/generator.o \
./UserCode/Src/pcb_status_led.o \
./UserCode/Src/rng_handler.o \
./UserCode/Src/transmitter.o 

CPP_DEPS += \
./UserCode/Src/animation.d \
./UserCode/Src/event_handler.d \
./UserCode/Src/generator.d \
./UserCode/Src/pcb_status_led.d \
./UserCode/Src/rng_handler.d \
./UserCode/Src/transmitter.d 


# Each subdirectory must supply rules for building sources it contributes
UserCode/Src/animation.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/animation.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UserCode/Src/event_handler.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/event_handler.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UserCode/Src/generator.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/generator.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UserCode/Src/pcb_status_led.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/pcb_status_led.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UserCode/Src/rng_handler.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/rng_handler.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
UserCode/Src/transmitter.o: /home/xaverius/Desktop/projekt/random/UserCode/Src/transmitter.cpp UserCode/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/home/xaverius/Desktop/projekt/random/UserCode/Inc" -Ofast -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-UserCode-2f-Src

clean-UserCode-2f-Src:
	-$(RM) ./UserCode/Src/animation.d ./UserCode/Src/animation.o ./UserCode/Src/event_handler.d ./UserCode/Src/event_handler.o ./UserCode/Src/generator.d ./UserCode/Src/generator.o ./UserCode/Src/pcb_status_led.d ./UserCode/Src/pcb_status_led.o ./UserCode/Src/rng_handler.d ./UserCode/Src/rng_handler.o ./UserCode/Src/transmitter.d ./UserCode/Src/transmitter.o

.PHONY: clean-UserCode-2f-Src

