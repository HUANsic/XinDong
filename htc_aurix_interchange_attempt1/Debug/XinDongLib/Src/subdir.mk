################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../XinDongLib/Src/HUANsic_Camera.c \
../XinDongLib/Src/HUANsic_GPIO.c \
../XinDongLib/Src/HUANsic_IT.c \
../XinDongLib/Src/HUANsic_Timers.c \
../XinDongLib/Src/HUANsic_UART1_BLE.c 

OBJS += \
./XinDongLib/Src/HUANsic_Camera.o \
./XinDongLib/Src/HUANsic_GPIO.o \
./XinDongLib/Src/HUANsic_IT.o \
./XinDongLib/Src/HUANsic_Timers.o \
./XinDongLib/Src/HUANsic_UART1_BLE.o 

COMPILED_SRCS += \
./XinDongLib/Src/HUANsic_Camera.src \
./XinDongLib/Src/HUANsic_GPIO.src \
./XinDongLib/Src/HUANsic_IT.src \
./XinDongLib/Src/HUANsic_Timers.src \
./XinDongLib/Src/HUANsic_UART1_BLE.src 

C_DEPS += \
./XinDongLib/Src/HUANsic_Camera.d \
./XinDongLib/Src/HUANsic_GPIO.d \
./XinDongLib/Src/HUANsic_IT.d \
./XinDongLib/Src/HUANsic_Timers.d \
./XinDongLib/Src/HUANsic_UART1_BLE.d 


# Each subdirectory must supply rules for building sources it contributes
XinDongLib/Src/%.src: ../XinDongLib/Src/%.c XinDongLib/Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fC:/Users/m1830/Documents/ZonghuanWu/XinDong/XinDong/htc_aurix_interchange_attempt1/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

XinDongLib/Src/%.o: ./XinDongLib/Src/%.src XinDongLib/Src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


