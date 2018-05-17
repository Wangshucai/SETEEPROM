/**********************************************
;**********************************************
;***      定时器 
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define HW_ERROR

#include "hw_error.h"
#include "logic.h"

uint8_t record_heater = 0;  //电加热开启标记

void F_CommOutBoard(void)
{
  OUT_COMM_ERR = 0;
//  gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02 = 0;
  if(gSysStatusPara.CommCheck < 30)
  {
    gSysStatusPara.CommCheck ++;
  }    
  else
//    gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02 = 1;
  {
    OUT_COMM_ERR = 1;
  }
}


void F_CheckReturnSensorErr(uint8_t input)
{
  static uint8_t ent = 0;
  if(0 == input)
  {
    ent = 0;
    RETURN_SENSOR_ERR = 0;
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit00 = 0;
    return;
  }
  if(gSysStatusPara.INParaBuf.ReturnTemp <= -32760)
  {
    if(ent < 5) 
      ent ++;
    else 
    {
      RETURN_SENSOR_ERR = 1;
    }
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit00 = 1;
  }
  else
  {
    if(ent) 
      ent --;
    else 
    {
//      gSysStatusPara.INParaBit.ErrInfo.Field.bit00 = 0;
      RETURN_SENSOR_ERR = 0;
    }
  }
}
/*
@函数名：void F_CheckOutSensorErr(uint8_t input)
@输入：input 0和1   0代表手动清除故障  1代表正常检测
@功能：出水温度传感器检测  

*/
void F_CheckOutSensorErr(uint8_t input)
{
  static uint8_t ent = 0;//检测持续时间
  if(0 == input)
  {
    ent = 0;    
    OUT_SENSOR_ERR = 0;
    return;
  }
  
  if(gSysStatusPara.INParaBuf.InTemp <= -32760)
  {
    if(ent < 5) 
      ent ++;
    else 
    {
      OUT_SENSOR_ERR = 1;
    }
  }
  else
  {
    if(ent) 
      ent --;
    else 
    {
      OUT_SENSOR_ERR = 0;
    }
  }
}
/*
@函数名：F_CheckFreonSensorErr(uint8_t input)
@输入：input 0和1   0代表手动清除故障  1代表正常检测
@功能：冷媒进温度传感器检测  

疑问：ent在故障后就不会累加了么？

*/

void F_CheckFreonSensorErr(uint8_t input)
{
  static uint8_t ent = 0;
  if(0 == input)
  {
    ent = 0;
    FREON_SENSOR_ERR = 0;
    return;
  }
  if(gSysStatusPara.INParaBuf.FreonInTemp <= -32760)
  {
    if(ent < 5) 
      ent ++;
    else 
    {
      FREON_SENSOR_ERR = 1;
    }
  }
  else
  {
    if(ent) 
      ent --;
    else 
    {
      FREON_SENSOR_ERR = 0;
    }
  }
}


uint8_t F_CheckMode(void)
{
  if(MODE_TEST != 0x01)
  {
    if(gTmpDebugPara.TestMode == 0x01)  return 1;
    else if(gTmpDebugPara.TestMode == 0x02) return 2;
  }
  return gTmpUserPara.SetMode;
}

void F_ClrPowerFlowProtect(void)
{
  static uint8_t old_power =0xFF;
  if(old_power != gSysStatusPara.INParaBuf.PowerSta)
  {
    old_power = gSysStatusPara.INParaBuf.PowerSta;
    if(0 == gSysStatusPara.INParaBuf.PowerSta)
    {
      FLOW_PROTECT = 0;
    }
  }
}


/*
@函数名：void F_CheckFlowProtect(uint8_t data)
@输入：
@功能：检测水流保护

*/
void F_CheckFlowProtect(uint8_t data)
{
  static uint8_t Step = 0;
  static uint8_t ent = 0;
  static uint8_t ext = 0;
  static uint8_t ent1 = 0;
  static uint8_t ent2 = 0;
  uint8_t tmp = 0;
	
  if(2 == input.di.di_2)  return;
	
	//下面这是啥？
  if(0 == gSysStatusPara.INParaBit.RelaySta.Field.bit00) /*泵关闭后 ++*/
  {
    F_ClrPowerFlowProtect();
  }
  
   
  if(0 == data)   //清除水流保护
  {
    ent = 0;
    FLOW_PROTECT = 0;
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 0;
    return;
  }
  if(gSysStatusPara.INParaBit.RelaySta.Field.bit00 == 0) /*水泵状态，水泵没开*/   
  {
    Step = input.di.di_2 ? 1 : 2; /* 1: 断开 2： 闭合     检查是否闭合  */
    FLOW_CHECK_END = 0;
    return;
  }
  
  tmp = (MODE_TEST == 0x08) ? 30 : 120;
  if(gSysStatusPara.PumpRunTime > tmp) /*水泵运行时间   水流故障检测完毕*/
  {
    FLOW_CHECK_END = 1;
    if(input.di.di_2)                 /*断开*/
    {
      if(ent1 < 5)
      {
        ent1 ++;
      }
      else
      {
        FLOW_PROTECT = 1;
//        gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
        return;
      }
    }
    else 
    {
      ent1 = 0;
    }
    
  }
  if(1== Step) /*水泵状态 开关断开*/
  {
    if(0 == input.di.di_2) ent = 0;
//    tmp = (MODE_TEST == 0x08) ? 30 : 120;
    if(gSysStatusPara.PumpRunTime > 120 
    && input.di.di_2)          /*断开*/
    {
      if(ent < 5)
        ent ++;
      else 
      {
        FLOW_PROTECT = 1;
        FLOW_CHECK_END = 1;
//        gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
        return;
      }
    }
    
  }
  else if(2 == Step)    /*闭合*/
  {
    if(gSysStatusPara.INParaBit.ErrInfo.uiBuf & 0x03)
    {
      if(gSysStatusPara.PumpRunTime)
      {
        FLOW_PROTECT = 1;
//        gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
        FLOW_CHECK_END = 1;
        return;
      }
      
    }
    else if(0x02 == F_CheckMode())
    {        
      if(gSysStatusPara.ComRunTime > 120
      && 0 == gSysStatusPara.INParaBit.ErrInfo.Field.bit02/*冷媒进口温度*/
      && 0 == gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit05
      && gSysStatusPara.OutParaBuf.ExhaultTemp > -32760
      && gSysStatusPara.INParaBuf.FreonInTemp > -32760)/*排气温度传感器故障*/
      {
        if(gSysStatusPara.OutParaBuf.ExhaultTemp - gSysStatusPara.INParaBuf.FreonInTemp <= 30)
        {
         if(ent < 30)
         {
           ent ++;
           
         }
         else 
         {
           FLOW_PROTECT = 1; 
//             gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
           FLOW_CHECK_END = 1;
           return;
         }
        }
        else
        {
         ent = 0;
        }      
      }
      if(gSysStatusPara.ComRunTime > 30
      && gSysStatusPara.INParaBuf.ReturnTemp > -32760
      && gSysStatusPara.INParaBuf.InTemp > -32760)
      {
        if(gSysStatusPara.INParaBuf.InTemp - gSysStatusPara.INParaBuf.ReturnTemp > 100)
        {
          if(ent2 < 30)
          {
            ent2 ++;
          }        
          else 
          {
            FLOW_PROTECT = 1;
            FLOW_CHECK_END = 1;
  //            gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
            return;
          }
        }
        else
        {
          ent2 = 0;
        }
      }
    }
    else if(0x01 == F_CheckMode())
    {
      if(gSysStatusPara.ComRunTime > 30
      && gSysStatusPara.INParaBuf.ReturnTemp > -32760
      && gSysStatusPara.INParaBuf.InTemp > -32760)
      {
        if(gSysStatusPara.INParaBuf.ReturnTemp - gSysStatusPara.INParaBuf.InTemp > 100)
        {
          if(ent < 30)
          {
            ent ++;
          }        
          else 
          {
            FLOW_PROTECT =  1;
            FLOW_CHECK_END = 1;
  //            gSysStatusPara.INParaBit.ErrInfo.Field.bit04 = 1;
          }
        }
      }    
      else
      {
        ent = 0;
      }      
    }
    else
    {
      ent = 0;
      ent2 = 0;
    }
  }
}

void F_CheckHeaterProtect(uint8_t data)
{
  static uint8_t ent = 0;
  if(input.di.di_5 == 2 || data == 0)
  {
    ent = 0;
    return;
  }
  if(1 == input.di.di_5)
  {
    if(ent < 5)
      ent ++;
    else 
    {
      HEATER_PROTECT = 1;
//      gSysStatusPara.INParaBit.ErrInfo.Field.bit06 = 1;
    }
  }
  else
  {
    if(ent)
      ent --;
    else 
    {
//      gSysStatusPara.INParaBit.ErrInfo.Field.bit06 = 0;
      HEATER_PROTECT = 0;
    }
  }
}
void F_CoolFarAwayFreeze(uint8_t input)
{
  static uint8_t ent = 0;
  if(0 == input || (gSysStatusPara.INParaBuf.InTemp <= -32760))
  {
    ent = 0;
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit05 = 0;
    COOL_FAR_AWAY_FREEZE_ERR = 0;
    return;
  }
  if(0x01 == F_CheckMode() 
    && gSysStatusPara.ComRunTime > 180)
  {
    if(gSysStatusPara.OutParaBuf.CondInTemp >= -300
      && gSysStatusPara.OutParaBuf.CondInTemp <= -10)
    {
      if(ent < 60 )
      {
        ent ++;
      }
      else
      {
//        gSysStatusPara.INParaBit.ErrInfo.Field.bit05 = 1;
        COOL_FAR_AWAY_FREEZE_ERR = 1;
      }
    }
    else
    {
      ent = 0;
    }
  }
  if(COOL_FAR_AWAY_FREEZE_ERR)
  {
    if(gSysStatusPara.INParaBuf.InTemp >= 70)
    {
//      gSysStatusPara.INParaBit.ErrInfo.Field.bit05 = 0;
      COOL_FAR_AWAY_FREEZE_ERR = 0;
    }
      
  }
}
uint8_t F_JudgeSensorErr(void)
{
  SysStaus8BitType sensor;
  sensor.ucBuf = 0;
  sensor.Field.bit00 = gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit07;/*OAT温度传感器*/
  sensor.Field.bit01 = gSysStatusPara.OutParaBit.ErrInfo[0].Field.bit04;/*化霜温度传感器*/
  sensor.Field.bit02 = RETURN_SENSOR_ERR;/*回水温度传感器*/
  sensor.Field.bit03 = OUT_SENSOR_ERR;/*出水温度传感器*/
  return sensor.ucBuf;
}


/*
@函数名：uint8_t F_CheckOutLast(int16_t temp)
@输入：出水温度值
@功能：提供30s连续检测时间
*/
uint8_t F_CheckOutLast(int16_t temp)
{
  static uint8_t time = 0;
  uint8_t tmp = 0;
  if(temp >= -300 && temp <= 30)
  {
    if(time <30)
    {
      time ++;
    }
    else
    {
      tmp = 1;
    }
  }
  else
  {
    time = 0;
  }
  return tmp;
}



void F_WinterFarAwayFreeze(uint8_t input)
{
  static uint16_t ent_time = 1800;
  static uint8_t ent = 0;          //5s计时变量
  static uint8_t ent1 = 0;         //5s计时变量
  static uint8_t Old_err_sta = 0;  //冬季防冻进入标志  0为未进入
  uint8_t sensor = 0;              //传感器故障状态
  uint8_t finish = 0;            //冬季防冻进入方式
  int16_t env1 = 0, env2 = 0;    //进入防冻温度
  uint8_t tmp = 0;               //30s计时变量
	
  sensor = F_JudgeSensorErr();  /*全故障*/
  if(0 == input || 0x0F == sensor || 1 == gSysStatusPara.INParaBuf.PowerSta)
  {
    ent = 0; 
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit07 = 0;
    WINTER_FAR_AWAY_FREEZE = 0;
    FAR_AWAY_FREEZE = 0;
    return;
  }

  env1 = (gSysStatusPara.OutParaBuf.OutTemp > -32760) ? gSysStatusPara.OutParaBuf.OutTemp :gSysStatusPara.OutParaBuf.DefrostTemp;
  env2 = (gSysStatusPara.INParaBuf.InTemp > -32760) ? gSysStatusPara.INParaBuf.InTemp :gSysStatusPara.INParaBuf.ReturnTemp;
  
  tmp = F_CheckOutLast(env2);
  if(record_heater && WINTER_FAR_AWAY_FREEZE)
  {
    if(env2 > 60)
    {
      record_heater = 0;
      WINTER_FAR_AWAY_FREEZE = 0;
    }
    return;
  }
  record_heater = 0;
  
  if(ent_time < 1800)//重新进入检测的条件20分钟和30分钟
  {
    ent_time ++;
    return;
  }
  if(env1 < -32760 
  && (0x03 == (0x03 & sensor)))
  {
    if(env2 < 40 && env2 >= -300)
    {
      finish = 1; /*OAT有故障*/
    }
  }
  else if(env2 < -32760 
  && (0x0C & sensor))
  {
    if(env1 < 0 && env1 >= -300)
    {
      finish = 2;    /*出回水故障*/  
    }
  }
  else
  {
    if(env1 >= -300 & env1 <= -50
      && env2 >- 300 && env2 <= 30)
    {
      if(tmp)
      {
        finish = 3;     /*normal*/
      }
      
    }
  }
  FAR_AWAY_FREEZE = finish ? 1 : 0;

        
  if(finish && 0 == gSysStatusPara.INParaBuf.PowerSta && MODE_TEST == 0x01) 
  {
    WINTER_FAR_AWAY_FREEZE = 1;
    Old_err_sta = finish;
//    gSysStatusPara.INParaBit.ErrInfo.Field.bit07 = 1; /*室内防冻保护*/
  }
  if(FLOW_PROTECT && FAR_AWAY_FREEZE)  /*水流量保护*/ 
  {
	  
    WINTER_FAR_AWAY_FREEZE = 0; /*室内防冻保护*/
	  
  }  
  else if(WINTER_FAR_AWAY_FREEZE)
  {
    if(Old_err_sta >= 1)
    {
      if(gSysStatusPara.PumpRunTime >= 600)//水泵运行十分钟后
      {        
        if(env2 >= 40)
        {   
          ent1 = 0;      
          if(ent < 5)
          {
            ent ++;
          }
          else
          {
            ent_time = 0;
            WINTER_FAR_AWAY_FREEZE = 0;
          }
        }
        else if(env2 > 20)
        {
          ent = 0;
          if(ent1 < 5)
          {
            ent1 ++;
          }
          else
          {
            ent_time = 600;
            WINTER_FAR_AWAY_FREEZE = 0;
          }
        }
        else
        {
          ent = 0;
          ent1 = 0;  
			
          if(gSysStatusPara.INParaBit.RelaySta.Field.bit00)
          {
            record_heater = 1;
          }            
        }      
        
      }
    }

  }    
}



/*
@函数名：void check_error(void)
@功能：目测手动清除故障标志
*/
void check_error(void)
{
  if(CLR_ERR)
  {
    CLR_ERR = 0;
    F_CheckReturnSensorErr(0);
    F_CheckOutSensorErr(0);
    F_CheckFreonSensorErr(0);
    F_CheckFlowProtect(0);
    F_CheckHeaterProtect(0);
    F_CoolFarAwayFreeze(0);
    F_WinterFarAwayFreeze(0);
    return;
  }
  F_CheckReturnSensorErr(1);
  F_CheckOutSensorErr(1);
  F_CheckFreonSensorErr(1);
  F_CheckFlowProtect(1);
  F_CheckHeaterProtect(1);
//  F_CoolFarAwayFreeze(1);
  F_WinterFarAwayFreeze(1);


}























































































