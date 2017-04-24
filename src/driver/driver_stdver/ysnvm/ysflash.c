/******************************************************************************
 *
 * 描述   :
 *
 * 创建人 :
 *
 *****************************************************************************/

/********************************* include ***********************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hi_type.h"
#include "hi_flash.h"
#include "drv_os.h"
#include "drv_memory.h"

/************************** macro & enum & struct ****************************/
/********************************** extern ***********************************/
/********************************* internal **********************************/
static HI_HANDLE flash_fd = INVALID_FD;

HI_S32 DRV_FLASH_OPEN(char *flash_name)
{
#ifdef EMMC_FLASH  //eMMC or NAND flash
    flash_fd = HI_Flash_Open(HI_FLASH_TYPE_EMMC_0, flash_name, 0, 0);
#else
    flash_fd = HI_Flash_Open(HI_FLASH_TYPE_NAND_0, flash_name, 0, 0);
#endif
    if(INVALID_FD == flash_fd)
    {
        printf("HI_Flash_Open error\n");
        return -1;
    }
	
    return 0;
}

HI_S32 DRV_FLASH_CLOSE()
{
    HI_S32 ret;
    ret = HI_Flash_Close(flash_fd);
    if(INVALID_FD == ret)
    {
        printf("HI_Flash_Close error\n");
        return -1;
    }
	
    return 0;
}

HI_S32 DRV_FLASH_READ(HI_U64 StartAddr, unsigned char *ReadBuff, HI_U64 len)
{
    HI_S32 ret;
    ret = HI_Flash_Read(flash_fd, StartAddr, ReadBuff, (HI_U32)len, HI_FLASH_RW_FLAG_RAW); 
    if (0 >=ret)
    {
        if (HI_FLASH_END_DUETO_BADBLOCK != ret)
        {
            printf("> %d Line: Flash Read failure(ret=%#x)!\n", __LINE__, ret); 
            return -1;
        }
		else
		{
			return 1;    //遇到坏块退出
		}
    }

    return 0;
}

HI_S32 DRV_FLASH_WRITE(HI_U64 StartAddr, unsigned char *WBuff, HI_U64 len)
{
    HI_S32 ret;
    ret = HI_Flash_Write(flash_fd, StartAddr, WBuff, (HI_U32)len, HI_FLASH_RW_FLAG_RAW);
    if (0 >= ret)
    {
        if (HI_FLASH_END_DUETO_BADBLOCK != ret)
        {
            printf("> %d Line: Flash Write failure(ret=%#x)!\n", __LINE__, ret); 
            return -1;
        } 
		else
		{
			return 1;    //遇到坏块退出
    }    	    	
    } 
	
    return 0;
}

HI_S32 DRV_FLASH_ERASE(HI_U64 StartAddr, HI_U64 len)
{
#ifdef EMMC_FLASH  //eMMC or NAND flash
    //return 0;
    
	/*
        *       modify by wwqing
	*	Emmc flash can not be erase, 
	*	So HI_Flash_Write() instead of HI_Flash_Erase()
	*/
	unsigned char *WBuff = NULL;
	HI_S32 ret = 0;

	extern yspartition_t *SystemPartition;

	WBuff = (unsigned char *)DRV_OSMalloc(SystemPartition, len);
	if(NULL == WBuff)
	{
		printf("[ERROR]> %d Line: %s, malloc(0x%llx) failed!!\n", __LINE__, __func__, len);
		return -1;
	}
	memset(WBuff, 0xFF, len);

	ret = DRV_FLASH_WRITE(StartAddr, WBuff, len);
	printf("[INFO]> %d Line: %s, Write 0xFF to StartAddr[0x%llx],  len[0x%llx], ret[%d]!!\n", __LINE__, __func__, StartAddr, len, ret);

	DRV_OSFree(SystemPartition, WBuff);
	WBuff = NULL;
	return ret;
	
#else
    HI_S32 ret;
    ret = HI_Flash_Erase(flash_fd, StartAddr, len);
    if (0 >= ret)
    {
        if (HI_FLASH_END_DUETO_BADBLOCK != ret)
        {
            printf("> %d Line: HI_Flash_Erase failure(ret=%#x)!\n", __LINE__, ret); 
            return -1;
        } 
		else
		{
			return 1;    //遇到坏块退出
		}
    }    	    	

    return 0;
#endif  //eMMC or NAND flash
}

