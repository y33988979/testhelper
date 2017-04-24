#########################################################################
#  File name: ysstb.mak
#########################################################################
#说明
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
	##drv_sys.c中及testtool中文件操作时需要用到，不能定义OSPLUS及ST_OS21
	CFLAGS := $(CFLAGS) -DST_OSLINUX

	##该宏开关控制是否使用多屏Transcoder功能，默认不支持
	#CFLAGS := $(CFLAGS) -DDRV_TRANSCODER_SERVER
	# 链接TRANSCODER模块
	#STBCFG_MODULE_INCLUDE_TRANSCODER = 1
####################
## Cas                             ##
####################
    ## 该宏开关请保持开启状态,主要是定义包含了CA 相关通用接口.Lqjun.20090316.
    # 集成 CAS 
    # 宏开关
    #CFLAGS := $(CFLAGS) -DINCLUDE_CAS
    # 链接CAS模块
    #STBCFG_MODULE_INCLUDE_CAS = 1

#####################################################
#加载CAS 模块中，具体的CA 版本
#不同的CA，仅仅在这选择，从而决定对应的API库和CAS库的名字
#需要手动选择
#####################################################
BUILD_CAS_TFCAS21PRO :=0#普通的同方版本,对应stapi_stpti.lib
BUILD_CAS_TFCAS21PROEX :=0#新的TF CAS，对应stapi_stpti_TF21PROEX.lib
BUILD_CAS_CDCAS30 :=0#新的TF CAS，对应stapi_stpti_TF21PROEX.lib
BUILD_CAS_SMSX51 :=0#对应stapi_stpti.lib
BUILD_CAS_IRDETO221 :=0#对应stapi_stpti_IRDETO.lib
BUILD_CAS_IRDETO3420 :=0#对应stapi_stpti_IRDETO.lib
BUILD_CAS_IRDETO378 :=0#对应stapi_stpti_IRDETO.lib
BUILD_CAS_NAGRA_CAK6 :=0#对应stapi_stpti_NAGRA.lib
BUILD_CAS_DTVIA21 :=0#对应stapi_stpti_DTVIA.lib
BUILD_CAS_CTICAS225 :=0#对应stapi_stpti_CTICAS.lib
BUILD_CAS_CTICAS300 :=0#暂不支持
BUILD_CAS_CTICAS301 :=0#对应stapi_stpti_CTICAS.lib
BUILD_CAS_DTCAS201 :=0#对应stapi_stpti.lib
BUILD_CAS_DVNCAS202 :=0#暂不支持
BUILD_CAS_CDMCAS21 :=0#中数CA宏控制对应stapi_stpti_TF21PROEX.lib
BUILD_CAS_SUCCEED:=0

##BUILD_CAS_TFCAS21PRO:=1
ifeq "$(BUILD_CAS_TFCAS21PRO)" "1"
  CFLAGS := $(CFLAGS) -DTFCAS21PRO
  CA_LIB_NAME :=_tfcas21pro
  API_LIB_NAME :=#必须为空，且不能有空格
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
  API_LIB_NAME :=#必须为空，且不能有空格  
  INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/ysinclude/ca/sm_cas/include
  BUILD_CAS_SUCCEED:=1
endif

##BUILD_CAS_DVN:=1
ifeq "$(BUILD_CAS_DVN)" "1"
  CFLAGS := $(CFLAGS) -DDVN
  CA_LIB_NAME :=_dvn
  API_LIB_NAME :=#必须为空，且不能有空格  
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
  API_LIB_NAME :=#必须为空，且不能有空格  
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
## IP 默认打开    ##
####################
	JZ_IP_SUPPORT = 1
ifeq "$(JZ_IP_SUPPORT)" "1"
    CFLAGS := $(CFLAGS) -DJZ_IP_SUPPORT
endif

####################
## Dbc                             ##
####################

    #是否集成数据广播、股票、VOD、短信等(是否包含\ysstb\dbc\模块)
    CFLAGS := $(CFLAGS) -D_INCLUDE_DBC_
    STBCFG_MODULE_INCLUDE_DATABROADCAST = 1	

    #####################################################
    #浏览器                 
    #####################################################

    # 集成 佳创数据广播 、股票、广告(注意：佳创 和 茁壮数据广播不能并存)
    #CFLAGS := $(CFLAGS) -D_AVIT_DBC_
    #STBCFG_MODULE_INCLUDE_AVIT_DBC = 1	

    # 集成 茁壮数据广播  、股票(注意：佳创 和 茁壮数据广播不能并存)
    CFLAGS := $(CFLAGS) -D_EIS_DBC_
    STBCFG_MODULE_INCLUDE_EIS_DBC = 1
    INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/dbc/eis/include

    # 集成 茁壮数据广播2.8  版本  (注意： 茁壮2.8版本不能与2.8之前的版本并存）
    #STBCFG_MODULE_INCLUDE_EIS28_DBC = 1
    #INCLUDES := $(INCLUDES) -I$(YSENV_DIR_PROJ)/ysstb/dbc/eis28/include    
    
    #集成天柏数据广播、股票
    #CFLAGS := $(CFLAGS) -D_DVN_DBC_  
    #STBCFG_MODULE_INCLUDE_DVN_DBC = 1

    #集成影丽驰数据广播(包含股票vod)
    #CFLAGS := $(CFLAGS) -D_ENREACH_DBC_
    #STBCFG_MODULE_INCLUDE_ENREACH_DBC = 1

    #集成东太数据广播、股票
    #CFLAGS := $(CFLAGS) -D_DT_DBC_
    #STBCFG_MODULE_INCLUDE_DT_DBC = 1

    #####################################################
    #插件
    #####################################################
    # 集成华数插件
    #CFLAGS := $(CFLAGS) -D_WASU_PLUG_
    STBCFG_MODULE_INCLUDE_WASU_PLUG = 1    
    #STBCFG_MODULE_INCLUDE_WASU_CLOUD_PLUG = 1    
    STBCFG_MODULE_INCLUDE_WASU_CA_PLUG = 1    
    # 使用新的手柄规范
    #CFLAGS := $(CFLAGS) -D_WASU_PLUG_JOYSTICK_

    #####################################################
    #股票
    #####################################################
    # 集成海特荣德股票
    #CFLAGS := $(CFLAGS) -D_HTRD_STOCK_
    #STBCFG_MODULE_INCLUDE_HTRD_STOCK = 1
   
    # 集成长虹股票
    #CFLAGS := $(CFLAGS) -D_CHANGHONG_STOCK_
    #STBCFG_MODULE_INCLUDE_CHANGHONG_STOCK = 1

    #####################################################
    #VOD
    #####################################################  
    #集成华数VOD
    #CFLAGS := $(CFLAGS) -D_WASU_VOD_
    #STBCFG_MODULE_INCLUDE_WASU_VOD = 1	

    # 集成摩托罗拉VOD
    #CFLAGS := $(CFLAGS) -D_MOTOROLA_VOD_
    #STBCFG_MODULE_INCLUDE_MOTOROLA_VOD = 1

    # 集成华为VOD
    #CFLAGS := $(CFLAGS) -D_HUAWEI_VOD_
    #STBCFG_MODULE_INCLUDE_HUAWEI_VOD = 1

    # 集成同州VOD
    #CFLAGS := $(CFLAGS) -D_COSHIP_VOD_
    #STBCFG_MODULE_INCLUDE_COSHIP_VOD = 1
    
    #集成思迁VOD
    #CFLAGS := $(CFLAGS) -D_SEACHANGE_VOD_
    #STBCFG_MODULE_INCLUDE_SEACHANGE_VOD = 1	

    # 集成UTStarcomVOD
    #CFLAGS := $(CFLAGS) -D_UTSTARCOM_VOD_
    #STBCFG_MODULE_INCLUDE_UTSTARCOM_VOD = 1

    # 集成东方广视VOD
    #CFLAGS := $(CFLAGS) -D_OVT_VOD_
    #STBCFG_MODULE_INCLUDE_OVT_VOD = 1
    
    #####################################################
    #广告及其他
    #####################################################  
    #集成华视短消息系统 (需重编译 appentry,dvbtable,update,digitv)
    #CFLAGS := $(CFLAGS) -D_HS_MESSAGE_
    #STBCFG_MODULE_INCLUDE_HS_MESSAGE_ = 1

    # 集成赛科广告   
    #CFLAGS := $(CFLAGS) -D_CYCLE_AD_
    #STBCFG_MODULE_INCLUDE_CYCLE_AD_ = 1
    
    # 集成茁壮广告   
    #CFLAGS := $(CFLAGS) -D_IPANEL_AD_
    #STBCFG_MODULE_INCLUDE_IPANEL_AD_ = 1

#####################################################
#应用模块功能选择
#
#需要手动选择
#####################################################
    #是否支持umt升级
    #CFLAGS := $(CFLAGS) -D_JZ_UMT_UPGRADE_

    #是否支持开机广告
    #CFLAGS := $(CFLAGS) -D_JZ_ADVERTISING_



    #  CAS EMM 调试
    ##CFLAGS := $(CFLAGS) -DSTBCFG_CAS_EMM_DEBUG
    #机顶盒软件版本号
    ifeq "$(SoftwareVersion)" ""
    $(error you should select right SoftwareVersion!)
	endif
    YSSTB_SOFTWARE_VERSION:=$(SoftwareVersion)
    CFLAGS := $(CFLAGS) -DYSSTB_SOFTWARE_VERSION=$(SoftwareVersion)
    #机顶盒软件类型
    CFLAGS := $(CFLAGS) -DYSSTB_SOFTWARE_TYPE=0x00

    #单频道音量记忆(默认开启)
    CFLAGS := $(CFLAGS) -D_NVM_FOR_SERVICE_VOL_
    
    #32阶音量
    CFLAGS := $(CFLAGS) -D_VOLUME_32_

    #集成马赛克功能
    CFLAGS := $(CFLAGS) -D_MOSAIC_
	
    #频道号(1或3位模式)默认定义1位
    CFLAGS := $(CFLAGS) -D_CHNUM_INPUTMODE_ONE_
    
    #输入频道号未满3位按确认键换台
    #CFLAGS := $(CFLAGS) -D_CHNUMBER_AUTO_ENTER_
    
    #声道调整在dtv下即时显示和通过应用调整(打开此开关表示即时显示)
    CFLAGS := $(CFLAGS) -D_SOUNDTYPE_BANNERSHOW_

    #换台保留最后一帧
    #CFLAGS := $(CFLAGS) -D_CHCHAN_KEEP_LAST_FRAME_

    #菜单切换保留上次焦点
    #CFLAGS := $(CFLAGS) -D_MENU_KEEP_FOCUS

    # EPG和EIS共用内存,需重编译 dvbtable dbc main
    #CFLAGS := $(CFLAGS) -D_SHARED_MEM_OF_EIT_AND_DBC_

    # 集成九州电视网站
    #JZ_TVNET_SUPPORT = 1
	ifeq "$(JZ_TVNET_SUPPORT)" "1"
    CFLAGS := $(CFLAGS) -D_JZ_TVNET_SUPPORT_
	endif

    # 集成九州广告
    #CFLAGS := $(CFLAGS) -D_JIUZHOU_AD_

    #集成九州开机画面广告
    #CFLAGS := $(CFLAGS) -D_JIUZHOU_PDT_

    # 集成康佳广告
    #CFLAGS := $(CFLAGS) -D_KONKA_AD_

    # 集成创维广告
    #CFLAGS := $(CFLAGS) -D_SKYWORTH_AD_

	# 集成多伴音功能
	CFLAGS := $(CFLAGS) -D_MULTY_AUDIO_

	# 集成多Subtitle功能
	#CFLAGS := $(CFLAGS) -D_MULTY_SUBTITLE_

	# 集成节目简介功能
	#CFLAGS := $(CFLAGS) -D_SERVICE_INFORMATION_

	# 杜比音频功能
	CFLAGS := $(CFLAGS) -D_DOLBY_SUPPORT_
	STBCFG_MODULE_INCLUDE_DOLBY = 1	
	#CFLAGS := $(CFLAGS) -DDRV_JZBY_AUDIO

	#支持USB功能
	JZ_USB_SUPPORT = 1
ifeq "$(JZ_USB_SUPPORT)" "1"
	# 集成USB应用功能
	CFLAGS := $(CFLAGS) -DJZ_USB_SUPPORT
	STBCFG_MODULE_INCLUDE_USBAPP = 1	

	# 集成PVR录制播放功能
	CFLAGS := $(CFLAGS) -DJZ_PVR_SUPPORT
	STBCFG_MODULE_INCLUDE_PVR = 1
	
	# 集成PVR预约录制功能
	CFLAGS := $(CFLAGS) -DJZ_PVR_BOOK_SUPPORT
	STBCFG_MODULE_INCLUDE_PVR_BOOK = 1
	
	# 集成时移手动设置大小功能(默认关闭)
	#CFLAGS := $(CFLAGS) -DJZ_TIMESHIFT_SETBYHAND
endif

	# 支持开机无缝显示
	CFLAGS := $(CFLAGS) -DJZ_SEAMLESS_DISPLAY_SUPPORT



	#支持TUNER  选择 ATMB886X功能
ifeq "$(HardwareVersion)" "0x684"
	CFLAGS := $(CFLAGS) -DATBM886
	ATBM886X = 1
endif

