#include "adc.h"

uint16_t adc1_value;
float VoltageValue;

void adc_init(){
	CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//����ADCʱ��
  	ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1�����β�����12λ��1��Ƶ  
  	ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//������������
  	ADC_VrefintCmd(ENABLE);//�ڲ���׼��ѹʹ��
  	ADC_ChannelCmd (ADC1,ADC_Channel_Vrefint,ENABLE);//ʹ���ڲ���׼��ѹ����ͨ��
  	ADC_Cmd(ADC1,ENABLE);//ADC1ʹ��
	
	adc1_value = 0;
	VoltageValue = 0;
}


void vol_measure(){	
	ADC_SoftwareStartConv (ADC1);//�������ת��
             
        while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//�ȴ�ת������
        ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//�����ر�ʶ
       
        adc1_value=ADC_GetConversionValue (ADC1);//��ȡת��ֵ
     //   VoltageValue=adc1_value*0.000805664;	//3.3UL/4095UL=0.000805664
	VoltageValue=5013.504/adc1_value; //4096*1.224
	
}
