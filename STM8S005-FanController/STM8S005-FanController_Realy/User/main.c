#include "stm8s.h"
#include "user_uart.h"
void delay(uint32_t time)
{
    while(--time);
}
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
    enableInterrupts();
    printf("\r\n硬件平台为:%s\r\n","STM8S005K6 开发板");
    printf("\r\n修改时间：%s\r\n","2018-04-01");
    printf("\r\n本例程测试方法：%s\r\n","在串口助手输入字符或者字符串必须要按下回车键，再点击发送");
    while(1)
    {
        /*
        GPIO_WriteLow(GPIOD,GPIO_PIN_2);
        if(++ii>250)
                ii=0;
        printf("\r\n ii=%3d",ii);

        delay(9000);
        GPIO_WriteReverse(GPIOA,GPIO_PIN_1);
        delay(9000);
        */
        if(UART_RX_NUM&0x80)
        {
            len=UART_RX_NUM&0x3f;/*得到此次接收到的数据长度*/
            uart2SendString("You sent the messages is:",sizeof("You sent the messages is"));
            uart2SendString(RxBuffer,len);
            printf("\r\n得到此次接收到的数据长度:%dByte\r\n",len);
            UART_RX_NUM=0;
        }
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