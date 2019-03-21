#include "stm8s.h"
#include "user_uart.h"
#include "delay.h"
#include "ht1621.h"
#include "Gpio.h"
#include "KeyBorad.h"
#include "DS1302.h"
extern u8 Dis_Bling;
extern u8 Dis_Door_Bling;
struct Peripheral peripheral;
extern struct Hepa hepa;
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
struct ALLDATE Ds1302_Alldate;
struct ALLDATE Ds1302_Alldate_Init;
extern u16 Peripheral_A11_Max;
extern u8 Pm_Time;
u8 day;
u8 hour;
u8 KeyVaule;
void Hepa_Time_Conversion()
{
    day = Ds1302_Alldate.yd.day - Ds1302_Alldate_Init.yd.day; 
    hour = Ds1302_Alldate.hms.hour - Ds1302_Alldate_Init.hms.hour;
    hepa_time = day*24 + hour;
}
void Peripheral_Conversion()
{
  peripheral.a11 = (int)((float)(peripheral.a11/1024)*Peripheral_A11_Max);
  peripheral.a12 = (int)((float)(peripheral.a11/1024)*100);
  peripheral.a13 = (int)((float)(peripheral.a11/1024)*100);
}
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
    peripheral.a11 = 1;
    peripheral.a12 = 648;
    peripheral.a13 = 50;
    KeyHandle.Fan_Seepd_State = 0;
    hepa.Fan_Seepd = 2;
    
    KeyHandle.Door_State = 1;
    ht1621_init();
    enableInterrupts();
    KeyBorad_PinInit();
    Back_Light_On();
    Ds1302_Init();
    Ds1302_Alldate = ds1302_readTime(); 
    Ds1302_Alldate_Init = Ds1302_Alldate;
    printf("%d \n",Ds1302_Alldate.yd.year);
    peripheral.Fr = 1;
    peripheral.Dp = 0;
    KeyHandle.Door_State = 0;
#endif  
    while(1)
    {
#if 1     
      peripheral = Peripheral_Realy;
      
      Peripheral_Conversion();
      Ds1302_Alldate = ds1302_readTime();
      Hepa_Time_Conversion();
      KeyVaule=KeyBorad_Scan();
      if(KeyVaule !=KEYNULL)
      {
        KeyBorad_Hnadle(KeyVaule);
        Uart_Transmit_Hnadle(KeyHandle);
      }
      Peripheral_Realy.Door_Up = 1;
      Peripheral_Realy.Door_Do = 1;
      if(Peripheral_Realy.Door_Up | Peripheral_Realy.Door_Do);
      else Dis_Door_Bling = 1;
      Display_all(peripheral,KeyHandle,hepa,Ds1302_Alldate);
      
      //ht1621_Char_write1(1,T_Addr[15],T_Mask[15],0,0);
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