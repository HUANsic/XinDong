################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../AsclinApp.c \
../CameraShenyan.c \
../CameraUtils.c \
../Cpu0_Main.c \
../Cpu1_Main.c \
../Mpu6050.c \
../PwmDemo.c \
../ServeSource.c \
../StmDemo.c \
../TimDemo.c \
../UserSource.c 

OBJS += \
./AsclinApp.o \
./CameraShenyan.o \
./CameraUtils.o \
./Cpu0_Main.o \
./Cpu1_Main.o \
./Mpu6050.o \
./PwmDemo.o \
./ServeSource.o \
./StmDemo.o \
./TimDemo.o \
./UserSource.o 

COMPILED_SRCS += \
./AsclinApp.src \
./CameraShenyan.src \
./CameraUtils.src \
./Cpu0_Main.src \
./Cpu1_Main.src \
./Mpu6050.src \
./PwmDemo.src \
./ServeSource.src \
./StmDemo.src \
./TimDemo.src \
./UserSource.src 

C_DEPS += \
./AsclinApp.d \
./CameraShenyan.d \
./CameraUtils.d \
./Cpu0_Main.d \
./Cpu1_Main.d \
./Mpu6050.d \
./PwmDemo.d \
./ServeSource.d \
./StmDemo.d \
./TimDemo.d \
./UserSource.d 


# Each subdirectory must supply rules for building sources it contributes
%.src: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fC:/Users/m1830/Desktop/Tests/test_dma01/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

%.o: ./%.src subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


