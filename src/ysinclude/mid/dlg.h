/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : Dlg.h

    Description : 对话框功能

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _DLG_H_
#define _DLG_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define DLG_PART_SIZE 					(1024 * 10)

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum
{
	DLG_PART_TITLE_BACK = 13,
	DLG_PART_TITLE = 10,
	DLG_PART_CONTENT = 11,
	DLG_PART_BUTTON_BANNER = 12,
	DLG_PART_BUTTON_CONFIRM_CANCEL = 1,
	DLG_PART_BUTTON_YES = 2,
	DLG_PART_BUTTON_NO = 3
}Dlg_DlgPart_NameSpace;

typedef enum DlgSizeConfig_e
{
	DLG_TITLE_HEIGHT = 346,
	DLG_TITLE_BUTTON_BANNER_HEIGHT = 113
}DlgSizeConfig_t;
 
typedef enum DlgTitleShowStyle_e
{
	DLG_TITLE_SHOW_MIDDLE =0,
	DLG_TITLE_SHOW_LEFT=1,
	DLG_TITLE_SHOW_RIGHT = 2,
	DLG_TITLE_SHOW_NONE = 3	//表示没有标题栏
}DlgTitleShowStyle_t;

typedef enum DlgContentShowStyle_e
{
	DLG_CONTENT_SHOW_UP_LEFT = 0,
	DLG_CONTENT_SHOW_UP_MIDDLE = 1,
	DLG_CONGENT_SHOW_UP_RIGHT = 2,
	DLG_CONTENT_SHOW_MIDDLE_LEFT = 3,
	DLG_CONTENT_SHOW_MIDDLE_MIDDLE = 4,
	DLG_CONTENT_SHOW_MIDDLE_RIGHT = 5,
	DLG_CONTENT_SHOW_DOWN_LEFT = 6,
	DLG_CONTENT_SHOW_DOWN_MIDDLE  = 7,
	DLG_CONTENT_SHOW_DOWN_RIGHT = 8
}DlgContentShowStyle_t;

typedef enum DlgButtonType
{
	DLG_BUTTON_NONE = 0,
	DLG_BUTTON_CONFIRM = 1,
	DLG_BUTTON_YES_NO = 2,
	DLG_BUTTON_YES_NO_CANCEL = 3
}DlgButtonType_e;

typedef enum DlgButtonShowStyle_e
{
	DLG_BUTTON_SHOW_MIDDLE =0,
	DLG_BUTTON_SHOW_LEFT=1,
	DLG_BUTTON_SHOW_RIGHT = 2
}DlgButtonShowStyle_t;

typedef enum DlgProcessType
{
	DLG_NONE_PROCESS,						//不需要进行处理
	DLG_CHANGE_CHNL_PROCESS,				//需要进行切台操作
	DLG_CHANGE_TO_TV_PROCESS,				//需要进行切换到电视模式的操作
	DLG_CHANGE_TO_RADIO_PROCESS,			//需要进行切换到广播模式的操作
	DLG_PROCESS_COUNT
}DlgProcessType_t;

typedef struct DlgButtonText
{
	unsigned char	*pConfirmCancelText;	//在ucButtonType 为DLG_BUTTON_CONFIRM和DLG_BUTTON_YES_NO_CANCEL时使用
	unsigned char	*pYesText;				//在ucButtonType 为DLG_BUTTON_YES_NO和DLG_BUTTON_YES_NO_CANCEL时使用
	unsigned char	*pNoText;				//在ucButtonType 为DLG_BUTTON_YES_NO和DLG_BUTTON_YES_NO_CANCEL时使用
}DlgButtonText_t;

typedef struct DlgButtonStyle
{
	unsigned short	ButtonWidth;
	unsigned short	ButtonHeight;
	unsigned short	ButtonBottomMargin;
	unsigned char	ButtonTextAlign;		//暂不支持
	unsigned char	ButtonTextColor;		//暂不支持
	unsigned char	ButtonTextFocusColor;	//暂不支持
	unsigned char	ButtonBackColor;		//暂不支持
	unsigned char	ButtonBackFocusColor;	//暂不支持
}DlgButtonStyle_t;

typedef struct DlgInfo
{
	unsigned char 	*pTitle;				//标题字符串
	unsigned char 	*pContent;				//内容字符串
	DlgButtonText_t	ButtonTxt;				//按钮文本
	DlgButtonStyle_t ButtonStyle;			//按钮风格
	unsigned char	ucTitleStyle;			//标题显示方式，取值见DlgTitleShowStyle_e类型定义******暂不支持
	unsigned char	ucContentStyle;			//内容显示方式，取值见DlgContentShowStyle_e类型定义******暂不支持
	DlgButtonType_e	ucButtonType;			//按钮类型，取值见DlgButtonType_e类型定义
	unsigned char	ucShowTime;				//对话框自动消失时间，以秒为单位，取0为不自动消失
	Rect_t			*pDlgRect;				//对话框的区域
	DRVOS_MsgQueue_t *pMsgQueueofAppCreateDlg;//发起对话框的应用的消息队列
	unsigned int 	DlgProcessType;			//对话框显示后需要进行的处理
	unsigned int	Priority;				//对话框优先级(暂时不支持)
}DlgInfo_t;

/*----------------------------------------------------------------------------
 *	Variable
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Function
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgTitle
  Description     : 设置对话框标题内容
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetDlgTitle(DlgInfo_t *pDialogInfo, Rect_t *DialogRect);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgContent
  Description     : 设置对话框文本
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetDlgContent(DlgInfo_t *pDialogInfo,Rect_t *DialogRect);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgButton
  Description     : 设置对话框按钮
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetDlgButton(DlgInfo_t *pDialogInfo, Rect_t *DialogRect);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_Init
  Description     : 对话框初始化
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_Init(DlgInfo_t* pDialogInfo,DRVOS_MsgQueue_t *CreaterMsgQueue,Rect_t *DialogRect,char *TitleStr,char *ContentStr,
				DlgButtonType_e	 ButtonNum,char *pConfirmCancelText,char *pYesText,char *pNoText,char AutoExitTime);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetTitle
  Description     : 设置对话框标题
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetTitle(DlgInfo_t *pDialogInfo,unsigned char TitleStyle,unsigned char *TitleStr);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetContent
  Description     : 设置对话框文本内容
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetContent(DlgInfo_t* pDialogInfo, unsigned char *pContentStr);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetButton
  Description     : 设置对话框按钮
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 注意，button的字符串应该与所设置的button个数相对应
*****************************************************************************/ 
void Dlg_SetButton(DlgInfo_t* pDialogInfo, DlgButtonType_e ButtonNum,unsigned char *pConfirmButtonTxt,unsigned char *pYesButtonTxt,
					unsigned char *pNoButtonTxt,bool UseDefaultSizeAndPosition, unsigned short ButtonWidth, unsigned short ButtonHeight,
					unsigned char ButtonTxtAlign, unsigned short ButtonBottomMargin);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgAutoExitTime
  Description     : 设置对话框自动退出时间
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetDlgAutoExitTime(DlgInfo_t* pDialogInfo,unsigned char AutoExitTiime);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgProcessType
  Description     : 设置对话框处理类型
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void Dlg_SetDlgProcessType(DlgInfo_t* pDialogInfo,unsigned int ProcessType);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_Show
  Description     : 对话框显示
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int Dlg_Show(DlgInfo_t *pDialogInfo, APP_AppInfo_t *CurAppInfo,DRVOS_Partition_t *MemPart,DRVOS_MsgQueue_t *MsgQueue,BmpNameFun orgBmpSetFunc);
#endif

