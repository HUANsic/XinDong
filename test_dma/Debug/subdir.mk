################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CameraShenyan.c \
../CameraUtils.c \
../Cpu0_Main.c \
../Cpu1_Main.c \
../DMADemo.c 

OBJS += \
./CameraShenyan.o \
./CameraUtils.o \
./Cpu0_Main.o \
./Cpu1_Main.o \
./DMADemo.o 

COMPILED_SRCS += \
./CameraShenyan.src \
./CameraUtils.src \
./Cpu0_Main.src \
./Cpu1_Main.src \
./DMADemo.src 

C_DEPS += \
./CameraShenyan.d \
./CameraUtils.d \
./Cpu0_Main.d \
./Cpu1_Main.d \
./DMADemo.d 


# Each subdirectory must supply rules for building sources it contributes
%.src: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fC:/Users/abcde/Documents/ShutDown/Tsinghua/XinDongSystem_HUANsic/test_dma/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

%.o: ./%.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


