#include "adc.h"

uint16_t adc1_value;
float VoltageValue;

void adc_init(){
	CLK_PeripheralClockConfig (CLK_Peripheral_ADC1,ENABLE);//开启ADC时钟
  	ADC_Init (ADC1,ADC_ConversionMode_Single,ADC_Resolution_12Bit,ADC_Prescaler_1);//ADC1，单次采样，12位，1分频  
  	ADC_SamplingTimeConfig(ADC1,ADC_Group_FastChannels,ADC_SamplingTime_384Cycles);//采样周期设置
  	ADC_VrefintCmd(ENABLE);//内部基准电压使能
  	ADC_ChannelCmd (ADC1,ADC_Channel_Vrefint,ENABLE);//使能内部基准电压测量通道
  	ADC_Cmd(ADC1,ENABLE);//ADC1使能
	
	adc1_value = 0;
	VoltageValue = 0;
}


void vol_measure(){	
	ADC_SoftwareStartConv (ADC1);//开启软件转换
             
        while(!ADC_GetFlagStatus (ADC1,ADC_FLAG_EOC));//等待转换结束
        ADC_ClearFlag (ADC1,ADC_FLAG_EOC);//清除相关标识
       
        adc1_value=ADC_GetConversionValue (ADC1);//获取转换值
     //   VoltageValue=adc1_value*0.000805664;	//3.3UL/4095UL=0.000805664
	VoltageValue=5013.504/adc1_value; //4096*1.224
	
}
