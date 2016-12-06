################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ADC_MEASUREMENT/adc_measurement.c \
../Dave/Generated/ADC_MEASUREMENT/adc_measurement_conf.c 

OBJS += \
./Dave/Generated/ADC_MEASUREMENT/adc_measurement.o \
./Dave/Generated/ADC_MEASUREMENT/adc_measurement_conf.o 

C_DEPS += \
./Dave/Generated/ADC_MEASUREMENT/adc_measurement.d \
./Dave/Generated/ADC_MEASUREMENT/adc_measurement_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ADC_MEASUREMENT/%.o: ../Dave/Generated/ADC_MEASUREMENT/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC1302_T016x0016 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/RadarSense2Go/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC1300_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -I"$(PROJECT_LOC)/Libraries/RadarSense2Go/bin" -O2 -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m0 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

