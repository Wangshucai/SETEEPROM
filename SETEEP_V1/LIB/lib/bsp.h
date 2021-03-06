#ifndef __BSP_H
#define __BSP_H

#include <stdint.h>
#include "utils.h"

/****************************************************************************
 * 数字输入
 ****************************************************************************/
 
 /* 输入值（经过去抖）定义 */
#define HFIM5_DIF_VAL_L						0									/* 输入低电平 */
#define HFIM5_DIF_VAL_H						1									/* 输入高电平 */
#define HFIM5_DIF_VAL_UNKNOWN			2									/* 输入值未知 */

/* 板载数字输入数据结构定义 */
typedef struct {
	uint16_t di_1:						2;											/* 对应板上丝印DI1,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_2:						2;											/* 对应板上丝印DI2,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_3:						2;											/* 对应板上丝印DI3,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_4:						2;											/* 对应板上丝印DI4,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_5:						2;											/* 对应板上丝印DI5,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_6:						2;											/* 对应板上丝印DI6,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_7:						2;											/* 对应板上丝印DI7,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_8:						2;											/* 对应板上丝印DI8,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_9:						2;											/* 对应板上丝印DI9,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_10:						2;											/* 对应板上丝印DI10, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_11:						2;											/* 对应板上丝印DI11, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_17:						2;											/* 对应板上丝印DI17, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_18:						2;											/* 对应板上丝印DI18, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_19:						2;											/* 对应板上丝印DI19, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t di_20:						2;											/* 对应板上丝印DI20, 取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
	uint16_t test:						2;											/* 对应板上丝印TST,  取值范围： HFIM5_DIF_VAL_L~HFIM5_DIF_VAL_UNKNOWN */
}hfim5_di_filter_t;

/* 风速检测端口定义 */
typedef enum {
	HFIM5_TM_1 = 0,																		/* 冷冻水：对应板上丝印DI1，其它：对应板上丝印DI9 */
	HFIM5_TM_2 = 1,																		/* 冷冻水：对应板上丝印DI2，其它：对应板上丝印DI11 */
	HFIM5_TM_3 = 2																		/* 冷冻水：对应板上丝印DI3，其它： */
}hfim5_tm_id;

/****************************************************************************
 * 数字输出
 ****************************************************************************/

/* 数字输入端口定义 */
typedef enum {
	HFIM5_DO_1 = 0,																		/* 对应板上丝印DO1  */
	HFIM5_DO_2 = 1,																		/* 对应板上丝印DO2  */
	HFIM5_DO_3 = 2,																		/* 对应板上丝印DO3  */
	HFIM5_DO_4 = 3,																		/* 对应板上丝印DO4  */
	HFIM5_DO_5 = 4,																		/* 对应板上丝印DO5  */
	HFIM5_DO_6 = 5,																		/* 对应板上丝印DO6  */
	HFIM5_DO_7 = 6,																		/* 对应板上丝印DO7  */
	HFIM5_DO_8 = 7,																		/* 对应板上丝印DO8  */
	HFIM5_DO_9 = 8,																		/* 对应板上丝印DO9  */
	HFIM5_DO_10 = 9,																	/* 对应板上丝印DO10 */
	HFIM5_DO_11 = 10,																	/* 对应板上丝印DO11 */
	HFIM5_DO_12 = 11,																	/* 对应板上丝印DO12 */
	HFIM5_DO_13 = 12,																	/* 对应板上丝印DO13 */
	HFIM5_DO_14 = 13,																	/* 对应板上丝印DO14 */
	HFIM5_DO_15 = 14,																	/* 对应板上丝印DO15 */
	HFIM5_DO_16 = 15,																	/* 对应板上丝印DO16 */
	HFIM5_DO_17 = 16,																	/* 对应板上丝印DO17 */
	HFIM5_DO_18 = 17																	/* 对应板上丝印DO18 */
}hfim5_do_id;

/* 输出值定义 */
#define HFIM5_DO_VAL_L						0									/* 输出低电平 */
#define HFIM5_DO_VAL_H						1									/* 输出高电平 */

/****************************************************************************
 * 模拟输入
 ****************************************************************************/

/* 板载模拟输入（经过滤波）数据结构定义 */
typedef struct {
	uint16_t ai_1;																		/* 对应板上丝印AI1  */
	uint16_t ai_2;																		/* 对应板上丝印AI2  */
	uint16_t ai_3;																		/* 对应板上丝印AI3  */
	uint16_t ai_4;																		/* 对应板上丝印AI4  */
	uint16_t ai_5;																		/* 对应板上丝印AI5  */
	uint16_t ai_6;																		/* 对应板上丝印AI6  */
	uint16_t ai_7;																		/* 对应板上丝印AI7  */
	uint16_t ai_8;																		/* 对应板上丝印AI8  */
	uint16_t ai_9;																		/* 对应板上丝印AI9  */
	uint16_t ai_10;																		/* 对应板上丝印AI10 */
	uint16_t ai_11;																		/* 对应板上丝印AI11 */
	uint16_t ai_12;																		/* 对应板上丝印AI12 */
	uint16_t ai_13;																		/* 对应板上丝印AI13 */
	uint16_t ai_14;																		/* 对应板上丝印AI14 */
}hfim5_ai_filter_t;

/* 模拟输入值定义 */
#define HFIM5_AI_UNKNOWN				-32767							/* 未知(检测中) */

/****************************************************************************
 * 模拟输出
 ****************************************************************************/

/* 模拟输出端口定义 */
typedef enum {
	HFIM5_AO_1 = 0,																		/* 对应板上丝印AO1  */
	HFIM5_AO_2 = 1																		/* 对应板上丝印AO2  */
}hfim5_ao_id;

/****************************************************************************
 * EEPROM
 ****************************************************************************/

/* EEPROM状态定义 */
typedef enum {
	ES_BUSY_WRITE = 1,																/* 写操作忙      */
	ES_BUSY_READ = 2,																  /* 读操作忙      */
	ES_READY = 3,																      /* EEPROM就绪  */
	ES_ERROR = 4																      /* 操作错误      */
}EEPROM_State;

/****************************************************************************
 * 串口
 ****************************************************************************/

/* 串口端口定义 */
typedef enum {
	HFIM5_UART_1 = 0,																	/* 对应functions定义USART1 */
	HFIM5_UART_2 = 1,																	/* 对应functions定义USART2 */
	HFIM5_UART_3 = 2,																	/* 对应functions定义USART3 */
	HFIM5_UART_4 = 3,																	/* 对应functions定义USART4 */
}hfim5_uart_id;

/* 串口停止位定义 */
typedef enum {
	BSPU_STOPBITS_1 = 0,															/* 1位停止位 */
	BSPU_STOPBITS_2 = 1															  /* 2位停止位 */
}bsp_uart_stopbits;

/* 串口校验位定义 */
typedef enum {
	BSPU_PARITY_NONE = 0,															/* 无校验 */
	BSPU_PARITY_EVEN = 1,															/* 偶校验 */
	BSPU_PARITY_ODD = 2																/* 奇校验 */
}bsp_uart_parity;

/* 串口数据位定义 */
typedef enum {
	BSPU_DATABITS_7 = 0,															/* 7位数据 */
	BSPU_DATABITS_8 = 1,															/* 8位数据 */
	BSPU_DATABITS_9 = 2																/* 9位数据 */
}bsp_uart_databits;

/* 缓冲区结构体定义 */
typedef struct {
	uint8_t *buf;																			/* 缓冲区地址 */
	uint16_t size;																		/* 缓冲区大小 */
}bsp_buffer_t;

/* 串口操作故障码定义 */
#define BSP_UART_ERROR_NONE      ((uint32_t)0x00000000)				/* 无错误     */
#define BSP_UART_ERROR_PE        ((uint32_t)0x00000001)				/* 校验错误    */
#define BSP_UART_ERROR_NE        ((uint32_t)0x00000002)				/* 噪声错误    */
#define BSP_UART_ERROR_FE        ((uint32_t)0x00000004)				/* 帧错误     */
#define BSP_UART_ERROR_ORE       ((uint32_t)0x00000008)				/* 溢出错误    */
#define BSP_UART_ERROR_DMA       ((uint32_t)0x00000010)				/* DMA传输错误 */
#define BSP_UART_ERROR_BO				 ((uint32_t)0x00000020)				/* 缓冲区溢出错误 */

/* 串口设备配置结构体定义 */
typedef struct {
	uint32_t baudrate;																					/* 波特率     */
	uint32_t timeouts;																					/* 字节间超时时间 */
	bsp_uart_databits databits;																	/* 数据位     */
	bsp_uart_stopbits stopbits;																	/* 停止位     */
	bsp_uart_parity parity;																			/* 校验位     */
}bsp_uart_dev_s;

/* 串口缓冲区结构体定义 */
typedef struct {
	bsp_buffer_t rx;																						/* 接收缓冲区 */
	bsp_buffer_t tx;																						/* 发送缓冲区 */
}bsp_uart_buffers_s;

/* 串口设备回调函数结构体定义 */
typedef struct {
/****************************************************************************
 * 串口接收一个字节/字数据的回调函数
 * 入口参数：
 *     data:      		接收到的数据
 *     abyte:     		数据长度(TRUE:单字节；FALSE:字。依赖于bsp_uart_dev_s结构体中databits的设置)
 * 出口参数：
 *     无
 * 说明：
 *     1、当bsp_uart_dev_s结构体中timeouts等于0时，应用层需将接收到的内容放入到接收缓冲区中，
 *     并累加接收长度等信息；当timeouts>0时，驱动会做这些处理
 *     2、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*on_rx_a_data)(uint16_t data, uint8_t abyte);
	
/****************************************************************************
 * 串口发送完一帧数据的回调函数
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*tx_cplt_callback)(void);
	
/****************************************************************************
 * 串口接收到一帧数据的回调函数
 * 入口参数：
 *     count:      		接收到的数据的数量(如bsp_uart_dev_s结构体中databits为9位数据位，
 *                    则接收的字节数为count * 2)
 * 出口参数：
 *     无
 * 说明：
 *     1、当bsp_uart_dev_s结构体中timeouts等于0时，驱动不会调用此回调函数
 *     2、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*rx_frame_callback)(uint16_t count);
	
/****************************************************************************
 * 串口在接收/发送过程中产生的错误
 * 入口参数：
 *     error:      		错误代码，见BSP_UART_ERROR_NONE~BSP_UART_ERROR_BO
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*error_callback)(uint32_t error);
}bsp_uart_calllback_s;

/* 串口结构体定义 */
typedef struct {
	bsp_uart_dev_s dev;																					/* 配置   */
	bsp_uart_buffers_s buffer;																	/* 缓冲区  */
	bsp_uart_calllback_s callbacks;															/* 回调函数 */
}bsp_uart_t;

/****************************************************************************
 * SPI
 ****************************************************************************/
#define BSP_SPI_ERROR_NONE   			(uint32_t)0x00000000				/* 无错误                   */
#define BSP_SPI_ERROR_MODF   			(uint32_t)0x00000001  			/* 模式错误                  */
#define BSP_SPI_ERROR_CRC    			(uint32_t)0x00000002  			/* CRC校验错误               */
#define BSP_SPI_ERROR_OVR    			(uint32_t)0x00000004  			/* 溢出错误                  */
#define BSP_SPI_ERROR_FRE    			(uint32_t)0x00000008  			/* TI帧格式错误               */
#define BSP_SPI_ERROR_DMA    			(uint32_t)0x00000010  			/* DMA传输错误               */
#define BSP_SPI_ERROR_FLAG   			(uint32_t)0x00000020  			/* BSY/TXE/FTLVL/FRLVL标志 */
#define BSP_SPI_ERROR_UNKNOW 			(uint32_t)0x00000040  			/* 未知错误                  */
#define BSP_SPI_ERROR_BO					(uint32_t)0x00000080				/* 缓冲区溢出                 */


/* SPI主从机定义 */
typedef enum {
	BSPS_SLAVE = 0,																							/* SPI从机 */
	BSPS_MASTER = 1,																						/* SPI主机 */
}bsp_spi_master_slave;

/* SPI设备配置结构体定义 */
typedef struct {
	bsp_spi_master_slave mode;																	/* 主/从机    */
	uint32_t timeouts;																					/* 字节间的超时时间，需大于0 */
}bsp_spi_dev_s;

/* SPI缓冲区结构体定义 */
typedef struct {
	bsp_buffer_t rx;																						/* 接收缓冲区 */
	bsp_buffer_t tx;																						/* 发送缓冲区 */
}bsp_spi_buffers_s;

/* SPI设备回调函数结构体定义 */
typedef struct {

/****************************************************************************
 * SPI发送完一帧数据的回调函数
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*tx_cplt_callback)(void);
	
/****************************************************************************
 * SPI接收到一个字节数据的回调函数
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*on_rx_a_data)(uint8_t val);
	
/****************************************************************************
 * SPI接收到一帧数据的回调函数
 * 入口参数：
 *     count:      		接收到的数据的字节数
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*rx_frame_callback)(uint16_t count);
	
/****************************************************************************
 * SPI在接收/发送过程中产生的错误
 * 入口参数：
 *     error:      		错误代码，BSP_SPI_ERROR_NONE~BSP_SPI_ERROR_BO
 * 出口参数：
 *     无
 * 说明：
 *     1、应用程序在此回调函数的执行时间需尽可能少，且不能有阻塞语句，否则会影响其他中断执行
 ****************************************************************************/
	void (*error_callback)(uint32_t error);
}bsp_spi_calllback_s;

/* SPI结构体定义 */
typedef struct {
	bsp_spi_dev_s dev;																					/* 配置   */
	bsp_spi_buffers_s buffer;																		/* 缓冲区  */
	bsp_spi_calllback_s callbacks;															/* 回调函数 */
}bsp_spi_t;

/****************************************************************************
 * 相序保护
 ****************************************************************************/

/* 相序保护检测结果定义 */
typedef enum {
	BSP_PO_NORMAL = 0,																					/* 正常 */
	BSP_PO_P_REV = 1,																						/* 逆相 */
	BSP_PO_P_LST = 2,																						/* 反相 */
	BSP_PO_UNKNOWN = 3																					/* 未知 */
}bsp_po_result;

/****************************************************************************
 * 整板
 ****************************************************************************/

/* 板载外设初始化结构体定义 */
typedef struct {
	struct {
		uint32_t feed_time;																				/* 独立看门狗的复位时间，单位:ms */
	}iwdg;
	struct {
		uint32_t clk_freq;																				/* 定时器时钟频率, 频率范围24000000/65535Hz~24MHz */
		uint32_t cycle;																						/* 定时周期，单位:us */
		void (*on_time)(void);																		/* 定时中断回调函数 */
	}hw_tmr;																										/* 用户定时器 */

	bsp_uart_t uart1;																						/* 串口1初始化，丝印：A1+、A1- */
	bsp_uart_t uart2;																						/* 串口2初始化，丝印：A2+、A2- */
	bsp_uart_t uart3;																						/* 串口3初始化，丝印：A3+、A3- */
	bsp_uart_t uart4;																						/* 串口4初始化，丝印：A4+、A4- */
	bsp_spi_t spi;																							/* SPI初始化 */
	
}bsp_init_t;

/* 板载输入接口数据结构定义 */
typedef struct {
	hfim5_di_filter_t di;																				/* 数字输入 */
	hfim5_ai_filter_t ai;																				/* 模拟输入 */
	int16_t spd1;																								/* 风速1 */
	int16_t spd2;																								/* 风速2 */
	int16_t spd3;																								/* 风速3 */
	bsp_po_result po;																						/* 相序检测 */
}hfim5_inputs;


/****************************************************************************
 * 主板外设初始化
 * 入口参数：
 *     hfim5:      		指向外设初始化配置信息的指针
 *     input:         指向外设输入信息的指针
 * 出口参数：
 *     无
 * 说明：
 *     此函数需在main函数开始处被调用，且只能被调用一次
 ****************************************************************************/
void hfim5_init(bsp_init_t *hfim5, hfim5_inputs *input);

/****************************************************************************
 * 主板驱动程序运行
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 * 说明：
 *     此函数需在main函数中的while循环中调用
 ****************************************************************************/
void hfim5_run(void);

/****************************************************************************
 * 使能/禁止风速检测
 * 入口参数：
 *     id:            风速检测口ID
 *     en:            使能(TRUE)/禁止(FALSE)
 *     ppr:           每转脉冲数，需大于1
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_tm_set_en(hfim5_tm_id id, uint8_t en, uint8_t ppr);

/****************************************************************************
 * 指定数字输出口的输出
 * 入口参数：
 *     id:            数字输出口ID
 *     val:           输出值(0/1)
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_do_output(hfim5_do_id id, uint8_t val);

/****************************************************************************
 * 翻转指定数字输出口的输出
 * 入口参数：
 *     id:            数字输出口ID
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_do_toggle(hfim5_do_id id);

/****************************************************************************
 * 指定模拟输出口的输出
 * 入口参数：
 *     id:            模拟输出口ID
 *     vol:           输出值，单位：mv
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_ao_output_mv(hfim5_ao_id ao, uint16_t vol);

/****************************************************************************
 * 指定模拟输出口的输出
 * 入口参数：
 *     id:            模拟输出口ID
 *     percent:       输出值，单位：%
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_ao_output_per(hfim5_ao_id ao, uint16_t percent);

/****************************************************************************
 * EEPROM写入
 * 入口参数：
 *     mem_addr:      写入到EEPROM中的地址
 *     buffer:        待写入数据的首址
 *     size:          待写入的长度(字节)
 * 出口参数：
 *     无
 * 说明：
 *     1、此函数仅实现EEPROM页面内的写操作，不能跨页写
 *     2、建议在此操作前后都先通过调用hfim5_eeprom_get_state()来获取当前的状态
 ****************************************************************************/
void hfim5_eeprom_page_write(uint16_t mem_addr, uint8_t *buffer, uint16_t size);

/****************************************************************************
 * EEPROM读取
 * 入口参数：
 *     mem_addr:      写入到EEPROM中的地址
 *     buffer:        待读入缓冲区的首址
 *     size:          待读取的长度(字节)
 * 出口参数：
 *     无
 * 说明：
 *     1、建议在此操作前后都先通过调用hfim5_eeprom_get_state()来获取当前的状态
 ****************************************************************************/
void hfim5_eeprom_read(uint16_t mem_addr, uint8_t *buffer, uint16_t size);

/****************************************************************************
 * 获取EEPROM当前的状态
 * 入口参数：
 *     无
 * 出口参数：
 *     EEPROM当前的状态，见EEPROM_State定义
 ****************************************************************************/
EEPROM_State hfim5_eeprom_get_state(void);

/****************************************************************************
 * 启用用户定时器
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_usr_tmr_run(void);

/****************************************************************************
 * 停止用户定时器
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_usr_tmr_stop(void);

/****************************************************************************
 * 使能/禁止指定的串口
 * 入口参数：
 *     id:            指定串口的ID号
 *     en:            使能(TRUE)/禁止(FALSE)
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_uart_set_en(hfim5_uart_id id, uint8_t en);

/****************************************************************************
 * 设置指定的串口的发送使能端口
 * 入口参数：
 *     id:            指定串口的ID号
 *     en:            使能(TRUE)/禁止(FALSE)
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_uart_set_tx_en(hfim5_uart_id id, uint8_t en);

/****************************************************************************
 * 设定指定的串口的参数
 * 入口参数：
 *     id:            指定串口的ID号
 *     setting:       指向设定参数的指针
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_uart_setting(hfim5_uart_id id, bsp_uart_dev_s *setting);

/****************************************************************************
 * 允许指定的串口开始接收数据
 * 入口参数：
 *     id:            指定串口的ID号
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_uart_start_receive(hfim5_uart_id id);

/****************************************************************************
 * 允许指定的串口的开始发送数据
 * 入口参数：
 *     id:            指定串口的ID号
 *     count:         待发送数据的长度
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_uart_start_transmit(hfim5_uart_id id, uint16_t count);

/****************************************************************************
 * 使能/禁止SPI接口
 * 入口参数：
 *     en:            使能(TRUE)/禁止(FALSE)
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_spi_set_en(uint8_t en);

/****************************************************************************
 * 设定SPI接口的参数
 * 入口参数：
 *     setting:       指向设定参数的指针
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_spi_setting(bsp_spi_dev_s *setting);

/****************************************************************************
 * 允许SPI开始接收数据
 * 入口参数：
 *     无
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_spi_start_receive(void);

/****************************************************************************
 * 允许SPI开始发送数据
 * 入口参数：
 *     count:         待发送数据的长度
 * 出口参数：
 *     无
 ****************************************************************************/
void hfim5_spi_start_transmit(uint16_t count);
#endif
