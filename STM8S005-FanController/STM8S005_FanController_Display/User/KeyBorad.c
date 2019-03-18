#include "KeyBorad.h"
struct KEYHANDLE KeyHandle;
extern u16 Peripheral_A11_Max;
struct Hepa hepa;
extern u8 Pm_Time;
extern u8 Od_State;
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
        if(KeyHandle.Fan_Seepd_State  > 3) KeyHandle.Fan_Seepd_State  = 0;
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
        if(KeyHandle.HEAP_State == 1)
        {
          KeyHandle.HEAP_Dis_State ++;
          if(KeyHandle.HEAP_Dis_State > 1)
            KeyHandle.HEAP_Dis_State = 0;
        }
        else if(KeyHandle.Oper_Mode_State == 1)
        {
          KeyHandle.Oper_Mode_Dis_State ++;
          if(KeyHandle.Oper_Mode_Dis_State > 2)
          {
            KeyHandle.Oper_Mode_Dis_State = 0;
          }
        }
        else 
        {
          KeyHandle.Fan_Seepd_Max_State++;
          if(KeyHandle.Fan_Seepd_Max_State > 1)
           KeyHandle.Fan_Seepd_Max_State = 0;
        }
      break;
      case S6_DOWN_VALUE:      
        if(KeyHandle.Fan_Seepd_Max_State == 1)
        {
          Peripheral_A11_Max ++;
          if (Peripheral_A11_Max > 1000) 
            Peripheral_A11_Max = 0;
        }
        else if(KeyHandle.Oper_Mode_State == 1)
        {
          if(KeyHandle.Oper_Mode_Dis_State == 1)
          {
            if(KeyHandle.Od_State.Od_Num ==0)
            {
              KeyHandle.Od_State.Fan_State++;
              if(KeyHandle.Od_State.Fan_State > 3)
                KeyHandle.Od_State.Fan_State = 0;
            }
            else if(KeyHandle.Od_State.Od_Num ==1)
            {
              KeyHandle.Od_State.Led_P1_State++;
              if(KeyHandle.Od_State.Led_P1_State > 1)
                KeyHandle.Od_State.Led_P1_State = 0;              
            }
            else if(KeyHandle.Od_State.Od_Num ==2) 
            {
              KeyHandle.Od_State.Led_P2_State++;
              if(KeyHandle.Od_State.Led_P2_State > 1)
                KeyHandle.Od_State.Led_P2_State = 0;
            }
            else if(KeyHandle.Od_State.Od_Num ==3)
            {
              if(KeyHandle.Od_State.Led_P2_State == 0)
              {
                KeyHandle.Od_State.Door_State++;
                if(KeyHandle.Od_State.Door_State > 1)
                  KeyHandle.Od_State.Door_State = 0;
              }
              else
                KeyHandle.Od_State.Door_State = 0;              
            }
          }
          if(KeyHandle.Oper_Mode_Dis_State == 2)
          {
            if(KeyHandle.Pm_State.Pm_Num ==0)
            {
              KeyHandle.Pm_State.Fan_State++;
              if(KeyHandle.Pm_State.Fan_State > 3)
                KeyHandle.Pm_State.Fan_State = 0;
            }
            else if(KeyHandle.Pm_State.Pm_Num ==1)
            {
              KeyHandle.Pm_State.Led_P1_State++;
              if(KeyHandle.Pm_State.Led_P1_State > 1)
                KeyHandle.Pm_State.Led_P1_State = 0;              
            }
            else if(KeyHandle.Pm_State.Pm_Num ==2) 
            {
              KeyHandle.Pm_State.Led_P2_State++;
              if(KeyHandle.Pm_State.Led_P2_State > 1)
                KeyHandle.Pm_State.Led_P2_State = 0;
            }
            else if(KeyHandle.Pm_State.Pm_Num ==3)
            {
              if(KeyHandle.Pm_State.Led_P2_State == 0)
              {
                KeyHandle.Pm_State.Door_State++;
                if(KeyHandle.Pm_State.Door_State > 1)
                  KeyHandle.Pm_State.Door_State = 0;
              }
              else
                KeyHandle.Pm_State.Door_State = 0;              
            }
            else if(KeyHandle.Pm_State.Pm_Num == 4)
            {
              KeyHandle.Pm_State.Pm_Time ++;
              if(KeyHandle.Pm_State.Pm_Time > 7)
                KeyHandle.Pm_State.Pm_Time = 0;
            }
          }
        }
        else
        {
          KeyHandle.HEAP_State++;
          if(KeyHandle.HEAP_State > 1) 
            KeyHandle.HEAP_State = 0;
        }
      break;
      case S7_DOWN_VALUE: 
      if (KeyHandle.Fan_Seepd_Max_State == 1)
      {
        Peripheral_A11_Max --;  
        if (Peripheral_A11_Max < 1) Peripheral_A11_Max = 1000;
      }
      else if(KeyHandle.HEAP_State == 1)
      {
        if(KeyHandle.HEAP_Dis_State == 1)
        {  
          hepa.Fan_Seepd++;
          if(hepa.Fan_Seepd > 998)
            hepa.Fan_Seepd = 0;
        }
        else if(KeyHandle.HEAP_Dis_State == 0)
        {  
          hepa.Work_Time++;
          if(hepa.Work_Time > 9998)
            hepa.Work_Time = 0;
        }
      }
      else
      {
        KeyHandle.Oper_Mode_State ++;
        if(KeyHandle.Oper_Mode_State > 1) 
        {
          KeyHandle.Oper_Mode_State = 0;
        }
      }
      
      break;
      case S8_DOWN_VALUE: 
        if(KeyHandle.HEAP_State == 1)
        {
          if(KeyHandle.HEAP_Dis_State == 1)
          {  
            hepa.Fan_Seepd --;
            if(hepa.Fan_Seepd < 1)
              hepa.Fan_Seepd = 999;
          }
          else if(KeyHandle.HEAP_Dis_State == 0)
          {
            hepa.Work_Time --;
            if(hepa.Work_Time < 1)
              hepa.Work_Time = 9999;
          }
        }
        else if(KeyHandle.Oper_Mode_State == 1)
        {
          // KeyHandle.Oper_Mode_Dis_Time_Set_State ++;
          // if(KeyHandle.Oper_Mode_Dis_Time_Set_State > 1)
          //   KeyHandle.Oper_Mode_Dis_Time_Set_State = 0;
          if(KeyHandle.Oper_Mode_Dis_State == 2)
          {
            KeyHandle.Pm_State.Pm_Num++;
            if(KeyHandle.Pm_State.Pm_Num > 5)
              KeyHandle.Pm_State.Pm_Num = 0;            
          }
          else if(KeyHandle.Oper_Mode_Dis_State == 1)
          {
            KeyHandle.Od_State.Od_Num++;
            if(KeyHandle.Od_State.Od_Num > 4)
              KeyHandle.Od_State.Od_Num = 0;
          }
        }
        
      break;      
      default : 
      break;
  }
}
