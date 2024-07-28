#include "adc.h"
#include "delay.h"		 


u16 ADC_ConvertedValue[4]={0};
//初始化ADC		
void  Adc_Init(void)
{    
    GPIO_InitTypeDef       GPIO_InitStructure;
	ADC_CommonInitTypeDef  ADC_CommonInitStructure;
	ADC_InitTypeDef        ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_4|GPIO_Pin_0 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
  ADC_InitStructure.ADC_NbrOfConversion = 4;
  ADC_Init(ADC1, &ADC_InitStructure);
 

  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles );		    
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 2, ADC_SampleTime_3Cycles );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_3Cycles );
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 4, ADC_SampleTime_3Cycles );
  ADC_Cmd(ADC1, ENABLE);		
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_SoftwareStartConv(ADC1);			
 
}

void Dma_Init(void)
{
    DMA_InitTypeDef DMA_InitStructure;
 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 

    DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)ADC1+0x4c);	
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
    DMA_InitStructure.DMA_BufferSize = 4;	
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 

    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 

	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	

	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

	DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  

    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  

    DMA_InitStructure.DMA_Channel = DMA_Channel_0; 

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);
 
}
 
//void  Adc_Init(void)
//{    
//  GPIO_InitTypeDef  GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟

//  //先初始化ADC1通道5 IO口
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_3|GPIO_Pin_4;//PA5 通道5
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化  
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
// 
//	
//  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
//  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
//  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
//  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
//	
// 
//	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	

//}				  
//获得ADC值
//ch: @ref ADC_channels 
//通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
//返回值:转换结果
//u16 Get_Adc(u8 ch)   
//{
//	  	//设置指定ADC的规则组通道，一个序列，采样时间
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
//  
//	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

//	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
//}
////获取通道ch的转换值，取times次,然后平均 
////ch:通道编号
////times:获取次数
////返回值:通道ch的times次转换结果平均值
//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 
//	 
//u16 Lsens_Get(u8 ch)
//{
//	uint16_t temp_val=Get_Adc(ch);
//	if(temp_val>4000)temp_val=4000;
//	return (100-(temp_val/40));

//}





