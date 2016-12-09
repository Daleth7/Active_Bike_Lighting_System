################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_f32.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q15.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q31.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_f32.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q15.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q31.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_f32.c \
../Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_q31.c 

OBJS += \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_f32.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q15.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q31.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_f32.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q15.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q31.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_f32.o \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_q31.o 

C_DEPS += \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_f32.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q15.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_init_q31.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_f32.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q15.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_pid_reset_q31.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_f32.d \
./Dave/Generated/CMSIS_DSP/ControllerFunctions/arm_sin_cos_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/CMSIS_DSP/ControllerFunctions/%.o: ../Dave/Generated/CMSIS_DSP/ControllerFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"C:\DAVEv4-64bit\DAVE-4.2.6\eclipse\ARM-GCC-49/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4200_Q48x256 -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Libraries\XMCLib\inc" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core/Libraries/CMSIS/Include" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core/Libraries/CMSIS/Infineon/XMC4200_series/Include" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Dave\Generated" -I"C:\Users\Depes Crystalline\Desktop\school\Active_Bike_Lighting_System\Radar_Core\Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=soft -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

