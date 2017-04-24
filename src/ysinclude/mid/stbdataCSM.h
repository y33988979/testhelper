/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataCSM.h

    Description : 

    Others:      

    History:    1. llh creat 2004-3-30


*******************************************************************************/
#ifndef	__YSSTBDATACSM_
#define	__YSSTBDATACSM_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
typedef struct SICODEInfo
{
	unsigned char ucCodeStart;
	SHORT2BYTE usCodeLength;
	WORD2BYTE NetID;           
	WORD2BYTE TSID;                  
	WORD2BYTE ServiceID;
	SHORT2BYTE CodeCS;                   
	unsigned char ucCodeEnd;   
}SICODEInfo_t;


/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetTVChannelCode
  Description     : 获取进入TV节目Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetTVChannelCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetRadioChannelCode
  Description     : 获取进入Radio节目Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetRadioChannelCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetNvodServiceCode
  Description     : 获取进入Nvod节目Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetNvodServiceCode(STBDATAServiceInfo_t *pSTBServiceInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetPowerCode
  Description     : 获取进入待机Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetPowerCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetBootLogoCode
  Description     : 获取显示开机画面Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetBootLogoCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetMainMenuCode
  Description     : 获取进入主菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetMainMenuCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetDBCMainMenuCode
  Description     : 获取进入Dbc菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetDBCMainMenuCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetNvodMainMenuCode
  Description     : 获取进入Nvod菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetNvodMainMenuCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetEPGCode
  Description     : 获取进入EPG菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetEPGCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetStockCode
  Description     : 获取进入股票Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetStockCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetEMailCode
  Description     : 获取进入邮件菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetEMailCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetGameCode
  Description     : 获取进入Game菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetGameCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetStartImageCode
  Description     : 获取开机Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetStartImageCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetMosaicCode
  Description     : 获取进入Mosaic菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetMosaicCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetVolletCode
  Description     : 获取进入Vollet菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetVolletCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_GetUpgradeCode
  Description     : 获取进入升级菜单Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
SICODEInfo_t *SICODE_GetUpgradeCode(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :    llh
  Function Name   : SICODE_SendCode
  Description     : 发送Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SICODE_SendCode(SICODEInfo_t * SICodeInfo);

#endif

