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

#ifdef  HW_GPIO 
  #define HW_GPIO
#else
  #define HW_GPIO  extern                    
#endif
  
/*
HFIM5_DO_17  ============ RED
HFIM5_DO_18  ============ green
*/
  
#define RED_ON    hfim5_do_output(HFIM5_DO_17,HFIM5_DO_VAL_L)
#define RED_OFF   hfim5_do_output(HFIM5_DO_17,HFIM5_DO_VAL_H)
#define GREEN_ON  hfim5_do_output(HFIM5_DO_18,HFIM5_DO_VAL_H)
#define GREEN_OFF hfim5_do_output(HFIM5_DO_18,HFIM5_DO_VAL_L)
  
  
#define PUMP_RELAY_ON     hfim5_do_output(HFIM5_DO_1,HFIM5_DO_VAL_H)
#define PUMP_RELAY_OFF    hfim5_do_output(HFIM5_DO_1,HFIM5_DO_VAL_L)
  
#define HEATER_RELAY_ON   hfim5_do_output(HFIM5_DO_6,HFIM5_DO_VAL_H)
#define HEATER_RELAY_OFF  hfim5_do_output(HFIM5_DO_6,HFIM5_DO_VAL_L)
  
#define ALARM_RELAY_ON   hfim5_do_output(HFIM5_DO_11,HFIM5_DO_VAL_H)
#define ALARM_RELAY_OFF  hfim5_do_output(HFIM5_DO_11,HFIM5_DO_VAL_L)
  
  
  
  
  
  
  
  
  
  
  
  
