/**********************************************
;**********************************************
;***      串口通讯 头文件
;***      作者：李鹏
;***      Date:   2015-02-5
;**********************************************
;**********************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "bsp.h"

#ifdef  MBAPP
#define MBAPP

#else
#define MBAPP  extern                    
#endif

uint16_t MB_RdHoding(uint16_t slave_node,uint8_t type,uint16_t reg, uint16_t *perr);
void MB_WrHoding(uint16_t slave_node, uint8_t type,uint16_t reg, uint16_t reg_val, uint16_t *perr);
