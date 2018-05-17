/**********************************************
;**********************************************
;***      MAIN 文件
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "bsp.h"
#include "hw_clock.h"
#include "hw_gpio.h"
#include "logic.h"
#include "hw_uart.h"
#include "outComm.h"
#include "task.h"
#include "hw_at24cxx.h"

#ifndef __DEBUG

  #define WD_TIME 1000
#else
  #define WD_TIME 0
#endif


/*10ms定时*/
static void usr_tmr_ontime(void) {
  u10msInt ++;

}



/****************************************************************************
 * 主板初始化数据
 ****************************************************************************/
static const bsp_init_t bsp_init = {
	{WD_TIME},                                                   /*看门狗1000ms*/
	{10000, 10000, usr_tmr_ontime},
	
};

uint8_t Data[64];
uint8_t DataRead[64];
	

int main(void)
{
  int num = 0;

  hfim5_init((bsp_init_t *)&bsp_init, &input);
  hfim5_usr_tmr_run();                                  
  RED_ON;
  GREEN_OFF;
	
  Init_Thread();

	
  for(num = 0;num < 64;num ++)
  {
	Data[num] = 0XFF; 
  }
	
	
	
	
  while(1)
  {
    hfim5_run();
    RunTask();
    F_Time_check();
    u10msRunTask();

  }
}

