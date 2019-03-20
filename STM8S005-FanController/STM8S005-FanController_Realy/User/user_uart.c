#include "user_uart.h"
#include "stdio.h"
#include "Gpio.h"
/*1位起始位 8位数据位 结束位由CR3设置 不使用奇偶校验 不使能奇偶校验中断*/
/*使能发送和接收 接收中断使能 禁止发送中断*/
/*设置1位停止位 不使能SCLK，波特率115200*/  
void uart2Init()
{
    UART2_DeInit();
    UART2_Init((uint32_t)115200, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, \
    UART2_PARITY_NO , UART2_SYNCMODE_CLOCK_DISABLE , UART2_MODE_TXRX_ENABLE);
    UART2_ITConfig(UART2_IT_RXNE_OR,ENABLE  );
    UART2_Cmd(ENABLE );
}
//串口发送一个字节
void uart2SendByte(uint8_t data)
{
    UART2->DR=data;
   /* Loop until the end of transmission */
   while (!(UART2->SR & UART2_FLAG_TXE));
}
//串口改善字符串
void uart2SendString(uint8_t* Data,uint16_t len)
{
  uint16_t i=0;
  for(;i<len;i++)
    uart2SendByte(Data[i]);
}
//串口接收一个字节
uint8_t uart2ReceiveByte(void)
{
     uint8_t USART2_RX_BUF; 
     while (!(UART2->SR & UART2_FLAG_RXNE));
     USART2_RX_BUF=(uint8_t)UART2->DR;
     return  USART2_RX_BUF;
}
/*将Printf内容发往串口*/ 
/*
int fputc(int ch, FILE *f)
{  
  UART2->DR=(unsigned char)ch;
  while (!(UART2->SR & UART2_FLAG_TXE));
  return (ch);
}
*/
void Uart_IT_Receive_Control(u8 data)
{
  switch(data)
  {
    case FAN_OFF:  GPIO_WriteHigh(FAN_PORT,FAN_L_PIN);
                   GPIO_WriteHigh(FAN_PORT,FAN_M_PIN);
                   GPIO_WriteHigh(FAN_PORT,FAN_B_PIN);
    break;	
    case FAN_ON_L: GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
    break;
    case FAN_ON_M: GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
    break;
    case FAN_ON_B: GPIO_WriteLow(FAN_PORT,FAN_B_PIN);
    break;		
    case LED_P1_ON: GPIO_WriteLow(LED_PORT,LED_P1_PIN);
    break;
    case LED_P1_OFF: GPIO_WriteHigh(LED_PORT,LED_P1_PIN);
    break;
    case LED_P2_ON: GPIO_WriteLow(LED_PORT,LED_P2_PIN);
    break;
    case LED_P2_OFF: GPIO_WriteHigh(LED_PORT,LED_P2_PIN);
    break;
    case DOOR_UP_ON:  GPIO_WriteLow(DOOR_UP_PORT,DOOR_UP_PIN);
    break;		
    case DOOR_DO_ON:  GPIO_WriteLow(DOOR_DO_PORT,DOOR_DO_PIN);
    break;
    case DOOR_OFF:  GPIO_WriteHigh(DOOR_DO_PORT,DOOR_DO_PIN);
                    GPIO_WriteHigh(DOOR_DO_PORT,DOOR_DO_PIN); 
    break;			
    case ER_ON: GPIO_WriteLow(ER_PORT,ER_PIN);
    break;
    case ER_OFF: GPIO_WriteHigh(ER_PORT,ER_PIN);
    break;    
  }
}
void Uart_Send_data(struct Peripheral data)
{
    if(data.Fr) uart2SendByte(FR_HIGH);
    else uart2SendByte(FR_LOW);
    if(data.Dp) uart2SendByte(DP_HIGH);
    else uart2SendByte(DP_LOW);
    
    uart2SendByte(A11_VALUE);
    uart2SendByte((u8)(data.a11/256));
    uart2SendByte((u8)(data.a11%256));
    uart2SendByte(A12_VALUE);
    uart2SendByte((u8)(data.a12/256));
    uart2SendByte((u8)(data.a12%256));
    uart2SendByte(A13_VALUE);
    uart2SendByte((u8)(data.a13/256));
    uart2SendByte((u8)(data.a13%256));	
}