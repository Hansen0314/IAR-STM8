#include "KeyBorad.h"
#include "ht1621.h"
struct KEYHANDLE KeyHandle;
extern u16 Peripheral_A11_Max;
extern struct Peripheral peripheral;
struct Hepa hepa;
extern u8 Pm_Time;
extern u8 Od_State;
extern u16 Door_Move_time;
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：不精确延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
} 

void KeyBorad_PinInit(void)
{
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_4_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_3_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_2_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_H_1_PIN, GPIO_MODE_IN_PU_NO_IT);              
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_L_1_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);         
    GPIO_Init(KEYBORAD_PROT, KEYBORAD_L_2_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);         
	
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
          case S1_DOWN_VALUE: while(S1_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S1_DOWN_VALUE;
          case S2_DOWN_VALUE: while(S2_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S2_DOWN_VALUE;
          case S3_DOWN_VALUE: while(S3_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S3_DOWN_VALUE;
          case S4_DOWN_VALUE: while(S4_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S4_DOWN_VALUE;	
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
          case S7_DOWN_VALUE: while(S7_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S7_DOWN_VALUE;
          case S8_DOWN_VALUE: while(S8_DOWN_VALUE == GPIO_ReadInputData(KEYBORAD_PROT));return S8_DOWN_VALUE;
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
        if(KeyHandle.Oper_Mode_Dis_State == 0)
        {
          KeyHandle.Fan_State ++;
          if(KeyHandle.Fan_State  > 3) KeyHandle.Fan_State  = 0;
        }
        else if (KeyHandle.Oper_Mode_Dis_State == 1)
        { 
          if(KeyHandle.Oper_Mode_State == 1)
          {
            KeyHandle.Od_State.Fan_State ++;
            if(KeyHandle.Od_State.Fan_State > 3)KeyHandle.Od_State.Fan_State = 0;
          }
        }
        else if(KeyHandle.Oper_Mode_Dis_State == 2)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            KeyHandle.Pm_State.Fan_State ++;
            if(KeyHandle.Pm_State.Fan_State > 3)KeyHandle.Pm_State.Fan_State = 0;
          }
        }
      break;
      case S2_DOWN_VALUE: 
        if(KeyHandle.Oper_Mode_Dis_State == 0)
        {
          KeyHandle.Led_P1_State ++;
          if(KeyHandle.Led_P1_State > 1)
            KeyHandle.Led_P1_State = 0;
        }
        else if (KeyHandle.Oper_Mode_Dis_State == 1)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            KeyHandle.Od_State.Led_P1_State ++;
            if(KeyHandle.Od_State.Led_P1_State > 1)
              KeyHandle.Od_State.Led_P1_State = 0;
          }
        }
        else if(KeyHandle.Oper_Mode_Dis_State == 2)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            KeyHandle.Pm_State.Led_P1_State ++;
            if(KeyHandle.Pm_State.Led_P1_State > 1)
              KeyHandle.Pm_State.Led_P1_State = 0;
          }
        }
      break;
      case S3_DOWN_VALUE: 
        if(KeyHandle.Oper_Mode_Dis_State == 0)
        {
          if(KeyHandle.Dis_Door_State != 2)
          {
            if(peripheral.Door_Do == 0)
            KeyHandle.Led_P2_State ++;
            if(KeyHandle.Led_P2_State > 1)
              KeyHandle.Led_P2_State = 0;
          }
        }
        else if (KeyHandle.Oper_Mode_Dis_State == 1)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            if(KeyHandle.Od_State.Dis_Door_State != 2)
            {
               // if(peripheral.Door_Do == 0)
                KeyHandle.Od_State.Led_P2_State ++;
                if(KeyHandle.Od_State.Led_P2_State > 1)
                  KeyHandle.Od_State.Led_P2_State = 0;              
            }
          }
        }
        else if(KeyHandle.Oper_Mode_Dis_State == 2)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            if(KeyHandle.Pm_State.Dis_Door_State != 2)
            {
              //if(peripheral.Door_Do == 0)
              KeyHandle.Pm_State.Led_P2_State ++;
              if(KeyHandle.Pm_State.Led_P2_State > 1)
                KeyHandle.Pm_State.Led_P2_State = 0;
            }
          }
        }        
      break;
      case S4_DOWN_VALUE: 
        if(KeyHandle.Oper_Mode_Dis_State == 0)
        {
          if(KeyHandle.Led_P2_State != 1)
          {
            KeyHandle.Dis_Door_State ++;
            KeyHandle.Door_State = 2;
          }
          Door_Move_time = 0;
          if(KeyHandle.Dis_Door_State > 2)
          {
            KeyHandle.Dis_Door_State = 1;
            KeyHandle.Door_State = 1;
          }
        }
        else if (KeyHandle.Oper_Mode_Dis_State == 1)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            if(KeyHandle.Od_State.Led_P2_State != 1)
            {
              KeyHandle.Od_State.Dis_Door_State++;
              KeyHandle.Od_State.Door_State = 2;
            }
            Door_Move_time = 0;
            if(KeyHandle.Od_State.Dis_Door_State > 2)
            {
              KeyHandle.Od_State.Dis_Door_State = 1;
              KeyHandle.Od_State.Door_State = 1;
            }
          }
        }
        else if(KeyHandle.Oper_Mode_Dis_State == 2)
        {
          if(KeyHandle.Oper_Mode_State == 1)
          {
            if(KeyHandle.Pm_State.Led_P2_State != 1)
            {
              KeyHandle.Pm_State.Dis_Door_State ++;
              KeyHandle.Pm_State.Door_State = 2;
            }
            Door_Move_time = 0;
            if(KeyHandle.Pm_State.Dis_Door_State > 2)
            {
              KeyHandle.Pm_State.Dis_Door_State = 1;
              KeyHandle.Pm_State.Door_State = 1;
            }
          }
        }          
      break;
      case S5_DOWN_VALUE:
        if(KeyHandle.Oper_Mode_Dis_State == 0)
        {
          if(KeyHandle.Fan_Seepd_Max_State == 1)
          {
            Peripheral_A11_Max ++;
            if (Peripheral_A11_Max > 999) 
              Peripheral_A11_Max = 0;
          }
          else if(KeyHandle.HEAP_State == 1)
          {
            if(KeyHandle.HEAP_Dis_State == 1)
            {  

              hepa.Fan_Seepd++;
              if(hepa.Fan_Seepd > 999)
                hepa.Fan_Seepd = 0;
            }
            else if(KeyHandle.HEAP_Dis_State == 0)
            {  
              hepa.Work_Time++;
              if(hepa.Work_Time > 9998)
                hepa.Work_Time = 0;
            }
          }
          else if(KeyHandle.Time_State == 1)
          {
            KeyHandle.AllDate=ds1302_readTime();
            KeyHandle.AllDate.md.month++;
            if(KeyHandle.AllDate.md.month > 12)KeyHandle.AllDate.md.month = 1;
            ds1302_setTime(KeyHandle.AllDate);
          }
          else if(KeyHandle.Time_State == 2)
          {
            KeyHandle.AllDate=ds1302_readTime();
            KeyHandle.AllDate.md.date++;
            if(KeyHandle.AllDate.md.date > 31)KeyHandle.AllDate.md.date = 0;
            ds1302_setTime(KeyHandle.AllDate);
          }
          else if(KeyHandle.Time_State == 3)
          {
            KeyHandle.AllDate=ds1302_readTime();
            KeyHandle.AllDate.hms.hour++;
            if(KeyHandle.AllDate.hms.hour > 23)KeyHandle.AllDate.hms.hour = 0;
            ds1302_setTime(KeyHandle.AllDate);
          }
          else if(KeyHandle.Time_State == 4)
          {
            KeyHandle.AllDate=ds1302_readTime();
            KeyHandle.AllDate.hms.min++;
            if(KeyHandle.AllDate.hms.min > 59)KeyHandle.AllDate.hms.min = 0;
            ds1302_setTime(KeyHandle.AllDate);
          }
          else if(KeyHandle.Time_State == 5)
          {
            KeyHandle.AllDate=ds1302_readTime();
            KeyHandle.AllDate.yd.day++;
            if(KeyHandle.AllDate.yd.day > 7)KeyHandle.AllDate.yd.day = 1;
            ds1302_setTime(KeyHandle.AllDate);
          }           
        }
        else if(KeyHandle.Oper_Mode_Dis_State == 2)
        {
          if(KeyHandle.Pm_State.Pm_Num == 4)
          {
            KeyHandle.Pm_State.On_alldate.md.date ++;
            if(KeyHandle.Pm_State.On_alldate.md.date > 31)KeyHandle.Pm_State.On_alldate.md.date = 0;
          }
          else if(KeyHandle.Pm_State.Pm_Num == 5)
          {
            KeyHandle.Pm_State.On_alldate.hms.hour ++;
            if(KeyHandle.Pm_State.On_alldate.hms.hour > 23)KeyHandle.Pm_State.On_alldate.hms.hour = 0;
          }  
          else if(KeyHandle.Pm_State.Pm_Num == 6)
          {
            KeyHandle.Pm_State.On_alldate.hms.min ++;
            if(KeyHandle.Pm_State.On_alldate.hms.min > 59)KeyHandle.Pm_State.On_alldate.hms.min = 0;
          }
          else if(KeyHandle.Pm_State.Pm_Num == 7)
          {
            KeyHandle.Pm_State.Off_alldate.md.date ++;
            if(KeyHandle.Pm_State.Off_alldate.md.date > 30)KeyHandle.Pm_State.Off_alldate.md.date = 0;
          }
          else if(KeyHandle.Pm_State.Pm_Num == 8)
          {
            KeyHandle.Pm_State.Off_alldate.hms.hour ++;
            if(KeyHandle.Pm_State.Off_alldate.hms.hour > 23)KeyHandle.Pm_State.Off_alldate.hms.hour = 0;
          }  
          else if(KeyHandle.Pm_State.Pm_Num == 9)
          {
            KeyHandle.Pm_State.Off_alldate.hms.min ++;
            if(KeyHandle.Pm_State.Off_alldate.hms.min > 59)KeyHandle.Pm_State.Off_alldate.hms.min = 0;
          }            
        }
        
        
      break;
      case S6_DOWN_VALUE:      
      if(KeyHandle.Oper_Mode_Dis_State == 0)
      {
        if (KeyHandle.Fan_Seepd_Max_State  == 1)
        {
          Peripheral_A11_Max --;  
          if (Peripheral_A11_Max < 1) Peripheral_A11_Max = 999;
        }
        else if(KeyHandle.HEAP_State == 1)
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
        else if(KeyHandle.Time_State == 1)
        {
          KeyHandle.AllDate=ds1302_readTime();
          KeyHandle.AllDate.md.month--;
          if(KeyHandle.AllDate.md.month < 1)KeyHandle.AllDate.md.month = 12;
          ds1302_setTime(KeyHandle.AllDate);
        }
        else if(KeyHandle.Time_State == 2)
        {
          KeyHandle.AllDate=ds1302_readTime();
          KeyHandle.AllDate.md.date--;
          if(KeyHandle.AllDate.md.date > 254)KeyHandle.AllDate.md.date = 30;
          ds1302_setTime(KeyHandle.AllDate);
        }
        else if(KeyHandle.Time_State == 3)
        {
          KeyHandle.AllDate=ds1302_readTime();
          KeyHandle.AllDate.hms.hour--;
          if(KeyHandle.AllDate.hms.hour > 254)KeyHandle.AllDate.hms.hour = 23;
          ds1302_setTime(KeyHandle.AllDate);
        }
        else if(KeyHandle.Time_State == 4)
        {
          KeyHandle.AllDate=ds1302_readTime();
          KeyHandle.AllDate.hms.min--;
          if(KeyHandle.AllDate.hms.min > 254)KeyHandle.AllDate.hms.min = 59;
          ds1302_setTime(KeyHandle.AllDate);
        }  
        else if(KeyHandle.Time_State == 5)
        {
          KeyHandle.AllDate=ds1302_readTime();
          KeyHandle.AllDate.yd.day--;
          if(KeyHandle.AllDate.yd.day > 254)KeyHandle.AllDate.yd.day = 7;
          ds1302_setTime(KeyHandle.AllDate);
        }  
      }
      else if (KeyHandle.Oper_Mode_Dis_State == 2)
      {
         if(KeyHandle.Pm_State.Pm_Num == 4)
        {
          KeyHandle.Pm_State.On_alldate.md.date --;
          if(KeyHandle.Pm_State.On_alldate.md.date > 99)KeyHandle.Pm_State.On_alldate.md.date = 0;
        }
        else if(KeyHandle.Pm_State.Pm_Num == 5)
        {
          KeyHandle.Pm_State.On_alldate.hms.hour --;
          if(KeyHandle.Pm_State.On_alldate.hms.hour > 99)KeyHandle.Pm_State.On_alldate.hms.hour = 0;
        }  
        else if(KeyHandle.Pm_State.Pm_Num == 6)
        {
          KeyHandle.Pm_State.On_alldate.hms.min --;
          if(KeyHandle.Pm_State.On_alldate.hms.min > 99)KeyHandle.Pm_State.On_alldate.hms.min = 0;
        }
        else if(KeyHandle.Pm_State.Pm_Num == 7)
        {
          KeyHandle.Pm_State.Off_alldate.md.date --;
          if(KeyHandle.Pm_State.Off_alldate.md.date > 99)KeyHandle.Pm_State.Off_alldate.md.date = 0;     
          
        }
        else if(KeyHandle.Pm_State.Pm_Num == 8)
        {
          KeyHandle.Pm_State.Off_alldate.hms.hour --;
          if(KeyHandle.Pm_State.Off_alldate.hms.hour > 99)KeyHandle.Pm_State.Off_alldate.hms.hour = 0;
        }  
        else if(KeyHandle.Pm_State.Pm_Num == 9)
        {
          KeyHandle.Pm_State.Off_alldate.hms.min --;
          if(KeyHandle.Pm_State.Off_alldate.hms.min > 99)KeyHandle.Pm_State.Off_alldate.hms.min = 59;
        }         
      }
      break;
      case S7_DOWN_VALUE: 
      KeyHandle.Wm_Num++;
      if(KeyHandle.Wm_Num > 16)
      KeyHandle.Wm_Num = 0;
      if(KeyHandle.Wm_Num == 0)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;
        KeyHandle.Oper_Mode_State = 0;
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0; 
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 1)
      {
        KeyHandle.Fan_Seepd_Max_State = 1;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;   
        KeyHandle.Oper_Mode_State = 0;
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0; 
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 2)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 1;
        KeyHandle.HEAP_Dis_State = 0;   
        KeyHandle.Oper_Mode_State = 0;
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;  
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 3)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 1;
        KeyHandle.HEAP_Dis_State = 1;     
        KeyHandle.Oper_Mode_State = 0;
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;  
        KeyHandle.Time_State = 0;
      }      
      else if(KeyHandle.Wm_Num == 4)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 0;   
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 1;
      }
      else if(KeyHandle.Wm_Num == 5)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 0;   
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 2;
      }
      else if(KeyHandle.Wm_Num == 6)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 0;   
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 3;
      }      
      else if(KeyHandle.Wm_Num == 7)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 0;   
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 4;
      }
      else if(KeyHandle.Wm_Num == 8)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 0;   
        KeyHandle.Oper_Mode_Dis_State = 0;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 5;
      }      
      else if(KeyHandle.Wm_Num == 9)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 1;
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 10)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 0;
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 11)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 4;
        KeyHandle.Time_State = 0;
      }      
      else if(KeyHandle.Wm_Num == 12)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 5;  
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 13)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 6; 
        KeyHandle.Time_State = 0;
      }      
      else if(KeyHandle.Wm_Num == 14)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 7;  
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 15)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 8;   
        KeyHandle.Time_State = 0;
      }
      else if(KeyHandle.Wm_Num == 16)
      {
        KeyHandle.Fan_Seepd_Max_State = 0;
        KeyHandle.HEAP_State = 0;
        KeyHandle.HEAP_Dis_State = 0;     
        KeyHandle.Oper_Mode_State = 1;   
        KeyHandle.Oper_Mode_Dis_State = 2;
        ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        KeyHandle.Pm_State.Pm_Num = 9;  
        KeyHandle.Time_State = 0;
      }  
          
//      else if(KeyHandle.Wm_Num == 12)
      break;
      case S8_DOWN_VALUE:  
        if(KeyHandle.Wm_Num > 8) 
        {
          KeyHandle.Oper_Mode_State = 0;
          Door_Move_time = 0;
        }
      break;      
      default : 
      break;
  }
}
