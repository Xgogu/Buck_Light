#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "pwm.h"
#include "timer.h"
#include "hc05.h"
#include "usart3.h"
#include "usart2.h"
#include "string.h"	
#include "dht11.h"
#include <cstdint>  



int main(void)
{ 
	u8 reclen=0;
	u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;
	
	delay_init(168);    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ�����ڲ�����Ϊ115200	    
	TIM3_PWM_Init(100-1,84-1);
	printf("0");
	LED_Init();					//��ʼ��LED 
 	LCD_Init();         //��ʼ��LCD�ӿ�
	Adc_Init();         //��ʼ��ADC
    Dma_Init();
	uart2_init(9600);
	while(HC05_Init()) 		//��ʼ��ATK-HC05ģ��  
	{
	LCD_ShowString(30,90,200,16,16,"ATK-HC05 Error!"); 
	delay_ms(500);
	LCD_ShowString(30,90,200,16,16,"Please Check!!!"); 
	delay_ms(100);
		printf("1");	}
 	while(DHT11_Init())	//DHT11��ʼ��	
	{
		LCD_ShowString(30,130,200,16,16,"DHT11 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
		printf("2");
	}
	printf("HC-05 OK");
	delay_ms(100);
	USART3_RX_STA=0;
	TIM4_Init(100-1,840-1);
	POINT_COLOR=RED; 
	LCD_ShowString(30,50,200,16,16,"Explorer STM32F4");	
	LCD_ShowString(30,70,200,16,16,"ADC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2014/5/6");	  
	POINT_COLOR=BLUE;
//	LCD_ShowString(30,130,200,16,16,"ADC1_CH5_VAL:");	      
//	LCD_ShowString(30,150,200,16,16,"ADC1_CH4_VAL:");
//	LCD_ShowString(30,170,200,16,16,"ADC1_CH3_VAL:");		  
//	
	while(1)
	{ 
//		LCD_ShowxNum(134,130,ADC_ConvertedValue[0],4,16,0);    //��ʾADCC�������ԭʼֵ
//		LCD_ShowxNum(134,150,ADC_ConvertedValue[1],4,16,0);    //��ʾADCC�������ԭʼֵ
//		LCD_ShowxNum(134,170,ADC_ConvertedValue[2],4,16,0);    //��ʾADCC�������ԭʼֵ
	  if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
			LCD_Fill(30,200,240,320,WHITE);	//�����ʾ
 			reclen=USART3_RX_STA&0X3FFF;	//�õ����ݳ���
		  	USART3_RX_BUF[reclen]=0;	 	//���������
 			LCD_ShowString(30,200,200,16,16,USART3_RX_BUF);//��ʾ���յ�������
			int pwm_1=0;
			for(int i=0;i<reclen-1;i++)
			{
			pwm_1=pwm_1*10+(USART3_RX_BUF[i]-48);
			
			}
			printf("%d",pwm_1);
			
			if(pwm_1==340)
			{check=0;}
			else if(pwm_1==3342)
			{check=1;}
			else if(pwm_1<=100 &&pwm_1>=0)
			{pwm=pwm_1;}
//			printf("%d",USART3_RX_BUF[0]-48);
//			printf("%d",USART3_RX_BUF[1]-48);
 			USART3_RX_STA=0;	 
		}
		if(t%10==0)//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);		//��ȡ��ʪ��
			u3_printf("temp:%d",temperature);
			u3_printf("hum%d",humidity);
			LCD_ShowNum(30+40,150,temperature,2,16);		//��ʾ�¶�	   		   
			LCD_ShowNum(30+40,170,humidity,2,16);			//��ʾʪ��	 	   
		}				   
	 	delay_ms(10);
		t++;
		if(USART2_RX_STA == 1)
        {
        switch(USART2_RX_CMD)/*caseֵ�����ܹ�Ԫ�Ĳ�����һ��*/
        {
		case	0x01:
		Uart2_SU03T_SendCMD0(0X01); 
		delay_ms(500);
		break;	
		case	0x02:
		Uart2_SU03T_SendCMD2(0X02, light_test); 
		delay_ms(500);
		break;	
		case	0x03:
		pwm=90;
		LED0=!LED0;
		delay_ms(50);
		break;			
		case	0x04:
		pwm=10;
		LED0=!LED0;
		delay_ms(50);
		break;							
		case	0x06:
		if(pwm<100)
		{		
		pwm+=20;
		}
		LED0=!LED0;
		delay_ms(50);
		break;
		case	0x07:
		if(pwm>0)
		{		
		pwm-=20;
		}
		LED0=!LED0;
		delay_ms(50);
		break;
		case	0x08:
		check=1;
		LED0=!LED0;
		delay_ms(50);
		break;
		case	0x09:
		check=0;
		LED0=!LED0;
		delay_ms(50);
		break;
		
		}
		USART2_RX_STA=0;
		}
		
	}
}



