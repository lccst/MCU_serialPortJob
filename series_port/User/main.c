/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ�ADC����
ʵ��ƽ̨������STM32F407VET6 ������
��汾  ��V1.2.1 
***********************************************************************/
#include <stdlib.h>
#include "sys.h"	
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "decendEdgeCnt.h"
#include "myADC.h"

#define WAIT	 0
#define WORK	 1
char status = WAIT;

int cmd_analyse(char *cmd)
{
	char cmdLocal[] = "EDGENUM:";		// ��������Ŀ��ͨ����λ�����ڷ���
	int i = 0, j = 0, k = 0, num = 0;
	char rbuf[10] = {0};
	
	while( (*(cmd+i)!='\0')&&(cmdLocal[i]!='\0') ){
		if(*(cmd+i)==cmdLocal[0])
			break;
		else
			i++;
	}
	if(*(cmd+i)=='\0')
		return -1;
	for(j=0; j<8; j++){
		if(cmdLocal[j] != *(cmd+i+j))
			return -1;
	}
	
	while(*(cmd+i+j)!='\0'){
		if( (*(cmd+i+j)>'9')||(*(cmd+i+j)<'0') )	//�����������
			return -1;
		rbuf[k] = *(cmd+i+j);
		i++;
		k++;
	}
	num = atoi(rbuf);
	
	return num;
}


int getStatus(){
	int i = -1;
	char revBuf[100] = {0};

	if(serial_Rev(revBuf)!=0)
		i = cmd_analyse(revBuf);
	if(i!=-1)
		status = WORK;
	else
		status = WAIT;
	
	return i;
}

int main(void)
{
	int edgesNum = 0;
	
	delay_init(168);
	uart_init(115200);
	initGPIO();
	lcc_ADCInit();
	
	while(1){
		edgesNum = getStatus();
		if(status==WORK){
			if(edgesNum>0)
				riseEdgeCnt(edgesNum, 10000000);
		}
	}

}


