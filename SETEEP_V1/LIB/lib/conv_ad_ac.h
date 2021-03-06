#ifndef __CONV_AD_AC_H
#define __CONV_AD_AC_H
#include <stdint.h>

/*******************************************************************************
* 将12位的AD采样值转换为交流电压有效值
* 入口参数：
*     val:  AD采样值
* 出口参数：
*     转换后的电压值(范围：0~280V)
*******************************************************************************/
uint16_t AD_Conv_AC(uint16_t val);
#endif
