/*
 * Generated by TASKING Pin Mapper for AURIX
 * - device  : TC26x
 * - package : QFP144
 */

/*
 * Instructions on how to build this generated file and the Infineon's iLLD library with the TASKING  VX-toolset
 * for TriCore:
 * 1. Provide appropriate include paths to the compiler (-I option) to locate the iLLD library.
 * 2. Provide macro IFX_CFG_USE_COMPILER_DEFAULT_LINKER to the compiler (-D option) to prevent multiple
 *    definitions of symbol '_START' (in cstart.c and IfxCpu_CStart0.c).
 * 3. Disable compiler option 'Automatic inclusion of .sfr files' (-H option) to prevent macros to be redefined.
 */

#ifndef AURIX_PIN_MAPPINGS_H_
#define AURIX_PIN_MAPPINGS_H_

/* Infineon's iLLD library include files */

#include <Port/Io/IfxPort_Io.h>

#include <_PinMap/IfxPort_PinMap.h>


/* Symbolic names for GPIO ports */

// p00_0
#define IFXCFG_PORT_P00_0_Y0                    IfxPort_P00_0
#define IFXCFG_PORT_P00_0_Y0_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_0_Y0_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_1
#define IFXCFG_PORT_P00_1_Y1                    IfxPort_P00_1
#define IFXCFG_PORT_P00_1_Y1_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_1_Y1_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_2
#define IFXCFG_PORT_P00_2_Y2                    IfxPort_P00_2
#define IFXCFG_PORT_P00_2_Y2_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_2_Y2_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_3
#define IFXCFG_PORT_P00_3_Y3                    IfxPort_P00_3
#define IFXCFG_PORT_P00_3_Y3_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_3_Y3_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_4
#define IFXCFG_PORT_P00_4_Y4                    IfxPort_P00_4
#define IFXCFG_PORT_P00_4_Y4_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_4_Y4_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_5
#define IFXCFG_PORT_P00_5_Y5                    IfxPort_P00_5
#define IFXCFG_PORT_P00_5_Y5_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_5_Y5_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_6
#define IFXCFG_PORT_P00_6_Y6                    IfxPort_P00_6
#define IFXCFG_PORT_P00_6_Y6_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_6_Y6_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p00_7
#define IFXCFG_PORT_P00_7_Y7                    IfxPort_P00_7
#define IFXCFG_PORT_P00_7_Y7_MODE               IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P00_7_Y7_PAD_DRIVER         IfxPort_PadDriver_cmosAutomotiveSpeed1

// p02_3
#define IFXCFG_PORT_P02_3_HREF                  IfxPort_P02_3
#define IFXCFG_PORT_P02_3_HREF_MODE             IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P02_3_HREF_PAD_DRIVER       IfxPort_PadDriver_cmosAutomotiveSpeed1

// p02_4
#define IFXCFG_PORT_P02_4_SDA                   IfxPort_P02_4
#define IFXCFG_PORT_P02_4_SDA_MODE              IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P02_4_SDA_PAD_DRIVER        IfxPort_PadDriver_cmosAutomotiveSpeed4

// p02_5
#define IFXCFG_PORT_P02_5_SCL                   IfxPort_P02_5
#define IFXCFG_PORT_P02_5_SCL_MODE              IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P02_5_SCL_PAD_DRIVER        IfxPort_PadDriver_cmosAutomotiveSpeed4

// p02_6
#define IFXCFG_PORT_GPIO1                       IfxPort_P02_6
#define IFXCFG_PORT_GPIO1_MODE                  IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_GPIO1_PAD_DRIVER            IfxPort_PadDriver_cmosAutomotiveSpeed4

// p15_1
#define IFXCFG_PORT_P15_1_KEY                   IfxPort_P15_1
#define IFXCFG_PORT_P15_1_KEY_MODE              IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P15_1_KEY_PAD_DRIVER        IfxPort_PadDriver_cmosAutomotiveSpeed1

// p20_10
#define IFXCFG_PORT_GPIO2                       IfxPort_P20_10
#define IFXCFG_PORT_GPIO2_MODE                  IfxPort_Mode_inputPullUp
#define IFXCFG_PORT_GPIO2_PAD_DRIVER            IfxPort_PadDriver_cmosAutomotiveSpeed1

// p20_13
#define IFXCFG_PORT_P20_13_KEY                  IfxPort_P20_13
#define IFXCFG_PORT_P20_13_KEY_MODE             IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P20_13_KEY_PAD_DRIVER       IfxPort_PadDriver_cmosAutomotiveSpeed1

// p21_2
#define IFXCFG_PORT_P21_2_DIPSW1                IfxPort_P21_2
#define IFXCFG_PORT_P21_2_DIPSW1_MODE           IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P21_2_DIPSW1_PAD_DRIVER     IfxPort_PadDriver_cmosAutomotiveSpeed1

// p21_3
#define IFXCFG_PORT_P21_3_DIPSW2                IfxPort_P21_3
#define IFXCFG_PORT_P21_3_DIPSW2_MODE           IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P21_3_DIPSW2_PAD_DRIVER     IfxPort_PadDriver_cmosAutomotiveSpeed1

// p21_4
#define IFXCFG_PORT_P21_4_DIPSW3                IfxPort_P21_4
#define IFXCFG_PORT_P21_4_DIPSW3_MODE           IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P21_4_DIPSW3_PAD_DRIVER     IfxPort_PadDriver_cmosAutomotiveSpeed1

// p21_5
#define IFXCFG_PORT_P21_5_DIPSW4                IfxPort_P21_5
#define IFXCFG_PORT_P21_5_DIPSW4_MODE           IfxPort_Mode_inputNoPullDevice
#define IFXCFG_PORT_P21_5_DIPSW4_PAD_DRIVER     IfxPort_PadDriver_cmosAutomotiveSpeed1

// p33_8
#define IFXCFG_PORT_P33_8_LED                   IfxPort_P33_8
#define IFXCFG_PORT_P33_8_LED_MODE              IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P33_8_LED_PAD_DRIVER        IfxPort_PadDriver_cmosAutomotiveSpeed4

// p33_9
#define IFXCFG_PORT_P33_9_LED                   IfxPort_P33_9
#define IFXCFG_PORT_P33_9_LED_MODE              IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P33_9_LED_PAD_DRIVER        IfxPort_PadDriver_cmosAutomotiveSpeed4

// p33_10
#define IFXCFG_PORT_P33_10_LED                  IfxPort_P33_10
#define IFXCFG_PORT_P33_10_LED_MODE             IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P33_10_LED_PAD_DRIVER       IfxPort_PadDriver_cmosAutomotiveSpeed4

// p33_11
#define IFXCFG_PORT_P33_11_LED                  IfxPort_P33_11
#define IFXCFG_PORT_P33_11_LED_MODE             IfxPort_Mode_outputPushPullGeneral
#define IFXCFG_PORT_P33_11_LED_PAD_DRIVER       IfxPort_PadDriver_cmosAutomotiveSpeed4


/* Generic port I/O configuration */

#define IFXCFG_P00_0_IO_CONFIG                  { &IFXCFG_PORT_P00_0_Y0, IFXCFG_PORT_P00_0_Y0_MODE, IFXCFG_PORT_P00_0_Y0_PAD_DRIVER }
#define IFXCFG_P00_1_IO_CONFIG                  { &IFXCFG_PORT_P00_1_Y1, IFXCFG_PORT_P00_1_Y1_MODE, IFXCFG_PORT_P00_1_Y1_PAD_DRIVER }
#define IFXCFG_P00_2_IO_CONFIG                  { &IFXCFG_PORT_P00_2_Y2, IFXCFG_PORT_P00_2_Y2_MODE, IFXCFG_PORT_P00_2_Y2_PAD_DRIVER }
#define IFXCFG_P00_3_IO_CONFIG                  { &IFXCFG_PORT_P00_3_Y3, IFXCFG_PORT_P00_3_Y3_MODE, IFXCFG_PORT_P00_3_Y3_PAD_DRIVER }
#define IFXCFG_P00_4_IO_CONFIG                  { &IFXCFG_PORT_P00_4_Y4, IFXCFG_PORT_P00_4_Y4_MODE, IFXCFG_PORT_P00_4_Y4_PAD_DRIVER }
#define IFXCFG_P00_5_IO_CONFIG                  { &IFXCFG_PORT_P00_5_Y5, IFXCFG_PORT_P00_5_Y5_MODE, IFXCFG_PORT_P00_5_Y5_PAD_DRIVER }
#define IFXCFG_P00_6_IO_CONFIG                  { &IFXCFG_PORT_P00_6_Y6, IFXCFG_PORT_P00_6_Y6_MODE, IFXCFG_PORT_P00_6_Y6_PAD_DRIVER }
#define IFXCFG_P00_7_IO_CONFIG                  { &IFXCFG_PORT_P00_7_Y7, IFXCFG_PORT_P00_7_Y7_MODE, IFXCFG_PORT_P00_7_Y7_PAD_DRIVER }
#define IFXCFG_P02_3_IO_CONFIG                  { &IFXCFG_PORT_P02_3_HREF, IFXCFG_PORT_P02_3_HREF_MODE, IFXCFG_PORT_P02_3_HREF_PAD_DRIVER }
#define IFXCFG_P02_4_IO_CONFIG                  { &IFXCFG_PORT_P02_4_SDA, IFXCFG_PORT_P02_4_SDA_MODE, IFXCFG_PORT_P02_4_SDA_PAD_DRIVER }
#define IFXCFG_P02_5_IO_CONFIG                  { &IFXCFG_PORT_P02_5_SCL, IFXCFG_PORT_P02_5_SCL_MODE, IFXCFG_PORT_P02_5_SCL_PAD_DRIVER }
#define IFXCFG_P02_6_IO_CONFIG                  { &IFXCFG_PORT_GPIO1, IFXCFG_PORT_GPIO1_MODE, IFXCFG_PORT_GPIO1_PAD_DRIVER }
#define IFXCFG_P15_1_IO_CONFIG                  { &IFXCFG_PORT_P15_1_KEY, IFXCFG_PORT_P15_1_KEY_MODE, IFXCFG_PORT_P15_1_KEY_PAD_DRIVER }
#define IFXCFG_P20_10_IO_CONFIG                 { &IFXCFG_PORT_GPIO2, IFXCFG_PORT_GPIO2_MODE, IFXCFG_PORT_GPIO2_PAD_DRIVER }
#define IFXCFG_P20_13_IO_CONFIG                 { &IFXCFG_PORT_P20_13_KEY, IFXCFG_PORT_P20_13_KEY_MODE, IFXCFG_PORT_P20_13_KEY_PAD_DRIVER }
#define IFXCFG_P21_2_IO_CONFIG                  { &IFXCFG_PORT_P21_2_DIPSW1, IFXCFG_PORT_P21_2_DIPSW1_MODE, IFXCFG_PORT_P21_2_DIPSW1_PAD_DRIVER }
#define IFXCFG_P21_3_IO_CONFIG                  { &IFXCFG_PORT_P21_3_DIPSW2, IFXCFG_PORT_P21_3_DIPSW2_MODE, IFXCFG_PORT_P21_3_DIPSW2_PAD_DRIVER }
#define IFXCFG_P21_4_IO_CONFIG                  { &IFXCFG_PORT_P21_4_DIPSW3, IFXCFG_PORT_P21_4_DIPSW3_MODE, IFXCFG_PORT_P21_4_DIPSW3_PAD_DRIVER }
#define IFXCFG_P21_5_IO_CONFIG                  { &IFXCFG_PORT_P21_5_DIPSW4, IFXCFG_PORT_P21_5_DIPSW4_MODE, IFXCFG_PORT_P21_5_DIPSW4_PAD_DRIVER }
#define IFXCFG_P33_8_IO_CONFIG                  { &IFXCFG_PORT_P33_8_LED, IFXCFG_PORT_P33_8_LED_MODE, IFXCFG_PORT_P33_8_LED_PAD_DRIVER }
#define IFXCFG_P33_9_IO_CONFIG                  { &IFXCFG_PORT_P33_9_LED, IFXCFG_PORT_P33_9_LED_MODE, IFXCFG_PORT_P33_9_LED_PAD_DRIVER }
#define IFXCFG_P33_10_IO_CONFIG                 { &IFXCFG_PORT_P33_10_LED, IFXCFG_PORT_P33_10_LED_MODE, IFXCFG_PORT_P33_10_LED_PAD_DRIVER }
#define IFXCFG_P33_11_IO_CONFIG                 { &IFXCFG_PORT_P33_11_LED, IFXCFG_PORT_P33_11_LED_MODE, IFXCFG_PORT_P33_11_LED_PAD_DRIVER }


/* Generic alternate I/O configuration */

// asclin1
#define IFXCFG_P11_10_IO_CONFIG                 { &IfxPort_P11_10, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1 }
#define IFXCFG_P11_12_IO_CONFIG                 { &IfxPort_P11_12, IfxPort_Mode_outputPushPullAlt2, IfxPort_PadDriver_cmosAutomotiveSpeed4 }

// dap3p
#define IFXCFG_P21_7_IO_CONFIG                  { &IfxPort_P21_7, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4 }

// eruin2
#define IFXCFG_P02_1_IO_CONFIG                  { &IfxPort_P02_1, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1 }

// eruin4
#define IFXCFG_P15_5_IO_CONFIG                  { &IfxPort_P15_5, IfxPort_Mode_inputPullDown, IfxPort_PadDriver_cmosAutomotiveSpeed1 }

// eruin7
#define IFXCFG_P20_9_IO_CONFIG                  { &IfxPort_P20_9, IfxPort_Mode_inputNoPullDevice, IfxPort_PadDriver_cmosAutomotiveSpeed1 }

// gtm0_tim0_ch6
#define IFXCFG_P20_0_IO_CONFIG                  { &IfxPort_P20_0, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1 }

// i2c0
#define IFXCFG_P13_1_IO_CONFIG                  { &IfxPort_P13_1, IfxPort_Mode_outputPushPullAlt6, IfxPort_PadDriver_cmosAutomotiveSpeed4 }
#define IFXCFG_P13_1_IO_CONFIG_IN               { &IfxPort_P13_1, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed4 }
#define IFXCFG_P13_2_IO_CONFIG                  { &IfxPort_P13_2, IfxPort_Mode_outputPushPullAlt6, IfxPort_PadDriver_cmosAutomotiveSpeed4 }
#define IFXCFG_P13_2_IO_CONFIG_IN               { &IfxPort_P13_2, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed4 }

// t12ccu0
#define IFXCFG_P02_8_IO_CONFIG                  { &IfxPort_P02_8, IfxPort_Mode_inputPullUp, IfxPort_PadDriver_cmosAutomotiveSpeed1 }

// t12ccu1
#define IFXCFG_P20_11_IO_CONFIG                 { &IfxPort_P20_11, IfxPort_Mode_outputPushPullAlt7, IfxPort_PadDriver_cmosAutomotiveSpeed4 }

// tout104
#define IFXCFG_P10_2_IO_CONFIG                  { &IfxPort_P10_2, IfxPort_Mode_outputPushPullAlt1, IfxPort_PadDriver_cmosAutomotiveSpeed4 }

// tout105
#define IFXCFG_P10_3_IO_CONFIG                  { &IfxPort_P10_3, IfxPort_Mode_outputPushPullAlt1, IfxPort_PadDriver_cmosAutomotiveSpeed4 }


/* Initialization routines */

extern void gpio_init_pins(void);

extern void asclin1_init_pins(void);

extern void dap3p_init_pins(void);

extern void eruin2_init_pins(void);

extern void eruin4_init_pins(void);

extern void eruin7_init_pins(void);

extern void gtm0_tim0_ch6_init_pins(void);

extern void i2c0_init_pins(void);

extern void t12ccu0_init_pins(void);

extern void t12ccu1_init_pins(void);

extern void tout104_init_pins(void);

extern void tout105_init_pins(void);


#endif /* AURIX_PIN_MAPPINGS_H_ */
