#########################################################################
#  File name: ysstb.mak
#########################################################################
#˵��
#
#
#
####################
## System                        ##
####################
ifeq "$(DVD_PLATFORM)" "hmp"
	SRC_STRING := _HMP7109
endif
ifeq "$(DVD_PLATFORM)" "cab7167"
	SRC_STRING := _CAB7167
endif

    CFLAGS := $(CFLAGS) -D_YS7101_
ifneq "$(DVD_ADDRESSMODE)" "29"
    CFLAGS := $(CFLAGS) -DADDRESSMODE32
endif

ifneq "$(JZBY_NAND_FLASH_SUPPORT)" "1"
    CFLAGS := $(CFLAGS) -DJZBY_NAND_FLASH_SUPPORT
endif
ifeq "$(BUILD_MODULE_VC)" "1"
    CFLAGS := $(CFLAGS) -DJZ_CAMERA
endif
ifeq "$(BUILD_MODULE_PLAYREC)" "1"
    CFLAGS := $(CFLAGS) -DJZ_PLAYREC
endif

ifeq "$(YSENV_PLATFORM)" "HI3716M"
    CFLAGS := $(CFLAGS) -DCHIP_HI3716M
endif

ifeq "$(YSENV_PLATFORM)" "HI3716MV300"
    CFLAGS := $(CFLAGS) -DCHIP_HI3716MV300
endif

ifeq "$(YSENV_PLATFORM)" "HI3716MV300TFADV"
    CFLAGS := $(CFLAGS) -DCHIP_HI3716MV300TFADV
endif

ifeq "$(YSENV_PLATFORM)" "HI3716MV300SMADV"
    CFLAGS := $(CFLAGS) -DCHIP_HI3716MV300SMADV
endif

ifeq "$(YSENV_PLATFORM)" "HI3716C"
    CFLAGS := $(CFLAGS) -DCHIP_HI3716C
endif

ifeq "$(BIGUA)" "TRUE"
    CFLAGS := $(CFLAGS) -DBIGUA
endif

ifeq "$(FLASH_TYPE)" "EMMC"
    CFLAGS := $(CFLAGS) -DEMMC_FLASH
endif

ifeq "$(DRV_HDMISWITCH)" "TRUE"
	CFLAGS := $(CFLAGS) -DDRV_HdmiSwitch
endif

    BUILD_MODULE_PLAYREC:=0
    DVD_HWVER_BIND:=$(HardwareVersion)
    LOCALIZATION_STRING := _stdver
    CFLAGS := $(CFLAGS)  -DSTDVER
    CFLAGS := $(CFLAGS) -DBUILDER="\"$(USER)\""
    CFLAGS := $(CFLAGS) -DKERNELVER=$(KernelVersion)
    CFLAGS := $(CFLAGS) -DROOTWVER=$(RootVersion)
    CFLAGS := $(CFLAGS) -DHWVER=$(HardwareVersion)
    CFLAGS := $(CFLAGS) -DSWVER=$(SoftwareVersion)
    CFLAGS := $(CFLAGS) -DCATPYE="\"$(CAType)\""
    CFLAGS := $(CFLAGS) -DFILETYPE="\"$(FileType)\""
    CFLAGS := $(CFLAGS) -DDRV_IPTV_SUPPORT
    CFLAGS := $(CFLAGS) -DDRVVER=$(DriverVersion)
    CFLAGS := $(CFLAGS) -DLOADERVER=$(LoaderVersion)

####################
## Include                        ##
####################
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/pub/$(CFG_HI3716_CHIP_TYPE)/include
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/pub/$(CFG_HI3716_CHIP_TYPE)/lib
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/pub/$(CFG_HI3716_CHIP_TYPE)/rootbox/usr/lib
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ysdrvinner
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/plus
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/mid
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/drv
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/plus
INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/dbc/eis/wasu/cloud/include/adapter

ifeq "$(BUILD_MODULE_PLAYREC)" "1"
    INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/driver/driver$(LOCALIZATION_STRING)/ysplayrec/include
endif
####################
## Driver                         ##
####################
	##drv_sys.c�м�testtool���ļ�����ʱ��Ҫ�õ������ܶ���OSPLUS��ST_OS21
	CFLAGS := $(CFLAGS) -DST_OSLINUX

	##�ú꿪�ؿ����Ƿ�ʹ�ö���Transcoder���ܣ�Ĭ�ϲ�֧��
	#CFLAGS := $(CFLAGS) -DDRV_TRANSCODER_SERVER
	# ����TRANSCODERģ��
	#STBCFG_MODULE_INCLUDE_TRANSCODER = 1
####################
## Cas                             ##
####################
    ## �ú꿪���뱣�ֿ���״̬,��Ҫ�Ƕ��������CA ���ͨ�ýӿ�.Lqjun.20090316.
    # ���� CAS 
    # �꿪��
    #CFLAGS := $(CFLAGS) -DINCLUDE_CAS
    # ����CASģ��
    #STBCFG_MODULE_INCLUDE_CAS = 1

#####################################################
#����CAS ģ���У������CA �汾
#��ͬ��CA����������ѡ�񣬴Ӷ�������Ӧ��API���CAS�������
#��Ҫ�ֶ�ѡ��
#####################################################
BUILD_CAS_TFCAS21PRO :=0#��ͨ��ͬ���汾,��Ӧstapi_stpti.lib
BUILD_CAS_TFCAS21PROEX :=0#�µ�TF CAS����Ӧstapi_stpti_TF21PROEX.lib
BUILD_CAS_CDCAS30 :=0#�µ�TF CAS����Ӧstapi_stpti_TF21PROEX.lib
BUILD_CAS_SMSX51 :=0#��Ӧstapi_stpti.lib
BUILD_CAS_IRDETO221 :=0#��Ӧstapi_stpti_IRDETO.lib
BUILD_CAS_IRDETO3420 :=0#��Ӧstapi_stpti_IRDETO.lib
BUILD_CAS_IRDETO378 :=0#��Ӧstapi_stpti_IRDETO.lib
BUILD_CAS_NAGRA_CAK6 :=0#��Ӧstapi_stpti_NAGRA.lib
BUILD_CAS_DTVIA21 :=0#��Ӧstapi_stpti_DTVIA.lib
BUILD_CAS_CTICAS225 :=0#��Ӧstapi_stpti_CTICAS.lib
BUILD_CAS_CTICAS300 :=0#�ݲ�֧��
BUILD_CAS_CTICAS301 :=0#��Ӧstapi_stpti_CTICAS.lib
BUILD_CAS_DTCAS201 :=0#��Ӧstapi_stpti.lib
BUILD_CAS_DVNCAS202 :=0#�ݲ�֧��
BUILD_CAS_CDMCAS21 :=0#����CA����ƶ�Ӧstapi_stpti_TF21PROEX.lib
BUILD_CAS_SUCCEED:=0

##BUILD_CAS_TFCAS21PRO:=1
ifeq "$(BUILD_CAS_TFCAS21PRO)" "1"
  CFLAGS := $(CFLAGS) -DTFCAS21PRO
  CA_LIB_NAME :=_tfcas21pro
  API_LIB_NAME :=#����Ϊ�գ��Ҳ����пո�
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/tfcas21pro
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_TFCAS21PROEX :=1
ifeq "$(BUILD_CAS_TFCAS21PROEX)" "1"
  CFLAGS := $(CFLAGS) -DTFCAS21PROEX
  CA_LIB_NAME :=_tfcas21proex
  API_LIB_NAME :=_TF21PROEX
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/tfcas21proex
  BUILD_CAS_SUCCEED:=1
endif

ifeq "$(BUILD_CAS_CDCAS30)" "1"
  CFLAGS := $(CFLAGS) -DCDCAS30
  CA_LIB_NAME :=_cdcas30
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/tf_cas/include
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_SMSX51:=1
ifeq "$(BUILD_CAS_SMSX51)" "1"
  CFLAGS := $(CFLAGS) -DSMSX51
  CA_LIB_NAME :=_smsx51
  API_LIB_NAME :=#����Ϊ�գ��Ҳ����пո�  
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/sm_cas/include
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_DVN:=1
ifeq "$(BUILD_CAS_DVN)" "1"
  CFLAGS := $(CFLAGS) -DDVN
  CA_LIB_NAME :=_dvn
  API_LIB_NAME :=#����Ϊ�գ��Ҳ����пո�  
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/dvn_cas/include
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_IRDETO221:=1
ifeq "$(BUILD_CAS_IRDETO221)" "1"
  CFLAGS := $(CFLAGS) -DIRDETO221
  CA_LIB_NAME :=_irdeto221
  API_LIB_NAME :=_IRDETO
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/irdeto221
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_IRDETO3420:=1
ifeq "$(BUILD_CAS_IRDETO3420)" "1"
  CFLAGS := $(CFLAGS) -DIRDETO3420
  CA_LIB_NAME :=_irdeto3420
  API_LIB_NAME :=_IRDETO
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/irdeto3420
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_IRDETO378:=1
ifeq "$(BUILD_CAS_IRDETO378)" "1"
  CFLAGS := $(CFLAGS) -DIRDETO378
  CA_LIB_NAME :=_irdeto378
  API_LIB_NAME :=_IRDETO
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/irdeto378
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_NAGRA_CAK6:=1
ifeq "$(BUILD_CAS_NAGRA_CAK6)" "1"
  CFLAGS := $(CFLAGS) -DNAGRA_CAK6
  CA_LIB_NAME :=_nagracak6
  API_LIB_NAME :=_NAGRA
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/nagracak6
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_DTVIA21:=1
ifeq "$(BUILD_CAS_DTVIA21)" "1"
  CFLAGS := $(CFLAGS) -DDTVIA21
  CA_LIB_NAME :=_dtvia21
  API_LIB_NAME :=_DTVIA
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/dtvia21
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_CTICAS225:=1
ifeq "$(BUILD_CAS_CTICAS225)" "1"
  CFLAGS := $(CFLAGS) -DCTICAS225
  CA_LIB_NAME :=_cticas225
  API_LIB_NAME :=_CTICAS
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/cticas225
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_CTICAS301:=1
ifeq "$(BUILD_CAS_CTICAS301)" "1"
  CFLAGS := $(CFLAGS) -DCTICAS301
  CA_LIB_NAME :=_cticas301
  API_LIB_NAME :=_CTICAS
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/cticas301
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_DTCAS201:=1
ifeq "$(BUILD_CAS_DTCAS201)" "1"
  CFLAGS := $(CFLAGS) -DDTCAS201
  CA_LIB_NAME :=_dtcas201
  API_LIB_NAME :=#����Ϊ�գ��Ҳ����пո�  
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/dtcas201
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_CDMCAS21:=1
ifeq "$(BUILD_CAS_CDMCAS21)" "1"
  CFLAGS := $(CFLAGS) -DCDMCAS21
  CA_LIB_NAME :=_cdmcas21
  API_LIB_NAME :=_TF21PROEX
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/cdmcas21
  BUILD_CAS_SUCCEED:=1
endif

#ifeq "$(BUILD_CAS_SUCCEED)" "0"
#  $(error you should select right CAS type!)
#endif

####################
## IP Ĭ�ϴ�    ##
####################
	JZ_IP_SUPPORT = 1
ifeq "$(JZ_IP_SUPPORT)" "1"
    CFLAGS := $(CFLAGS) -DJZ_IP_SUPPORT
endif

####################
## Dbc                             ##
####################

    #�Ƿ񼯳����ݹ㲥����Ʊ��VOD�����ŵ�(�Ƿ����\ysstb\dbc\ģ��)
    CFLAGS := $(CFLAGS) -D_INCLUDE_DBC_
    STBCFG_MODULE_INCLUDE_DATABROADCAST = 1	

    #####################################################
    #�����                 
    #####################################################

    # ���� �Ѵ����ݹ㲥 ����Ʊ�����(ע�⣺�Ѵ� �� ��׳���ݹ㲥���ܲ���)
    #CFLAGS := $(CFLAGS) -D_AVIT_DBC_
    #STBCFG_MODULE_INCLUDE_AVIT_DBC = 1	

    # ���� ��׳���ݹ㲥  ����Ʊ(ע�⣺�Ѵ� �� ��׳���ݹ㲥���ܲ���)
    CFLAGS := $(CFLAGS) -D_EIS_DBC_
    STBCFG_MODULE_INCLUDE_EIS_DBC = 1
    INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/dbc/eis/include

    # ���� ��׳���ݹ㲥2.8  �汾  (ע�⣺ ��׳2.8�汾������2.8֮ǰ�İ汾���棩
    #STBCFG_MODULE_INCLUDE_EIS28_DBC = 1
    #INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/dbc/eis28/include    
    
    #����������ݹ㲥����Ʊ
    #CFLAGS := $(CFLAGS) -D_DVN_DBC_  
    #STBCFG_MODULE_INCLUDE_DVN_DBC = 1

    #����Ӱ�������ݹ㲥(������Ʊvod)
    #CFLAGS := $(CFLAGS) -D_ENREACH_DBC_
    #STBCFG_MODULE_INCLUDE_ENREACH_DBC = 1

    #���ɶ�̫���ݹ㲥����Ʊ
    #CFLAGS := $(CFLAGS) -D_DT_DBC_
    #STBCFG_MODULE_INCLUDE_DT_DBC = 1

    #####################################################
    #���
    #####################################################
    # ���ɻ������
    #CFLAGS := $(CFLAGS) -D_WASU_PLUG_
    STBCFG_MODULE_INCLUDE_WASU_PLUG = 1    
    #STBCFG_MODULE_INCLUDE_WASU_CLOUD_PLUG = 1    
    STBCFG_MODULE_INCLUDE_WASU_CA_PLUG = 1    
    # ʹ���µ��ֱ��淶
    #CFLAGS := $(CFLAGS) -D_WASU_PLUG_JOYSTICK_

    #####################################################
    #��Ʊ
    #####################################################
    # ���ɺ����ٵ¹�Ʊ
    #CFLAGS := $(CFLAGS) -D_HTRD_STOCK_
    #STBCFG_MODULE_INCLUDE_HTRD_STOCK = 1
   
    # ���ɳ����Ʊ
    #CFLAGS := $(CFLAGS) -D_CHANGHONG_STOCK_
    #STBCFG_MODULE_INCLUDE_CHANGHONG_STOCK = 1

    #####################################################
    #VOD
    #####################################################  
    #���ɻ���VOD
    #CFLAGS := $(CFLAGS) -D_WASU_VOD_
    #STBCFG_MODULE_INCLUDE_WASU_VOD = 1	

    # ����Ħ������VOD
    #CFLAGS := $(CFLAGS) -D_MOTOROLA_VOD_
    #STBCFG_MODULE_INCLUDE_MOTOROLA_VOD = 1

    # ���ɻ�ΪVOD
    #CFLAGS := $(CFLAGS) -D_HUAWEI_VOD_
    #STBCFG_MODULE_INCLUDE_HUAWEI_VOD = 1

    # ����ͬ��VOD
    #CFLAGS := $(CFLAGS) -D_COSHIP_VOD_
    #STBCFG_MODULE_INCLUDE_COSHIP_VOD = 1
    
    #����˼ǨVOD
    #CFLAGS := $(CFLAGS) -D_SEACHANGE_VOD_
    #STBCFG_MODULE_INCLUDE_SEACHANGE_VOD = 1	

    # ����UTStarcomVOD
    #CFLAGS := $(CFLAGS) -D_UTSTARCOM_VOD_
    #STBCFG_MODULE_INCLUDE_UTSTARCOM_VOD = 1

    # ���ɶ�������VOD
    #CFLAGS := $(CFLAGS) -D_OVT_VOD_
    #STBCFG_MODULE_INCLUDE_OVT_VOD = 1
    
    #####################################################
    #��漰����
    #####################################################  
    #���ɻ��Ӷ���Ϣϵͳ (���ر��� appentry,dvbtable,update,digitv)
    #CFLAGS := $(CFLAGS) -D_HS_MESSAGE_
    #STBCFG_MODULE_INCLUDE_HS_MESSAGE_ = 1

    # �������ƹ��   
    #CFLAGS := $(CFLAGS) -D_CYCLE_AD_
    #STBCFG_MODULE_INCLUDE_CYCLE_AD_ = 1
    
    # ������׳���   
    #CFLAGS := $(CFLAGS) -D_IPANEL_AD_
    #STBCFG_MODULE_INCLUDE_IPANEL_AD_ = 1

#####################################################
#Ӧ��ģ�鹦��ѡ��
#
#��Ҫ�ֶ�ѡ��
#####################################################
    #�Ƿ�֧��umt����
    #CFLAGS := $(CFLAGS) -D_JZ_UMT_UPGRADE_

    #�Ƿ�֧�ֿ������
    #CFLAGS := $(CFLAGS) -D_JZ_ADVERTISING_



    #  CAS EMM ����
    ##CFLAGS := $(CFLAGS) -DSTBCFG_CAS_EMM_DEBUG
    #����������汾��
    ifeq "$(SoftwareVersion)" ""
    $(error you should select right SoftwareVersion!)
	endif
    YSSTB_SOFTWARE_VERSION:=$(SoftwareVersion)
    CFLAGS := $(CFLAGS) -DYSSTB_SOFTWARE_VERSION=$(SoftwareVersion)
    #�������������
    CFLAGS := $(CFLAGS) -DYSSTB_SOFTWARE_TYPE=0x00

    #��Ƶ����������(Ĭ�Ͽ���)
    CFLAGS := $(CFLAGS) -D_NVM_FOR_SERVICE_VOL_
    
    #32������
    CFLAGS := $(CFLAGS) -D_VOLUME_32_

    #���������˹���
    CFLAGS := $(CFLAGS) -D_MOSAIC_
	
    #Ƶ����(1��3λģʽ)Ĭ�϶���1λ
    CFLAGS := $(CFLAGS) -D_CHNUM_INPUTMODE_ONE_
    
    #����Ƶ����δ��3λ��ȷ�ϼ���̨
    #CFLAGS := $(CFLAGS) -D_CHNUMBER_AUTO_ENTER_
    
    #����������dtv�¼�ʱ��ʾ��ͨ��Ӧ�õ���(�򿪴˿��ر�ʾ��ʱ��ʾ)
    CFLAGS := $(CFLAGS) -D_SOUNDTYPE_BANNERSHOW_

    #��̨�������һ֡
    #CFLAGS := $(CFLAGS) -D_CHCHAN_KEEP_LAST_FRAME_

    #�˵��л������ϴν���
    #CFLAGS := $(CFLAGS) -D_MENU_KEEP_FOCUS

    # EPG��EIS�����ڴ�,���ر��� dvbtable dbc main
    #CFLAGS := $(CFLAGS) -D_SHARED_MEM_OF_EIT_AND_DBC_

    # ���ɾ��ݵ�����վ
    #JZ_TVNET_SUPPORT = 1
	ifeq "$(JZ_TVNET_SUPPORT)" "1"
    CFLAGS := $(CFLAGS) -D_JZ_TVNET_SUPPORT_
	endif

    # ���ɾ��ݹ��
    #CFLAGS := $(CFLAGS) -D_JIUZHOU_AD_

    #���ɾ��ݿ���������
    #CFLAGS := $(CFLAGS) -D_JIUZHOU_PDT_

    # ���ɿ��ѹ��
    #CFLAGS := $(CFLAGS) -D_KONKA_AD_

    # ���ɴ�ά���
    #CFLAGS := $(CFLAGS) -D_SKYWORTH_AD_

	# ���ɶ��������
	CFLAGS := $(CFLAGS) -D_MULTY_AUDIO_

	# ���ɶ�Subtitle����
	#CFLAGS := $(CFLAGS) -D_MULTY_SUBTITLE_

	# ���ɽ�Ŀ��鹦��
	#CFLAGS := $(CFLAGS) -D_SERVICE_INFORMATION_

	# �ű���Ƶ����
	CFLAGS := $(CFLAGS) -D_DOLBY_SUPPORT_
	STBCFG_MODULE_INCLUDE_DOLBY = 1	
	#CFLAGS := $(CFLAGS) -DDRV_JZBY_AUDIO

	#֧��USB����
	JZ_USB_SUPPORT = 1
ifeq "$(JZ_USB_SUPPORT)" "1"
	# ����USBӦ�ù���
	CFLAGS := $(CFLAGS) -DJZ_USB_SUPPORT
	STBCFG_MODULE_INCLUDE_USBAPP = 1	

	# ����PVR¼�Ʋ��Ź���
	CFLAGS := $(CFLAGS) -DJZ_PVR_SUPPORT
	STBCFG_MODULE_INCLUDE_PVR = 1
	
	# ����PVRԤԼ¼�ƹ���
	CFLAGS := $(CFLAGS) -DJZ_PVR_BOOK_SUPPORT
	STBCFG_MODULE_INCLUDE_PVR_BOOK = 1
	
	# ����ʱ���ֶ����ô�С����(Ĭ�Ϲر�)
	#CFLAGS := $(CFLAGS) -DJZ_TIMESHIFT_SETBYHAND
endif

	# ֧�ֿ����޷���ʾ
	CFLAGS := $(CFLAGS) -DJZ_SEAMLESS_DISPLAY_SUPPORT



	#֧��TUNER  ѡ�� ATMB886X����
ifeq "$(HardwareVersion)" "0x684"
	CFLAGS := $(CFLAGS) -DATBM886
	ATBM886X = 1
endif

