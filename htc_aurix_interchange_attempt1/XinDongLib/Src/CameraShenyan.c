/*
 * CameraShenyan.c
 *
 *  Created on: 7 Dec 2022
 *      Author: Zonghuan Wu
 * Description:
 */

#include "CameraShenyan.h"
#include <Ifx_Fifo.h>
#include <IfxAsclin.h>
#include <IfxAsclin_Asc.h>
#include <IfxAsclin_PinMap.h>

/**********************************************************************/
/****************************GPIO related******************************/
/**********************************************************************/

#define PIN_MODE_OUTPUT          IfxPort_Mode_outputPushPullGeneral
#define PIN_MODE_OUTPUT_OD       IfxPort_Mode_outputOpenDrainGeneral
#define PIN_MODE_INPUT           IfxPort_Mode_inputNoPullDevice
#define PIN_MODE_INPUT_PULLUP    IfxPort_Mode_inputPullUp
#define PIN_MODE_INPUT_PULLDOWN  IfxPort_Mode_inputPullDown

#define PIN_GetModule(GPIO_NAME) (Ifx_P*)(0xF0030000u | (GPIO_NAME & 0xFF00))

#define PIN_GetIndex(GPIO_NAME)   (uint8)(GPIO_NAME & 0x000F)

typedef enum  //
{
	P00_0 = 0xA000,
	P00_1 = 0xA001,
	P00_2 = 0xA002,
	P00_3 = 0xA003,
	P00_4 = 0xA004,
	P00_5 = 0xA005,
	P00_6 = 0xA006,
	P00_7 = 0xA007,
	P00_8 = 0xA008,
	P00_9 = 0xA009,
	P00_11 = 0xA00B,
	P00_12 = 0xA00C,

	P02_0 = 0xA200,
	P02_1 = 0xA201,
	P02_2 = 0xA202,
	P02_3 = 0xA203,
	P02_4 = 0xA204,
	P02_5 = 0xA205,
	P02_6 = 0xA206,
	P02_7 = 0xA207,
	P02_8 = 0xA208,

	P10_1 = 0xB001,
	P10_2 = 0xB002,
	P10_3 = 0xB003,
	P10_5 = 0xB005,
	P10_6 = 0xB006,

	P11_2 = 0xB102,
	P11_3 = 0xB103,
	P11_6 = 0xB106,
	P11_9 = 0xB109,
	P11_10 = 0xB10A,
	P11_11 = 0xB10B,
	P11_12 = 0xB10C,

	P13_0 = 0xB300,
	P13_1 = 0xB301,
	P13_2 = 0xB302,
	P13_3 = 0xB303,

	P14_0 = 0xB400,
	P14_1 = 0xB401,
	P14_2 = 0xB402,
	P14_3 = 0xB403,
	P14_4 = 0xB404,
	P14_5 = 0xB405,
	P14_6 = 0xB406,

	P15_0 = 0xB500,
	P15_1 = 0xB501,
	P15_2 = 0xB502,
	P15_3 = 0xB503,
	P15_4 = 0xB504,
	P15_5 = 0xB505,
	P15_6 = 0xB506,
	P15_7 = 0xB507,
	P15_8 = 0xB508,

	P20_0 = 0xC000,
	P20_2 = 0xC002,
	P20_3 = 0xC003,
	P20_6 = 0xC006,
	P20_7 = 0xC007,
	P20_8 = 0xC008,
	P20_9 = 0xC009,
	P20_10 = 0xC00A,
	P20_11 = 0xC00B,
	P20_12 = 0xC00C,
	P20_13 = 0xC00D,
	P20_14 = 0xC00E,

	P21_2 = 0xC102,
	P21_3 = 0xC103,
	P21_4 = 0xC104,
	P21_5 = 0xC105,
	P21_6 = 0xC106,
	P21_7 = 0xC107,

	P22_0 = 0xC200,
	P22_1 = 0xC201,
	P22_2 = 0xC202,
	P22_3 = 0xC203,

	P23_1 = 0xC301,

	P32_0 = 0xD200,
	P32_4 = 0xD204,

	P33_4 = 0xD304,
	P33_5 = 0xD305,
	P33_6 = 0xD306,
	P33_7 = 0xD307,
	P33_8 = 0xD308,
	P33_9 = 0xD309,
	P33_10 = 0xD30A,
	P33_11 = 0xD30B,
	P33_12 = 0xD30C,
	P33_13 = 0xD30D,

} GPIO_Name_t;

void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output){

	Ifx_P *port = PIN_GetModule(pin);
	unsigned char pinIndex = PIN_GetIndex(pin);

	IfxPort_setPinMode(port, pinIndex, mode);

	IfxPort_setPinPadDriver(port, pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed2);

	if(0 == output){
		IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
	}else{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
	}
}

uint8 PIN_Read(GPIO_Name_t pin){
	Ifx_P *port = PIN_GetModule(pin);
	unsigned char pinIndex = PIN_GetIndex(pin);
	return IfxPort_getPinState(port, pinIndex);
}

void PIN_Write(GPIO_Name_t pin, unsigned char output){
	Ifx_P *port = PIN_GetModule(pin);
	unsigned char pinIndex = PIN_GetIndex(pin);

	if(0 == output){
		IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
	}else{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
	}
}

void PIN_Dir(GPIO_Name_t pin, unsigned char mode){
	Ifx_P *port = PIN_GetModule(pin);
	unsigned char pinIndex = PIN_GetIndex(pin);

	if(0 == mode){
		IfxPort_setPinMode(port, pinIndex, PIN_MODE_INPUT);
	}else{
		IfxPort_setPinMode(port, pinIndex, PIN_MODE_OUTPUT);
	}
}

void PIN_Reverse(GPIO_Name_t pin){
	Ifx_P *port = PIN_GetModule(pin);
	unsigned char pinIndex = PIN_GetIndex(pin);

	IfxPort_togglePin(port, pinIndex);
}

/**********************************************************************/
/****************************SCCB related******************************/
/**********************************************************************/

#define MT9V034_SCL_Out   PIN_Dir(MT9V034_SCL_PIN, 1)
#define MT9V034_SDA_Out   PIN_Dir(MT9V034_SDA_PIN, 1)
#define MT9V034_SDA_In    PIN_Dir(MT9V034_SDA_PIN, 0)
#define MT9V034_SCL_High  PIN_Write(MT9V034_SCL_PIN, 1)
#define MT9V034_SCL_Low   PIN_Write(MT9V034_SCL_PIN, 0)
#define MT9V034_SDA_High  PIN_Write(MT9V034_SDA_PIN, 1)
#define MT9V034_SDA_Low   PIN_Write(MT9V034_SDA_PIN, 0)
#define MT9V034_SDA_Data  PIN_Read(MT9V034_SDA_PIN)

void SCCB_Init(void){
	PIN_InitConfig(MT9V034_SCL_PIN, PIN_MODE_OUTPUT, 1);
	PIN_InitConfig(MT9V034_SDA_PIN, PIN_MODE_OUTPUT, 1);
	IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SCL_PIN), PIN_GetIndex(MT9V034_SCL_PIN),
			IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SDA_PIN), PIN_GetIndex(MT9V034_SDA_PIN),
			IfxPort_PadDriver_cmosAutomotiveSpeed1);
	MT9V034_SCL_High;
	MT9V034_SDA_High;
}

void SCCB_Wait(void){
	int i;
	for(i = 0; i < 500; ++i){
		__asm("NOP"); /* delay */
	}
}

void SCCB_Star(void){
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	SCCB_Wait();
	MT9V034_SDA_High;
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

void SCCB_Stop(void){
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	SCCB_Wait();
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SDA_High;
	SCCB_Wait();
}

unsigned char SCCB_SendByte(unsigned char Data){
	unsigned char i;
	unsigned char Ack;
	MT9V034_SDA_Out;
	for(i = 0; i < 8; i++){
		if(Data & 0x80){
			MT9V034_SDA_High;
		}else{
			MT9V034_SDA_Low;
		}
		Data <<= 1;
		SCCB_Wait();
		MT9V034_SCL_High;
		SCCB_Wait();
		MT9V034_SCL_Low;
		SCCB_Wait();
	}
	MT9V034_SDA_High;
	MT9V034_SDA_In;
	SCCB_Wait();

	MT9V034_SCL_High;
	SCCB_Wait();
	Ack = MT9V034_SDA_Data;
	MT9V034_SCL_Low;
	SCCB_Wait();
	return Ack;
}

unsigned char SCCB_ReadByte(void){
	unsigned char i;
	volatile unsigned char byte;
	MT9V034_SCL_Out;
	MT9V034_SDA_In;
	for(i = 0; i < 8; i++){
		MT9V034_SCL_Low;
		SCCB_Wait();
		MT9V034_SCL_High;
		SCCB_Wait();
		byte = (byte << 1) | (MT9V034_SDA_Data & 1);
	}
	MT9V034_SCL_Low;
	MT9V034_SDA_Out;
	SCCB_Wait();
	return byte;
}

static void SCCB_Ack(void){
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	MT9V034_SCL_Low;
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

static void SCCB_NAck(void){
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	MT9V034_SCL_Low;
	SCCB_Wait();
	MT9V034_SDA_High;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

void SCCB_RegWrite(unsigned char Device, unsigned char reg, unsigned short Data){
	unsigned char i;
	unsigned char Ack;

	for(i = 0; i < 3; i++){
		SCCB_Star();
		Ack = SCCB_SendByte(Device);
		if(Ack == 1){
			continue;
		}

		Ack = SCCB_SendByte(reg);
		if(Ack == 1){
			continue;
		}

		SCCB_SendByte((unsigned char)(Data >> 8));
		Ack = SCCB_SendByte((unsigned char)Data);
		if(Ack == 1){
			continue;
		}

		SCCB_Stop();
		if(Ack == 0) break;
	}
}

unsigned char SCCB_RegRead(unsigned char Device, unsigned char reg, unsigned short *Data){
	unsigned char Ack = 0;
	Device = Device << 1;
	SCCB_Star();
	Ack += SCCB_SendByte(Device);

	Ack += SCCB_SendByte(reg);

	SCCB_Star();
	Ack += SCCB_SendByte(Device + 1);

	*Data = SCCB_ReadByte();
	SCCB_Ack();
	*Data = *Data << 8;

	*Data += SCCB_ReadByte();
	SCCB_NAck();

	SCCB_Stop();

	return Ack;
}

/**********************************************************************/
/***************************MT9V034 related*****************************/
/**********************************************************************/

#define MT9V034_I2C_ADDR                        0xB8 //(0xB8 >> 1)=0x5C
#define MAX_IMAGE_HEIGHT            480
#define MAX_IMAGE_WIDTH             752
#define MT9V034_PIXEL_ARRAY_HEIGHT              492
#define MT9V034_PIXEL_ARRAY_WIDTH               782
#define MT9V034_CHIP_VERSION                0x00
#define MT9V034_CHIP_ID                     0x1324

#define MT9V034_COLUMN_START                        0x01
#define MT9V034_COLUMN_START_MIN                1
#define MT9V034_COLUMN_START_DEF                1
#define MT9V034_COLUMN_START_MAX                752

#define MT9V034_ROW_START                       0x02
#define MT9V034_ROW_START_MIN               4
#define MT9V034_ROW_START_DEF               4
#define MT9V034_ROW_START_MAX               482

#define MT9V034_WINDOW_HEIGHT               0x03
#define MT9V034_WINDOW_HEIGHT_MIN               1
#define MT9V034_WINDOW_HEIGHT_DEF               64
#define MT9V034_WINDOW_HEIGHT_MAX               480

#define MT9V034_WINDOW_WIDTH                        0x04
#define MT9V034_WINDOW_WIDTH_MIN                1
#define MT9V034_WINDOW_WIDTH_DEF                64
#define MT9V034_WINDOW_WIDTH_MAX                752

#define MINIMUM_HORIZONTAL_BLANKING     91

#define MT9V034_HORIZONTAL_BLANKING             0x05
#define MT9V034_HORIZONTAL_BLANKING_MIN     43
#define MT9V034_HORIZONTAL_BLANKING_MAX     1023

#define MT9V034_VERTICAL_BLANKING               0x06
#define MT9V034_VERTICAL_BLANKING_MIN       4
#define MT9V034_VERTICAL_BLANKING_MAX       3000

#define MT9V034_CHIP_CONTROL                        0x07
#define MT9V034_CHIP_CONTROL_MASTER_MODE         (1 << 3)
#define MT9V034_CHIP_CONTROL_DOUT_ENABLE         (1 << 7)
#define MT9V034_CHIP_CONTROL_SEQUENTIAL     (1 << 8)

#define MT9V034_SHUTTER_WIDTH1              0x08
#define MT9V034_SHUTTER_WIDTH2              0x09
#define MT9V034_SHUTTER_WIDTH_CONTROL       0x0A
#define MT9V034_TOTAL_SHUTTER_WIDTH     0x0B
#define MT9V034_TOTAL_SHUTTER_WIDTH_MIN     1
#define MT9V034_TOTAL_SHUTTER_WIDTH_DEF     480
#define MT9V034_TOTAL_SHUTTER_WIDTH_MAX     32767

#define MT9V034_RESET                       0x0C

#define MT9V034_READ_MODE                       0x0D
#define MT9V034_READ_MODE_ROW_BIN_MASK      (3 << 0)
#define MT9V034_READ_MODE_ROW_BIN_SHIFT     0
#define MT9V034_READ_MODE_COLUMN_BIN_MASK        (3 << 2)
#define MT9V034_READ_MODE_COLUMN_BIN_SHIFT       2
#define MT9V034_READ_MODE_ROW_BIN_2         (1<<0)
#define MT9V034_READ_MODE_ROW_BIN_4         (1<<1)
#define MT9V034_READ_MODE_COL_BIN_2         (1<<2)
#define MT9V034_READ_MODE_COL_BIN_4         (1<<3)
#define MT9V034_READ_MODE_ROW_FLIP              (1 << 4)
#define MT9V034_READ_MODE_COLUMN_FLIP       (1 << 5)
#define MT9V034_READ_MODE_DARK_COLUMNS      (1 << 6)
#define MT9V034_READ_MODE_DARK_ROWS             (1 << 7)

#define MT9V034_PIXEL_OPERATION_MODE                0x0F
#define MT9V034_PIXEL_OPERATION_MODE_COLOR       (1 << 2)
#define MT9V034_PIXEL_OPERATION_MODE_HDR         (1 << 6)

#define MT9V034_V1_CTRL_REG_A           0x31
#define MT9V034_V2_CTRL_REG_A           0x32
#define MT9V034_V3_CTRL_REG_A           0x33
#define MT9V034_V4_CTRL_REG_A           0x34

#define MT9V034_ANALOG_GAIN                     0x35
#define MT9V034_ANALOG_GAIN_MIN             16
#define MT9V034_ANALOG_GAIN_DEF             16
#define MT9V034_ANALOG_GAIN_MAX             64

#define MT9V034_MAX_ANALOG_GAIN             0x36
#define MT9V034_MAX_ANALOG_GAIN_MAX             127

#define MT9V034_FRAME_DARK_AVERAGE              0x42
#define MT9V034_DARK_AVG_THRESH             0x46
#define MT9V034_DARK_AVG_LOW_THRESH_MASK         (255 << 0)
#define MT9V034_DARK_AVG_LOW_THRESH_SHIFT        0
#define MT9V034_DARK_AVG_HIGH_THRESH_MASK       (255 << 8)
#define MT9V034_DARK_AVG_HIGH_THRESH_SHIFT      8

#define MT9V034_ROW_NOISE_CORR_CONTROL      0x70
#define MT9V034_ROW_NOISE_CORR_ENABLE       (1 << 5)
#define MT9V034_ROW_NOISE_CORR_USE_BLK_AVG      (1 << 7)

#define MT9V034_PIXEL_CLOCK                 0x74
#define MT9V034_PIXEL_CLOCK_INV_LINE            (1 << 0)
#define MT9V034_PIXEL_CLOCK_INV_FRAME       (1 << 1)
#define MT9V034_PIXEL_CLOCK_XOR_LINE            (1 << 2)
#define MT9V034_PIXEL_CLOCK_CONT_LINE       (1 << 3)
#define MT9V034_PIXEL_CLOCK_INV_PXL_CLK     (1 << 4)

#define MT9V034_TEST_PATTERN                    0x7f
#define MT9V034_TEST_PATTERN_DATA_MASK      (1023 << 0)
#define MT9V034_TEST_PATTERN_DATA_SHIFT     0
#define MT9V034_TEST_PATTERN_USE_DATA       (1 << 10)
#define MT9V034_TEST_PATTERN_GRAY_MASK      (3 << 11)
#define MT9V034_TEST_PATTERN_GRAY_NONE      (0 << 11)
#define MT9V034_TEST_PATTERN_GRAY_VERTICAL      (1 << 11)
#define MT9V034_TEST_PATTERN_GRAY_HORIZONTAL        (2 << 11)
#define MT9V034_TEST_PATTERN_GRAY_DIAGONAL      (3 << 11)
#define MT9V034_TEST_PATTERN_ENABLE         (1 << 13)
#define MT9V034_TEST_PATTERN_FLIP           (1 << 14)

#define MT9V034_AEC_AGC_ENABLE          0xAF
#define MT9V034_AEC_ENABLE                  (1 << 0)
#define MT9V034_AGC_ENABLE                  (1 << 1)
#define MT9V034_THERMAL_INFO                    0xc1
#define MT9V034_ANALOG_CTRL                     (0xC2)
#define MT9V034_ANTI_ECLIPSE_ENABLE                 (1<<7)
#define MT9V034_MAX_GAIN                        (0xAB)
#define MT9V034_FINE_SHUTTER_WIDTH_TOTAL_A      (0xD5)
#define MT9V034_HDR_ENABLE_REG          0x0F
#define MT9V034_ADC_RES_CTRL_REG            0x1C
#define MT9V034_ROW_NOISE_CORR_CTRL_REG 0x70
#define MT9V034_TEST_PATTERN_REG        0x7F
#define MT9V034_TILED_DIGITAL_GAIN_REG  0x80
#define MT9V034_AGC_AEC_DESIRED_BIN_REG 0xA5
#define MT9V034_MAX_GAIN_REG            0xAB
#define MT9V034_MIN_EXPOSURE_REG        0xAC
#define MT9V034_MAX_EXPOSURE_REG        0xAD
#define MT9V034_AEC_AGC_ENABLE_REG  0xAF
#define MT9V034_AGC_AEC_PIXEL_COUNT_REG 0xB0

#define CAMERA_CONTRAST          0x01c7

#define CAMERA_AUTO_EXPOSURE     1

#define CAMERA_AUTO_EXPOSURE_BRIGHTNESS  50

#define CAMERA_MAX_EXPOSURE_TIME  250
#define CAMERA_MIN_EXPOSURE_TIME  1

#define CAMERA_EXPOSURE_TIME  150

void MT9V034_SetFrameRate(unsigned char fps){
	float vertical = 0;
	if(fps > 200){
		vertical = 2;
	}else if(fps >= 150){
		vertical = -1.06 * fps + 251;
	}else if(fps > 100){
		vertical = -2.14 * fps + 413;
	}else if(fps >= 71){
		vertical = -4.5017 * fps + 649.17;
	}else if(fps > 50){
		vertical = -8.8517 * fps + 957.58;
	}else if(fps > 20){
		vertical = -31.167 * fps + 2073.3;
	}else{
		vertical = -155 * fps + 4550;
	}

	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (unsigned short)vertical);
}

void MTV_IICWriteReg16(unsigned char reg, unsigned short val){
	SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}

static void MT9V034_SetFrameResolution(unsigned short height, unsigned short width){
	unsigned short data = 0;

	if((height * 4) <= MAX_IMAGE_HEIGHT){
		height *= 4;
		data |= MT9V034_READ_MODE_ROW_BIN_4;
	}else if((height * 2) <= MAX_IMAGE_HEIGHT){
		height *= 2;
		data |= MT9V034_READ_MODE_ROW_BIN_2;
	}

	if((width * 4) <= MAX_IMAGE_WIDTH){
		width *= 4;
		data |= MT9V034_READ_MODE_COL_BIN_4;
	}else if((width * 2) <= MAX_IMAGE_WIDTH){
		width *= 2;
		data |= MT9V034_READ_MODE_COL_BIN_2;
	}

	data |= (MT9V034_READ_MODE_ROW_FLIP | MT9V034_READ_MODE_COLUMN_FLIP);
	MTV_IICWriteReg16(MT9V034_READ_MODE, data);

	MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH, width);
	MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height);

	MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);
	MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);

}

void MT9V034_SetAutoExposure(char enable){
	unsigned short reg = 0;
	SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, &reg);
	if(enable){

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg | MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE);

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG,
		CAMERA_MAX_EXPOSURE_TIME);

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG,
		CAMERA_MIN_EXPOSURE_TIME);

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);

	}else{

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg & ~(MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE));

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);

		SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_TOTAL_SHUTTER_WIDTH,
		CAMERA_EXPOSURE_TIME);

	}

}

void MT9V034_Init(unsigned char fps){
	unsigned short data = 0;

	SCCB_Init();

	if(SCCB_RegRead(MT9V034_I2C_ADDR >> 1, MT9V034_CHIP_VERSION, &data) == 0){
		if(data != MT9V034_CHIP_ID){

			while(1)
				;
		}
	}else{
		while(1)
			;
	}

	MT9V034_SetFrameResolution(IMAGEH, IMAGEW);

	MT9V034_SetFrameRate(fps);

	MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);
	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL,
	MT9V034_ANTI_ECLIPSE_ENABLE);
	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);  //0x1C  here is the way to regulate darkness :)
	////
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x13, 0x2D2E);  //We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x20, CAMERA_CONTRAST);  //Recommended by design to improve performance in HDR mode and when frame rate is low.
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x24, 0x0010);  //Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2B, 0x0003);    //Improves column FPN.
	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2F, 0x0003);  //Improves FPN at near-saturation.

	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_V2_CTRL_REG_A, 0x001A);  //0x32   0x001A
	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_HDR_ENABLE_REG, 0x0103);  //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.

	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_DESIRED_BIN_REG,
	CAMERA_AUTO_EXPOSURE_BRIGHTNESS);

}

/**********************************************************************/
/***************************Camera related*****************************/
/**********************************************************************/

uint8 Camera_Flag = 0;
uint8 Image_Data[IMAGEH][IMAGEW];

void CAMERA_Init(unsigned char fps){
	PIN_InitConfig(P00_0, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_1, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_2, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_3, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_4, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_5, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_6, PIN_MODE_INPUT_PULLDOWN, 0);
	PIN_InitConfig(P00_7, PIN_MODE_INPUT_PULLDOWN, 0);

	MT9V034_Init(fps);
}

uint8 CAMERA_GetFlag(void){
	return Camera_Flag;
}

void CAMERA_ResetFlag(void){
	Camera_Flag = 0;
}

uint8** CAMERA_GetArray(void){
	return Image_Data;
}
