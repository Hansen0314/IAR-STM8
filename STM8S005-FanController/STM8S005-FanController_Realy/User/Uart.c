#include "Uart.h"
#include "Gpio.h"
void Uart_IT_Receive_Control(u8 data)
{
  switch(data)
  {
    case FAN_OFF:  GPIO_SetBits(FAN_PORT,FAN_L_PIN);
                   GPIO_SetBits(FAN_PORT,FAN_M_PIN);
                   GPIO_SetBits(FAN_PORT,FAN_B_PIN);
    break;	
    case FAN_ON_L: GPIO_ResetBits(FAN_PORT,FAN_L_PIN);
    break;
    case FAN_ON_M: GPIO_ResetBits(FAN_PORT,FAN_M_PIN);
    break;
    case FAN_ON_B: GPIO_ResetBits(FAN_PORT,FAN_B_PIN);
    break;		
    case LED_P1_ON: GPIO_ResetBits(LED_PORT,LED_P1_PIN);
    break;
    case LED_P1_OFF: GPIO_SetBits(LED_PORT,LED_P1_PIN);
    break;
    case LED_P2_ON: GPIO_ResetBits(LED_PORT,LED_P2_PIN);
    break;
    case LED_P2_OFF: GPIO_SetBits(LED_PORT,LED_P2_PIN);
    break;
    case DOOR_UP_ON:  GPIO_ResetBits(DOOR_UP_PORT,DOOR_UP_PIN);
    break;
    case DOOR_UP_OFF: GPIO_SetBits(DOOR_UP_PORT,DOOR_UP_PIN);
    break;		
    case DOOR_DO_ON:  GPIO_ResetBits(DOOR_DO_PORT,DOOR_DO_PIN);
    break;
    case DOOR_DO_OFF: GPIO_SetBits(DOOR_DO_PORT,DOOR_DO_PIN);
    break;			
  
  }
}
void Uart_Send_data(struct Peripheral data)
{
    if(data.Fr) USART_SendData8(USART1,FR_HIGH);
    else USART_SendData8(USART1,FR_LOW);
    if(data.Dp) USART_SendData8(USART1,DP_HIGH);
    else USART_SendData8(USART1,DP_LOW);
    
    USART_SendData8(USART1,A11_VALUE);
    USART_SendData8(USART1,(u8)(data.a11/256));
    USART_SendData8(USART1,(u8)(data.a11%256));
    USART_SendData8(USART1,A12_VALUE);
    USART_SendData8(USART1,(u8)(data.a12/256));
    USART_SendData8(USART1,(u8)(data.a12%256));
    USART_SendData8(USART1,A13_VALUE);
    USART_SendData8(USART1,(u8)(data.a13/256));
    USART_SendData8(USART1,(u8)(data.a13%256));	
}