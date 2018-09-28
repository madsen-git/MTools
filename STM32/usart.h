#ifndef __USART_H__
#define	__USART_H__
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USART_RX_BUFFER_LEN		64					// ���ջ�������С
#define USART_TX_BUFFER_LEN		64					// ���ͻ�������С
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 ����1�շ�������
extern char g_rxBuffer1[USART_RX_BUFFER_LEN];		// ���ջ�����
extern char g_txBuffer1[USART_TX_BUFFER_LEN];		// ���ͻ�����
extern uint8_t 	g_rxCurrentIndex1;					// ��ǰ�������ݴ�ŵ�λ������������0
extern volatile uint8_t 	g_rxOver1;							// ������ɣ�������"\r\n"(0x0D0xA)��ʾ��β
extern uint8_t 	g_txDataLen1;						// Ҫ�������ݵĳ���
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 ����2�շ�������
extern char g_rxBuffer2[USART_RX_BUFFER_LEN];		// ���ջ�����
extern char g_txBuffer2[USART_TX_BUFFER_LEN];		// ���ͻ�����
extern uint8_t 	g_rxCurrentIndex2;					// ��ǰ�������ݴ�ŵ�λ������������0
extern uint8_t	g_rxLengthToRead2;					// ׼��Ҫ��ȡ�����ݳ���
extern volatile uint8_t 	g_rxOver2;							// ������ɣ�������"\r\n"(0x0D0xA)��ʾ��β
extern uint8_t 	g_txDataLen2;						// Ҫ�������ݵĳ���
	
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 ���ڱ��
typedef enum {
	BEUSARTNumber1			= 0x0001,		// ����1 TX��GPIOA_Pin_9  RX��GPIOA_Pin_10
	BEUSARTNumber2			= 0x0002		// 
}BEUSARTNumber;	
	
/*****************************************************************************************
	<< --- usart_config		2015-10-20 --- >>
	˵������������
	������
		com			= ��Ҫ�������ڣ�����1
		baudRate	= ������ �磺115200
		dataLength	= ����λ �磺8
		stopBits	= ֹͣλ �磺1
		parity		= ��żУ�� �磺0
	����ֵ��
*****************************************************************************************/
extern void usart_config(BEUSARTNumber com, uint32_t baudRate, uint16_t dataLength, uint16_t stopBits, uint16_t parity);
	
/*****************************************************************************************
	<< --- usart_print		2015-10-20 --- >>
	˵�����������������
	������
		com		= ����
		pinfo	= �ַ���
	����ֵ��
*****************************************************************************************/
extern void usart_print(BEUSARTNumber com, char *pinfo);
extern void usart_printData(BEUSARTNumber com, char *pinfo, uint8_t infoLen);
extern void usart_printHex(BEUSARTNumber com, char *pinfo, uint8_t infoLen);

#ifdef __cplusplus
}
#endif

#endif	// __USART_H__
