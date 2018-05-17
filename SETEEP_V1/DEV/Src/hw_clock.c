/**********************************************
;**********************************************
;***      ��ʱ�� 
;***      Author: ����
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

/*���ܣ��ڲ�ʱ�亯��
  ���룺��
  ���أ���
  ��д�ˣ�����
  ʱ�䣺2014-5-12*/
void F_Time_check(void)
{
  u100ms = 0;
  u1s = 0;
  
  u10msMain = u10msInt;  /*ע��˴�u10msMainһֱ��1������ֱ��ʹ��*/
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

