#include "uart0.h"
#include "hal_uart.h"
#include "string.h"
#include "user_printf.h"
#include "OSAL.h"
#include "comdef.h"
#include "ZComDef.h"
#include "OnBoard.h"

#define NO_TASK_ID 0xFF

void Uart0_Handle( void );
void Uart0_Reset( void );

uint8 UART0_RX_BUFF[UART0_RX_BUFF_MAX];//���ջ�����
uint8 UART0_RX_STA = 0;                //����״̬���
uint8 UART0_RX_LEN = 0;                //�������ݳ���

static uint8 registeredUart0TaskID = NO_TASK_ID;

uint8 RegisterForUart0( uint8 task_id )
{
  if ( registeredUart0TaskID == NO_TASK_ID )
  {
    registeredUart0TaskID = task_id;
    return ( true );
  }
  else
    return ( false );
}

void Uart0_Init(uint8 baudRate)
{
  halUARTCfg_t uartConfig;

  uartConfig.configured           = TRUE;
  uartConfig.baudRate             = baudRate;
  uartConfig.flowControl          = FALSE;
  uartConfig.flowControlThreshold = UART0_THRESHOLD;
  uartConfig.rx.maxBufSize        = UART0_RX_BUFF_MAX;
  uartConfig.tx.maxBufSize        = UART0_TX_BUFF_MAX;
  uartConfig.idleTimeout          = UART0_IDLE_TIMEOUT;
  uartConfig.intEnable            = TRUE;
  uartConfig.callBackFunc         = Uart0_Process;

  HalUARTOpen (HAL_UART_PORT_0, &uartConfig);
}

void Uart0_Process( uint8 port, uint8 event )
{
  UART0_RX_STA = 0;
  UART0_RX_LEN = 0;
  uint8 Res;
  (void)event;  // Intentionally unreferenced parameter

  while (Hal_UART_RxBufLen(port))
  {
    HalUARTRead (port, &Res, 1);//��ȡһ���ֽ�
    UART0_RX_BUFF[UART0_RX_STA&0x3F]=Res ;
    UART0_RX_STA++;

    if((UART0_RX_STA&0x80)==0)//����δ���
    {
      if(UART0_RX_STA&0x40)//ǰһ�����յ������ݽ������ġ�ǰһ�롱
      {
        if     (Res==0x0A)
        {
          UART0_RX_STA |= 0x80;//���������
        }
        else if(Res!=0x0D)
        {
          UART0_RX_STA &= 0xBF;//���ǽ���������ǰ�����������־��0
        }
      }
      else
      {
        if(Res==0x0D)
        {
          UART0_RX_STA |= 0x40;
        }
      }
    }

    if((UART0_RX_STA&0x80)==0x80)
    {
      UART0_RX_LEN = (UART0_RX_STA&0x3F)-2;
      Uart0_Handle();
      Uart0_Reset();
      break;
    }
  }
}

void Uart0_Handle( void )
{
//  if(strstr((const char*)UART0_RX_BUFF,"SW1_TOGGLE"))
//  {
//    printf("SW1_TOGGLE\n");
//    UART0_t *msgPtr;
//    msgPtr = (UART0_t *)osal_msg_allocate( sizeof(UART0_t) );
//    if ( msgPtr )
//    {
//      msgPtr->hdr.event = UART0_MESSAGE_SW1_TOGGLE;
//
//      osal_msg_send( registeredUart0TaskID, (uint8 *)msgPtr );
//    }
//  }
//  else if(strstr((const char*)UART0_RX_BUFF,"SW2_TOGGLE"))
//  {
//    printf("SW2_TOGGLE\n");
//    UART0_t *msgPtr;
//    msgPtr = (UART0_t *)osal_msg_allocate( sizeof(UART0_t) );
//    if ( msgPtr )
//    {
//      msgPtr->hdr.event = UART0_MESSAGE_SW2_TOGGLE;
//
//      osal_msg_send( registeredUart0TaskID, (uint8 *)msgPtr );
//    }
//  }
}

void Uart0_Reset( void )
{
  memset(UART0_RX_BUFF,0,UART0_RX_BUFF_MAX);
  UART0_RX_STA = 0;
}