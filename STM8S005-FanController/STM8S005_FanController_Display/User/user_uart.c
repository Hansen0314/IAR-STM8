#include "stm8s.h"
#include "stdio.h"
#include "user_uart.h"
u8 IT_Receive_A,IT_Receive_A_num;
struct Peripheral Peripheral_Realy;
/*1λ��ʼλ 8λ����λ ����λ��CR3���� ��ʹ����żУ�� ��ʹ����żУ���ж�*/
/*ʹ�ܷ��ͺͽ��� �����ж�ʹ�� ��ֹ�����ж�*/
/*����1λֹͣλ ��ʹ��SCLK��������115200*/  
void uart2Init()
{
    UART2_DeInit();
    UART2_Init((uint32_t)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
    UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE);
    UART2_Cmd(ENABLE);
    
}
//���ڷ���һ���ֽ�
void uart2SendByte(uint8_t data)
{
    UART2->DR=data;
   /* Loop until the end of transmission */
   while (!(UART2->SR & UART2_FLAG_TXE));
}
//���ڸ����ַ���
void uart2SendString(uint8_t* Data,uint16_t len)
{
  uint16_t i=0;
  for(;i<len;i++)
    uart2SendByte(Data[i]);
}
//���ڽ���һ���ֽ�
uint8_t uart2ReceiveByte(void)
{
     uint8_t USART2_RX_BUF; 
     while (!(UART2->SR & UART2_FLAG_RXNE));
     USART2_RX_BUF=(uint8_t)UART2->DR;
     return  USART2_RX_BUF;
}
/*��Printf���ݷ�������*/ 
#if 0
int fputc(int ch, FILE *f)
{  
  UART2->DR=(unsigned char)ch;
  while (!(UART2->SR & UART2_FLAG_TXE));
  return (ch);
}
#endif
void Uart_IT_Receive_Hnadle(u8 data)
{
    switch(IT_Receive_A)
    {
      case 1: 
        Peripheral_Realy.a11 |= data<<4;
        IT_Receive_A_num++;
        if(IT_Receive_A_num>1)
        {
          Peripheral_Realy.a11 |= data;
          IT_Receive_A_num = 0;
          IT_Receive_A =0;
        }
      break;
      case 2: 
        Peripheral_Realy.a12 |= data<<4;
        IT_Receive_A_num++;
        if(IT_Receive_A_num>1)
        {
          Peripheral_Realy.a12 |= data;
          IT_Receive_A_num = 0;
          IT_Receive_A =0;
        }
      break;
      case 3: 
          Peripheral_Realy.a13 |= data<<4;
          IT_Receive_A_num++;
          if(IT_Receive_A_num>1)
          {
            Peripheral_Realy.a13 |= data;
            IT_Receive_A_num = 0;
            IT_Receive_A =0;
          }
      break;
      default :
      break;    
    }
    
   switch(data)
   {
      case FR_HIGH:Peripheral_Realy.Fr = 1;
      break;	
      case FR_LOW: Peripheral_Realy.Fr = 0;
      break;
      case DP_HIGH: Peripheral_Realy.Dp = 1;
      break;
      case DP_LOW: Peripheral_Realy.Dp = 0;
      break;		
      case A11_VALUE: IT_Receive_A = 1;
      break;
      case A12_VALUE: IT_Receive_A = 2;
      break;
      case A13_VALUE: IT_Receive_A = 3;
      break;
      default :
      break;
  }

   
}