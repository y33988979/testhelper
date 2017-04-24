/*****************************************************************************
*
* Description: 
*������洢ģ��FLASH��ȡ����ʵ���ļ�
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2006-10-09    lxk            ����
*
*
*****************************************************************************/
#include <string.h>

#include <stdio.h>
#include <unistd.h>  /*call shell command "mount" */
#include <errno.h>
#include <stddef.h>

#include "ysnvm.h"
#include "ysfdata.h"
#include "drv_debug.h"
#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/types.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <mtd/mtd-user.h>

#include "hi_type.h"
#include "hi_flash.h"
#include "drv_os.h"



#ifdef __cplusplus
extern "C" {
#endif


#define CURRENT_MODULE DRV_MODULE_NVM

/* One sector of HY_AMD_ST flash is 128K Bytes */
#define FLASH_ONE_SECTOR_LENGTH  0x20000

#define FLASH_CHANNEL_DATA_SIZE  (32*1024)
#define FLASE_CA_DATA_SIZE       (64*1024) 
#define FLASE_MPG_DATA_SIZE      (64*1024) 
#define FLASH_BOOT_DATA_SIZE     (8*1024)
#define FLASH_PO_DATA_SIZE       (8*1024)
#define FLASH_STOCK_DATA_SIZE    (64*1024)
#define FLASH_ERROR_INDEX  0xFFFFFFFF

#define LOADER_START_ADDR    (0x00060000)
#define KERNEL_START_ADDR    (0x00400000)
#define APP_START_ADDR       (0x00800000)

void drv_htonl(HI_U32 *addr)
{
    HI_U32 len = 0;

    len |= (*addr  & 0x000000FF)<<24;
    len |= (*addr  & 0x0000FF00)<<8;
    len |= (*addr  & 0x00FF0000)>>8;
    len |= (*addr  & 0xFF000000)>>24;

    *addr = len;

    return;
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : AVIT_FlashRead
  Description     : ��ȡFlashData��AVITData����
  Input           : offset:��Ч��������Ϊ0
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode AVIT_FlashRead(U32_YS offset,  U32_YS Length, U8_YS* pData)
{
    FILE *ChannelData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
	if(offset != 0)
	{
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
	}
    
    ChannelData_fp   =   fopen("/FlashData/AVITData","r");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
    
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (ReadNum != Length)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }

	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : AVIT_FlashWrite
  Description     : ����AVITData���ݵ�FlashData
  Input           : offset:��Ч��������Ϊ0
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
DRV_ErrCode AVIT_FlashWrite(U32_YS offset,  U32_YS Length, U8_YS* pData)
{
    FILE *ChannelData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

    if(offset != 0)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
        
    ChannelData_fp = fopen("/FlashData/AVITData","w");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : BOOK_FlashRead
  Description     : ��ȡBOOK����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode BOOK_FlashRead(U8_YS* pData, U32_YS Length)
{
    FILE *BookData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BookData_fp = fopen("/FlashData/BookData","r");
    if(BookData_fp == NULL)
    {
        memset(pData,0,Length);
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
    
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,BookData_fp);
    fclose(BookData_fp);  
    if (ReadNum != Length) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : BOOK_FlashWrite
  Description     : д��BOOK����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode BOOK_FlashWrite(U8_YS * pData, U32_YS Length)
{
    FILE *BookData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BookData_fp = fopen("/FlashData/BookData","w");
    if(BookData_fp == NULL)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }  

    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,BookData_fp);
    fclose(BookData_fp);
    if (WriteNum != Length)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }

    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : HTRD_FlashRead
  Description     : ��ȡHTRDData����
  Input           : offset:��Ч��������Ϊ0
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode HTRD_FlashRead(U32_YS offset,  U32_YS Length, U8_YS* pData)
{
    FILE *ChannelData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

	if(offset != 0)
	{
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ChannelData_fp = fopen("/FlashData/HTRDData","r");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
 
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (ReadNum != Length)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ); 
    }

	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);  
}
    
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : HTRD_FlashWrite
  Description     : д��HTRDData����
  Input           : offset:��Ч��������Ϊ0
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode HTRD_FlashWrite(U32_YS offset,  U32_YS Length, U8_YS* pData)
{
    FILE *ChannelData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

	if(offset != 0)
	{
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
    
    ChannelData_fp = fopen("/FlashData/HTRDData","w");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}



/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadChannelData
  Description     : ��ȡƵ������.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadChannelData(U8_YS * pData,U32_YS Length)
 {
    FILE *ChannelData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    ChannelData_fp = fopen("/FlashData/ChannelData","r");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (ReadNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }

	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteChannelData
  Description     : д��Ƶ������.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteChannelData(U8_YS *pData,U32_YS Length)
 {
    FILE *ChannelData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    ChannelData_fp = fopen("/FlashData/ChannelData","w");
    if(ChannelData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,ChannelData_fp);
    fclose(ChannelData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadCaData
  Description     : ��ȡCA����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadCaData(U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    CaData_fp   =   fopen("/FlashData/CaData","r");
    if(CaData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (ReadNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadCaDataByName
  Description     : ��ȡCA����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadCaDataByName(const char *name, U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS ReadNum;
    char file_url[64];
    
    if(pData == NULL || NULL == name)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    sprintf(file_url, "/FlashData/%s", name);
    
    CaData_fp   =   fopen(file_url,"r");
    if(CaData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (ReadNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadCaData1
  Description     : ��ȡCA1����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadCaData1(U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    CaData_fp   =   fopen("/FlashData/CaData1","r");
    if(CaData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (ReadNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteCaData
  Description     : д��CA����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteCaData(U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    CaData_fp = fopen("/FlashData/CaData","w");
    if(CaData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (WriteNum != Length)     
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteCaDataByName
  Description     : д��CA����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteCaDataByName(const char *name, U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS WriteNum; 
    char file_url[64];
    
    if(pData == NULL || NULL == name)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    sprintf(file_url, "/FlashData/%s", name);
    
    CaData_fp = fopen(file_url,"w");
    if(CaData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (WriteNum != Length)     
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DRV_FLASH_WriteCaData1
  Description     : д��CA1����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteCaData1(U8_YS *pData,  U32_YS Length)
{
    FILE *CaData_fp = NULL;
    U32_YS WriteNum; 

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    CaData_fp = fopen("/FlashData/CaData1","w");
    if(CaData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,CaData_fp);
    fclose(CaData_fp);
    if (WriteNum != Length)     
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    } 
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DRV_FLASH_ReadBootData
  Description     : ��ȡBOOT����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadBootData(U8_YS *BootData_p,  U32_YS Length)
 {
	FILE *BootData_fp = NULL;
	U32_YS ReadNum;

    if(BootData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BootData_fp = fopen("/FlashData/BootData","r");
    if(BootData_fp == NULL) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)BootData_p,sizeof(U8_YS),Length,BootData_fp);
    fclose(BootData_fp);
    if (ReadNum != Length) 
    {    
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
   
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteBootData
  Description     : д��BOOT����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteBootData( U8_YS *BootData_p,  U32_YS Length)
 {
    FILE *BootData_fp = NULL;
    U32_YS WriteNum; 

    if(BootData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BootData_fp = fopen("/FlashData/BootData","w");
    if(BootData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)BootData_p,sizeof(U8_YS),Length,BootData_fp);
    fclose(BootData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadPoData
  Description     : ��ȡԤԼ����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadPoData(U8_YS *PoData_p,  U32_YS Length)
{
	FILE *PoData_fp = NULL;
	U32_YS ReadNum;

    if(PoData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    PoData_fp   =   fopen("/FlashData/PoData","r");
    if(PoData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)PoData_p,sizeof(U8_YS),Length,PoData_fp);
    fclose(PoData_fp);  
    if (ReadNum != Length) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WritePoData
  Description     : д��ԤԼ����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WritePoData( U8_YS *PoData_p,  U32_YS Length)
 {
    FILE *PoData_fp = NULL;
    U32_YS WriteNum;

    if(PoData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    PoData_fp = fopen("/FlashData/PoData","w");
    if(PoData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)PoData_p,sizeof(U8_YS),Length,PoData_fp);
    fclose(PoData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadStockData
  Description     : ��ȡStockData����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadStockData(U8_YS *PoData_p,  U32_YS Length)
{
    FILE *StockData_fp = NULL;
    U32_YS ReadNum;

    if(PoData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    StockData_fp = fopen("/FlashData/StockData","r");
    if(StockData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)PoData_p,sizeof(U8_YS),Length,StockData_fp);
    fclose(StockData_fp);  
    if (ReadNum != Length) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadStockData
  Description     : д��StockData����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteStockData( U8_YS *PoData_p,  U32_YS Length)
 {
    FILE *StockData_fp = NULL;
    U32_YS WriteNum; 

    if(PoData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    StockData_fp = fopen("/FlashData/StockData","w");
    if(StockData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)PoData_p,sizeof(U8_YS),Length,StockData_fp);
    fclose(StockData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadStockData
  Description     : ��ȡӦ�ó�������.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadAppData(U8_YS *AppData_p,  U32_YS Length)
{
    FILE *AppData_fp = NULL;
    U32_YS ReadNum;

    if(AppData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    AppData_fp = fopen("/FlashData/AppData","r");
    if(AppData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)AppData_p,sizeof(U8_YS),Length,AppData_fp);
    fclose(AppData_fp);  
    if (ReadNum != Length) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DRV_FLASH_WriteAppData
  Description     : д��Ӧ�ó�������.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteAppData( U8_YS *AppData_p,  U32_YS Length)
{
    FILE *AppData_fp = NULL;
    U32_YS WriteNum; 

    if(AppData_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    AppData_fp = fopen("/FlashData/AppData","w");
    if(AppData_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)AppData_p,sizeof(U8_YS),Length,AppData_fp);
    fclose(AppData_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-09-29
  Author          : xyan 
  Function Name   : DRV_FLASH_ErasePicture
  Description     : ������������ӿ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ErasePicture(void)
{	
    FILE *BootLogo_fp = NULL;
	U32 Mark[2];	
    U32_YS WriteNum; 
    
    BootLogo_fp = fopen("/FlashData/BootLogo","w");
    if(BootLogo_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    Mark[0]=0;
    Mark[1]=0; 
    WriteNum = fwrite((void *)Mark,sizeof(U32_YS),2,BootLogo_fp);
    fclose(BootLogo_fp);
    if (WriteNum != 2)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : xyan  
  Function Name   : DRV_FLASH_WritePicture
  Description     : д�뿪��ͼƬ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WritePicture(U8_YS * Data_p,U32_YS Length)
{
    FILE *BootLogo_fp = NULL;
	U32 Mark[2];	
    U32_YS WriteNum;

    if(Data_p == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BootLogo_fp = fopen("/FlashData/BootLogo","w");
    if(BootLogo_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    Mark[0]=0xBEEBEBEE;
    Mark[1]=Length; 
    WriteNum = fwrite((void *)Mark,sizeof(U32_YS),2,BootLogo_fp);
	if(WriteNum != 2)
	{
		fclose(BootLogo_fp); 
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
	}

    fseek(BootLogo_fp,8L,0);
    WriteNum = fwrite((void *)Data_p,sizeof(U8_YS),Length,BootLogo_fp);
    fclose(BootLogo_fp);
    if (WriteNum != Length)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }

	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan  
  Function Name   : DRV_FLASH_ReadPicture
  Description     : ��ȡ����ͼƬ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadPicture(U8_YS *pData, U32_YS Length)
{
    FILE *BootLogo_fp = NULL;	
    U32_YS ReadNum;

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    BootLogo_fp = fopen("/FlashData/BootLogo","r");
    if(BootLogo_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    fseek(BootLogo_fp,8L,0);
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,BootLogo_fp);
    fclose(BootLogo_fp);  
    if (ReadNum != Length) 
	{
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
	}

	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan  
  Function Name   : DRV_FLASH_ReadPictureLength
  Description     : ��ȡ��������ͼƬ���ݳ���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadPictureLength(U32_YS *pLength)
{
    FILE *BootLogo_fp = NULL;	
	U32 Mark[2];	
    U32_YS ReadNum;

    if(pLength == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

    BootLogo_fp = fopen("/FlashData/BootLogo","r");
    if(BootLogo_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    ReadNum = fread((void *)Mark,sizeof(U32),2,BootLogo_fp);
    fclose(BootLogo_fp);
    if(ReadNum != 2)  
    {
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }

	if((Mark[0] != 0xBEEBEBEE) || (Mark[1] > 128*1024))
	{
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
	}

	*pLength = Mark[1];
	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WritePicinBlock
  Description     : д��ͼƬ����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WritePicinBlock( U8_YS Blocknum,U8_YS *pData,U32_YS Length)
{
    FILE *PicinBlock_fp = NULL;
    U32_YS WriteNum; 
	U32 Mark[2];	
    char filename[30] = {0};

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

    Mark[0]=0xBEEBEBEE;
    Mark[1]=Length; 
    sprintf(filename, "/FlashData/PicinBlock%d", Blocknum);
    PicinBlock_fp  = fopen(filename,"w");
    if(PicinBlock_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)Mark,sizeof(U32_YS),2,PicinBlock_fp);
    if (WriteNum != 2)
    {
    	fclose(PicinBlock_fp);
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
	
    fseek(PicinBlock_fp,8L,0);
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,PicinBlock_fp);
    fclose(PicinBlock_fp);
    if (WriteNum != Length)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}


/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WritePicinBlock
  Description     : ��ȡͼƬ����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadPicinBlock(U8_YS Blocknum, U8_YS *pData, U32_YS Length)
{
	FILE *PicinBlock_fp = NULL;
	U32_YS ReadNum;
	char filename[30] = {0};

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    sprintf(filename, "/FlashData/PicinBlock%d", Blocknum);
    PicinBlock_fp = fopen(filename,"r");
    if(PicinBlock_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    fseek(PicinBlock_fp,8L,0);
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,PicinBlock_fp);
    fclose(PicinBlock_fp);  
    if (ReadNum != Length) 
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_ReadPicinBlockLength
  Description     : ��ȡͼƬ���ݳ���.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadPicinBlockLength(U8_YS Blocknum,U32_YS *pLength)
{
 	FILE *PicinBlock_fp = NULL;
    U32_YS ReadNum;
	U32 Mark[2];	
    char filename[30] = {0};

    if(pLength == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }

	sprintf(filename, "/FlashData/PicinBlock%d", Blocknum);
    PicinBlock_fp = fopen(filename,"r");
    if(PicinBlock_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }

    ReadNum = fread((void *)Mark,sizeof(U32),2,PicinBlock_fp);
    fclose(PicinBlock_fp);
    if(ReadNum != 2)
    {
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }

	if(Mark[0] != 0xBEEBEBEE)
	{
		YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
	}

	*pLength = Mark[1];
	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteDatainBlock
  Description     : д��DatainBlock����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteDatainBlock( U8_YS Blocknum, U8_YS *pData, U32_YS Length)
{
    FILE *DatainBlock_fp = NULL;
    U32_YS WriteNum; 
    char filename[30] = {0};

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    sprintf(filename, "/FlashData/DatainBlock%d", Blocknum);
    DatainBlock_fp = fopen(filename,"w");
    if(DatainBlock_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    WriteNum = fwrite((void *)pData,sizeof(U8_YS),Length,DatainBlock_fp);
    fclose(DatainBlock_fp);
    if (WriteNum != Length)
    {
    	YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_WRITE);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DRV_FLASH_ReadDatainBlock
  Description     : ��ȡDatainBlock����.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ReadDatainBlock(U8_YS Blocknum, U8_YS *pData, U32_YS Length)
{
    FILE *DatainBlock_fp = NULL;
    U32_YS ReadNum;
    char filename[30] = {0};

    if(pData == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);	
    }
    
    sprintf(filename, "/FlashData/DatainBlock%d", Blocknum);
    DatainBlock_fp = fopen(filename,"r");
    if(DatainBlock_fp == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_OPEN);
    }
        
    ReadNum = fread((void *)pData,sizeof(U8_YS),Length,DatainBlock_fp);
    fclose(DatainBlock_fp);  
    if (ReadNum != Length) 
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_NVM_ERROR_FLASH_FILE_READ);
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan 
  Function Name   : DRV_FLASH_WriteApp
  Description     : дAppӦ�ó���
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : Ŀǰû��ʵ��
*****************************************************************************/
DRV_ErrCode DRV_FLASH_WriteApp(U8_YS* pData, U32_YS Length)
{
	return DRV_NO_ERROR;
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : wrwei modify
  Function Name   : drv_FlashBadBlockCheck
  Description     : ������
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_FlashBadBlockCheck( U32_YS  Mtdnumber,U32_YS Offset,BOOL_YS *IsBadBlock)
{
	FILE *fp;
	char  Mtdpath[16]={'\0'};
	U64 offs;//������64λ���򱨴�
	struct erase_info_user MTD_EraseInfoUser;
	int ret;
	*IsBadBlock = FALSE;
	if(Offset%FLASH_ONE_SECTOR_LENGTH != 0)
	{ 
		printf("drv_FlashBadBlockCheck not align at 0x20000,Offset[%d]!\n",Offset);
		return DRV_ERROR_BAD_PARAMETER;
	}
	/*ע����ʹ��ioctl������±�����open��������fopen*/
	
	sprintf(Mtdpath,"/dev/mtd%d",Mtdnumber);
	
	fp =open(Mtdpath,O_RDWR|O_SYNC);
	if((fp == NULL) || (fp == -1))
	{
		printf("drv_FlashBadBlockCheck Device /dev/mtd%d  open  failed !\n",Mtdnumber);
		return DRV_ERROR_DEVICE_BUSY;
	}
	offs = Offset;
	MTD_EraseInfoUser.start  = Offset;
	MTD_EraseInfoUser.length = FLASH_ONE_SECTOR_LENGTH;
	//ret = ioctl((int)fp,MEMLOCK,&MTD_EraseInfoUser);
	//if (ret!=0)
    //{
	//	printf("drv_FlashBadBlockCheck MEMLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
	//	close(fp);
	//	return(ST_ERROR_BAD_PARAMETER);
    //}
    ret = ioctl((int)fp,MEMGETBADBLOCK,&offs);
	if (ret!=0)
    {
		printf("drv_FlashBadBlockCheck MEMGETBADBLOCK Device /dev/mtd%d  have bad block at offs 0x%08x!\n",Mtdnumber,Offset);
		*IsBadBlock = TRUE;
    }
    //ret = ioctl((int)fp,MEMUNLOCK,&MTD_EraseInfoUser);
	//if (ret!=0)
    //{
	//	printf("drv_FlashBadBlockCheck MEMUNLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
	//	close(fp);
	//	return(ST_ERROR_BAD_PARAMETER);
    //}

	if (close(fp)< 0) 
	{	
		printf("drv_FlashBadBlockCheck close Device  /dev/mtd%d  close  failed !\n",Mtdnumber);
		return   DRV_ERROR_DEVICE_BUSY;
	}

	return 	DRV_NO_ERROR;

}

/*���豸����*/
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : 
  Function Name   : drv_NvmReadFlashData
  Description     : ���豸������ȡFlashData(via mtd device)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_NvmReadFlashData( U32_YS  Mtdnumber,
                                  U32_YS Offset,
                                  U8_YS* Data_p,
                                  U32_YS Length)
{
	int fp;
	U32_YS ReadNum;
	U32_YS SectorNum,SectorPlus,index;
	U64 offs;//������64λ���򱨴�
	struct erase_info_user MTD_EraseInfoUser;
	int ret;
	U8_YS* DataRead_p;
	U32_YS Trytimes=0;
	U32_YS MaxTryTimes=5;
	char  Mtdpath[16]={'\0'};
	DataRead_p = Data_p;

	if((Offset%FLASH_ONE_SECTOR_LENGTH != 0)||(DataRead_p == NULL))
	{ 
		printf("drv_NvmReadFlashData not align at 0x20000,Offset[%d],Length[%d]!\n",Offset,Length);
		return DRV_ERROR_BAD_PARAMETER;
	}
	
	sprintf(Mtdpath,"/dev/mtd%d",Mtdnumber);
	fp = open(Mtdpath,O_RDWR|O_SYNC);
	if((fp == NULL) || (fp == -1))
	{
		printf("drv_NvmReadFlashData Device /dev/mtd%d  open  failed !\n",Mtdnumber);
		return DRV_ERROR_DEVICE_BUSY;
	}

    SectorNum = Length/FLASH_ONE_SECTOR_LENGTH;
    SectorPlus = Length%FLASH_ONE_SECTOR_LENGTH;

	for(index=0;index<SectorNum;index++)
	{
		offs = Offset + index*FLASH_ONE_SECTOR_LENGTH;
		//MTD_EraseInfoUser.start  = offs;
		//MTD_EraseInfoUser.length = FLASH_ONE_SECTOR_LENGTH;
		//ret = ioctl((int)fp,MEMLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	    ret = ioctl((int)fp,MEMGETBADBLOCK,&offs);
		if (ret!=0)
	    {
			printf("drv_NvmReadFlashData MEMGETBADBLOCK Device /dev/mtd%d  have bad block at offs 0x%08x,and jump it!\n",Mtdnumber,offs);
			SectorNum += 1;//����ǻ��飬��������һ��
	    }
	    else//�ÿ飬��ȥ��ȡ����
	    {
			lseek(fp,offs,SEEK_SET);
	    	if(index == SectorNum)
	    	{
				do
                {
					ReadNum = read(fp,DataRead_p,SectorPlus);
					Trytimes++;
				} while(Trytimes<MaxTryTimes&&ReadNum!=SectorPlus);
				DataRead_p += SectorPlus;
			}
			else
			{
				do
                {
					ReadNum = read(fp,DataRead_p,FLASH_ONE_SECTOR_LENGTH);
					Trytimes++;
				} while(Trytimes<MaxTryTimes&&ReadNum!=FLASH_ONE_SECTOR_LENGTH);
				DataRead_p += FLASH_ONE_SECTOR_LENGTH;
			}
	    }
	    //ret = ioctl((int)fp,MEMUNLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMUNLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	}
	if (close(fp)< 0) 
	{	
		printf("drv_NvmReadFlashData Device  /dev/mtd%d  close  failed !\n",Mtdnumber);
		return   DRV_ERROR_DEVICE_BUSY;
	}
	if(Trytimes>=MaxTryTimes )
	{
		return DRV_ERROR_TIMEOUT;
	}

	return 	DRV_NO_ERROR;

}
//���豸����
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : 
  Function Name   : drv_NvmEraseFlashData
  Description     : ���豸��������FlashData(via mtd device)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_NvmEraseFlashData( U32_YS  Mtdnumber,
                                  U32_YS Offset,
                                  U32_YS Length)
{
	int  fp;
	U32   SectorNum,index;
	U64   offs;
	int ret;
	char  Mtdpath[16]={'\0'};
	struct erase_info_user MTD_EraseInfoUser;

	if((Offset%FLASH_ONE_SECTOR_LENGTH != 0)||(Length%FLASH_ONE_SECTOR_LENGTH != 0))
	{ 
		printf("drv_NvmEraseFlashData not align at 0x20000,start[%d],Length[%d]!\n",Offset,Length);
		return DRV_ERROR_BAD_PARAMETER;
	}
	/*ע����ʹ��ioctl������±�����open��������fopen*/
	
	sprintf(Mtdpath,"/dev/mtd%d",Mtdnumber);
	fp =open(Mtdpath,O_RDWR|O_SYNC);
	if((fp == NULL) || (fp == -1)) 
	{
		printf("drv_NvmEraseFlashData Device /dev/mtd%d  open  failed !\n",Mtdnumber);
		return DRV_ERROR_DEVICE_BUSY;
	}
    
    SectorNum = Length/FLASH_ONE_SECTOR_LENGTH;
	for(index=0;index<SectorNum;index++)
	{
		offs = Offset + index*FLASH_ONE_SECTOR_LENGTH;
		MTD_EraseInfoUser.start  = offs;
		MTD_EraseInfoUser.length = FLASH_ONE_SECTOR_LENGTH;
		//ret = ioctl((int)fp,MEMLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	    ret = ioctl((int)fp,MEMGETBADBLOCK,&offs);
		if (ret!=0)
	    {
			printf("drv_NvmEraseFlashData MEMGETBADBLOCK Device /dev/mtd%d  have bad block at offs 0x%08x,and jump it!\n",Mtdnumber,offs);
			SectorNum += 1;//������ʱ���л���Ҳ������
	    }
	    else//�ÿ飬��ȥ���Բ���
	    {
	    	ret = ioctl((int)fp,MEMERASE,&MTD_EraseInfoUser);
			if (ret!=0)
		    {
				printf("drv_NvmEraseFlashData MEMERASE Device /dev/mtd%d  faild at offs 0x%08x,and we should mask it!\n",Mtdnumber,offs);
				ret = ioctl((int)fp,MEMSETBADBLOCK,&offs);
				if (ret!=0)
			    {
					printf("drv_NvmEraseFlashData MEMSETBADBLOCK Device /dev/mtd%d  faild at offs 0x%08x!\n",Mtdnumber,offs);
					close(fp);
					return(ST_ERROR_BAD_PARAMETER);
			    }
			    SectorNum += 1;//������ʱ���л���Ҳ������
		    }
	    }
	    //ret = ioctl((int)fp,MEMUNLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMUNLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	}

	if (close(fp)< 0) 
	{	
		printf("drv_NvmEraseFlashData close Device  /dev/mtd%d  close  failed !\n",Mtdnumber);
		return   DRV_ERROR_DEVICE_BUSY;
	}

	return 	DRV_NO_ERROR;

}

//���豸����
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : 
  Function Name   : drv_NvmWriteFlashData
  Description     : ���豸����д��FlashData(via mtd device)
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode drv_NvmWriteFlashData( U32_YS  Mtdnumber,
                                   U32_YS Offset,
                                  U8_YS* Data_p,
                                   U32_YS Length)
{
	int fp;
	U32_YS ReadNum;
	U32_YS Trytimes=0;
	U32_YS MaxTryTimes=5;
	char  Mtdpath[16]={'\0'};
	U64 offs;//������64λ���򱨴�
	struct erase_info_user MTD_EraseInfoUser;
	int ret;
	U32_YS SectorNum,SectorPlus,index;
	U8_YS* DataWrite_p;
    DataWrite_p = Data_p;
	
	if((Offset%FLASH_ONE_SECTOR_LENGTH != 0)||(DataWrite_p == NULL))
	{ 
		printf("drv_NvmReadFlashData not align at 0x20000,Offset[%d],Length[%d]!\n",Offset,Length);
		return DRV_ERROR_BAD_PARAMETER;
	}
	
	sprintf(Mtdpath,"/dev/mtd%d",Mtdnumber);
	fp =open(Mtdpath,O_RDWR|O_SYNC);
	lseek(fp,Offset,SEEK_SET);
	if((fp == NULL) || (fp == -1))
	{
		printf("drv_NvmWriteFlashData Device /dev/mtd%d  open  failed !\n",Mtdnumber);
		return DRV_ERROR_DEVICE_BUSY;
	}
    
    SectorNum = Length/FLASH_ONE_SECTOR_LENGTH;
    SectorPlus = Length%FLASH_ONE_SECTOR_LENGTH;
	for(index=0;index<SectorNum;index++)
	{
		offs = Offset + index*FLASH_ONE_SECTOR_LENGTH;
		//MTD_EraseInfoUser.start  = offs;
		//MTD_EraseInfoUser.length = FLASH_ONE_SECTOR_LENGTH;
		//ret = ioctl((int)fp,MEMLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	    ret = ioctl((int)fp,MEMGETBADBLOCK,&offs);
		if (ret!=0)
	    {
			printf("drv_NvmReadFlashData MEMGETBADBLOCK Device /dev/mtd%d  have bad block at offs 0x%08x,and jump it!\n",Mtdnumber,offs);
			SectorNum += 1;//����ǻ��飬��������һ��
	    }
	    else//�ÿ飬��ȥд������
	    {
			lseek(fp,offs,SEEK_SET);
	    	if(index == SectorNum)
	    	{
				do
                {
					ReadNum = write(fp,DataWrite_p,SectorPlus);
					sync();
					Trytimes++;
				} while(Trytimes<MaxTryTimes&&ReadNum!=SectorPlus);
				DataWrite_p += SectorPlus;
			}
			else
			{
				do
                {
					ReadNum = write(fp,DataWrite_p,FLASH_ONE_SECTOR_LENGTH);
					Trytimes++;
				} while(Trytimes<MaxTryTimes&&ReadNum!=FLASH_ONE_SECTOR_LENGTH);
				DataWrite_p += FLASH_ONE_SECTOR_LENGTH;
			}
	    }
	    //ret = ioctl((int)fp,MEMUNLOCK,&MTD_EraseInfoUser);
		//if (ret!=0)
	    //{
		//	printf("drv_NvmEraseFlashData MEMUNLOCK Device /dev/mtd%d  ioctl  failed !\n",Mtdnumber);
		//	close(fp);
		//	return(ST_ERROR_BAD_PARAMETER);
	    //}
	}


	if (close(fp)< 0) 
	{	
		printf("drv_NvmWriteFlashData Device  /dev/mtd%d  close  failed !\n",Mtdnumber);
		return   DRV_ERROR_DEVICE_BUSY;
	}
	if(Trytimes>=MaxTryTimes )
	{
		return DRV_ERROR_TIMEOUT;
	}

	return 	DRV_NO_ERROR;

}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : wrwei modify 
  Function Name   : DRV_ReadLoaderVer
  Description     : ��ȡLoaderVer��Ϣ.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_ReadLoaderVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver)
{
    HI_U64 u64Addr = 0;
    HI_U64 u64Len = 0;
    HI_U64 u64LoadLen = 0;
    HI_U8  *pReadBuf;
    HI_U8  *pData;
    HI_S32 ret = 0;
    HI_BOOL bResult = HI_FALSE;
    HI_HANDLE fd = INVALID_FD;
    HI_U32 wtihoob = 0;
    HI_CHAR FileTypeStr[] = "loader";
    uImage_header_t HIBinHeader;
    DRV_RomTag_t YSRomTag;
    HI_U32 ImgDataSize = 0;
    HI_U32 BinExtendLength = 0;
 
    u64Addr = 0;
	u64Len  = (HI_U64)(sizeof(uImage_header_t));
	wtihoob = HI_FLASH_RW_FLAG_RAW;

    //��loader����
#ifdef EMMC_FLASH  //eMMC or NAND flash
	fd = HI_Flash_Open(HI_FLASH_TYPE_EMMC_0, FileTypeStr, u64Addr, u64Len);
#else
	fd = HI_Flash_Open(HI_FLASH_TYPE_NAND_0, FileTypeStr, u64Addr, u64Len);
#endif
	printf("> %d Line: HI_Flash_Open(fd=%#x)!\n", __LINE__, fd); 
	if (INVALID_FD == fd)
	{
        return -1;
	}
	
    pReadBuf = (HI_U8 *)malloc(u64Len);
    if (pReadBuf == NULL)
	{
        printf("HIBinHeader malloc failure!\n");
        return -1;
    }	    
    memset(pReadBuf, '\0', u64Len);
    
    //��ȡloaderͷ��Ϣ(uImage_header_t)������OOB
    ret = HI_Flash_Read(fd, u64Addr, pReadBuf, (HI_U32)u64Len, wtihoob);  
    if (0 >= ret)
    {
        if (HI_FLASH_END_DUETO_BADBLOCK != ret)
        {
            printf("> %d Line: Flash Read failure(ret=%#x)!\n", __LINE__, ret); 
        }   	               
    }
    else
    {
        memset(&HIBinHeader,0,u64Len);
        memcpy(&HIBinHeader,pReadBuf,u64Len);
        if(HIBinHeader.ih_magic == 0x56190527)
        {
            ImgDataSize = HIBinHeader.ih_size;
            drv_htonl(&(HIBinHeader.ih_size));
            drv_htonl(&(HIBinHeader.ih_dcrc));
            drv_htonl(&ImgDataSize);
            if((ImgDataSize&0x0000000F) != 0x0)
            {
                BinExtendLength = 0x10 - (ImgDataSize&0x0000000F);
        	}
            
            memset(&YSRomTag,0,sizeof(DRV_RomTag_t));
            u64LoadLen = u64Len+HIBinHeader.ih_size+BinExtendLength+1024;
            pData=malloc(u64LoadLen);
            if(pData==NULL)
        	{
                printf("DRV_ReadLoaderVer can not malloc \n");
                bResult=HI_FALSE;
        	}
            else
        	{
                ret=HI_Flash_Read(fd,0, pData, u64LoadLen, 0);
                if(ret<=0)
                {
                    bResult = HI_FALSE;
                }
                else
                {
                    //printf("HIBinHeader.ih_size=0x%x \n",HIBinHeader.ih_size);
                    memcpy(&YSRomTag,pData+u64LoadLen-1024,sizeof(DRV_RomTag_t));
                    //printf("YSRomTag.LDFileTypeU32 =%d \n",YSRomTag.LDFileTypeU32);
                    free(pData);
                    if(strcmp(FileTypeStr,"loader")==0)
                    {
                        *pSoftver=YSRomTag.LDSWVerU32;//����汾��    
                        printf("Get loader version =0x%x  YSRomTag.LDSWVerU32=0x%x\n",*pSoftver,YSRomTag.LDSWVerU32);
                        bResult=HI_TRUE;
                	}
                    else
                    {
                        printf("string is not match \n");
                        bResult=HI_FALSE;
                    }
                }
            }
        }
    }
    free(pReadBuf);

	ret = HI_Flash_Close(fd);
    if(0 != ret)
    {
        printf("> %d Line: Flash Close failure!\n", __LINE__);
        bResult=HI_FALSE;
    }
    return bResult;
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : wrwei modify 
  Function Name   : DRV_ReadKernelVer
  Description     : ��ȡKernelVer��Ϣ.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_ReadKernelVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver)
{
    HI_U64 u64Addr = 0;
    HI_U64 u64Len = 0;
    HI_U64 u64LoadLen = 0;
    HI_U8  *pReadBuf;
    HI_U8  *pData;
    HI_S32 ret = 0;
    HI_BOOL bResult = HI_FALSE;
    HI_HANDLE fd = INVALID_FD;
    HI_U32 wtihoob = 0;
    HI_CHAR FileTypeStr[] = "kernel";
    uImage_header_t HIBinHeader;
    DRV_RomTag_t YSRomTag;
    HI_U32 ImgDataSize = 0;
    HI_U32 BinExtendLength = 0;
	
    u64Addr = 0;
	u64Len  = (HI_U64)(sizeof(uImage_header_t));
	wtihoob = HI_FLASH_RW_FLAG_RAW;
    
    //��kernel����
#ifdef EMMC_FLASH   //eMMC or NAND flash
	fd = HI_Flash_Open(HI_FLASH_TYPE_EMMC_0, FileTypeStr, u64Addr, u64Len);
#else
	fd = HI_Flash_Open(HI_FLASH_TYPE_NAND_0, FileTypeStr, u64Addr, u64Len);
#endif
	printf("> %d Line: HI_Flash_Open(fd=%#x)!\n", __LINE__, fd); 
	if (INVALID_FD == fd)
	{
        return -1;
	}
	
    pReadBuf = (HI_U8 *)malloc(u64Len);
    if (pReadBuf == NULL)
    {
        printf("HIBinHeader malloc failure!\n");
        return -1;
    }	    
    memset(pReadBuf, '\0', u64Len);	    

    //��ȡkernelͷ��Ϣ(uImage_header_t)������OOB
    ret = HI_Flash_Read(fd, u64Addr, pReadBuf, (HI_U32)u64Len, wtihoob);  
    if (0 >= ret)
    {
        if (HI_FLASH_END_DUETO_BADBLOCK != ret)
        {
            printf("> %d Line: Flash Read failure(ret=%#x)!\n", __LINE__, ret); 
        }   	               
    }
    else
    {
        memset(&HIBinHeader,0,u64Len);
        memcpy(&HIBinHeader,pReadBuf,u64Len);
        if(HIBinHeader.ih_magic == 0x56190527)
        {
            ImgDataSize = HIBinHeader.ih_size;
            drv_htonl(&(HIBinHeader.ih_size));
            drv_htonl(&(HIBinHeader.ih_dcrc));
            drv_htonl(&ImgDataSize);
            if((ImgDataSize&0x0000000F) != 0x0)
            {
                BinExtendLength = 0x10 - (ImgDataSize&0x0000000F);
            }

            memset(&YSRomTag,0,sizeof(DRV_RomTag_t));
            u64LoadLen = u64Len+HIBinHeader.ih_size+BinExtendLength+1024;
            pData=malloc(u64LoadLen);
            if(pData==NULL)
        	{
                printf("DRV_ReadLoaderVer can not malloc \n");
                bResult=HI_FALSE;
        	}
            else
            {
                ret=HI_Flash_Read(fd,0, pData, u64LoadLen, 0);
                if(ret<=0)
                {
                    bResult = HI_FALSE;
                }
                else
                {
                    //printf("HIBinHeader.ih_size=0x%x \n",HIBinHeader.ih_size);
                    memcpy(&YSRomTag,pData+u64LoadLen-1024,sizeof(DRV_RomTag_t));
                    //printf("YSRomTag.LDFileTypeU32 =%d \n",YSRomTag.LDFileTypeU32);
                    free(pData);
                    if(strcmp(FileTypeStr,"kernel")==0)
                    {
                        *pSoftver=YSRomTag.LDSWVerU32;//����汾��    
                        printf("Get kernel version =0x%x  YSRomTag.LDSWVerU32=0x%x\n",*pSoftver,YSRomTag.LDSWVerU32);
                        bResult=HI_TRUE;
                    }
                    else
                    {
                        printf("string is not match \n");
                        bResult=HI_FALSE;
                    }
                }
            }
        }
    }
    free(pReadBuf);

	ret = HI_Flash_Close(fd);
    if(0 != ret)
	{
        printf("> %d Line: Flash Close failure!\n", __LINE__);
        bResult=HI_FALSE;
	}
    return bResult;
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : wrwei modify 
  Function Name   : DRV_ReadLinuxAppVer
  Description     : ��ȡLinuxAppVer��Ϣ.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/			
DRV_ErrCode DRV_ReadLinuxAppVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver)
{
    *pSoftver = SWVER;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          : wrwei modify 
  Function Name   : DRV_ReadSoftVer
  Description     : ��ȡSoftVer��Ϣ.
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/	
U32_YS DRV_ReadSoftVer(DRV_BinSoftVer type)
{
	U32_YS version;
	DRV_ErrCode errCode = DRV_NO_ERROR;
	if(type==DRV_BIN_LOADER)
	{
		errCode = DRV_ReadLoaderVer(LOADER_START_ADDR,&version);
	}
	if(type==DRV_BIN_KERNEL)
	{
		errCode = DRV_ReadKernelVer(KERNEL_START_ADDR,&version);
	}
	if(type==DRV_BIN_FS)
	{
		errCode = DRV_ReadLinuxAppVer(APP_START_ADDR,&version);
}
	return version;
}
#if 0
#define FLASH_DEVICE_INFO_OFFSET           0xA0000  
#define FLASH_HEADER_OFFSET                  0xC0000
typedef struct Flash_Header_s
{
   DRV_U16           software_versoion;                    
   U8_YS             download_time_date[5];                         
   DRV_U16           download_service_id;   
   DRV_NvmDownloadParameters_t    last_download_parameters;           
   DRV_NvmDownloadParameters_t    default_download_parameters[4];  
   U32_YS           AS_CRC32;   
   U32_YS           software_size;
   U8_YS             reserved_future_use[155];
   U32_YS           Flash_Header_CRC32;                               
} DRV_NvmFlashHeader_t; 

DRV_ErrCode DRV_NvmReadDeviceInfo(DRV_NvmDeviceInfo_t * Device_Info_p)
{
	drv_NvmReadFlashData(0,FLASH_DEVICE_INFO_OFFSET,(U32_YS *)Device_Info_p,sizeof(DRV_NvmDeviceInfo_t));
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_NvmWriteDeviceInfo(DRV_NvmDeviceInfo_t * Device_Info_p)
{
    DRV_ErrCode errCode = DRV_NO_ERROR;
    errCode = drv_NvmWriteFlashData(0,FLASH_DEVICE_INFO_OFFSET, (U32_YS *)Device_Info_p, sizeof(DRV_NvmDeviceInfo_t));

    return errCode;
}

DRV_ErrCode DRV_NvmReadFlashHeader(DRV_NvmFlashHeader_t * Flash_Header_p)
{
	drv_NvmReadFlashData(0,FLASH_HEADER_OFFSET,(U32_YS *)Flash_Header_p,sizeof(DRV_NvmFlashHeader_t));
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_NvmWriteFlashHeader(DRV_NvmFlashHeader_t  * Flash_Header_p)
{
    DRV_ErrCode errCode = DRV_NO_ERROR;
		
    errCode = drv_NvmWriteFlashData(0,FLASH_HEADER_OFFSET, (U32_YS *)Flash_Header_p,sizeof(DRV_NvmFlashHeader_t));
    return errCode;
}
#endif

#ifdef __cplusplus
}
#endif


