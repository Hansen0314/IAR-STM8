#include "stm8s.h"
#include "stdio.h"
#include "user_uart.h"
u8 IT_Receive_A,IT_Receive_A_num;
struct Peripheral Peripheral_Realy;
void uart2Init()
{
    UART2_DeInit();
    UART2_Init((uint32_t)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
    UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE);
    UART2_Cmd(ENABLE);
    
}
void uart2SendByte(uint8_t data)
{
    UART2->DR=data;
   /* Loop until the end of transmission */
   while (!(UART2->SR & UART2_FLAG_TXE));
}
void uart2SendString(uint8_t* Data,uint16_t len)
{
  uint16_t i=0;
  for(;i<len;i++)
    uart2SendByte(Data[i]);
}
uint8_t uart2ReceiveByte(void)
{
     uint8_t USART2_RX_BUF; 
    // while (!(UART2->SR & UART2_FLAG_RXNE));
     USART2_RX_BUF=(uint8_t)UART2->DR;
     return  USART2_RX_BUF;
}
#if 0
int fputc(int ch, FILE *f)
{  
  UART2->DR=(unsigned char)ch;
  while (!(UART2->SR & UART2_FLAG_TXE));
  return (ch);
}
#endif
void Uart_IT_Receive_Hnadle(u8* str)
{
   if(str[5] == A11_VALUE)
   {
     Peripheral_Realy.Fr = str[1];
     Peripheral_Realy.Dp = str[2];
     Peripheral_Realy.Door_Do = str[3];
     Peripheral_Realy.Door_Up = str[4];
     Peripheral_Realy.a11 = ((u16)str[6]<<8)|str[7];
     Peripheral_Realy.a13 = ((u16)str[9]<<8)|str[10];
     Peripheral_Realy.a12 = ((u16)str[12]<<8)|str[13];
   }
}
void Uart_Transmit_Hnadle(struct KEYHANDLE KeyHandle)
{
  uint8_t str_KeyHandle[7] = {0};
  
  str_KeyHandle[0] = 0Xfd;
  
  if(KeyHandle.Oper_Mode_Dis_State == 0)
  {

    str_KeyHandle[1] = KeyHandle.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Fan_State;

    str_KeyHandle[4] = KeyHandle.Door_State;  // 2 UP 1 DO
    
    str_KeyHandle[5] = KeyHandle.Er;
  }
  else if(KeyHandle.Oper_Mode_Dis_State ==1)
  {

    str_KeyHandle[1] = KeyHandle.Od_State.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Od_State.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Od_State.Fan_State;

    str_KeyHandle[4] = KeyHandle.Od_State.Door_State;  // 2 UP 1 DO  
    
    str_KeyHandle[5] = KeyHandle.Od_State.Er;
  
  }
  else if(KeyHandle.Oper_Mode_Dis_State == 2)
  {
    str_KeyHandle[1] = KeyHandle.Pm_State.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Pm_State.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Pm_State.Fan_State;

    str_KeyHandle[4] = KeyHandle.Pm_State.Door_State;  // 2 UP 1 DO   
    
    str_KeyHandle[5] = KeyHandle.Od_State.Er;
    
  }
  str_KeyHandle[6] = 0xff;

  uart2SendString(str_KeyHandle,7);
}