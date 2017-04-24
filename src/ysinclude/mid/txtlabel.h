

 #ifndef _TXTLABEL_H
 #define _TXTLABEL_H

#include "ctrlst.h"
#include "stberror.h"

/*
typedef struct TextLabelInfo {
	Rect_t		rcTextLabel;			//TextLabel���Ͻǵ����꣬�߶ȺͿ�ȡ�
	char			*pBody;				//����
	int			nLen;					//�ı�����������
	Size_t		szFont;				//�����С
	ColorMap_t		colorSet;				//The combination of color setting
	int			nSetting;				//�ؼ��趨
}TextLabelInfo_t;

*/

/*---------MidControlTextLabelSetCat----------*/
/*	Description:set the type information of control
                         ���ÿؼ�������Ϣ�ṹ����Ҫ�ǿؼ������ĺ���ָ�븳ֵ
	input:pointer to the control item category information
	output: void 
*/
void			MidControlTextLabelSetCat(ControlCatInfo_t *pCatInfo);
/*----------MidControlTextLabelCreate-----------*/
/*
Descrpition ��Construct TextLabel
                  ��ͷ�˷��ͽ����еĿؼ������ַ������ؼ�����������ṹ��
                   �������ɵĿؼ����뵽�ؼ�����β
Input ��pointer to the control list,the string of control item, the string length of control 
Output��pointer to the control item. if create error , return null.
*/
ControlItem_t	*MidControlTextLabelCreate(ControlList_t *pcontrolList , char *pstrFeature, int nLen);
/*	-----------MidControlTextLabelDraw----------- */
/*	Description: draw the textlabel 
                          ���� TextLabel 
	Input:pointer to the textlabel,the style of textlabel
	Output:void
*/
void			MidControlTextLabelDraw(ControlItem_t *pTextLabelItem, int nStyle);
/*	-----------MidControlTextLabelProcess----------*/

/*	Description:function processing the input message for the the cell and return  false, it dont process message
	Input:	pointer to the cell, pointer to the input and output message
	Output:	processed or not
*/
bool			MidControlTextLabelProcess (ControlItem_t *pTextLabelItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);
/*	--------------MidControlTextLabelGetFocus-------------- */
/*	Description:The subindexblock get focus, set the tag and redraw the item
	Input:		Pointer to the subindexblock
	Output:		succeed or not*/

HRESULT		MidControlTextLabelGetFocus (ControlItem_t *pTextLabelItem);
/*  ------------MidControlTextLabelLoseFocus------------*/
/*	Description:The cell lose focus, simply clear the highlighted mark with original mark
	Input:		Pointer to the cell
	Output:		operation succeed or not*/

HRESULT		MidControlTextLabelLoseFocus (ControlItem_t *pTextLabelItem);
/*	
	Descrpition : Determinate if Textlabel is accepting focus at current status
	Input : Pointer to Textlabel 
	Initial condition : Textlabel object has exist and has been enroled
	Ouptut: if permit get focus then return true.else return false.
*/
bool			MidControlTextLabelIsAcceptingFocus(ControlItem_t *pTextLabelItem);
/*	MidControlTextLabelLoadFromString--------------*/
/*	Description:load the layout setting from the string
                         ���ַ������TextLabel��ControlItem�ṹ
	Input:		Pointer to the subindexblock, pointer to the string, string length
	Output:		error code*/
HRESULT		MidControlTextLabelLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t* pControlList);
/*  -----------	MidControlTextLabelDestory--------------*/
/*	Destription:Destroy the instance of textlabel
                         ���TextLabel��ControlItem�ṹ
	Input: pointer to the textlabel
	Output:error code
*/
HRESULT		MidControlTextLabelDestroy(ControlItem_t *pTextLabelItem,ControlList_t* pControlList);
/*	-----------MidControlTextLabelCopy-----------*/
/*	Description:Copy the information of a TextLabel to a new TextLabel.It is not implemented here.
                         ���ƿؼ�
	Input:pointer to the TextLabel, pointer to the control list
	Output:pointer to the new TextLabel
*/
ControlItem_t	*MidControlTextLabelCopy(ControlItem_t *pTextLabelItem,ControlList_t* pControlList);

void MidControlDrawRoundConerRect(Rect_t rect, int color, int Radius);

/*-------------MidControlTextLabelSetText-----------*/
/*
Descrpation :Evalute a string to TextLabel control.
                   ����TextLabel����ʾ�ı�            
Input :  Point to TextLabel, string, string's length, Control list
Output : if operation success then return true else return error
*/
HRESULT MidControlTextLabelSetText(ControlItem_t *pItem, char *pText, int nLen, ControlList_t* pControlList);

#endif


