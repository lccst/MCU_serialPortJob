#ifndef __DECENDEDGECNT_H__
#define __DECENDEDGECNT_H__

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "myADC.h"

void initGPIO(void);
u32 riseEdgeCnt(u16 edgesNum, u32 timeout);

#endif







