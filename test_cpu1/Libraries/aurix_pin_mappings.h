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


/* Generic alternate I/O configuration */

// dap3p
#define IFXCFG_P21_7_IO_CONFIG                  { &IfxPort_P21_7, IfxPort_Mode_outputPushPullGeneral, IfxPort_PadDriver_cmosAutomotiveSpeed4 }


/* Initialization routines */

extern void dap3p_init_pins(void);


#endif /* AURIX_PIN_MAPPINGS_H_ */
