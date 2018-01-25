#include "myADC.h"

int lcc_ADCInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//�ٿ��� ADC �� GPIO ���ʱ�Ӻͳ�ʼ�� GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);							//ʹ�� GPIOA ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 							//ʹ�� ADC1 ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;										//PA3 ͨ�� 3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;									//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;								//����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//��ʼ��
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);  							//ADC1 ��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);  							//��λ���� 
	//�ڳ�ʼ��ͨ������
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//����ģʽ
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//���������׶�֮����ӳ� 5 ��ʱ��
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMA ʧ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;						//Ԥ��Ƶ 4 ��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADC ʱ����ò�Ҫ���� 36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);										//��ʼ��
	
	//�۳�ʼ�� ADC1 ��ز���
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12 λģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;									//��ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;								//�ر�����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//��ֹ������⣬ʹ���������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//�Ҷ��� 
	ADC_InitStructure.ADC_NbrOfConversion = 1;										//1 ��ת���ڹ���������
	ADC_Init(ADC1, &ADC_InitStructure);												//ADC ��ʼ��
	
	//�ܿ��� ADC ת��
	ADC_Cmd(ADC1, ENABLE);															//���� AD ת����
	
	return 1;
}
 

//��� ADC ֵ	//����ָ�� ADC �Ĺ�����ͨ����һ�����У�����ʱ��
//ch:ͨ��ֵ 0~16: ch: @ref ADC_channels
//����ֵ:ת�����
ADCVALUE lcc_getADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_15Cycles ); 
	ADC_SoftwareStartConv(ADC1); 													//ʹ��ָ���� ADC1 �����ת����������
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));									//�ȴ�ת������
	
	return ADC_GetConversionValue(ADC1);  											//�������һ�� ADC1 �������ת�����
}
//��ȡͨ�� ch ��ת��ֵ��ȡ times ��,Ȼ��ƽ��
//ch:ͨ����� times:��ȡ����
//����ֵ:ͨ�� ch �� times ��ת�����ƽ��ֵ
ADCVALUE lcc_getAverageADC(u8 ch,u16 times)
{
	u32 temp_val=0; u16 t;
	for(t=0;t<times;t++)
	{
		temp_val += lcc_getADC(ch); 
		delay_us(2);
	}
	return temp_val/times;
}





