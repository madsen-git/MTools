#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"


//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 串口1收发缓存区
char g_rxBuffer1[USART_RX_BUFFER_LEN] = {0};		// 接收缓存区
char g_txBuffer1[USART_TX_BUFFER_LEN] = {0};		// 发送缓存区
uint8_t g_rxCurrentIndex1 = 0;					// 当前接收数据存放的位置索引，基于0
volatile uint8_t g_rxOver1 = false;						// 接收完成
uint8_t g_txDataLen1 = 0;						// 要发送数据的长度

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 串口2收发缓存区
char g_rxBuffer2[USART_RX_BUFFER_LEN] = {0};		// 接收缓存区
char g_txBuffer2[USART_TX_BUFFER_LEN] = {0};		// 发送缓存区
uint8_t g_rxCurrentIndex2 = 0;					// 当前接收数据存放的位置索引，基于0
uint8_t	g_rxLengthToRead2 = 0;					// 准备要读取的数据长度
volatile uint8_t g_rxOver2 = false;						// 接收完成
uint8_t g_txDataLen2 = 0;						// 要发送数据的长度

/*****************************************************************************************
	<< --- usart_getUSARTx		2015-10-20 --- >>
	说明：串口选取
	参数：
		com	= 串口标记
	返回值：对应串口USARTx
*****************************************************************************************/
static USART_TypeDef* usart_getUSARTx(BEUSARTNumber com)
{
	USART_TypeDef *usartx = NULL;
	switch((int)com)
	{
	case BEUSARTNumber1:
		{
			usartx = USART1;
		}
		break;
	case BEUSARTNumber2:
		{
			usartx = USART2;
		}
		break;
	default:
		usartx = NULL;
		break;
	}
	
	return usartx;
}

/*****************************************************************************************
	<< --- nvic_config		2015-10-20 --- >>
	说明：配置串口中断
	参数：
		usart_irqn	= 串口中断号
	返回值：
*****************************************************************************************/
static void usart_nvic_config(uint8_t usart_irqn, uint8_t priority)
{
	/*  结构声明*/
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */ 
	/* Configure one bit for preemption priority */
	/* 优先级组 说明了抢占优先级所用的位数，和响应优先级所用的位数   在这里是0， 4 
		0组：  抢占优先级占0位， 响应优先级占4位
		1组：  抢占优先级占1位， 响应优先级占3位
		2组：  抢占优先级占2位， 响应优先级占2位
		3组：  抢占优先级占3位， 响应优先级占1位
		4组：  抢占优先级占4位， 响应优先级占0位  
	*/   			     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

	NVIC_InitStructure.NVIC_IRQChannel = usart_irqn;			    //设置串口1中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   	//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = priority;				//子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能
	NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************************
	<< --- usart1_config		2015-10-20 --- >>
	说明：配置串口1
	参数：
	返回值：
*****************************************************************************************/
static void usart1_config(uint32_t baudRate, uint16_t dataLength, uint16_t stopBits, uint16_t parity)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// rcc
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	// nvic
	usart_nvic_config(USART1_IRQn, 0);
	// gpio
	/* 默认复用功能 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		//USART1 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 	//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 	//A端口 
	/* 复用功能的输入引脚必须配置为输入模式（浮空/上拉/下拉的一种）*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 	//USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	//复用浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 	//A端口 
	// usart1
	USART_InitStructure.USART_BaudRate = baudRate;				//速率115200bps
	USART_InitStructure.USART_WordLength = dataLength;			//数据位8位
	USART_InitStructure.USART_StopBits = stopBits;				//停止位1位
	USART_InitStructure.USART_Parity = parity;					//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);					//配置串口参数函数
 
  
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				//使能接收中断
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				//使能发送缓冲空中断   

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

/*****************************************************************************************
	<< --- usart2_config		2015-10-20 --- >>
	说明：配置串口2
	参数：
	返回值：
*****************************************************************************************/
static void usart2_config(uint32_t baudRate, uint16_t dataLength, uint16_t stopBits, uint16_t parity)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// rcc
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOD |RCC_APB2Periph_AFIO  , ENABLE); 
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);  
	// nvic
	usart_nvic_config(USART2_IRQn, 1);
	// gpio
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//USART2 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//A端口 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//复用开漏输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//A端口 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//RS485输入输出控制
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOD, GPIO_Pin_12);						//RS485输出模式 禁止485输入

	// usart2
	USART_InitStructure.USART_BaudRate = baudRate;			//速率115200bps
	USART_InitStructure.USART_WordLength = dataLength;		//数据位8位
	USART_InitStructure.USART_StopBits = stopBits;			//停止位1位
	USART_InitStructure.USART_Parity = parity;				//无校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件流控
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);				//配置串口参数函数
 
  
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//使能接收中断
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);			//使能发送缓冲空中断   

	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);
}
void usart_config(BEUSARTNumber com, uint32_t baudRate, uint16_t dataLength, uint16_t stopBits, uint16_t parity)
{
	if(com == BEUSARTNumber1)
		usart1_config(baudRate, dataLength, stopBits, parity);
	else if(com == BEUSARTNumber2)
		usart2_config(baudRate, dataLength, stopBits, parity);

}

void usart_print(BEUSARTNumber com, char *pinfo)
{
	//assert_param(pinfo);
	//uint8_t idx = 0;
	USART_TypeDef *usartx = NULL;
	usartx = usart_getUSARTx(com);
	//assert_param(usartx);
	if(!usartx || !pinfo) return;
	usartx->SR;
	while(*pinfo != 0)
	{
		USART_SendData(usartx, *pinfo++);
		while(USART_GetFlagStatus(usartx, USART_FLAG_TC)==RESET);
	}
}

void usart_printData(BEUSARTNumber com, char *pinfo, uint8_t infoLen)
{
	uint8_t idx = 0;
	USART_TypeDef *usartx = NULL;
	usartx = usart_getUSARTx(com);
	//assert_param(usartx);
	if(!usartx || !pinfo || infoLen == 0) return;
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
	usartx->SR;
	for(; idx < infoLen; ++idx)
	{
		USART_SendData(usartx, pinfo[idx]);
		while(USART_GetFlagStatus(usartx, USART_FLAG_TC)==RESET);
	}
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);  
}
void usart_printHex(BEUSARTNumber com, char *pinfo, uint8_t infoLen)
{
	//assert_param(pinfo);
	uint8_t idx = 0;
	USART_TypeDef *usartx = NULL;
	char pbuf[3] = {0};
	usartx = usart_getUSARTx(com);
	//assert_param(usartx);
	if(!usartx || !pinfo || infoLen == 0) return;
	usartx->SR;
	for(; idx < infoLen; ++idx)
	{
		sprintf(pbuf, "%02X", pinfo[idx]);
		USART_SendData(usartx, pbuf[0]);
		while(USART_GetFlagStatus(usartx, USART_FLAG_TC)==RESET);
		USART_SendData(usartx, pbuf[1]);
		while(USART_GetFlagStatus(usartx, USART_FLAG_TC)==RESET);
	}
}

