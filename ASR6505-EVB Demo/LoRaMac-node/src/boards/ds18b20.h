#ifndef __DS18B20_H
#define	__DS18B20_H
#include "stm8l15x.h"

extern float tempc;

#define DS18B20_PORT GPIOE
#define DS18B20_PIN GPIO_Pin_3
#define BITSHIFT 1

#define DS18B20_H DS18B20_PORT->ODR |= DS18B20_PIN//HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_SET)//
#define DS18B20_L DS18B20_PORT->ODR &= ~DS18B20_PIN//HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET)//

#define DS18B20_IN ((DS18B20_PORT->IDR & DS18B20_PIN)>>BITSHIFT)//HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)

//#define DS18B20_IO_IN()  	GPIOB->MODER &= ~(3<<16)
//#define DS18B20_IO_OUT()  GPIOB->MODER |= (1<<16)
//#define DS18B20_IO_IN()  	{DS18B20_PORT->DDR &= ~DS18B20_PIN; DS18B20_PORT->CR1 |= DS18B20_PIN; DS18B20_PORT->CR2 &= (uint8_t)(~(DS18B20_PIN));}
//#define DS18B20_IO_OUT() 	{DS18B20_PORT->DDR |= DS18B20_PIN; DS18B20_PORT->CR1 |= DS18B20_PIN; DS18B20_PORT->CR2 |= DS18B20_PIN;}

#define DS18B20_IO_IN() GPIO_Init(DS18B20_PORT, DS18B20_PIN, GPIO_Mode_In_PU_No_IT)
#define DS18B20_IO_OUT() GPIO_Init(DS18B20_PORT, DS18B20_PIN, GPIO_Mode_Out_PP_High_Fast)

//void DS18B20_IO_IN(void);
void DS18B20_RST(void); // 1.复位操作: 	引脚拉低——延时480-960us——引脚高——延时15-60us
int DS18B20_Check(void); //响应1--失败		响应0----成功
uint8_t DS18B20_Read_Bit(void); //3. 读0、读1操作
void DS18B20_Write_One(void); //写1操作 设置引脚为输出模式——引脚拉低——延时2us——引脚拉高——延时大于60us
void DS18B20_Write_Zero(void);  //写0操作 设置引脚为输出模式——引脚拉低——延时60-120us——引脚拉高——延时2us
uint8_t DS18B20_Read_Byte(void);    //读取一个字节8bit
void DS18B20_Write_Byte(uint8_t data);  //写一个字节
void DS18B20_Start(void); //启动设备
float	DS18B20_Get_Temp(void);
void DS18B20_Task_Init(void );
//#define InputDS 	PCin(11)  //定义引脚输入PC11
//#define OutputDS 	PCout(11) //定义引脚输出PC11
//#define DS18B20_IO_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}//10：上拉输入 00：输入模式												
//#define DS18B20_IO_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;} //00：推挽输出模式  11：输出模式，最大速度50MHz

#endif
