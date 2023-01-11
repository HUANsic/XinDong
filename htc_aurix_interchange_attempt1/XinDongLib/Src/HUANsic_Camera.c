#include "../Inc/HUANsic_Camera.h"

#include <IfxPort.h>
#include <IfxPort_reg.h>

#include "../Inc/XinDong_Config.h"

//#define		MT9V034_SCL_PIN				P02_5
//#define		MT9V034_SDA_PIN				P02_4

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

/* private function prototypes */
static inline void SDA_OUT(void);
static inline void SDA_IN(void);
static inline uint8 SDA_VALUE(void);
static inline void SDA_HIGH(void);
static inline void SDA_LOW(void);
static inline void SCL_HIGH(void);
static inline void SCL_LOW(void);
static inline void SCCB_Wait(void);
static void SCCB_Start(void);
static void SCCB_Stop(void);
static void SCCB_SendBit(uint8 bit);
static uint8 SCCB_ReadBit(void);
static uint8 SCCB_SendByte(uint8 data);
static uint8 SCCB_ReadByte(uint8 ack);
static void SCCB_RegWrite(uint8 reg, uint16 data);
static uint16 SCCB_RegRead(uint8 reg);

/* private variables */
uint8 __camera_flag;
uint8 g_ImageData[IMAGEH][IMAGEW];

/* methods for the user to call */
void camera_init(void){
	/* initialize pins */
	// Camera Interface
	IfxPort_setPinMode(&MODULE_P00, 0, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 1, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 2, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 3, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 4, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 5, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 6, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P00, 7, IfxPort_Mode_inputNoPullDevice);
	IfxPort_setPinMode(&MODULE_P15, 5, IfxPort_InputMode_pullDown);
	IfxPort_setPinMode(&MODULE_P02, 1, IfxPort_InputMode_pullUp);
	// SCCB
	IfxPort_setPinMode(&MODULE_P02, 4, IfxPort_Mode_outputOpenDrainGeneral);
	IfxPort_setPinMode(&MODULE_P02, 5, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinPadDriver(&MODULE_P02, 4, IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxPort_setPinPadDriver(&MODULE_P02, 5, IfxPort_PadDriver_cmosAutomotiveSpeed1);
	IfxPort_setPinState(&MODULE_P02, 4, IfxPort_State_high);
	IfxPort_setPinState(&MODULE_P02, 5, IfxPort_State_high);

	/* configuration via SCCB */
	uint16 data;

	// check ID
	data = SCCB_RegRead(MT9V034_CHIP_VERSION);
	if(data != MT9V034_CHIP_ID){
		while(1)
			;
	}

	// set frame resolution
	data = 0;
#if(IMAGEH * 4 <= MAX_IMAGE_HEIGHT)
	SCCB_RegWrite(MT9V034_WINDOW_HEIGHT, IMAGEH * 4);
	data |= MT9V034_READ_MODE_ROW_BIN_4;
#elif(IMAGEH * 2 <= MAX_IMAGE_HEIGHT)
	SCCB_RegWrite(MT9V034_WINDOW_HEIGHT, IMAGEH * 2);
	data |= MT9V034_READ_MODE_ROW_BIN_2;
#endif
#if(IMAGEW * 4 <= MAX_IMAGE_WIDTH)
	SCCB_RegWrite(MT9V034_WINDOW_WIDTH, IMAGEW * 4);
	data |= MT9V034_READ_MODE_COL_BIN_4;
#elif(IMAGEW * 2 <= MAX_IMAGE_WIDTH)
	SCCB_RegWrite(MT9V034_WINDOW_WIDTH, IMAGEW * 2);
	data |= MT9V034_READ_MODE_COL_BIN_2;
#endif
	data |= (MT9V034_READ_MODE_ROW_FLIP | MT9V034_READ_MODE_COLUMN_FLIP);
	SCCB_RegWrite(MT9V034_READ_MODE, data);

	// set fps
#if(CAM_FPS > 200)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, 2);
#elif(CAM_FPS >= 150)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, (uint16)(-1.06 * CAM_FPS + 251));
#elif(CAM_FPS > 100)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, (uint16)(-2.14 * CAM_FPS + 413));
#elif(CAM_FPS >= 71)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, (uint16)(-4.5017 * CAM_FPS + 649.17));
#elif(CAM_FPS > 50)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, (uint16)(-8.8517 * CAM_FPS + 957.58));
#elif(CAM_FPS > 20)
	SCCB_RegWrite(MT9V034_VERTICAL_BLANKING, (uint16)(-31.167 * CAM_FPS + 2073.3));
#else
	SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (uint16)(-155 * CAM_FPS + 4550));
#endif

	// set auto exposure
	data = SCCB_RegRead(MT9V034_AEC_AGC_ENABLE);
	SCCB_RegWrite(MT9V034_AEC_AGC_ENABLE, data | MT9V034_AEC_ENABLE | MT9V034_AGC_ENABLE);
	SCCB_RegWrite(MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
	SCCB_RegWrite(MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
	SCCB_RegWrite(MT9V034_MAX_GAIN_REG, 20);
	SCCB_RegWrite(MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);

	// others
	SCCB_RegWrite(0x2C, 0x0004);
	SCCB_RegWrite(MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);
	SCCB_RegWrite(MT9V034_ADC_RES_CTRL_REG, 0x0303);  //0x1C  here is the way to regulate darkness :)
	SCCB_RegWrite(0x13, 0x2D2E);  //We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
	SCCB_RegWrite(0x20, CAMERA_CONTRAST);  //Recommended by design to improve performance in HDR mode and when frame rate is low.
	SCCB_RegWrite(0x24, 0x0010);  //Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
	SCCB_RegWrite(0x2B, 0x0003);    //Improves column FPN.
	SCCB_RegWrite(0x2F, 0x0003);  //Improves FPN at near-saturation.
	SCCB_RegWrite(MT9V034_V2_CTRL_REG_A, 0x001A);  //0x32   0x001A
	SCCB_RegWrite(MT9V034_HDR_ENABLE_REG, 0x0103);  //0x0F High Dynamic Range enable, bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.
	SCCB_RegWrite(MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);
}

uint8 camera_hasImage(void){
	return __camera_flag == 2;
}

uint8 camera_getFlag(void){
	return __camera_flag;
}

void camera_resetFlag(void){
	__camera_flag = 0;
}

void camera_incrementFlag(void){
	__camera_flag++;
}

/* SCCB functional units */
static inline void SDA_OUT(void){
	IfxPort_setPinMode(&MODULE_P02, 4, IfxPort_Mode_outputPushPullGeneral);
}

static inline void SDA_IN(void){
	IfxPort_setPinMode(&MODULE_P02, 4, IfxPort_Mode_inputNoPullDevice);
}

static inline uint8 SDA_VALUE(void){
	return IfxPort_getPinState(&MODULE_P02, 4);
}

static inline void SDA_HIGH(void){
	IfxPort_setPinState(&MODULE_P02, 4, IfxPort_State_high);
}

static inline void SDA_LOW(void){
	IfxPort_setPinState(&MODULE_P02, 4, IfxPort_State_low);
}

static inline void SCL_HIGH(void){
	IfxPort_setPinState(&MODULE_P02, 5, IfxPort_State_high);
}

static inline void SCL_LOW(void){
	IfxPort_setPinState(&MODULE_P02, 5, IfxPort_State_low);
}

static inline void SCCB_Wait(void){
	uint16 i;
	for(i = 0; i < 500; ++i){
		__asm("NOP"); /* delay */
	}
}

static void SCCB_Start(void){
	SCL_HIGH();		// already high in any case, but force it anyway
	SDA_HIGH();
	SDA_OUT();
	SCCB_Wait();
	SDA_LOW();		// SDA falls when SCL is high
	SCCB_Wait();
	SCL_LOW();		// SCL then falls
	// no waiting
}

static void SCCB_Stop(void){
	SCL_LOW();		// already low in any case, but force it anyway
	SDA_LOW();
	SDA_OUT();		// SDA must be low when SCL rises
	SCCB_Wait();
	SCL_HIGH();		// SCL rises first
	SCCB_Wait();
	SDA_HIGH();		// SDA then rises when SCL is high
	SDA_IN();		// release the line immediately
	SCCB_Wait();
}

static void SCCB_SendBit(uint8 bit){
	SCCB_Wait();
	if(bit) SDA_HIGH();		// switch SDA while SCL is low
	else SDA_LOW();
	SCCB_Wait();
	SCL_HIGH();		// SCL rises
	SCCB_Wait();
	SCL_LOW();		// SCL falls; make sure nothing happens in between for safety
	// no waiting
}

static uint8 SCCB_ReadBit(void){
	uint8 retState;
	SCCB_Wait();
	SCL_HIGH();		// SCL rises
	retState = SDA_VALUE();		// read SDA at the same time
	SCCB_Wait();
	SCL_LOW();		// SCL falls; make sure nothing happens in between for safety
	SCCB_Wait();
	return retState;
}

static uint8 SCCB_SendByte(uint8 data){
	uint8 i;
	SDA_OUT();
	for(i = 0; i < 8; i++)
		SCCB_SendBit(data & (1 << (7 - i)));
	SDA_IN();
	return SCCB_ReadBit() ? 0 : 1;		// ACK bit
}

static uint8 SCCB_ReadByte(uint8 ack){
	uint8 i;
	uint8 data = 0;
	SDA_IN();
	for(i = 0; i < 8; i++)
		data |= SCCB_ReadBit() << (7 - i);
	SDA_OUT();
	SCCB_SendBit(!ack);		// ACK bit
	return data;
}

static void SCCB_RegWrite(uint8 reg, uint16 data){
	SCCB_Start();
	SCCB_SendByte(CAM_Address << 1);	// send slave address and #W
	SCCB_SendByte(reg);
	SCCB_SendByte((uint8)(data >> 8));
	SCCB_SendByte((uint8)data);
	SCCB_Stop();
}

static uint16 SCCB_RegRead(uint8 reg){
	uint16 retData;
	SCCB_Start();
	SCCB_SendByte(CAM_Address << 1);	// send slave address and #W
	SCCB_SendByte(reg);		// dummy write

	SCCB_Start();
	SCCB_SendByte((CAM_Address << 1) | 1);	// send slave address and R
	retData = SCCB_ReadByte(1);		// ACK
	retData <<= 8;
	retData |= SCCB_ReadByte(0);	// no ACK
	SCCB_Stop();

	return retData;
}
