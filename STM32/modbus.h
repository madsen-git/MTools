#ifndef __MODBUS_H__
#define __MODBUS_H__
/*****************************************************************************************
*	����modbus rtu��ͨ��
*
*
*
*
*
*
*****************************************************************************************/
#include "basedef.h"

#ifdef __cplusplus
extern "C" {
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MODBUS

/* Internal use */
#define MSG_LENGTH_UNDEFINED -1
/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 1 page 12)
 * Quantity of Coils to read (2 bytes): 1 to 2000 (0x7D0)
 * (chapter 6 section 11 page 29)
 * Quantity of Coils to write (2 bytes): 1 to 1968 (0x7B0)
 */
#define MODBUS_MAX_READ_BITS              2000
#define MODBUS_MAX_WRITE_BITS             1968

/* Modbus_Application_Protocol_V1_1b.pdf (chapter 6 section 3 page 15)
 * Quantity of Registers to read (2 bytes): 1 to 125 (0x7D)
 * (chapter 6 section 12 page 31)
 * Quantity of Registers to write (2 bytes) 1 to 123 (0x7B)
 * (chapter 6 section 17 page 38)
 * Quantity of Registers to write in R/W registers (2 bytes) 1 to 121 (0x79)
 */
#define MODBUS_MAX_READ_REGISTERS          125
#define MODBUS_MAX_WRITE_REGISTERS         123
#define MODBUS_MAX_RW_WRITE_REGISTERS      121

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (7) + function (1) + address (2) + number (2)
 * - HEADER_LENGTH_RTU (1) + function (1) + address (2) + number (2) + CRC (2)
 */
#define _MIN_REQ_LENGTH 12

#define _REPORT_SLAVE_ID 180

#define _MODBUS_EXCEPTION_RSP_LENGTH 5
/* Max between RTU and TCP max adu length (so TCP) */
#ifdef __ARM__
	#define MAX_MESSAGE_LENGTH 260
#else
	#define MAX_MESSAGE_LENGTH 16
#endif
/* Timeouts in microsecond (0.5 s) */
#define _RESPONSE_TIMEOUT    500000
#define _BYTE_TIMEOUT        500000

/* Function codes */
#define _FC_READ_COILS                0x01
#define _FC_READ_DISCRETE_INPUTS      0x02
#define _FC_READ_HOLDING_REGISTERS    0x03
#define _FC_READ_INPUT_REGISTERS      0x04
#define _FC_WRITE_SINGLE_COIL         0x05
#define _FC_WRITE_SINGLE_REGISTER     0x06
#define _FC_READ_EXCEPTION_STATUS     0x07
#define _FC_WRITE_MULTIPLE_COILS      0x0F
#define _FC_WRITE_MULTIPLE_REGISTERS  0x10
#define _FC_REPORT_SLAVE_ID           0x11
#define _FC_WRITE_AND_READ_REGISTERS  0x17

// modbus-rtuר��
#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2
// modbus-tcpר��
#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0
/*********************************************************************
-- ���ܣ�������ȡ���ּĴ������ݵı���ADU
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ������ı����ֽ���
-- ������xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_buildRequestByReadRegisters(uint8_t slave, uint8_t addr, uint8_t regnums, uint8_t *preqbuf, uint8_t reqbuflen);
/*********************************************************************
-- ���ܣ�������ȡ����Ĵ������ݵı���ADU
-- ������slave=�������豸��ַ
--		 addr=�Ĵ�������ʼ��ַ
--		 regnums=Ҫ��ȡ�Ĵ�����������ÿ���Ĵ���2Byte
--		 preqbuf=��Ź�����������
--		 reqbuflen=�����С
-- ���أ������ı����ֽ���
-- ������xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_buildRequestByInputRegisters(uint8_t slave, uint8_t addr, uint8_t regnums, uint8_t *preqbuf, uint8_t reqbuflen);
/*********************************************************************
-- ���ܣ�����Ӧ����ADU�н������Ĵ�����ֵ
-- ������req=�Ѿ����͵�������
--		 rsp=�յ�����Ӧ����
--		 rsplen=��Ӧ���ĳ���
--		 pregval=��żĴ���ֵ��ÿ���Ĵ���ռ��2Byte
--		 regvallen=������pregval��ά��
-- ���أ�pregval��Ч���ݳ���
-- ������xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_resolveDataFromResponseRegisters(uint8_t *req, uint8_t *rsp, uint8_t rsplen, uint32_t *pregval, uint8_t regvallen);
/*********************************************************************
-- ���ܣ�����Ӧ����ADU�н���������ֵ
-- ������rsp=�յ�����Ӧ����
--		 rsplen=��Ӧ���ĳ���
--		 addr=�豸��ַ
-- ���أ���ų��صĲ���ֵ��ռ��4Byte
-- ������xu,2013-10-14
**********************************************************************/
extern int32_t rtu_ads_ResolveData(uint8_t *rsp, uint8_t rsplen, uint8_t *addr);
	
	
#ifdef __cplusplus
}
#endif


#endif // __MODBUS_H__

