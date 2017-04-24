/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : factcfg.h

    Description : 出厂参数设置
 
    Others:      
    
    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSFACTCFG_H
#define _YSFACTCFG_H

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#include "stbtype.h"

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif                        

/*!!!注意，此数据结构修改需与bios对应，并注意版本问题!!!*/
typedef struct FACTORY_Config_s
{
	unsigned int		CfgMark;	/* 0xbeebebee*/
    unsigned int    	HWVersion;	/*硬件版本*/
    unsigned char     	CAType;		/*CA类型*/
    unsigned char     	TunerType;	/*高频头类型 -C -S -T*/
    unsigned char     	VoutType;	/*分量/S端子*/
    unsigned char     	DencMode;	/*N/P制*/
    unsigned int    	BoxSerial;	/*机器编号*/
    unsigned int    	ZipCode; 	/*用邮政编码作为区域码*/

    unsigned int		FreAuto;	/*自动搜索的缺省频点*/
    unsigned short		SymAuto;	/*自动搜索的缺省符号率*/
    unsigned short		QamAuto;	/*自动搜索的缺省调制方式*/

    unsigned int    	FreUpgrade;	/*升级数据的缺省频点*/
    unsigned short		SymUpgrade;	/*升级数据的缺省符号率*/
    unsigned short		QamUpgrade;	/*升级数据的缺省调制方式*/
    unsigned short		UMT_pid;	/*升级数据的缺省UMT Pid*/
    unsigned short		UMT_tableid;/*升级数据的缺省UMT TableId*/

    unsigned char      	TVSoundChannel;/*电视节目默认声道*/
    unsigned char      	RadioSoundChannel;/*广播节目默认声道*/
    unsigned char      	DisplayProportion;/*显示比例*/
    unsigned char      	BackgroundTransparence;/*背景透明度*/
    unsigned int		FreHand;	/*手动搜索缺省频点*/
    unsigned short		SymHand;	/*手动搜索缺省符号率*/
    unsigned short		QamHand;	/*手动搜索缺省调制方式*/
    unsigned short		SymFull; 	/*全频搜索缺省符号率*/
    unsigned short		QamFull;	/*全频搜索缺省调制方式*/
    unsigned char      	ChannelSearchMode; /*搜索模式*/
    unsigned char      	UpgradeTag; /*升级提示标志*/
    unsigned char      	NetworkSoftWareVersion; /*网上软件版本号*/
    unsigned char      	ucApplicationDataVersion;/*应用数据版本号*/
	
    /*已经使用60个字节*/
    unsigned char 		Reserved[36];
    unsigned int 		ChkckSum;
	/*TOTAL 100bytes*/
}FACTORY_Config_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Init
  Description     : 从E2P中读取FactCfg参数
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Init(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_NVMSetData
  Description     : 保存FactCfg参数
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_NVMSetData(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_GetFactInitStatus
  Description     : 获取FactCfg初始化状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Fact_GetFactInitStatus(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_VoutType
  Description     : 获取FactCfg中的输出模式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char Fact_Get_VoutType(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_VoutType
  Description     : 设置FactCfg中的输出模式
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_VoutType(unsigned char Type);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_BoxSerial
  Description     : 获取FactCfg中的机顶盒序列号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_BoxSerial(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_BoxSerial
  Description     : 设置FactCfg中的机顶盒序列号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_BoxSerial(unsigned int Serial);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_ZipCode
  Description     : 获取FactCfg中的区域码
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_ZipCode(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_ZipCode
  Description     : 设置FactCfg中的区域码
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_ZipCode(unsigned int zip);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_AutoSearchTS
  Description     : 获取FactCfg中的自动搜索频点信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_AutoSearchTS(unsigned int *pFre,unsigned short *pSym,unsigned short *pQam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_AutoSearchTS
  Description     : 设置FactCfg中的自动搜索频点信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_AutoSearchTS(unsigned int Fre,unsigned short Sym,unsigned short Qam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_UpgradeTS
  Description     : 获取FactCfg中的升级频点信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_UpgradeTS(unsigned int *pFre,unsigned short *pSym,unsigned short *pQam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_UpgradeTS
  Description     : 设置FactCfg中的升级频点信息
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_UpgradeTS(unsigned int Fre,unsigned short Sym,unsigned short Qam);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_UMTID
  Description     : 获取FactCfg中的升级UMTPid
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Get_UMTID(unsigned short *pPid,unsigned short *pTableid);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Set_UMTID
  Description     : 设置FactCfg中的升级UMTPid
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Fact_Set_UMTID(unsigned short Pid,unsigned short Tableid);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_HWVersion
  Description     : 获取FactCfg中的硬件版本号
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned int Fact_Get_HWVersion(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_GetVoutStatus
  Description     : 获取分量输出的状态
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL Fact_GetVoutStatus(void);

/*****************************************************************************
  Date&Time       : 2011-11-18
  Author          :   xyan
  Function Name   : Fact_Get_CAType
  Description     : 获取FactCfg中的CA类型
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char Fact_Get_CAType(void);

#ifdef __cplusplus
}
#endif                        

#endif 

