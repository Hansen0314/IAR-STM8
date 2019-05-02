#include "stm8s.h"
#include "user_uart.h"
#include "Gpio.h"
#include "Adc.h"
#include "delay.h"
void delay(uint32_t time)
{
    while(--time);
}
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
extern u8 Send_peripheral;
struct Peripheral peripheral;
void main()
{   
    u8 i;
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    Gpio_Init();
    uart2Init();
    Adc_Init();
    Tim4_Init();
    Tim1_Init();
    enableInterrupts();
    while(1)
    {
    
#if 0
    GPIO_WriteLow(ER_PORT,ER_PIN);
    Delay_Ms(1000);
    GPIO_WriteHigh(ER_PORT,ER_PIN);
    Delay_Ms(1000); 
#else
      peripheral.Dp = GPIO_ReadInputPin(DP_PORT,DP_PIN);
      peripheral.Fr = GPIO_ReadInputPin(FR_PORT,FR_PIN); 
      peripheral.a11 = Adc_Concersion(A11_CHANNEL);
      peripheral.a12 = Adc_Concersion(A12_CHANNEL);
      peripheral.a13 = Adc_Concersion(A13_CHANNEL);
      peripheral.Door_Up = GPIO_ReadInputPin(DOOR_UP_IN_PORT,DOOR_UP_IN_PIN);
      peripheral.Door_Do = GPIO_ReadInputPin(DOOR_DO_IN_PORT,DOOR_DO_IN_PIN); 

      if(Send_peripheral == 1)
      {
        Uart_Send_data(peripheral);
        //uart2SendByte(0x01);
        Send_peripheral = 0;
//        printf("%d \n",peripheral.Door_Do);
//        printf("%d \n",peripheral.Door_Up);
      }
//      if(peripheral.Dp & peripheral.Fr)
//      {
//        GPIO_WriteLow(ER_PORT,ER_PIN);
//      }
//      else
//      {
//        GPIO_WriteHigh(ER_PORT,ER_PIN);
//      }    
#endif
    }
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif