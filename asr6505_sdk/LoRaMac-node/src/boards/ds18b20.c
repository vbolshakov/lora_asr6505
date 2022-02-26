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
//      i=6;  //�Լ�����
//      while(i--) ;    
   }
}

void DS18B20_RST(void) // 1.��λ����: 	�������͡�����ʱ480-960us�������Ÿߡ�����ʱ15-60us
{
	DS18B20_IO_OUT(); //�л�Ϊ���ģʽ
	delay_us(750);
	DS18B20_L;//����
	delay_us(750);
	DS18B20_H;//����
//	delay_2us(1);
}
int DS18B20_Check(void) //��Ӧ1--ʧ��		��Ӧ0----�ɹ�
{	
	//2.Ӧ�����:	����������Ϊ����ģʽ�����ж����ŵ͵�ƽʱ���Ƿ����60us��С��240us��������Ӧ����	
	retry=0;
	DS18B20_IO_IN();			//�л�������ģʽ
	while(DS18B20_IN && retry<100) //����һֱΪ�ߣ�δ���豸�������͡��ṩ200us�ĳ�ʱʱ��
	{
		retry++;
		delay_us(2);
	}
	retry=retry;
	if(retry>=100) 
		return 1;	//��ʱ��Ϊ��Ӧ��200us��
    else retry=0;
	while(!DS18B20_IN && retry<120)// ������Ӧ��  �ж����ŵ͵�ƽʱ���Ƿ����60us��С��240us��������Ӧ����
	{
		retry++;
		delay_us(2);
	}
	retry=retry;
	if(retry>=120)
		return 2;  //Ӧ���ʱ�����ʧ��     
//	printf("true\r\n");
    return 0; 			//����ɹ�������0
	
}
uint8_t DS18B20_Read_Bit(void) //3. ��0����1����
{
	//����������Ϊ���ģʽ�����������͡�����ʱ2us�����������ߡ�������Ϊ����ģʽ������ʱ2us������ȡ����״̬�������ض�ȡ���	����60us
	uint8_t data;//�ݴ�����
	DS18B20_IO_OUT();		//�л����ģʽ
	delay_us(10);
	DS18B20_L; //����
	delay_us(2);
	DS18B20_H; //�ͷ�
	DS18B20_IO_IN();		//�л�����ģʽ
	delay_us(2);
//	delay_2us(1);
	data =DS18B20_IN;
//	if(DS18B20_IN)data =1 ;
//	else data = 0;
	delay_us(50);
	//printf("���ݣ�%d\r\n",data);
	return data;

}
void DS18B20_Write_One(void) //д1���� ��������Ϊ���ģʽ�����������͡�����ʱ2us�����������ߡ�����ʱ����60us
{
    DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
	DS18B20_L;//����2us
//	delay_2us(1);     
    	delay_us(2);
	DS18B20_H;
	delay_us(60);
}
void DS18B20_Write_Zero(void)  //д0���� ��������Ϊ���ģʽ�����������͡�����ʱ60-120us�����������ߡ�����ʱ2us
{
	DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
	DS18B20_L;//����2us   
	delay_us(50);                           
	DS18B20_H;
//	delay_2us(1);
	delay_us(2);
}
uint8_t DS18B20_Read_Byte(void)    //��ȡһ���ֽ�8bit
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
void DS18B20_Write_Byte(uint8_t data)  //дһ���ֽ�
{            
	uint8_t i;

   DS18B20_IO_OUT();        //SET PG11 OUTPUT;
	delay_us(10);
   for(i=1;i<=8;i++)
   {
		if(data&0x01) //��д��λ,1��д1
		DS18B20_Write_One();
		else
		DS18B20_Write_Zero();
		data >>= 1;//����λ
   }
}
void DS18B20_Start(void) //�����豸
{                                                                  
   DS18B20_RST();            	//��λ
   trynum=DS18B20_Check();    		 	//�ȴ���Ӧ
   DS18B20_Write_Byte(0xcc);  	// ����һ�� ROM ָ��		0ccH����ROM
   DS18B20_Write_Byte(0x44);    // ���ʹ洢��ָ��		�����¶�ת��	
}
// ��ȡ��������ֵ
float	DS18B20_Get_Temp(void)
{
	
	uint8_t TH=0,TL=0;
	uint16_t TB=0;
    DS18B20_Start();
    DS18B20_RST();
    DS18B20_Check();
    DS18B20_Write_Byte(0xCC); // ����ROM��ַ��ֱ�ӷ�������
    DS18B20_Write_Byte(0xBE); // ��ȡ�ݴ�����9�ֽ�����
	TL = DS18B20_Read_Byte(); // ��8λ
	TH = DS18B20_Read_Byte(); // �Ͱ�λ
	
	TB = TH;
	TB <<=8;
	TB = TB|TL;
//	printf("TB��%d\r\n",TB);
	data = TB* 0.0625;
    //data = ((uint16_t)DS18B20_Read_Byte() | DS18B20_Read_Byte()  << 8) * 0.0625f;
    return data;
}

