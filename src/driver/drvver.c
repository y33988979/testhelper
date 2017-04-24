/*******************************************************************************

********************************************************************************/
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "sttbx.h"
#include "drv_typedef.h"
#include "stddef.h"
#define LOADER_VER_MAGIC_ADDR 0x4004FFD0
#define LOADER_VER_MAGIC_DATA 0x45565748 /*new loader*/
#ifndef ROM_TAG_SIZE
#define ROM_TAG_SIZE 1024
#endif

//�޸İ汾��ʱͬʱ�޸����������ط�,��һ������ֱ�Ӵӿ���鿴
//DrvVersion:0x152 �������粥�ţ���꣬tuner����Ӧ֧��
static const unsigned int g_DrvVersion  = DRVVER;//0x152;

unsigned int GetDrvVersion(void)
{
	return g_DrvVersion;
}

unsigned int DRV_GetHardwareVersion(void)
{
	return HWVER;
}

void ShowDrvVersion(void )
{
	STTBX_Print(("g_DrvVersion = 0x%x",g_DrvVersion));
	STTBX_Print(("    Build: %s (%s %s)\n",BUILDER, __TIME__,__DATE__));
//	STTBX_Print("ToolSet: %s\n", __ICC_VERSION_STRING__);
}


