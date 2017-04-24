
#ifndef BUTTON_H__
#define BUTTON_H__

#include   "ctrlst.h"
#include   "stberror.h"

/*#include   "partitio.h"
#define	BLOCK_SIZE					512
static	partition_t					pMy_partition;
static	int						iMyPartition[2000];*/

#define	PRESS_DOWN					0x00000100
#define	PRESS_STAY					0x00000200
#define	PRESS_RELEASE				0x00000300	
#define	PRESS_FREE					0x00000400
#define	ANIMATE_PRESS				0x00000500

/*
#define	C_ButtonFocusBack					1		//The background color when the control has focus
#define	C_ButtonFocusFont					2		//the color of the text on control when it has the focus
#define	C_ButtonNormalBack				3		//The background color when without focus
#define	C_ButtonNormalFont				4		//the color of the text when without focus

*/




#define	WHITE_SHADOW					3
#define	BLACK_SHADOW					2	
#define	FOCUS_SIGN						5	 
#define	FOCUS_SHADOW					4	

#if 0
typedef struct ButtonInfo {
	//Generic
	int				nKeyCode;		//按钮对应的键码
	Rect_t			rcBotton;		//按钮左上角、右下角的坐标。
	UI32				nSetting;		//按钮初始设定
	UI32				nMsgCode;		//按键对应的消息代码
	//Button specific
	char				*pLabel;		//显示的字符串
	int				nAlignment;		//字符串在按钮上的位置CENTER, LEFT, RIGHT
	int				nFontsize;		//字体大小
	ColorMap_t			colorSet;		//The combination of color setting
}ButtonInfo_t;
#endif

/*typedef struct ButtonFeature {
	MidLineStyle_t stFocusLine;
	MidLineStyle_t stUpShadowLine;
	MidLineStyle_t stDownShadowLine;
}ButtonFeature_t;*/



ControlItem_t	*MidControlButtonCreate (ControlList_t *pcontrolList, char *pstrFeature, int nLen);
bool			MidControlButtonProcess(ControlItem_t * pButtonItem,MessageInfo_t * pMsgIn,MessageInfo_t * pMsgOut,ControlList_t *pcontrolList );

void			MidControlButtonSetStatus(ControlItem_t * pButtonItem,bool bFocus,bool bPress); 
HRESULT		MidControlButtonGetFocus(ControlItem_t * pButtonItem); 
HRESULT		MidControlButtonLoseFocus(ControlItem_t * pButtonItem);
void			MidControlButtonDraw(ControlItem_t * pButtonItem, int nStyle);

void			MidControlButtonDrawNormal(void * pItem,int nStyle);		//draw button with normal status(without focus)
void			MidControlButtonDrawWithFocus(ControlItem_t * pButtonItem);		//draw button with focus
void			MidControlButtonDrawPressDown(ControlItem_t * pButtonItem);		//draw button with pressed status

void			MidControlButtonSetCat(ControlCatInfo_t *pCatInfo);
HRESULT		MidControlButtonDestroy(ControlItem_t *pRadioGroupItem,ControlList_t* pControlList);
HRESULT		MidControlButtonLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t *pControlList);
ControlItem_t	*MidControlButtonCopy(ControlItem_t *pButtonItem,ControlList_t* pControlList );

HRESULT MidControlButtonSetCallback(struct ControlItem *this, void *pFunc, int nType);

#endif//BUTTON_H__  
