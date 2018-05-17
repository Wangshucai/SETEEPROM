#include "stm32f0xx.h"


#define cMSGLength                    20
#define cTaskCount                    20
#define cThreadCount                  3

#define TMR_TYPE_10MS                   0
#define TMR_TYPE_100MS                  1
#define TMR_TYPE_1SEC                   2
#define TMR_TYPE_1MIN                   3

#ifdef  __TASK_H
    #define TASK_extern
#else
    #define TASK_extern extern
#endif

#ifndef TRUE
  #define TRUE                    1
#endif

#ifndef FALSE
  #define FALSE                   0
#endif
    
#define  DisableInterrupt()            __disable_irq() 
#define  EnableInterrupt()             __enable_irq()

#define  uBYTE8         unsigned char
#define  uINT16         unsigned short int
#define  uINT32         unsigned int
#define  uINT64         unsigned long int
#define  sBYTE8         signed char
#define  sINT16         signed short int
#define  sINT32         signed int
#define  sINT64         signed long int
#define  FLOAT          float

/**************************************************************************************************/
/*                                           º¯ Êý Éù Ã÷                                          */
/**************************************************************************************************/
extern void AddMSGList(void (*pFunc)(uBYTE8, uBYTE8, uBYTE8), uBYTE8 MSGPara1, uBYTE8 MSGPara2, uBYTE8 MSGPara3);
extern void InitMSGList(void);

extern void InitThreadList(void);
extern void RunTask(void);
extern uBYTE8 AddThreadList(uBYTE8 Type, void (*pFunc)(void));
extern void OptSys(void);
