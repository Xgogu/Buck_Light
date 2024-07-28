#ifndef __USART2_H
#define __USART2_H	 
#include "sys.h"  
   

extern u8 USART2_RX_STA;         		//接收状态标记
extern u8 USART2_RX_CMD;						//接收数据状态

void uart2_init(u32 bound);			//串口3初始化 
void USART2_SendByte(uint8_t  Data);
void USART2_IRQHandler(void);

void Uart2_SU03T_SendCmd(int len);
void Uart2_SU03T_SendCMD0(int dat1);
void Uart2_SU03T_SendCMD1(int dat1, int dat2, int dat3);
void Uart2_SU03T_SendCMD2(int dat1, int dat2);

void Uart2_SU03T_SendCMD3(int dat1, float dat2, float dat3);


#endif













