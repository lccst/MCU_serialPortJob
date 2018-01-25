#include "decendEdgeCnt.h"

void initGPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//ʹ�� GPIOD ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			// | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			//��ͨ����ģʽ
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//������� �����ò����üĴ���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);					//��ʼ�� GPIO
	GPIO_SetBits(GPIOD,GPIO_Pin_9);			//GPIOD9���ø�
	
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			// | GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ����ģʽ
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//������� �����ò����üĴ���
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
//	
//	GPIO_Init(GPIOD, &GPIO_InitStructure);					//��ʼ�� GPIO
}

/*****************************************************************
*						�����ؼ�������
*		��������� edgesNum �������أ����߳�ʱ����ֹͣ�ú���
******************************************************************/
u16 sendBuf[50000];
u32 riseEdgeCnt(u16 edgesNum, u32 timeout)
{
	ADCVALUE adc;
	u8 status1, status2;
	u16 cnt = 0;
	u32 timeCnt = 0;
	
	status1 = 1;
	status2 = 1;
	
	sendBuf[0] = 0xffff;
	while(1){
		status1 = PDin(9);
		if( (status1==1) && (status2==0) ){
			adc = lcc_getAverageADC(ADC_Channel_3, 20);
			sendBuf[cnt] = adc;
			cnt ++;	
			timeCnt = 0;
		}
		else{
			delay_us(1);
			timeCnt ++;
		}
		status2 = status1;
		if(cnt>edgesNum){
			if(sendBuf[0] != 0xffff)		// ��Ϊ��������£�ֻ�о����ɼ��ˣ�sendBuf[1] �Ų�Ϊ 0xffff
				uart_sendData(sendBuf, cnt);
			break;
		}
		if(timeCnt>timeout)
			break;
	}
	
	return cnt;
}







