/**
  ******************************************************************************
  * @file    delay.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   delay functions
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"
__IO uint32_t TimingDelay = 0;
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay_Ms(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while (TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}
/**
  * @brief  delay for some time in 10us unit(partial accurate)
  * @caller auto_test
  * @param n_us is how many us of time to delay
  * @retval None
  */
void Delay_Us(u16 n_us)
{
#if 0
/* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
/* SYS_CLK_HSI_DIV1 Auto-Reload value: 16M / 1 = 16M, 16M / 1M = 16 */
  TIM4_TimeBaseInit(TIM4_Prescaler_1,16);
  TIM4_SetCounter(0);
  TIM4_ClearFlag(TIM4_FLAG_Update);
  TIM4_Cmd(ENABLE);
  while(n_us--)
  {
    while(TIM4_GetFlagStatus(TIM4_FLAG_Update) == 0) ;
    TIM4_ClearFlag(TIM4_FLAG_Update);
  }
  TIM4_Cmd(DISABLE);
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
#else
  while(n_us--){
  nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
  }
#endif
}

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
void Tim4_Init(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125 000 Hz
  - With 125 000 Hz we can generate time base:
      max time base is 2.048 ms if TIM4_PERIOD = 255 --> (255 + 1) / 125000 = 2.048 ms
      min time base is 0.016 ms if TIM4_PERIOD = 1   --> (  1 + 1) / 125000 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  
  /* enable interrupts */
  enableInterrupts();

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}
