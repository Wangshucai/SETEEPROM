/**********************************************
;**********************************************
;***      UART ͨѶ
;***      Author: ����
;***      Date:   2016-12-19
;**********************************************
;**********************************************/

#define HW_UART

#include "hw_uart.h"

/****************************************************************************
 * UART1�ӿڻص�����
 ****************************************************************************/
void uart1_rx_adata(uint16_t data, uint8_t abyte) {

}

void uart1_rx_frame(uint16_t count) {

}

void uart1_tx_clpt(void) {

}



/****************************************************************************
 * UART4�ӿڻص�����
// ****************************************************************************/
//void uart4_rx_adata(uint16_t data, uint8_t abyte) {
////  UartParams[HFIM5_UART_4].RecAFrameFlag = TRUE;
//}

//void uart4_rx_frame(uint16_t count) {
////  UartParams[HFIM5_UART_4].Rx.RxBufByteCtr = count;
//}

//void uart4_tx_clpt(void) {
////  hfim5_uart_set_tx_en(HFIM5_UART_4, FALSE);  /*���ܺ���*/
////  hfim5_uart_start_receive(HFIM5_UART_4); 
////  UartParams[HFIM5_UART_4].TxStop = 1;
//}




