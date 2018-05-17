/**********************************************
;**********************************************
;***      动环配置 头文件
;***      Author: 李鹏
;***      Date:   2016-11-12
;**********************************************
;**********************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "bsp.h"

#ifdef  DH_APP 

  #define DH_APP
#else
  #define DH_APP  extern                    
#endif
  
void Uart4_Tmr(void);
void F_CheckDHCommSta(void);
  
