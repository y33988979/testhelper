/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosdecompress.h

    Description : 

    Others:      

    History:      1. llh creat 2004-3-24


*******************************************************************************/
#ifndef _YSOSDECOPRESS_H_
#define _YSOSDECOPRESS_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#define RING_BUFFER_SIZE                4096   /* size of ring buffer */
#define UPPER_LIMIT                  18   /* upper limit for match_length */
#define THRESHOLD       2   /* encode string into position and length
                                                   if match_length is greater than this */
#if 0
#define COMPRESSINDEXLENGTH      53 


typedef struct CompressData
{
       const U8   *pCompressDataPointer; //压缩数据的指针值
	U8            *pDataPointer;//应用数据的指针值
	U32           uiCompressDataLength;//压缩数据长?

} CompressData_t;


typedef struct CompressDataIndex
{
	U8     ucStatus;//自身状?
	U8     ucDataStatus;//数据的状态
       CompressData_t *   pCompressData;//压缩数据指针
} CompressDataIndex_t;

enum STBCompressIndexStatus
{
	DATA_NO_DECOMPRESS,
	DATA_IN_DECOMPRESSING,
	DATA_DECOMPRESSED,
	INDEX_IN_USE,
	INDEX_FREE
};




BOOL  DRV_DECDecompressInit ( void ) ;


void  DRV_DECDecompressTerm ( void ) ;


void InitDecompressIndex(void);

void DecompressAllData(void);



void DRV_DECDecompressPic( STOSD_Bitmap_t  DecompressVerify);
BOOL_YS DRV_DECDecompressCompleted(void);
BOOL_YS DRV_DECFontDecompressCompleted(void);
BOOL_YS DRV_DECLogoDecompressCompleted(void);



#endif

void DecompressData( const unsigned char *pIndata, unsigned char *pOutdata, int iDataLength);
/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
