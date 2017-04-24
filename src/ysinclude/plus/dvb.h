/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvb.h

    Description : 

    Others:      

    History:      1. llh creat 2004-3-30


*******************************************************************************/
#ifndef _DVB_H_
#define _DVB_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stberror.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
//#define DVB_SUMA_MOSAIC

#define DVB_DELIVERY_TYPE_CABLE			0
#define DVB_DELIVERY_TYPE_SATELLITE		1
#define DVB_DELIVERY_TYPE_TERRESTRIAL		2
#define DVB_DELIVERY_TYPE_INVALID			3


#define DVB_INFO_MAX_SERVICE_NAME_LENGTH		20
#define DVB_INFO_MAX_PROVIDER_NAME_LENGTH		20

#define DVB_INFO_MAX_NETWORK_NAME_LENGTH		20
#define DVB_INFO_MAX_CAPROVIDER_NAME_LENGTH			20
#define DVB_INFO_MAX_EVENT_NAME_LENGTH			42
#define DVB_INFO_MAX_ES_DESCRIPTION_LENGTH			20

#define DVB_INFO_MAX_EVENT_DESCRIPTION_LENGTH	256 

#define DVB_INFO_MAX_BOUQUET_NAME_LENGTH    20
#define DVB_INFO_MAX_DATA_BROADCAST_TEXT_LENGTH     20



#define MAX_PID_VALUE             0x1FFF                /* Max SCID value */




#define DVB_INFO_INVALID_TABLE_VERSION_NO 0XFF
#define DVB_INFO_INVALID_ES_TAG               0xff
#define DVB_INFO_INVALID_SERVICE_TYPE        0X00

#define DVB_INFO_INVALID_EIT_TABLE_SEGMENT_NO     -1
#define DVB_INFO_INVALID_EIT_TABLE_SEGMENT_LAST_SECTION_NO     -1
#define DVB_INFO_EIT_SECTION_NOT_RECEIVED    -1
#define DVB_INFO_EIT_SECTION_RECEIVED     1

#define DVB_INFO_INVALID_EIT_TABLE_SUBTABLE_ID     -1
#define DVB_INFO_INVALID_EIT_TABLE_LAST_SECTION_NO     -1
#define DVB_INFO_INVALID_EIT_TABLE_LAST_TABLE_ID     -1
#define DVB_INFO_INVALID_EIT_TABLE_RECEIVE_START_TIME    -1


#define DVB_INFO_INVALID_DATABROADCAST_CAROUSEL_TYPE_ID  0X00
#define DVB_INFO_INVALID_DATABROADCAST_TRANSACTION_ID      0X00000000
#define DVB_INFO_INVALID_DATABROADCAST_DSI_TIME_OUT          0Xffffffff
#define DVB_INFO_INVALID_DATABROADCAST_DII_TIME_OUT           0Xffffffff
#define DVB_INFO_INVALID_DATABROADCAST_LEAK_RATE                0X00



/* all the fixed PID definitions */
#define	PAT_PID				0x0000	/* program association */
#define	CAT_PID				0x0001	/* conditional access */
#define	NIT_PID				0x0010	/* network information */
#define	SDT_PID				0x0011	/* service description */
#define BAT_PID             0x0011
#define	EIT_PID				0x0012	/* event information */
#define	RST_PID				0x0013	/* running status */
#define	TDT_PID				0x0014
#define	TOT_PID				0x0014
#define	DIT_PID				0x001e
#define	SIT_PID				0x001f
#define	DEMUX_INVALID_PID                0X1FFF

/* all the TABLE_ID definitions */
#define PAT_TABLE_ID						0x00
#define CAT_TABLE_ID						0x01
#define PMT_TABLE_ID						0x02
#define NIT_ACTUAL_NETWORK_TABLE_ID			0x40
#define NIT_OTHER_NETWORK_TABLE_ID			0x41
#define SDT_ACTUAL_TS_TABLE_ID				0x42
#define SDT_OTHER_TS_TABLE_ID				0x46
#define BAT_TABLE_ID						0x4A
#define EIT_ACTUAL_TS_PRESENT_FOLLOWING_TABLE_ID	0x4E
#define EIT_OTHER_TS_PRESENT_FOLLOWING_TABLE_ID		0x4F
#define EIT_ACTUAL_TS_SCHEDULE_HEAD_TABLE_ID		0x50
#define EIT_ACTUAL_TS_SCHEDULE_END_TABLE_ID			0x5f
#define EIT_OTHER_TS_SCHEDULE_HEAD_TABLE_ID			0x60
#define EIT_OTHER_TS_SCHEDULE_END_TABLE_ID			0x6f
#define TDT_TABLE_ID						0x70
#define RUNNING_STATUS_TABLE_ID				0x71
#define STUFFING_TABLE_ID					0x72

/*------------------------------------------------------------
 * descriptor tag constants by MPEG-II defined
 *------------------------------------------------------------*/
#define	VIDEO_STREAM_DESC				0x02
#define	AUDIO_STREAM_DESC				0x03
#define	HIERARCHY_DESC					0x04
#define	REGISTRATION_DESC				0x05
#define	DATA_STREAM_ALIGNMENT_DESC		0x06
#define	TARGET_BKGND_GRID_DESC			0x07
#define	VIDEO_WINDOW_DESC				0x08
#define	CA_DESC							0x09
#define	ISO_639_LANGUAGE_DESC			0x0A
#define	SYSTEM_CLOCK_DESC				0x0B
#define	MULTIPLEX_BUFFER_UTILIZATION_DESC	0x0C		/* multiplex buffer utilization desc */
#define	COPYRIGHT_DESC					0x0D
#define	MAXIMUM_BITRATE_DESC			0x0E
#define	PRIVATE_DATA_INDICATOR_DESC		0x0F
#define	CAROUSEL_IDENTIFIER_DESC		0x13
#define	SMOOTHING_BUFFER_DESC			0x16

/*------------------------------------------------------------
 * descriptor tag constants by DVB defined
 *------------------------------------------------------------*/
#define NETWORK_NAME_DESC				0x40
#define SERVICE_LIST_DESC				0x41
#define SATELLITE_DELIVERY_SYSTEM_DESC	0x43
#define CABLE_DELIVERY_SYSTEM_DESC		0x44	
#define BOUQUET_NAME_DESC				0x47	
#define SERVICE_DESC					0x48
#define COUNTRY_AVAILABILITY_DESC		0x49	
#ifndef DVB_SUMA_MOSAIC
#define LINKAGE_DESC					0x4A	
#else
#define LINKAGE_DESC					0x86	
#endif
#define NVOD_REFERENCE_DESC				0x4B   	
#define TIME_SHIFTED_SERVICE_DESC		0x4C	
#define SHORT_EVENT_DESC				0x4D
#define EXTENDED_EVENT_DESC				0x4E	
#define TIME_SHIFTED_EVENT_DESC			0x4F	
#define COMPONENT_DESC					0x50	
#define MOSAIC_DESC						0x51 	

#ifndef DVB_SUMA_MOSAIC
#define STREAM_IDENTIFIER_DESC			0x52	
#else
#define STREAM_IDENTIFIER_DESC			0x85
#endif

#define CA_IDENTIFIER_DESC				0x53	
#define CONTENT_DESC					0x54
#define PARENTAL_RATING_DESC			0x55
#define TELE_TEXT_DESC					0x56
#define LOCAL_TIME_OFFSET_DESC			0x58
#define SUBTITLING_DESC					0x59
#define TERRESTRIAL_DELIVERY_SYSTER_DESC	0x5A
#define MULTILINGUAL_NETWORK_NAME_DESC	0x5B 
#define MULTILINGUAL_BOUQUET_NAME_DESC	0x5C 
#define MULTILINGUAL_SERVICE_NAME_DESC	0x5D 
#define MULTILINGUAL_COMPONENT_NAME_DESC	0x5E 
#define PRIVATE_DATA_SPECIFIER_DESC 	0x5F
#define SERVICE_MOVE_DESC 				0x60
#define SHORT_SMOOTHING_BUFFER_DESC 	0x61
#define FREQUENCY_LIST_DESC 			0x62
#define PARTIAL_TRANSPORT_STREAM_DESC 	0x63
#define DATA_BROADCAST_DESC				0x64
#define CA_SYSTEM_DESC 					0x65
#define DATA_BROADCAST_ID_DESC 			0x66
#define TRANSPORT_STREAM_DESC 			0x67
#define AC3_DESC                        0x6A
#define ANNOUNCEMENT_SUPPORT_DESC		0x6E
#define DD_PLUS_DESC                    0x7A

#define JZ_DATABROADCAST_SERVICE 0X90


/*------------------------------------------------------------
 * descriptor tag constants by user defined
 *------------------------------------------------------------*/


/* invalid ts,service and network ID definition*/
#define INVALID_NET_ID						0xffff
#define INVALID_TS_ID						0xffff
#define INVALID_SERVICE_ID					0xffff
#define INVALID_EVENT_ID                    0xffff
#define INVALID_CAS_ID                      0xffff
#define INVALID_BOUQUET_ID                  0xffff

/*---------------------------------------------------------------
 *Stream type definition
 *---------------------------------------------------------------*/

#define	MPEG1_VIDEO_STREAM				0x01
#define	MPEG2_VIDEO_STREAM				0x02
#define	MPEG1_AUDIO_STREAM				0x03
#define	MPEG2_AUDIO_STREAM				0x04
#define	PRIVATE_DATA_STREAM				0x06
#define	MPEG2_PCR_STREAM				0x82
#define AC3_AUDIO_STREAM                0x86  /* Add AC3 type, Qiujordan 2006-03-27 */
#define DDP_AUDIO_STREAM                0x87  /* Add DDP type,  2013-05-30 */
#define H264_VIDEO_STREAM               0x1b
#define AC3_AUDIO2_STREAM               0x81
#define AC3_AUDIO3_STREAM               0x06

#define AVS_VIDEO_STREAM                0x42  /*Add AVS type, Wrwei 2012-06-12*/
#define	MPEG4_VIDEO_STREAM				0x10  /*Add MPEG4 type, wwqing 2013-03-06*/

#define MPEG_AAC                        0x0f
#define MPEG_HE_AAC                     0x11

#define	INVALID_LINK					-1


#define DIGITAL_TELEVISION_SERVICE  	0X01
#define DIGITAL_RADIO_SOUND_SERVICE  	0X02
#define TELETEXT_SERVICE  				0X03
#define NVOD_REFERENCE_SERVICE  		0X04
#define NVOD_TIME_SHIFTED_SERVICE  		0X05
#define MOSAIC_SERVICE  				0X06
#define PAL_CODED_SIGNAL  				0X07
#define SECAM_CODED_SIGNAL  			0X08
#define D_D2_MAC  						0X09
#define FM_RADIO  						0X0A
#define NTSC_CODED_SIGNAL  				0X0B
#define DATA_BROADCAST_SERVICE 			0X0C


#define INVALID_DATA_BROADCAST_ID       	0xffff
#define DATA_PIPE_DATA_BROADCAST   			0X0001
#define ASYNCHRONOUS_DATA_DATA_BROADCAST	0X0002
#define SYNCHRONOUS_DATA_DATA_BROADCAST   	0X0003
#define SYNCHRONISED_DATA_DATA_BROADCAST   	0X0004
#define MULTI_PROTOCOL_ENCAPSULATION_DATA_BROADCAST   0X0005
#define DATA_CAROUSEL_DATA_BROADCAST   		0X0006
#define OBJECT_CAROUSEL_DATA_BROADCAST   	0X0007
#define DVB_ATM_STREAMS_DATA_BROADCAST   	0X0008


#define	PARSE_SUCCESS				PARSER_BASE+1
#define	PARSE_COMPLETE				PARSER_BASE+2
#define PARSE_FAILED				PARSER_BASE+4


/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*定义多语言的索引*/
typedef enum LanguageCode
{
	CHINESE_LANGUAGE=0,		//中文chi/zho/zh
	ENGLISH_LANGUAGE,     	//英语eng/en
	FRENCH_LANGUAGE,      	//法语fre/fra/fr
	GERMAN_LANGUAGE,      	//德语ger/deu/de
	JAPANESE_LANGUAGE,    	//日本语jpn/ja
	RUSSIAN_LANGUAGE,     	//俄罗斯语rus/ru
	ITALIAN_LANGUAGE,     	//意大利语ita/it
	SPANISH_LANGUAGE,   	//西班牙语spa/es
	KOREAN_LANGUAGE,      	//韩国语kor/ko
	THAI_LANGUAGE,       	//泰国语tha/th
	UNCODED_LANGUAGE,     	//没有编码过的语言mis
	/*预留扩展*/
	
    UNDEFINED_LANGUAGE=0xfe,//未定义语言
	INVALID_LANGUAGE=0xff
}LanguageCode_e;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
#endif



