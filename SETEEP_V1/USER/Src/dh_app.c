/**********************************************
;**********************************************
;***      ����
;***      Author: ����
;***      Date:   2016-12-19
;**********************************************
;**********************************************/
#define DH_APP
#include "outComm.h"
#include "hw_uart.h"
#include "lwcComm.h"
#include "mbapp.h"
#include "logic.h"
#include "dh_app.h"

static uint8_t DHCommTime = 0;
static uint8_t MBS_RxTask(void);
static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx);
void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr);
uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr);

//uart4_rx_buf[]����256
#define  MB4_RX_FRAME_ADDR      (uart4_rx_buf[0]) //��ַ
#define  MB4_RX_FRAME_FC        (uart4_rx_buf[1])  //������
#define  MB4_RX_FRAME_DATA      (uart4_rx_buf[2])  //����


#define  MB4_TX_FRAME_ADDR      (uart4_tx_buf[0])  //���͵�ַ
#define  MB4_TX_FRAME_FC        (uart4_tx_buf[1])  //������
#define  MB4_TX_FRAME_DATA      (uart4_tx_buf[2])  //����

#define  MB4_RX_DATA_START     (((uint16_t)uart4_rx_buf[2] << 8) + (uint16_t)uart4_rx_buf[3])//�Ĵ�����ַ
#define  MB4_RX_DATA_REGS      (((uint16_t)uart4_rx_buf[4] << 8) + (uint16_t)uart4_rx_buf[5])//д������


/****************************************************************************
 * UART4�ӿڻص�����
 ****************************************************************************/
void uart4_rx_adata(uint16_t data, uint8_t abyte) {
  MB_Params[HFIM5_UART_4].RTU_TimeoutCtr = MB_Params[HFIM5_UART_4].RTU_TimeoutCnts;
  MB_Params[HFIM5_UART_4].RTU_TimeoutEn = TRUE;
//  gSysStatusPara.CommCheck  = 0;
}

void uart4_rx_frame(uint16_t count) {
  
  MB_Params[HFIM5_UART_4].RxBufByteCtr = count;
}

/*�жϵ��ú���*/
void uart4_tx_clpt(void) {
	hfim5_uart_set_tx_en(HFIM5_UART_4, FALSE);  /*���ܺ���*/
	hfim5_uart_start_receive(HFIM5_UART_4);
  MB_Params[HFIM5_UART_4].TxEnd = TRUE;
}


/*
���ش���У����
*/
static void MB_ErrResponse(MB_ParamsType *tx, uint8_t errCode) {
  MB4_TX_FRAME_ADDR   = MB4_RX_FRAME_ADDR;
  MB4_TX_FRAME_FC     = MB4_RX_FRAME_FC | 0x80;
  MB4_TX_FRAME_DATA   = errCode;
  tx->TxBufByteCtr = 3;                 //�������ݼ���
}


/*
@��������static uint8_t MB_Func03_Handle(MB_ParamsType *rx)
@���룺03 ����֡
@���ܣ������ּĴ���
*/

static uint8_t MB_Func03_Handle(MB_ParamsType *rx) {
	
  uint16_t reg;    //�Ĵ�����ַ
  uint16_t nbr_regs; //��Ҫ���ļĴ�������
  uint16_t val;    //�Ĵ���ֵ
  uint16_t err;    //���ϱ�־
  uint8_t nbr_bytes;
	
  uint8_t *presp;

	
  if (rx->RxBufByteCtr != 8)  //��ַ 1 + ������ 1 + �Ĵ�����ַ 2 + ���� 2 + У�� 2
    return FALSE;
  
  reg = MB4_RX_DATA_START;    //�Ĵ����׵�ַ
  nbr_regs = MB4_RX_DATA_REGS; //��Ҫ���ļĴ�������
  
  if ((nbr_regs == 0) || (nbr_regs > 125)) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);//���ظ�������Χ����
    return TRUE;
  }
  
  nbr_bytes = (uint8_t)(nbr_regs * sizeof(uint16_t));//��Ҫ���͵��ֽ���
  
  presp = &uart4_tx_buf[0];                        //ȡ�����׵�ַ
  
  *presp++ =  MB4_RX_FRAME_ADDR;                   //uart4_tx_buf[0] = MB4_RX_FRAME_ADDR; 
  *presp++ =  MB4_RX_FRAME_FC;                     //uart4_tx_buf[1] = MB4_RX_FRAME_FC; 
  *presp++ = (uint8_t)nbr_bytes;                   //uart4_tx_buf[2] = (uint8_t)nbr_bytes; 

  while(nbr_regs) {
	  
    val = DH_RdHoding(reg,&err);                   //��reg��ַ��ֵ��ֵ��val ͬʱ���ض�ȡ�����־λ
	  
    if (err == MB_ERR_NONE) {                      //��ȡ�ɹ����ֱ�val�ĸߵͰ�λ��ֵ������
      *presp++ = (uint8_t)((val >> 8) & 0x00FF);   //��һ�� uart4_tx_buf[3] = �߰�λ
      *presp++ = (uint8_t)(val & 0x00FF);          //��һ�� uart4_tx_buf[4] = �Ͱ�λ
    }
    else {
      MB_ErrResponse(rx, err);                     //������󣬷��ش�����
      return TRUE;
    }
    reg++;                                         //��ȡһ���Ĵ��������һ���Ĵ���
    nbr_regs--;                                    //ͬʱ��Ҫ��ȡ������Ҫ��һ
  }
  rx->TxBufByteCtr = nbr_bytes + 3;                //������ܹ����ֽ���Ϊ  ��ַ1 + ������1 + �ֽ���1
  return(TRUE);                                    //���ͳһ��CRCУ��  ��rx->TxBufByteCtr + 2
}
/*
@��������static uint8_t MB_Func06_Handle(MB_ParamsType *rx)
@���룺���յ�06����������֡
@���ܣ��򵥸��Ĵ���д��ֵ
*/

static uint8_t MB_Func06_Handle(MB_ParamsType *rx) {
  uint16_t reg;
  uint16_t val;
  uint16_t err;
	
  if (rx->RxBufByteCtr != 8)//��ַ1  ������1 �Ĵ�����ַ2  ����2  У��2  
    return FALSE;
  
  reg =  MB4_RX_DATA_START;  //����ĵ�ַ
  val =  MB4_RX_DATA_REGS;   //ֵ   �����ֽڽ��кϲ�
  
  DH_WrHoding(reg,val,&err); //д���Ӧ�Ĵ���
  
  
  if (err != MB_ERR_NONE) {
    MB_ErrResponse(rx, err); //�����Ƿ�����
    return TRUE;
  }
  
  MB4_TX_FRAME_ADDR = MB4_RX_FRAME_ADDR;  /* д����ȷ�󷵻سɹ���Ӧ�� */
  MB4_TX_FRAME_FC   = MB4_RX_FRAME_FC;    
  uart4_tx_buf[2] = uart4_rx_buf[2];       
  uart4_tx_buf[3] = uart4_rx_buf[3];
  uart4_tx_buf[4] = uart4_rx_buf[4];
  uart4_tx_buf[5] = uart4_rx_buf[5];
  rx->TxBufByteCtr = 6;
  return TRUE;
}


/*
@��������static uint8_t MB_Func16_Handle(MB_ParamsType *rx)
@���룺���ڽ�����������֡  д����Ĵ���
@���ܣ���Ӧ��Ӧ������
*/

static uint8_t MB_Func16_Handle(MB_ParamsType *rx) {
  uint16_t reg;           //�Ĵ�����ַ
  uint16_t nbr_regs;      //��Ҫд�ļĴ�������
  uint16_t nbr_bytes;     //��Ҫ���͵��ֽ���
  uint16_t val;           //�Ĵ���ֵ
  uint16_t err;           //����״̬
  uint8_t *prx;           //����ַ�Ⱥ󱣴�Ҫ���ռĴ�����ֵ

  reg = MB4_RX_DATA_START;  //��ȡ���������еļĴ����׵�ַ
  nbr_regs = MB4_RX_DATA_REGS;//��ȡ��Ҫ���͵ļĴ�������
  
  if (nbr_regs == 0 || nbr_regs > 125) {
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY); //�жϸ����Ƿ����     
    return (TRUE);
  }
  prx  = &uart4_rx_buf[6];    //��ȡд���ֽ����ĵ�ַ
  nbr_bytes = (uint16_t)*prx++; //��������������
  
  if ((rx->RxBufByteCtr - 9) != nbr_bytes) {  //��ַ1 + ����1 + �Ĵ�����ַ2 + �Ĵ������� 2 + �ֽ���1 + CRC 2 = 9
    MB_ErrResponse(rx, MB_ERR_ILLEGAL_DATA_QTY);//�Ĵ���ֵ����������Ӧ
    return (TRUE);
  }
  if (((nbr_bytes / nbr_regs) != (uint16_t)sizeof(uint16_t))) { //�ֽ���/�Ĵ���������ֵ���� 2
    MB_ErrResponse(rx,MB_ERR_ILLEGAL_DATA_VAL);                 //��������ֵ������Ӧ
    return (TRUE);
  }
	

  while(nbr_regs) {                                         //Ҫд��ļĴ���������Ϊ0
    val = ((uint16_t)*prx++) << 8;                          //ȡ�������ĸߵ�8λ����val
    val += (uint16_t)*prx++;
    DH_WrHoding(reg, val, &err);                            //��ȡ����ֵд��Ĵ��� 
    if (err == MB_ERR_NONE) {                               //������
      reg++;                                                //д��ɹ�������һ���Ĵ���
      nbr_regs--;                                           //��Ҫд�ļĴ�������һ��
    }
    else {
      MB_ErrResponse(rx, err);                              //д������򷵻ش������
      return (TRUE);
    }
  }
  
  rx->TxBufByteCtr = 6;                                     //ȫ����ȷ�򷵻���Ӧ����֡
  MB4_TX_FRAME_ADDR = MB4_RX_FRAME_ADDR;
  MB4_TX_FRAME_FC = MB4_RX_FRAME_FC;
  uart4_tx_buf[2] = uart4_rx_buf[2];
  uart4_tx_buf[3] = uart4_rx_buf[3];
  uart4_tx_buf[4] = uart4_rx_buf[4];
  uart4_tx_buf[5] = uart4_rx_buf[5];
  return (TRUE);
}


/*
@��������static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx)
@���룺
@���ܣ����������
*/
static uint8_t MB_FCxx_Handle(uint8_t addr, MB_ParamsType *rx) {
  uint8_t reply = FALSE;

  if ((MB4_RX_FRAME_ADDR == addr)  || (MB4_RX_FRAME_ADDR == 0)){
    switch(MB4_RX_FRAME_FC) {

    case MB_FC_03:
      reply = MB_Func03_Handle(rx);
      break;
    case MB_FC_06:
      reply = MB_Func06_Handle(rx);
      break;
    case MB_FC_16:
      reply = MB_Func16_Handle(rx);
      break;
    default:
      MB_ErrResponse(rx, MB_ERR_ILLEGAL_FC);
      reply = TRUE;
    }
  }

  if (MB4_RX_FRAME_ADDR == 0) {
    return FALSE;
  }
  else {
    return(reply);
  }
}

static uint8_t MBS_RxTask(void) {
  uint8_t ok;
  uint8_t reply = FALSE;
  if (MB_Params[HFIM5_UART_4].RxBufByteCtr >= MB_RTU_MIN_MSG_SIZE) {
  
		ok = MB_ChkFrame(HFIM5_UART_4 , &uart4_rx_buf[0]);
	
		if (ok == TRUE) {
      DHCommTime = 20;
			reply = MB_FCxx_Handle(MB_Params[HFIM5_UART_4].NodeAddr,&MB_Params[HFIM5_UART_4]);

			if (reply == TRUE) {
				MB_RTU_TX(uart4_tx_buf,&MB_Params[HFIM5_UART_4]);
			}
		}
	}
	return reply;
}

void Uart4_Tmr(void)
{
  if (MB_Params[HFIM5_UART_4].RTU_TimeoutEn == TRUE) {
      MB_Params[HFIM5_UART_4].RTU_TimeoutCtr--;
    if (MB_Params[HFIM5_UART_4].RTU_TimeoutCtr == 0) {
      MB_Params[HFIM5_UART_4].RTU_TimeoutEn = FALSE;
      MB_Params[HFIM5_UART_4].RecAFrameFlag = TRUE;
    }
  }
  if (MB_Params[HFIM5_UART_4].SleepAfterRxCtr) {
			MB_Params[HFIM5_UART_4].SleepAfterRxCtr--;
		}
}

void DH_Slave_Task(MB_ParamsType *up) {
	if (up->QueryStep == S_WAIT_REC) {
		if (up->RecAFrameFlag) {
      if(MBS_RxTask())
      {
        up->TxEnd = FALSE;
        up->TxBufPtr =  &uart4_tx_buf[0];
        up->SleepAfterRxCtr = up->SleepAfterRxCnt;
        up->QueryStep = S_DLY_RESP;
      }
      else
      {
        up->RxBufPtr = &uart4_rx_buf[0];
        up->RxBufByteCtr = 0;
      }

			up->RecAFrameFlag = FALSE;
		}
	}
	else if (up->QueryStep == S_DLY_RESP) {
		if (up->SleepAfterRxCtr == 0) {
			up->TxBufPtr = &uart4_tx_buf[0];
      hfim5_uart_set_tx_en(HFIM5_UART_4,TRUE);
      hfim5_uart_start_transmit(HFIM5_UART_4,up->TxBufByteCtr);
			up->QueryStep = S_WAIT_REC;
			up->RxBufPtr = &uart4_rx_buf[0];
			up->RxBufByteCtr = 0;
		}
	}
}


void MB4_Init(uint8_t addr,MB_ParamsType *param) {  
  param->NodeAddr = 1;                           //ͨ�ŵ�ַ��ֵ
  param->RxBufByteCtr = 0;                       //���ڽ��������ֽڼ���
  param->RTU_TimeoutCtr = 0;                     //
  param->RTU_TimeoutCnts = 2;                    //
  param->FirstRxTimeOutCnt = 5;                  //
  param->RxBufPtr = &uart4_rx_buf[0];             //���ݽ��ջ����׵�ַ
  param->TxCtr = param->TxBufByteCtr = 0;         //���������ֽڼ���
  param->TxBufPtr = &uart4_tx_buf[0];             //���ݷ��ͻ����׵�ַ 
  param->SleepAfterRxCnt = 2;                     //
}



/*
@��������uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr)
@���룺��Ҫ��ȡ�ļĴ�����ַ��״̬��־
@���ܣ���������Ĵ�����ַ��ֵ��ͬʱ���ض�ȡ״̬
*/

uint16_t DH_RdHoding(uint16_t reg, uint16_t *perr)
{
  *perr = MB_ERR_NONE;  //���Ƚ�״̬����Ϊ����
	
  if(reg >= 30001 && reg <= 30005)  //�жϼĴ�������
  {
    switch(reg)                     //�жϵ�ַ
    {
      case 30001: return gTmpUserPara.SetMode;  //��ģʽ
      case 30002: return POWER_SWITCH;     //�����ػ�
      case 30003: return gTmpUserPara.SetCoolTemp;//�����趨
      case 30004: return gTmpUserPara.SetHeatTemp;//�����趨
      case 30005: return EN_LOCAL;   //���ز���
    }  //��default

  }
  else if(reg >= 50001 && reg <= 50013)
  {
    switch(reg)
    {
      case 50001: return POWER_SWITCH;   //Զ�̿��ػ�
      case 50002: return gTmpUserPara.SetMode;  //����ģʽ
      case 50003: return gSysStatusPara.OutParaBuf.ActHZ; //ѹ��������Ƶ��
      case 50004: return gSysStatusPara.OutParaBuf.EEVStep; //�������ͷ�����
      case 50005:                                           //�澯����
        return ((gSysStatusPara.INParaBit.ErrInfo.uiBuf & 0x37F)  \
        | gSysStatusPara.OutParaBit.ErrInfo[0].uiBuf              \
        | gSysStatusPara.OutParaBit.ErrInfo[1].uiBuf              \
        | gSysStatusPara.OutParaBit.ErrInfo[2].uiBuf) ? 1 : 0;
      case 50006: return gSysStatusPara.INParaBuf.ReturnTemp;  //��ˮ�¶�
       
      case 50007: return gSysStatusPara.INParaBuf.InTemp;   //��ˮ�¶�
      case 50008: return gSysStatusPara.OutParaBuf.LowPressNum; //����ѹ��
      case 50009: return gSysStatusPara.OutParaBuf.OutTemp;     //�����¶�
      case 50010: return gSysStatusPara.OutParaBuf.ExhaultTemp; //�����¶�
      case 50011: return gSysStatusPara.OutParaBuf.InhaleTemp;  //�����¶�
      case 50012: return gSysStatusPara.OutParaBuf.DefrostTemp;  //�����¶�
      case 50013: return gSysStatusPara.INParaBuf.FreonInTemp;     //��ý���¶�   
        
    }
  }else if(reg >= 60001 && reg <= 60026)
  {
    switch(reg)
	{
	  case 60001: return gSysStatusPara.OutParaBuf.CondInTemp;     //�ͱ����¶�
	  case 60002: return gSysStatusPara.OutParaBuf.IPMTemp;        // IPM�¶�                             
	  case 60003: return gSysStatusPara.OutParaBuf.PFCTemp;        // PFC�¶�
	  case 60004: return gSysStatusPara.OutParaBuf.ByPassOutTemp;  // ��·�����¶�
	  case 60005: return gSysStatusPara.OutParaBuf.ByPassInTemp;   // ��·�����¶�
	  case 60006: return gSysStatusPara.OutParaBuf.ByPassEEVStep;  // �����������ͷ�
	  case 60007: return gSysStatusPara.OutParaBuf.DriverType;      // ��������
	  case 60008: return gSysStatusPara.OutParaBuf.MachType;        // �ͺ�
	  case 60009: return gSysStatusPara.OutParaBuf.OBJHZ;           //  Ŀ��Ƶ��               
	  case 60010: return gSysStatusPara.OutParaBuf.CalCapa;         //  ��������
	  case 60011: return gSysStatusPara.OutParaBuf.ExhSuperHeat;     //  �������ȶ�
	  case 60012: return gSysStatusPara.OutParaBuf.InhalSuperHeat;   //  �������ȶ�
	  case 60013: return gSysStatusPara.OutParaBuf.ComI;             //  ѹ��������
	  case 60014: return gSysStatusPara.OutParaBuf.ACI;              //  AC����
	  case 60015: return gSysStatusPara.OutParaBuf.ExhProtectLevel;   //  ���������ȼ�
	  case 60016: return gSysStatusPara.OutParaBuf.ComProtectLevel;   //  ѹ���������ȼ�
	  case 60017: return gSysStatusPara.OutParaBuf.IPMProtectLevel;   //  IPM�¶ȱ����ȼ�
      case 60018: return gSysStatusPara.OutParaBuf.PFCProtectLevel;   //  PFC�¶ȱ����ȼ�  
	  case 60019: return gSysStatusPara.OutParaBuf.LowFarFreezeLV;    //  ��ѹ���������ȼ�
	  case 60020: return gSysStatusPara.OutParaBuf.ACIProtectLevel;    //  AC���������ȼ�
	  case 60021: return gSysStatusPara.OutParaBuf.DISta[0].uiBuf;     //  DI״̬
	  case 60022: return gSysStatusPara.OutParaBuf.DISta[1].uiBuf;       //  DI״̬
	  case 60023: return gSysStatusPara.MachSta.ucBuf;               //  ϵͳ����
	  case 60024: return gTmpUserPara.SetCoolTemp;                   //  �����趨�¶�
	  case 60025: return gTmpUserPara.SetHeatTemp;                   //  �����趨�¶�
	  case 60026: if(( uint16_t)gSysStatusPara.INParaBit.ErrInfo.Field.bit07)
	  {
	    return  (uint16_t)1;		     //  �ж��������ȼ�
	  }else
	  {
	    return ( uint16_t)0;	         //  �޶��������ȼ�
	  }
	}
  
  
  }
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;          //��ַ���ڷ�Χ���ص�ַ����
  }
  return 0;
}



static uint8_t Delay = 0;


/*
@��������void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr)
@���룺�Ĵ�����ַ��д��ֵ�����ش���״ָ̬��
@���ܣ���ָ���Ĵ���д��ָ��ֵ
*/

void DH_WrHoding(uint16_t reg, uint16_t reg_val, uint16_t *perr)
{
 if(reg >= 30001 && reg <= 30005)
  {
    Delay = 2;
    switch(reg)
    {
      case 30001:  gTmpUserPara.SetMode = reg_val; break;       //ģʽ
      case 30002:  POWER_SWITCH = reg_val;          break;      //���ػ�
      case 30003:  gTmpUserPara.SetCoolTemp = reg_val;  break;  //�����趨�¶�
      case 30004:  gTmpUserPara.SetHeatTemp = reg_val;  break;  //�����趨�¶�
      case 30005:  EN_LOCAL = reg_val;                break;    //���ز���
    }
    *perr = MB_ERR_NONE;
    return;
  }
  else {
    *perr = MB_ERR_ILLEGAL_DATA_ADDR;//���ص�ַ�����־λ
  }

  

}



void F_CheckDHCommSta(void)
{
  DH_COMM_STA = 1;
  if(Delay)
  {
    Delay --;
    if(0 == Delay)
    {
      LWC_INIT_STA = 1;
      gSysStatusPara.EEDly  = 2;
    }
  }
  if(DHCommTime)
  {
    DHCommTime --;
  }
  else
  {
    DH_COMM_STA = 0;
  }
}









