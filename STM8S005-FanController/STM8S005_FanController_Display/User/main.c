#include "stm8s.h"
#include "user_uart.h"
#include "delay.h"
#include "ht1621.h"
#include "Gpio.h"
#include "KeyBorad.h"
#include "DS1302.h"
extern u8 Dis_Bling;
struct Peripheral peripheral;
extern struct Hepa hepa;
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
struct ALLDATE Ds1302_Alldate;
extern u16 Peripheral_A11_Max;
extern u8 Pm_Time;
void main()
{  
    u8 i = 0;
    
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_HSICmd(ENABLE);
#if 1 
    uart2Init();
    Tim4_Init();
    Tim1_Init();
    Gpio_Init();
    peripheral = Peripheral_Realy;
    peripheral.a11 = 245;
    peripheral.a12 = 648;
    peripheral.a13 = 50;
    peripheral.Fr = 0;
    peripheral.Dp = 0;
    ht1621_init();
    enableInterrupts();
    KeyBorad_PinInit();
    Back_Light_On();
    Ds1302_Init();
    Ds1302_Alldate = ds1302_readTime(); 
    printf("%d \n",Ds1302_Alldate.yd.year);
    KeyHandle.Fan_Seepd_State = 0;
    hepa.Fan_Seepd = 999;
    
#endif  
    while(1)
    {
#if 1     
      Ds1302_Alldate = ds1302_readTime();
      Delay_Ms(1000);
      KeyBorad_Hnadle(KeyBorad_Scan());
      Display_all(peripheral,KeyHandle,hepa,Ds1302_Alldate);        
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