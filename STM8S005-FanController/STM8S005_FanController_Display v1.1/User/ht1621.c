#include "ht1621.h"
extern u8 Dis_Bling;
extern u8 Dis_Err_Bling;
extern u8 Dis_Door_Err_Bling;
u16 Peripheral_A11_Max = 100;
u8 Pm_Time = 0;
u16 hepa_time;
extern u8 Dis_Door_Bling;
extern u8 Dis_Time;
u16 Door_Move_time;
u8 alldate_Updata;
struct KEYHANDLE KeyHandle_Door;
//1 ~ 11
const unsigned char Dis_Digitron_Addr[] = 
{
  0XFF,0x00,0x02,0x04,0x06,0X08,0X0A,0X0C,0X0E,0X15,0x17,0x19,
  0x1A,0X18,0x16,0x14,0X02,0X04,0X06,0x08,0X0A,0x0C,0x0E,0X10,0X12
};
//12 ~ 24
/*
const unsigned char Cs2_Dis_Digitron_Addr[] = 
{
  0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
  0x1A,0X18,0x16,0x14,0X02,0X04,0X06,0x08,0X0A,0x0C,0x0E,0X10,0X12
};
*/
//12~15
//
const unsigned char Cs1_Dis_Digitron_Num[] = 
{   
    0xf5, 0X05, 0Xd3, 0X97, 0X27, 0XB6, 0Xf6, 0X15, 0Xf7, 0Xb7 
};
const unsigned char Cs2_12_15_Dis_Digitron_Num[] = 
{   
    0xAF,0xA0,0xCB,0xE9,0xE4,0x6D,0x6F,0xA8,0xEF,0xED
};
const unsigned char Cs2_16_19_Dis_Digitron_Num[] =
{
    0xF5,0x05,0xB6,0x97,0x47,0xD3,0xF3,0x85,0xF7,0xD7
};
const unsigned char Cs2_20_24_Dis_Digitron_Num[] =
{
    0xFA,0x0A,0xD6,0x9E,0x2E,0xBC,0xFC,0x1A,0xFE,0xBE
};

const unsigned char T_Addr[] =
{
    0x00,0x04,0x0e,0x10,0x10,0x12,0x12,0x12,0x12,0x10,0x10,0x10,0x10,0x12,0x12,0x12,0x12,0x14,0x14,0x14,0x19,
    0x14,0X00,0X00,0X00,0X00,0X00,0X00,0X1A,0X00,0X00,0X10,0X0C,0X0E  
};
const unsigned char T_Mask[] =
{
    0x00,0x08,0x08,0x80,0x02,0x80,0x40,0x20,0x10,0x01,0x40,0x20,0x10,0x08,0x04,0x02,0x01,0x20,0x40,0x10,0x08,
    0X10,0X20,0X40,0X80,0X08,0X04,0X02,0X10,0X10,0X01,0X01,0X01,0X01 
};

const unsigned char S_Addr[] = {0X00,0x10,0x10};
const unsigned char S_Mask[] = {0X00,0x08,0x04};

const unsigned char P_Addr[] = {0X00,0X00,0x02,0X06,0X08,0X0C,0X15,0X17};
const unsigned char P_Mask[] = {0X00,0x08,0X08,0X08,0X08,0X08,0X08,0X08};

const unsigned char Col_Addr[] = {0x00,0x04};
const unsigned char Col_Mask[] = {0x00,0X08};

unsigned char cs1_ram_map[33] = 
{
	0
};
unsigned char cs2_ram_map[33] = 
{
	0
};
unsigned char cs1_Digitron_ram_map[33] = 
{
	0
};
unsigned char cs2_Digitron_ram_map[33] = 
{
	0
};
void ht1621_send_high_order_data(u8 data, u8 len)
{
	u8 i;
 
	for (i=0; i<len; i++)
	{
                
		if((data&0x80) == 0)
		{
			Set_Port_Val(DATA_PIN, 0);	
		}
		else
		{
			Set_Port_Val(DATA_PIN, 1);
		}
                
		Set_Port_Val(WR_PIN, 0);
		Delay_Us(DELAY_US_NUM);
		Set_Port_Val(WR_PIN, 1);	
		
		data <<= 1;
	}
}


void ht1621_send_low_order_data(u8 data, u8 len)
{
	u8 i;
	
	for (i=0; i<len; i++)
	{
                
		if((data&0x01) == 0)
		{
			Set_Port_Val(DATA_PIN, 0);	
		}
		else
		{
			Set_Port_Val(DATA_PIN, 1);
		}
                
		Set_Port_Val(WR_PIN, 0);
		Delay_Us(DELAY_US_NUM);
		Set_Port_Val(WR_PIN, 1);	
		
		data >>= 1;
	}
}


void ht1621_send_cmd(u8 command)
{
	Set_Port_Val(CS_1_PIN, 0);
        Set_Port_Val(CS_2_PIN, 0);
	ht1621_send_high_order_data(0x80, 4);
	ht1621_send_high_order_data(command, 8);
        Set_Port_Val(CS_2_PIN, 1);
	Set_Port_Val(CS_1_PIN, 1);	
}
 
void ht1621_write(u8 cs ,u8 addr, u8 data)
{
  
  if(cs==1)
  { 
	Set_Port_Val(CS_1_PIN, 0);
	ht1621_send_high_order_data(0xA0, 3);
	ht1621_send_high_order_data(addr<<2, 6);
        if(addr == 0x14)
          ht1621_send_high_order_data(data, 4);
        else
          ht1621_send_high_order_data(data, 8);
	Set_Port_Val(CS_1_PIN, 1);
  }
  else if(cs==2)
  {     
        Set_Port_Val(CS_2_PIN, 0);
        ht1621_send_high_order_data(0xA0, 3);
        ht1621_send_high_order_data(addr<<2, 6);
        ht1621_send_high_order_data(data, 8);
        Set_Port_Val(CS_2_PIN, 1);  
  }      
}
void ht1621_Char_write1(u8 cs ,u8 addr, u8 data ,u8 status,u8 Char)
{
 // u8 cs_1=cs;
  u8 t_data;
  u8 t2_data;
  if(cs == 1)
  {
	  t_data=cs1_ram_map[addr];
	if(Char)
	{
          t2_data = cs1_Digitron_ram_map[addr];
          cs1_Digitron_ram_map[addr] = data;
          t_data &= ~ t2_data;
	}	  
  }
  else if (cs == 2)
  {
    t_data=cs2_ram_map[addr];
    if(Char)
    {
      t2_data = cs2_Digitron_ram_map[addr];
      cs2_Digitron_ram_map[addr] = data;
      t_data &= ~ t2_data;
    }
  }
  if(status)t_data |= data;
  else t_data &= ~data;
  
  if(cs == 1)
  {
    cs1_ram_map[addr] = t_data;
  }
  else if (cs == 2)
  { 
    cs2_ram_map[addr] = t_data; 
  }
  ht1621_write(cs,addr,t_data);
}
void ht1621_Char_write(u8 cs ,u8 addr, u8 data ,u8 status)
{
 // u8 cs_1=cs;
  u8 t_data;

  if(cs == 1)t_data=cs1_ram_map[addr];
  else if (cs == 2)t_data=cs2_ram_map[addr];
  if(cs == 2)
  {
    t_data &= ~cs2_Digitron_ram_map[addr];
  }
  
  if(status)t_data |= data;
  else t_data &= ~data;
  
  if(cs == 1)
  {
    cs1_ram_map[addr] = t_data;
  }
  else if (cs == 2)
  { 
    cs2_ram_map[addr] = t_data; 
  }
  ht1621_write(cs,addr,t_data);
}
/*
void ht1621_Digitron_write(u8 cs ,u8 addr, u8 data)
{
  u8 t_data;
  if(cs == 1)t_data=cs1_ram_map[addr];
  else if (cs == 2)t_data=cs2_ram_map[addr];  
  
  
  
}
*/
void ht1621_Clear()
{
  u8 i;
  for(i=0;i<50;i++)ht1621_write(1,i,0);
  for(i=0;i<50;i++)ht1621_write(2,i,0);
  for(i=0;i<33;i++)cs2_ram_map[i] = 0;
  for(i=0;i<33;i++)cs1_ram_map[i] = 0;
}
void ht1621_Full()
{
  u8 i;
  for(i=0;i<50;i++)ht1621_write(1,i,0XFF);
  for(i=0;i<50;i++)ht1621_write(2,i,0XFF);
}
void ht1621_char_display()
{
    ht1621_Char_write1(1,T_Addr[1],T_Mask[1],1,0);
    ht1621_Char_write1(1,T_Addr[3],T_Mask[3],1,0);
    ht1621_Char_write1(1,T_Addr[4],T_Mask[4],0,0); 
    ht1621_Char_write1(1,T_Addr[19],T_Mask[19],1,0);
    ht1621_Char_write1(1,T_Addr[20],T_Mask[20],1,0);
    ht1621_Char_write1(2,T_Addr[21],T_Mask[21],1,0);
    ht1621_Char_write1(2,T_Addr[31],T_Mask[31],1,0);  
//    ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
//    ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);  
} 
void ht1621_init()
{
        u8 i;
	GPIO_Init(LCD_PROT,CS_1_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
        GPIO_Init(LCD_PROT,RD_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
        GPIO_Init(LCD_PROT,WR_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
        GPIO_Init(LCD_PROT,DATA_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
        GPIO_Init(LCD_PROT,CS_2_PIN,GPIO_MODE_OUT_PP_LOW_SLOW);
        ht1621_send_cmd(HT_SYS_DIS_EN);
        ht1621_send_cmd(HT_SYS_EN);
	ht1621_send_cmd(HT_RCOSC);
	ht1621_send_cmd(HT_BISA_COM);
	ht1621_send_cmd(HT_LCD_ON);      
        ht1621_Clear();
#if 0        
         
        for(i=CS1_DIGITRON_START;i<CS1_DIGITRON_END+1;i++)
          ht1621_write(1,Dis_Digitron_Addr[i],Cs1_Dis_Digitron_Num[3]);
        
        
        for(i=CS1_S_START;i<CS1_S_END+1;i++)
          ht1621_Char_write1(1,S_Addr[i],S_Mask[i],1);
        
        
        for(i=CS1_P_START;i<CS1_P_END+1;i++)
          ht1621_Char_write1(1,P_Addr[i],P_Mask[i],1);
        
        
        for(i=CS1_T_START;i<CS1_T_END+1;i++)
          ht1621_Char_write1(1,T_Addr[i],T_Mask[i],1);   
        
        
        for(i=CS2_12_15_DIGITRON_START;i<CS2_12_15_DIGITRON_END+1;i++)
          ht1621_write(2,Dis_Digitron_Addr[i],Cs2_12_15_Dis_Digitron_Num[1]);

        for(i=CS2_16_19_DIGITRON_START;i<CS2_16_19_DIGITRON_END+1;i++)
          ht1621_write(2,Dis_Digitron_Addr[i],Cs2_16_19_Dis_Digitron_Num[1]);

        for(i=CS2_20_24_DIGITRON_START;i<CS2_20_24_DIGITRON_END+1;i++)
          ht1621_write(2,Dis_Digitron_Addr[i],Cs2_20_24_Dis_Digitron_Num[1]);
        
        for(i=CS2_T_START;i<CS2_T_END+1;i++)        
          ht1621_Char_write1(2,T_Addr[i],T_Mask[i],1);
        
#endif    
        //ht1621_Char_write1(2,0x14,0X10,1);
         //ht1621_write(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[1]);
         //ht1621_Char_write1(2,T_Addr[21],T_Mask[21],1);
        //ht1621_write(2,Cs2_Dis_Digitron_Addr[12],Cs2_Dis_Digitron_Num[8]);
        //ht1621_write(1,Cs1_Dis_Digitron_Addr[15],Cs1_Dis_Digitron_Num[2]);
        //ht1621_write(1,Dis_Digitron_Addr[3],Cs1_Dis_Digitron_Num[1]);
        
}

void set_lcd_on()
{
	ht1621_send_cmd(HT_LCD_ON);
}
 
void set_lcd_off()
{
	ht1621_send_cmd(HT_LCD_OFF);
}
void Peripheral_Rceive_Display(struct Peripheral peripheral,u8 Fan_Seepd_Max_State)
{
   if(Fan_Seepd_Max_State == 0)
   {
		ht1621_Char_write1(1,Dis_Digitron_Addr[1],Cs1_Dis_Digitron_Num[peripheral.a11/100],1,1);
		ht1621_Char_write1(1,Dis_Digitron_Addr[2],Cs1_Dis_Digitron_Num[peripheral.a11%100/10],1,1);
	   	ht1621_Char_write1(1,P_Addr[2],P_Mask[2],1,0);
		ht1621_Char_write1(1,Dis_Digitron_Addr[3],Cs1_Dis_Digitron_Num[peripheral.a11%10],1,1);
   }
   else
   {
     if(Dis_Bling)
     {
		ht1621_Char_write1(1,Dis_Digitron_Addr[1],Cs1_Dis_Digitron_Num[8],0,1);
		ht1621_Char_write1(1,Dis_Digitron_Addr[2],Cs1_Dis_Digitron_Num[8],0,1);
	     	ht1621_Char_write1(1,P_Addr[2],P_Mask[2],0,0);
		ht1621_Char_write1(1,Dis_Digitron_Addr[3],Cs1_Dis_Digitron_Num[8],0,1); 
		//Dis_Bling = 0;
     }
     else
     {
		ht1621_Char_write1(1,Dis_Digitron_Addr[1],Cs1_Dis_Digitron_Num[Peripheral_A11_Max/100],1,1);
		ht1621_Char_write1(1,Dis_Digitron_Addr[2],Cs1_Dis_Digitron_Num[Peripheral_A11_Max%100/10],1,1);
	     	ht1621_Char_write1(1,P_Addr[2],P_Mask[2],1,0);
		ht1621_Char_write1(1,Dis_Digitron_Addr[3],Cs1_Dis_Digitron_Num[Peripheral_A11_Max%10],1,1); 
     }
   }
	ht1621_Char_write1(1,Dis_Digitron_Addr[4],Cs1_Dis_Digitron_Num[peripheral.a12/100],1,1);
	ht1621_Char_write1(1,Dis_Digitron_Addr[5],Cs1_Dis_Digitron_Num[peripheral.a12%100/10],1,1);
	ht1621_Char_write1(1,P_Addr[4],P_Mask[4],1,0);
	ht1621_Char_write1(1,Dis_Digitron_Addr[6],Cs1_Dis_Digitron_Num[peripheral.a12%10],1,1);
   
	ht1621_Char_write1(1,Dis_Digitron_Addr[7],Cs1_Dis_Digitron_Num[peripheral.a13/10],1,1);
	ht1621_Char_write1(1,Dis_Digitron_Addr[8],Cs1_Dis_Digitron_Num[peripheral.a13%10],1,1);
        
      
      if(peripheral.Dp == 1) ht1621_Char_write1(1,T_Addr[14],T_Mask[14],0,0);
      else if (peripheral.Dp == 0) ht1621_Char_write1(1,T_Addr[14],T_Mask[14],1,0);
      
      if(peripheral.Fr == 1) ht1621_Char_write1(1,T_Addr[13],T_Mask[13],0,0);
      else if (peripheral.Fr == 0) ht1621_Char_write1(1,T_Addr[13],T_Mask[13],1,0);  
}
void Hepa_Set_Display(struct Hepa hepa,struct KEYHANDLE KeyHandle,struct Peripheral peripheral)
{
  
   if(KeyHandle.HEAP_State == 0)
   {
      ht1621_Char_write1(1,Dis_Digitron_Addr[9],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd/100],1,1);
      ht1621_Char_write1(1,Dis_Digitron_Addr[10],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%100/10],1,1);
      ht1621_Char_write1(1,Dis_Digitron_Addr[11],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%10],1,1);   
      ht1621_Char_write1(2,Dis_Digitron_Addr[12],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time/1000],1,1);
      ht1621_Char_write1(2,Dis_Digitron_Addr[13],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%1000/100],1,1);
      ht1621_Char_write1(2,Dis_Digitron_Addr[14],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%100/10],1,1); 
      ht1621_Char_write1(2,Dis_Digitron_Addr[15],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%10],1,1);  
      //if(hepa.Fan_Seepd > )          
   }
   else
   {
     if(KeyHandle.HEAP_Dis_State == 1)
     {
        if(Dis_Bling == 0)
        {
			ht1621_Char_write1(1,Dis_Digitron_Addr[9],Cs1_Dis_Digitron_Num[8],0,1);
			ht1621_Char_write1(1,Dis_Digitron_Addr[10],Cs1_Dis_Digitron_Num[8],0,1);
			ht1621_Char_write1(1,Dis_Digitron_Addr[11],Cs1_Dis_Digitron_Num[8],0,1);
			ht1621_Char_write1(1,P_Addr[7],P_Mask[7],0,0);
        }
        else
        {
		ht1621_Char_write1(1,Dis_Digitron_Addr[9],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd/100],1,1);
		ht1621_Char_write1(1,Dis_Digitron_Addr[10],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%100/10],1,1);
		ht1621_Char_write1(1,P_Addr[7],P_Mask[7],1,0);
		ht1621_Char_write1(1,Dis_Digitron_Addr[11],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%10],1,1);  
          //Dis_Bling =0;
        }
          ht1621_Char_write1(2,Dis_Digitron_Addr[12],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time/1000],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[13],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%1000/100],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[14],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%100/10],1,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[15],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%10],1,1);          
     }
     else
     {
        ht1621_Char_write1(1,Dis_Digitron_Addr[9],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd/100],1,1);
        ht1621_Char_write1(1,Dis_Digitron_Addr[10],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%100/10],1,1);
	ht1621_Char_write1(1,P_Addr[7],P_Mask[7],1,0);
        ht1621_Char_write1(1,Dis_Digitron_Addr[11],Cs1_Dis_Digitron_Num[hepa.Fan_Seepd%10],1,1);  
        if(Dis_Bling == 0)
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[12],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time/1000],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[13],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%1000/100],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[14],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%100/10],1,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[15],Cs2_12_15_Dis_Digitron_Num[hepa.Work_Time%10],1,1);              
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[12],Cs2_12_15_Dis_Digitron_Num[8],0,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[13],Cs2_12_15_Dis_Digitron_Num[8],0,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[14],Cs2_12_15_Dis_Digitron_Num[8],0,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[15],Cs2_12_15_Dis_Digitron_Num[8],0,1); 
         // Dis_Bling =0;
        }
     }
     
   }
  if((hepa.Fan_Seepd > peripheral.a11)||(hepa.Work_Time < hepa_time)) 
  {
    if(Dis_Err_Bling)
    {
      ht1621_Char_write1(1,T_Addr[15],T_Mask[15],1,0);
      ht1621_Char_write1(1,T_Addr[2],T_Mask[2],1,0);
      //Dis_Err_Bling = 0;
    }
    else
    {
      ht1621_Char_write1(1,T_Addr[15],T_Mask[15],0,0);
      ht1621_Char_write1(1,T_Addr[2],T_Mask[2],0,0);      
    }
  }
  else 
  {
    ht1621_Char_write1(1,T_Addr[15],T_Mask[15],0,0);
    ht1621_Char_write1(1,T_Addr[2],T_Mask[2],0,0);
  }
}
void Oper_Mode_Disply(u8 Oper_Mode_State,u8 Oper_Mode_Dis_State)
{
  if(Oper_Mode_State)
  {
    if(Oper_Mode_Dis_State == 1)
    {
      if(Dis_Bling ==1)
      {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],1,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
        //Dis_Bling = 0;
      }
      else
      {
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
      }
      
    }
    else if(Oper_Mode_Dis_State == 2)
    {
      if(Dis_Bling ==1)
      {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],1,0);
       // Dis_Bling = 0;
      }
      else
      {
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
      }
    }
    else
    {
      if(Dis_Bling ==1)
      {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],1,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0); 
        //Dis_Bling = 0;
      }
      else
      {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
      } 
    }
  }
  else
  {
    if(Oper_Mode_Dis_State == 1)
    {

        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],1,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0);
      
    }
    else if(Oper_Mode_Dis_State == 2)
    {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],0,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],1,0);
    }
    else
    {
        ht1621_Char_write1(1,T_Addr[10],T_Mask[10],1,0);
        ht1621_Char_write1(1,T_Addr[11],T_Mask[11],0,0);
        ht1621_Char_write1(1,T_Addr[12],T_Mask[12],0,0); 
    }
  }
  
  
}


void Week_Display(u8 date)
{
    switch(date)
    {
      case 1:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],1,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;
      case 2:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],1,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;
      case 3:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],1,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;
      case 4:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],1,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;              
      case 5:             
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],1,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;              
      case 6:             
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],1,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;
      case 7:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],1,0);
      break;  
      case 8:
        ht1621_Char_write1(2,T_Addr[22],T_Mask[22],0,0);
        ht1621_Char_write1(2,T_Addr[23],T_Mask[23],0,0);
        ht1621_Char_write1(2,T_Addr[24],T_Mask[24],0,0);
        ht1621_Char_write1(2,T_Addr[25],T_Mask[25],0,0);
        ht1621_Char_write1(2,T_Addr[26],T_Mask[26],0,0);
        ht1621_Char_write1(2,T_Addr[27],T_Mask[27],0,0);
        ht1621_Char_write1(2,T_Addr[28],T_Mask[28],0,0);
      break;
      default:
      break;
    }  
}

void Now_Time_Display(struct ALLDATE alldate , struct KEYHANDLE KeyHandl)
{
    if(KeyHandle.Oper_Mode_Dis_State == 2)
    {
        if(KeyHandle.Oper_Mode_State == 1)
        {
          if(alldate_Updata == 1)
          {
            KeyHandle.Pm_State.On_alldate = alldate;
            KeyHandle.Pm_State.Off_alldate = alldate;
            KeyHandle.Pm_State.Off_alldate.hms.min = alldate.hms.min+1;
            alldate_Updata = 0;
          }
          if(KeyHandle.Pm_State.Pm_Num > 6)
          {
            ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.hms.hour/10],1,1);
            ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.hms.hour%10],1,1);

            ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.hms.min/10],1,1); 
            ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.hms.min%10],1,1); 
                 
            ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.md.date/10],1,1);
            ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[KeyHandl.Pm_State.Off_alldate.md.date%10],1,1);
            
            ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
            ht1621_Char_write1(2,T_Addr[30],T_Mask[30],1,0);  
          }
          else if(KeyHandle.Pm_State.Pm_Num > 3)
          {
            ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.hms.hour/10],1,1);
            ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.hms.hour%10],1,1);

            ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.hms.min/10],1,1); 
            ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.hms.min%10],1,1); 
                 
            ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.md.date/10],1,1);
            ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[KeyHandl.Pm_State.On_alldate.md.date%10],1,1);
            ht1621_Char_write1(2,T_Addr[29],T_Mask[29],1,0);
            ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0); 
          }  
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

          ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 

          if(alldate.hms.sec%2)
          {
            ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
          }
          else
          {
            ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
          } 
          
          ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
          ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
          ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
          ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 

          if(alldate.md.month > 9) 
          {  
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
          }
          else
          {
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
          }       
          Week_Display(alldate.yd.day);   
      }
      ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
      ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
      
      ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
      
      if(alldate.md.month > 9) 
      {  
        ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
        ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
      }
      else
      {
        ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
        ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
      }       
      Week_Display(alldate.yd.day);
    }
    else if(KeyHandle.Oper_Mode_Dis_State == 0)
    {
        alldate_Updata = 1;
        if(KeyHandle.Time_State == 1)
        {
        ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

        ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
        ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
        
        if(alldate.hms.sec%2)
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
        }
        else
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
        }    
        ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
        ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
        ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);        
        if(Dis_Time)
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
          
          if(alldate.md.month > 9) 
          {  
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
          }
          else
          {
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
          }  
          //Dis_Time = 0;
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],0,1); 
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],0,0);                             
        }
        
      }
      else if(KeyHandle.Time_State == 2)
      {
        if(Dis_Time)
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
          //Dis_Time = 0;
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],0,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],0,1);                        
        } 
        ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

        ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
        ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
        
        if(alldate.hms.sec%2)
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
        }
        else
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
        }    
//        ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
//        ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
        ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
        ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
        
        if(alldate.md.month > 9) 
        {  
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
        }
        else
        {
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
        }       
        Week_Display(alldate.yd.day);           
        
      }
      else if(KeyHandle.Time_State == 3)
      {
        if(Dis_Time)
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);
          //Dis_Time = 0;
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],0,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],0,1);                      
        }  
//        ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
//        ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

        ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
        ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
        
        if(alldate.hms.sec%2)
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
        }
        else
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
        }    
        ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
        ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
        ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
        
        if(alldate.md.month > 9) 
        {  
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
        }
        else
        {
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
        }       
        Week_Display(alldate.yd.day);              
      }
      else if(KeyHandle.Time_State == 4)
      {
        if(Dis_Time)
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
          //Dis_Time = 0;
        }
        else
        {
          ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],0,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],0,1);                      
        }        
        ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

//        ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
//        ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
        
        if(alldate.hms.sec%2)
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
        }
        else
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
        }    
        ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
        ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
        ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
        
        if(alldate.md.month > 9) 
        {  
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
        }
        else
        {
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
        }       
        Week_Display(alldate.yd.day);          
      }
      else if(KeyHandle.Time_State == 5)
      {
        if(Dis_Time)
        {
          Week_Display(8); 
          //Dis_Time = 0;
        }
        else
        {
          Week_Display(alldate.yd.day);                    
        }           
          ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

          ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
          ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 

          if(alldate.hms.sec%2)
          {
            ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
          }
          else
          {
            ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
          }    
          ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
          ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
          ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);

          ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
          ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);

          ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 

          if(alldate.md.month > 9) 
          {  
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
          }
          else
          {
            ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
            ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
          }                
      }
      else
      {
        ht1621_Char_write1(2,Dis_Digitron_Addr[16],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[17],Cs2_16_19_Dis_Digitron_Num[alldate.hms.hour%10],1,1);

        ht1621_Char_write1(2,Dis_Digitron_Addr[18],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min/10],1,1); 
        ht1621_Char_write1(2,Dis_Digitron_Addr[19],Cs2_16_19_Dis_Digitron_Num[alldate.hms.min%10],1,1); 
        
        if(alldate.hms.sec%2)
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],0,0);
        }
        else
        {
          ht1621_Char_write1(2,Col_Addr[1],Col_Mask[1],1,0);
        }    
        ht1621_Char_write1(2,Dis_Digitron_Addr[23],Cs2_20_24_Dis_Digitron_Num[alldate.md.date/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[24],Cs2_20_24_Dis_Digitron_Num[alldate.md.date%10],1,1); 
        ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
        ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[20],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year/10],1,1);
        ht1621_Char_write1(2,Dis_Digitron_Addr[21],Cs2_20_24_Dis_Digitron_Num[alldate.yd.year%10],1,1);
        
        ht1621_Char_write1(2,Dis_Digitron_Addr[22],Cs2_20_24_Dis_Digitron_Num[alldate.md.month%10],1,1); 
        
        if(alldate.md.month > 9) 
        {  
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],0,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);
        }
        else
        {
          ht1621_Char_write1(2,T_Addr[32],T_Mask[32],1,0);
          ht1621_Char_write1(2,T_Addr[33],T_Mask[33],1,0);      
        }       
        Week_Display(alldate.yd.day);  
      }
    }
    

}
/*
void Fan_Speed_State_Display(struct KEYHANDLE KeyHandle)
{

  if(KeyHandle.Fan_Seepd_Max_State == 1)
  {




  } 
}
*/
void Fan_Speed_State_Display(u8 Fan_Seepd_State)
{
  switch(Fan_Seepd_State)
  {
    case FAN_OFF_STATE:
      ht1621_Char_write1(1,T_Addr[5],T_Mask[5],0,0);
      ht1621_Char_write1(1,T_Addr[6],T_Mask[6],0,0);
      ht1621_Char_write1(1,T_Addr[7],T_Mask[7],0,0);
      ht1621_Char_write1(1,S_Addr[1],S_Mask[1],0,0);
      ht1621_Char_write1(1,S_Addr[2],S_Mask[2],0,0);
      break;
    case FAN_W_STATE:
      ht1621_Char_write1(1,T_Addr[5],T_Mask[5],1,0);
      ht1621_Char_write1(1,T_Addr[6],T_Mask[6],0,0);
      ht1621_Char_write1(1,T_Addr[7],T_Mask[7],0,0);
      ht1621_Char_write1(1,S_Addr[1],S_Mask[1],1,0);
      ht1621_Char_write1(1,S_Addr[2],S_Mask[2],0,0);            
      break;
    case FAN_M_STATE:
      ht1621_Char_write1(1,T_Addr[5],T_Mask[5],1,0);
      ht1621_Char_write1(1,T_Addr[6],T_Mask[6],1,0);
      ht1621_Char_write1(1,T_Addr[7],T_Mask[7],0,0);
      ht1621_Char_write1(1,S_Addr[1],S_Mask[1],1,0);
      ht1621_Char_write1(1,S_Addr[2],S_Mask[2],1,0);
      break;
    case FAN_H_STATE:
      ht1621_Char_write1(1,T_Addr[5],T_Mask[5],1,0);
      ht1621_Char_write1(1,T_Addr[6],T_Mask[6],1,0);
      ht1621_Char_write1(1,T_Addr[7],T_Mask[7],1,0);
      ht1621_Char_write1(1,S_Addr[1],S_Mask[1],1,0);
      ht1621_Char_write1(1,S_Addr[2],S_Mask[2],1,0);
      break;
    default: break;
  }  
}
void Door_State_Display(u8 Door_State,struct Peripheral peripheral)
{
    switch(Door_State)
    {
      case DOOR_UP_STATE:
        if(Door_Move_time < 10)
        {
          if(peripheral.Door_Up)
          {
            if(Dis_Door_Bling)
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],1,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0); 
            }
            else
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0);               
            }
          }
          else //检测到反馈
          {
            
            ht1621_Char_write1(1,T_Addr[16],T_Mask[16],1,0);
            ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0); 
            KeyHandle_Door = KeyHandle;
            KeyHandle_Door.Door_State = 3;
            KeyHandle_Door.Od_State.Door_State = 3;
            KeyHandle_Door.Pm_State.Door_State = 3;
            Uart_Transmit_Hnadle(KeyHandle_Door);
            Door_Move_time = 0;
          }
          ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        }
        else
        {
          KeyHandle_Door = KeyHandle;
          KeyHandle_Door.Door_State = 3;
          KeyHandle_Door.Od_State.Door_State = 3;
          KeyHandle_Door.Pm_State.Door_State = 3;
          Uart_Transmit_Hnadle(KeyHandle_Door);          
          if(peripheral.Door_Up)
          {
            //Dis_Door_Err_Bling = 0;
            if(Dis_Door_Bling)
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],1,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0); 
            }
            else
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0);               
            }            
            if(Dis_Door_Err_Bling)
            {
              ht1621_Char_write1(1,T_Addr[18],T_Mask[18],1,0); 
              //Dis_Door_Err_Bling = 0;
            }
            else
            {             
              ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
            }   
          }
          else
          {
           // Dis_Door_Err_Bling = 1;
            
            Door_Move_time = 0;
            ht1621_Char_write1(1,T_Addr[18],T_Mask[18],1,0);
          }
        }         
      break;
      case DOOR_DO_STATE:
        if(Door_Move_time < 10)
        {
          if(peripheral.Door_Do)
          {
            if(Dis_Door_Bling)
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],1,0); 
            }
            else
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0);               
            }
          }
          else //检测到反馈
          {
            
            ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
            ht1621_Char_write1(1,T_Addr[17],T_Mask[17],1,0); 
            KeyHandle_Door = KeyHandle;
            KeyHandle_Door.Door_State = 3;
            KeyHandle_Door.Od_State.Door_State = 3;
            KeyHandle_Door.Pm_State.Door_State = 3;
            Uart_Transmit_Hnadle(KeyHandle_Door);
            Door_Move_time = 0;
          }
          ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
        }
        else
        {
          KeyHandle_Door = KeyHandle;
          KeyHandle_Door.Door_State = 3;
          KeyHandle_Door.Od_State.Door_State = 3;
          KeyHandle_Door.Pm_State.Door_State = 3;
          Uart_Transmit_Hnadle(KeyHandle_Door);          
          if(peripheral.Door_Do)
          {
            //Dis_Door_Err_Bling = 0;
            if(Dis_Door_Bling)
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],1,0); 
            }
            else
            {
              ht1621_Char_write1(1,T_Addr[16],T_Mask[16],0,0);
              ht1621_Char_write1(1,T_Addr[17],T_Mask[17],0,0);               
            }            
            if(Dis_Door_Err_Bling)
            {
              ht1621_Char_write1(1,T_Addr[18],T_Mask[18],1,0); 
              //Dis_Door_Err_Bling = 0;
            }
            else
            {             
              ht1621_Char_write1(1,T_Addr[18],T_Mask[18],0,0);
            }   
          }
          else
          {
           // Dis_Door_Err_Bling = 1;
            
            Door_Move_time = 0;
            ht1621_Char_write1(1,T_Addr[18],T_Mask[18],1,0);
          }
        } 
      break;    
      default:
      break;
    } 
}
void Led_P1_State_Display(u8 Led_P1_State)
{
  if(Led_P1_State)
  {  
    ht1621_Char_write1(1,T_Addr[8],T_Mask[8],1,0);
  }
  else 
  {  
    ht1621_Char_write1(1,T_Addr[8],T_Mask[8],0,0);
  }
  
}
void Led_P2_State_Display(u8 Led_P2_State)
{
  if(Led_P2_State)
  {
    ht1621_Char_write1(1,T_Addr[9],T_Mask[9],1,0);
  }
  else 
  {  
    ht1621_Char_write1(1,T_Addr[9],T_Mask[9],0,0);
  }
}
void Display_OD()
{
  ht1621_Char_write1(1,T_Addr[8],T_Mask[8],KeyHandle.Od_State.Led_P1_State,0);
  ht1621_Char_write1(1,T_Addr[9],T_Mask[9],KeyHandle.Od_State.Led_P2_State,0);
  Fan_Speed_State_Display(KeyHandle.Od_State.Fan_State);
  ht1621_Char_write1(1,T_Addr[18],T_Mask[18],KeyHandle.Od_State.Door_State,0);
  
}
void Display_all(struct Peripheral peripheral,struct KEYHANDLE KeyHandle,struct Hepa Hepa,struct ALLDATE alldate)
{
    
    if(KeyHandle.Oper_Mode_Dis_State == 0)
    {
      if(peripheral.Door_Do)
      {
        KeyHandle.Led_P2_State = 0;
        Uart_Transmit_Hnadle(KeyHandle);
      }
      Led_P2_State_Display(KeyHandle.Led_P2_State);
      Led_P1_State_Display(KeyHandle.Led_P1_State);
      Door_State_Display(KeyHandle.Door_State,peripheral);
      Fan_Speed_State_Display(KeyHandle.Fan_State);  
      Hepa_Set_Display(Hepa,KeyHandle,peripheral);
      Now_Time_Display(alldate,KeyHandle); 
      ht1621_Char_write1(2,T_Addr[29],T_Mask[29],0,0);
      ht1621_Char_write1(2,T_Addr[30],T_Mask[30],0,0);  
    }
    else if(KeyHandle.Oper_Mode_Dis_State == 1)
    {
      if(peripheral.Door_Do)
      {
        KeyHandle.Od_State.Led_P2_State = 0;
        Uart_Transmit_Hnadle(KeyHandle);
      }
      Fan_Speed_State_Display(KeyHandle.Od_State.Fan_State);
      Led_P2_State_Display(KeyHandle.Od_State.Led_P2_State);
      Led_P1_State_Display(KeyHandle.Od_State.Led_P1_State);
      Door_State_Display(KeyHandle.Od_State.Door_State,peripheral);
    }
    else if(KeyHandle.Oper_Mode_Dis_State == 2)
    {
      if(peripheral.Door_Do)
      {
        KeyHandle.Pm_State.Led_P2_State = 0;
        Uart_Transmit_Hnadle(KeyHandle);
      }    
      Fan_Speed_State_Display(KeyHandle.Pm_State.Fan_State);
      Led_P2_State_Display(KeyHandle.Pm_State.Led_P2_State);
      Led_P1_State_Display(KeyHandle.Pm_State.Led_P1_State);
      Door_State_Display(KeyHandle.Pm_State.Door_State,peripheral);
      Now_Time_Display(alldate,KeyHandle); 
  
    }
    Peripheral_Rceive_Display(peripheral,KeyHandle.Fan_Seepd_Max_State);
    ht1621_char_display();
    Oper_Mode_Disply(KeyHandle.Oper_Mode_State,KeyHandle.Oper_Mode_Dis_State);
     
}
