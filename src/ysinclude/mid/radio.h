
#ifndef _RADIO_H_
#define _RADIO_H_

#include	"ctrlst.h"
#include    "stberror.h"
/*
#define	C_RadioFocusBack					1		//The background color when the control has focus
#define	C_RadioFocusFont					2		//the color of the text on control when it has the focus
#define	C_RadioNormalBack					3		//The background color when without focus
#define	C_RadioNormalFont					4
*/
#define	C_RADIOWHITE					5
#define	C_RADIOBLACK					6

#define	CENTER					0
#define	LEFT						1
#define	RIGHT						2


#define	X_DISTANCE					10
#define	Y_DISTANCE					3

#define	X_SIDE					10
#define	Y_SIDE					15


ControlItem_t	*MidControlRadioCreate (ControlList_t *pControlList , char *pstrFeature, int nLen);
HRESULT		MidControlRadioGetFocus(ControlItem_t *pRadioItem);
HRESULT		MidControlRadioLoseFocus(ControlItem_t *pRadioItem);
void			MidControlRadioSetCheck(ControlItem_t *pRadioItem);
void			MidControlRadioLoseCheck(ControlItem_t *pRadioItem);
bool			MidControlRadioProcess(ControlItem_t *pRadioItem, MessageInfo_t *pMsgIn,  MessageInfo_t *pMsgOut,ControlList_t *pControlList);//modify by    
void			MidControlRadioDraw(ControlItem_t *pRadioItem,int nStyle);

void			MidControlRadioDrawWithFocus(ControlItem_t *pRadioItem);		//draw radio with focus
void			MidControlRadioDrawWithCheck(ControlItem_t *pRadioItem);		//draw radio with check
void			MidControlRadioDrawWithCheciNormalkProcess(ControlItem_t *pRadioItem);//draw radio with checked process

void			MidControlRadioSetCat(ControlCatInfo_t *pCatInfo);
HRESULT		MidControlRadioDestroy(ControlItem_t *pRadioItem,ControlList_t* pControlList );
HRESULT		MidControlRadioLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t* pControlList);
ControlItem_t	*MidControlRadioCopy(ControlItem_t *pRadioItem,ControlList_t* pControlList);


/*-----------MidControlRadioRestoreSetting-------------*/
/*	说明：恢复控件原始设定
	输入：控件指针
	输出：无
*/
void MidControlRadioRestoreSetting(ControlItem_t * pRadioItem);


/*------------MidControlRadioDrawNormal--------------*/
/*	说明：绘制正常状态（没有焦点）的控件。
	输入：控件指针
	输出：无
*/
//void MidControlRadioDrawNormal(ControlItem_t *pRadioItem);
void MidControlRadioDrawNormal(void *pItem,int nStyle);			//draw radio with normal status(without focus)

/*------------MidControlRadioDrawFocus--------------*/
/*	说明：绘制控件的焦点标记。
	输入：控件指针
	输出：无
*/
void MidControlRadioDrawFocus(ControlItem_t *pRadioItem);

/*------------MidControlRadioDrawClearFocus--------------*/
/*	说明：清除控件的焦点标记。
	输入：控件指针
	输出：无
*/
void MidControlRadioDrawClearFocus(ControlItem_t *pRadioItem);

/*------------MidControlRadioDrawCheck--------------*/
/*	说明：绘制控件的选中标记。
	输入：控件指针
	输出：无
*/
void MidControlRadioDrawCheck(ControlItem_t *pRadioItem);

/*------------MidControlRadioDrawClearCheck--------------*/
/*	说明：清除控件的选中标记。
	输入：控件指针
	输出：无
*/
void MidControlRadioDrawClearCheck(ControlItem_t *pRadioItem);

HRESULT MidControlRadioSetCallback(struct ControlItem *this, void *pFunc, int nType);

#endif

