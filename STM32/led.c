#include "led.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"


////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 根据BELEDNumber得到端口信息
static GPIO_TypeDef* led_getInfo(BELEDNumber num, uint16_t *ppin)
{
	GPIO_TypeDef *type;
	assert_param(ppin);
	switch((int)num)
	{
	case BELEDNumber1:
		{
			*ppin = GPIO_Pin_5;
			type = GPIOB;
		}
		break;
	case BELEDNumber2:
		{
			*ppin = GPIO_Pin_6;
			type = GPIOD;
		}
		break;
	case BELEDNumber3:
		{
			*ppin = GPIO_Pin_3;
			type = GPIOD;
		}
		break;
	}
	
	return type;
}

void led_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD , ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				// LED1  V6	   //将V6,V7,V8 配置为通用推挽输出  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 口线翻转速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_3;	// LED2, LED3	 V7 V8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				// LCD背光控制
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);						// LCD背光关闭	
}


void led_on(BELEDNumber num)
{
	uint16_t pin = 0;
	GPIO_TypeDef *gpioType = led_getInfo(num, &pin);
	if(gpioType)
	{
		GPIO_SetBits(gpioType, pin); 
	}
}

void led_off(BELEDNumber num)
{
	uint16_t pin = 0;
	GPIO_TypeDef *gpioType = led_getInfo(num, &pin);
	if(gpioType)
	{
		GPIO_ResetBits(gpioType, pin); 
	}
}


