#ifndef __MYADC_H__
#define __MYADC_H__


#include "sys.h"
#include "delay.h"

typedef unsigned short ADCVALUE;

int lcc_ADCInit(void);
ADCVALUE lcc_getADC(u8 ch);
ADCVALUE lcc_getAverageADC(u8 ch,u16 times);

#endif





