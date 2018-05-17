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

#ifdef  HW_UART 

  #define HW_UART
#else
  #define HW_UART  extern                    
#endif
  
typedef enum{Coil =0, Holding}MBType;

#define UART1_RX_BUF_SIZE			256
#define UART1_TX_BUF_SIZE			256
  
HW_UART uint8_t uart1_rx_buf[UART1_RX_BUF_SIZE];
HW_UART uint8_t uart1_tx_buf[UART1_TX_BUF_SIZE];

void uart1_rx_adata(uint16_t data, uint8_t abyte);
void uart1_rx_frame(uint16_t count);
void uart1_tx_clpt(void);
/****************************************************************************
 * UART2
 ****************************************************************************/

#define UART2_RX_BUF_SIZE			256
#define UART2_TX_BUF_SIZE			256
  
HW_UART uint8_t uart2_rx_buf[UART2_RX_BUF_SIZE];
HW_UART uint8_t uart2_tx_buf[UART2_TX_BUF_SIZE];
  
void uart2_rx_adata(uint16_t data, uint8_t abyte);
void uart2_rx_frame(uint16_t count);
void uart2_tx_clpt(void);
/****************************************************************************
 * UART3
 ****************************************************************************/


#define UART3_RX_BUF_SIZE			256
#define UART3_TX_BUF_SIZE			256

HW_UART uint8_t uart3_rx_buf[UART3_RX_BUF_SIZE];
HW_UART uint8_t uart3_tx_buf[UART3_TX_BUF_SIZE];

void uart3_rx_adata(uint16_t data, uint8_t abyte);
void uart3_rx_frame(uint16_t count);
void uart3_tx_clpt(void);

/****************************************************************************
 * UART4
 ****************************************************************************/
#define UART4_RX_BUF_SIZE			256
#define UART4_TX_BUF_SIZE			256
HW_UART uint8_t uart4_rx_buf[UART4_RX_BUF_SIZE];
HW_UART uint8_t uart4_tx_buf[UART4_TX_BUF_SIZE];

void uart4_rx_adata(uint16_t data, uint8_t abyte);
void uart4_rx_frame(uint16_t count);
void uart4_tx_clpt(void);


typedef  __packed struct  {
  uint8_t   NodeAddr;    //通信地址
  uint8_t   RTU_TimeoutEn;   //超时
  uint16_t	RTU_TimeoutCnts; //超时计数赋值变量 ：RTU_TimeoutCnts->RTU_TimeoutCtr
  uint16_t	RTU_TimeoutCtr;  //超时计数器  --
  
	uint8_t QueryStep;     //回复步骤
	uint8_t Delay;	
	uint8_t RecFrmFlag;     //接收标志
	uint8_t MasterSlave;    //主从模式

  uint32_t	RxTimeout;    //接收时间超时计数
  uint32_t	RxCtr;        
  uint16_t	RxBufByteCtr; //接收字节计数
  uint8_t   *RxBufPtr;   //接收缓存数组

  uint32_t	TxCtr;
  uint16_t	TxBufByteCtr; //发送字节计数
  uint8_t   *TxBufPtr;    //发送缓存地址
  uint8_t   TxEnd;          //返回发送结束
  uint8_t   FirstRxTimeOutCnt;//首次接收时间超时计数
  uint8_t	SleepAfterRxCnt;  //
  volatile uint8_t		SleepAfterRxCtr;
  uint8_t RecAFrameFlag;   //接收标志
} MB_ParamsType;


#define MB_RTU_MIN_MSG_SIZE                 4

#define Q_STEP_QUERY						0
#define Q_STEP_WAIT_TX					    1
#define Q_STEP_WAIT							2
#define Q_STEP_SLEEP						3

#define S_WAIT_REC							0
#define S_DLY_RESP							1

#define  MB_ERR_NONE                            0
#define  MB_ERR_ILLEGAL_FC                      1  //功能码错误
#define  MB_ERR_ILLEGAL_DATA_ADDR               2  //地址错误
#define  MB_ERR_ILLEGAL_DATA_QTY                3  //数据个数范围错误
#define  MB_ERR_ILLEGAL_DATA_VAL                4  //数据值错误
#define  MB_ERR_SLAVE_DEVICE_BUSY	        	6  //从机忙

#define  MB_ERR_TIMED_OUT                    3000

#define  MB_ERR_COIL_ADDR                    5000   //
#define  MB_ERR_COIL_WR                      5001
#define  MB_ERR_SLAVE_ADDR                   5002
#define  MB_ERR_FC                           5003
#define  MB_ERR_BYTE_COUNT                   5004
#define  MB_ERR_COIL_QTY                     5005
#define  MB_ERR_REG_ADDR                     5006
#define  MB_ERR_NBR_REG                      5007
#define  MB_ERR_SUB_FNCT                     5008
#define  MB_ERR_DIAG                         5009
#define  MB_ERR_WR                           5010

#define  MB_ERR_RX                           6000

HW_UART MB_ParamsType MB_Params[4];

void Uart_Master_Pool(MB_ParamsType *up);

void MB3_Init(MB_ParamsType *param);

void UART_Slave_Task(MB_ParamsType *up);
void MB2_Init(uint8_t addr,MB_ParamsType *param);
void MB_RTU_TX(uint8_t *tx,MB_ParamsType *param);

void DH_Slave_Task(MB_ParamsType *up);
void MB4_Init(uint8_t addr,MB_ParamsType *param);





