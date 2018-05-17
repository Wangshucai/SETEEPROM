/**********************************************
;**********************************************
;***      定时器 
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define OUT_COMM
#include "outComm.h"
#include "hw_uart.h"
#include "lwcComm.h"
#include "mbapp.h"
#include "logic.h"

static uint8_t OutCommTime = 0;
static uint8_t MBS_RxTask(void);
static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx);
#define  MB2_RX_FRAME_ADDR      (uart2_rx_buf[0])
#define  MB2_RX_FRAME_FC        (uart2_rx_buf[1])
#define  MB2_RX_FRAME_DATA      (uart2_rx_buf[2])


#define  MB2_TX_FRAME_ADDR      (uart2_tx_buf[0])
#define  MB2_TX_FRAME_FC        (uart2_tx_buf[1])
#define  MB2_TX_FRAME_DATA      (uart2_tx_buf[2])

#define  MB2_RX_DATA_START     (((uint16_t)uart2_rx_buf[2] << 8) + (uint16_t)uart2_rx_buf[3])
#define  MB2_RX_DATA_REGS      (((uint16_t)uart2_rx_buf[4] << 8) + (uint16_t)uart2_rx_buf[5])


/****************************************************************************
 * UART2接口回调函数
 ****************************************************************************/
void uart2_rx_adata(uint16_t data, uint8_t abyte) {
  MB_Params[HFIM5_UART_2].RTU_TimeoutCtr = MB_Params[HFIM5_UART_2].RTU_TimeoutCnts;
  MB_Params[HFIM5_UART_2].RTU_TimeoutEn = TRUE;
  gSysStatusPara.CommCheck  = 0;
}

void uart2_rx_frame(uint16_t count) {
  
  MB_Params[HFIM5_UART_2].RxBufByteCtr = count;
}

void uart2_tx_clpt(void) {
	hfim5_uart_set_tx_en(HFIM5_UART_2, FALSE);  /*接受函数*/
	hfim5_uart_start_receive(HFIM5_UART_2);
  MB_Params[HFIM5_UART_2].TxEnd = TRUE;
}

static void MB_ErrResponse(MB_ParamsType *tx, uint8_t errCode) {
  MB2_TX_FRAME_ADDR   = MB2_RX_FRAME_ADDR;
  MB2_TX_FRAME_FC     = MB2_RX_FRAME_FC | 0x80;
  MB2_TX_FRAME_DATA   = errCode;
  tx->TxBufByteCtr = 3; 
}

static uint8_t MB_Func01_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t nbr_regs;
  uint16_t val;
  uint16_t err;
  uint8_t nbr_bytes;
  uint8_t *presp;
  uint8_t i = 0 , data = 0;;
	
  if (rx->RxBufByteCtr != 8)
    return FALSE;
  
  reg = MB2_RX_DATA_START;
  nbr_regs = MB2_RX_DATA_REGS;
  
  if ((nbr_regs == 0) || (nbr_regs > 125)) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);
    return TRUE;
  }
  
  nbr_bytes = (nbr_regs % 8) ? (nbr_regs / 8) + 1 : (nbr_regs / 8);
  presp = &uart2_tx_buf[0];
  *presp++ =  MB2_RX_FRAME_ADDR;
  *presp++ =  MB2_RX_FRAME_FC;
  *presp++ = (uint8_t)nbr_bytes;

  while(nbr_regs) {
    data = 0;
    for(i = 0; i < 8; i ++)
    {
      val = MB_RdHoding(OUT_ADDR,Coil,reg,&err);
      if (err == MB_ERR_NONE)
      {
        if(val) data |= (0x01 << i);        
      }
      else {
        MB_ErrResponse(rx, err);
        return TRUE;
      }
      reg++;
      nbr_regs--;
      if(0 == nbr_regs) break;
    }
		*presp++ = data;
  }
  rx->TxBufByteCtr = nbr_bytes + 3;
  return(TRUE);
}

static uint8_t MB_Func03_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t nbr_regs;
  uint16_t val;
  uint16_t err;
  uint8_t nbr_bytes;
  uint8_t *presp;

	
  if (rx->RxBufByteCtr != 8)
    return FALSE;
  
  reg = MB2_RX_DATA_START;
  nbr_regs = MB2_RX_DATA_REGS;
  
  if ((nbr_regs == 0) || (nbr_regs > 125)) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);
    return TRUE;
  }
  
  nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t));
  presp = &uart2_tx_buf[0];
  *presp++ =  MB2_RX_FRAME_ADDR;
  *presp++ =  MB2_RX_FRAME_FC;
  *presp++ = (uint8_t)nbr_bytes;

  while(nbr_regs) {
		val = MB_RdHoding(OUT_ADDR,Holding,reg,&err);
    if (err == MB_ERR_NONE) {
      *presp++ = (uint8_t)((val >> 8) & 0x00FF);
      *presp++ = (uint8_t)(val & 0x00FF);
    }
    else {
      MB_ErrResponse(rx, err);
      return TRUE;
    }
    reg++;
    nbr_regs--;
  }
  rx->TxBufByteCtr = nbr_bytes + 3;
  return(TRUE);
}

static uint8_t MB_Func06_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t val;
  uint16_t err;
	
  if (rx->RxBufByteCtr != 8)
    return FALSE;
  
  reg =  MB2_RX_DATA_START;
  val = MB2_RX_DATA_REGS;
  

	MB_WrHoding(OUT_ADDR,Holding,reg,val,&err);
  
  if (err != MB_ERR_NONE) {
    MB_ErrResponse(rx, err);
    return TRUE;
  }
  
  MB2_TX_FRAME_ADDR = MB2_RX_FRAME_ADDR;
  MB2_TX_FRAME_FC   = MB2_RX_FRAME_FC;
  uart2_tx_buf[2] = uart2_rx_buf[2];
  uart2_tx_buf[3] = uart2_rx_buf[3];
  uart2_tx_buf[4] = uart2_rx_buf[4];
  uart2_tx_buf[5] = uart2_rx_buf[5];
  rx->TxBufByteCtr = 6;
  return TRUE;
}

static uint8_t MB_Func15_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t nbr_regs;
  uint16_t nbr_bytes;
  uint16_t val;
  uint16_t err;
  uint8_t *prx;
  uint8_t i = 0 ,len = 0 ,Data = 0;
  reg = MB2_RX_DATA_START;
  nbr_regs = MB2_RX_DATA_REGS;
  
  if (nbr_regs == 0 || nbr_regs > 125) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);      
    return (TRUE);
  }
  prx  = &uart2_rx_buf[6];
  nbr_bytes = (uint16_t)*prx++;
  if ((rx->RxBufByteCtr - 9) != nbr_bytes) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);
    return (TRUE);
  }
  
  len = nbr_regs / 8;
  if(nbr_regs % 8) len ++;
  
  if (nbr_bytes != len) {
    MB_ErrResponse(rx,MB_ERR_ILLEGAL_DATA_VAL);
    return (TRUE);
  }
	

  while(nbr_regs) {
    val = ((uint16_t)*prx++) ;
    for(i = 0; i < 8 ; i ++)
    {
      Data = (val >> i) & 0x01;
      MB_WrHoding(OUT_ADDR,Coil,reg, Data, &err);
      if (err == MB_ERR_NONE) {
        reg ++;
        nbr_regs--;
        if(0 == nbr_regs) break;
      }
      else {
        MB_ErrResponse(rx, err);
        return (TRUE);
      }
    }
  }
  
  rx->TxBufByteCtr = 6;
  MB2_TX_FRAME_ADDR = MB2_RX_FRAME_ADDR;
  MB2_TX_FRAME_FC = MB2_RX_FRAME_FC;
  uart2_tx_buf[2] = uart2_rx_buf[2];
  uart2_tx_buf[3] = uart2_rx_buf[3];
  uart2_tx_buf[4] = uart2_rx_buf[4];
  uart2_tx_buf[5] = uart2_rx_buf[5];
  return (TRUE);
}

static uint8_t MB_Func16_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t nbr_regs;
  uint16_t nbr_bytes;
  uint16_t val;
  uint16_t err;
  uint8_t *prx;

  reg = MB2_RX_DATA_START;
  nbr_regs = MB2_RX_DATA_REGS;
  
  if (nbr_regs == 0 || nbr_regs > 125) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);      
    return (TRUE);
  }
  prx  = &uart2_rx_buf[6];
  nbr_bytes = (uint16_t)*prx++;
  if ((rx->RxBufByteCtr - 9) != nbr_bytes) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);
    return (TRUE);
  }
  if (((nbr_bytes / nbr_regs) != (uint16_t)sizeof(uint16_t))) {
    MB_ErrResponse(rx,MB_ERR_ILLEGAL_DATA_VAL);
    return (TRUE);
  }
	

  while(nbr_regs) {
    val = ((uint16_t)*prx++) << 8;
    val += (uint16_t)*prx++;
    MB_WrHoding(OUT_ADDR,Holding,reg, val, &err);
    if (err == MB_ERR_NONE) {
      reg++;
      nbr_regs--;
    }
    else {
      MB_ErrResponse(rx, err);
      return (TRUE);
    }
  }
  
  rx->TxBufByteCtr = 6;
  MB2_TX_FRAME_ADDR = MB2_RX_FRAME_ADDR;
  MB2_TX_FRAME_FC = MB2_RX_FRAME_FC;
  uart2_tx_buf[2] = uart2_rx_buf[2];
  uart2_tx_buf[3] = uart2_rx_buf[3];
  uart2_tx_buf[4] = uart2_rx_buf[4];
  uart2_tx_buf[5] = uart2_rx_buf[5];
  return (TRUE);
}

static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx) {
  uint8_t reply = FALSE;

  if ((MB2_RX_FRAME_ADDR == addr)  || (MB2_RX_FRAME_ADDR == 0)){
    switch(MB2_RX_FRAME_FC) {
    case MB_FC_01:
      reply = MB_Func01_Handle(rx);
    break;
    case MB_FC_03:
      reply = MB_Func03_Handle(rx);
      break;
    case MB_FC_06:
      reply = MB_Func06_Handle(rx);
      break;
    case MB_FC_15:
      reply = MB_Func15_Handle(rx);
    break;
    case MB_FC_16:
      reply = MB_Func16_Handle(rx);
      break;
    default:
      MB_ErrResponse(rx, MB_ERR_ILLEGAL_FC);
      reply = TRUE;
    }
  }

  if (MB2_RX_FRAME_ADDR == 0) {
    return FALSE;
  }
  else {
    return(reply);
  }
}

static uint8_t MBS_RxTask(void) {
  uint8_t ok;
  uint8_t reply = FALSE;
if (MB_Params[HFIM5_UART_2].RxBufByteCtr >= MB_RTU_MIN_MSG_SIZE) {
  
		ok = MB_ChkFrame(HFIM5_UART_2 , &uart2_rx_buf[0]);
	
		if (ok == TRUE) {
      OutCommTime = 0;
			reply = MB_FCxx_Handle(OUT_ADDR,&MB_Params[HFIM5_UART_2]);

			if (reply == TRUE) {
				MB_RTU_TX(uart2_tx_buf,&MB_Params[HFIM5_UART_2]);
			}
		}
	}
	return reply;
}

void Uart2_Tmr(void)
{
  if (MB_Params[HFIM5_UART_2].RTU_TimeoutEn == TRUE) {
    MB_Params[HFIM5_UART_2].RTU_TimeoutCtr--;
    if (MB_Params[HFIM5_UART_2].RTU_TimeoutCtr == 0) {
      MB_Params[HFIM5_UART_2].RTU_TimeoutEn = FALSE;
      MB_Params[HFIM5_UART_2].RecAFrameFlag = TRUE;
    }
  }
  if (MB_Params[HFIM5_UART_2].SleepAfterRxCtr) {
			MB_Params[HFIM5_UART_2].SleepAfterRxCtr--;
		}
}

void UART_Slave_Task(MB_ParamsType *up) {
	if (up->QueryStep == S_WAIT_REC) {
		if (up->RecAFrameFlag) {
      if(MBS_RxTask())
      {
        up->TxEnd = FALSE;
        up->TxBufPtr =  &uart2_tx_buf[0];
        up->SleepAfterRxCtr = up->SleepAfterRxCnt;
        up->QueryStep = S_DLY_RESP;
      }
      else
      {
        up->RxBufPtr = &uart2_rx_buf[0];
        up->RxBufByteCtr = 0;
      }

			up->RecAFrameFlag = FALSE;
		}
	}
	else if (up->QueryStep == S_DLY_RESP) {
		if (up->SleepAfterRxCtr == 0) {
			up->TxBufPtr = &uart2_tx_buf[0];
      hfim5_uart_set_tx_en(HFIM5_UART_2,TRUE);
      hfim5_uart_start_transmit(HFIM5_UART_2,up->TxBufByteCtr);
			up->QueryStep = S_WAIT_REC;
			up->RxBufPtr = &uart2_rx_buf[0];
			up->RxBufByteCtr = 0;
		}
	}
}


void MB2_Init(uint8_t addr,MB_ParamsType *param) {  
  param->NodeAddr = addr;  //通讯地址
  param->RxBufByteCtr = 0;  //缓存区接收数据数量
  param->RTU_TimeoutCtr = 0; //
  param->RTU_TimeoutCnts = 2; //
  param->FirstRxTimeOutCnt = 5;
  param->RxBufPtr = &uart2_rx_buf[0];
  param->TxCtr = param->TxBufByteCtr = 0;
  param->TxBufPtr = &uart2_tx_buf[0];
  param->SleepAfterRxCnt = 2;
}

void F_CheckOutErr(void)
{
  
  if(OutCommTime < 20)
  {
    OutCommTime ++;
  }
  else
  {
    gSysStatusPara.OutParaBit.ErrInfo[1].Field.bit02 = 1;
  }
}
