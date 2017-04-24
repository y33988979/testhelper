/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : Dlg.h

    Description : �Ի�����

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
	DLG_TITLE_SHOW_NONE = 3	//��ʾû�б�����
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
	DLG_NONE_PROCESS,						//����Ҫ���д���
	DLG_CHANGE_CHNL_PROCESS,				//��Ҫ������̨����
	DLG_CHANGE_TO_TV_PROCESS,				//��Ҫ�����л�������ģʽ�Ĳ���
	DLG_CHANGE_TO_RADIO_PROCESS,			//��Ҫ�����л����㲥ģʽ�Ĳ���
	DLG_PROCESS_COUNT
}DlgProcessType_t;

typedef struct DlgButtonText
{
	unsigned char	*pConfirmCancelText;	//��ucButtonType ΪDLG_BUTTON_CONFIRM��DLG_BUTTON_YES_NO_CANCELʱʹ��
	unsigned char	*pYesText;				//��ucButtonType ΪDLG_BUTTON_YES_NO��DLG_BUTTON_YES_NO_CANCELʱʹ��
	unsigned char	*pNoText;				//��ucButtonType ΪDLG_BUTTON_YES_NO��DLG_BUTTON_YES_NO_CANCELʱʹ��
}DlgButtonText_t;

typedef struct DlgButtonStyle
{
	unsigned short	ButtonWidth;
	unsigned short	ButtonHeight;
	unsigned short	ButtonBottomMargin;
	unsigned char	ButtonTextAlign;		//�ݲ�֧��
	unsigned char	ButtonTextColor;		//�ݲ�֧��
	unsigned char	ButtonTextFocusColor;	//�ݲ�֧��
	unsigned char	ButtonBackColor;		//�ݲ�֧��
	unsigned char	ButtonBackFocusColor;	//�ݲ�֧��
}DlgButtonStyle_t;

typedef struct DlgInfo
{
	unsigned char 	*pTitle;				//�����ַ���
	unsigned char 	*pContent;				//�����ַ���
	DlgButtonText_t	ButtonTxt;				//��ť�ı�
	DlgButtonStyle_t ButtonStyle;			//��ť���
	unsigned char	ucTitleStyle;			//������ʾ��ʽ��ȡֵ��DlgTitleShowStyle_e���Ͷ���******�ݲ�֧��
	unsigned char	ucContentStyle;			//������ʾ��ʽ��ȡֵ��DlgContentShowStyle_e���Ͷ���******�ݲ�֧��
	DlgButtonType_e	ucButtonType;			//��ť���ͣ�ȡֵ��DlgButtonType_e���Ͷ���
	unsigned char	ucShowTime;				//�Ի����Զ���ʧʱ�䣬����Ϊ��λ��ȡ0Ϊ���Զ���ʧ
	Rect_t			*pDlgRect;				//�Ի��������
	DRVOS_MsgQueue_t *pMsgQueueofAppCreateDlg;//����Ի����Ӧ�õ���Ϣ����
	unsigned int 	DlgProcessType;			//�Ի�����ʾ����Ҫ���еĴ���
	unsigned int	Priority;				//�Ի������ȼ�(��ʱ��֧��)
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
  Description     : ���öԻ����������
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
  Description     : ���öԻ����ı�
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
  Description     : ���öԻ���ť
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
  Description     : �Ի����ʼ��
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
  Description     : ���öԻ������
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
  Description     : ���öԻ����ı�����
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
  Description     : ���öԻ���ť
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ע�⣬button���ַ���Ӧ���������õ�button�������Ӧ
*****************************************************************************/ 
void Dlg_SetButton(DlgInfo_t* pDialogInfo, DlgButtonType_e ButtonNum,unsigned char *pConfirmButtonTxt,unsigned char *pYesButtonTxt,
					unsigned char *pNoButtonTxt,bool UseDefaultSizeAndPosition, unsigned short ButtonWidth, unsigned short ButtonHeight,
					unsigned char ButtonTxtAlign, unsigned short ButtonBottomMargin);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : Dlg_SetDlgAutoExitTime
  Description     : ���öԻ����Զ��˳�ʱ��
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
  Description     : ���öԻ���������
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
  Description     : �Ի�����ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int Dlg_Show(DlgInfo_t *pDialogInfo, APP_AppInfo_t *CurAppInfo,DRVOS_Partition_t *MemPart,DRVOS_MsgQueue_t *MsgQueue,BmpNameFun orgBmpSetFunc);
#endif

