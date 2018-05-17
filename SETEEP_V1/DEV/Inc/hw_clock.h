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

#ifdef  HW_CLOCK 
  #define HW_CLOCK
#else
  #define HW_CLOCK  extern                    
#endif
  
HW_CLOCK volatile uint16_t u10msInt;  
HW_CLOCK volatile uint16_t u10msMain; 
HW_CLOCK uint16_t uc100msctr;
HW_CLOCK uint16_t uc1sctr;

HW_CLOCK volatile uint8_t u10ms;
HW_CLOCK volatile uint8_t u100ms;
HW_CLOCK volatile uint8_t u1s;
  
void F_Time_check(void);
void u10msRunTask(void);


