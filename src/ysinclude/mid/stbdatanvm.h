/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatanvm.h

    Description : 频道数据存储管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef	_YSSTBDATANVM_
#define	_YSSTBDATANVM_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#ifdef USE_DATA_BLOCK_64K_MODE
#define STBDATA_APPLICATION_FLASH_SIZE          		(1024*64)
#else
#define STBDATA_APPLICATION_FLASH_SIZE          		(1024*128)
#endif

#define STBDATA_TYPE_LENGTH       						0x10
#define NVM_CHNLINFO_MEM_DATA_TYPE_BASE        			0x0000
#define NVM_CHNLINFO_MEM_DATA_TYPE_END          		0x0010

#define NVM_CHNLINFO_MEM_INDEX_OFFSET         			0x0060

/*用于保存所有频道个数信息，系统设置使用 12 bytes */
#define NVM_CHNLINFO_MEM_TS_NUMBER_BASE        			(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0000)
#define NVM_CHNLINFO_MEM_TS_NUMBER_END         			(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)
#define NVM_CHNLINFO_MEM_CHNLINFO_NUMBER_BASE  			(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)
#define NVM_CHNLINFO_MEM_CHNLINFO_NUMBER_END    		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)
#define NVM_CHNLINFO_MEM_CHNLINDEX_NUMBER_BASE  		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)
#define NVM_CHNLINFO_MEM_CHNLINDEX_NUMBER_END   		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x000c)
#define NVM_CHNLINFO_MEM_RADIO_CHNLINDEX_NUMBER_BASE	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x000c)
#define NVM_CHNLINFO_MEM_RADIO_CHNLINDEX_NUMBER_END   	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0010)
#define NVM_CHNLINFO_MEM_NVOD_CHNLINDEX_NUMBER_BASE  	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0010)
#define NVM_CHNLINFO_MEM_NVOD_CHNLINDEX_NUMBER_END   	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0014)

#define NVM_CHNLINFO_MEM_BAT_CHNLINDEX_NUMBER_BASE 		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0014)
#define NVM_CHNLINFO_MEM_BAT_CHNLINDEX_NUMBER_END 		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0018)
#define NVM_CHNLINFO_MEM_MOSAIC_CHNLINDEX_NUMBER_BASE 	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0018)
#define NVM_CHNLINFO_MEM_MOSAIC_CHNLINDEX_NUMBER_END 	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x001C)

#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_NUMBER_BASE 	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x001C)
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_NUMBER_END 	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0020)

/*预留32(0x20)个字节以备扩展(0x0020-0x0040)*/

/*用于保存实际物理频道参数信息*/
#ifdef USE_DATA_BLOCK_64K_MODE
/*(16+4)*100=2000(0x7d0) for ts(预留4个字节)(0x0040-0x0810)*/
#define NVM_CHNLINFO_MEM_STBTSINFO_BASE         (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0040)
#define NVM_CHNLINFO_MEM_STBTSINFO_END          (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0810)

/*(56+4)*700=42000(0xa410) for serivce(预留4个字节)(0x0810-0xac20)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0810)
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xac20)

/*(8+4)*500=6000(0x1770) for tv channel(预留4个字节)(0xac20-0xc390)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xac20)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xc390)

/*(8+4)*200=2400(0x960) for radio channel(预留4个字节)(0xc390-0xccf0)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xc390)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xccf0)

/*(6)*30=180(0xb4) for jz_dbc(0xccf0-0xcda4)*/
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xccf0)
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xcda4)

/*(46)*(20)=920(0x398) for nvod(0xcda4-0xd13c)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xcda4)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xd13c)

/*(27+40*2)*(30+10)=4280(0x10b8) for bat(0xd13c-0xe1f4)(预留10个BAT)(平均每个BAT关联40个频道)*/
#define NVM_CHNLINFO_MEM_STBBATINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xd13c)
#define NVM_CHNLINFO_MEM_STBBATINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xe1f4)

/*(542)*5=2710(0xa96) for mosaic(0xe1f4-0xec8a)*/
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_BASE		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xe1f4)
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xec8a)

/*800(0x320)for CA product id(0xec8a-0xefaa)*/
#define NVM_CA_INFO_BASE	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xec8a)
#define NVM_CA_INFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xefaa)
#else
/*(16+4)*150=3000(0xbb8) for ts(预留4个字节)(0x0040-0x0bf8)*/
#define NVM_CHNLINFO_MEM_STBTSINFO_BASE         (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0040)
#define NVM_CHNLINFO_MEM_STBTSINFO_END          (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0bf8)

/*(56+4)*1000=60000(0xea60) for serivce(预留4个字节)(0x0bf8-0xf658)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0bf8)
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xf658)

/*(8+4)*800=9600(0x2580) for tv channel(预留4个字节)(0xf658-0x11bd8)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xf658)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x11bd8)

/*(8+4)*200=2400(0x960) for radio channel(预留4个字节)(0x11bd8-0x12538)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x11bd8)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12538)

/*(6)*40=240(0xf0) for jz_dbc(0x12538-0x12628)*/
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12538)
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12628)

/*(46)*(40)=1840(0x730) for nvod(0x12628-0x12d58)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12628)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12d58)

/*(27+40*2)*(40+10)=5350(0x14e6) for bat(0x12d58-0x1423e)(预留10个BAT)(平均每个BAT关联40个频道)*/
#define NVM_CHNLINFO_MEM_STBBATINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12d58)
#define NVM_CHNLINFO_MEM_STBBATINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1423e)

/*(542)*10=5420(0x152c) for mosaic(0x1423E-0x1576a)*/
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_BASE		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1423e)
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1576a)

/*800(0x320)for CA product id(0x1576a-0x15a8a)*/
#define NVM_CA_INFO_BASE	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1576a)
#define NVM_CA_INFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x15a8a)
#endif


/*多伴音、节目简介和Subtitle信息作为扩展频道数据存放于扩展频道数据Block中*/
#ifdef USE_DATA_BLOCK_64K_MODE
#define STBDATA_EXTENDED_APPLICATION_FLASH_SIZE         (1024*64)
#else
#define STBDATA_EXTENDED_APPLICATION_FLASH_SIZE         (1024*128)
#endif

#define NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET         	0x0000

/*用于保存所有频道扩展信息个数*/
#define NVM_CHNLINFO_MEM_AUDIO_NUMBER_BASE        	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0000)
#define NVM_CHNLINFO_MEM_AUDIO_NUMBER_END         	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)

#define NVM_SERVICE_INFORMATION_NUMBER_BASE   		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)
#define NVM_SERVICE_INFORMATION_NUMBER_END    		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)

#define NVM_CHNLINFO_MEM_SUBTITLE_NUMBER_BASE       (NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)
#define NVM_CHNLINFO_MEM_SUBTITLE_NUMBER_END        (NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x000c)

/*预留32(0x20)个字节以备扩展(0x0000-0x0020)*/

/*用于保存实际物理扩展频道参数信息*/
#ifdef USE_DATA_BLOCK_64K_MODE
/*用于保存节目多伴音信息*/
/*(84)*(200)=16800(0x41a0) for Service AudioInfo(0x0020-0x41c0)*/
#define NVM_CHNLINFO_MEM_AUDIOINFO_BASE    			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0020)
#define NVM_CHNLINFO_MEM_AUDIOINFO_END     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x41c0)

/*用于保存节目简介信息*/
/*(268)*(150)=40200(0x9d08) for Service Information(0x41c0-0xd1c8)*/
#define NVM_SERVICE_INFORMATION_BASE     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x41c0)
#define NVM_SERVICE_INFORMATION_END      			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xd1c8)

/*用于保存节目多Subtitle信息*/
/*(126)*(90)=11340(0x2c4c) for Service Subtitle(0xd1c8-0xfe14)*/
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_BASE      	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xd1c8)
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_END      		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xfe14)
#else
/*用于保存节目多伴音信息*/
/*(84)*(300)=25200(0x6270) for Service AudioInfo(0x0020-0x6290)*/
#define NVM_CHNLINFO_MEM_AUDIOINFO_BASE    			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0020)
#define NVM_CHNLINFO_MEM_AUDIOINFO_END     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x6290)

/*用于保存节目简介信息*/
/*(268)*(250)=67000(0x105b8) for Service Information(0x6290-0x16848)*/
#define NVM_SERVICE_INFORMATION_BASE     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x6290)
#define NVM_SERVICE_INFORMATION_END      			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x16848)

/*用于保存节目多Subtitle信息*/
/*(126)*(150)=18900(0x49d4) for Service Subtitle(0x16848-0x1b21c)*/
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_BASE      	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x16848)
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_END      		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x1b21c)
#endif

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadTsInfoCount
  Description     : 从NVM中读取TS数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadTsInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteTsInfoCount
  Description     : 将TS数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTsInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadTsInfo
  Description     : 从NVM中读取TS数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadTsInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteTsInfo
  Description     : 将TS数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTsInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceInfoCount
  Description     : 从NVM 中读取service 数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadServiceInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceInfoCount
  Description     : 将service数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceInfo
  Description     : 从NVM中读取service数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceInfo
  Description     : 将service数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadTVChannelInfoCount
  Description     : 从NVM中读取电视频道数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadTVChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteTVChannelInfoCount
  Description     : 将电视频道 数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTVChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadTVChannelInfo
  Description     : 从NVM中读取电视频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteTVChannelInfo
  Description     : 将电视频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTVChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadRadioChannelInfoCount
  Description     : 从NVM中读取广播频道数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadRadioChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteRadioChannelInfoCount
  Description     : 将广播频道数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteRadioChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadRadioChannelInfo
  Description     : 从NVM中读取广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteRadioChannelInfo
  Description     : 将广播频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadJZDBCChnlInfoCount
  Description     : 从NVM中读取数据广播频道数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBDATA_NVMLoadJZDBCChnlInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteJZDBCChnlInfoCount
  Description     : 将数据广播频道数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteJZDBCChnlInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadJZDBCChnlInfo
  Description     : 从NVM中读取数据广播频道信息
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadJZDBCChnlInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteJZDBCChnlInfo
  Description     : 将数据广播频道信息写入NVM
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteJZDBCChnlInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadNVODChannelInfoCount
  Description     : 从NVM中读取广播频道数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadNVODChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteNVODChannelInfoCount
  Description     : 将广播频道数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteNVODChannelInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadNVODChannelInfo
  Description     : 从NVM中读取广播频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteRadioChannelInfo
  Description     : 将广播频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteNVODChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadBATInfoCount
  Description     : 从NVM 中读取BAT INFO数据的个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBDATA_NVMLoadBATInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteBATInfoCount
  Description     : 将BAT INFO数据的个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteBATInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadBATInfo
  Description     : 从NVM中读取BAT INFO数据  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteBATInfo
  Description     : 将BAT INFO数据写入NVM 中  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteBATInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadMosaicInfoCount
  Description     : 从NVM中读取马赛克频道数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadMosaicInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteMosaicInfoCount
  Description     : 将马赛克频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteMosaicInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadMosaicInfo
  Description     : 从NVM中读取马赛克频道数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadMosaicInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteMosaicInfo
  Description     : 将马赛克频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteMosaicInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadCAInfo
  Description     : 从NVM中读取CA信息 数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadCAInfo(unsigned int *pCAInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteCAInfo
  Description     : 将CA信息数据写入NVM  中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteCAInfo(unsigned int *pCAInfo);

 /*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceAudioInfoCount
  Description     : 从NVM 中读取伴音信息个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadServiceAudioInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceAudioInfoCount
  Description     : 将伴音信息个数写入NVM 中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceAudioInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceAudioInfo
  Description     : 将伴音信息从NVM中读取
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceAudioInfo
  Description     : 将伴音信息写入NVM
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceAudioInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceSubtitleInfoCount
  Description     : 从NVM 中读取Subtitle信息个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadServiceSubtitleInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceSubtitleInfoCount
  Description     : 将Subtitle信息个数写入NVM 中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceSubtitleInfoCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceSubtitleInfo
  Description     : 将Subtitle信息从NVM中读取
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceSubtitleInfo
  Description     : 将Subtitle信息写入NVM
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceSubtitleInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    xyan
  Function Name   : STBDATA_NVMLoadServiceInformationCount
  Description     : 从NVM中读取ServiceInformation数据个数
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int	STBDATA_NVMLoadServiceInformationCount(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceInformationCount
  Description     : 将ServiceInformation数据个数写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceInformationCount(int iCount);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadServiceInformation
  Description     : 从NVM中读取ServiceInformation数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteServiceInformation
  Description     : 将ServiceInformation数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteServiceInformation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteTVOrRadioChannelInfo
  Description     : 将广播频道和电视频道数据写入NVM中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTVOrRadioChannelInfo(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_ReadApplicationFlashBuffer
  Description     : 将STB数据从NVM缓冲中读出
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_ReadApplicationFlashBuffer(int iOffset, int nLength, unsigned char *data );

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WriteApplicationFlashBuffer
  Description     : 将STB数据写入NVM缓冲
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_WriteApplicationFlashBuffer( int iOffset, int nLength, unsigned char *data );

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_LoadApplicationFlashData
  Description     : 将flash中STB数据读入NVM缓冲
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_LoadApplicationFlashData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WriteApplicationFlashData
  Description     : 将NVM缓冲中STB 数据写入flash 中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_WriteApplicationFlashData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_ReadExtendedApplicationFlashBuffer
  Description     : 将STB数据从扩展NVM缓冲中读出
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_ReadExtendedApplicationFlashBuffer( int iOffset, int nLength, unsigned char *data);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WriteExtendedApplicationFlashBuffer
  Description     : 将STB数据写入扩展NVM缓冲
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_WriteExtendedApplicationFlashBuffer( int iOffset, int nLength, unsigned char *data);

 /*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_LoadExtendedApplicationFlashData
  Description     : 将扩展flash中STB数据读入NVM缓冲
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_LoadExtendedApplicationFlashData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_WriteExtendedApplicationFlashData
  Description     : 将NVM 缓冲中STB 数据写入扩展flash 中
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL STBDATA_WriteExtendedApplicationFlashData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadDataType
  Description     : 从flash中读出stbdata数据类型
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMLoadDataType(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   llhui
  Function Name   : STBDATA_NVMWriteDataType
  Description     : 将StbData数据类型写入Flash中 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteDataType(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_LoadApplicationData
  Description     : 将STBDATA 读入应用
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_LoadApplicationData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadUpgradeZone
  Description     : 读取升级区域值 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadUpgradeZone(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteUpgradeZone
  Description     : 保存升级区域值 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteUpgradeZone(unsigned char ucUpgradeZone);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteShowTime
  Description     : 将菜单显示时间写入E2p
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteShowTime(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMSetShowTime
  Description     : 设置菜单显示时间
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMSetShowTime(unsigned char ucShowTime);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMLoadShowTime
  Description     : 获取菜单显示时间
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMLoadShowTime(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadBrightness
  Description     : 读取亮度值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadBrightness(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteBrightness
  Description     : 保存亮度值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteBrightness(unsigned char ucBrightness);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadContrast
  Description     : 读取对比度
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadContrast(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteContrast
  Description     : 保存对比度
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteContrast(unsigned char ucContrast);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadSaturation
  Description     : 读取色度
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadSaturation(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteSaturation
  Description     : 保存对比度
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteSaturation(unsigned char ucSaturation);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadADPVersionNO
  Description     : 从E2P读取九州开机画面版本号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadADPVersionNO(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteADPVersionNO
  Description     : 将九州开机画面版本号写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteADPVersionNO(unsigned char VerNumber);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadBGPVersionNO
  Description     : 从E2P中读取九州广播图片版本号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadBGPVersionNO(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteBGPVersionNO
  Description     : 将九州广播图片版本号写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteBGPVersionNO(unsigned char VerNumber);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadPowerOffFlag
  Description     : 从E2P中读取自动关机标识  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadPowerOffFlag(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietPowerOffFlag
  Description     : 将自动关机标识写入到E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietPowerOffFlag(unsigned char Flag);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadPowerOffTime
  Description     : 从E2P中读取自动关机时间 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBDATA_NVMReadPowerOffTime(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietPowerOffTime
  Description     : 将自动关机时间写入到E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietPowerOffTime(int Time);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadPowerOnFlag
  Description     : 从E2P中读取自动开机标识  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadPowerOnFlag(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietPowerOnFlag
  Description     : 将自动开机标识写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietPowerOnFlag(unsigned char Flag);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadPowerOnTime
  Description     : 从E2P中读取自动开机时间  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int STBDATA_NVMReadPowerOnTime(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietPowerOnTime
  Description     : 将自动开机时间写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietPowerOnTime(int Time);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadPowerMode
  Description     : 从E2P中读取自动开关机模式
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadPowerMode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietPowerMode
  Description     : 将自动开关机模式写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietPowerMode(unsigned char Flag);

/*****************************************************************************
  Date&Time    	  : 2013-01-16
  Author          :   wwqing	
  Function Name   : STBDATA_NVMReadShowTimeModeWhenStandby
  Description     : 从E2P中读取待机时否显示时间
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 0,不显示; 1，显示
*****************************************************************************/ 
unsigned char STBDATA_NVMReadShowTimeModeWhenStandby(void);

/*****************************************************************************
  Date&Time    	  : 2013-01-16
  Author          :   wwqing
  Function Name   : STBDATA_NVMWriteShowTimeModeWhenStandby
  Description     : 将待机时否显示时间写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :  0,不显示; 1，显示
*****************************************************************************/ 
void STBDATA_NVMWriteShowTimeModeWhenStandby(unsigned char Flag);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadSicodeFlag
  Description     : 从E2P中读取Sicode标示
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadSicodeFlag(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWrietSicodeFlag
  Description     : 将Sicode标示写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWrietSicodeFlag(unsigned char ucFlag);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadChannelVolume
  Description     : 读取频道音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadChannelVolume(unsigned short usServiceNo);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadChannelTrack
  Description     : 读取频道声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadChannelTrack(unsigned short usServiceNo);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteChannelVolume
  Description     : 保存频道音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteChannelVolume(unsigned short usServiceNo, unsigned char ucVolume);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteChannelTrack
  Description     : 保存频道声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteChannelTrack(unsigned short usServiceNo, unsigned char ucTrack);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadDTVVolume
  Description     : 读取DTV统一音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadDTVVolume(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadDTVTrack
  Description     : 读取DTV统一声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadDTVTrack(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteDTVVolume
  Description     : 保存DTV统一音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteDTVVolume(unsigned char ucVolume);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteDTVTrack
  Description     : 保存DTV统一声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteDTVTrack(unsigned char ucTrack);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadNvodVolume
  Description     : 读取NVOD统一音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadNvodVolume(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadNvodTrack
  Description     : 读取NVOD统一声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadNvodTrack(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteNvodVolume
  Description     : 保存NVOD统一音量值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteNvodVolume(unsigned char ucVolume);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteNvodTrack
  Description     : 保存NVOD统一声道值
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteNvodTrack(unsigned char ucTrack);

/*****************************************************************************
  Date&Time       : 2011-11-08   
  Author          :   zhubo
  Function Name   : STBDATA_NVMReadVoutType
  Description     : 读取输出模式
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadVoutType(void);

/*****************************************************************************
  Date&Time       : 2011-11-08   
  Author          :   zhubo
  Function Name   : STBDATA_NVMWriteVoutType
  Description     : 保存输出模式  
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteVoutType(unsigned char ucVoutType);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadFactoryModeFlag
  Description     : 从E2P中读取生产模式标识
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int STBDATA_NVMReadFactoryModeFlag(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteFactoryModeFlag
  Description     : 将生产模式标识写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteFactoryModeFlag(unsigned int Flag);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadBatchNo
  Description     : 从E2P中读取生产批次号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned short STBDATA_NVMReadBatchNo(void);

/*****************************************************************************
  Date&Time       : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMWriteBatchNo
  Description     : 将生产批次号写入E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteBatchNo(unsigned short BatchNo);

/*****************************************************************************
  Date&Time       : 2010-11-22   
  Author          : xyan   
  Function Name   : STBDATA_ClearE2pData
  Description     : 擦除E2p数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_ClearE2pData(unsigned int uiDataLength);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMReadPvrPathDiskNo
  Description     : 读取PVR保存设备号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadPvrPathDiskNo(void);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMWritePvrPathDiskNo
  Description     : 保存PVR保存设备号 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWritePvrPathDiskNo(unsigned char ucDiskNo);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMReadPvrPathPart
  Description     : 读取PVR保存分区号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadPvrPathPartNo(void);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMWritePvrPathPartNo
  Description     : 保存PVR保存分区号
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWritePvrPathPartNo(unsigned char ucPartNo);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMReadTimeShiftFlag
  Description     : 读取时移文件大小修改标志
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char STBDATA_NVMReadTimeShiftFlag(void);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMWriteTimeShiftFlag
  Description     : 保存时移文件大小修改标志
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTimeShiftFlag(unsigned char ucFlag);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMReadTimeShiftFileSize
  Description     : 读取时移文件大小 
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int STBDATA_NVMReadTimeShiftFileSize(void);

/*****************************************************************************
  Date&Time       : 2012-02-15   
  Author          :   wpeng
  Function Name   : STBDATA_NVMWriteTimeShiftFileSize
  Description     : 保存时移文件大小
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteTimeShiftFileSize(unsigned int uiFileSize);

/*****************************************************************************
  Date&Time       : 2012-12-15   
  Author          :   lzhe
  Function Name   : STBDATA_ReadAreaLockFre
  Description     : 读取区域锁定的频点、调制方式、符号率
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 数码视讯CA使用
*****************************************************************************/ 
void STBDATA_ReadAreaLockFre(unsigned int *fre, unsigned int *Syml, unsigned short*QAM);

/*****************************************************************************
  Date&Time       : 2012-12-15   
  Author          :   lzhe
  Function Name   : STBDATA_WriteAreaLockFre
  Description     : 读取区域锁定的频点、调制方式、符号率
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 数码视讯CA使用
*****************************************************************************/ 
void STBDATA_WriteAreaLockFre(unsigned int fre, unsigned int Syml, unsigned short QAM);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCAScUpInfoAddr
  Description     : 写入智能卡升级信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void STBDATA_WriteCAScUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCAScUpInfoAddr
  Description     : 读取智能卡升级信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void STBDATA_ReadCAScUpInfoAddr(unsigned char* pScUpInfo);

/*****************************************************************************
  Date&Time       : 2013-01-10 
  Author          :   wwqing
  Function Name   : STBDATA_NVMReadSmsxNITLockPDSD
  Description     : 读取数码视讯nit区域锁定所需的pdsd数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int STBDATA_NVMReadSmsxNITLockPDSD(void);

/*****************************************************************************
  Date&Time       : 2013-01-10   
  Author          :   wwqing
  Function Name   : STBDATA_NVMWriteSmsxNITLockPDSD
  Description     : 保存数码视讯nit区域锁定所需的pdsd数据
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteSmsxNITLockPDSD(unsigned int uiPdsd);

#endif
