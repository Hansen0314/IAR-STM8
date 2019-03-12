#include "stm8s.h"
#include "user_uart.h"
#include "delay.h"
#include "ht1621.h"
uint8_t ii;
#define RxBufferSize 64
extern u8 RxBuffer[RxBufferSize];
extern u8 UART_RX_NUM;
void main()
{
    uint8_t len;    
    CLK_HSICmd(ENABLE);
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
    uart2Init();
    Tim4_Init();
    ht1621_init();
    enableInterrupts();
    printf("\r\nӲ��ƽ̨Ϊ:%s\r\n","STM8S005K6 ������");
    printf("\r\n�޸�ʱ�䣺%s\r\n","2018-04-01");
    printf("\r\n�����̲��Է�����%s\r\n","�ڴ������������ַ������ַ�������Ҫ���»س������ٵ������");
    while(1)
    {
        if(UART_RX_NUM&0x80)
        {
            len=UART_RX_NUM&0x3f;/*�õ��˴ν��յ������ݳ���*/
            uart2SendString("You sent the messages is:",sizeof("You sent the messages is"));
            uart2SendString(RxBuffer,len);
            printf("\r\n�õ��˴ν��յ������ݳ���:%dByte\r\n",len);
            UART_RX_NUM=0;
        }
        printf("\r\nӲ��ƽ̨Ϊ:%s\r\n","STM8S005K6 ������");
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