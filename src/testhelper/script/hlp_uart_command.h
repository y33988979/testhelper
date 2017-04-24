/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_UART_COMMAND_H__
#define __HLP_UART_COMMAND_H__

#include <hlp_core.h>

typedef enum Command_type
{
	STB_REBOOT = 0x10,          /* �����豸 */
	STB_HeartBeat,              /* ���� */
	STB_GetSysStatus,           /* ��Դʹ���� */
	STB_GetBaseInfo,            /* ��ȡ������Ϣ */
	STB_GetSystemInfo,          /* ��ȡϵͳ��Ϣ */
	STB_GetPlayInfo             /* ��ǰ������Ϣ */
}CommandType;

typedef struct _STB_SystemInfo
{
	hlp_u16_t   SWVER;             /* ����汾 */
	hlp_u16_t   HWVER;             /* Ӳ���汾 */
	hlp_u8_t    TEVER[16];         /* �ն˰汾�� */
	hlp_u8_t    MIDVER[32];        /* �м���汾 */ 
	hlp_u8_t    UIVER[16];         /* UI�汾 */
	hlp_u32_t   STBID;             /* ���к� */
	hlp_u8_t    ProduceYear;       /* ������ */
	hlp_u8_t    ProduceWeek;       /* ������ */
	hlp_u16_t   BathNo;            /* ���� */
	hlp_u32_t   CA_vendor;         /* CA���� */
	hlp_u32_t   CA_version;        /* CA�汾 */
	hlp_u32_t   IpAddr;            /* IP��ַ */
	hlp_u8_t    MacAddr[16];       /* Mac��ַ */
	hlp_u8_t    Description[32];
}STB_SystemInfo;

typedef struct _STB_SysStatus
{
	hlp_u16_t   cpu;               /* cpu */
	hlp_u16_t   mem;               /* memory */
	hlp_u8_t    io[16];            /* io */ 
}STB_SysStatus;

typedef struct _UartCommand
{
	hlp_u16_t             dataLen;
	CommandType           Cmd_type;
	hlp_u8_t              Cmd_args;
	hlp_u8_t              checksum;
	hlp_u8_t              m_CRLF;
	
}UartCommand;



#endif /* __HLP_UART_COMMAND_H__ */

