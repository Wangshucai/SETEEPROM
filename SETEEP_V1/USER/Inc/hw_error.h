/**********************************************
;**********************************************
;***      GPIO�ܽ����� ͷ�ļ�
;***      Author: ����
;***      Date:   2016-11-12
;**********************************************
;**********************************************/
#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "bsp.h"

#ifdef  HW_ERROR 
  #define HW_ERROR
#else
  #define HW_ERROR  extern                    
#endif
  
void F_CommOutBoard(void);
  
void check_error(void);
