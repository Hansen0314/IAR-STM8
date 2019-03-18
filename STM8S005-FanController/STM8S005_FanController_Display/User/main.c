#include "stm8s.h"
#include "user_uart.h"
#include "delay.h"
#include "ht1621.h"
#include "Gpio.h"
#include "KeyBorad.h"
#include "DS1302.h"
 u8 Dis_Bling;
struct Peripheral peripheral;
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
struct ALLDATE Ds1302_Alldate;
extern u16 Peripheral_A11_Max;
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
//    printf("%d \n",i);
    KeyHandle.Fan_Seepd_State = 4;
#endif  
    while(1)
    {
#if 1     
       if(Dis_Bling == 1)
       {
          printf("1\n");
          Dis_Bling = 0;
       }
       /*      
       Delay_Ms(1000);
       Ds1302_Alldate = ds1302_readTime();
       printf("now time is %d:%d:%d\n",(int)Ds1302_Alldate.hms.hour,(int)Ds1302_Alldate.hms.min,(int)Ds1302_Alldate.hms.sec); 
       */
      KeyBorad_Hnadle(KeyBorad_Scan());
      Display_all(KeyHandle);
      //Delay_Ms(1000);
      //Peripheral_Rceive_Display(Peripheral_Realy,1,1,1);
      //Fan_Speed_State_Display(KeyHandle.Fan_Seepd_State);      
      
#endif
#if 0 
      key_value = KeyBorad_Scan();
      
      if(key_value != 0x00)
       printf("%X\n",(int)key_value);       
      Delay_Ms(1000);
      ht1621_Char_write(1,P_Addr[1],P_Mask[1],1);
      Delay_Ms(1000);
      ht1621_Char_write(1,P_Addr[1],P_Mask[1],0);
       
      for(i=CS1_DIGITRON_START;i<CS1_DIGITRON_END+1;i++)
      {
           ht1621_write(1,Dis_Digitron_Addr[i],Cs1_Dis_Digitron_Num[3]);
           Delay_Ms(1000);
      }    
      for(i=CS1_T_START;i<CS1_T_END+1;i++)
      {
          ht1621_Char_write(1,T_Addr[i],T_Mask[i],1);
          Delay_Ms(1000);
      }
      for(i=CS2_T_START;i<CS2_T_END+1;i++)
      {
          ht1621_Char_write(2,T_Addr[i],T_Mask[i],1);
          Delay_Ms(1000);
      }
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