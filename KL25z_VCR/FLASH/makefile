################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 43
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := "$(GNU_Make_Install_DirEnv)/rm" -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Sources/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include Generated_Code/subdir.mk
-include .Analysis\ Data/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
VCR_v1_0.elf \

EXECUTABLES_QUOTED += \
"VCR_v1_0.elf" \

EXECUTABLES_OS_FORMAT += \
VCR_v1_0.elf \

SECONDARY_FLASH += \
VCR_v1_0.hex \

SECONDARY_FLASH_QUOTED += \
"VCR_v1_0.hex" \

SECONDARY_FLASH_OS_FORMAT += \
VCR_v1_0.hex \

SECONDARY_SIZE += \
VCR_v1_0.siz \

SECONDARY_SIZE_QUOTED += \
"VCR_v1_0.siz" \

SECONDARY_SIZE_OS_FORMAT += \
VCR_v1_0.siz \


# All Target
all: warning-messages explicit-dependencies VCR_v1_0.elf secondary-outputs

# Tool invocations
VCR_v1_0.elf: $(OBJS) $(USER_OBJS) ../Project_Settings/Linker_Files/ProcessorExpert.ld
	@echo 'Building target: $@'
	@echo 'Executing target #44 $@'
	@echo 'Invoking: ARM Ltd Windows GCC C Linker'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED) @"VCR_v1_0.args" -o"VCR_v1_0.elf"
	@echo 'Finished building target: $@'
	@echo ' '

VCR_v1_0.hex: VCR_v1_0.elf
	@echo 'Executing target #45 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Create Flash Image'
	"$(ARMSourceryDirEnv)/arm-none-eabi-objcopy"  -O srec VCR_v1_0.elf "VCR_v1_0.hex"
	@echo 'Finished building: $@'
	@echo ' '

VCR_v1_0.siz: VCR_v1_0.elf
	@echo 'Executing target #46 $@'
	@echo 'Invoking: ARM Ltd Windows GNU Print Size'
	"$(ARMSourceryDirEnv)/arm-none-eabi-size"  --format=berkeley VCR_v1_0.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(S_DEPS_QUOTED) $(SECONDARY_FLASH_QUOTED) "./*/*.o" "./*/*/*.o"  $(S_UPPER_DEPS_QUOTED) $(ASM_DEPS_QUOTED) $(SECONDARY_SIZE_QUOTED) $(ASM_UPPER_DEPS_QUOTED) $(SX_DEPS_QUOTED) $(EXECUTABLES_QUOTED) "./*/*.d" "./*/*/*.d"  $(ProjDirPath)/FLASH/VCR_v1_0.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: ../Project_Settings/Linker_Files/ProcessorExpert.ld VCR_v1_0.elf VCR_v1_0.elf 

warning-messages: 

