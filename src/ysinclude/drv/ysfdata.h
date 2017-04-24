/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : 	ysnvm.h
    author   : 		yzb creat 2004-3-17
    Description : 
    Others:       
 
*******************************************************************************/

#ifndef _YSFDATA_H_
#define _YSFDATA_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "ys_typedef.h"
#include <stddefs.h>

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 enum
{
    DRV_NVM_ERROR_HANDLE = DRV_NVM_BASE + 1,
    DRV_NVM_ERROR_E2P_READ,
    DRV_NVM_ERROR_E2P_WRITE,
    DRV_NVM_ERROR_BOLCK_MODE,
    DRV_NVM_ERROR_FLASH_NO_FACT_CFG,
    DRV_NVM_ERROR_FLASH_NO_APP_CFG,
    DRV_NVM_ERROR_FLASH_NOT_SUPPORT_DEVICE,
    DRV_NVM_ERROR_FLASH_CHECK_ERASED,
    DRV_NVM_ERROR_FLASH_WRITE_DATA,
    DRV_NVM_ERROR_FLASH_WRITE_PODATA,
    DRV_NVM_ERROR_FLASH_POLL_TIMEOUT,
    DRV_NVM_ERROR_FLASH_POLL_ERROR,
    DRV_NVM_ERROR_FLASH_MEMORY_ALLOCATE,
    DRV_NVM_ERROR_FLASH_FILE_OPEN,
    DRV_NVM_ERROR_FLASH_FILE_READ,
    DRV_NVM_ERROR_FLASH_FILE_WRITE,
    DRV_NVM_ERROR_FLASH_FILE_SEEK
};

/*----------------------------------------------------------------------------
 *	Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

extern U32_YS FLASH_BIOS_START_ADD; 
extern U32_YS FLASH_APP_START_ADD;
extern U32_YS FLASH_BOOK_START_ADD;
extern U32_YS FLASH_CA_START_ADD; 
extern U32_YS FLASH_LOGO_START_ADD;
extern U32_YS FLASH_CH_START_ADD; 
extern U32_YS FLASH_AVIT_START_ADD;
extern U32_YS FLASH_DBD_START_ADD; 
extern U32_YS FLASH_DATA_START_ADD;
extern U32_YS FLASH_WHOLE_CHIP_SIZE;
#define LOADER_TAG_SIZE   (1*1024)

/* Bin file infors */
/* ----------- */
typedef struct DRV_RomTag_s
{
    unsigned char  DesStringReserved[16];
    unsigned char  DesStringLDBuilder[16];   
    unsigned char  LDBuilderStr[48]; 
    unsigned char  DesStringLDBuildDate[16]; 
    unsigned char  LDBuildDateStr[16];
    unsigned char  DesStringLDBuildTime[16]; 
    unsigned char  LDBuildTimeStr[16];
    unsigned char  DesStringLDSWVerStr[16];  
    unsigned char  LDSWVerStr[16];
    unsigned char  DesStringLDSWVer[12];     
    unsigned int   LDSWVerU32;
    unsigned char  DesStringLDHWVer[12];     
    unsigned int   LDHWVerU32;
    unsigned char  DesStringLDCaType[12];    
    unsigned int   LDCaTypeU32;
    unsigned char  DesStringLDFileType[12];    
    unsigned int   LDFileTypeU32;
	unsigned char  DesStringLDCompressTag[16]; 
	 unsigned int   LDCompressTag; /*Compressed:0x51DB51DB*/
    unsigned char  LDReserved[LOADER_TAG_SIZE-276];  
    unsigned char  DesStringCheckSum[12]; 
    unsigned int   CheckSumU32; 
} DRV_RomTag_t;//总长度为1K (1024) Byte


typedef struct image_header {
       U32 image_magic_code;// 0x59425A4A  JZBY
	U32	load_address;
	U32	image_size_address;
	U32	image_compressed_address;/*其地址表示实际数据存放的Flash地址，这里仅对boot有效*/
	U32	image_compressed_size_address;	//非压缩是0x00000000，如果压缩的，则为压缩后的数据大小
	U32	start_address;
	U32	stack_address;		//0x8f9ffffc
	U32	dbg_connected_address;	//0x8c53f3ec
} image_header_t;


#define UIH_NMLEN		32
typedef struct uImage_header {
	HI_U32	ih_magic;	/* Image Header Magic Number	*/
	HI_U32	ih_hcrc;	/* Image Header CRC Checksum	*/
	HI_U32	ih_time;	/* Image Creation Timestamp	*/
	HI_U32	ih_size;	/* Image Data Size		*/
	HI_U32	ih_load;	/* Data	 Load  Address		*/
	HI_U32	ih_ep;		/* Entry Point Address		*/
	HI_U32	ih_dcrc;	/* Image Data CRC Checksum	*/
	HI_U8		ih_os;		/* Operating System		*/
	HI_U8		ih_arch;	/* CPU architecture		*/
	HI_U8		ih_type;	/* Image Type			*/
	HI_U8		ih_comp;	/* Compression Type		*/
	HI_U8		ih_name[UIH_NMLEN];	/* Image Name		*/
} uImage_header_t;




#define IH_NMLEN		32	/* Image Name Length		*/
typedef struct linux_image_header {
	U32	ih_magic;	/* Image Header Magic Number	*/
	U32	ih_hcrc;	/* Image Header CRC Checksum	*/
	U32	ih_time;	/* Image Creation Timestamp	*/
	U32	ih_size;	/* Image Data Size		*/
	U32	ih_load;	/* Data	 Load  Address		*/
	U32	ih_ep;		/* Entry Point Address		*/
	U32	ih_dcrc;	/* Image Data CRC Checksum	*/
	U8		ih_os;		/* Operating System		*/
	U8		ih_arch;	/* CPU architecture		*/
	U8		ih_type;	/* Image Type			*/
	U8		ih_comp;	/* Compression Type		*/
	U8		ih_name[IH_NMLEN];	/* Image Name		*/
} linux_image_header_t;

typedef enum DRV_BinSoftVer_e
{
	DRV_BIN_LOADER = 1,/*loader*/
	DRV_BIN_KERNEL,/*kernel*/
	DRV_BIN_FS /*linux app*/
}DRV_BinSoftVer;

DRV_ErrCode AVIT_FlashRead(U32_YS offset,  U32_YS Length, U8_YS* pData);
DRV_ErrCode AVIT_FlashWrite(U32_YS offset,  U32_YS Length, U8_YS* pData);
DRV_ErrCode BOOK_FlashRead(U8_YS* pData, U32_YS Length);
DRV_ErrCode BOOK_FlashWrite(U8_YS * pData, U32_YS Length);
DRV_ErrCode HTRD_FlashRead(U32_YS offset,  U32_YS Length, U8_YS* pData);
DRV_ErrCode HTRD_FlashWrite(U32_YS offset,  U32_YS Length, U8_YS* pData);
DRV_ErrCode DRV_FLASH_WriteChannelData(U8_YS *pData,U32_YS Length);
DRV_ErrCode DRV_FLASH_ReadChannelData(U8_YS * pData,U32_YS Length);

DRV_ErrCode DRV_FLASH_ReadCaData(U8_YS *pData,  U32_YS Length);
DRV_ErrCode DRV_FLASH_WriteCaData(U8_YS *pData,  U32_YS Length);
DRV_ErrCode DRV_FLASH_ReadCaData1(U8_YS *pData,  U32_YS Length);
DRV_ErrCode DRV_FLASH_WriteCaData1(U8_YS *pData,  U32_YS Length);
/* name字符串长度<64*/
DRV_ErrCode DRV_FLASH_ReadCaDataByName(const char *name, U8_YS *pData,  U32_YS Length);
DRV_ErrCode DRV_FLASH_WriteCaDataByName(const char *name, U8_YS *pData,  U32_YS Length);

/*下列接受说明
*对于Read的接口，最后返回的实际是Flash的地址偏移和长度，
*通过得到这两个参数以后，外界准备好buffer,然后通过
*DRV_FLASH_Read接口来将输出读取出来
*/
// 这里的Picture实际上是至blog
DRV_ErrCode DRV_FLASH_ReadPictureLength(U32_YS *pLength);
DRV_ErrCode DRV_FLASH_WritePicture(U8_YS *pData,U32_YS Length);
DRV_ErrCode DRV_FLASH_ReadPicture(U8_YS *pData, U32_YS Length);

DRV_ErrCode DRV_FLASH_ReadPicinBlockLength(U8_YS Blocknum,U32_YS *pLength);
DRV_ErrCode DRV_FLASH_WritePicinBlock( U8_YS Blocknum,U8_YS *pData,U32_YS Length);
DRV_ErrCode DRV_FLASH_ReadPicinBlock(U8_YS Blocknum, U8_YS *pData, U32_YS Length);

DRV_ErrCode DRV_FLASH_ReadPicinBlockLength(U8_YS Blocknum,U32_YS *pLength);
DRV_ErrCode DRV_FLASH_WriteDatainBlock( U8_YS Blocknum, U8_YS *pData, U32_YS Length);
DRV_ErrCode DRV_FLASH_ReadDatainBlock(U8_YS Blocknum, U8_YS *pData, U32_YS Length);

/*****************************************************************************
  Date&Time       : 2004-09-29
  Author          : wrwei 
  Function Name   : DRV_FLASH_ErasePicture
  Description     : 擦除开机画面接口
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
DRV_ErrCode DRV_FLASH_ErasePicture(void);

DRV_ErrCode DRV_FLASH_WriteApp(U8_YS* pData, U32_YS Length);


DRV_ErrCode DRV_ReadLoaderVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver);
DRV_ErrCode DRV_ReadKernelVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver);
DRV_ErrCode DRV_ReadLinuxAppVer(U32_YS ImageFlashStartAddr,U32_YS *pSoftver);
/*返回值为软件版本号*/
U32_YS DRV_ReadSoftVer(DRV_BinSoftVer type);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif




