################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Libraries/iLLD/TC26B/Tricore/_PinMap/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/_Lib/DataHandling/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/_Impl/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Stm/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Src/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Scu/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Port/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Mtu/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/I2c/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/I2c/I2c/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Gtm/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Dma/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Dma/Dma/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Cpu/Trap/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Cpu/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Cpu/Irq/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Cpu/CStart/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Ccu6/TPwm/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Ccu6/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Asclin/Std/subdir.mk
-include Libraries/iLLD/TC26B/Tricore/Asclin/Asc/subdir.mk
-include Libraries/Service/CpuGeneric/SysSe/Bsp/subdir.mk
-include Libraries/Service/CpuGeneric/StdIf/subdir.mk
-include Libraries/Service/CpuGeneric/If/subdir.mk
-include Libraries/Infra/Platform/Tricore/Compilers/subdir.mk
-include Libraries/subdir.mk
-include Configurations/Debug/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := test_dma
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_SIZE += \
test_dma.siz \


# All Target
all: main-build

# Main-build Target
main-build: test_dma.elf secondary-outputs

# Tool invocations
test_dma.elf: $(OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: TASKING Linker'
	cctc -lrt -lfp_fpu -lcs_fpu  -Wl-Oc -Wl-OL -Wl-Ot -Wl-Ox -Wl-Oy -Wl--map-file="test_dma.map" -Wl-mc -Wl-mf -Wl-mi -Wl-mk -Wl-ml -Wl-mm -Wl-md -Wl-mr -Wl-mu --no-warnings= -Wl--error-limit=42 --exceptions --strict --anachronisms --force-c++ -Ctc26xb   -o"test_dma.elf" -Wl-o"test_dma.hex:IHEX" --lsl-core=vtc  --lsl-file=../Lcf_Tasking_Tricore_Tc.lsl $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

test_dma.siz: test_dma.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	elfsize  "test_dma.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	find . ! -name "*.mk" ! -name "makefile" ! -name "." -type f -delete 
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
