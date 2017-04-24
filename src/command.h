#ifndef __HLP_UART_COMMAND_H__
#define __HLP_UART_COMMAND_H__

#include <hlp_core.h>

typedef enum Command_type
{
	STB_REBOOT = 0x10,          /* 重启设备 */
	STB_HeartBeat,              /* 心跳 */
	STB_GetSysStatus,           /* 资源使用率 */
	STB_GetBaseInfo,            /* 获取基本信息 */
	STB_GetSystemInfo,          /* 获取系统信息 */
	STB_GetPlayInfo             /* 当前播放信息 */
}CommandType;

typedef struct _STB_SystemInfo
{
	hlp_u16_t   SWVER;             /* 软件版本 */
	hlp_u16_t   HWVER;             /* 硬件版本 */
	hlp_u8_t    TEVER[16];         /* 终端版本号 */ 
	hlp_u8_t    MIDVER[32];        /* 中间件版本 */ 
	hlp_u8_t    UIVER[16];         /* UI版本 */
	hlp_u32_t   STBID;             /* 序列号 */
	hlp_u8_t    ProduceYear;       /* 生产年 */
	hlp_u8_t    ProduceWeek;       /* 生产周 */
	hlp_u16_t   BathNo;            /* 批次 */
	hlp_u32_t   CA_vendor;         /* CA厂商 */
	hlp_u32_t   CA_version;        /* CA版本 */
	hlp_u32_t   IpAddr;            /* IP地址 */
	hlp_u8_t    MacAddr[16];       /* Mac地址 */
	hlp_u8_t    Description[32];
}STB_SystemInfo;

typedef struct _STB_SysStatus
{
	hlp_u16_t   cpu;               /* cpu */
	hlp_u16_t   mem;               /* 内存 */
	hlp_u8_t    io[16];            /* io */ 
}STB_SysStatus;

typedef struct _UartCommand
{
	hlp_u16_t             dataLen;
	CommandType           Cmd_type;
	hlp_u8_t              Cmd_args;
	hlp_u8_t              checksum;
	hlp_u8_t              CRLF;
	
}UartCommand;

#endif /* __HLP_UART_COMMAND_H__ */

