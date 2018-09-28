#ifndef __MODBUS_H__
#define __MODBUS_H__
/*****************************************************************************************
*	基于modbus rtu的通信
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

// modbus-rtu专用
#define _MODBUS_RTU_HEADER_LENGTH      1
#define _MODBUS_RTU_PRESET_REQ_LENGTH  6
#define _MODBUS_RTU_PRESET_RSP_LENGTH  2

#define _MODBUS_RTU_CHECKSUM_LENGTH    2
// modbus-tcp专用
#define _MODBUS_TCP_HEADER_LENGTH      7
#define _MODBUS_TCP_PRESET_REQ_LENGTH 12
#define _MODBUS_TCP_PRESET_RSP_LENGTH  8

#define _MODBUS_TCP_CHECKSUM_LENGTH    0
/*********************************************************************
-- 功能：构建读取保持寄存器数据的报文ADU
-- 参数：slave=被访问设备地址
--		 addr=寄存器的起始地址
--		 regnums=要读取寄存器的数量。每个寄存器2Byte
--		 preqbuf=存放构建的请求报文
--		 reqbuflen=缓存大小
-- 返回：构建的报文字节数
-- 描述：xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_buildRequestByReadRegisters(uint8_t slave, uint8_t addr, uint8_t regnums, uint8_t *preqbuf, uint8_t reqbuflen);
/*********************************************************************
-- 功能：构建读取输入寄存器数据的报文ADU
-- 参数：slave=被访问设备地址
--		 addr=寄存器的起始地址
--		 regnums=要读取寄存器的数量。每个寄存器2Byte
--		 preqbuf=存放构建的请求报文
--		 reqbuflen=缓存大小
-- 返回：构建的报文字节数
-- 描述：xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_buildRequestByInputRegisters(uint8_t slave, uint8_t addr, uint8_t regnums, uint8_t *preqbuf, uint8_t reqbuflen);
/*********************************************************************
-- 功能：从响应报文ADU中解析出寄存器的值
-- 参数：req=已经发送的请求报文
--		 rsp=收到的响应报文
--		 rsplen=响应报文长度
--		 pregval=存放寄存器值，每个寄存器占有2Byte
--		 regvallen=缓存区pregval的维数
-- 返回：pregval有效数据长度
-- 描述：xu,2013-06-14
**********************************************************************/
extern uint8_t rtu_resolveDataFromResponseRegisters(uint8_t *req, uint8_t *rsp, uint8_t rsplen, uint32_t *pregval, uint8_t regvallen);
/*********************************************************************
-- 功能：从响应报文ADU中解析出测量值
-- 参数：rsp=收到的响应报文
--		 rsplen=响应报文长度
--		 addr=设备地址
-- 返回：存放称重的测量值，占有4Byte
-- 描述：xu,2013-10-14
**********************************************************************/
extern int32_t rtu_ads_ResolveData(uint8_t *rsp, uint8_t rsplen, uint8_t *addr);
	
	
#ifdef __cplusplus
}
#endif


#endif // __MODBUS_H__

