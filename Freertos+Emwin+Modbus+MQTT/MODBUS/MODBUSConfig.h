#ifndef __MODBUSCONFIG_H
#define __MODBUSCONFIG_H



#define SADDR485	1
#define SBAUD485	UART3_BAUD

/* 01H 读强制单线圈 */
/* 05H 写强制单线圈 */
#define REG_D01		0x0101
#define REG_D02		0x0102
#define REG_D03		0x0103
#define REG_D04		0x0104
#define REG_DXX 	REG_D04

/* 02H 读取输入状态 */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */
#define SLAVE_REG_P01		0x0301
#define SLAVE_REG_P02		0x0302

/* 04H 读取输入寄存器(模拟信号) */
#define REG_A01		0x0401
#define REG_AXX		REG_A01


/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128



#endif



