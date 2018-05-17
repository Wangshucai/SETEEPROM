/**********************************************
;**********************************************
;***      定时器 
;***      Author: 李鹏
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define LWC_COMM

#include "lwcComm.h"
#include "hw_uart.h"
#include "logic.h"
#include "hw_gpio.h"
#include "hw_at24cxx.h"
#include "mbapp.h"

#define  MB3_RX_FRAME_ADDR      (uart3_rx_buf[0])
#define  MB3_RX_FRAME_FC        (uart3_rx_buf[1])
#define  MB3_RX_FRAME_DATA      (uart3_rx_buf[2])


#define  MB3_TX_FRAME_ADDR      (uart3_tx_buf[0])
#define  MB3_TX_FRAME_FC        (uart3_tx_buf[1])
#define  MB3_TX_FRAME_DATA      (uart3_tx_buf[2])

#define  MB3_RX_DATA_START     (((uint16_t)uart3_rx_buf[2] << 8) + (uint16_t)uart3_rx_buf[3])
#define  MB3_RX_DATA_REGS      (((uint16_t)uart3_rx_buf[4] << 8) + (uint16_t)uart3_rx_buf[5])


/****************************************************************************
 * UART3接口回调函数
 ****************************************************************************/
void uart3_rx_adata(uint16_t data, uint8_t abyte) {
  MB_Params[HFIM5_UART_3].RTU_TimeoutCtr = 10;
  MB_Params[HFIM5_UART_3].RTU_TimeoutEn = TRUE;

}

void uart3_rx_frame(uint16_t count) {
  MB_Params[HFIM5_UART_3].RxBufByteCtr = count;
}

void uart3_tx_clpt(void) {
	hfim5_uart_set_tx_en(HFIM5_UART_3, FALSE);  /*接受函数*/
	hfim5_uart_start_receive(HFIM5_UART_3);
    MB_Params[HFIM5_UART_3].TxEnd = TRUE;
}
uint16_t MBM_FC03_RxReply(MB_ParamsType *params) {
  uint16_t nbr_regs;
  uint16_t nbr_bytes;

  if (MB3_RX_FRAME_FC & 0x80)
    return MB3_RX_FRAME_DATA;

  nbr_bytes = uart3_rx_buf[2];
  nbr_regs = (uart3_tx_buf[4] << 8) + uart3_tx_buf[5];
  if (nbr_bytes != nbr_regs * 2)
    return MB_ERR_BYTE_COUNT;
  return MBM_DATA_holdingRegRd();
}

uint16_t MBM_FC06_RxReply(MB_ParamsType *params) {
  return MB_ERR_NONE;
}

uint16_t MBM_FC16_RxReply(MB_ParamsType *params) {
  return MB_ERR_NONE;
}
uint16_t MBM_FCxx_Handle(MB_ParamsType *param) {
  if (MB3_TX_FRAME_ADDR != MB3_RX_FRAME_ADDR)
          return MB_ERR_SLAVE_ADDR;
  if (MB3_TX_FRAME_FC != (MB3_RX_FRAME_FC & 0x7F))
          return MB_ERR_FC;

  switch (MB3_RX_FRAME_FC & 0x7f) {
    case MB_FC_03:
      return MBM_FC03_RxReply(param);
    case MB_FC_06:
      return MBM_FC06_RxReply(param);
    case MB_FC_16:
      return MBM_FC16_RxReply(param);
    default:
      return MB_ERR_FC;
  }
}
uint8_t MB_ChkFrame(hfim5_uart_id ch,uint8_t *rx) {
	uint16_t rx_size;
	uint16_t crc;

	rx_size = MB_Params[ch].RxBufByteCtr;

	crc = rx[rx_size-2] + (rx[rx_size-1] << 8);
	if (crc == MB_Crc16(&rx[0], rx_size-2)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

uint16_t MBM_RTU_RxReply(MB_ParamsType *params) {
	if (MB_Params[HFIM5_UART_3].RxBufByteCtr >= MB_RTU_MIN_MSG_SIZE) {
		if (MB_ChkFrame(HFIM5_UART_3 , &uart3_rx_buf[0]) == TRUE) {
			return MBM_FCxx_Handle(params);
		}
		else {
			return MB_ERR_RX;
		}
	}
	else {
		return MB_ERR_RX;
	}
}

void MB_RTU_TX(uint8_t *tx,MB_ParamsType *param) {
  uint16_t crc;
  uint8_t *pmsg;
  
  crc = MB_Crc16(&tx[0], param->TxBufByteCtr);
  pmsg = &tx[param->TxBufByteCtr];
  *pmsg++ = (uint8_t)(crc & 0x00FF);
  *pmsg = (uint8_t)(crc >> 8);
  param->TxBufByteCtr += 2;
}

uint16_t MBM_FC03_Cmd(uint8_t *tx, uint8_t slave_node, uint16_t slave_addr, uint16_t nbr_regs) {
	uint16_t err = MB_ERR_NONE;
	
	if (nbr_regs == 0 || nbr_regs > 125)
		return MB_ERR_ILLEGAL_DATA_QTY;

  MBM_TX_FRAME_ADDR = slave_node;
  MBM_TX_FRAME_FC = 3;
  MBM_TX_DATA_START_H = slave_addr >> 8;
  MBM_TX_DATA_START_L = (uint8_t)slave_addr;
  MBM_TX_DATA_REG_H = nbr_regs >> 8;
  MBM_TX_DATA_REG_L = (uint8_t)nbr_regs;
  MBM_TX_FRAME_NBYTES = 6;
  MB_RTU_TX(uart3_tx_buf,&MB_Params[HFIM5_UART_3]);
	return err;
}

uint16_t MBM_FC15_Cmd(uint8_t *tx, uint8_t slave_node, uint16_t slave_addr, uint16_t nbr_regs) {
	uint8_t *pData;
	uint8_t i, nbr_bytes;
    uint8_t data = 0;
	uint16_t err = MB_ERR_NONE;
	uint16_t reg;
	uint16_t reg_val;

	
	if (nbr_regs == 0 || nbr_regs > 125)
		return MB_ERR_ILLEGAL_DATA_QTY;

	MBM_TX_FRAME_ADDR = slave_node;
	MBM_TX_FRAME_FC = 15;
	MBM_TX_DATA_START_H = slave_addr >> 8;
	MBM_TX_DATA_START_L = (uint8_t)slave_addr;
	MBM_TX_DATA_REG_H = nbr_regs >> 8;
	MBM_TX_DATA_REG_L = (uint8_t)nbr_regs;

	pData =  &tx[6];

	nbr_bytes = (nbr_regs % 8) ? (nbr_regs / 8) + 1 : (nbr_regs / 8);
	*pData++ = nbr_bytes;


	reg = slave_addr;
  while(nbr_regs)
  {
    data  = 0;
    for(i = 0; i < 8 ; i ++)
    {
      reg_val = MB_RdHoding(LWC_ADDR , Coil , reg , &err);;//Coil线圈
      if(reg_val)  data |= (0x01 << i);
      reg++; 
      nbr_regs --;
      if(0 == nbr_regs) break;
    }
    *pData++ = data;
  }
	
	MBM_TX_FRAME_NBYTES = 7 + nbr_bytes;
	MB_RTU_TX(uart3_tx_buf,&MB_Params[HFIM5_UART_3]);
	return err;
}



uint16_t MBM_FC16_Cmd(uint8_t *tx, uint8_t slave_node, uint16_t slave_addr, uint16_t nbr_regs) {
	uint8_t *pData;
	uint8_t i, nbr_bytes;
	uint16_t err = MB_ERR_NONE;
	uint16_t reg;
	uint16_t reg_val;

	
	if (nbr_regs == 0 || nbr_regs > 125)
		return MB_ERR_ILLEGAL_DATA_QTY;

	MBM_TX_FRAME_ADDR = slave_node;
	MBM_TX_FRAME_FC = 16;
	MBM_TX_DATA_START_H = slave_addr >> 8;
	MBM_TX_DATA_START_L = (uint8_t)slave_addr;
	MBM_TX_DATA_REG_H = nbr_regs >> 8;
	MBM_TX_DATA_REG_L = (uint8_t)nbr_regs;

	pData =  &tx[6];
	nbr_bytes = nbr_regs * 2;
	*pData++ = nbr_bytes;


	reg = slave_addr;
	for (i = 0; i < nbr_regs; i ++) {
		reg_val = MB_RdHoding(LWC_ADDR , Holding , reg , &err);
		*pData++ = reg_val >> 8;
		*pData++ = reg_val;
		reg++;
	}
	
	MBM_TX_FRAME_NBYTES = 7 + nbr_bytes;
	MB_RTU_TX(uart3_tx_buf , &MB_Params[HFIM5_UART_3]);
	return err;
}


void RcCom_App_Query(void) {
  static uint8_t tree = 0;

  uint8_t finish = 0;
  if(OUT_VER_STA)
  {
    OUT_VER_STA = 0;
    MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 1001, 10);
    return;
  }
  else if(OUT_SET_STA)
  {
    OUT_SET_STA = 0;
    MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 4001, 26);
    return;
  }
  else if(OUT2_SET_STA)
  {
    OUT2_SET_STA = 0;
    MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 5001, 19);
    return;
  }
  else if(LWC_INIT_STA)
  {
    LWC_INIT_STA = 0;
    MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 3001, 5);
    return;
  }
  else if(RST_DEBUG_STA)
  {
    RST_DEBUG_STA = 0;
    MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 2001, 12);
    return;
  }
  switch(tree)
  {
    case 0:
      if(COMM_DIR)  /*写入*/
      {
//        if(COMM_DEBUG_STA)
//        {
//          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 2001, 11);
//          finish = 1;
//        }
//        else 
        if(COMM_IN_BUF_STA && SEND_READY)
        {
          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 3001, 5);
          finish = 1;
        }
        else if(COMM_OUT_BUT_STA && HAVE_OUT_BUF)
        {
          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 4001, 26);
          finish = 1;
        }
       
        else if(COMM_OUT2_BUT_STA && HAVE_OUT2_BUF)
        {
          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 5001, 19);
          finish = 1;
        }
        
        else if(COMM_IN_VER)
        {
          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 1, 10);
          finish = 1;
        }
        else if(COMM_OUR_VER && HAVE_OUT_VER)
        {
          MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 1001, 10);
          finish = 1;
        }        
      }
      else          /*读取*/
      {
        if(COMM_DEBUG_STA)
        {
          MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 2001, 12);
          finish = 1;;
        }
        else if(COMM_IN_BUF_STA)
        {
          MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 3001, 5);
          finish = 1;;
        }
        else if(COMM_OUT_BUT_STA)
        {
          MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 4001, 26);
          finish = 1;;
        }
        else if(COMM_OUT2_BUT_STA)
        {
          MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 5001, 19);
          finish = 1;;
        }
      }
      if(1 == finish)
      {
        tree = 4;
        return;
      }
      MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 7001, 32); /*室外板状态*/
      tree = 1;
      break;
    case 1:
      MBM_FC15_Cmd(&uart3_tx_buf[0], LWC_ADDR, 7001, 58); /*室外板故障*/
      tree = 2;
      break;
    case 2:
      MBM_FC16_Cmd(&uart3_tx_buf[0], LWC_ADDR, 9001, 9); /*室内板状态*/
      tree = 3;
      break;
    case 3:
      MBM_FC15_Cmd(&uart3_tx_buf[0], LWC_ADDR, 9001, 12); /*室内板故障*/
      tree = 4;
      break;
    default:
      MBM_FC03_Cmd(&uart3_tx_buf[0], LWC_ADDR, 8001, 2); /*室内板故障*/
      tree = 0;
      break;
  }
}

void Uart3_Tmr(void)
{
    if (MB_Params[HFIM5_UART_3].RTU_TimeoutEn == TRUE) {
        MB_Params[HFIM5_UART_3].RTU_TimeoutCtr--;
    if (MB_Params[HFIM5_UART_3].RTU_TimeoutCtr == 0) {
        MB_Params[HFIM5_UART_3].RTU_TimeoutEn = FALSE;
    }
  }
  
    if (MB_Params[HFIM5_UART_3].SleepAfterRxCtr) {
		MB_Params[HFIM5_UART_3].SleepAfterRxCtr--;
		}
}


void Uart_Master_Pool(MB_ParamsType *up) {
	
	if (up->QueryStep == Q_STEP_QUERY) {

    RcCom_App_Query();		
    if (up->TxBufByteCtr > 0)
    {
      up->TxEnd = FALSE;
      up->TxBufPtr = &uart3_tx_buf[0];
      hfim5_uart_set_tx_en(HFIM5_UART_3,TRUE);
      hfim5_uart_start_transmit(HFIM5_UART_3,up->TxBufByteCtr);
      up->RTU_TimeoutCtr = up->FirstRxTimeOutCnt;
      up->QueryStep = Q_STEP_WAIT_TX;
    }
	}
	else if (up->QueryStep == Q_STEP_WAIT_TX) {
		if (up->TxEnd == TRUE) {
			up->RTU_TimeoutEn = TRUE;
			up->QueryStep = Q_STEP_WAIT;
		}
	}
	else if (up->QueryStep == Q_STEP_WAIT) {
		if (up->RTU_TimeoutCtr == 0) {
      MBM_RTU_RxReply(&MB_Params[HFIM5_UART_3]);
			
			up->RTU_TimeoutEn = FALSE;
			up->RxBufPtr = &uart3_rx_buf[0];
			up->RxBufByteCtr = 0;

			up->SleepAfterRxCtr = up->SleepAfterRxCnt;
			up->QueryStep = Q_STEP_SLEEP;
			up->TxEnd = FALSE;
		}
	}
	else if (up->QueryStep == Q_STEP_SLEEP) {
		if (up->SleepAfterRxCtr == 0) {
			up->QueryStep = Q_STEP_QUERY;
		}
	}
	else {
		up->QueryStep = Q_STEP_QUERY;
	}
}



void MB3_Init(MB_ParamsType *param) {  
  param->RxBufByteCtr = 0;
  param->RTU_TimeoutCtr = 0;
  param->RTU_TimeoutCnts = 5; 
  param->FirstRxTimeOutCnt = 5;
  param->RxBufPtr = &uart3_rx_buf[0];
  param->TxCtr = param->TxBufByteCtr = 0;
  param->TxBufPtr = &uart3_tx_buf[0];
  param->SleepAfterRxCnt = 3;
}



uint16_t MBM_DATA_holdingRegRd(void) {
  uint8_t *prx_data;
  uint16_t reg;
  uint16_t reg_val_16;
  uint16_t nbr_regs;
  uint16_t err;
  nbr_regs = ((uart3_tx_buf[4]) << 8) | (uart3_tx_buf[5]);
  reg = ((uart3_tx_buf[2]) << 8) | (uart3_tx_buf[3]);
  prx_data = &uart3_rx_buf[3];

  while(nbr_regs) {

    reg_val_16 = (*prx_data++) << 8;
    reg_val_16 |= *prx_data++;
      
    MB_WrHoding(LWC_ADDR,Holding,reg, reg_val_16, &err);
    
    if (err == MB_ERR_NONE) {
      reg ++;
      nbr_regs --;
    }
    else{
      return err;
    }
  }
  return MB_ERR_NONE;
}


