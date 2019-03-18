#include "DS1302.h"

//SCL-PE5
#define IIC_SCL_H     (GPIOE->ODR |=  (1 << 5))
#define IIC_SCL_L     (GPIOE->ODR &= ~(1 << 5))

//SDA-PC1
#define IIC_SDA_H     (GPIOC->ODR |=  (1 << 1))
#define IIC_SDA_L     (GPIOC->ODR &= ~(1 << 1))
#define IIC_SDA_R     ((GPIOC->IDR &  (1 << 1))>>1)

void delay_us()
{
    nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
    nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();nop();
}

//初始化IIC
void IIC_Init(void)
{                        
    //由于STM8单片机，可以在输入和输出条件下读取IO口状态，故直接设置为输出。
    GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW );
    GPIO_Init(GPIOE, GPIO_PIN_5, GPIO_MODE_OUT_OD_HIZ_SLOW );
    IIC_SCL_H;
    IIC_SDA_H;
}
//产生IIC起始信号
void IIC_Start(void)
{
    IIC_SDA_H;                    
    IIC_SCL_H;
    delay_us();
    IIC_SDA_L; //START:when CLK is high,DATA change form high to low 
    delay_us();
    IIC_SCL_L; //钳住I2C总线，准备发送或接收数据 
}
//产生IIC停止信号
void IIC_Stop(void)
{
    IIC_SCL_L;

    IIC_SDA_L;//STOP:when CLK is high DATA change form low to high
    delay_us();
    IIC_SCL_H; 
    IIC_SDA_H;//发送I2C总线结束信号
    delay_us();                                                                   
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrtime=0;

    IIC_SDA_H;nop();           
    IIC_SCL_H;nop();
    while(IIC_SDA_R)
    {
        ucErrtime++;
        if(ucErrtime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_L;//时钟输出0            
    return 0;  
}
//产生ACK应答
void IIC_Ack(void)
{
    IIC_SCL_L;
    IIC_SDA_L;
    delay_us();
    IIC_SCL_H;
    delay_us();
    IIC_SCL_L;
}
//不产生ACK应答                    
void IIC_NAck(void)
{
    IIC_SCL_L;
    IIC_SDA_H;
    delay_us();
    IIC_SCL_H;
    delay_us();
    IIC_SCL_L;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答                          
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;       
    IIC_SCL_L;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd&0x80)>>7)
            IIC_SDA_H;
        else
            IIC_SDA_L;
        txd<<=1;           
        delay_us();   //对TEA5767这三个延时都是必须的
        IIC_SCL_H;
        delay_us(); 
        IIC_SCL_L;        
        delay_us();
    }         
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
     unsigned char i,receive=0;
     for(i=0;i<8;i++ )

    {
        IIC_SCL_L; 
        delay_us();
        IIC_SCL_H;
        receive<<=1;
        if(IIC_SDA_R)receive++;   
        nop(); 
    }                                         
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}
/*
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef PortPins,u8 State)
{
  if(State)
    GPIO_WriteHigh(GPIOx,PortPins);
  else 
    GPIO_WriteLow(GPIOx,PortPins);
}
void IIC_Start(void)
{
	GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);     //sda线输出
	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, SET);	  	  
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);
	Delay_Us(4);
 	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, RESET);//START:when CLK is high,DATA change form high to low 
	Delay_Us(4);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);//钳住I2C总线，准备发送或接收数据 
}

//产生IIC停止信号
void IIC_Stop(void)
{
	GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);//sda线输出
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);
	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, RESET);//STOP:when CLK is high DATA change form low to high
 	Delay_Us(4);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET); 
	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, SET);//发送I2C总线结束信号
	Delay_Us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
        GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);      //SDA设置为输入
        GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, SET);Delay_Us(1);   
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);Delay_Us(1);	 
	while(GPIO_ReadInputPin(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN))
	{
              ucErrTime++;
              if(ucErrTime>250)
              {
                      IIC_Stop();
                      return 1;
              }
	}
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);
	GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, RESET);
	Delay_Us(2);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);
	Delay_Us(2);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);
	GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, SET);
	Delay_Us(2);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);
	Delay_Us(2);
	GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
    GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_OUT_PP_HIGH_FAST); 	    
    GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
          if((txd&0x80)>>7)
                  GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, SET);
          else
                  GPIO_WriteBit(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, RESET);
          txd<<=1; 	  
          Delay_Us(2);   //对TEA5767这三个延时都是必须的
          GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);
          Delay_Us(2); 
          GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET);	
          Delay_Us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	GPIO_Init(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN, GPIO_MODE_IN_PU_NO_IT);//SDA设置为输入
        for(i=0;i<8;i++ )
        {
            GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, RESET); 
            Delay_Us(2);
            GPIO_WriteBit(DS1302_IIC_SCL_PROT,DS1302_IIC_SCL_PIN, SET);
            receive<<=1;
            if(GPIO_ReadInputPin(DS1302_IIC_SDA_PROT,DS1302_IIC_SDA_PIN))receive++;   
            Delay_Us(1); 
        }					 
        if (!ack)
            IIC_NAck();//发送nACK
        else
            IIC_Ack(); //发送ACK   
        return receive;
}
*/
void ds1302_writeByte(u8 Addr,u8 Data)
{

    IIC_Start();
    IIC_Send_Byte(Addr);
    while(!IIC_Wait_Ack());
    IIC_Send_Byte(Data);
    while(!IIC_Wait_Ack());
    IIC_Stop();
}
u8 ds1302_readByte(u8 Addr)
{
	u8 data;
        IIC_Start(); 
	IIC_Send_Byte(Addr);
        while(!IIC_Wait_Ack());
	data = IIC_Read_Byte(1);
        IIC_Stop();
        Delay_Us(10);
	return data;
}
unsigned char BCDToDec(unsigned char mData)
{

	unsigned char Dec = 0;
	Dec = mData & 0x0f; //取得最低位
	mData = mData & 0x70; //剔除最高位和最低四位
	Dec += mData>>1; //先左移1代表*2
	Dec += mData>>3; //再左移3代表*8，所以总的相当于乘以10
	return Dec;
}
unsigned char DecToBCD(unsigned char mData)
{
	unsigned char BCD = 0;
	while(mData >= 10)
	{
		mData -= 10;
		BCD ++;
	};
	BCD <<= 4;
	BCD |= mData;
	return BCD;
}
struct ALLDATE convertToDisplayTime(struct ALLDATE allDate)
{
      struct ALLDATE mAllDate;
      mAllDate.yd.year = (((allDate.yd.year&0x80)>>4)*10) + BCDToDec(allDate.yd.year);
      mAllDate.yd.day = BCDToDec(allDate.yd.day);
      mAllDate.md.month = BCDToDec(allDate.md.month);
      mAllDate.md.date = BCDToDec(allDate.md.date);
      mAllDate.hms.hour = BCDToDec(allDate.hms.hour);
      mAllDate.hms.min = BCDToDec(allDate.hms.min);
      mAllDate.hms.sec = BCDToDec(allDate.hms.sec);
      return mAllDate;
}
struct ALLDATE convertToSetTime(struct ALLDATE allDate)
{
	struct ALLDATE mAllDate;
	mAllDate.yd.year = DecToBCD(allDate.yd.year);
	mAllDate.yd.day = DecToBCD(allDate.yd.day);
	mAllDate.md.month = DecToBCD(allDate.md.month);
	mAllDate.md.date = DecToBCD(allDate.md.date);
	mAllDate.hms.hour = DecToBCD(allDate.hms.hour);
	mAllDate.hms.min = DecToBCD(allDate.hms.min);
	mAllDate.hms.sec = CH + DecToBCD(allDate.hms.sec);
	return mAllDate;
}

void ds1302_setTime(struct ALLDATE allDate)
{
        IIC_Init();
        GPIO_Init(GPIOC,GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_SLOW);
        GPIO_WriteLow(GPIOC,GPIO_PIN_2); 
	allDate = convertToSetTime(allDate);
	ds1302_writeByte(0x8e, 0x00);//control为的最高位wp
	ds1302_writeByte(0x8c, allDate.yd.year); //year
	ds1302_writeByte(0x8a, allDate.yd.day);//day
	ds1302_writeByte(0x88, allDate.md.month); //month
	ds1302_writeByte(0x86, allDate.md.date); //date
	ds1302_writeByte(0x84, allDate.hms.hour); //hour
	ds1302_writeByte(0x82, allDate.hms.min); //min
	ds1302_writeByte(0x80, allDate.hms.sec); //sec
}
struct ALLDATE ds1302_readTime( void )
{
	struct ALLDATE allDate;
	allDate.yd.year = ds1302_readByte(0x8d); //year
	allDate.yd.day = ds1302_readByte(0x8b); //day
	allDate.md.month = ds1302_readByte(0x89); //month
	allDate.md.date = ds1302_readByte(0x87); //date
	allDate.hms.hour = ds1302_readByte(0x85); //hour
	allDate.hms.min = ds1302_readByte(0x83); //min
	allDate.hms.sec = ds1302_readByte(0x81); //sec 
	allDate = convertToDisplayTime(allDate);
	return allDate;

}
void Ds1302_Init()
{
	struct ALLDATE allDate;
	allDate.yd.year = 18;
	allDate.yd.day = 16;
	allDate.md.month = 3;
	allDate.md.date = 6;
        allDate.hms.hour = 20;
        allDate.hms.min = 15;
        allDate.hms.sec = 30;
        IIC_Init();
	ds1302_setTime(allDate);
}