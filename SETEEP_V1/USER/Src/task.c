

#define  TASK_H
#include "task.h"

typedef struct
{
  uBYTE8 MSGHead;                         /* Queue head */
  uBYTE8 MSGTail;                         /* Queue tail */
  struct{
      void  (*pFunName)(uBYTE8, uBYTE8, uBYTE8);
      uBYTE8 Para1;
      uBYTE8 Para2;
      uBYTE8 Para3;
  }MSG[cMSGLength];//cMSGLength = 20;
}MSGType;//队列格式



typedef struct {
    uBYTE8 TaskCount;
    uBYTE8 RunTaskNo;
    void (*pFunName[cTaskCount])(void);
}Threadype;

typedef struct {
    uBYTE8     ThreadNo;
    Threadype  ThreadList[cThreadCount];
}ProcessType;   //线程结构体

//static  MSGType MSGList;    //声明一个队列，长度20

static  ProcessType gsProcess;//三个线程列表  10ms,100ms,1s

static  uBYTE8 MSec_100Ctr;
static  uBYTE8 SecCtr;

static void CloseDevice(void);
static void ResetTask(uBYTE8 ThreadNo);

void CloseDevice(void)
{

}




void RunTask(void) /*实时*/
{
  uBYTE8 lu8ThreadNo, lu8RunTaskNo;
  Threadype *pThread;
  
 													
 
    lu8ThreadNo  = gsProcess.ThreadNo;  //当前进程程号
    pThread      = (Threadype *)(&gsProcess.ThreadList[lu8ThreadNo]);//当前进程指针
    lu8RunTaskNo = pThread->RunTaskNo;//当前进程运行的当前线程

    if (pThread->pFunName[lu8RunTaskNo] == 0)//进程完成
    {
        gsProcess.ThreadNo++;                //进程加1
        if (gsProcess.ThreadNo >= cThreadCount)//进程是否完成
        {
            gsProcess.ThreadNo = 0;            //进程完成,复位进程
        }
        return;
    }

    if (pThread->RunTaskNo < pThread->TaskCount)//是否有需要运行的线程
    {
        pThread->pFunName[lu8RunTaskNo]();      //运行线程
    
        pThread->RunTaskNo++;                   //线程加1
        if (pThread->RunTaskNo >= pThread->TaskCount)//下一时间片到,该进程是否有线程运行
        {
            gsProcess.ThreadNo++;                    //没有,进程加1
			
            if (gsProcess.ThreadNo >= cThreadCount)  //进程是否完成
            {
                gsProcess.ThreadNo = 0;              //进程完成,复位进程
            }
        }
    }
  
}

void ResetTask(uBYTE8 lu8ThreadNo)
{
    Threadype *pThread;

    pThread      = (Threadype *)(&gsProcess.ThreadList[lu8ThreadNo]);
    
    if (pThread->pFunName[0] == 0)
    {
        return;
    }
    
    if (pThread->RunTaskNo >= pThread->TaskCount)
    {
        pThread->RunTaskNo = 0;
    }
}
/******************************************************************
*  @函数名    ：  void InitThreadList(void)                                 
*  @功能描述  ：  初始化线程列表
*  @输入参数  ：  无                     
*  @输出参数  ：  无                                        
*******************************************************************/
void InitThreadList(void)  //初始化线程列表
{
    uBYTE8 i = 0, j = 0;
    
    for(i = 0; i < cThreadCount; i++)//cThreadCount进程计数器
    {
        for (j = 0; j < cTaskCount; j++)//cTaskCount线程计数器
        {
          gsProcess.ThreadList[i].pFunName[j] = 0;//线程列表清零
        }
        gsProcess.ThreadList[i].TaskCount       = 0;//线程计数清零
        gsProcess.ThreadList[i].RunTaskNo       = 0;//当前运行的线程清零
    }
    gsProcess.ThreadNo = 0;//当前运行的线程复位
}


/******************************************************************
*  @函数名    ：  uBYTE8 AddThreadList                                    
*  @功能描述  ：  根据进程名增加线程
*  @输入参数  ：  进程名和线程函数                     
*  @输出参数  ：  true/false                                       
*******************************************************************/
uBYTE8 AddThreadList(uBYTE8 Type, void (*pFunc)(void))   /*执行周期？*/
{
    if (Type >= cThreadCount)
    {
        return FALSE;
    }
    
    if (gsProcess.ThreadList[Type].TaskCount == cTaskCount)  /*20*/
    {
        return FALSE;
    }
    else
    {
        gsProcess.ThreadList[Type].pFunName[gsProcess.ThreadList[Type].TaskCount] = pFunc;
        gsProcess.ThreadList[Type].TaskCount++;
        gsProcess.ThreadList[Type].RunTaskNo++;
        return TRUE;
    }
}


/*
@函数名：void OptSys(void)
@输入：无
@功能：分时复位任务
*/
void OptSys(void)  /*10MS*/
{  
  ResetTask(TMR_TYPE_10MS);
	
  if (++MSec_100Ctr == 10)
  {
      MSec_100Ctr = 0;
      ResetTask(TMR_TYPE_100MS);
  }
  if (++SecCtr == 100)
  {
      SecCtr = 0;
       
      ResetTask(TMR_TYPE_1SEC);
  }
}
