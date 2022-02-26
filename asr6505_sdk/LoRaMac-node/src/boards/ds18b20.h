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
void DS18B20_RST(void); // 1.��λ����: 	�������͡�����ʱ480-960us�������Ÿߡ�����ʱ15-60us
int DS18B20_Check(void); //��Ӧ1--ʧ��		��Ӧ0----�ɹ�
uint8_t DS18B20_Read_Bit(void); //3. ��0����1����
void DS18B20_Write_One(void); //д1���� ��������Ϊ���ģʽ�����������͡�����ʱ2us�����������ߡ�����ʱ����60us
void DS18B20_Write_Zero(void);  //д0���� ��������Ϊ���ģʽ�����������͡�����ʱ60-120us�����������ߡ�����ʱ2us
uint8_t DS18B20_Read_Byte(void);    //��ȡһ���ֽ�8bit
void DS18B20_Write_Byte(uint8_t data);  //дһ���ֽ�
void DS18B20_Start(void); //�����豸
float	DS18B20_Get_Temp(void);
void DS18B20_Task_Init(void );
//#define InputDS 	PCin(11)  //������������PC11
//#define OutputDS 	PCout(11) //�����������PC11
//#define DS18B20_IO_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}//10���������� 00������ģʽ												
//#define DS18B20_IO_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;} //00���������ģʽ  11�����ģʽ������ٶ�50MHz

#endif
