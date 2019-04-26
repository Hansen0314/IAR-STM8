/**
  ******************************************************************************
  * @file    delay.h
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   delay functions header
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
#include "stm8s.h"
#define TIM4_PERIOD       124
//��Ҫ����ϵͳʱ��Ϊ16M
//CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
//------------------------------------------------------------------------------
// Function Name : delay_ms
// Description   : delay for some time in ms unit
// Input         : n_ms is how many ms of time to delay
//------------------------------------------------------------------------------
void Delay_Ms(__IO uint32_t nTime);

void Delay_Us(u16 n_us);

void TimingDelay_Decrement(void);

extern __IO uint32_t TimingDelay;
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
void Tim4_Init(void);
void Tim1_Init(void);