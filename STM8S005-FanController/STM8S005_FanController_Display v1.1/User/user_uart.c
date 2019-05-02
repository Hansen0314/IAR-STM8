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
  uint8_t str_KeyHandle[6] = {0};
  
  str_KeyHandle[0] = 0Xfd;
  
  if(KeyHandle.Oper_Mode_Dis_State == 0)
  {

    str_KeyHandle[1] = KeyHandle.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Fan_State;

    str_KeyHandle[4] = KeyHandle.Door_State;  // 2 UP 1 DO
  }
  else if(KeyHandle.Oper_Mode_Dis_State ==1)
  {

    str_KeyHandle[1] = KeyHandle.Od_State.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Od_State.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Od_State.Fan_State;

    str_KeyHandle[4] = KeyHandle.Od_State.Door_State;  // 2 UP 1 DO  
  
  }
  else if(KeyHandle.Oper_Mode_Dis_State == 2)
  {
    str_KeyHandle[1] = KeyHandle.Pm_State.Led_P1_State;

    str_KeyHandle[2] = KeyHandle.Pm_State.Led_P2_State;

    str_KeyHandle[3] = KeyHandle.Pm_State.Fan_State;

    str_KeyHandle[4] = KeyHandle.Pm_State.Door_State;  // 2 UP 1 DO   
  }
  str_KeyHandle[5] = 0xff;
  
  /*
  else if(KeyHandle.Oper_Mode_Dis_State == 1)
  {
    if(KeyHandle.Od_State.Led_P1_State == 1)
    {
      Transmit_Data = LED_P1_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Od_State.Led_P1_State == 0)
    {
      Transmit_Data = LED_P1_OFF;
      uart2SendByte(Transmit_Data);
    }    
    if(KeyHandle.Od_State.Led_P2_State == 1)
    {
      Transmit_Data = LED_P2_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Od_State.Led_P2_State == 0)
    {
      Transmit_Data = LED_P2_OFF;
      uart2SendByte(Transmit_Data);
    }    
    if(KeyHandle.Od_State.Fan_State == 0)
    {
      Transmit_Data = FAN_OFF;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Od_State.Fan_State == 1)
    {
      Transmit_Data = FAN_ON_L;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Od_State.Fan_State == 2)
    {
      Transmit_Data = FAN_ON_M;
      uart2SendByte(Transmit_Data);
    }     
    else if (KeyHandle.Od_State.Fan_State == 3)
    {
      Transmit_Data = FAN_ON_B;
      uart2SendByte(Transmit_Data);
    } 
    if(KeyHandle.Od_State.Door_State == 2)
    {
      Transmit_Data = DOOR_UP_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Od_State.Door_State == 1)
    {
      Transmit_Data = DOOR_DO_ON;
      uart2SendByte(Transmit_Data);
    }
    //¹ÊÕÏ±¨¾¯¿ª¹Ø
  }
  else if(KeyHandle.Oper_Mode_Dis_State == 2)
  {
    if(KeyHandle.Pm_State.Led_P1_State == 1)
    {
      Transmit_Data = LED_P1_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Pm_State.Led_P1_State == 0)
    {
      Transmit_Data = LED_P1_OFF;
      uart2SendByte(Transmit_Data);
    }    
    if(KeyHandle.Pm_State.Led_P2_State == 1)
    {
      Transmit_Data = LED_P2_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Pm_State.Led_P2_State == 0)
    {
      Transmit_Data = LED_P2_OFF;
      uart2SendByte(Transmit_Data);
    }    
    if(KeyHandle.Pm_State.Fan_State == 0)
    {
      Transmit_Data = FAN_OFF;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Pm_State.Fan_State == 1)
    {
      Transmit_Data = FAN_ON_L;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Pm_State.Fan_State == 2)
    {
      Transmit_Data = FAN_ON_M;
      uart2SendByte(Transmit_Data);
    }     
    else if (KeyHandle.Pm_State.Fan_State == 3)
    {
      Transmit_Data = FAN_ON_B;
      uart2SendByte(Transmit_Data);
    } 
    if(KeyHandle.Pm_State.Door_State == 2)
    {
      Transmit_Data = DOOR_UP_ON;
      uart2SendByte(Transmit_Data);
    }
    else if(KeyHandle.Pm_State.Door_State == 1)
    {
      Transmit_Data = DOOR_DO_ON;
      uart2SendByte(Transmit_Data);
    }  
  }
  */
  uart2SendString(str_KeyHandle,6);
}