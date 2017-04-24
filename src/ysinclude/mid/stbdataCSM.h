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
  Description     : ��ȡ����TV��ĿCode
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
  Description     : ��ȡ����Radio��ĿCode
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
  Description     : ��ȡ����Nvod��ĿCode
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
  Description     : ��ȡ�������Code
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
  Description     : ��ȡ��ʾ��������Code
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
  Description     : ��ȡ�������˵�Code
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
  Description     : ��ȡ����Dbc�˵�Code
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
  Description     : ��ȡ����Nvod�˵�Code
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
  Description     : ��ȡ����EPG�˵�Code
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
  Description     : ��ȡ�����ƱCode
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
  Description     : ��ȡ�����ʼ��˵�Code
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
  Description     : ��ȡ����Game�˵�Code
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
  Description     : ��ȡ����Code
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
  Description     : ��ȡ����Mosaic�˵�Code
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
  Description     : ��ȡ����Vollet�˵�Code
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
  Description     : ��ȡ���������˵�Code
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
  Description     : ����Code
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void SICODE_SendCode(SICODEInfo_t * SICodeInfo);

#endif

