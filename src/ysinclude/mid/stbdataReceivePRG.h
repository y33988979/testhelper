/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceivePRG.h

    Description : 九州频道数据更新功能

    Others:      

    History:     1. yliu creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdata_updataPRG_
#define _YSstbdata_updataPRG_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define	APPLICATION_RECEIVE_TABLE_ID 				0X80
#define  APPLICATION_RECEIVE_TABLE_PID     			0X1ba3
#define STBDATA_MAX_PRODUCT_NUMBER              	100
#define STBDATA_INFO_MAX_PRODUCT_NAME_LENGTH		60
#define STBDATA_INFO_MAX_PRODUCT_DETIALINFO_LENGTH  200
#define STBDATA_INFO_MAX_PRODUCT_EXTENDINFO_LENGTH 	100
#define STBDATA_INFO_MAX_SERVICEINPACKAGE_COUNT     50
#define PRODUCT_NAME_DESC                          	0x80
#define PRODUCT_SYMBOL_DESC                      	0x81
#define PRODUCT_DETAIL_DESC                       	0x82
#define PRODUCT_SERVICE_DESC                     	0x83
#define PRODUCT_INFO_TIME_OUT                   	2000

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct ProductDataSection
{
	unsigned char 	aucSectionData[1024];	
	unsigned short  usDataLength;
	BOOL 		    bInUsed;				
}ProductDataSection_t;

typedef struct STBDATAPRODUCTInfoTemp
{
	unsigned short 	usProductID;
	unsigned char  	ucProductPrice;
	unsigned int  	ucProductLabel;
	unsigned short  usExchangeRate;
}STBDATAPRODUCTInfoTemp_t;

typedef struct STBDATASERVICEINPACKAGE
{
    unsigned short 	usServiceID;
    unsigned short 	usNetID;
    unsigned short 	usTSID;
}STBDATASERVICEINPACKAGE_t;

/*Product 信息结构 */
typedef struct STBDATAProductInfo
{
	unsigned char	scProductName[STBDATA_INFO_MAX_PRODUCT_NAME_LENGTH];
	unsigned short 	usProductID;
	unsigned int   	ucProductPrice;
	unsigned char  	ucPackageNUM[5]; 
	unsigned char  	ucDetailInfo[STBDATA_INFO_MAX_PRODUCT_DETIALINFO_LENGTH];
	unsigned char  	ucExtendInfo[STBDATA_INFO_MAX_PRODUCT_EXTENDINFO_LENGTH];
	unsigned char  	ucManagedNum;
	unsigned char  	ucServiceCount;
	STBDATASERVICEINPACKAGE_t   Service[STBDATA_INFO_MAX_SERVICEINPACKAGE_COUNT];
}STBDATAProductInfo_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Start_ReceiveProductData
  Description     : 开始接收频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Start_ReceiveProductData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : Finish_ReceiveProductData
  Description     : 结束更新频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Finish_ReceiveProductData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : GetAppData_ReceiveStatus
  Description     : 获取更新频道数据状态
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetAppData_ReceiveStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : UpdateCaextendData
  Description     : 更新频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void UpdateCaextendData(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_GetProductInfo
  Description     : 根据NetID、TsID和ServiceID获取频道信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBDATAProductInfo_t *STBDATA_GetProductInfo(unsigned short usNetID,unsigned short usTSID,unsigned short usServiceID);

#endif

