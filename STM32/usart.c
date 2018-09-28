#include "usart.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"


//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 ����1�շ�������
char g_rxBuffer1[USART_RX_BUFFER_LEN] = {0};		// ���ջ�����
char g_txBuffer1[USART_TX_BUFFER_LEN] = {0};		// ���ͻ�����
uint8_t g_rxCurrentIndex1 = 0;					// ��ǰ�������ݴ�ŵ�λ������������0
volatile uint8_t g_rxOver1 = false;						// �������
uint8_t g_txDataLen1 = 0;						// Ҫ�������ݵĳ���

//////////////////////////////////////////////////////////////////////////////////////////
//	2015-10-20 ����2�շ�������
char g_rxBuffer2[USART_RX_BUFFER_LEN] = {0};		// ���ջ�����
char g_txBuffer2[USART_TX_BUFFER_LEN] = {0};		// ���ͻ�����
uint8_t g_rxCurrentIndex2 = 0;					// ��ǰ�������ݴ�ŵ�λ������������0
uint8_t	g_rxLengthToRead2 = 0;					// ׼��Ҫ��ȡ�����ݳ���
volatile uint8_t g_rxOver2 = false;						// �������
uint8_t g_txDataLen2 = 0;						// Ҫ�������ݵĳ���

/*****************************************************************************************
	<< --- usart_getUSARTx		2015-10-20 --- >>
	˵��������ѡȡ
	������
		com	= ���ڱ��
	����ֵ����Ӧ����USARTx
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
	˵�������ô����ж�
	������
		usart_irqn	= �����жϺ�
	����ֵ��
*****************************************************************************************/
static void usart_nvic_config(uint8_t usart_irqn, uint8_t priority)
{
	/*  �ṹ����*/
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */ 
	/* Configure one bit for preemption priority */
	/* ���ȼ��� ˵������ռ���ȼ����õ�λ��������Ӧ���ȼ����õ�λ��   ��������0�� 4 
		0�飺  ��ռ���ȼ�ռ0λ�� ��Ӧ���ȼ�ռ4λ
		1�飺  ��ռ���ȼ�ռ1λ�� ��Ӧ���ȼ�ռ3λ
		2�飺  ��ռ���ȼ�ռ2λ�� ��Ӧ���ȼ�ռ2λ
		3�飺  ��ռ���ȼ�ռ3λ�� ��Ӧ���ȼ�ռ1λ
		4�飺  ��ռ���ȼ�ռ4λ�� ��Ӧ���ȼ�ռ0λ  
	*/   			     
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	    

	NVIC_InitStructure.NVIC_IRQChannel = usart_irqn;			    //���ô���1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	   	//��ռ���ȼ� 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = priority;				//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ��
	NVIC_Init(&NVIC_InitStructure);
}

/*****************************************************************************************
	<< --- usart1_config		2015-10-20 --- >>
	˵�������ô���1
	������
	����ֵ��
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
	/* Ĭ�ϸ��ù��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         		//USART1 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		 	//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		    		 	//A�˿� 
	/* ���ù��ܵ��������ű�������Ϊ����ģʽ������/����/������һ�֣�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	         	 	//USART1 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   	//���ø�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);		         	 	//A�˿� 
	// usart1
	USART_InitStructure.USART_BaudRate = baudRate;				//����115200bps
	USART_InitStructure.USART_WordLength = dataLength;			//����λ8λ
	USART_InitStructure.USART_StopBits = stopBits;				//ֹͣλ1λ
	USART_InitStructure.USART_Parity = parity;					//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	/* Configure USART1 */
	USART_Init(USART1, &USART_InitStructure);					//���ô��ڲ�������
 
  
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);				//ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);				//ʹ�ܷ��ͻ�����ж�   

	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

/*****************************************************************************************
	<< --- usart2_config		2015-10-20 --- >>
	˵�������ô���2
	������
	����ֵ��
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//A�˿� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//USART2 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//���ÿ�©����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//A�˿� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//RS485�����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOD, GPIO_Pin_12);						//RS485���ģʽ ��ֹ485����

	// usart2
	USART_InitStructure.USART_BaudRate = baudRate;			//����115200bps
	USART_InitStructure.USART_WordLength = dataLength;		//����λ8λ
	USART_InitStructure.USART_StopBits = stopBits;			//ֹͣλ1λ
	USART_InitStructure.USART_Parity = parity;				//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);				//���ô��ڲ�������
 
  
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);			//ʹ�ܽ����ж�
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);			//ʹ�ܷ��ͻ�����ж�   

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

