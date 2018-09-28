/*****************************************************************************************
*	���ư���3��LED�ƵĿ��ƣ�
*	���Ų���GPIO_Mode_Out_PP����ͨ�����������������ĵ͵�ƽ��0V���ߵ�ƽ��3.3V�����
*	LED1����������PB5
*	LED2����������PD6
*	LED3����������PD3
*
*
*****************************************************************************************/

#ifndef __LED_H__
#define __LED_H__
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 �ܶ�V5LED���	
typedef enum {
	BELEDNumber1	= 0,	// GPIOB_Pin_5
	BELEDNumber2	,		// GPIOD_Pin_6
	BELEDNumber3			// GPIOD_Pin_3
}BELEDNumber;

/*****************************************************************************************
	<< --- led_config		2015-10-20 --- >>
	˵����LED�ܽ�����
	������
	����ֵ��
*****************************************************************************************/
extern void led_config(void);
		
/*****************************************************************************************
	<< --- led_config		2015-10-20 --- >>
	˵��������LED
	������
		num		= ������LED�ı��
	����ֵ��
*****************************************************************************************/
extern void led_on(BELEDNumber num);

/*****************************************************************************************
	<< --- led_config		2015-10-20 --- >>
	˵������LED
	������
		num		= ����LED�ı��
	����ֵ��
*****************************************************************************************/
extern void led_off(BELEDNumber num);

#ifdef __cplusplus
}
#endif
#endif	/*__LED_H__*/