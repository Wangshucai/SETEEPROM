/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

// DI端口引脚定义
#define DI1_Pin 							GPIO_PIN_9
#define DI1_GPIO_Port 				GPIOF
#define DI2_Pin 							GPIO_PIN_12
#define DI2_GPIO_Port 				GPIOB
#define DI3_Pin 							GPIO_PIN_2
#define DI3_GPIO_Port 				GPIOB
#define DI4_Pin 							GPIO_PIN_7
#define DI4_GPIO_Port 				GPIOE
#define DI5_Pin 							GPIO_PIN_8
#define DI5_GPIO_Port 				GPIOE
#define DI6_Pin 							GPIO_PIN_10
#define DI6_GPIO_Port 				GPIOE
#define DI7_Pin 							GPIO_PIN_11
#define DI7_GPIO_Port 				GPIOE
#define DI8_Pin 							GPIO_PIN_12
#define DI8_GPIO_Port 				GPIOE
#define DI9_Pin 							GPIO_PIN_10
#define DI9_GPIO_Port 				GPIOF
#define DI10_Pin 							GPIO_PIN_4
#define DI10_GPIO_Port 				GPIOE
#define DI11_Pin 							GPIO_PIN_5
#define DI11_GPIO_Port 				GPIOE
#define TST_Pin 							GPIO_PIN_6
#define TST_GPIO_Port 				GPIOE
#define DI17_Pin 							GPIO_PIN_10
#define DI17_GPIO_Port 				GPIOD
#define DI18_Pin 							GPIO_PIN_11
#define DI18_GPIO_Port 				GPIOD
#define DI19_Pin 							GPIO_PIN_13
#define DI19_GPIO_Port 				GPIOD
#define DI20_Pin 							GPIO_PIN_14
#define DI20_GPIO_Port 				GPIOD

// DO端口引脚定义
#define DO1_Pin 							GPIO_PIN_15
#define DO1_GPIO_Port 				GPIOD
#define DO2_Pin 							GPIO_PIN_6
#define DO2_GPIO_Port 				GPIOC
#define DO3_Pin 							GPIO_PIN_7
#define DO3_GPIO_Port 				GPIOC
#define DO4_Pin 							GPIO_PIN_8
#define DO4_GPIO_Port 				GPIOC
#define DO5_Pin 							GPIO_PIN_9
#define DO5_GPIO_Port 				GPIOC
#define DO6_Pin 							GPIO_PIN_8
#define DO6_GPIO_Port 				GPIOA
#define DO7_Pin 							GPIO_PIN_11
#define DO7_GPIO_Port 				GPIOA
#define DO8_Pin 							GPIO_PIN_6
#define DO8_GPIO_Port 				GPIOF
#define DO9_Pin 							GPIO_PIN_12
#define DO9_GPIO_Port 				GPIOC
#define DO10_Pin 							GPIO_PIN_7
#define DO10_GPIO_Port 				GPIOD
#define DO11_Pin 							GPIO_PIN_4
#define DO11_GPIO_Port 				GPIOB
#define DO12_Pin 							GPIO_PIN_5
#define DO12_GPIO_Port 				GPIOB
#define DO13_Pin 							GPIO_PIN_0
#define DO13_GPIO_Port 				GPIOD
#define DO14_Pin 							GPIO_PIN_1
#define DO14_GPIO_Port 				GPIOD
#define DO15_Pin 							GPIO_PIN_2
#define DO15_GPIO_Port 				GPIOD
#define DO16_Pin 							GPIO_PIN_3
#define DO16_GPIO_Port 				GPIOD
#define DO17_Pin 							GPIO_PIN_0
#define DO17_GPIO_Port 				GPIOE
#define DO18_Pin 							GPIO_PIN_1
#define DO18_GPIO_Port 				GPIOE

// AI端口引脚定义
#define AI1_Pin 							GPIO_PIN_0
#define AI1_GPIO_Port 				GPIOC
#define AI2_Pin 							GPIO_PIN_1
#define AI2_GPIO_Port 				GPIOC
#define AI3_Pin 							GPIO_PIN_2
#define AI3_GPIO_Port 				GPIOC
#define AI4_Pin 							GPIO_PIN_3
#define AI4_GPIO_Port 				GPIOC
#define AI5_Pin 							GPIO_PIN_0
#define AI5_GPIO_Port 				GPIOA
#define AI6_Pin 							GPIO_PIN_1
#define AI6_GPIO_Port 				GPIOA
#define AI7_Pin 							GPIO_PIN_2
#define AI7_GPIO_Port 				GPIOA
#define AI8_Pin 							GPIO_PIN_3
#define AI8_GPIO_Port 				GPIOA
#define AI9_Pin 							GPIO_PIN_6
#define AI9_GPIO_Port 				GPIOA
#define AI10_Pin 							GPIO_PIN_7
#define AI10_GPIO_Port 				GPIOA
#define AI11_Pin 							GPIO_PIN_4
#define AI11_GPIO_Port 				GPIOC
#define AI12_Pin 							GPIO_PIN_0
#define AI12_GPIO_Port 				GPIOB
#define AI13_Pin 							GPIO_PIN_1
#define AI13_GPIO_Port 				GPIOB
#define AI14_Pin 							GPIO_PIN_5
#define AI14_GPIO_Port 				GPIOC

// AO端口引脚定义
#define AO1_Pin 							GPIO_PIN_4
#define AO1_GPIO_Port 				GPIOA
#define AO2_Pin 							GPIO_PIN_5
#define AO2_GPIO_Port 				GPIOA

// SPI端口引脚定义
#define SCK_Pin 							GPIO_PIN_13
#define SCK_GPIO_Port 				GPIOE
#define MISO_Pin 							GPIO_PIN_14
#define MISO_GPIO_Port 				GPIOE
#define MOSI_Pin 							GPIO_PIN_15
#define MOSI_GPIO_Port 				GPIOE

// I2C端口引脚定义
#define SCL2_Pin 							GPIO_PIN_13
#define SCL2_GPIO_Port 				GPIOB
#define SDA2_Pin 							GPIO_PIN_14
#define SDA2_GPIO_Port 				GPIOB

// 串口端口引脚定义(RS485)
// UART1
#define TXD1_Pin 							GPIO_PIN_9
#define TXD1_GPIO_Port 				GPIOA
#define RXD1_Pin 							GPIO_PIN_10
#define RXD1_GPIO_Port 				GPIOA
#define COT1_Pin 							GPIO_PIN_12
#define COT1_GPIO_Port 				GPIOA

// UART2
#define TXD2_Pin 							GPIO_PIN_5
#define TXD2_GPIO_Port 				GPIOD
#define RXD2_Pin 							GPIO_PIN_6
#define RXD2_GPIO_Port 				GPIOD
#define COT2_Pin 							GPIO_PIN_4
#define COT2_GPIO_Port 				GPIOD

// UART3
#define TXD3_Pin 							GPIO_PIN_8
#define TXD3_GPIO_Port 				GPIOD
#define RXD3_Pin 							GPIO_PIN_9
#define RXD3_GPIO_Port 				GPIOD
#define COT3_Pin 							GPIO_PIN_12
#define COT3_GPIO_Port 				GPIOD

// UART4
#define TXD4_Pin 							GPIO_PIN_10
#define TXD4_GPIO_Port 				GPIOC
#define RXD4_Pin 							GPIO_PIN_11
#define RXD4_GPIO_Port 				GPIOC
#define COT4_Pin 							GPIO_PIN_15
#define COT4_GPIO_Port 				GPIOA

// 输入捕捉端口定义
#define U_CHK_Pin 						GPIO_PIN_10
#define U_CHK_GPIO_Port 			GPIOB
#define V_CHK_Pin 						GPIO_PIN_11
#define V_CHK_GPIO_Port 			GPIOB
#define W_CHK_Pin 						GPIO_PIN_3
#define W_CHK_GPIO_Port 			GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
