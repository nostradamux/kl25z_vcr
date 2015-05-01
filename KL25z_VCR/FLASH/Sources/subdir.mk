################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/Application.c" \
"../Sources/DebugSerial.c" \
"../Sources/Events.c" \
"../Sources/main.c" \
"../Sources/nRF24L01.c" \
"../Sources/nRF24L01_application.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/Application.c \
../Sources/DebugSerial.c \
../Sources/Events.c \
../Sources/main.c \
../Sources/nRF24L01.c \
../Sources/nRF24L01_application.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/Application.o \
./Sources/DebugSerial.o \
./Sources/Events.o \
./Sources/main.o \
./Sources/nRF24L01.o \
./Sources/nRF24L01_application.o \
./Sources/sa_mtb.o \

OBJS_QUOTED += \
"./Sources/Application.o" \
"./Sources/DebugSerial.o" \
"./Sources/Events.o" \
"./Sources/main.o" \
"./Sources/nRF24L01.o" \
"./Sources/nRF24L01_application.o" \
"./Sources/sa_mtb.o" \

C_DEPS += \
./Sources/Application.d \
./Sources/DebugSerial.d \
./Sources/Events.d \
./Sources/main.d \
./Sources/nRF24L01.d \
./Sources/nRF24L01_application.d \
./Sources/sa_mtb.d \

OBJS_OS_FORMAT += \
./Sources/Application.o \
./Sources/DebugSerial.o \
./Sources/Events.o \
./Sources/main.o \
./Sources/nRF24L01.o \
./Sources/nRF24L01_application.o \
./Sources/sa_mtb.o \

C_DEPS_QUOTED += \
"./Sources/Application.d" \
"./Sources/DebugSerial.d" \
"./Sources/Events.d" \
"./Sources/main.d" \
"./Sources/nRF24L01.d" \
"./Sources/nRF24L01_application.d" \
"./Sources/sa_mtb.d" \


# Each subdirectory must supply rules for building sources it contributes
Sources/Application.o: ../Sources/Application.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Application.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Application.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/DebugSerial.o: ../Sources/DebugSerial.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/DebugSerial.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/DebugSerial.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Events.o: ../Sources/Events.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Events.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Events.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nRF24L01.o: ../Sources/nRF24L01.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nRF24L01.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nRF24L01.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/nRF24L01_application.o: ../Sources/nRF24L01_application.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/nRF24L01_application.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/nRF24L01_application.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


