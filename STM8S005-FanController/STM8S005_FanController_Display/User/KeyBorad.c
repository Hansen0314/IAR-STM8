#include "KeyBorad.h"
struct KEYHANDLE KeyHandle;
void KeyBorad_PinInit(void)
{
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_4_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_3_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_2_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_1_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_L_1_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);         
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_L_2_PIN, GPIO_MODE_OUT_PP_HIGH_SLOW);         
	
}
u8 KeyBorad_Scan(void)
{
    u8 KeyVaul;
    GPIO_WriteHigh(KEYBORAD_PROT,KEYBORAD_L_1_PIN);
    GPIO_WriteLow(KEYBORAD_PROT,KEYBORAD_L_2_PIN);
    KeyVaul = GPIO_ReadInputData(KEYBORAD_PROT);
    //KeyVaul = KeyVaul|KEY_L_1_MASK;
    if(KeyVaul != KEY_L_1_MASK){
      Delay_Ms(KEY_DELAY_MS);
      KeyVaul = GPIO_ReadInputData(KEYBORAD_PROT);
      //KeyVaul = KeyVaul|KEY_L_1_MASK;		
      if(KeyVaul != KEY_L_1_MASK){
        switch(KeyVaul){
          case S1_DOWN_VALUE: return S1_DOWN_VALUE;
          case S2_DOWN_VALUE: return S2_DOWN_VALUE;
          case S3_DOWN_VALUE: return S3_DOWN_VALUE;
          case S4_DOWN_VALUE: return S4_DOWN_VALUE;	
          default : return KEYNULL;
        }
      }
    }
    
    GPIO_WriteLow(KEYBORAD_PROT,KEYBORAD_L_1_PIN);
    GPIO_WriteHigh(KEYBORAD_PROT,KEYBORAD_L_2_PIN);
    KeyVaul = GPIO_ReadInputData(KEYBORAD_PROT);
    //KeyVaul = KeyVaulKEY_L_2_MASK;
    if(KeyVaul != KEY_L_2_MASK){
      Delay_Ms(KEY_DELAY_MS);
      KeyVaul = GPIO_ReadInputData(KEYBORAD_PROT);
     // KeyVaul = KeyVaul|KEY_L_2_MASK;		
      if(KeyVaul != KEY_L_2_MASK){
        switch(KeyVaul){
          case S5_DOWN_VALUE: return S5_DOWN_VALUE;
          case S6_DOWN_VALUE: return S6_DOWN_VALUE;
          case S7_DOWN_VALUE: return S7_DOWN_VALUE;
          case S8_DOWN_VALUE: return S8_DOWN_VALUE;
          default : return KEYNULL;
        }
      }
    }
  return KEYNULL;      
}
void KeyBorad_Hnadle(u8 KeyVaul)
{
   
   switch(KeyVaul)
   {
      case S1_DOWN_VALUE: 
        KeyHandle.Fan_Seepd_State ++;
        if(KeyHandle.Fan_Seepd_State  > 4) KeyHandle.Fan_Seepd_State  = 1;
      break;
      case S2_DOWN_VALUE: 
        KeyHandle.Led_P1_State ++;
        if(KeyHandle.Led_P1_State > 1)
          KeyHandle.Led_P1_State = 0;
      break;
      case S3_DOWN_VALUE: 
        KeyHandle.Led_P2_State ++;
        if(KeyHandle.Led_P2_State > 1)
          KeyHandle.Led_P2_State = 0;
      break;
      case S4_DOWN_VALUE: 
        KeyHandle.Door_State ++;
        if(KeyHandle.Door_State > 3)
        KeyHandle.Door_State = 1;
      break;
      case S5_DOWN_VALUE:
        KeyHandle.Fan_Seepd_Max_State++;
        if(KeyHandle.Fan_Seepd_Max_State > 1)
          KeyHandle.Fan_Seepd_Max_State = 0;
      break;
      case S6_DOWN_VALUE: 
/*       
        if(KeyHandle.Fan_Seepd_Max_State == 1)
        {
          KeyHandle.Fan_Seepd_Max_State = 
          
        }
*/
      break;
      case S7_DOWN_VALUE: 
      break;
      case S8_DOWN_VALUE: 
      break;      
      default : 
      break;
  }
}
