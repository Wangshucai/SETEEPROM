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

#ifdef  LOGIC 
  #define LOGIC
#else
  #define LOGIC  extern                    
#endif
  
//#define __DEBUG
typedef enum
{
  Reset = 0,
  nor,
}ErrType;
typedef union
{
  struct 
  {
    uint8_t bit00:1;
    uint8_t bit01:1;
    uint8_t bit02:1;
    uint8_t bit03:1;    
    uint8_t bit04:1;
    uint8_t bit05:1;
    uint8_t bit06:1;
    uint8_t bit07:1;
  }Field ;
  uint8_t ucBuf;	
}SysStaus8BitType;

typedef union
{
  struct 
  {
    uint16_t bit00:1;
    uint16_t bit01:1;
    uint16_t bit02:1;
    uint16_t bit03:1;    
    uint16_t bit04:1;
    uint16_t bit05:1;
    uint16_t bit06:1;
    uint16_t bit07:1;
    uint16_t bit08:1;
    uint16_t bit09:1;
    uint16_t bit10:1;
    uint16_t bit11:1;    
    uint16_t bit12:1;
    uint16_t bit13:1;
    uint16_t bit14:1;
    uint16_t bit15:1;
  }Field ;
  uint16_t uiBuf;	
}SysStaus16BitType;

LOGIC hfim5_inputs input __attribute__((aligned (4)));

typedef union
{
  struct 
  {
    uint16_t Part0:8;
    
    uint16_t Part1:1;
    uint16_t Part2:1;
    uint16_t Part3:1;    
    uint16_t Part4:1;     
    uint16_t Part5:1;     
    uint16_t Part6:2;
    uint16_t Part7:1;
    
  }Field ;
  uint16_t uiBuf;	
}HandSysStaus16BitType;

typedef struct
{
  uint16_t EEVNum;
  uint16_t ByPassEEVNum;
  HandSysStaus16BitType HandRelay;
  uint16_t ComHZ;
  uint8_t RunTime;
  uint8_t TestMode;
}DebugParaType;

LOGIC DebugParaType gTmpDebugPara __attribute__((aligned (2)));
//LOGIC DebugParaType gSysDebugPara __attribute__((aligned (2)));

#define MODE_TEST               (gTmpDebugPara.HandRelay.Field.Part0)
#define HAND_PUMP               (gTmpDebugPara.HandRelay.Field.Part1)
#define HAND_HEARTER            (gTmpDebugPara.HandRelay.Field.Part2)
#define HAND_BYPASS_VALVE       (gTmpDebugPara.HandRelay.Field.Part3)
#define HAND_SAVE_VALVE         (gTmpDebugPara.HandRelay.Field.Part4)
#define HAND_HEATER_COM         (gTmpDebugPara.HandRelay.Field.Part5)

#define HAND_OUT_FAN            (gTmpDebugPara.HandRelay.Field.Part6)
#define HAND_FOUR_WAY_VALVE     (gTmpDebugPara.HandRelay.Field.Part7)

typedef struct
{
  
  int16_t SetCoolTemp;   
  int16_t SetHeatTemp;
  
  int8_t SetMode;
  SysStaus8BitType UserSys8Bit;
  uint8_t MachineType;
  uint8_t DH_Addr;
  uint8_t crc[2];
}UserParaType;

LOGIC UserParaType gTmpUserPara __attribute__((aligned (2)));//
LOGIC UserParaType gSysUserPara __attribute__((aligned (2)));//

#define POWER_SWITCH    gTmpUserPara.UserSys8Bit.Field.bit00  //开关机
#define EN_POWER        gTmpUserPara.UserSys8Bit.Field.bit01  //来电自启状态
#define EN_LOCAL        gTmpUserPara.UserSys8Bit.Field.bit02  //本地操作
typedef struct
{
  int16_t DefrostSetTemp;//化霜温度
  int16_t DefrostComHZ;  //化霜频率
  int16_t DefrostEEVStep1;//EEV
  int16_t DefrostEEVStep2;
  int16_t DefrostEEVStep3;
  int16_t DefrostMinRunTime;  //最小时间
  int16_t DefrostMaxRunTime;  //最大时间
  int16_t ProduceLineComHZ;   //产线测试频率
  
  int16_t ProduceLineEEVStep; //产线测试EEV
  int16_t EEVAdjustTime;      
  int16_t ComMinStopTime;  
  int16_t ComMinRunTime;
  int16_t ComObjHZCalTime;  
  int16_t ReturnFreonHZ;
  int16_t ReturnFreonRunTime;
  int16_t IntallTestTime;   //安装测试时间
  int16_t HandTestTime;    //手动调机时间
  int16_t ProduceTestTime;  //产线测试时间
  int16_t ProduceByPassEEVStep;
  int16_t FanType;
  int8_t Deforst_modulus;
  int8_t EnEEP2;
  
  int8_t AdjustMHz;
  uint8_t BigLoadDiff;
  uint8_t SmallLoadDiff;
  uint8_t LoadCalWay;
}OutParaType;

LOGIC OutParaType gTmpOutPara;
//LOGIC OutParaType gSysOutPara;


typedef struct
{
  int16_t HeaterResonHZ1;//制热共振频率
  int16_t HeaterResonHZ2;
  int16_t HeaterResonHZ3;
  int16_t HeaterResonHZ4;
  int16_t HeaterResonHZ5;
  
  int16_t CoolResonHZ1;//制冷共振频率
  int16_t CoolResonHZ2;
  int16_t CoolResonHZ3;
  int16_t CoolResonHZ4;
  int16_t CoolResonHZ5;
  
  int16_t SetComI1;  //电流设置
  int16_t SetComI2;
  int16_t SetComI3;
  
  int16_t SetIPM1;  //IPM温度设置
  int16_t SetIPM2;
  int16_t SetIPM3;
  
  int16_t SetPFCI1;//PFC温度设置
  int16_t SetPFCI2;
  int16_t SetPFCI3;
}OutPara2Type;

LOGIC OutPara2Type gTmpOut2Para;
//LOGIC OutPara2Type gSysOut2Para;//未用

typedef struct
{
  SysStaus16BitType RelaySta;
  SysStaus16BitType ErrInfo[3];
}OutParaBitType;

typedef union
{
  struct 
  {
    uint16_t Part0:2;
    uint16_t Part1:2;
    uint16_t Part2:2;
    uint16_t Part3:2;   
    
    uint16_t Part4:2;    
    uint16_t Part5:2; 
    uint16_t Part6:2;    
    uint16_t Part7:2;
  }Field ;
  uint16_t uiBuf;	
}OutDIType;

typedef struct
{
  int16_t ByPassOutTemp;
  int16_t ByPassInTemp;
  int16_t ByPassEEVStep;
  int16_t DriverType;
  
  int16_t OutTemp;
  int16_t ExhaultTemp;
  int16_t InhaleTemp;
  int16_t DefrostTemp;
  int16_t CondInTemp;
  int16_t MachType;
  int16_t ActHZ;
  int16_t OBJHZ;
  int16_t CalCapa;
  int16_t ExhSuperHeat;
  int16_t InhalSuperHeat;
  int16_t EEVStep;
  int16_t SysSta;
  int16_t ComI;
  int16_t ACI;
  int16_t IPMTemp;
  int16_t PFCTemp;
  int16_t ACVol;
  int16_t ExhProtectLevel;
  int16_t ComProtectLevel;
  int16_t IPMProtectLevel;
  int16_t PFCProtectLevel;
  int16_t ACIProtectLevel;
  int16_t Tn;
  OutDIType DISta[2];
  int16_t LowPressNum;
  int16_t LowFarFreezeLV;
}OutParaBufType;



typedef struct
{
  SysStaus16BitType RelaySta;
  SysStaus16BitType ErrInfo;
}INParaBitType;

typedef union
{
  struct 
  {
    uint16_t Part0:2;
    uint16_t Part1:1;
    uint16_t Part2:1;
    uint16_t      :12;   
  }Field ;
  uint16_t uiBuf;	
}INDIType;

typedef struct
{
  int16_t ReturnTemp;//回水温度
  int16_t InTemp;   //进水温度
  int16_t FreonInTemp;//冷媒进温度
  int16_t FreonOutTemp;//冷媒出温度
  INDIType DipNum;   //拨码数值
  int16_t PowerSta;  //开关机状态
  int16_t ModeSta;   //模式设定
  int16_t CoolSetTemp;//制冷设定温度
  int16_t HeatSetTemp;//制热设定温度
}INParaBufType;

typedef union
{
  uint8_t ucBuf;
  struct
  {
    uint8_t func  :2;
    uint8_t num   :1;
    uint8_t freq  :1;
    uint8_t       :4;
  }Field;
}MachType;

typedef struct
{   
  uint8_t HardwareVer_MainBoard[10];
  uint8_t SoftwareVer_MainBoard[10];
  uint8_t HardwareVer_OutBoard[10];
  uint8_t SoftwareVer_OutBoard[10];
  MachType MachSta;
  uint8_t EEDly;
  uint8_t EECheckNO;
  int16_t AD_Temp[4];
  SysStaus16BitType LwcCommBit;
  SysStaus16BitType OutCommBit;
  SysStaus16BitType InnerBit;
  
  int16_t ComRunTime;
  int16_t ComStopTime;
  
  int16_t PumpRunTime;
  int16_t PumpStopTime;
  int16_t heaterONOatTime; 
  int16_t NeedFullTime;     //符合需求
  int16_t HeaterStopTime;
  
  uint8_t DIP_Val;
  SysStaus8BitType OutBoardWrPara;
  
  OutParaBitType OutParaBit;
  OutParaBufType OutParaBuf;
  INParaBitType INParaBit;
  INParaBufType INParaBuf;
  uint8_t DlyLed;
  uint8_t CommCheck;
}SysStatusParaType;

LOGIC SysStatusParaType gSysStatusPara;

#define FREON_IN_TEMP       gSysStatusPara.AD_Temp[0]  //冷媒进温度
#define FREON_OUT_TEMP      gSysStatusPara.AD_Temp[1]  //冷媒出温度
#define IN_TEMP             gSysStatusPara.AD_Temp[2]  //回水温度
#define OUT_TEMP            gSysStatusPara.AD_Temp[3]  //出水温度
  

#define OUT_VER_STA         gSysStatusPara.OutBoardWrPara.Field.bit00
#define OUT_SET_STA         gSysStatusPara.OutBoardWrPara.Field.bit01
#define OUT2_SET_STA        gSysStatusPara.OutBoardWrPara.Field.bit02


#define COMM_DIR            gSysStatusPara.LwcCommBit.Field.bit00
#define COMM_DEBUG_STA      gSysStatusPara.LwcCommBit.Field.bit01
#define COMM_IN_BUF_STA     gSysStatusPara.LwcCommBit.Field.bit02
#define COMM_OUT_BUT_STA    gSysStatusPara.LwcCommBit.Field.bit03
#define COMM_OUT2_BUT_STA   gSysStatusPara.LwcCommBit.Field.bit04
#define COMM_IN_VER         gSysStatusPara.LwcCommBit.Field.bit05
#define COMM_OUR_VER        gSysStatusPara.LwcCommBit.Field.bit06

#define DEBUG_BUF_STA       gSysStatusPara.OutCommBit.Field.bit00
#define USER_BUF_STA        gSysStatusPara.OutCommBit.Field.bit01
#define OUT_BUF_STA         gSysStatusPara.OutCommBit.Field.bit02
#define OUT2_BUF_STA        gSysStatusPara.OutCommBit.Field.bit03
#define OUT_CLR_ERR         gSysStatusPara.OutCommBit.Field.bit05
#define OUT_RESET_PARA      gSysStatusPara.OutCommBit.Field.bit06

#define INIT_STA            gSysStatusPara.InnerBit.Field.bit00
#define SEND_READY          gSysStatusPara.InnerBit.Field.bit01
#define FAR_AWAY_FREEZE     gSysStatusPara.InnerBit.Field.bit02
#define HAVE_OUT_VER        gSysStatusPara.InnerBit.Field.bit03
#define HAVE_OUT_BUF        gSysStatusPara.InnerBit.Field.bit04
#define LWC_INIT_STA        gSysStatusPara.InnerBit.Field.bit05
#define CLR_ERR             gSysStatusPara.InnerBit.Field.bit06
#define RESET_PARA          gSysStatusPara.InnerBit.Field.bit07

#define RST_DEBUG_STA       gSysStatusPara.InnerBit.Field.bit08
#define FLOW_CHECK_END      gSysStatusPara.InnerBit.Field.bit09 //水流检测
#define HAVE_OUT2_BUF       gSysStatusPara.InnerBit.Field.bit10
#define DH_COMM_STA         gSysStatusPara.InnerBit.Field.bit11 /*1 有通讯 0 无通讯*/

//#define ObjHeater          gSysStatusPara.InnerBit.Field.bit12
//#define ActHeater          gSysStatusPara.InnerBit.Field.bit13

#define COM_HEATER_STA        gSysStatusPara.OutParaBit.RelaySta.Field.bit00
#define COOL_FAN_HIGH_STA     gSysStatusPara.OutParaBit.RelaySta.Field.bit01
#define COOL_FAN_MID_STA      gSysStatusPara.OutParaBit.RelaySta.Field.bit02
#define COOL_FAN_LOW_STA      gSysStatusPara.OutParaBit.RelaySta.Field.bit03
#define FIX_HZ_COM_STA        gSysStatusPara.OutParaBit.RelaySta.Field.bit04
#define SAVE_ENERGY_VALVE_STA gSysStatusPara.OutParaBit.RelaySta.Field.bit05
#define HEATER_CHASS_STA      gSysStatusPara.OutParaBit.RelaySta.Field.bit06
#define PUMP_STA              gSysStatusPara.OutParaBit.RelaySta.Field.bit07

#define FOUR_WAY_VALVE_STA    gSysStatusPara.OutParaBit.RelaySta.Field.bit08
#define LIP_PUMP_VALVE_STA    gSysStatusPara.OutParaBit.RelaySta.Field.bit09
#define ERR_STA               gSysStatusPara.OutParaBit.RelaySta.Field.bit10
#define DO12_STA              gSysStatusPara.OutParaBit.RelaySta.Field.bit11


#define OUT_COMM_ERR          gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02


#define RETURN_SENSOR_ERR           gSysStatusPara.INParaBit.ErrInfo.Field.bit00//回水传感器故障
#define OUT_SENSOR_ERR              gSysStatusPara.INParaBit.ErrInfo.Field.bit01//出水传感器故障
#define FREON_SENSOR_ERR            gSysStatusPara.INParaBit.ErrInfo.Field.bit02//冷媒传感器故障

#define FLOW_PROTECT                gSysStatusPara.INParaBit.ErrInfo.Field.bit04
#define COOL_FAR_AWAY_FREEZE_ERR    gSysStatusPara.INParaBit.ErrInfo.Field.bit05
#define HEATER_PROTECT              gSysStatusPara.INParaBit.ErrInfo.Field.bit06//电加热故障
#define WINTER_FAR_AWAY_FREEZE      gSysStatusPara.INParaBit.ErrInfo.Field.bit07 
#define EEPROM_ERR                  gSysStatusPara.INParaBit.ErrInfo.Field.bit08






void Init_Thread(void);

void F_PowerInit(void);

void F_CheckOutErr(void);


uint8_t F_CheckMode(void);








