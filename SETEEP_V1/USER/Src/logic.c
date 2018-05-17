/**********************************************
;**********************************************
;***      ¬ﬂº≠øÿ÷∆
;***      Author: ¿Ó≈Ù
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define LOGIC

//#define __AD_DEBUG



#include "logic.h"
#include "task.h"
//#include "conv_ad_t_10k.h"
//#include "hw_at24cxx.h"
#include "hw_gpio.h"
//#include "hw_error.h"
//#include "dh_app.h"



extern uint8_t Data[64];
extern uint8_t DataRead[64];
int count = 0;
int SetFinish = 0;
int NumCount = 0;
int ERRCount = 0;
int ERRPasg = 5;

void EEPROMSET ()
{
   
	
	if(count == 256)
	{
	  SetFinish = 1;
	}
	
	if((ES_READY ==hfim5_eeprom_get_state()) && (count <256))
	{
		
	   hfim5_eeprom_page_write(64, (uint8_t *)Data,sizeof(Data));
		count ++;
	}
	
	
	
}

void EEPROMREAD()
{
	
	if(SetFinish)
	{
	  if((ES_READY ==hfim5_eeprom_get_state()) && (NumCount < 256))
	  {
		  hfim5_eeprom_read(NumCount *64, (uint8_t *)DataRead,sizeof(DataRead)); 
		  
	  }
	}
	
}




void ReadCrc()
{
	int s;

	
	if((ES_READY ==hfim5_eeprom_get_state()) && SetFinish && NumCount < 256)
	{
		
	    for(s = 0;s < 64; s++)
		{
			 
			
			if(DataRead[s] == 0XFF)
			{
			  ERRCount ++;
			  if(ERRCount == 16384)
			  {
			    RED_OFF;
				GREEN_ON;
			  }
			}
			DataRead[s] = 0;
		}
		
		 NumCount ++;
	}
	
   
}

void Test()
{
   int t;
	for(t = 0;t < 0xFFFF;t++);
}


void Init_Thread(void)
{
 
	
     AddThreadList(TMR_TYPE_10MS,EEPROMSET);
     AddThreadList(TMR_TYPE_10MS,EEPROMREAD);
	 AddThreadList(TMR_TYPE_10MS,Test);
     AddThreadList(TMR_TYPE_10MS,ReadCrc);
  

}

