/*
 * UserSource.h
 *
 *  Created on: 2019Äê1ÔÂ7ÈÕ
 *      Author: ZHANGZH
 */

#ifndef _USERSOURCE_H_
#define _USERSOURCE_H_

#include "ServeSource.h"
#include "CameraShenyan.h"
IFX_EXTERN void UserCpu0Main(void);
IFX_EXTERN void UserCpu1Main(void);
IFX_EXTERN uint32 UserInterupt10ms(void);
IFX_EXTERN uint32 UserInterupt100ms(void);
IFX_EXTERN uint32 UserInterupt1000ms(void);
IFX_EXTERN void UserInteruptIO(void) ;

#endif /* 0_SRC_APPSW_TRICORE_MAIN_USERSOURCE_H_ */
