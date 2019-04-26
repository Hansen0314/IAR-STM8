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
extern u16 Door_Move_time;
extern u8 Uart;
extern u8 Uart_Char;
u8 day;
u8 hour;
u8 KeyVaule;
extern u8 Uart_Char_Num;
void Hepa_Time_Conversion()
{
    day = Ds1302_Alldate.yd.day - Ds1302_Alldate_Init.yd.day; 
    hour = Ds1302_Alldate.hms.hour - Ds1302_Alldate_Init.hms.hour;
    hepa_time = day*24 + hour;
}
struct Peripheral Peripheral_Conversion()
{
  
  peripheral.a11 = (float)Peripheral_Realy.a11/1024*Peripheral_A11_Max;
  peripheral.a12 = (float)Peripheral_Realy.a12/1024*100;
  peripheral.a13 = (float)Peripheral_Realy.a13/1024*100;
  peripheral.Door_Do = Peripheral_Realy.Door_Do;
  peripheral.Door_Up = Peripheral_Realy.Door_Up;
  peripheral.Dp = Peripheral_Realy.Dp;
  peripheral.Fr = Peripheral_Realy.Fr;
  return peripheral;
  
}
void main()
{  
    
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    CLK_HSICmd(ENABLE);
#if 1 
    uart2Init();
    Tim4_Init();
    Tim1_Init();
    Gpio_Init();   
    KeyHandle.Door_State = 1;
    ht1621_init();
    enableInterrupts();
    KeyBorad_PinInit();
    Back_Light_On();
    Ds1302_Init();
    Ds1302_Alldate = ds1302_readTime(); 
    Ds1302_Alldate_Init = Ds1302_Alldate;
    KeyHandle.Door_State = 0;
    hepa.Fan_Seepd = 100;
    hepa.Work_Time = 100;
    //预约模式停以后再开机 需要再次开机必须需要人为干预
    //led异常
    //预约开机后进入wm模式（led1 ,led2 ,fan,OFF  Door,Up）
    //预约模式二次关机
#endif  
    //值班模式 选择完了再改
    while(1)
    {
#if 1 
      
      if(KeyHandle.Oper_Mode_Dis_State == 2)
      {
        if(KeyHandle.Oper_Mode_State == 0)
        {
          if((Ds1302_Alldate.yd.day == KeyHandle.Pm_State.Off_alldate.yd.day)&&(Ds1302_Alldate.hms.hour == KeyHandle.Pm_State.Off_alldate.hms.hour)&&(Ds1302_Alldate.hms.min == KeyHandle.Pm_State.Off_alldate.hms.min))
          {
            ht1621_Clear();
            KeyHandle.Pm_State.Led_P1_State = 0;
            KeyHandle.Pm_State.Led_P2_State = 0;
            KeyHandle.Pm_State.Fan_State = 0;
            KeyHandle.Pm_State.Door_State = 2;
            
            Uart_Transmit_Hnadle(KeyHandle);
            while(1)
            {
              Ds1302_Alldate = ds1302_readTime();
              Now_Time_Display(Ds1302_Alldate,KeyHandle); 
              if((Ds1302_Alldate.yd.day == KeyHandle.Pm_State.On_alldate.yd.day)&&(Ds1302_Alldate.hms.hour == KeyHandle.Pm_State.On_alldate.hms.hour)&&(Ds1302_Alldate.hms.min == KeyHandle.Pm_State.On_alldate.hms.min))
              break;
              else
              {
                KeyVaule=KeyBorad_Scan();
                if(KeyVaule == S7_DOWN_VALUE) break;
              }
            }
          }
        }
      }  
      
      
      Ds1302_Alldate = ds1302_readTime();
      Hepa_Time_Conversion();
      KeyVaule=KeyBorad_Scan();
      if(KeyVaule !=KEYNULL)
      {
        KeyBorad_Hnadle(KeyVaule);
        if(KeyHandle.Oper_Mode_State == 0)
        {
          Uart_Transmit_Hnadle(KeyHandle);
        }
      } 
      
      //门升降没有检测到
      Display_all(Peripheral_Conversion(),KeyHandle,hepa,Ds1302_Alldate); 
      
      //ht1621_Char_write1(1,T_Addr[15],T_Mask[15],0,0);
#else
      Uart_Transmit_Hnadle(KeyHandle);
      Delay_Ms(1000);
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