################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Startup/system_XMC4200.c 

S_UPPER_SRCS += \
../Startup/startup_XMC4200.S 

OBJS += \
./Startup/startup_XMC4200.o \
./Startup/system_XMC4200.o 

S_UPPER_DEPS += \
./Startup/startup_XMC4200.d 

C_DEPS += \
./Startup/system_XMC4200.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC Assembler'
	"C:\DAVEv4-64bit\DAVE-4.2.6\eclipse\ARM-GCC-49/bin/arm-none-eabi-gcc" -MMD -MT "$@" -x assembler-with-cpp -DXMC4200_Q48x256 -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Libraries\XMCLib\inc" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Dave\Generated" -Wall -Wa,-adhlns="$@.lst" -mfloat-abi=soft -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.
Startup/%.o: ../Startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"C:\DAVEv4-64bit\DAVE-4.2.6\eclipse\ARM-GCC-49/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4200_Q48x256 -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Libraries\XMCLib\inc" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core/Libraries/CMSIS/Include" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core/Libraries/CMSIS/Infineon/XMC4200_series/Include" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Dave\Generated" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=soft -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

