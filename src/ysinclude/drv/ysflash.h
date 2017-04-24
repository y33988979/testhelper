/*****************************************************************************
*
* Description: 
*    驱动层存储模块FLASH头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1  2004-03-17    yzb            创建
*
*****************************************************************************/
#ifndef _YSFLASH_H_
#define _YSFLASH_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

int DRV_FLASH_OPEN(char *flash_name);
int DRV_FLASH_CLOSE();
int DRV_FLASH_READ(HI_U64 StartAddr, unsigned char *ReadBuff, HI_U64 len);
int DRV_FLASH_WRITE(HI_U64 StartAddr, unsigned char *WBuff, HI_U64 len);
int DRV_FLASH_ERASE(HI_U64 StartAddr, HI_U64 len);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
