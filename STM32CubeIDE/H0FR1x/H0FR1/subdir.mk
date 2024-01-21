################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
D:/Hexabitz\ release/H0FR1x/H0FR1/startup_stm32f091xc.s 

C_SRCS += \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_adc.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_dma.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_eeprom.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_gpio.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_inputs.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_it.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_rtc.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_timers.c \
D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_uart.c 

OBJS += \
./H0FR1/H0FR1.o \
./H0FR1/H0FR1_adc.o \
./H0FR1/H0FR1_dma.o \
./H0FR1/H0FR1_eeprom.o \
./H0FR1/H0FR1_gpio.o \
./H0FR1/H0FR1_inputs.o \
./H0FR1/H0FR1_it.o \
./H0FR1/H0FR1_rtc.o \
./H0FR1/H0FR1_timers.o \
./H0FR1/H0FR1_uart.o \
./H0FR1/startup_stm32f091xc.o 

S_DEPS += \
./H0FR1/startup_stm32f091xc.d 

C_DEPS += \
./H0FR1/H0FR1.d \
./H0FR1/H0FR1_adc.d \
./H0FR1/H0FR1_dma.d \
./H0FR1/H0FR1_eeprom.d \
./H0FR1/H0FR1_gpio.d \
./H0FR1/H0FR1_inputs.d \
./H0FR1/H0FR1_it.d \
./H0FR1/H0FR1_rtc.d \
./H0FR1/H0FR1_timers.d \
./H0FR1/H0FR1_uart.d 


# Each subdirectory must supply rules for building sources it contributes
H0FR1/H0FR1.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_adc.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_adc.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_adc.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_dma.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_dma.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_dma.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_eeprom.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_eeprom.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_eeprom.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_gpio.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_gpio.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_gpio.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_inputs.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_inputs.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_inputs.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_it.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_it.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_it.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_rtc.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_rtc.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_rtc.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_timers.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_timers.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_timers.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/H0FR1_uart.o: D:/Hexabitz\ release/H0FR1x/H0FR1/H0FR1_uart.c H0FR1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=c99 -g -DUSE_HAL_DRIVER -DDEBUG -DSTM32F091xC -D_module=1 -DH0FR1 -c -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc/Legacy -I../../Thirdparty/STM32F0xx_HAL_Driver/Inc -I../../BOS -I../../User -I../../H0FR1 -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/CMSIS_RTOS -I../../Thirdparty/CMSIS/STM32F0xx/Include -I../../Thirdparty/CMSIS/STM32F0xx/Device/ST/STM32F0xx/Include -I../../Thirdparty/Middleware/STM32F0xx/FreeRTOS/Source/portable/GCC/ARM_CM0 -O2 -ffunction-sections -fstack-usage -MMD -MP -MF"H0FR1/H0FR1_uart.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"
H0FR1/startup_stm32f091xc.o: D:/Hexabitz\ release/H0FR1x/H0FR1/startup_stm32f091xc.s H0FR1/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0 -g3 -c -x assembler-with-cpp -MMD -MP -MF"H0FR1/startup_stm32f091xc.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-H0FR1

clean-H0FR1:
	-$(RM) ./H0FR1/H0FR1.d ./H0FR1/H0FR1.o ./H0FR1/H0FR1.su ./H0FR1/H0FR1_adc.d ./H0FR1/H0FR1_adc.o ./H0FR1/H0FR1_adc.su ./H0FR1/H0FR1_dma.d ./H0FR1/H0FR1_dma.o ./H0FR1/H0FR1_dma.su ./H0FR1/H0FR1_eeprom.d ./H0FR1/H0FR1_eeprom.o ./H0FR1/H0FR1_eeprom.su ./H0FR1/H0FR1_gpio.d ./H0FR1/H0FR1_gpio.o ./H0FR1/H0FR1_gpio.su ./H0FR1/H0FR1_inputs.d ./H0FR1/H0FR1_inputs.o ./H0FR1/H0FR1_inputs.su ./H0FR1/H0FR1_it.d ./H0FR1/H0FR1_it.o ./H0FR1/H0FR1_it.su ./H0FR1/H0FR1_rtc.d ./H0FR1/H0FR1_rtc.o ./H0FR1/H0FR1_rtc.su ./H0FR1/H0FR1_timers.d ./H0FR1/H0FR1_timers.o ./H0FR1/H0FR1_timers.su ./H0FR1/H0FR1_uart.d ./H0FR1/H0FR1_uart.o ./H0FR1/H0FR1_uart.su ./H0FR1/startup_stm32f091xc.d ./H0FR1/startup_stm32f091xc.o

.PHONY: clean-H0FR1

