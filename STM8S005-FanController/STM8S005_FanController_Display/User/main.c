#include "stm8s.h"
#include "user_uart.h"
#include "delay.h"
#include "ht1621.h"
struct Peripheral peripheral;
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
void main()
{  
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    uart2Init();
    Tim4_Init();
    peripheral.a11 = 245;
    peripheral.a12 = 648;
    peripheral.a13 = 50;
    peripheral.Fr = 0;
    peripheral.Dp = 0;
    ht1621_init();
    enableInterrupts();
    while(1)
    {
      
#if 0      
       ht1621_write(1,Cs1_Dis_Digitron_Addr[1],Cs1_Dis_Digitron_Num[peripheral.a11/100]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[2],Cs1_Dis_Digitron_Num[peripheral.a11%100/10]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[3],Cs1_Dis_Digitron_Num[peripheral.a11%10]);
       
       ht1621_write(1,Cs1_Dis_Digitron_Addr[4],Cs1_Dis_Digitron_Num[peripheral.a12/100]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[5],Cs1_Dis_Digitron_Num[peripheral.a12%100/10]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[6],Cs1_Dis_Digitron_Num[peripheral.a12%10]);
       
       ht1621_write(1,Cs1_Dis_Digitron_Addr[7],Cs1_Dis_Digitron_Num[peripheral.a13/10]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[8],Cs1_Dis_Digitron_Num[peripheral.a13%10]);
            
       ht1621_write(1,Cs1_Dis_Digitron_Addr[9],Cs1_Dis_Digitron_Num[peripheral.Fr/100]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[10],Cs1_Dis_Digitron_Num[peripheral.Fr%100/10]);
       ht1621_write(1,Cs1_Dis_Digitron_Addr[11],Cs1_Dis_Digitron_Num[peripheral.Fr%10]); 
       
       if(peripheral.Fr + peripheral.Dp) ht1621_Char_write(1,T_Addr[2],T_Mask[2],1);
       else ht1621_Char_write(1,T_Addr[2],T_Mask[2],0);
       
       Delay_Ms(1000);
       ht1621_Char_write(1,P_Addr[1],P_Mask[1],1);
       Delay_Ms(1000);
       ht1621_Char_write(1,P_Addr[1],P_Mask[1],0);
       
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