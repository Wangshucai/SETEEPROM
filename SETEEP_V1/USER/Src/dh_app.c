/**********************************************
;**********************************************
;***      动环
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define DH_APP
#include "outComm.h"
#include "hw_uart.h"
#include "lwcComm.h"
#include "mbapp.h"
#include "logic.h"
#include "dh_app.h"

static uint8_t DHCommTime = 0;
static uint8_t MBS_RxTask(void);
static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx);
void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr);
uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr);

//uart4_rx_buf[]长度256
#define  MB4_RX_FRAME_ADDR      (uart4_rx_buf[0]) //地址
#define  MB4_RX_FRAME_FC        (uart4_rx_buf[1])  //功能码
#define  MB4_RX_FRAME_DATA      (uart4_rx_buf[2])  //数据


#define  MB4_TX_FRAME_ADDR      (uart4_tx_buf[0])  //发送地址
#define  MB4_TX_FRAME_FC        (uart4_tx_buf[1])  //功能码
#define  MB4_TX_FRAME_DATA      (uart4_tx_buf[2])  //数据

#define  MB4_RX_DATA_START     (((uint16_t)uart4_rx_buf[2] << 8) + (uint16_t)uart4_rx_buf[3])//寄存器地址
#define  MB4_RX_DATA_REGS      (((uint16_t)uart4_rx_buf[4] << 8) + (uint16_t)uart4_rx_buf[5])//写入数据


/****************************************************************************
 * UART4接口回调函数
 ****************************************************************************/
void uart4_rx_adata(uint16_t data, uint8_t abyte) {
  MB_Params[HFIM5_UART_4].RTU_TimeoutCtr = MB_Params[HFIM5_UART_4].RTU_TimeoutCnts;
  MB_Params[HFIM5_UART_4].RTU_TimeoutEn = TRUE;
//  gSysStatusPara.CommCheck  = 0;
}

void uart4_rx_frame(uint16_t count) {
  
  MB_Params[HFIM5_UART_4].RxBufByteCtr = count;
}

/*中断调用函数*/
void uart4_tx_clpt(void) {
	hfim5_uart_set_tx_en(HFIM5_UART_4, FALSE);  /*接受函数*/
	hfim5_uart_start_receive(HFIM5_UART_4);
  MB_Params[HFIM5_UART_4].TxEnd = TRUE;
}


/*
返回错误校验码
*/
static void MB_ErrResponse(MB_ParamsType *tx, uint8_t errCode) {
  MB4_TX_FRAME_ADDR   = MB4_RX_FRAME_ADDR;
  MB4_TX_FRAME_FC     = MB4_RX_FRAME_FC | 0x80;
  MB4_TX_FRAME_DATA   = errCode;
  tx->TxBufByteCtr = 3;                 //发送数据计数
}


/*
@函数名：static uint8_t MB_Func03_Handle(MB_ParamsType *rx)
@输入：03 功能帧
@功能：读保持寄存器
*/

static uint8_t MB_Func03_Handle(MB_ParamsType *rx) {
	
  uint16_t reg;    //寄存器地址
  uint16_t nbr_regs; //需要读的寄存器个数
  uint16_t val;    //寄存器值
  uint16_t err;    //故障标志
  uint8_t nbr_bytes;
	
  uint8_t *presp;

	
  if (rx->RxBufByteCtr != 8)  //地址 1 + 功能码 1 + 寄存器地址 2 + 数据 2 + 校验 2
    return FALSE;
  
  reg = MB4_RX_DATA_START;    //寄存器首地址
  nbr_regs = MB4_RX_DATA_REGS; //需要读的寄存器个数
  
  if ((nbr_regs == 0) || (nbr_regs > 125)) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);//返回个数超范围错误
    return TRUE;
  }
  
  nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t));//需要发送的字节数
  
  presp = &uart4_tx_buf[0];                        //取数据首地址
  
  *presp++ =  MB4_RX_FRAME_ADDR;                   //uart4_tx_buf[0] = MB4_RX_FRAME_ADDR; 
  *presp++ =  MB4_RX_FRAME_FC;                     //uart4_tx_buf[1] = MB4_RX_FRAME_FC; 
  *presp++ = (uint8_t)nbr_bytes;                   //uart4_tx_buf[2] = (uint8_t)nbr_bytes; 

  while(nbr_regs) {
	  
    val = DH_RdHoding(reg,&err);                   //将reg地址的值赋值给val 同时返回读取错误标志位
	  
    if (err == MB_ERR_NONE) {                      //读取成功，分别将val的高低八位赋值到数组
      *presp++ = (uint8_t)((val >> 8) & 0x00FF);   //第一次 uart4_tx_buf[3] = 高八位
      *presp++ = (uint8_t)(val & 0x00FF);          //第一次 uart4_tx_buf[4] = 低八位
    }
    else {
      MB_ErrResponse(rx, err);                     //如果错误，返回错误码
      return TRUE;
    }
    reg++;                                         //读取一个寄存器后读下一个寄存器
    nbr_regs--;                                    //同时需要读取的数据要减一
  }
  rx->TxBufByteCtr = nbr_bytes + 3;                //到最后总共的字节数为  地址1 + 功能码1 + 字节数1
  return(TRUE);                                    //最后统一加CRC校验  并rx->TxBufByteCtr + 2
}
/*
@函数名：static uint8_t MB_Func06_Handle(MB_ParamsType *rx)
@输入：接收的06功能码数据帧
@功能：向单个寄存器写入值
*/

static uint8_t MB_Func06_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t val;
  uint16_t err;
	
  if (rx->RxBufByteCtr != 8)//地址1  功能码1 寄存器地址2  数据2  校验2  
    return FALSE;
  
  reg =  MB4_RX_DATA_START;  //这里的地址
  val =  MB4_RX_DATA_REGS;   //值   两个字节进行合并
  
  DH_WrHoding(reg,val,&err); //写入对应寄存器
  
  
  if (err != MB_ERR_NONE) {
    MB_ErrResponse(rx, err); //检验是否有误
    return TRUE;
  }
  
  MB4_TX_FRAME_ADDR = MB4_RX_FRAME_ADDR;  /* 写入正确后返回成功响应码 */
  MB4_TX_FRAME_FC   = MB4_RX_FRAME_FC;    
  uart4_tx_buf[2] = uart4_rx_buf[2];       
  uart4_tx_buf[3] = uart4_rx_buf[3];
  uart4_tx_buf[4] = uart4_rx_buf[4];
  uart4_tx_buf[5] = uart4_rx_buf[5];
  rx->TxBufByteCtr = 6;
  return TRUE;
}


/*
@函数名：static uint8_t MB_Func16_Handle(MB_ParamsType *rx)
@输入：串口接收请求数据帧  写多个寄存器
@功能：响应对应的请求
*/

static uint8_t MB_Func16_Handle(MB_ParamsType *rx) {
  uint16_t reg;           //寄存器地址
  uint16_t nbr_regs;      //需要写的寄存器个数
  uint16_t nbr_bytes;     //需要发送的字节数
  uint16_t val;           //寄存器值
  uint16_t err;           //错误状态
  uint8_t *prx;           //按地址先后保存要接收寄存器的值

  reg = MB4_RX_DATA_START;  //获取接收数据中的寄存器首地址
  nbr_regs = MB4_RX_DATA_REGS;//获取需要发送的寄存器个数
  
  if (nbr_regs == 0 || nbr_regs > 125) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY); //判断个数是否符合     
    return (TRUE);
  }
  prx  = &uart4_rx_buf[6];    //提取写入字节数的地址
  nbr_bytes = (uint16_t)*prx++; //保存后进入数据区
  
  if ((rx->RxBufByteCtr - 9) != nbr_bytes) {  //地址1 + 功能1 + 寄存器地址2 + 寄存器数量 2 + 字节数1 + CRC 2 = 9
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);//寄存器值数量错误响应
    return (TRUE);
  }
  if (((nbr_bytes / nbr_regs) != (uint16_t)sizeof(uint16_t))) { //字节数/寄存器数量的值不是 2
    MB_ErrResponse(rx,MB_ERR_ILLEGAL_DATA_VAL);                 //返回数据值错误响应
    return (TRUE);
  }
	

  while(nbr_regs) {                                         //要写入的寄存器数量不为0
    val = ((uint16_t)*prx++) << 8;                          //取数据区的高低8位赋给val
    val += (uint16_t)*prx++;
    DH_WrHoding(reg, val, &err);                            //将取出的值写入寄存器 
    if (err == MB_ERR_NONE) {                               //检测错误
      reg++;                                                //写入成功换到下一个寄存器
      nbr_regs--;                                           //需要写的寄存器减少一个
    }
    else {
      MB_ErrResponse(rx, err);                              //写入错误则返回错误代码
      return (TRUE);
    }
  }
  
  rx->TxBufByteCtr = 6;                                     //全部正确则返回响应数据帧
  MB4_TX_FRAME_ADDR = MB4_RX_FRAME_ADDR;
  MB4_TX_FRAME_FC = MB4_RX_FRAME_FC;
  uart4_tx_buf[2] = uart4_rx_buf[2];
  uart4_tx_buf[3] = uart4_rx_buf[3];
  uart4_tx_buf[4] = uart4_rx_buf[4];
  uart4_tx_buf[5] = uart4_rx_buf[5];
  return (TRUE);
}


/*
@函数名：static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx)
@输入：
@功能：功能码解析
*/
static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx) {
  uint8_t reply = FALSE;

  if ((MB4_RX_FRAME_ADDR == addr)  || (MB4_RX_FRAME_ADDR == 0)){
    switch(MB4_RX_FRAME_FC) {

    case MB_FC_03:
      reply = MB_Func03_Handle(rx);
      break;
    case MB_FC_06:
      reply = MB_Func06_Handle(rx);
      break;
    case MB_FC_16:
      reply = MB_Func16_Handle(rx);
      break;
    default:
      MB_ErrResponse(rx, MB_ERR_ILLEGAL_FC);
      reply = TRUE;
    }
  }

  if (MB4_RX_FRAME_ADDR == 0) {
    return FALSE;
  }
  else {
    return(reply);
  }
}

static uint8_t MBS_RxTask(void) {
  uint8_t ok;
  uint8_t reply = FALSE;
  if (MB_Params[HFIM5_UART_4].RxBufByteCtr >= MB_RTU_MIN_MSG_SIZE) {
  
		ok = MB_ChkFrame(HFIM5_UART_4 , &uart4_rx_buf[0]);
	
		if (ok == TRUE) {
      DHCommTime = 20;
			reply = MB_FCxx_Handle(MB_Params[HFIM5_UART_4].NodeAddr,&MB_Params[HFIM5_UART_4]);

			if (reply == TRUE) {
				MB_RTU_TX(uart4_tx_buf,&MB_Params[HFIM5_UART_4]);
			}
		}
	}
	return reply;
}

void Uart4_Tmr(void)
{
  if (MB_Params[HFIM5_UART_4].RTU_TimeoutEn == TRUE) {
      MB_Params[HFIM5_UART_4].RTU_TimeoutCtr--;
    if (MB_Params[HFIM5_UART_4].RTU_TimeoutCtr == 0) {
      MB_Params[HFIM5_UART_4].RTU_TimeoutEn = FALSE;
      MB_Params[HFIM5_UART_4].RecAFrameFlag = TRUE;
    }
  }
  if (MB_Params[HFIM5_UART_4].SleepAfterRxCtr) {
			MB_Params[HFIM5_UART_4].SleepAfterRxCtr--;
		}
}

void DH_Slave_Task(MB_ParamsType *up) {
	if (up->QueryStep == S_WAIT_REC) {
		if (up->RecAFrameFlag) {
      if(MBS_RxTask())
      {
        up->TxEnd = FALSE;
        up->TxBufPtr =  &uart4_tx_buf[0];
        up->SleepAfterRxCtr = up->SleepAfterRxCnt;
        up->QueryStep = S_DLY_RESP;
      }
      else
      {
        up->RxBufPtr = &uart4_rx_buf[0];
        up->RxBufByteCtr = 0;
      }

			up->RecAFrameFlag = FALSE;
		}
	}
	else if (up->QueryStep == S_DLY_RESP) {
		if (up->SleepAfterRxCtr == 0) {
			up->TxBufPtr = &uart4_tx_buf[0];
      hfim5_uart_set_tx_en(HFIM5_UART_4,TRUE);
      hfim5_uart_start_transmit(HFIM5_UART_4,up->TxBufByteCtr);
			up->QueryStep = S_WAIT_REC;
			up->RxBufPtr = &uart4_rx_buf[0];
			up->RxBufByteCtr = 0;
		}
	}
}


void MB4_Init(uint8_t addr,MB_ParamsType *param) {  
  param->NodeAddr = 1;                           //通信地址赋值
  param->RxBufByteCtr = 0;                       //串口接收数据字节计数
  param->RTU_TimeoutCtr = 0;                     //
  param->RTU_TimeoutCnts = 2;                    //
  param->FirstRxTimeOutCnt = 5;                  //
  param->RxBufPtr = &uart4_rx_buf[0];             //数据接收缓存首地址
  param->TxCtr = param->TxBufByteCtr = 0;         //发送数据字节计数
  param->TxBufPtr = &uart4_tx_buf[0];             //数据发送缓存首地址 
  param->SleepAfterRxCnt = 2;                     //
}



/*
@函数名：uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr)
@输入：需要读取的寄存器地址，状态标志
@功能：返回输入寄存器地址的值，同时返回读取状态
*/

uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr)
{
  *perr = MB_ERR_NONE;  //首先将状态设置为正常
	
  if(reg >= 30001 && reg <= 30005)  //判断寄存器区间
  {
    switch(reg)                     //判断地址
    {
      case 30001: return gTmpUserPara.SetMode;  //读模式
      case 30002: return POWER_SWITCH;     //读开关机
      case 30003: return gTmpUserPara.SetCoolTemp;//制冷设定
      case 30004: return gTmpUserPara.SetHeatTemp;//制热设定
      case 30005: return EN_LOCAL;   //本地操作
    }  //少default

  }
  else if(reg >= 50001 && reg <= 50013)
  {
    switch(reg)
    {
      case 50001: return POWER_SWITCH;   //远程开关机
      case 50002: return gTmpUserPara.SetMode;  //运行模式
      case 50003: return gSysStatusPara.OutParaBuf.ActHZ; //压缩机运行频率
      case 50004: return gSysStatusPara.OutParaBuf.EEVStep; //电子膨胀阀开度
      case 50005:                                           //告警保护
        return ((gSysStatusPara.INParaBit.ErrInfo.uiBuf & 0x37F)  \
        | gSysStatusPara.OutParaBit.ErrInfo[0].uiBuf              \
        | gSysStatusPara.OutParaBit.ErrInfo[1].uiBuf              \
        | gSysStatusPara.OutParaBit.ErrInfo[2].uiBuf) ? 1 : 0;
      case 50006: return gSysStatusPara.INParaBuf.ReturnTemp;  //回水温度
       
      case 50007: return gSysStatusPara.INParaBuf.InTemp;   //出水温度
      case 50008: return gSysStatusPara.OutParaBuf.LowPressNum; //吸气压力
      case 50009: return gSysStatusPara.OutParaBuf.OutTemp;     //环境温度
      case 50010: return gSysStatusPara.OutParaBuf.ExhaultTemp; //排气温度
      case 50011: return gSysStatusPara.OutParaBuf.InhaleTemp;  //吸气温度
      case 50012: return gSysStatusPara.OutParaBuf.DefrostTemp;  //管盘温度
      case 50013: return gSysStatusPara.INParaBuf.FreonInTemp;     //冷媒进温度   
        
    }
  }else if(reg >= 60001 && reg <= 60026)
  {
    switch(reg)
	{
	  case 60001: return gSysStatusPara.OutParaBuf.CondInTemp;     //低饱和温度
	  case 60002: return gSysStatusPara.OutParaBuf.IPMTemp;        // IPM温度                             
	  case 60003: return gSysStatusPara.OutParaBuf.PFCTemp;        // PFC温度
	  case 60004: return gSysStatusPara.OutParaBuf.ByPassOutTemp;  // 辅路出口温度
	  case 60005: return gSysStatusPara.OutParaBuf.ByPassInTemp;   // 辅路进口温度
	  case 60006: return gSysStatusPara.OutParaBuf.ByPassEEVStep;  // 辅助电子膨胀阀
	  case 60007: return gSysStatusPara.OutParaBuf.DriverType;      // 驱动类型
	  case 60008: return gSysStatusPara.OutParaBuf.MachType;        // 型号
	  case 60009: return gSysStatusPara.OutParaBuf.OBJHZ;           //  目标频率               
	  case 60010: return gSysStatusPara.OutParaBuf.CalCapa;         //  计算能力
	  case 60011: return gSysStatusPara.OutParaBuf.ExhSuperHeat;     //  排气过热度
	  case 60012: return gSysStatusPara.OutParaBuf.InhalSuperHeat;   //  吸气过热度
	  case 60013: return gSysStatusPara.OutParaBuf.ComI;             //  压缩机电流
	  case 60014: return gSysStatusPara.OutParaBuf.ACI;              //  AC电流
	  case 60015: return gSysStatusPara.OutParaBuf.ExhProtectLevel;   //  排气保护等级
	  case 60016: return gSysStatusPara.OutParaBuf.ComProtectLevel;   //  压缩机保护等级
	  case 60017: return gSysStatusPara.OutParaBuf.IPMProtectLevel;   //  IPM温度保护等级
      case 60018: return gSysStatusPara.OutParaBuf.PFCProtectLevel;   //  PFC温度保护等级  
	  case 60019: return gSysStatusPara.OutParaBuf.LowFarFreezeLV;    //  低压防冻保护等级
	  case 60020: return gSysStatusPara.OutParaBuf.ACIProtectLevel;    //  AC电流保护等级
	  case 60021: return gSysStatusPara.OutParaBuf.DISta[0].uiBuf;     //  DI状态
	  case 60022: return gSysStatusPara.OutParaBuf.DISta[1].uiBuf;       //  DI状态
	  case 60023: return gSysStatusPara.MachSta.ucBuf;               //  系统类型
	  case 60024: return gTmpUserPara.SetCoolTemp;                   //  制冷设定温度
	  case 60025: return gTmpUserPara.SetHeatTemp;                   //  制热设定温度
	  case 60026: if(( uint16_t)gSysStatusPara.INParaBit.ErrInfo.Field.bit07)
	  {
	    return  (uint16_t)1;		     //  有冬季防冻等级
	  }else
	  {
	    return ( uint16_t)0;	         //  无冬季防冻等级
	  }
	}
  
  
  }
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;          //地址不在范围返回地址错误
  }
  return 0;
}



static uint8_t Delay = 0;


/*
@函数名：void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr)
@输入：寄存器地址，写入值，返回错误状态指针
@功能：向指定寄存器写入指定值
*/

void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr)
{
 if(reg >= 30001 && reg <= 30005)
  {
    Delay = 2;
    switch(reg)
    {
      case 30001:  gTmpUserPara.SetMode = reg_val; break;       //模式
      case 30002:  POWER_SWITCH = reg_val;          break;      //开关机
      case 30003:  gTmpUserPara.SetCoolTemp = reg_val;  break;  //制冷设定温度
      case 30004:  gTmpUserPara.SetHeatTemp = reg_val;  break;  //制热设定温度
      case 30005:  EN_LOCAL = reg_val;                break;    //本地操作
    }
    *perr = MB_ERR_NONE;
    return;
  }
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;//返回地址错误标志位
  }

  

}



void F_CheckDHCommSta(void)
{
  DH_COMM_STA = 1;
  if(Delay)
  {
    Delay --;
    if(0 == Delay)
    {
      LWC_INIT_STA = 1;
      gSysStatusPara.EEDly  = 2;
    }
  }
  if(DHCommTime)
  {
    DHCommTime --;
  }
  else
  {
    DH_COMM_STA = 0;
  }
}









