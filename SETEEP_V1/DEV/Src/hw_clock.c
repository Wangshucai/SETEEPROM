/**********************************************
;**********************************************
;***      定时器 
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define HW_CLOCK

#include "hw_clock.h"
#include "hw_uart.h"
#include "lwcComm.h"
#include "outComm.h"
#include "dh_app.h"

void OptSys(void);  /*10MS*/

/*功能：内部时间函数
  输入：无
  返回：无
  编写人：李鹏
  时间：2014-5-12*/
void F_Time_check(void)
{
  u100ms = 0;
  u1s = 0;
  
  u10msMain = u10msInt;  /*注意此次u10msMain一直是1，不能直接使用*/
  u10msInt = 0; 
  uc100msctr  += u10msMain;
	
  if(uc100msctr < 10) return ; /*10ms*/
  uc100msctr = 0;
  u100ms = 1;
  uc1sctr ++;
  if(uc1sctr < 10) return; 
  uc1sctr = 0;
  u1s = 1;
}

void u10msRunTask(void)
{
  if(u10msMain)
  {
    OptSys();
//    Uart4_Tmr();
//    Uart3_Tmr();
//    Uart2_Tmr();
//    DH_Slave_Task(&MB_Params[HFIM5_UART_4]);
//    Uart_Master_Pool(&MB_Params[HFIM5_UART_3]);
//    UART_Slave_Task(&MB_Params[HFIM5_UART_2]);
  }

}

