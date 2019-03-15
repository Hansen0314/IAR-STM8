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

struct Peripheral peripheral;
void main()
{   
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    Gpio_Init();
    uart2Init();
    Adc_Init();
    enableInterrupts();
    while(1)
    {
        peripheral.Dp = GPIO_ReadInputPin(DP_PORT,DP_PIN);
        peripheral.Fr = GPIO_ReadInputPin(FR_PORT,FR_PIN);
        peripheral.a11 = Adc_Concersion(A11_PIN_C);
        peripheral.a12 = Adc_Concersion(A12_PIN_C);
        peripheral.a13 = Adc_Concersion(A13_PIN_C);
        Uart_Send_data(peripheral);
        Delay_Ms(1000);
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