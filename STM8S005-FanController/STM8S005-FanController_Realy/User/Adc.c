#include "Adc.h"
void Adc_Init(void)
{       
      
      GPIO_Init(ANALOG_PORT, A11_PIN, GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(ANALOG_PORT, A12_PIN, GPIO_MODE_IN_FL_NO_IT);
      GPIO_Init(ANALOG_PORT, A13_PIN, GPIO_MODE_IN_FL_NO_IT);
      ADC1_DeInit();
      ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS,ADC1_CHANNEL_3,ADC1_PRESSEL_FCPU_D2,\
	  ADC1_EXTTRIG_TIM,DISABLE,ADC1_ALIGN_RIGHT,ADC1_SCHMITTTRIG_CHANNEL9,\
	  DISABLE);  
      //ADC1_StartConversion();
      /* Enable EOC interrupt */
      //ADC1_ITConfig(ADC1_IT_EOCIE,ENABLE);
      /* Enable general interrupts */  
      //enableInterrupts();
}
u16 Adc_Concersion(ADC1_Channel_TypeDef ADC_Channels)
{
	u16 data;
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_SINGLE,ADC_Channels,ADC1_ALIGN_RIGHT);
        ADC1_StartConversion();
	while(!ADC1_GetFlagStatus(ADC1_FLAG_EOC));//等待转换结束
	ADC1_ClearFlag(ADC1_FLAG_EOC);//清除相关标识
	data = ADC1_GetConversionValue();
        //if(ADC_Channels == )
	data = data*5000UL/1024UL;
	return data;

}