/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : 	ysnvm.h
    author   : 		yzb creat 2004-3-17
    Description : 
    Others:       
 
*******************************************************************************/

#ifndef _YSNVM_H_
#define _YSNVM_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "drv_basic.h"

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/**********************************
------------------
������Ӧ��ȥ����ʹ�û����汾����Ҫ�Ĳ���
--------------------  
| RADIO_CHANNEL      |1000  bytes
--------------------
| TV_CHANNEL           |1000  bytes
--------------------
| CA������Ϣ    |1000 bytes
--------------------NVM_CA_DATA_BASE
| Ӧ����Ϣ        |700 bytes
--------------------NVM_APP_INFO_BASE
| LWIP INFO        |50 bytes
--------------------NVM_LWIP_INFO_BASE
| Tuner��Ϣ           |50   bytes
--------------------NVM_TUNER_INFO_BASE
| ��������        |100  bytes �����ݽṹ��ȫ��Ӧ
--------------------NVM_FACT_CFG_BASE
| ��������        |100   bytes
--------------------NVM_UPGRADE_BASE(NVM_BASE)
***********************************/
#define NVM_BASE                                  (0x0000)
#define NVM_UPGRADE_BASE                  (NVM_BASE)
#define NVM_FACT_CFG_BASE                 (NVM_BASE+100)
#define NVM_TUNER_INFO_BASE             (NVM_BASE+200)
#define NVM_LWIP_INFO_BASE               (NVM_BASE+250)
#define NVM_APP_INFO_BASE                 (NVM_BASE+300)
#define NVM_CA_DATA_BASE                   (NVM_BASE+1000)


/*ǿ�������Ĳ���*/
#define NVM_APPLICATION_MARK_ADDR NVM_UPGRADE_BASE
#define NVM_APPLICATION_MARK_SIZE   2
#define NVM_UPGRADE_MARK_ADDR       (NVM_UPGRADE_BASE+2)/*���ֶ����Զ�*/
#define NVM_UPGRADE_MARK_SIZE         2
#define NVM_UPGRADE_TS_ADDR		   (NVM_UPGRADE_BASE+4)/*����Ƶ�������*/
#define NVM_UPGRADE_TS_SIZE	         10/*����Ƶ�㣬�����ʣ����Ʒ�ʽ��Tuner����*/
#define NVM_UPGRADE_UMT_ADDR	   (NVM_UPGRADE_BASE+14)/*UMT������*/
#define NVM_UPGRADE_UMT_SIZE	         4
#define NVM_UPGRADE_TYPE_ADDR        (NVM_UPGRADE_BASE+18) /*������������ͨ��IREDE*/
#define NVM_UPGRADE_TYPE_SIZE	         2
#define NVM_UPGRADE_MODULE_ADDR   (NVM_UPGRADE_BASE+20) /*����ģ��*/
#define NVM_UPGRADE_MODULE_SIZE    4
#define NVM_UPGRADE_CA_TYPE_ADDR  (NVM_UPGRADE_BASE+24)/*CA ���͵�����*/
#define NVM_UPGRADE_CA_TYPE_SIZE	  2
#define NVM_DTVIA_TAG_ADDR				(NVM_UPGRADE_BASE + 26)/*������LOADER��־*/
#define NVM_DTVIA_TAG_SIZE				1
#define NVM_APPLICATION_TYPE_ADDR				(NVM_UPGRADE_BASE + 27)/*Ӧ������*/
#define NVM_APPLICATION_TYPE_SIZE				1

/*  ��һ�ο�����ʼ��ʹ�ò�Ҫ����ռ��*/
#define NVM_STARTER_INIT_ADDR        (NVM_BASE + 28)
#define NVM_STARTER_INIT_SIZE         2
#define NVM_UPGRADE_PANEL_TYPE_ADDR        (NVM_BASE + 30)/*ǰ��崥��ʱ���������ͣ���usb��cable*/
#define NVM_UPGRADE_PANEL_TYPE_SIZE         2
#define NVM_BACKDOOR_TAG_ADDR        (NVM_BASE + 32)/*�ṩ��Ӧ�õĺ��ų�����ڣ�д�������־*/
#define NVM_BACKDOOR_TAG_SIZE         2
#define NVM_SOFT_VERSION_FOR_IPANEL_ADDR         (NVM_BASE + 36)
#define NVM_SOFT_VERSION_FOR_IPANEL_SIZE         2

#define NVM_UPGRADE_ZONE_ADDR			(NVM_UPGRADE_BASE + 40)/*��������ֵ*/
#define NVM_UPGRADE_ZONE_SIZE					1



/*�������õ�Ӳ���͹�������*/
#define NVM_FACT_CFG_ADDR			NVM_FACT_CFG_BASE
#define NVM_FACT_CFG_SIZE			100
#define NVM_VOUT_MODE		(NVM_FACT_CFG_ADDR+10)/*��Ƶ���ģʽ*/
#define VOUT_MODE_SIZE	1

#define NVM_IPANEL30_STBNUM_EXTEND_ADDR    (NVM_FACT_CFG_BASE+60) /*�������ͺ�������������ܺ�*/
#define NVM_IPANEL30_STBNUM_EXTEND_SIZE    4

/*Ӳ���汾�ţ���boot��ǿ��*/
#define NVM_HD_VER_ADDR        (NVM_BASE + 104)
#define NVM_HD_VER_SIZE       4
/*���кźţ���boot��ǿ��*/
#define NVM_STB_SER_ADDR        (NVM_BASE + 112)
#define NVM_STB_SER_SIZE       4


/*TUNER��ز���*/
#define NVM_TUNER_INFO_ADDR		NVM_TUNER_INFO_BASE
#define NVM_TUNER_INFO_SIZE		50


/*LWIP��ز���*/
#define NVM_LWIP_INFO_ADDR		NVM_LWIP_INFO_BASE
#define NVM_LWIP_INFO_SIZE		50

#define NVM_MAC_ADDR    (NVM_LWIP_INFO_ADDR)
#define NVM_MAC_SIZE    6
#define NVM_LWIP_IP_AUTO_ADDR    (NVM_LWIP_INFO_ADDR + 6)/* IP �Ƿ��Զ���ȡ*/
#define NVM_LWIP_IP_AUTO_SIZE    1
#define NVM_LWIP_DNS_AUTO_ADDR    (NVM_LWIP_INFO_ADDR + 7)/* DNS �Ƿ��Զ���ȡ*/
#define NVM_LWIP_DNS_AUTO_SIZE    1
#define NVM_LWIP_IPADDR_ADDR    (NVM_LWIP_INFO_ADDR + 8)
#define NVM_LWIP_IPADDR_SIZE    4
#define NVM_LWIP_MASK_ADDR    (NVM_LWIP_INFO_ADDR + 12)
#define NVM_LWIP_MASK_SIZE    4
#define NVM_LWIP_GW_ADDR    (NVM_LWIP_INFO_ADDR + 16)
#define NVM_LWIP_GW_SIZE    4
#define NVM_LWIP_DNS_ADDR    (NVM_LWIP_INFO_ADDR + 20)
#define NVM_LWIP_DNS_SIZE    4
#define NVM_VOD_SERVER_ADDR    (NVM_LWIP_INFO_ADDR + 24)
#define NVM_VOD_SERVER_SIZE    4
#define NVM_HTTP_SERVER_ADDR    ( NVM_LWIP_INFO_ADDR + 28)
#define NVM_HTTP_SERVER_SIZE    4
#define NVM_IPUPDATE_SERVER_ADDR (NVM_LWIP_INFO_ADDR   + 32)
#define NVM_IPUPDATE_SERVER_SIZE 4


/*Ӧ���������*/
#define NVM_APP_INFO_ADDR			NVM_APP_INFO_BASE
#define NVM_APP_INFO_SIZE			700
#define NVM_APP_CFG_ADDR				(NVM_APP_INFO_ADDR)
#define NVM_APP_CFG_SIZE				50

/*���ڱ���DTV��Ϣ: ��ǰƵ���ź����� 3 bytes*/
#define NVM_DTV_VOLUME_ADDR         (NVM_APP_INFO_ADDR+50)
#define NVM_DTV_VOLUME_SIZE         1

#define DTV_NVM_CHANNEL_ADDR        (NVM_APP_INFO_ADDR+51)
#define DTV_NVM_CHANNEL_SIZE        2

#define NVM_CHANNEL_LOCK_PASSWORD_ADDR        (NVM_APP_INFO_ADDR+53)
#define NVM_CHANNEL_LOCK_PASSWORD_SIZE        4

#define NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_ADDR      (NVM_APP_INFO_ADDR+57)
#define NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_SIZE        1

#define NVM_DEFAULT_SETTING_TAG_ADDR        (NVM_APP_INFO_ADDR+58)
#define NVM_DEFAULT_SETTING_TAG_SIZE        2

#define NVM_CHNL_ORDER_CHANGED_TAG_ADDR       (NVM_APP_INFO_ADDR+60)
#define NVM_CHNL_ORDER_CHANGED_TAG_SIZE        1

#define NVM_REGION_NETWORK_ID_ADDR        (NVM_APP_INFO_ADDR+61)
#define NVM_REGION_NETWORK_ID_SIZE        2

#define NVM_PDT_VERSION_NO_ADDR        (NVM_APP_INFO_ADDR+63)
#define NVM_PDT_VERSION_NO_SIZE        1

#define NVM_STOCK_CHANNEL_NO_ADDR        (NVM_APP_INFO_ADDR+64)
#define NVM_STOCK_CHANNEL_NO_SIZE        2

#define NVM_LANGUAGE_SETTING_ADDR        (NVM_APP_INFO_ADDR+66)
#define NVM_LANGUAGE_SETTING_SIZE        1

#define NVM_CA_REGION_CODE_ADDR        (NVM_APP_INFO_ADDR+67)
#define NVM_CA_REGION_CODE_SIZE        4

#define NVM_DBC_TS_ADDR		(NVM_APP_INFO_ADDR+71)
#define NVM_DBC_TS_SIZE	8

#define NVM_CITY_EYES_CHANNEL_NO_ADDR        (NVM_APP_INFO_ADDR+79)
#define NVM_CITY_EYES_CHANNEL_NO_SIZE        2

#define NVM_TIME_ZONE_ADDR        (NVM_APP_INFO_ADDR+81)
#define NVM_TIME_ZONE_SIZE        1

#define NVM_TIME_ZONE_TAG_ADDR        (NVM_APP_INFO_ADDR+82)
#define NVM_TIME_ZONE_TAG_SIZE        1

#define NVM_RADIO_BACKGROUND_VERSION_NO_ADDR        (NVM_APP_INFO_ADDR+83)
#define NVM_RADIO_BACKGROUND_VERSION_NO_SIZE        1

#define NVM_SHOW_TIME_ADDR        (NVM_APP_INFO_ADDR+84)
#define NVM_SHOW_TIME_SIZE        1

#define NVM_SHOW_TIME_TAG_ADDR        (NVM_APP_INFO_ADDR+85)
#define NVM_SHOW_TIME_TAG_SIZE        1

#define NVM_VIDEO_MODE_ADDR        (NVM_APP_INFO_ADDR+86)
#define NVM_VIDEO_MODE_SIZE        1

#define NVM_VIDEO_MODE_TAG_ADDR        (NVM_APP_INFO_ADDR+87)
#define NVM_VIDEO_MODE_TAG_SIZE        1

#define NVM_USE_PASSWORD_TAG_ADDR        (NVM_APP_INFO_ADDR+88)
#define NVM_USE_PASSWORD_TAG_SIZE        1

#define NVM_START_FRE_ADDR         (NVM_APP_INFO_ADDR+89)
#define NVM_START_FRE_SIZE        2

#define NVM_END_FRE_ADDR        (NVM_APP_INFO_ADDR+91)
#define NVM_END_FRE_SIZE        2

#define NVM_SYSSET_PASSWORD_ADDR        (NVM_APP_INFO_ADDR+93)
#define NVM_SYSSET_PASSWORD_SIZE        4

#define NVM_SYSSET_PASSWORD_MODIFIED_TAG_ADDR        (NVM_APP_INFO_ADDR+97)
#define NVM_SYSSET_PASSWORD_MODIFIED_TAG_SIZE        1

#define NVM_SYSSET_MENU_SHOW_TIME_TAG_ADDR         (NVM_APP_INFO_ADDR+98)
#define NVM_SYSSET_MENU_SHOW_TIME_TAG_SIZE  1

#define NVM_NVOD_SERVICE_NO_ADDR        (NVM_APP_INFO_ADDR+99)
#define NVM_NVOD_SERVICE_NO_SIZE  2

#define NVM_NVOD_EVENT_ID_ADDR         (NVM_APP_INFO_ADDR+101)
#define NVM_NVOD_EVENT_ID_SIZE  2

#define NVM_CA_PIN_PASSWORD_ADDR        (NVM_APP_INFO_ADDR+103)
#define NVM_CA_PIN_PASSWORD_SIZE  4

#define NVM_INVERSION_TAG_ADDR        (NVM_APP_INFO_ADDR+107)
#define NVM_INVERSION_TAG_SIZE  1

#define NVM_PASSWORD_1_ADDR       (NVM_APP_INFO_ADDR+108)
#define NVM_PASSWORD_1_SIZE        2

#define NVM_CC_TIMING_ADDR         (NVM_APP_INFO_ADDR+110)
#define NVM_CC_TIMING_SIZE        36

#define NVM_DTVIA_DATA_VERSION_ADDR       (NVM_APP_INFO_ADDR+146)
#define NVM_DTVIA_DATA_VERSION_SIZE       16

/*���ڱ�������ģʽ��ʶ*/
#define NVM_FACTORY_MODE_FLAG_ADDR		(NVM_APP_INFO_ADDR + 162)
#define NVM_FACTORY_MODE_FLAG_SIZE			4

/*�����������κŵĵ�ַ*/
#define NVM_BATCH_NO_ADDR				(NVM_APP_INFO_ADDR + 166)
#define NVM_BATCH_NO_SIZE					2

/*��Ƶ���ģʽ(�ǳ�����������)*/
#define NVM_VOUTTYPE_ADDR        		(NVM_APP_INFO_ADDR+168)
#define NVM_VOUTTYPE_SIZE       			1

/*Nvod��Ŀ����������*/
#define NVM_NVOD_VOLUME_ADDR         	(NVM_APP_INFO_ADDR+169)
#define NVM_NVOD_VOLUME_SIZE         		1

#define NVM_BRIGHTNESS_ADDR        (NVM_APP_INFO_ADDR+170)
#define NVM_BRIGHTNESS_SIZE        1

#define NVM_CONTRAST_ADDR         (NVM_APP_INFO_ADDR+171)
#define NVM_CONTRAST_SIZE        1

#define NVM_SATURATION_ADDR        (NVM_APP_INFO_ADDR+172)
#define NVM_SATURATION_SIZE        1

/*���ڱ��涨ʱ���ػ�ʱ��ͱ�ʾ*/
#define NMV_POWEROFF_FLAG_ADDR   (NVM_APP_INFO_ADDR+173)
#define NMV_POWEROFF_FLAG_SIZE  1

#define NVM_POWEROFF_TIME_ADDR    (NVM_APP_INFO_ADDR+174)
#define NVM_POWEROFF_TIME_SIZE    4

#define NMV_POWERON_FLAG_ADDR   (NVM_APP_INFO_ADDR+178)
#define NMV_POWERON_FLAG_SIZE  1

#define NVM_POWERON_TIME_ADDR    (NVM_APP_INFO_ADDR+179)
#define NVM_POWERON_TIME_SIZE    4

#define NVM_POWER_MODE_ADDR    (NVM_APP_INFO_ADDR+183)
#define NVM_POWER_MODE_SIZE    1

#define NMV_MENUGIF_VERSION_ADDR  (NVM_APP_INFO_ADDR+184)
#define NMV_MENUGIF_VERSION_SIZE 1

#define NMV_BGPIC_VERSION_ADDR   (NVM_APP_INFO_ADDR+185)
#define NMV_BGPIC_VERSION_SIZE  1

#define NVM_FORCE_CHSRCH_FOR_NIT_UPDATE_ADDR    (NVM_APP_INFO_ADDR+186)
#define NVM_FORCE_CHSRCH_FOR_NIT_UPDATE_SIZE    1

#define NVM_NITVER_ADDR      (NVM_APP_INFO_ADDR+187)
#define NVM_NITVER_SIZE        1

#define NVM_MID_DBG_MASK_ADDR  (NVM_APP_INFO_ADDR+188)
#define NVM_MID_DBG_MASK_SIZE 1

#define NVM_MID_DBG_STARTREQUEST_ADDR  (NVM_APP_INFO_ADDR+189)
#define NVM_MID_DBG_STARTREQUEST_SIZE 4

#define NVM_SICODE_FLAG_ADDR (NVM_APP_INFO_ADDR+193)
#define NVM_SICODE_FLAG_SIZE  1

#define NVM_PVRPATH_DISK_ADDR (NVM_APP_INFO_ADDR+194)
#define NVM_PVRPATH_DISK_SIZE  1

#define NVM_PVRPATH_PART_ADDR (NVM_APP_INFO_ADDR+195)
#define NVM_PVRPATH_PART_SIZE  1

#define NVM_TIMESHIFT_FLAG_ADDR (NVM_APP_INFO_ADDR+196)
#define NVM_TIMESHIFT_FLAG_SIZE  1

#define NVM_TIMESHIFT_SIZE_ADDR (NVM_APP_INFO_ADDR+197)
#define NVM_TIMESHIFT_SIZE_SIZE  4

#define NVM_SHOWTIME_SIZE_ADDR (NVM_APP_INFO_ADDR+201)
#define NVM_SHOWTIME_SIZE_SIZE  1

#define NVM_JZ_PRIVATE_DATA_ADDR (NVM_APP_INFO_ADDR+202)
#define NVM_JZ_PRIVATE_DATA_SIZE  32

/*CA����ʹ�õ�E2p��ַ-------------START---*/
#define NVM_CA_PAIR_NO_ADDR        (NVM_CA_DATA_BASE)
#define NVM_CA_PAIR__NO_SIZE        8

#define NVM_CA_PAIR_VALID_FLAG_ADDR        (NVM_CA_DATA_BASE + 8)
#define NVM_CA_PAIR__VALID_FLAG_SIZE        2

#define NVM_USE_CA_PIN_TAG_ADDR        (NVM_CA_DATA_BASE + 10)
#define NVM_USE_CA_PIN_TAG_SIZE        1

/*��������ѶCA ʹ��*/
#define NMV_LOCKFLAG_ADDR  (NVM_CA_DATA_BASE + 11)
#define NMV_LOCKFLAG_SIZE  1

#define NVM_CA_BUFFER_ADDR        (NVM_CA_DATA_BASE + 12)
#define NVM_CA_BUFFER_SIZE        700

/*��Ҫ�����������Ӳ����ܿ�����Ӧ��, ����������Ϣ*/
#define NVM_CA_SC_UPGRADE_INFO 			(NVM_CA_DATA_BASE+712)
#define NVM_CA_SC_UPGRADE_INFO_SIZE 	32

/*��������ѶCA ʹ��*/
#define NVM_arealockfre_ADDR 			(NVM_CA_DATA_BASE + 800)
#define NVM_arealockfre_SIZE 			4
#define NVM_arealockSyml_ADDR 			(NVM_CA_DATA_BASE + 804)
#define NVM_arealockSyml_SIZE 			4
#define NVM_arealockQAM_ADDR 			(NVM_CA_DATA_BASE + 808)
#define NVM_arealockQAM_SIZE 			2

#define NVM_NIT_LOCK_ADDR 				(NVM_CA_DATA_BASE + 810)
#define NVM_NIT_LOCK_SIZE 				4

/*CA����ʹ�õ�E2p��ַ-------------END---*/

/*��Ƶ����������*/
#define NVM_CHANNEL_VOLUME_BASE_ADDR			 (NVM_BASE + 2000)
#define NVM_CHANNEL_VOLUME_BASE_SIZE		  		 1000

/*WASU*/
#define NVM_WASU_CITYCODE_ADDR       ( NVM_BASE + 4000)
#define NVM_WASU_CITYCODE_SIZE           2
#define NVM_WASU_STBID_ADDR          ( NVM_WASU_CITYCODE_ADDR + 2)
#define NVM_WASU_STBID_SIZE              32


/*Flashʹ�ÿ�滮,�����ϲ�ʹ�õ�Flash�鶼�����ڴ˽��ж���,��ֹ�ظ�ʹ��,������
�ڶ���ǰ��ؼ�����汾���ʣ���ĸ���,��ֹ�������,������0��ʼ�����ۼ�*/

/*���ݿ�������ʹ��ר�ŵĽӿڶ�д��*/

/*���ڱ���Ӧ����չƵ������*/
#define NVM_EXTENDED_APPLICATION_USE_BLOCK_NO			0

/*���ڱ���EIS����*/
#define NVM_EIS_DATA_USE_BLOCK_NO						3

/*���ڱ���㲥��������*/
#define NVM_RADIO_BACKGROUND_USE_BLOCK_NO				4

/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMReadtoImage
  Description     : ��E2P���ݿ������ڴ�
  Input           : 
  Outut           : �ɹ�����0; ʧ�ܷ���1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
S32_YS DRV_NVMReadtoImage (void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMRead
  Description     : ��ȡe2pָ��λ�á����ȵ�����
  Input           : 
  Outut           : �ɹ�����0; ʧ�ܷ���1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
S32_YS DRV_NVMRead (S32_YS iOffset, S32_YS nLength, U8_YS *data);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMWrite
  Description     : ��e2pд��ָ��λ�á����ȵ�����
  Input           : 
  Outut           : �ɹ�����0; ʧ�ܷ���1 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ���ö��д����ֹдʧ��
*****************************************************************************/ 
S32_YS DRV_NVMWrite ( S32_YS iOffset, S32_YS nLength, U8_YS *data );

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMFlush
  Description     : ��E2P����������Ϊ0xFF
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_NVMFlush(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_FactInited
  Description     : ��ȡE2p��FactCfg�����Ƿ��Ѿ�����ʼ����
  Input           : 
  Outut           : ����ʼ����������1�����򷵻�0
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_FactInited(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadFactCfg
  Description     : ��ȡE2p��FactCfg���������
  Input           : 
  Outut           : �������Ч���ݣ�����0�����򷵻�1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_ReadFactCfg(U8_YS *pData);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadFactCfg
  Description     : ��E2p��FactCfg����д��factcfg����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ������ǵ�һ��д��FactCfg����Ҫд��0xbeebebee��Ϊ��ʼ�����
*****************************************************************************/ 
void DRV_WriteFactCfg(U8_YS *pData);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadAppCfg
  Description     : ��ȡE2p��AppCfg��������
  Input           : 
  Outut           : �������Ч���ݣ�����0�����򷵻�1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_ReadAppCfg(U8_YS *pData);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteAppCfg
  Description     : д��E2p��AppCfg��������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteAppCfg(U8_YS *pData);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeMark
  Description     : д��ǿ��������ʶ��0x51DB
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeMark();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearUpgradeMark
  Description     : ����ǿ��������ʶ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearUpgradeMark();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPUpgrade
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteIPUpgrade(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPUpgradeNoAppMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteIPUpgradeNoAppMark(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeMark2
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeMark2();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeMark2
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U16 DRV_ReadUpgradeMark2();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeType
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U16_YS DRV_ReadUpgradeType(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteApplicationMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteApplicationMark();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearApplicationMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearApplicationMark();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeTS
  Description     : д������Ƶ����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeTS(U32_YS Fre,U16_YS Sym, U16_YS Qam);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeTS
  Description     : ��ȡ����Ƶ����Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ReadUpgradeTS(U32* pFre,U16* pSym, U8* pQam);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeTS
  Description     : д������UMT PID��TableID��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeUMT(U16_YS pid,U16_YS tableid);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeUMT
  Description     : ��ȡ����UMT PID��TableID��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ReadUpgradeUMT(U16* pPid, U16* pTableid);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDefaultSettingTag
  Description     : д��Ĭ������tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteDefaultSettingTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearDefaultSettingTag
  Description     : ���Ĭ������tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearDefaultSettingTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_CheckDefaultSettingTag
  Description     : ���Ĭ������tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_CheckDefaultSettingTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannel
  Description     : ��ȡ�ػ�ǰ��Ƶ��(��ǰƵ��)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ�㿪�����ã���ȡ�ػ�ǰ��Ƶ��
*****************************************************************************/ 
U16_YS DRV_DTVReadChannel(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannel
  Description     : д��ػ�ǰ��Ƶ��(��ǰƵ��)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ������̨����ã����浱ǰƵ��
*****************************************************************************/ 
void DRV_DTVWriteChannel(U16_YS chn);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannelLockPassword
  Description     : ��ȡƵ��������
  Input           : 
  Outut           : ��������
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ�����û�����
*****************************************************************************/ 
U32_YS DRV_DTVReadChannelLockPassword();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannelLockPassword
  Description     : д��Ƶ��������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ�����û�����
*****************************************************************************/ 
void DRV_DTVWriteChannelLockPassword(U32_YS uiPassword);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannelLockPasswordModifiedTag
  Description     : ��ȡƵ���������޸ı�ʶ
  Input           : 
  Outut           : ����Ƶ���������޸ı�ʶ
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ�����û�����
*****************************************************************************/ 
S8_YS DRV_DTVReadChannelLockPasswordModifiedTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannelLockPasswordModifiedTag
  Description     : д��Ƶ���������޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : һ�����û�����
*****************************************************************************/ 
void DRV_DTVWriteChannelLockPasswordModifiedTag(S8_YS  scTag);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadChnlOrderChangedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadChnlOrderChangedTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteChnlOrderChangedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteChnlOrderChangedTag(U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNetworkID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNetworkID();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNetworkID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNetworkID(U16_YS chn);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadPDTVersionNO
  Description     : ��ȡ���ݹ��İ汾��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadPDTVersionNO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WritePDTVersionNO
  Description     : д����ݹ��İ汾��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WritePDTVersionNO(U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStockChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadStockChannelNO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteStockChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteStockChannelNO(U8_YS usChannelNO);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadLanguageSetting
  Description     : ��ȡ��������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadLanguageSetting();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteLanguageSetting
  Description     : д����������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteLanguageSetting(U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCARegionCode
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadCARegionCode();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCARegionCode
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCARegionCode(U32_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDBCTS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadDBCTS(U32_YS *Fre,U16_YS *Sym, U8 *Qam);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDBCTS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDBCTS(U32_YS Fre,U16_YS Sym, U8 Qam);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCityEyesChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadCityEyesChannelNO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCityEyesChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCityEyesChannelNO(U8_YS usChannelNO);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDtviaTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadDtviaTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDtviaTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDtviaTag(U8_YS ucTag);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadApplicationType
  Description     : ��ȡ�������������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadApplicationType();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteApplicationType
  Description     : д��������������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteApplicationType(U8_YS ucTag);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadSYSSETPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
S32_YS DRV_DTVReadSYSSETPassword();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadSYSSETPasswordModifiedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
S8_YS DRV_DTVReadSYSSETPasswordModifiedTag();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteSYSSETPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
void DRV_DTVWriteSYSSETPassword(S32_YS uiPassword);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteSYSSETPasswordModifiedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
void DRV_DTVWriteSYSSETPasswordModifiedTag(S8_YS  scTag);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteSYSSETShowTime
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
void DRV_WriteSYSSETShowTime(S8_YS ShowTime);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadSYSSETShowTime
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :δʹ��
*****************************************************************************/ 
S8_YS DRV_ReadSYSSETShowTime(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNVODServiceNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNVODServiceNO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNVODServiceNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNVODServiceNO(U8_YS usChannelNO);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNVODEventID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNVODEventID();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNVODEventID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNVODEventID(U8_YS usChannelNO);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZone
  Description     : ��ȡʱ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadTimeZone(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZone
  Description     : д��ʱ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteTimeZone(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZoneTag
  Description     : ��ȡʱ���޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadTimeZoneTag(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteTimeZoneTag
  Description     : д��ʱ���޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteTimeZoneTag(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTime
  Description     : ��ȡ��ʾʱ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadShowTime(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteShowTime
  Description     : д����ʾʱ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteShowTime(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTimeTag
  Description     : ��ȡ��ʾʱ���޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadShowTimeTag(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTimeTag
  Description     : д����ʾʱ���޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteShowTimeTag(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVideoMode
  Description     : ��ȡ��Ƶ�����ʽ(N/P)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVideoMode(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVideoMode
  Description     : д����Ƶ�����ʽ(N/P)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVideoMode(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVideoModeTag
  Description     : ��ȡ��Ƶ�����ʽ(N/P)�޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVideoModeTag(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVideoModeTag
  Description     : д����Ƶ�����ʽ(N/P)�޸ı�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVideoModeTag(U8_YS ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUseCAPINTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadUseCAPINTag (void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUseCAPINTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteUseCAPINTag (U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUsePasswordTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadUsePasswordTag(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUsepasswordTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteUsepasswordTag(U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStartFre
  Description     : ��ȡ��Ƶ�������Ŀ�ʼ����Ƶ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadStartFre(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStartFre
  Description     : д���Ƶ�������Ŀ�ʼ����Ƶ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteStartFre(U16_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadEndFre
  Description     : ��ȡ��Ƶ�������Ľ�������Ƶ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadEndFre(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteEndFre
  Description     : д���Ƶ�������Ľ�������Ƶ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteEndFre(U16_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCAPINPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
S32_YS DRV_ReadCAPINPassword(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCAPINPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCAPINPassword(S32_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadInversionTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadInversionTag(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteInversionTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteInversionTag(U8_YS vol);

#ifndef NVM_USE_FIXED_SYSTEM_OFFSET
/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDtviaDataVersion
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32 DRV_ReadDtviaDataVersion(U8_YS ucFiletype );

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDtviaDataVersion
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDtviaDataVersion(U8_YS ucFiletype, U32 ucTimeZone);
#endif


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadBootPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16 DRV_ReadBootPassword(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteBootPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteBootPassword(U16 ucTimeZone);

#ifndef NVM_USE_FIXED_SYSTEM_OFFSET
/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadChnlChngTimingSet
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadChnlChngTimingSet(U8_YS *ucTimeZone);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteChnlChngTimingSet
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteChnlChngTimingSet(U8_YS *ucTimeZone);
#endif

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadRadioPDTVersionNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadRadioPDTVersionNO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteRadioPDTVersionNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteRadioPDTVersionNO(U8_YS vol);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPAddr(U32_YS ip);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPMask
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPMask(U32_YS mask);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPGW
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPGW(U32_YS gw);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPDNS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPDNS(U32_YS dns);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPAddr();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPMask
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPMask();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPGW
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPGW();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPDNS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPDNS();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPAUTO(U8_YS ipauto);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadIPAUTO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDNSAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDNSAUTO(U8_YS dnsauto);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDNSAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadDNSAUTO();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVODServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVODServerAddr(U32_YS ip);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVODServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadVODServerAddr();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteHTTPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteHTTPServerAddr(U32_YS ip);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadHTTPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadHTTPServerAddr();

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteMACAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteMACAddr(U8_YS * mac_addr);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadMACAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadMACAddr(U8_YS * mac_addr);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPServerAddr(void);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVoutMode
  Description     : д����Ƶ����ӿ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVoutMode(U8_YS vout_mode);

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVoutMode
  Description     : ��ȡ��Ƶ����ӿ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVoutMode(void);

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_ReadSTBParamsMark
  Description     : ��ȡ�����ŵı��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32 DRV_ReadSTBParamsMark(void);

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_WriteSTBParamsMark
  Description     : д�����ŵı��λ��0xABCD�ǽ���ı�ǣ���Ҫ��bootһ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteSTBParamsMark(HI_U16 STBParamsMark);

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_WriteJzPrivateData
  Description     : д����ų����˽�����ݣ�
  					һ��Ϊ�ط����е����û��������Ӧ���Լ����岢����ʹ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteJzPrivateData(unsigned char *pData, unsigned short usLen);

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_ReadJzPrivateData
  Description     : ��ȡ���ų����˽�����ݣ�
  					һ��Ϊ�ط����е����û��������Ӧ���Լ����岢����ʹ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadJzPrivateData(unsigned char *pData, unsigned short *usLen);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif



