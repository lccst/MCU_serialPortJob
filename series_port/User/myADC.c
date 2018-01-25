#include "myADC.h"

int lcc_ADCInit()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	//①开启 ADC 和 GPIO 相关时钟和初始化 GPIO
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);							//使能 GPIOA 时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 							//使能 ADC1 时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;										//PA3 通道 3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;									//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;								//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);											//初始化
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);  							//ADC1 复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);  							//复位结束 
	//②初始化通用配置
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟 5 个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMA 失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;						//预分频 4 分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC 时钟最好不要超过 36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);										//初始化
	
	//③初始化 ADC1 相关参数
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;							//12 位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;									//非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;								//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;		//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;							//右对齐 
	ADC_InitStructure.ADC_NbrOfConversion = 1;										//1 个转换在规则序列中
	ADC_Init(ADC1, &ADC_InitStructure);												//ADC 初始化
	
	//④开启 ADC 转换
	ADC_Cmd(ADC1, ENABLE);															//开启 AD 转换器
	
	return 1;
}
 

//获得 ADC 值	//设置指定 ADC 的规则组通道，一个序列，采样时间
//ch:通道值 0~16: ch: @ref ADC_channels
//返回值:转换结果
ADCVALUE lcc_getADC(u8 ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_15Cycles ); 
	ADC_SoftwareStartConv(ADC1); 													//使能指定的 ADC1 的软件转换启动功能
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));									//等待转换结束
	
	return ADC_GetConversionValue(ADC1);  											//返回最近一次 ADC1 规则组的转换结果
}
//获取通道 ch 的转换值，取 times 次,然后平均
//ch:通道编号 times:获取次数
//返回值:通道 ch 的 times 次转换结果平均值
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





