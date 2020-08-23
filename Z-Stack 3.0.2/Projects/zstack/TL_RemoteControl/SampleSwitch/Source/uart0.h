#ifndef UART0_H
#define UART0_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "hal_types.h"

#define UART0_RX_BUFF_MAX    50
#define UART0_TX_BUFF_MAX    50
#define UART0_THRESHOLD      (UART0_RX_BUFF_MAX / 2)
#define UART0_IDLE_TIMEOUT   6

extern uint8 UART0_RX_BUFF[UART0_RX_BUFF_MAX];//���ջ�����
extern uint8 UART0_RX_STA;                //����״̬���
extern uint8 UART0_RX_LEN;                //�������ݳ���

uint8 RegisterForUart0( uint8 task_id );
void Uart0_Init( uint8 baudRate );
void Uart0_Process( uint8 port, uint8 event );
void Uart0_Handle( void );
void Uart0_Reset( void );

#ifdef __cplusplus
}
#endif
#endif /* UART0_H */
