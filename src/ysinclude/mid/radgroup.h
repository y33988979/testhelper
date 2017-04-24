
#ifndef _RADGROUP_H_
#define _RADGROUP_H_

#include	"ctrlst.h"
#include    "stberror.h"




/* 
typedef struct RadioGroupInfo {
	Rect_t		rcRadioGroup;			//�ֱ�Ϊ���Ͻǵ����꣬�߶ȺͿ�ȡ�
	char			*pTitle;				//����
	int			nFontsize;				//�����С
	ColorMap_t		colorSet;				//The combination of color setting
	int			nRadio;				//Radio��Ŀ
	int			iFocus;				//����Radio���
	int			iCheck;				//ѡ��Radio���
	UI32			nSetting;				//RadioGroup��ʼ�趨
}RadioGroupInfo_t;
*/

typedef struct RadioGroupFeature {
	ControlItem_t	*pFocus;				//��ǰӵ�н����Radio��ָ��
	ControlItem_t	*pCheck;				//��ǰ��ѡ����Radio��ָ��
	int				nRadio;					//Radio��Ŀ
	ControlItem_t	*pHead;				//������ָ��		
	/*ControlItem_t	*pCurrent;			//��ǰ������ָ��	*/
	ControlItem_t	*pEnd;				//����βָ��		

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


