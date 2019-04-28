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
#if 0
int fputc(int ch, FILE *f)
{  
  UART2->DR=(unsigned char)ch;
  while (!(UART2->SR & UART2_FLAG_TXE));
  return (ch);
}
#endif
void Uart_IT_Receive_Control(u8* data)
{
  if(data[5] == 0Xff)
  {
    if(data[1] == 0)GPIO_WriteLow(LED_PORT,LED_P1_PIN);
    else GPIO_WriteHigh(LED_PORT,LED_P1_PIN);
    if(data[2] == 0)GPIO_WriteLow(LED_PORT,LED_P2_PIN);
    else GPIO_WriteHigh(LED_PORT,LED_P2_PIN); 
    if(data[3] == 0)
    {
      GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_B_PIN);       
    }
    else if(data[3] == 1)
    {
      GPIO_WriteHigh(FAN_PORT,FAN_L_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_B_PIN);  
    }
    else if(data[3] == 2)
    {
      GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
      GPIO_WriteHigh(FAN_PORT,FAN_M_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_B_PIN);  
    } 
    else if(data[3] == 3)
    {
      GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
      GPIO_WriteHigh(FAN_PORT,FAN_B_PIN);  
    } 
    if(data[4] == 2)
    {
      GPIO_WriteHigh(DOOR_UP_PORT,DOOR_UP_PIN);
      GPIO_WriteLow(DOOR_DO_PORT,DOOR_DO_PIN);      
    }
    else if(data[4] == 1)
    {
      GPIO_WriteHigh(DOOR_DO_PORT,DOOR_DO_PIN);
      GPIO_WriteLow(DOOR_UP_PORT,DOOR_UP_PIN);      
    }
    else
    {
      GPIO_WriteLow(DOOR_DO_PORT,DOOR_DO_PIN);
      GPIO_WriteLow(DOOR_UP_PORT,DOOR_UP_PIN);           
    }
    
  }
  /*
  if(data[1] == FAN_OFF)
  {
      GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_B_PIN);    
  }
  else if(data[1] == FAN_ON_L)
  {
      GPIO_WriteHigh(FAN_PORT,FAN_L_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
      GPIO_WriteLow(FAN_PORT,FAN_B_PIN);     
  
  }
  else if()
  */
  /*
  switch(data)
  {
    case FAN_OFF:   
                    GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
                    GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
                    GPIO_WriteLow(FAN_PORT,FAN_B_PIN);
    break;	//只能开一个，高是开
    case FAN_ON_L: 
                    GPIO_WriteHigh(FAN_PORT,FAN_L_PIN);
                    GPIO_WriteLow(FAN_PORT,FAN_M_PIN);
                    GPIO_WriteLow(FAN_PORT,FAN_B_PIN);      
    break;
    case FAN_ON_M: 
                    GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
                    GPIO_WriteHigh(FAN_PORT,FAN_M_PIN);
                    GPIO_WriteLow(FAN_PORT,FAN_B_PIN); 
    break;
    case FAN_ON_B: 
                    GPIO_WriteLow(FAN_PORT,FAN_L_PIN);
                    (FAN_PORT,FAN_M_PIN);
                    GPIO_WriteHigh(FAN_PORT,FAN_B_PIN);   
    break;		
    case LED_P1_ON: GPIO_WriteHigh(LED_PORT,LED_P1_PIN);
    break;
    case LED_P1_OFF: GPIO_WriteLow(LED_PORT,LED_P1_PIN);
    break;
    case LED_P2_ON: GPIO_WriteHigh(LED_PORT,LED_P2_PIN);
    break;
    case LED_P2_OFF: GPIO_WriteLow(LED_PORT,LED_P2_PIN);
    break;
    case DOOR_UP_ON: 
      GPIO_WriteHigh(DOOR_UP_PORT,DOOR_UP_PIN);
      GPIO_WriteLow(DOOR_DO_PORT,DOOR_DO_PIN);
    break;		
    case DOOR_DO_ON:  
      GPIO_WriteHigh(DOOR_DO_PORT,DOOR_DO_PIN);
      GPIO_WriteLow(DOOR_UP_PORT,DOOR_UP_PIN);
    break;		
    case ER_ON: GPIO_WriteHigh(ER_PORT,ER_PIN);
    break;
    case ER_OFF: GPIO_WriteLow(ER_PORT,ER_PIN);
    break;    
  }
  */
}
void Uart_Send_data(struct Peripheral data)
{
  
  uint8_t str[14] = {0};

    str[0] = 0xfd;
    if(data.Fr) 
    {
      str[1] = 1;
    }
    else if(data.Fr == 0)
    {
      str[1] = 0;
    }
    
    if(data.Dp) 
    {
      str[2] = 1;
    }
    else if(data.Dp == 0)
    {
      str[2] = 0;
    }
    
    if(data.Door_Do) 
    {
      str[3] = 1;
    }
    else if(data.Door_Do == 0)
    {
      str[3] = 0;
    }
    
    if(data.Door_Up) 
    {
      str[4] = 1;
    }
    else 
    {
      str[4] = 0;
    }
    
    str[5] = A11_VALUE;
    str[6] = data.a11/256;
    str[7] = data.a11%256;
    
    str[8] = A12_VALUE;
    str[9] = data.a12/256;
    str[10] = data.a12%256;    
    
    str[11] = A13_VALUE;
    str[12] = data.a13/256;
    str[13] = data.a13%256;  
    
    uart2SendString(str,14);
}