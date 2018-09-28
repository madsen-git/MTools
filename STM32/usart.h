#ifndef __USART_H__
#define	__USART_H__
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define USART_RX_BUFFER_LEN		64					// 接收缓存区大小
#define USART_TX_BUFFER_LEN		64					// 发送缓存区大小
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 串口1收发缓存区
extern char g_rxBuffer1[USART_RX_BUFFER_LEN];		// 接收缓存区
extern char g_txBuffer1[USART_TX_BUFFER_LEN];		// 发送缓存区
extern uint8_t 	g_rxCurrentIndex1;					// 当前接收数据存放的位置索引，基于0
extern volatile uint8_t 	g_rxOver1;							// 接收完成，数据以"\r\n"(0x0D0xA)表示结尾
extern uint8_t 	g_txDataLen1;						// 要发送数据的长度
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 串口2收发缓存区
extern char g_rxBuffer2[USART_RX_BUFFER_LEN];		// 接收缓存区
extern char g_txBuffer2[USART_TX_BUFFER_LEN];		// 发送缓存区
extern uint8_t 	g_rxCurrentIndex2;					// 当前接收数据存放的位置索引，基于0
extern uint8_t	g_rxLengthToRead2;					// 准备要读取的数据长度
extern volatile uint8_t 	g_rxOver2;							// 接收完成，数据以"\r\n"(0x0D0xA)表示结尾
extern uint8_t 	g_txDataLen2;						// 要发送数据的长度
	
//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 串口标记
typedef enum {
	BEUSARTNumber1			= 0x0001,		// 串口1 TX：GPIOA_Pin_9  RX：GPIOA_Pin_10
	BEUSARTNumber2			= 0x0002		// 
}BEUSARTNumber;	
	
/*****************************************************************************************
	<< --- usart_config		2015-10-20 --- >>
	说明：串口配置
	参数：
		com			= 需要开启串口，基于1
		baudRate	= 波特率 如：115200
		dataLength	= 数据位 如：8
		stopBits	= 停止位 如：1
		parity		= 奇偶校验 如：0
	返回值：
*****************************************************************************************/
extern void usart_config(BEUSARTNumber com, uint32_t baudRate, uint16_t dataLength, uint16_t stopBits, uint16_t parity);
	
/*****************************************************************************************
	<< --- usart_print		2015-10-20 --- >>
	说明：往串口输出数据
	参数：
		com		= 串口
		pinfo	= 字符串
	返回值：
*****************************************************************************************/
extern void usart_print(BEUSARTNumber com, char *pinfo);
extern void usart_printData(BEUSARTNumber com, char *pinfo, uint8_t infoLen);
extern void usart_printHex(BEUSARTNumber com, char *pinfo, uint8_t infoLen);

#ifdef __cplusplus
}
#endif

#endif	// __USART_H__
