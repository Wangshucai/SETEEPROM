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

#ifdef  LWC_COMM 
  #define LWC_COMM
  
#else
  #define LWC_COMM  extern                    
#endif
 
#define LWC_ADDR							0x40
  
//#define MB_RTU_MIN_MSG_SIZE                     4

#define MB_FC_01                                1
#define MB_FC_02                                2
#define MB_FC_03                                3
#define MB_FC_04                                4
#define MB_FC_05                                5
#define MB_FC_06                                6
#define MB_FC_07                                7
#define MB_FC_08                                8
#define MB_FC_11                                11
#define MB_FC_12                                12
#define MB_FC_13                                13
#define MB_FC_14                                14
#define MB_FC_15                                15
#define MB_FC_16                                16
#define MB_FC_17                                17
#define MB_FC_20                                20
#define MB_FC_21                                21
#define MB_FC_22                                22
#define MB_FC_23                                23
#define MB_FC_24                                24
#define MB_FC_43                                43

#define MB_SLAVE																0
#define MB_MASTER																1


#define  MBM_TX_FRAME_ADDR      (tx[0])
#define  MBM_TX_FRAME_FC        (tx[1])
#define  MBM_TX_DATA_START_H    (tx[2])
#define  MBM_TX_DATA_START_L    (tx[3])
#define  MBM_TX_DATA_COIL_H     (tx[4])
#define  MBM_TX_DATA_COIL_L     (tx[5])
#define  MBM_TX_DATA_REG_H      (tx[4])
#define  MBM_TX_DATA_REG_L      (tx[5])

#define  MBM_TX_FRAME_NBYTES    (MB_Params[HFIM5_UART_3].TxBufByteCtr)

#define  MBM_RX_FRAME_ADDR      (rx->RxBufAddr[0])
#define  MBM_RX_FRAME_FC        (rx->RxBufAddr[1])
#define  MBM_RX_DATA_START_H    (rx->RxBufAddr[2])
#define  MBM_RX_DATA_START_L    (rx->RxBufAddr[3])
#define  MBM_RX_DATA_REG_H      (rx->RxBufAddr[4])
#define  MBM_RX_DATA_REG_L      (rx->RxBufAddr[5])

uint16_t MBM_DATA_holdingRegRd(void);
void Uart3_Tmr(void);

uint8_t MB_ChkFrame(hfim5_uart_id ch,uint8_t *rx);





