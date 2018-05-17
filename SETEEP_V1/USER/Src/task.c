

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
}MSGType;//���и�ʽ



typedef struct {
    uBYTE8 TaskCount;
    uBYTE8 RunTaskNo;
    void (*pFunName[cTaskCount])(void);
}Threadype;

typedef struct {
    uBYTE8     ThreadNo;
    Threadype  ThreadList[cThreadCount];
}ProcessType;   //�߳̽ṹ��

//static  MSGType MSGList;    //����һ�����У�����20

static  ProcessType gsProcess;//�����߳��б�  10ms,100ms,1s

static  uBYTE8 MSec_100Ctr;
static  uBYTE8 SecCtr;

static void CloseDevice(void);
static void ResetTask(uBYTE8 ThreadNo);

void CloseDevice(void)
{

}




void RunTask(void) /*ʵʱ*/
{
  uBYTE8 lu8ThreadNo, lu8RunTaskNo;
  Threadype *pThread;
  
 													
 
    lu8ThreadNo  = gsProcess.ThreadNo;  //��ǰ���̳̺�
    pThread      = (Threadype *)(&gsProcess.ThreadList[lu8ThreadNo]);//��ǰ����ָ��
    lu8RunTaskNo = pThread->RunTaskNo;//��ǰ�������еĵ�ǰ�߳�

    if (pThread->pFunName[lu8RunTaskNo] == 0)//�������
    {
        gsProcess.ThreadNo++;                //���̼�1
        if (gsProcess.ThreadNo >= cThreadCount)//�����Ƿ����
        {
            gsProcess.ThreadNo = 0;            //�������,��λ����
        }
        return;
    }

    if (pThread->RunTaskNo < pThread->TaskCount)//�Ƿ�����Ҫ���е��߳�
    {
        pThread->pFunName[lu8RunTaskNo]();      //�����߳�
    
        pThread->RunTaskNo++;                   //�̼߳�1
        if (pThread->RunTaskNo >= pThread->TaskCount)//��һʱ��Ƭ��,�ý����Ƿ����߳�����
        {
            gsProcess.ThreadNo++;                    //û��,���̼�1
			
            if (gsProcess.ThreadNo >= cThreadCount)  //�����Ƿ����
            {
                gsProcess.ThreadNo = 0;              //�������,��λ����
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
*  @������    ��  void InitThreadList(void)                                 
*  @��������  ��  ��ʼ���߳��б�
*  @�������  ��  ��                     
*  @�������  ��  ��                                        
*******************************************************************/
void InitThreadList(void)  //��ʼ���߳��б�
{
    uBYTE8 i = 0, j = 0;
    
    for(i = 0; i < cThreadCount; i++)//cThreadCount���̼�����
    {
        for (j = 0; j < cTaskCount; j++)//cTaskCount�̼߳�����
        {
          gsProcess.ThreadList[i].pFunName[j] = 0;//�߳��б�����
        }
        gsProcess.ThreadList[i].TaskCount       = 0;//�̼߳�������
        gsProcess.ThreadList[i].RunTaskNo       = 0;//��ǰ���е��߳�����
    }
    gsProcess.ThreadNo = 0;//��ǰ���е��̸߳�λ
}


/******************************************************************
*  @������    ��  uBYTE8 AddThreadList                                    
*  @��������  ��  ���ݽ����������߳�
*  @�������  ��  ���������̺߳���                     
*  @�������  ��  true/false                                       
*******************************************************************/
uBYTE8 AddThreadList(uBYTE8 Type, void (*pFunc)(void))   /*ִ�����ڣ�*/
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
@��������void OptSys(void)
@���룺��
@���ܣ���ʱ��λ����
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
