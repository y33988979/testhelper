/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdatanvm.h

    Description : Ƶ�����ݴ洢����

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

/*���ڱ�������Ƶ��������Ϣ��ϵͳ����ʹ�� 12 bytes */
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

/*Ԥ��32(0x20)���ֽ��Ա���չ(0x0020-0x0040)*/

/*���ڱ���ʵ������Ƶ��������Ϣ*/
#ifdef USE_DATA_BLOCK_64K_MODE
/*(16+4)*100=2000(0x7d0) for ts(Ԥ��4���ֽ�)(0x0040-0x0810)*/
#define NVM_CHNLINFO_MEM_STBTSINFO_BASE         (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0040)
#define NVM_CHNLINFO_MEM_STBTSINFO_END          (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0810)

/*(56+4)*700=42000(0xa410) for serivce(Ԥ��4���ֽ�)(0x0810-0xac20)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0810)
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xac20)

/*(8+4)*500=6000(0x1770) for tv channel(Ԥ��4���ֽ�)(0xac20-0xc390)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xac20)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xc390)

/*(8+4)*200=2400(0x960) for radio channel(Ԥ��4���ֽ�)(0xc390-0xccf0)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xc390)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xccf0)

/*(6)*30=180(0xb4) for jz_dbc(0xccf0-0xcda4)*/
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xccf0)
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xcda4)

/*(46)*(20)=920(0x398) for nvod(0xcda4-0xd13c)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xcda4)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xd13c)

/*(27+40*2)*(30+10)=4280(0x10b8) for bat(0xd13c-0xe1f4)(Ԥ��10��BAT)(ƽ��ÿ��BAT����40��Ƶ��)*/
#define NVM_CHNLINFO_MEM_STBBATINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xd13c)
#define NVM_CHNLINFO_MEM_STBBATINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xe1f4)

/*(542)*5=2710(0xa96) for mosaic(0xe1f4-0xec8a)*/
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_BASE		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xe1f4)
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xec8a)

/*800(0x320)for CA product id(0xec8a-0xefaa)*/
#define NVM_CA_INFO_BASE	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xec8a)
#define NVM_CA_INFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xefaa)
#else
/*(16+4)*150=3000(0xbb8) for ts(Ԥ��4���ֽ�)(0x0040-0x0bf8)*/
#define NVM_CHNLINFO_MEM_STBTSINFO_BASE         (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0040)
#define NVM_CHNLINFO_MEM_STBTSINFO_END          (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0bf8)

/*(56+4)*1000=60000(0xea60) for serivce(Ԥ��4���ֽ�)(0x0bf8-0xf658)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x0bf8)
#define NVM_CHNLINFO_MEM_STBCHANNELINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xf658)

/*(8+4)*800=9600(0x2580) for tv channel(Ԥ��4���ֽ�)(0xf658-0x11bd8)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0xf658)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x11bd8)

/*(8+4)*200=2400(0x960) for radio channel(Ԥ��4���ֽ�)(0x11bd8-0x12538)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x11bd8)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_RADIO_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12538)

/*(6)*40=240(0xf0) for jz_dbc(0x12538-0x12628)*/
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12538)
#define NVM_CHNLINFO_MEM_JZDBC_CHNLINDEX_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12628)

/*(46)*(40)=1840(0x730) for nvod(0x12628-0x12d58)*/
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_BASE   (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12628)
#define NVM_CHNLINFO_MEM_STBCHANNELINDEX_NVOD_END    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12d58)

/*(27+40*2)*(40+10)=5350(0x14e6) for bat(0x12d58-0x1423e)(Ԥ��10��BAT)(ƽ��ÿ��BAT����40��Ƶ��)*/
#define NVM_CHNLINFO_MEM_STBBATINFO_BASE    (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x12d58)
#define NVM_CHNLINFO_MEM_STBBATINFO_END     (NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1423e)

/*(542)*10=5420(0x152c) for mosaic(0x1423E-0x1576a)*/
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_BASE		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1423e)
#define NVM_CHNLINFO_MEM_STBMOSAICINFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1576a)

/*800(0x320)for CA product id(0x1576a-0x15a8a)*/
#define NVM_CA_INFO_BASE	(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x1576a)
#define NVM_CA_INFO_END		(NVM_CHNLINFO_MEM_INDEX_OFFSET + 0x15a8a)
#endif


/*���������Ŀ����Subtitle��Ϣ��Ϊ��չƵ�����ݴ������չƵ������Block��*/
#ifdef USE_DATA_BLOCK_64K_MODE
#define STBDATA_EXTENDED_APPLICATION_FLASH_SIZE         (1024*64)
#else
#define STBDATA_EXTENDED_APPLICATION_FLASH_SIZE         (1024*128)
#endif

#define NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET         	0x0000

/*���ڱ�������Ƶ����չ��Ϣ����*/
#define NVM_CHNLINFO_MEM_AUDIO_NUMBER_BASE        	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0000)
#define NVM_CHNLINFO_MEM_AUDIO_NUMBER_END         	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)

#define NVM_SERVICE_INFORMATION_NUMBER_BASE   		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0004)
#define NVM_SERVICE_INFORMATION_NUMBER_END    		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)

#define NVM_CHNLINFO_MEM_SUBTITLE_NUMBER_BASE       (NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0008)
#define NVM_CHNLINFO_MEM_SUBTITLE_NUMBER_END        (NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x000c)

/*Ԥ��32(0x20)���ֽ��Ա���չ(0x0000-0x0020)*/

/*���ڱ���ʵ��������չƵ��������Ϣ*/
#ifdef USE_DATA_BLOCK_64K_MODE
/*���ڱ����Ŀ�������Ϣ*/
/*(84)*(200)=16800(0x41a0) for Service AudioInfo(0x0020-0x41c0)*/
#define NVM_CHNLINFO_MEM_AUDIOINFO_BASE    			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0020)
#define NVM_CHNLINFO_MEM_AUDIOINFO_END     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x41c0)

/*���ڱ����Ŀ�����Ϣ*/
/*(268)*(150)=40200(0x9d08) for Service Information(0x41c0-0xd1c8)*/
#define NVM_SERVICE_INFORMATION_BASE     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x41c0)
#define NVM_SERVICE_INFORMATION_END      			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xd1c8)

/*���ڱ����Ŀ��Subtitle��Ϣ*/
/*(126)*(90)=11340(0x2c4c) for Service Subtitle(0xd1c8-0xfe14)*/
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_BASE      	(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xd1c8)
#define NVM_CHNLINFO_MEM_SUBTITLEINFO_END      		(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0xfe14)
#else
/*���ڱ����Ŀ�������Ϣ*/
/*(84)*(300)=25200(0x6270) for Service AudioInfo(0x0020-0x6290)*/
#define NVM_CHNLINFO_MEM_AUDIOINFO_BASE    			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x0020)
#define NVM_CHNLINFO_MEM_AUDIOINFO_END     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x6290)

/*���ڱ����Ŀ�����Ϣ*/
/*(268)*(250)=67000(0x105b8) for Service Information(0x6290-0x16848)*/
#define NVM_SERVICE_INFORMATION_BASE     			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x6290)
#define NVM_SERVICE_INFORMATION_END      			(NVM_EXTENDED_CHNLINFO_MEM_INDEX_OFFSET + 0x16848)

/*���ڱ����Ŀ��Subtitle��Ϣ*/
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
  Description     : ��NVM�ж�ȡTS���ݸ���
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
  Description     : ��TS���ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡTS����
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
  Description     : ��TS����д��NVM��
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
  Description     : ��NVM �ж�ȡservice ���ݸ���
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
  Description     : ��service���ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡservice����
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
  Description     : ��service����д��NVM��
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
  Description     : ��NVM�ж�ȡ����Ƶ�����ݸ���
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
  Description     : ������Ƶ�� ���ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡ����Ƶ������
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
  Description     : ������Ƶ������д��NVM��
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
  Description     : ��NVM�ж�ȡ�㲥Ƶ�����ݸ���
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
  Description     : ���㲥Ƶ�����ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡ�㲥Ƶ������
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
  Description     : ���㲥Ƶ������д��NVM��
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
  Description     : ��NVM�ж�ȡ���ݹ㲥Ƶ�����ݸ���
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
  Description     : �����ݹ㲥Ƶ�����ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡ���ݹ㲥Ƶ����Ϣ
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
  Description     : �����ݹ㲥Ƶ����Ϣд��NVM
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
  Description     : ��NVM�ж�ȡ�㲥Ƶ�����ݸ���
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
  Description     : ���㲥Ƶ�����ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡ�㲥Ƶ������
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
  Description     : ���㲥Ƶ������д��NVM��
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
  Description     : ��NVM �ж�ȡBAT INFO���ݵĸ���
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
  Description     : ��BAT INFO���ݵĸ���д��NVM��
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
  Description     : ��NVM�ж�ȡBAT INFO����  
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
  Description     : ��BAT INFO����д��NVM ��  
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
  Description     : ��NVM�ж�ȡ������Ƶ�����ݸ���
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
  Description     : ��������Ƶ������д��NVM��
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
  Description     : ��NVM�ж�ȡ������Ƶ������
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
  Description     : ��������Ƶ������д��NVM��
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
  Description     : ��NVM�ж�ȡCA��Ϣ ����
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
  Description     : ��CA��Ϣ����д��NVM  ��
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
  Description     : ��NVM �ж�ȡ������Ϣ����
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
  Description     : ��������Ϣ����д��NVM ��
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
  Description     : ��������Ϣ��NVM�ж�ȡ
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
  Description     : ��������Ϣд��NVM
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
  Description     : ��NVM �ж�ȡSubtitle��Ϣ����
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
  Description     : ��Subtitle��Ϣ����д��NVM ��
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
  Description     : ��Subtitle��Ϣ��NVM�ж�ȡ
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
  Description     : ��Subtitle��Ϣд��NVM
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
  Description     : ��NVM�ж�ȡServiceInformation���ݸ���
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
  Description     : ��ServiceInformation���ݸ���д��NVM��
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
  Description     : ��NVM�ж�ȡServiceInformation����
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
  Description     : ��ServiceInformation����д��NVM��
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
  Description     : ���㲥Ƶ���͵���Ƶ������д��NVM��
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
  Description     : ��STB���ݴ�NVM�����ж���
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
  Description     : ��STB����д��NVM����
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
  Description     : ��flash��STB���ݶ���NVM����
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
  Description     : ��NVM������STB ����д��flash ��
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
  Description     : ��STB���ݴ���չNVM�����ж���
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
  Description     : ��STB����д����չNVM����
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
  Description     : ����չflash��STB���ݶ���NVM����
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
  Description     : ��NVM ������STB ����д����չflash ��
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
  Description     : ��flash�ж���stbdata��������
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
  Description     : ��StbData��������д��Flash�� 
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
  Description     : ��STBDATA ����Ӧ��
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
  Description     : ��ȡ��������ֵ 
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
  Description     : ������������ֵ 
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
  Description     : ���˵���ʾʱ��д��E2p
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
  Description     : ���ò˵���ʾʱ��
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
  Description     : ��ȡ�˵���ʾʱ��
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
  Description     : ��ȡ����ֵ
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
  Description     : ��������ֵ
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
  Description     : ��ȡ�Աȶ�
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
  Description     : ����Աȶ�
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
  Description     : ��ȡɫ��
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
  Description     : ����Աȶ�
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
  Description     : ��E2P��ȡ���ݿ�������汾��
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
  Description     : �����ݿ�������汾��д��E2P
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
  Description     : ��E2P�ж�ȡ���ݹ㲥ͼƬ�汾��
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
  Description     : �����ݹ㲥ͼƬ�汾��д��E2P
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
  Description     : ��E2P�ж�ȡ�Զ��ػ���ʶ  
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
  Description     : ���Զ��ػ���ʶд�뵽E2P
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
  Description     : ��E2P�ж�ȡ�Զ��ػ�ʱ�� 
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
  Description     : ���Զ��ػ�ʱ��д�뵽E2P
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
  Description     : ��E2P�ж�ȡ�Զ�������ʶ  
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
  Description     : ���Զ�������ʶд��E2P
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
  Description     : ��E2P�ж�ȡ�Զ�����ʱ��  
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
  Description     : ���Զ�����ʱ��д��E2P
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
  Description     : ��E2P�ж�ȡ�Զ����ػ�ģʽ
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
  Description     : ���Զ����ػ�ģʽд��E2P
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
  Description     : ��E2P�ж�ȡ����ʱ����ʾʱ��
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 0,����ʾ; 1����ʾ
*****************************************************************************/ 
unsigned char STBDATA_NVMReadShowTimeModeWhenStandby(void);

/*****************************************************************************
  Date&Time    	  : 2013-01-16
  Author          :   wwqing
  Function Name   : STBDATA_NVMWriteShowTimeModeWhenStandby
  Description     : ������ʱ����ʾʱ��д��E2P
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :  0,����ʾ; 1����ʾ
*****************************************************************************/ 
void STBDATA_NVMWriteShowTimeModeWhenStandby(unsigned char Flag);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : STBDATA_NVMReadSicodeFlag
  Description     : ��E2P�ж�ȡSicode��ʾ
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
  Description     : ��Sicode��ʾд��E2P
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
  Description     : ��ȡƵ������ֵ
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
  Description     : ��ȡƵ������ֵ
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
  Description     : ����Ƶ������ֵ
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
  Description     : ����Ƶ������ֵ
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
  Description     : ��ȡDTVͳһ����ֵ
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
  Description     : ��ȡDTVͳһ����ֵ
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
  Description     : ����DTVͳһ����ֵ
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
  Description     : ����DTVͳһ����ֵ
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
  Description     : ��ȡNVODͳһ����ֵ
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
  Description     : ��ȡNVODͳһ����ֵ
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
  Description     : ����NVODͳһ����ֵ
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
  Description     : ����NVODͳһ����ֵ
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
  Description     : ��ȡ���ģʽ
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
  Description     : �������ģʽ  
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
  Description     : ��E2P�ж�ȡ����ģʽ��ʶ
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
  Description     : ������ģʽ��ʶд��E2P
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
  Description     : ��E2P�ж�ȡ�������κ�
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
  Description     : ���������κ�д��E2P
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
  Description     : ����E2p����
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
  Description     : ��ȡPVR�����豸��
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
  Description     : ����PVR�����豸�� 
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
  Description     : ��ȡPVR���������
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
  Description     : ����PVR���������
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
  Description     : ��ȡʱ���ļ���С�޸ı�־
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
  Description     : ����ʱ���ļ���С�޸ı�־
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
  Description     : ��ȡʱ���ļ���С 
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
  Description     : ����ʱ���ļ���С
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
  Description     : ��ȡ����������Ƶ�㡢���Ʒ�ʽ��������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ������ѶCAʹ��
*****************************************************************************/ 
void STBDATA_ReadAreaLockFre(unsigned int *fre, unsigned int *Syml, unsigned short*QAM);

/*****************************************************************************
  Date&Time       : 2012-12-15   
  Author          :   lzhe
  Function Name   : STBDATA_WriteAreaLockFre
  Description     : ��ȡ����������Ƶ�㡢���Ʒ�ʽ��������
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ������ѶCAʹ��
*****************************************************************************/ 
void STBDATA_WriteAreaLockFre(unsigned int fre, unsigned int Syml, unsigned short QAM);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCAScUpInfoAddr
  Description     : д�����ܿ�������Ϣ
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
  Description     : ��ȡ���ܿ�������Ϣ
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
  Description     : ��ȡ������Ѷnit�������������pdsd����
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
  Description     : ����������Ѷnit�������������pdsd����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void STBDATA_NVMWriteSmsxNITLockPDSD(unsigned int uiPdsd);

#endif
