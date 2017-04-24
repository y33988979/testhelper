/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbtabledata.h

    Description : DVB数据上报管理

    Others:      

    History:   1. llh creat 2004-3-29
			   2. xyan Modify 2011-11-14

*******************************************************************************/
#ifndef _YSDVBTABLEDATA_
#define _YSDVBTABLEDATA_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define DVB_MAX_FILTER_NUMBER      			32

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum DvbSectionStatus
{
	DVB_SECTION_FREE,
	DVB_SECTION_IN_USE
}DvbSectionStatus_e;

typedef struct DvbFilterStatus
{
	signed char			scFilterNo;
	signed char			scReceivedSectionNo;
	unsigned int        uiSectionReceiveLowMask;
	unsigned int        uiSectionReceiveMidMask;
} DvbFilterStatus_t;

typedef struct DvbSectionIndex
{
	unsigned char		ucStatus;
	unsigned char		ucSectionNo;
}DvbSectionIndex_t;

typedef struct DvbSigleTableIndex
{
	unsigned char 		MaxSectionNumber;
	unsigned char 		SectionReceivedNumber;
	list_t				SectionList;
} DvbSigleTableIndex_t;

typedef struct DvbBATSubTableIndex
{
	unsigned short		BouquetID;
	unsigned char		MaxSectionNumber;
	unsigned char       SectionReceivedNumber;
	list_t				SectionList;
}DvbBATSubTableIndex_t;

typedef struct DvbSDTSubTableIndex
{
	unsigned short		NetworkID;
	unsigned short 		TsID;
	unsigned char		MaxSectionNumber;
	unsigned char       SectionReceivedNumber;
	list_t				SectionList;
}DvbSDTSubTableIndex_t;

typedef struct DvbMulTableIndex
{
	unsigned char		SubTableReceivedNumber;	
    list_t              SubTableList;
} DvbMulTableIndex_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_StandardTableReqCallBack
  Description     : 一个section一个section接收模式
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_StandardTableReqCallBack(int FilterNo,BOOLEAN isTimeOut,unsigned char *pData,unsigned short uiSectLength);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_StandardTableReqCallBackByMultySection
  Description     : 数据接收和解析分开的模式
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 主要针对数据Section比较多
*****************************************************************************/ 
void DVB_StandardTableReqCallBackByMultySection(int FilterNo,BOOLEAN isTimeOut,unsigned char *pData,unsigned short uiSectLength);      

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_BATMulTableReqCallBack
  Description     : 收取BAT多子表Section数据回调函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :一个Filter接收所有BAT的回调函数(一直接收数据直到Filter超时)
*****************************************************************************/ 
void DVB_BATMulTableReqCallBack(int FilterNo,BOOLEAN isTimeOut,unsigned char *pData,unsigned short uiSectLength);

/*****************************************************************************
  Date&Time       : 2011-11-14
  Author          :   xyan
  Function Name   : DVB_SDTMulTableReqCallBack
  Description     : 收取SDT_Other多子表Section数据回调函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :一个Filter接收所有SDT_Other的回调函数(一直接收数据直到Filter超时)
*****************************************************************************/ 
void DVB_SDTMulTableReqCallBack(int FilterNo,BOOLEAN isTimeOut,unsigned char *pData,unsigned short uiSectLength);

#endif


