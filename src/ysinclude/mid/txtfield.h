
#ifndef _TEXTFIELD_H_
#define _TEXTFIELD_H_


#include "ctrlst.h"
#include "stberror.h"

#define	BUFFER_STEP		32
#define	FIELT_X_SIDE		30
#define	FIELT_Y_SIDE		8

typedef struct TextFieldFeature {
	int		iCurPos;						//Position of current cursor
	char *OriginalText;
	int Width;
	int High;
	int FocusLineColor;
	int InputDelayMs;
	char **InputMap;
} TextFieldFeature_t;

/*---------MidControlTextFieldSetCat----------*/
/*	Description:set the type information of control
                        ���ÿؼ�������Ϣ�ṹ����Ҫ�ǿؼ������ĺ���ָ�븳ֵ
	input:pointer to the control item category information
	output: void 
*/
void MidControlTextFieldSetCat(ControlCatInfo_t *pCatInfo);

/*----------MidControlTextFieldCreate-----------*/
/*	Description:Create a instance of Control,insert the information of TextField into the control list
                        ��ͷ�˷��ͽ����еĿؼ������ַ������ؼ�����������ṹ��
                   �������ɵĿؼ����뵽�ؼ�����β
	Input: pointer to the control list,the string of control item, the string length of control 
	Output:pointer to the control item. if create error , return null.
*/
ControlItem_t * MidControlTextFieldCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);

/*----------MidControlTextFieldLoadFromString-------------*/
/*	Description:load the layout setting from the string
                         ���ַ������TextLabel��ControlItem�ṹ
	Input:		Pointer to the subindexblock, pointer to the string, string length
	Output:		error code*/
HRESULT MidControlTextFieldLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t* pControlList);

/*-----------MidControlTextFieldDestroy------------*/
/*	Destription:Destroy the instance of Textfield
                         ���TextField��ControlItem�ṹ
	Input: pointer to the Textfield
	Output:error code
*/	
HRESULT MidControlTextFieldDestroy(ControlItem_t *pTextFieldItem,ControlList_t* pControlList);

/*--------------MidControlTextFieldCopy-----------------*/
/*	Description:Copy the information of a Textfiled to a new Textfiled.It is not implemented here.
                         ���ƿؼ�
	Input:pointer to the Textfiled,pointer to the control list
	Output:pointer to the new Textfiled
*/
ControlItem_t *MidControlTextFieldCopy(ControlItem_t *pTextFieldItem,ControlList_t* pControlList);

/*-----------MidControlTextFieldMoveTo--------------*/
/*	Description��Set new position of cursor ,must insure new position not over border of pBody ;
                            ���ù�굽�µ�λ�ã�����ʾ
	Input��pointer to the Textfiled,new position value ;
	Output��If success then return true��else return false ;
	*/
bool MidControlTextFieldMoveTo(ControlItem_t *pTextFieldItem, int  iNewPos);

/*--------------MidControlTextFieldMove-------------*/
/*	
	Description : Move nStep step of cursor's position to a new position,neglect those which over border
	                   ���ӵ�ǰλ���ƶ�nStep��������ʾ
	Input��pointer to the Textfiled��Distance value for moving
	Ouput��Void
*/
void MidControlTextFieldMove(ControlItem_t *pTextFieldItem, int nStep);

/*----------------MidControlTextFieldAddChar---------------*/
/*	
	Description:  Added a new char behand current position ;
	                    �ڹ�굱ǰλ�ú�����ַ�������ʾ
	Input : pointer to the Textfiled,new char of Input,which only single char is valid ;
	Output��If success return true else return false ;
*/
bool MidControlTextFieldAddChar(ControlItem_t *pTextFieldItem, char cNew,ControlList_t* pControlList); 
/*-----------------MidControlTextFieldDeleteChar---------------*/
/*	Description��Delete a char at current position
                           ɾ����ǰ��괦���ַ�
	Input ��Pointer to a Textfield
	Output��void
*/
void MidControlTextFieldDeleteChar(ControlItem_t *pTextFieldItem);

/*------------------MidControlTextFieldGetFocus------------------*/
/*	Description:Textfield get focus
                         ��ý���
	Input:pointer to the Textfield
	Output:error code
*/
HRESULT MidControlTextFieldGetFocus(ControlItem_t *pTextFieldItem);

/*-----------------MidControlTextFieldLoseFocus-----------------*/
/*	Description:Textfield lose focus
                         ʧȥ����
	Input:pointer to the Textfield
	Output:error code
*/
HRESULT MidControlTextFieldLoseFocus(ControlItem_t *pTextFieldItem);

/*--------------MidControlTextFieldProcess---------------*/
/*	Description:process button press message
                        ��Ϣ����
	Input:pointer to the control item,pointer to the input and output message
	Output:process or not
*/
bool MidControlTextFieldProcess(ControlItem_t *pTextFieldItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);

/*----------MidControlTextFieldDraw------------*/
/*	Description: draw the TextField 
                        ����TextField
	Input:pointer to the TextField ,the style of TextField 
	Output:void
*/
void MidControlTextFieldDraw(ControlItem_t *pTextFieldItem, int nStyle);

/*-------------MidControlTextFieldDrawFrame-------------*/
/*	Descrpation : Display TextField's basic frame according special model
	Input ��Control pointer��Display model
	Output��void
	*/
void MidControlTextFieldDrawFrame(ControlItem_t *pTextFieldItem, int nStyle);

/*-------------MidControlTextFieldDrawPassword-------------*/
/*	Descrpation��No definition
	Input :
	Output��
	*/
void	MidControlTextFieldDrawPassword(ControlItem_t *pTextFieldItem);

/*-------------MidControlTextFieldDrawText-------------*/
/*	Descrpation��No definition
	Input :
	Output��
	*/
void 	MidControlTextFieldDrawText(ControlItem_t *pTextFieldItem);
/*-------------MidControlTextFieldDrawFocusFrame-------------*/
/*	Descrpation��No definition
	Input :
	Output��
	*/
void MidControlTextFieldDrawFocusFrame(ControlItem_t *pTextFieldItem);

 /*-------------MidControlTextFieldDrawNormalFrame-------------*/
/*	Descrpation��No definition
	Input :
	Output��
	*/
void MidControlTextFieldDrawNormalFrame(ControlItem_t *pTextFieldItem) ;

/*-------------MidControlTextFieldIsAcceptingFocus-----------*/
/*	
	Descrpition : Determinant if Textfield is accepting focus at current status
	                   �ж��Ƿ��ѻ�ý���
	Input : Pointer to Textfield 
	Initial condition : Textfield object has exist and has been enroled
	Ouptut: if permit get focus then return true.else return false.
*/
bool MidControlTextFieldIsAcceptingFocus(ControlItem_t *pTextFieldItem);

/*-------------MidControlTextFieldRestoreSetting-----------*/
/*	Descrpition��restore the setting of text field control to original setting
	Input�� Pointer to Textfield
	Output��void
	*/
void MidControlTextFieldRestoreSetting(ControlItem_t *pTextFieldItem);

void DrawCurPos(ControlItem_t *pTextFieldItem);
bool MidControlTextFieldInit(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t* pControlList);
HRESULT MidControlTextFieldSetCallback(struct ControlItem *this, void *pFunc, int nType);
bool MidControlTextFieldOverlayChar(ControlItem_t *pTextFieldItem, char cNew,	ControlList_t* pControlList) ;
HRESULT MidControlTextFieldUpdate(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t *pcontrolList);
void MidControlTextFieldNewMove(ControlItem_t *pTextFieldItem, int nStep, MessageInfo_t *pMsgOut);
bool MidControlTextFieldUpChar(ControlItem_t *pTextFieldItem);
bool MidControlTextFieldDownChar(ControlItem_t *pTextFieldItem);
void MidControlTxtFieldCharInputComplete();

#endif
