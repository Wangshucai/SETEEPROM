/**********************************************
;**********************************************
;***     modbus通讯函数
;***      Author: 李鹏
;***      Date:   2015-02-15
;**********************************************
;**********************************************/
#define  MBAPP
#include "mbapp.h"
#include "logic.h"
#include "hw_uart.h"
#include <string.h>

const char *SW_Str = "902VD-A";
const char *HW_Str = "HFIM4-V1";

/*********************************************
=================modbus??=====================
***********************************************/ 
uint16_t MB_RdHoding(uint16_t slave_node,uint8_t type,uint16_t reg, uint16_t *perr) {
  *perr = MB_ERR_NONE;  
  
  uint8_t *p;
  uint8_t sz = 0;
  uint16_t val = 0;
  uint8_t tmp = 0;
  if ((reg >= 1) && (reg <= 5)) {
     
      sz = strlen(SW_Str);
      reg = (reg - 1) * 2;
      if (reg >= sz)
      return 0;
      else {
        p = (uint8_t *)SW_Str + reg;
        val = *p;
        reg++;
        if (reg >= sz)
          return val;
        else {
          p++;
          return val | ((*p) << 8);
        }
      }
    }
    else if ((reg >= 6) && (reg <= 10)) {
      sz = strlen(HW_Str);
      reg = (reg - 6) * 2;
      if (reg >= sz)
      return 0;
      else {
        p = (uint8_t *)HW_Str + reg;
        val = *p;
        reg++;
        if (reg >= sz)
          return val;
        else {
          p++;
          return val | ((*p) << 8);
        }
      }
    }
    
    else if ((reg >= 1001) && (reg <= 1005)) {
     
      sz = strlen((const char *)gSysStatusPara.SoftwareVer_OutBoard);
      reg = (reg - 1001) * 2;
      if (reg >= sz)
      return 0;
      else {
        p = (uint8_t *)gSysStatusPara.SoftwareVer_OutBoard + reg;
        val = *p;
        reg++;
        if (reg >= sz)
          return val;
        else {
          p++;
          return val | ((*p) << 8);
        }
      }
    }
    else if ((reg >= 1006) && (reg <= 1010)) {
      sz = strlen((const char *)gSysStatusPara.HardwareVer_OutBoard);
      reg = (reg - 1006) * 2;
      if (reg >= sz)
      return 0;
      else {
        p = (uint8_t *)gSysStatusPara.HardwareVer_OutBoard + reg;
        val = *p;
        reg++;
        if (reg >= sz)
          return val;
        else {
          p++;
          return val | ((*p) << 8);
        }
      }
    }
    
    else if(reg >= 2001&&reg <= 2012)
    {
    if(slave_node != 0x40)  /*室外机读取*/
      DEBUG_BUF_STA = 0;
    switch(reg)
    {
      case 2001:  return MODE_TEST;      //状态模式
      case 2002:  return HAND_PUMP;      //水泵控制
      case 2003:  return HAND_HEARTER;   //电加热控制
      case 2004:  return HAND_BYPASS_VALVE;//液旁通电磁阀
      case 2005:  return HAND_SAVE_VALVE;  //经济器电磁阀
      case 2006:  return HAND_HEATER_COM;  //底盘加热带
      case 2007:  return HAND_OUT_FAN;     //外风机控制
      case 2008:  return HAND_FOUR_WAY_VALVE;    //四通阀控制
      case 2009:  return gTmpDebugPara.ComHZ;    //压缩机频率  测试状态
      case 2010:  return gTmpDebugPara.EEVNum;   //EEV开度
      case 2011:  return gTmpDebugPara.TestMode; //测试模式
      case 2012:  return gTmpDebugPara.ByPassEEVNum;//辅助EEV开度
    }
  }
  else if(reg >= 3001&&reg <= 3005)
  {
    if(slave_node != 0x40)
      USER_BUF_STA = 0;
    switch(reg)
    {
      case 3001:  return POWER_SWITCH | (EN_LOCAL << 1);  //开关机
      case 3002:  return gTmpUserPara.SetMode;            //设定模式
      case 3003:  return gTmpUserPara.SetCoolTemp;        //制冷设定温度
      case 3004:  return gTmpUserPara.SetHeatTemp;        //制热设定温度
      case 3005:  return EN_POWER;                        //来电自启状态
    }
  }
  else if(reg >= 4001&&reg <= 4026)
  {
    if(slave_node != 0x40)  /*室外机读取*/
      OUT_BUF_STA = 0;
    switch(reg)
    {
    case 4001:  
      if(type == Holding)    return gTmpOutPara.DefrostSetTemp;     //化霜设定温度
    case 4002:
      if(type == Holding)    return gTmpOutPara.DefrostComHZ;       //化霜频率
    case 4003:
      if(type == Holding)    return gTmpOutPara.DefrostEEVStep1;     //化霜EEV
    case 4004:
      if(type == Holding)    return gTmpOutPara.DefrostEEVStep2; 
    case 4005:
      if(type == Holding)    return gTmpOutPara.DefrostEEVStep3;
 
    case 4006:  
      if(type == Holding)    return gTmpOutPara.DefrostMinRunTime;    //化霜最小时间
    case 4007:
      if(type == Holding)    return gTmpOutPara.DefrostMaxRunTime;    //化霜最大时间
    case 4008:
      if(type == Holding)    return gTmpOutPara.ProduceLineComHZ;     //产线测试频率
    case 4009:
      if(type == Holding)    return gTmpOutPara.ProduceLineEEVStep;   //产线测试EEV设置
    case 4010:
      if(type == Holding)    return gTmpOutPara.EEVAdjustTime;         //电子膨胀阀调节周期
    case 4011:
      if(type == Holding)    return gTmpOutPara.ComMinStopTime;        //最小停机时间
    case 4012:
      if(type == Holding)    return gTmpOutPara.ComMinRunTime;         //最小运行时间
    case 4013:
      if(type == Holding)    return gTmpOutPara.ComObjHZCalTime;       //目标频率计算周期
    case 4014:
      if(type == Holding)    return gTmpOutPara.ReturnFreonHZ;         //回油频率
    case 4015:
      if(type == Holding)    return gTmpOutPara.ReturnFreonRunTime;    //回油时间
    case 4016:
      if(type == Holding)    return gTmpOutPara.IntallTestTime;        //安装测试时间
    case 4017:
      if(type == Holding)    return gTmpOutPara.HandTestTime;          
    case 4018:   
      if(type == Holding)    return gTmpOutPara.ProduceTestTime;        
    case 4019:
      if(type == Holding)    return gTmpOutPara.Deforst_modulus;      //化霜系数
    case 4020:
      if(type == Holding)    return gTmpOutPara.EnEEP2;                //EEP2使能
    case 4021:
      if(type == Holding)    return gTmpOutPara.AdjustMHz;             //最值修正值
    case 4022:
      if(type == Holding)    return gTmpOutPara.BigLoadDiff;            //大负荷温差
    case 4023:
      if(type == Holding)    return gTmpOutPara.SmallLoadDiff;          //小符合温差
    case 4024:
      if(type == Holding)    return gTmpOutPara.LoadCalWay;              //符合温差计算
    case 4025:
      if(type == Holding)    return gTmpOutPara.ProduceByPassEEVStep;   //产线测试辅助EEV开度
    case 4026:
      if(type == Holding)    return gTmpOutPara.FanType;                 //风机类型
    }
  }
  
  else if(reg >= 5001&&reg <= 5019)
  {
    if(slave_node != 0x40)  /*室外机读取*/
      OUT2_BUF_STA = 0;
    switch(reg)
    {
    case 5001:  
      if(type == Holding)    return gTmpOut2Para.HeaterResonHZ1;
    case 5002:
      if(type == Holding)    return gTmpOut2Para.HeaterResonHZ2;
    case 5003:
      if(type == Holding)    return gTmpOut2Para.HeaterResonHZ3;
    case 5004:
      if(type == Holding)    return gTmpOut2Para.HeaterResonHZ4; 
    case 5005:
      if(type == Holding)    return gTmpOut2Para.HeaterResonHZ5;
 
    case 5006:  
      if(type == Holding)    return gTmpOut2Para.CoolResonHZ1;  
    case 5007:
      if(type == Holding)    return gTmpOut2Para.CoolResonHZ2;
    case 5008:
      if(type == Holding)    return gTmpOut2Para.CoolResonHZ3;
    case 5009:
      if(type == Holding)    return gTmpOut2Para.CoolResonHZ4;
    case 5010:
      if(type == Holding)    return gTmpOut2Para.CoolResonHZ5;
    case 5011:
      if(type == Holding)    return gTmpOut2Para.SetComI1;
    case 5012:
      if(type == Holding)    return gTmpOut2Para.SetComI2;
    case 5013:
      if(type == Holding)    return gTmpOut2Para.SetComI3;
    case 5014:
      if(type == Holding)    return gTmpOut2Para.SetIPM1;
    case 5015:
      if(type == Holding)    return gTmpOut2Para.SetIPM2;
    case 5016:
      if(type == Holding)    return gTmpOut2Para.SetIPM3;  
    case 5017:
      if(type == Holding)    return gTmpOut2Para.SetPFCI1;
    case 5018:
      if(type == Holding)    return gTmpOut2Para.SetPFCI2;
    case 5019:
      if(type == Holding)    return gTmpOut2Para.SetPFCI3;
    }
  }
   else if(reg >= 7001&&reg <= 7058)
  {
    switch(reg)
    {
    case 7001:  
      if(type == Holding)    return gSysStatusPara.OutParaBuf.OutTemp;        //室外温度
      return COM_HEATER_STA;
    case 7002:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ExhaultTemp;    //排气温度
      return COOL_FAN_HIGH_STA;
    case 7003:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.InhaleTemp;     //吸气温度
      return COOL_FAN_MID_STA;
    case 7004:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.DefrostTemp;    //化霜温度
      return COOL_FAN_LOW_STA;
    case 7005:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.CondInTemp;     //低饱和温度
      return FIX_HZ_COM_STA;
 
    case 7006:  
      if(type == Holding)    return gSysStatusPara.OutParaBuf.MachType;       //型号
      return SAVE_ENERGY_VALVE_STA;
    case 7007:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ActHZ;          //运行频率
      return HEATER_CHASS_STA;
    case 7008:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.OBJHZ;          //目标频率
      return PUMP_STA;
    case 7009:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.CalCapa;        //计算能力
      return FOUR_WAY_VALVE_STA;
    case 7010:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ExhSuperHeat;    //排气过热度
      return LIP_PUMP_VALVE_STA;
    case 7011:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.InhalSuperHeat;  //吸气过热度
      return ERR_STA;
    case 7012:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.EEVStep;         //EEV开度
      return DO12_STA;
    case 7013:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.SysSta;          //故障状态
    case 7014:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ComI;             //压缩机电流
    case 7015:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ACI;              //AC电流
    case 7016:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.IPMTemp;      
    case 7017:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.PFCTemp;         //PFC温度
      return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit00;                  //高压保护
    case 7018:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ACVol;             //AC电压
      return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit01;                   //低压保护
    case 7019:
      if(type == Holding)    return gSysStatusPara.OutParaBuf.ExhProtectLevel;   //排气保护等级
      return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit02;                   //排气温度过高保护
    case 7020:
      if(type == Holding)   return gSysStatusPara.OutParaBuf.ComProtectLevel;    //压缩机保护等级
      return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit03;
      case 7021:
        if(type == Holding)
            return gSysStatusPara.OutParaBuf.IPMProtectLevel;                   //IPM保护等级
        return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit04;
      case 7022:
        if(type == Holding)
          return gSysStatusPara.OutParaBuf.PFCProtectLevel;                    //PFC保护等级
        return 
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit05;
      case 7023:
        if(type == Holding)
           return gSysStatusPara.OutParaBuf.ACIProtectLevel;                   //AC电流保护等级
        return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit06;
      case 7024:
        if(type == Holding)  return gSysStatusPara.OutParaBuf.Tn;              //Tn  整体机上为化霜间隔时间
        return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit07;
      case 7025:
        if(type == Holding) return gSysStatusPara.OutParaBuf.DISta[0].uiBuf;   
        return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit08;
      case 7026:
        if(type == Holding) return gSysStatusPara.OutParaBuf.DISta[1].uiBuf;
        return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit09;
      case 7027:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit10;           //低压压力
       return gSysStatusPara.OutParaBuf.LowPressNum;
      case 7028:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit11;            //低压防冻保护等级
        return gSysStatusPara.OutParaBuf.LowFarFreezeLV;
      case 7029:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit12;
        return gSysStatusPara.OutParaBuf.ByPassOutTemp;                        //辅路出口温度
      case 7030:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit13;
        return gSysStatusPara.OutParaBuf.ByPassInTemp;                         //辅路进口温度
      case 7031:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit14;
        return gSysStatusPara.OutParaBuf.ByPassEEVStep;                        //辅阀开度
      case 7032:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit15;
        return gSysStatusPara.OutParaBuf.DriverType;                           //驱动类型
      case 7033:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit00;
      case 7034:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit01;
      case 7035:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02;
      case 7036:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit03;
       
      case 7037:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit04; 
      case 7038:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit05;
      case 7039:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit06;
      case 7040:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit07;
      case 7041:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit08;
      case 7042:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit09;
        
      case 7043:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit10; 
      case 7044:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit11;
      case 7045:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit12;
      case 7046:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit13;
      case 7047:
        if(type == Coil)
        return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit14;
      case 7048:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit15;
      case 7049:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit00;
      case 7050:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit01;
      case 7051:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit02;  
      case 7052:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit03;
      case 7053:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit04;
      case 7054:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit05;
      case 7055:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit06;
      case 7056:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit07;
      case 7057:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit08;
      case 7058:
        if(type == Coil)
          return gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit09;
    }
  }
  
  else if(reg >= 9001&&reg <= 9012)
  {
    switch(reg)
    {
    case 9001:  
      if(type == Holding)    return gSysStatusPara.INParaBuf.ReturnTemp;
      return gSysStatusPara.INParaBit.RelaySta.Field.bit00;
    case 9002:
      if(type == Holding)    return gSysStatusPara.INParaBuf.InTemp;
      return gSysStatusPara.INParaBit.RelaySta.Field.bit01;
    case 9003:
      if(type == Holding)    return gSysStatusPara.INParaBuf.FreonInTemp;
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit00 ;
    case 9004:
      if(type == Holding)    return gSysStatusPara.INParaBuf.FreonOutTemp; 
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit01;
    case 9005:
      if(type == Holding)    return gSysStatusPara.MachSta.ucBuf;
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit02;
 
    case 9006:  
      if(type == Holding)    return gSysStatusPara.INParaBuf.PowerSta;  //开关机
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit03; 
    case 9007:
      if(type == Holding)    return gTmpUserPara.SetMode;                
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit04;
    case 9008:
      if(type == Holding)    return gTmpUserPara.SetCoolTemp;
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit05;
    case 9009:
      if(type == Holding)    return gTmpUserPara.SetHeatTemp;
      return gSysStatusPara.INParaBit.ErrInfo.Field.bit06 ;
    case 9010:
      if(type == Coil)
        return gSysStatusPara.INParaBit.ErrInfo.Field.bit07;
    case 9011:
      if(type == Coil)
        return gSysStatusPara.INParaBit.ErrInfo.Field.bit08;
  case 9012:
      if(type == Coil)
        return DH_COMM_STA;
    }
  }
  
  else if(reg >= 8001&&reg <= 8002)
  {
    switch(reg)
    {
    case 8001: return gSysStatusPara.OutCommBit.uiBuf;
    case 8002: return INIT_STA;
    }
  }  
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;
    return 0;
  }
  return 0;
}


/*********************************************
=================MODBUS??====================
***********************************************/ 
void MB_WrHoding(uint16_t slave_node, uint8_t type,uint16_t reg, uint16_t reg_val, uint16_t *perr)
{
 uint8_t tmp = 0;
  if(reg >= 1001 && reg <= 1010)
  {
    OUT_VER_STA = 1;
    HAVE_OUT_VER = 1;
    switch(reg)
    {
   case 1001:
      gSysStatusPara.SoftwareVer_OutBoard[0] = reg_val&0xFF;
      gSysStatusPara.SoftwareVer_OutBoard[1] = (reg_val>>8)&0xFF;
      break;
    case 1002:
      gSysStatusPara.SoftwareVer_OutBoard[2] = reg_val&0xFF;
      gSysStatusPara.SoftwareVer_OutBoard[3] = (reg_val>>8)&0xFF;
      break;
    case 1003:
      gSysStatusPara.SoftwareVer_OutBoard[4] = reg_val&0xFF;
      gSysStatusPara.SoftwareVer_OutBoard[5] = (reg_val>>8)&0xFF;
      break;
    case 1004:  
      gSysStatusPara.SoftwareVer_OutBoard[6] = reg_val&0xFF;
      gSysStatusPara.SoftwareVer_OutBoard[7] = (reg_val>>8)&0xFF;      
      break;     
    case 1005:  
      gSysStatusPara.SoftwareVer_OutBoard[8] = reg_val&0xFF;
      gSysStatusPara.SoftwareVer_OutBoard[9] = (reg_val>>8)&0xFF;     
      break;      
    case 1006:
      gSysStatusPara.HardwareVer_OutBoard[0] = reg_val&0xFF;
      gSysStatusPara.HardwareVer_OutBoard[1] = (reg_val>>8)&0xFF;
      break;
    case 1007:
      gSysStatusPara.HardwareVer_OutBoard[2] = reg_val&0xFF;
      gSysStatusPara.HardwareVer_OutBoard[3] = (reg_val>>8)&0xFF;
      break;
    case 1008:
      gSysStatusPara.HardwareVer_OutBoard[4] = reg_val&0xFF;
      gSysStatusPara.HardwareVer_OutBoard[5] = (reg_val>>8)&0xFF;
      break;
    case 1009:  
      gSysStatusPara.HardwareVer_OutBoard[6] = reg_val&0xFF;
      gSysStatusPara.HardwareVer_OutBoard[7] = (reg_val>>8)&0xFF;      
      break;     
    case 1010:  
      gSysStatusPara.HardwareVer_OutBoard[8] = reg_val&0xFF;
      gSysStatusPara.HardwareVer_OutBoard[9] = (reg_val>>8)&0xFF;     
      break;  
    }
    *perr = MB_ERR_NONE;
    return;
  }
  else if(reg >= 2001 && reg <= 2012)
  {
    DEBUG_BUF_STA = 1;
    switch(reg)
    {
    case 2001:
      MODE_TEST = reg_val;
      break;
    case 2002:
      HAND_PUMP = reg_val;
      break;
    case 2003:
      HAND_HEARTER = reg_val;
      break;
    case 2004:
      HAND_BYPASS_VALVE = reg_val;
      break;
    case 2005:
      HAND_SAVE_VALVE = reg_val;
      break;
    case 2006:
      HAND_HEATER_COM = reg_val;
      break;
    case 2007:
      HAND_OUT_FAN = reg_val;
      break;
    case 2008:
      HAND_FOUR_WAY_VALVE = reg_val;
      break;
    case 2009:
      gTmpDebugPara.ComHZ = reg_val;
      break;
    case 2010:
      gTmpDebugPara.EEVNum = reg_val;
      break;
    case 2011:
      gTmpDebugPara.TestMode = reg_val;
      break;
    case 2012:
      gTmpDebugPara.ByPassEEVNum = reg_val;
      break;
    }
    *perr = MB_ERR_NONE;
    return;
  }
  else if((reg >= 3001) && (reg <= 3005))
  {
//    USER_BUF_STA = 1; 室外板不读取此参数，室外板根据状态参数计算
    if(slave_node == 0x40)  /*只支持手操器写入*/
    {
      gSysStatusPara.EEDly  = 2;
		
      switch(reg){
      case 3001:
        
      POWER_SWITCH = reg_val & 0x01 ? 1 : 0;
        break;
      case 3002:
        gTmpUserPara.SetMode = reg_val;
        break;
      case 3003:
        gTmpUserPara.SetCoolTemp = reg_val;
        break;
      case 3004:
        gTmpUserPara.SetHeatTemp = reg_val;
        break;
      case 3005:
        EN_POWER = reg_val ? 1 : 0;   //来电自启动
        break;
      }
    }    
    *perr = MB_ERR_NONE;
    return;
  }
  else if(reg >= 4001 && reg <= 4026)
  {       
    if(slave_node == 0x40)  /*读取手操器数值*/
      OUT_BUF_STA = 1;
    else                  /*室外板写入数据*/
    {
      OUT_SET_STA = 1;

      HAVE_OUT_BUF = 1;
    }     
    switch(reg)
    {
    case 4001:
      gTmpOutPara.DefrostSetTemp = reg_val;
      break;
    case 4002:
      gTmpOutPara.DefrostComHZ = reg_val;
      break;
    case 4003:
      gTmpOutPara.DefrostEEVStep1 = reg_val;
      break;
    case 4004:
      gTmpOutPara.DefrostEEVStep2 = reg_val;
      break;
    case 4005:
      gTmpOutPara.DefrostEEVStep3 = reg_val;
      break;
    case 4006:
      gTmpOutPara.DefrostMinRunTime = reg_val;
      break;
    case 4007:
      gTmpOutPara.DefrostMaxRunTime = reg_val;
      break;
    case 4008:
      gTmpOutPara.ProduceLineComHZ = reg_val;
      break;
    case 4009:
      gTmpOutPara.ProduceLineEEVStep = reg_val;
      break;
    case 4010:
      gTmpOutPara.EEVAdjustTime = reg_val;
      break;
    case 4011:
      gTmpOutPara.ComMinStopTime = reg_val;
      break;
    case 4012:
      gTmpOutPara.ComMinRunTime = reg_val;
      break;
    case 4013:
      gTmpOutPara.ComObjHZCalTime = reg_val;
      break;
    case 4014:
      gTmpOutPara.ReturnFreonHZ = reg_val;
      break;
    case 4015:
      gTmpOutPara.ReturnFreonRunTime = reg_val;
      break;
    case 4016:
      gTmpOutPara.IntallTestTime = reg_val;
      break;
    case 4017:
      gTmpOutPara.HandTestTime = reg_val;
      break;
    case 4018:
      gTmpOutPara.ProduceTestTime = reg_val;
      break;
    case 4019:
      gTmpOutPara.Deforst_modulus = reg_val;
      break;   
    case 4020:
      gTmpOutPara.EnEEP2 = reg_val; 
      break;
    case 4021:
      gTmpOutPara.AdjustMHz = reg_val; 
      break;
    case 4022:
      gTmpOutPara.BigLoadDiff = reg_val; 
      break;
    case 4023:
      gTmpOutPara.SmallLoadDiff = reg_val; 
      break;
    case 4024:
      gTmpOutPara.LoadCalWay = reg_val; 
      break;
    case 4025:
      gTmpOutPara.ProduceByPassEEVStep = reg_val; 
      break;
    case 4026:
      gTmpOutPara.FanType = reg_val; 
      break;
      
    }
    *perr = MB_ERR_NONE;
    return;    
  }
  
  else if(reg >= 5001 && reg <= 5019)
  {       
    if(slave_node == 0x40)  /*读取手操器数值*/
      OUT2_BUF_STA = 1;
    else                  /*室外板写入数据*/
    {
      OUT2_SET_STA = 1;

      HAVE_OUT2_BUF = 1;
    }  
    switch(reg)
    {
    case 5001:
      gTmpOut2Para.HeaterResonHZ1 = reg_val;
      break;
    case 5002:
      gTmpOut2Para.HeaterResonHZ2 = reg_val;
      break;
    case 5003:
      gTmpOut2Para.HeaterResonHZ3 = reg_val;
      break;
    case 5004:
      gTmpOut2Para.HeaterResonHZ4 = reg_val;
      break;
    case 5005:
      gTmpOut2Para.HeaterResonHZ5 = reg_val;
      break;
    case 5006:
      gTmpOut2Para.CoolResonHZ1 = reg_val;
      break;
    case 5007:
      gTmpOut2Para.CoolResonHZ2 = reg_val;
      break;
    case 5008:
      gTmpOut2Para.CoolResonHZ3 = reg_val;
      break;
    case 5009:
      gTmpOut2Para.CoolResonHZ4 = reg_val;
      break;
    case 5010:
      gTmpOut2Para.CoolResonHZ5 = reg_val;
      break;
    case 5011:
      gTmpOut2Para.SetComI1 = reg_val;
      break;
    case 5012:
      gTmpOut2Para.SetComI2 = reg_val;
      break;
    case 5013:
      gTmpOut2Para.SetComI3 = reg_val;
      break;
    case 5014:
      gTmpOut2Para.SetIPM1 = reg_val;
      break;
    case 5015:
      gTmpOut2Para.SetIPM2 = reg_val;
      break;
    case 5016:
      gTmpOut2Para.SetIPM3 = reg_val;
      break;
    case 5017:
      gTmpOut2Para.SetPFCI1 = reg_val;
      break;
    case 5018:
      gTmpOut2Para.SetPFCI2 = reg_val;
      break;
    case 5019:
      gTmpOut2Para.SetPFCI3 = reg_val;
      break;    

      
    }
    *perr = MB_ERR_NONE;
    return;    
  }
  
  else if(reg >= 7001&&reg <= 7058)
  {

    switch(reg)
    {
      case 7001:
        if(type == Holding)
        {
          gSysStatusPara.OutParaBuf.OutTemp = reg_val;
        }
        else if(type == Coil)
        {
          COM_HEATER_STA = reg_val;
        }
        break;
      case 7002:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ExhaultTemp = reg_val;
        else if(type == Coil)
        {
          COOL_FAN_HIGH_STA = reg_val;
        }
        break;
      case 7003:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.InhaleTemp = reg_val;
        else if(type == Coil)
        {
          COOL_FAN_MID_STA = reg_val;
        }
        break;
      case 7004:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.DefrostTemp = reg_val;
        else if(type == Coil)
        {
          COOL_FAN_LOW_STA = reg_val;
        }
        break;
      case 7005:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.CondInTemp = reg_val;
        else if(type == Coil)
        {
          FIX_HZ_COM_STA = reg_val;
        }
        break;
      case 7006:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.MachType = reg_val;
        else if(type == Coil)
        {
          SAVE_ENERGY_VALVE_STA = reg_val;
        }
        break;
      case 7007:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ActHZ = reg_val;
        else if(type == Coil)
        {
          HEATER_CHASS_STA = reg_val;
        }
        break;
      case 7008:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.OBJHZ = reg_val;
        else if(type == Coil)
        {
          PUMP_STA = reg_val;
        }
        break;
      case 7009:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.CalCapa = reg_val;
        else if(type == Coil)
        {
          FOUR_WAY_VALVE_STA = reg_val;
        }
        break;
      case 7010:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ExhSuperHeat = reg_val;
        else
        {
          LIP_PUMP_VALVE_STA = reg_val;
        }
        break;
      case 7011:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.InhalSuperHeat = reg_val;
        else
        {
          ERR_STA = reg_val;
        }
        break;
      case 7012:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.EEVStep = reg_val;
        else
        {
          DO12_STA = reg_val;
        }
        break;
      case 7013:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.SysSta = reg_val;
        else
        {
          
        }
        break;
      case 7014:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ComI = reg_val;
        else
        {
          
        }
        break;
      case 7015:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ACI = reg_val;
        else
        {
          
        }
        break;
      case 7016:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.IPMTemp = reg_val;
        else
        {
          
        }
        break;      
      case 7017:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.PFCTemp = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit00 = reg_val;
        }
        break;      
      case 7018:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ACVol = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit01 = reg_val;
        }
        break;
      case 7019:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ExhProtectLevel = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit02 = reg_val;
        }
        break;
      case 7020:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ComProtectLevel = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit03 = reg_val;
        }
        break;
      case 7021:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.IPMProtectLevel = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit04 = reg_val;
        }
        break;
      case 7022:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.PFCProtectLevel = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit05 = reg_val;
        }
        break;
      case 7023:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.ACIProtectLevel = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit06 = reg_val;
        }
        break;
      case 7024:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.Tn = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit07 = reg_val;
        }
        break;
      case 7025:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.DISta[0].uiBuf = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit08 = reg_val;
        }
        break;
      case 7026:
        if(type == Holding)
          gSysStatusPara.OutParaBuf.DISta[1].uiBuf = reg_val;
        else
        {
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit09 = reg_val;
        }
        break;
      case 7027:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit10 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.LowPressNum = reg_val;
        }
      break;
      case 7028:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit11 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.LowFarFreezeLV = reg_val;
        }
      break;
      case 7029:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit12 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.ByPassOutTemp = reg_val;
        }
      break;
      case 7030:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit13 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.ByPassInTemp = reg_val;
        }
      break;
      case 7031:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit14 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.ByPassEEVStep = reg_val;
        }
      break;
      case 7032:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit15 = reg_val;
        else
        {
          gSysStatusPara.OutParaBuf.DriverType = reg_val;
        }
      break;
      case 7033:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit00 = reg_val;
      break;     
      case 7034:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit01 = reg_val;
      break;
      case 7035:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02 = reg_val;
      break;
      case 7036:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit03 = reg_val;
      break;
       
      case 7037:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit04 = reg_val;
      break;     
      case 7038:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit05 = reg_val;
      break;
      case 7039:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit06 = reg_val;
      break;
      case 7040:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit07 = reg_val;
      break;
      case 7041:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit08 = reg_val;
        break;
      case 7042:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit09 = reg_val;
        break;
        

      case 7043:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit10 = reg_val;
      break;
      case 7044:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit11 = reg_val;
      break;
      case 7045:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit12 = reg_val;
        break;
      case 7046:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit13 = reg_val;
        break;
      case 7047:
        if(type == Coil)
        gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit14 = reg_val;
        break;
      case 7048:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit15 = reg_val;
        break;
      case 7049:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit00 = reg_val;
        break;
      case 7050:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit01 = reg_val;
        break;
      case 7051:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit02 = reg_val;
        break;
        
       
      case 7052:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit03 = reg_val;
        break;
     
      case 7053:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit04 = reg_val;
        break;
       
      case 7054:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit05 = reg_val;
        break;
       
      case 7055:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit06 = reg_val;
        break;
       
      case 7056:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit07 = reg_val;
        break;
      
      case 7057:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit08 = reg_val;
        break;
      case 7058:
        if(type == Coil)
          gSysStatusPara.OutParaBit.ErrInfo[2].Field.bit09 = reg_val;
        break;

    }
    *perr = MB_ERR_NONE;
    return; 
  }

  else if(reg == 8001)
  {
    if(slave_node == 0x40)  
      gSysStatusPara.LwcCommBit.uiBuf = reg_val;
    else
      gSysStatusPara.OutCommBit.uiBuf = reg_val; 
    *perr = MB_ERR_NONE;
    return; 
  }
  else if(reg == 8002)
  {
    
    if(slave_node == 0x40)  
    {
      tmp = (reg_val & 0X02) ? 1 : 0;
      CLR_ERR |= tmp;
      OUT_CLR_ERR = CLR_ERR;
      tmp = (reg_val & 0X01) ? 1 : 0;
      RESET_PARA |= tmp;   
      OUT_RESET_PARA =    RESET_PARA;   
    }
    else
      INIT_STA = reg_val ? 1 : 0;
    *perr = MB_ERR_NONE;
    return; 
  }
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;
  }
}




























