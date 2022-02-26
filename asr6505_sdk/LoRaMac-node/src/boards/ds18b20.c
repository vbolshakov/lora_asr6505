#include "ds18b20.h"
#include "delay.h"

uint8_t trynum;
uint8_t retry=0;
float data;
float tempc;
//static void DS18B20_IO_OUT(){
//GPIO_InitTypeDef GPIO_InitStruct = {0};

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

//  /*Configure GPIO pin : PA0 */
//  GPIO_InitStruct.Pin = DS18B20_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
//}

//void DS18B20_IO_IN(){
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//  /*Configure GPIO pin : PA0 */
//  GPIO_InitStruct.Pin = DS18B20_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  HAL_GPIO_Init(DS18B20_PORT, &GPIO_InitStruct);
//}

void delay_5us(uint16_t time)
{    
//   uint16_t i=0;  
   while(time--)
   {
//      i=6;  //自己定义
//      while(i--) ;    
   }
}

void DS18B20_RST(void) // 1.复位操作: 	引脚拉低——延时480-960us——引脚高——延时15-60us
{
	DS18B20_IO_OUT(); //切换为输出模式
	delay_us(750);
	DS18B20_L;//拉低
	delay_us(750);
	DS18B20_H;//拉高
//	delay_2us(1);
}
int DS18B20_Check(void) //响应1--失败		响应0----成功
{	
	//2.应答操作:	将引脚设置为输入模式——判断引脚低电平时间是否大于60us、小于240us——返回应答结果	
	retry=0;
	DS18B20_IO_IN();			//切换到输入模式
	while(DS18B20_IN && retry<100) //引脚一直为高，未被设备主动拉低。提供200us的超时时间
	{
		retry++;
		delay_us(2);
	}
	retry=retry;
	if(retry>=100) 
		return 1;	//超时仍为响应（200us）
    else retry=0;
	while(!DS18B20_IN && retry<120)// 引脚响应则  判断引脚低电平时间是否大于60us、小于240us——返回应答结果
	{
		retry++;
		delay_us(2);
	}
	retry=retry;
	if(retry>=120)
		return 2;  //应答过时，检查失败     
//	printf("true\r\n");
    return 0; 			//检验成功，返回0
	
}
uint8_t DS18B20_Read_Bit(void) //3. 读0、读1操作
{
	//将引脚设置为输出模式——引脚拉低——延时2us——引脚拉高——设置为输入模式——延时2us——读取引脚状态——返回读取结果	周期60us
	uint8_t data;//暂存数据
	DS18B20_IO_OUT();		//切换输出模式
	delay_us(10);
	DS18B20_L; //拉低
	delay_us(2);
	DS18B20_H; //释放
	DS18B20_IO_IN();		//切换输入模式
	delay_us(2);
//	delay_2us(1);
	data =DS18B20_IN;
//	if(DS18B20_IN)data =1 ;
//	else data = 0;
	delay_us(50);
	//printf("数据：%d\r\n",data);
	return data;

}
void DS18B20_Write_One(void) //写1操作 设置引脚为输出模式——引脚拉低——延时2us——引脚拉高——延时大于60us
{
    DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
	DS18B20_L;//拉低2us
//	delay_2us(1);     
    	delay_us(2);
	DS18B20_H;
	delay_us(60);
}
void DS18B20_Write_Zero(void)  //写0操作 设置引脚为输出模式——引脚拉低——延时60-120us——引脚拉高——延时2us
{
	DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
	DS18B20_L;//拉低2us   
	delay_us(50);                           
	DS18B20_H;
//	delay_2us(1);
	delay_us(2);
}
uint8_t DS18B20_Read_Byte(void)    //读取一个字节8bit
{       
	uint8_t data=0;
	uint8_t i;
	
	for( i=0;i<8;i++)
	{
		data>>=1;
		if(DS18B20_Read_Bit())
		{
			data |=0x80;
		}
	}

	return data;
}
void DS18B20_Write_Byte(uint8_t data)  //写一个字节
{            
	uint8_t i;

   DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
   for(i=1;i<=8;i++)
   {
		if(data&0x01) //先写低位,1就写1
		DS18B20_Write_One();
		else
		DS18B20_Write_Zero();
		data >>= 1;//右移位
   }
}
void DS18B20_Start(void) //启动设备
{                                                                  
   DS18B20_RST();            	//复位
   trynum=DS18B20_Check();    		 	//等待响应
   DS18B20_Write_Byte(0xcc);  	// 发送一条 ROM 指令		0ccH跳过ROM
   DS18B20_Write_Byte(0x44);    // 发送存储器指令		启动温度转化	
}
// 读取传感器的值
float	DS18B20_Get_Temp(void)
{
	
	uint8_t TH=0,TL=0;
	uint16_t TB=0;
    DS18B20_Start();
    DS18B20_RST();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC); // 忽略ROM地址，直接发送命令
    DS18B20_Write_Byte(0xBE); // 读取暂存器中9字节数据
	TL = DS18B20_Read_Byte(); // 低8位
	TH = DS18B20_Read_Byte(); // 低八位
	
	TB = TH;
	TB <<=8;
	TB = TB|TL;
//	printf("TB：%d\r\n",TB);
	data = TB* 0.0625;
    //data = ((uint16_t)DS18B20_Read_Byte() | DS18B20_Read_Byte()  << 8) * 0.0625f;
    return data;
}

