################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/ADC_QUEUE/adc_queue.c \
../Dave/Generated/ADC_QUEUE/adc_queue_conf.c 

OBJS += \
./Dave/Generated/ADC_QUEUE/adc_queue.o \
./Dave/Generated/ADC_QUEUE/adc_queue_conf.o 

C_DEPS += \
./Dave/Generated/ADC_QUEUE/adc_queue.d \
./Dave/Generated/ADC_QUEUE/adc_queue_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/ADC_QUEUE/%.o: ../Dave/Generated/ADC_QUEUE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"C:\DAVEv4\DAVE-4.1.2\eclipse\ARM-GCC-49/bin/arm-none-eabi-gcc" -DXMC4200_Q48x256 -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003\Libraries\XMCLib\inc" -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003/Libraries/CMSIS/Include" -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003/Libraries/CMSIS/Infineon/XMC4200_series/Include" -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003" -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003\Dave\Generated" -I"C:\Workspaces\DAVE-4.1\S2G2_MD_SA_W002\S2GTWO_MD_SA_T003\Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo.

