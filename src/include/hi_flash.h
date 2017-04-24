/******************************************************************************
  Copyright (C), 2004-2050, Hisilicon Tech. Co., Ltd.
******************************************************************************
  File Name     : hi_flash.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       :
  Last Modified :
  Description   : header file for hiflash component
  Function List :
  History       :
  1.Date        :
  Author        :
  Modification  : Created file
******************************************************************************/

/**
 * \file
 * \brief describle the information about hiflash component. CNcomment:�ṩhiflash�����ؽӿڡ����ݽṹ��Ϣ��
 */

#ifndef __HI_FLASH__H__
#define __HI_FLASH__H__
#include "hi_type.h"

#define MAX_PARTS  32           /**< Flash max partition number*/
#define MAX_HANDLE 32           /**< Flash max handle number */
#define FLASH_NAME_LEN 32       /**< Flash Name max length */

/** flash type*/
typedef enum hiHI_FLASH_TYPE_E
{
	HI_FLASH_TYPE_SPI_0,
	HI_FLASH_TYPE_NAND_0,
	HI_FLASH_TYPE_EMMC_0,

	HI_FLASH_TYPE_BUTT
} HI_FLASH_TYPE_E;

#define  INVALID_FD -1
#define  SPAN_PART_HANDLE 1000

#define  HI_FLASH_RW_FLAG_RAW           0x0   /**< read and write without OOB,for example: kernel/uboot/ubi/cramfs.. */
#define  HI_FLASH_RW_FLAG_WITH_OOB      0x1   /**< read and write with OOB, example: yaffs2 filesystem image */
#define  HI_FLASH_RW_FLAG_ERASE_FIRST   0x2   /**< erase before write */

/* this macro for return value when nand flash have bad block or valid length less partition length */
/*CNcomment:��nand�л���ʱ,��/д/����ʱ,��Ч���ȿ���С�ڷ�����С/�򿪳���,��ʱΪ�˲�Խ��,��/д/��������Ч���Ⱥ�,���ظ�ֵ */
#define  HI_FLASH_END_DUETO_BADBLOCK    -10   

typedef struct hiFlash_PartInfo_S
{
    HI_U64  StartAddr;                  /*flash partiton start address*/
    HI_U64  PartSize; 
    HI_U32  BlockSize; 
    HI_FLASH_TYPE_E FlashType;
    HI_CHAR DevName[FLASH_NAME_LEN];
    HI_CHAR PartName[FLASH_NAME_LEN];
} HI_Flash_PartInfo_S;

typedef struct tagFLASH_OPT_S
{
    int (*raw_read)(int fd, unsigned long long *startaddr, unsigned char *buffer, 
        unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int read_oob, int skip_badblock);
    int (*raw_write)(int fd, unsigned long long *startaddr, unsigned char *buffer, 
        unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int write_oob);
    int (*raw_erase)(int fd, unsigned long long startaddr, 
        unsigned long long length, unsigned long long openaddr, unsigned long long limit_leng);
} FLASH_OPT_S;
    
/** Flash Infomation */
typedef struct hiFlash_InterInfo_S
{
    HI_U64  TotalSize;                  /* flash total size */
    HI_U64  PartSize;                   /* flash partition size*/
    HI_U32  BlockSize;                  /*flash block size */
    HI_U32  PageSize;                   /*flash page size*/
    HI_U32  OobSize;                    /*flash OOB size*/
    HI_S32  fd;                         /* file handle *//*CNcomment:< �ļ����(����ַ�򿪲��ܵõ���ʵ���) */
    HI_U64  OpenAddr;                   /* flash open address*/
    HI_U64  OpenLeng;                   /*flash open length*/
    HI_FLASH_TYPE_E FlashType;          /*flash type*/
    FLASH_OPT_S *pFlashopt;
    HI_Flash_PartInfo_S *pPartInfo;
} HI_Flash_InterInfo_S;


#if 0
#define HI_ERR_FLASH(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_USR, fmt)
            
#define HI_INFO_FLASH(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_USR, fmt) 
#else
#define HI_ERR_FLASH(fmt...)
#define HI_INFO_FLASH(fmt...)
#endif
          

/**
\brief: open flash partiton 
\attention \n
\param[in] enFlashType      Flash type
\param[in] pPartitionName   
\param[in] u64Address       open address  CNcomment:�򿪵�ַ(����������Чʱʹ��)
\param[in] u64Len           open length  CNcomment: �򿪳���(����������Чʱʹ��)
\retval    fd               Flash handle
\retval    INVALID_FD       invaild fd
\see \n

*/
HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len);

/**
\brief:  open flash patition by name
\attention \n
\param[in] pPartitionName   patition name
\retval    fd               Flash hande
\retval    INVALID_FD       invaild fd
\see \n
*/
HI_HANDLE HI_Flash_OpenByName(HI_CHAR *pPartitionName);

/**
\brief: open flash patition by  type and name
\attention \n
\param[in] enFlashType      flash type
\param[in] pPartitionName   patition name
\retval    fd                Flash hande
\retval    INVALID_FD       invaild fd
\see \n
*/
HI_HANDLE HI_Flash_OpenByTypeAndName(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName);

/**
\brief: open flash patition by  address
\attention \n
\param[in] enFlashType       flash type
\param[in] u64Address       open address
\param[in] u64Len           open length
\retval    fd               Flash hande
\retval    INVALID_FD        invaild fd
\see \n
*/
HI_HANDLE HI_Flash_OpenByTypeAndAddr(HI_FLASH_TYPE_E enFlashType, HI_U64 u64Address, HI_U64 u64Len);


/**
\brief: close flash partition
\attention \n
\param[in] hFlash    flash handle  
\retval ::HI_SUCCESS 
\retval ::HI_FAILURE 
\see \n
*/
HI_S32 HI_Flash_Close(HI_HANDLE hFlash);

/**
\brief: erase Flash partiton
\attention \n
\param[in] hFlash     flash handle
\param[in] u64Address  erase start address  CNcomment: ��д��ַ���ֽ�Ϊ��λ�������
\param[in] u32Len    data length  CNcomment:  ���ݳ��ȣ��ֽ�Ϊ��λ�������
\retval ::TotalErase  erase total length   
\retval ::HI_FLASH_END_DUETO_BADBLOCK     return value when nand flash have bad block or valid length less partition length CNcomment: ����Flash�����������(��������Ч�������ݺ�,���ظ�ֵ,һ�㲻��Ϊ�ò���ʧ��)
\retval ::HI_FAILURE    failure
\see \n
*/
HI_S32 HI_Flash_Erase(HI_HANDLE hFlash, HI_U64 u64Address, HI_U64 u64Len);

/**
\brief:  read data from flash 
\attention \n
\param[in] hFlash      flash handle
\param[in] u64Address   read start address
\param[in] pBuf           destination buffer pointer
\param[in] u32Len       destination data  length ,must align with page  size
\param[in] u32Flags     OOB flag  CNcomment:��ȡֵ HI_FLASH_RW_FLAG_WITH_OOB����ʾ���������Ƿ�� OOB ��
\retval ::TotalRead      read flash vaild  length 
\retval ::HI_FLASH_END_DUETO_BADBLOCK    CNcomment:��Flash�����������(������Ч�������ݺ�,���ظ�ֵ,���û��жϴ�������ɹ����)
\retval ::HI_FAILURE    failure
\see \n
*/
HI_S32 HI_Flash_Read(HI_HANDLE hFlash, HI_U64 u64Address, HI_U8 *pBuf,
                     HI_U32 u32Len, HI_U32 u32Flags);

/**
\brief: write data to flash 
\attention \n
1)  forbidden used the function when yaffs filesystem is using
2) can use HI_FLASH_RW_FLAG_ERASE_FIRST manner write flah , can write over all partition one time or write with block 
CNcomment: 1) ���ܵ��øýӿڸ��µ�ǰ��ʹ�õ�yaffs�ļ�ϵͳ
CNcomment: 2) ���øýӿ�дFlashʱ������ʹ��HI_FLASH_RW_FLAG_ERASE_FIRST������һ��д��Ҳ����һ��һ���д�����ǵ�дyaffs2ʱ��
   �����ȵ���HI_Flash_Erase��Ҫд�ķ�����ȫ����
\param[in] hFlash     flash handle
\param[in] u64Address   data start address
\param[in] pBuf       destination buffer pointer
\param[in] u32Len     destination data  length ,must align with page  size
\param[in] u32Flags   ��ȡֵ HI_FLASH_RW_FLAG_WITH_OOB����ʾ���������Ƿ�� OOB ��
\retval ::TotalWrite   write flash vaild  length 
\retval ::HI_FLASH_END_DUETO_BADBLOCK   have bad block  CNcomment: дFlash�����������(д����Ч�������ݺ�,���ظ�ֵ,���û��жϴ�������ɹ����)
\retval ::HI_FAILURE  
\see \n
*/
HI_S32 HI_Flash_Write(HI_HANDLE hFlash, HI_U64 u64Address,
                      HI_U8 *pBuf, HI_U32 u32Len, HI_U32 u32Flags);

/**
\brief: get flash partition info
\attention \n
info content: TotalSize,PartSize,BlockSize,PageSize,OobSize,fd
\param[in] hFlash    flash handle
\param[in] pInterInfo    info struct pointer
\retval ::HI_SUCCESS  
\retval ::HI_FAILURE  
\see \n
*/
HI_S32 HI_Flash_GetInfo(HI_HANDLE hFlash, HI_Flash_InterInfo_S *pFlashInfo);


#endif // __HI_FLASH__H__
