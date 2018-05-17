/**********************************************
;**********************************************
;***      GPIO管脚配置 头文件
;***      Author: 李鹏
;***      Date:   2016-11-12
;**********************************************
;**********************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "bsp.h"

#ifdef  OUT_COMM 
  #define OUT_COMM
  
#else
  #define OUT_COMM  extern                    
#endif
 
  
#define OUT_ADDR							0x60
//  #define OUT_ADDR							0x1
void Uart2_Tmr(void);

