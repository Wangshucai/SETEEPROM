/**********************************************
;**********************************************
;***      ��ʱ�� 
;***      Author: ����
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define HW_AT24CXX

#include "hw_at24cxx.h"
#include "logic.h"

#include <string.h>



/******************************************************************
*  @������    ��  uint16_t MB_Crc16( uint8_t *pBuf, uint16_t len )                                    
*  @��������  ��  ���������ݽ���CRCУ��
*  @�������  ��  pBuf,len    �����׵�ַ�����ݳ���                 
*  @�������  ��  ����CRCУ����                                        
*******************************************************************/

uint16_t MB_Crc16( uint8_t *pBuf, uint16_t len ) {
  uint16_t index; 
  uint8_t hi = 0xFF;
  uint8_t lo = 0xFF;


  while(len--)
  {
      index = lo ^ *(pBuf++);
      lo = (uint8_t)(hi ^ CrcTblHi[index]);
      hi = CrcTblLo[index];
  }
  return (uint16_t)(hi << 8 | lo);
}


/******************************************************************
*  @������    ��  void SetStatusDefault(UserParaType *pParams)                                  
*  @��������  ��  ���û������ָ�Ϊ����Ĭ��ֵ
*  @�������  ��  �û������ṹ��ָ��                    
*  @�������  ��  ��                                        
*******************************************************************/

static void SetStatusDefault(UserParaType *pParams) 
{
  uint16_t i = 0;
  
  pParams->SetCoolTemp = 120;
  pParams->SetHeatTemp = 410;
  pParams->SetMode = 2;
  pParams->DH_Addr = 1;
  pParams->UserSys8Bit.ucBuf = 0;
  i = MB_Crc16((uint8_t *)pParams,sizeof(UserParaType)-2);
  pParams->crc[1] = (uint8_t)(i >> 8);
  pParams->crc[0] = (uint8_t)(i);
}

uint8_t StatusParaCheckError(UserParaType *cfg)
{
  uint16_t crc = 0;
  crc = cfg->crc[1];
  crc <<= 8;
  crc += cfg->crc[0];
  if (crc == MB_Crc16((uint8_t *)cfg,sizeof(UserParaType) - 2)) return(0);
  return(1);
}

/*********************************************
=================��ȡϵͳ����=�û�����===
***********************************************/
uint8_t StatusParaRead(UserParaType *cfg)
{
  uint8_t err = 0;
  if(ES_READY !=hfim5_eeprom_get_state()) return (1);
  hfim5_eeprom_read(SAVE_USR_SETTING_ADDR, (uint8_t*) cfg, sizeof(UserParaType));
  HAL_Delay(10);
  err = StatusParaCheckError(cfg);
  if (err) return(err);
  return(0);/*��ȡ�ɹ�*/
}

/*********************************************
=================д����==�û�����=====
***********************************************/
uint8_t StatusParaWrite(UserParaType *cfg)
{
  uint16_t i = 0;
  i = MB_Crc16((uint8_t *)cfg,sizeof(UserParaType) - 2);
  cfg->crc[1] = (uint8_t)(i>>8);
  cfg->crc[0] = (uint8_t)(i);
  if(ES_READY !=hfim5_eeprom_get_state()) 
  {
    if(ES_ERROR == hfim5_eeprom_get_state())
    {
      if(gSysStatusPara.EECheckNO < 3)
        gSysStatusPara.EECheckNO ++;
      else EEPROM_ERR = 1; 
    }
    return (1);
  }
  hfim5_eeprom_page_write(SAVE_USR_SETTING_ADDR, (uint8_t *)cfg,sizeof(UserParaType));
  return(0);
}


/*********************************************
=================flash��ȡϵͳ����==����һ===========
***********************************************/
uint8_t F_ReadStatusPara(void)
{
  uint8_t err = 0;
  err = StatusParaRead(&gTmpUserPara);
  if(err)
  {
    SetStatusDefault(&gTmpUserPara);   /*����Ĭ�ϲ���ֵ*/
    gSysStatusPara.EEDly = 2;
    return 1;
  }   
  
  memcpy(&gSysUserPara,&gTmpUserPara,sizeof(UserParaType));/*�ڴ�ͬ��tmpcfg -> syscfg*/
  return(err);
}

/*********************************************
=================flashд�����=Set============
***********************************************/
void F_WriteStatusPara(void)   
{

  static uint8_t Wr_Ctrl = 0;
  static uint8_t tree = 0;
  uint8_t err = 0;
  if(RESET_PARA)
  {
    RESET_PARA = 0;
    gSysStatusPara.EEDly = 2;
    LWC_INIT_STA = 1; /*�ָ��û�����*/
    SetStatusDefault(&gTmpUserPara);
    return;
  }
  
  if(1 == EEPROM_ERR) return;
  switch(tree)
  {
    case 0:
      if(gSysStatusPara.EEDly)
      {
        gSysStatusPara.EEDly --;
        if(0 == gSysStatusPara.EEDly) tree = 1;
      }
      break;
    case 1:
      if(memcmp(&gSysUserPara, &gTmpUserPara, sizeof(UserParaType)))
      {
        err = StatusParaWrite(&gTmpUserPara);
        if(ES_ERROR == err)
        {
          if(Wr_Ctrl < 3)
            Wr_Ctrl ++;
          else EEPROM_ERR = 1;
        }
        else if(0 == err)
        {
          memcpy(&gSysUserPara,&gTmpUserPara,sizeof(UserParaType));
          tree = 0;
        }
      }else tree = 0;
      break;
  }
  
}

