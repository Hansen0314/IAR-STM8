#ifndef ADC_H
#define ADC_H
#include "stm8s.h"
#define ANALOG_PORT        GPIOB
#define A11_CHANNEL    ADC1_CHANNEL_4     //风速ADC_Channel_4
#define A12_CHANNEL    ADC1_CHANNEL_3     //湿度ADC_Channel_3
#define A13_CHANNEL    ADC1_CHANNEL_5     //温度ADC_Channel_5
#define A11_PIN            GPIO_PIN_4     //ADC_Channel_4
#define A12_PIN            GPIO_PIN_3     //ADC_Channel_3
#define A13_PIN            GPIO_PIN_5     //ADC_Channel_5
void Adc_Init(void);
u16 Adc_Concersion(ADC1_Channel_TypeDef ADC_Channels);
#endif