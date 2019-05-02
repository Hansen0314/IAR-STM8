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
u16 Now_time;    
u16 Pm_On_Time; 
u16 Pm_Off_Time; 
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
struct ALLDATE Ds1302_Alldate;
struct ALLDATE Ds1302_Alldate_Init;
extern u16 Peripheral_A11_Max;
extern u8 Pm_Time;
extern u16 Door_Move_time;
extern u8 Uart;
extern u8 Uart_Char;
extern u8 alldate_Updata;
struct KEYHANDLE KeyHandle_Init;
u8 day;
u8 hour;
u8 KeyVaule;
u8 Pm_OnState;
extern u8 Uart_Char_Num;
  float a12;
  u16 a12_1;
  u16 test;
  float a12_next;
  float a12_math;
u8 a12_str[10];
void Time_Conversion()
{
    hepa_time   = Ds1302_Alldate.md.date*24+Ds1302_Alldate.hms.hour-Ds1302_Alldate_Init.md.date*24+Ds1302_Alldate_Init.hms.hour;
    Now_time    = Ds1302_Alldate.md.date*24*60+Ds1302_Alldate.hms.hour*60+Ds1302_Alldate.hms.min;
    Pm_On_Time  = KeyHandle.Pm_State.On_alldate.md.date*24*60+KeyHandle.Pm_State.On_alldate.hms.hour*60+KeyHandle.Pm_State.On_alldate.hms.min;
    Pm_Off_Time = KeyHandle.Pm_State.Off_alldate.md.date*24*60+KeyHandle.Pm_State.Off_alldate.hms.hour*60+KeyHandle.Pm_State.Off_alldate.hms.min;
}
struct Peripheral Peripheral_Conversion()
{

  peripheral.a11 = (float)Peripheral_Realy.a11/1024*Peripheral_A11_Max;
  if(Peripheral_Realy.a12 == 0)
  peripheral.a12 = 0;
  else
  {
    a12 = (float)Peripheral_Realy.a12/1024*5;
    a12_next = a12*a12;
    a12_math = 117*a12_next-785*a12+1231;
    peripheral.a12 = a12_math*10;
    if(peripheral.a12 >= 999) peripheral.a12 = 999;
    if(peripheral.a12 <= 0) peripheral.a12 = 0;
  }
  peripheral.a13 = (float)Peripheral_Realy.a13/1024*100;//湿度
  peripheral.Door_Do = Peripheral_Realy.Door_Do;
  peripheral.Door_Up = Peripheral_Realy.Door_Up;
  peripheral.Dp = Peripheral_Realy.Dp;
  peripheral.Fr = Peripheral_Realy.Fr;
  return peripheral;
  
}
void FLASH_Init()
{
  u32  add;
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  
  add = 0x40A5;
  FLASH_ProgramByte(add,Ds1302_Alldate_Init.md.date);
  add = 0x40A6;
  FLASH_ProgramByte(add,Ds1302_Alldate_Init.hms.hour); 
  
  Ds1302_Alldate_Init.md.date = FLASH_ReadByte(add-1);
  Ds1302_Alldate_Init.hms.hour = FLASH_ReadByte(add);
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
    ht1621_init();
    enableInterrupts();
    KeyBorad_PinInit();
    Back_Light_On();
    Ds1302_Init();
    Ds1302_Alldate = ds1302_readTime(); 
    Ds1302_Alldate_Init = Ds1302_Alldate;
    //FLASH_Init();
    KeyHandle.Door_State = 0;
    hepa.Fan_Seepd = 40;
    hepa.Work_Time = 100;
    KeyHandle.Led_P1_State = 0;
    KeyHandle.Led_P2_State = 0;
    KeyHandle.Fan_State = 0;
    KeyHandle.Door_State = 2;
    KeyHandle.Dis_Door_State = 2;
    KeyHandle.HEAP_Dis_State = 2;
    KeyHandle.Pm_State.Dis_Door_State = 2;
    KeyHandle.Od_State.Dis_Door_State = 2;
    alldate_Updata = 1;
    //Peripheral_A11_Max = 50;
#endif  
    //Öµ°àÄ£Ê½ Ñ¡ÔñÍêÁËÔÙ¸Ä
    while(1)
    {
#if 1 
      Ds1302_Alldate = ds1302_readTime();
      Time_Conversion();
      if((KeyHandle.Oper_Mode_Dis_State == 2)&&(KeyHandle.Oper_Mode_State == 0))
      {
        if(Now_time < Pm_On_Time) //当前时间小于预约开机时间 那么关机 直到开机时间在开机
        {  
          ht1621_Clear();
          KeyHandle_Init.Pm_State.Led_P1_State = 0;
          KeyHandle_Init.Pm_State.Led_P2_State = 0;
          KeyHandle_Init.Pm_State.Fan_State = 0;
          KeyHandle_Init.Pm_State.Door_State = 3;    
          Uart_Transmit_Hnadle(KeyHandle_Init);
          TIM1_Cmd(DISABLE);
          while(1)
          {
            Ds1302_Alldate = ds1302_readTime();
            Time_Conversion();
            Now_Time_Display(Ds1302_Alldate,KeyHandle);    
            if(Now_time >= Pm_On_Time)
            {
              Pm_OnState = 1;
              Uart_Transmit_Hnadle(KeyHandle);
              TIM1_Cmd(ENABLE);
              break;
            }
            //if()
          }  
        }
        if(Pm_OnState == 1)
        {
          if(Now_time > Pm_Off_Time)
          {
            ht1621_Clear();
            KeyHandle_Init.Pm_State.Led_P1_State = 0;
            KeyHandle_Init.Pm_State.Led_P2_State = 0;
            KeyHandle_Init.Pm_State.Fan_State = 0;
            KeyHandle_Init.Pm_State.Door_State = 3;    
            KeyHandle_Init.Pm_State.Dis_Door_State = 3;
            Uart_Transmit_Hnadle(KeyHandle_Init);
            TIM1_Cmd(DISABLE);            
            Pm_OnState = 0;
            while(1)
            {
              Ds1302_Alldate = ds1302_readTime();
              Time_Conversion();
              Now_Time_Display(Ds1302_Alldate,KeyHandle);                
              KeyVaule=KeyBorad_Scan();
              if(KeyVaule == S8_DOWN_VALUE) 
              {
                KeyHandle.Led_P1_State = 0;
                KeyHandle.Led_P2_State = 0;
                KeyHandle.Fan_State = 0;
                KeyHandle.Door_State = 2;
                KeyHandle.Dis_Door_State = 2;
                KeyHandle.Oper_Mode_Dis_State = 0;
                Uart_Transmit_Hnadle(KeyHandle);
                TIM1_Cmd(ENABLE);
                break;
              }               
            }   
          }
        }
      }      
      Display_all(Peripheral_Conversion(),KeyHandle,hepa,Ds1302_Alldate); 
      KeyBorad_Hnadle(KeyBorad_Scan());
      if((hepa.Fan_Seepd > peripheral.a11)||(hepa.Work_Time < hepa_time)) 
      {
        KeyHandle.Er = 1;
        KeyHandle.Od_State.Er = 1;
        KeyHandle.Pm_State.Er = 1;
      //Uart_Transmit_Hnadle(KeyHandle);
      }
      else
      {
        KeyHandle.Er = 0;
        KeyHandle.Od_State.Er = 0;
        KeyHandle.Pm_State.Er = 0;
      }
      if(KeyHandle.Oper_Mode_State == 0)
      {
        Uart_Transmit_Hnadle(KeyHandle);
      }      
      //ht1621_Char_write1(1,T_Addr[15],T_Mask[15],0,0);
#else
      Display_all(peripheral,KeyHandle,hepa,Ds1302_Alldate);
      //ht1621_char_display();
      //Uart_Transmit_Hnadle(KeyHandle);
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
