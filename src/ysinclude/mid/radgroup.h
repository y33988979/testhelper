
#ifndef _RADGROUP_H_
#define _RADGROUP_H_

#include	"ctrlst.h"
#include    "stberror.h"




/* 
typedef struct RadioGroupInfo {
	Rect_t		rcRadioGroup;			//分别为左上角的坐标，高度和宽度。
	char			*pTitle;				//标题
	int			nFontsize;				//字体大小
	ColorMap_t		colorSet;				//The combination of color setting
	int			nRadio;				//Radio数目
	int			iFocus;				//焦点Radio序号
	int			iCheck;				//选中Radio序号
	UI32			nSetting;				//RadioGroup初始设定
}RadioGroupInfo_t;
*/

typedef struct RadioGroupFeature {
	ControlItem_t	*pFocus;				//当前拥有焦点的Radio的指针
	ControlItem_t	*pCheck;				//当前被选定的Radio的指针
	int				nRadio;					//Radio数目
	ControlItem_t	*pHead;				//链表首指针		
	/*ControlItem_t	*pCurrent;			//当前数据项指针	*/
	ControlItem_t	*pEnd;				//链表尾指针		

}RadioGroupFeature_t;

//Create
ControlItem_t	*MidControlRadioGroupCreate(ControlList_t *pcontrolList ,
							    char *pstrFeature, int nLen);
//CheckItem
void			MidControlRadioGroupSetCheckItem(ControlItem_t *pRadioGroupItem,
								   ControlItem_t *pRadioItem);
//GetFocus
HRESULT		MidControlRadioGroupGetFocus(ControlItem_t *pRadioGroupItem);
//LoseFocus
HRESULT		MidControlRadioGroupLoseFocus(ControlItem_t *pRadioGroupItem);
//Process
bool			MidControlRadioGroupProcess(ControlItem_t *pRadioGroupItem,  
					MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList) ;
//Draw
void			MidControlRadioGroupDraw(ControlItem_t *pRadioGroupItem , int nStyle );
//FocusDown
ControlItem_t	*MidControlRadioGroupFocusDown(ControlItem_t *pRadioGroupItem);
//FocusUp
ControlItem_t	*MidControlRadioGroupFocusUp(ControlItem_t *pRadioGroupItem);
//DrawFocus
void			MidControlRadioGroupDrawFocus(ControlItem_t *pRadioGroupItem);
//DrawClearFocus
void			MidControlRadioGroupDrawClearFocus(ControlItem_t *pRadioGroupItem);
//DrawNormal
void			MidControlRadioGroupDrawNormal(ControlItem_t *pRadioGroupItem);

//SetCat
void			MidControlRadioGroupSetCat(ControlCatInfo_t *pCatInfo);
//Destroy
HRESULT		MidControlRadioGroupDestroy(ControlItem_t *pRadioGroupItem,ControlList_t* pControlList);
//LoadFromString
HRESULT		MidControlRadioGroupLoadFromString(ControlItem_t *pItem, 
								     char *pstrItem, int nLen,ControlList_t* pControlList);
//Copy
ControlItem_t	*MidControlRadioGroupCopy(ControlItem_t *pRadioGroupItem,ControlList_t* pControlList);
//RestoreSetting
void			MidControlRadioGroupRestoreSetting(ControlItem_t *pRadioGroupItem);
 

HRESULT MidControlRadioGroupAddItem(ControlItem_t*pRadioGroup,ControlItem_t *pRadio);
HRESULT MidControlRadioGroupSetCallback(struct ControlItem *this, void *pFunc, int nType);
HRESULT MidControlRadioGroupSetText(ControlItem_t *pItem, char *pNull, int siCurrentID, ControlList_t* pControlList);
int MidControlRadioGroupGetChecked(ControlItem_t *pItem);

#endif


