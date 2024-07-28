#include "delay.h"
#include "usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"



static int Send_buf[10] = {0} ;

u8 USART2_RX_STA = 0;     //串口2接收状态标记
u8 USART2_RX_CMD;


void USART2_IRQHandler(void)
{
	u8 res;	    
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
	{	 
 
	res =USART_ReceiveData(USART2);	
	USART2_RX_CMD = res;                               //把接收到的数据传递给USART_RX_CMD
    USART2_RX_STA = 1;	
 }										 
}  
  
 
void uart2_init(u32 bound){
	 //GPIO????
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
	USART_DeInit(USART2);  //????2
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化GPIOB11，和GPIOB10
	
	
 	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOB10复用为USART3	 
 
	//USART ?????
 
	USART_InitStructure.USART_BaudRate = bound;//?????9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
	USART_InitStructure.USART_Parity = USART_Parity_No;//??????
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//????
 
	 USART_Init(USART2, &USART_InitStructure); //?????

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//?????3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//????3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ????
	NVIC_Init(&NVIC_InitStructure);	//??????????VIC???
 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//????

	USART_Cmd(USART2, ENABLE);                    //???? 
 
}

void USART2_SendByte(uint8_t  Data)                                 //串口发送一个字节；字节 (byte)    1byte=8bit
{
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);    //USART_FLAG_TXE发送寄存器空

    USART_SendData(USART2, Data);                                   //从串口2发送传入的数据

    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	    //USART_FLAG_TC发送完成标志
}

void Uart2_SU03T_SendCmd(int len)
{
    int i = 0 ;

    USART2_SendByte(0xAA);       //起始
    USART2_SendByte(0x55);

    for(i = 0; i < len; i++)     //数据
    {
        USART2_SendByte(Send_buf[i]);   //len 为8 ；依次将Send_buf[0]、Send_buf[1]~Send_buf[8]  发送出来
    }

    USART2_SendByte(0x55);
    USART2_SendByte(0xAA);      //结束

}


void Uart2_SU03T_SendCMD0(int dat1)
{
    Send_buf[0] = (int)(dat1);         //datal


    Uart2_SU03T_SendCmd(1);            //发送此帧数据
}
/*
 * 功能描述：串口2向外(SU-03T)发送命令(3位数据)
 * 内在逻辑：按照智能公元平台的数据接受协议，发送数据过去
*/
void Uart2_SU03T_SendCMD1(int dat1, int dat2, int dat3)
{
    Send_buf[0] = (int)(dat1);         //datal
    Send_buf[1] = (int)(dat2);         //data2
    Send_buf[2] = (int)(dat3);         //data3

    Uart2_SU03T_SendCmd(3);            //发送此帧数据
}

/*
 * 功能描述：串口2向外(SU-03T)发送命令(2位数据)
 * 内在逻辑：按照智能公元平台的数据接受协议，发送数据过去
*/
void Uart2_SU03T_SendCMD2(int dat1, int dat2)
{
    Send_buf[0] = (int)(dat1);         //datal
    Send_buf[1] = (int)(dat2);         //data2

    Uart2_SU03T_SendCmd(2);            //发送此帧数据
}

void Uart2_SU03T_SendCMD3(int dat1, float dat2, float dat3)
{
    Send_buf[0] = (int)(dat1);         //datal
    Send_buf[1] = (float)(dat2);         //data2
    Send_buf[2] = (float)(dat3);         //data3

    Uart2_SU03T_SendCmd(3);            //发送此帧数据
}
 
 










