/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : appregion.h

    Description : 应用显示Region管理

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef APPREGION_H__
#define APPREGION_H__

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_AppRegionCreate
  Description     : AppRegion创建
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int APPREG_AppRegionCreate(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_AppRegionShow
  Description     : AppRegion显示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APPREG_AppRegionShow(OSD_Palette_p Palette);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_AppRegionShow_Game
  Description     : 游戏Region显示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APPREG_AppRegionShow_Game(OSD_Palette_p Palette);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_AppRegionHide
  Description     : 隐藏AppRegion
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APPREG_AppRegionHide(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_AppRegionShow16
  Description     : 将AppRegion设置为当前Region
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APPREG_AppRegionShow16(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : APPREG_CursorRegionCreate
  Description     : Cursor层Region创建
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void APPREG_CursorRegionCreate(void);

#endif 


