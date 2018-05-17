/**********************************************
;**********************************************
;***      ¶¨Ê±Æ÷ 
;***      Author: ÀîÅô
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define HW_GPIO

#include "hw_gpio.h"
#include "logic.h"

void F_DIPCheck(void)//²¦Âë¼ì²â
{
  static uint8_t Check_End =  0;
  if(Check_End)   return;
  SysStaus8BitType dip;
  dip.ucBuf = 0;
  if(input.di.di_17 < 2)
  {
    dip.Field.bit01 = ~input.di.di_17;
    dip.Field.bit00 = ~input.di.di_18;
    dip.Field.bit02 = ~input.di.di_19;
    dip.Field.bit03 = ~input.di.di_20;
    Check_End = 1;
    gSysStatusPara.DIP_Val = 0xFF;
    gSysStatusPara.MachSta.ucBuf = dip.ucBuf;
    if(gTmpUserPara.MachineType != gSysStatusPara.MachSta.Field.func)
    {
      gTmpUserPara.MachineType = gSysStatusPara.MachSta.Field.func;
      gSysStatusPara.EEDly = 2;
      RESET_PARA = 1;
    }
  }
  
  
}



