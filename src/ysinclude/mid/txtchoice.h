

#ifndef _TXTCHOICE_H_
#define _TXTCHOICE_H_


#include "ctrlst.h"
#include "stberror.h"

#define	BUFFER_STEP		32
#define	FIELT_X_SIDE		30
#define	FIELT_Y_SIDE		8

typedef enum
{
	SHOW_WHEN_IN_FOCUS,
	SHOW_ALWAYS,
	SHOW_NEVER
}ShowTriangleMode_e;

typedef enum
{
	BIND_TO_ARRAY,
	BIND_TO_LIST
}ShowDataSourceMode_e;

typedef struct TriangleStyle
{
	COLOR			crNormal;	/*The color for normal status*/
	COLOR			crSpecial;	/*The color for special status*/
	unsigned short	TrangleWidth;
	unsigned short	TrangleHeight;
}TriangleStyle_t;

typedef struct TextChoiceFeature
{
	BOOL bFlag;
	BOOL IsCircle;
	int ArrayorList;	//0:绑定数组，1:绑定数组链表
	int Width;
	int High;
	int siItem;
	int siCurrent;
	char **ObjEnm;
	int ShowTglMode;//显示三角形的方式。0:老逻辑, 有焦点就画三角形，无焦点不画
					//							   1:不管有无焦点都画，颜色由TrangleStyle决定
					//							   2:不管有无焦点都不画三角形，方便用图的情况
	TriangleStyle_t *pTriangleStyle;
    MidControlGetText			GetText;
} TextChoiceFeature_t;
/*---------MidControlTextChoiceSetCat----------*/
/*	Description:set the type information of control
                        设置控件类型信息结构，主要是控件方法的函数指针赋值
	input:pointer to the control item category information
	output: void 
*/
void MidControlTextChoiceSetCat(ControlCatInfo_t *pCatInfo);

/*----------MidControlTextChoiceCreate-----------*/
/*	Description:Create a instance of Control,insert the information of TextChoice into the control list
                        用头端发送界面中的控件构成字符串填充控件链表数据项结构，
                   并把生成的控件加入到控件链表尾
	Input: pointer to the control list,the string of control item, the string length of control 
	Output:pointer to the control item. if create error , return null.
*/
ControlItem_t * MidControlTextChoiceCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);

/*----------MidControlTextChoiceLoadFromString-------------*/
/*	Description:load the layout setting from the string
                         从字符串填充TextLabel的ControlItem结构
	Input:		Pointer to the subindexblock, pointer to the string, string length
	Output:		error code*/
HRESULT MidControlTextChoiceLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t* pControlList);

/*-----------MidControlTextChoiceDestroy------------*/
/*	Destription:Destroy the instance of TextChoice
                         清除TextChoice的ControlItem结构
	Input: pointer to the TextChoice
	Output:error code
*/	
HRESULT MidControlTextChoiceDestroy(ControlItem_t *pTextChoiceItem,ControlList_t* pControlList);

/*--------------MidControlTextChoiceCopy-----------------*/
/*	Description:Copy the information of a Textfiled to a new Textfiled.It is not implemented here.
                         复制控件
	Input:pointer to the Textfiled,pointer to the control list
	Output:pointer to the new Textfiled
*/
ControlItem_t *MidControlTextChoiceCopy(ControlItem_t *pTextChoiceItem,ControlList_t* pControlList);

/*-----------MidControlTextChoiceMoveTo--------------*/
/*	Description：Set new position of cursor ,must insure new position not over border of pBody ;
                            设置光标到新的位置，并显示
	Input：pointer to the Textfiled,new position value ;
	Output：If success then return true；else return false ;
	*/
bool MidControlTextChoicePreMove(ControlItem_t *pTextChoiceItem);

/*--------------MidControlTextChoiceMove-------------*/
/*	
	Description : Move nStep step of cursor's position to a new position,neglect those which over border
	                   光标从当前位置移动nStep步，并显示
	Input：pointer to the Textfiled、Distance value for moving
	Ouput：Void
*/
bool MidControlTextChoiceBackMove(ControlItem_t *pTextChoiceItem);


/*------------------MidControlTextChoiceGetFocus------------------*/
/*	Description:TextChoice get focus
                         获得焦点
	Input:pointer to the TextChoice
	Output:error code
*/
HRESULT MidControlTextChoiceGetFocus(ControlItem_t *pTextChoiceItem);

/*-----------------MidControlTextChoiceLoseFocus-----------------*/
/*	Description:TextChoice lose focus
                         失去焦点
	Input:pointer to the TextChoice
	Output:error code
*/
HRESULT MidControlTextChoiceLoseFocus(ControlItem_t *pTextChoiceItem);

/*--------------MidControlTextChoiceProcess---------------*/
/*	Description:process button press message
                        消息处理
	Input:pointer to the control item,pointer to the input and output message
	Output:process or not
*/
bool MidControlTextChoiceProcess(ControlItem_t *pTextChoiceItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);

/*----------MidControlTextChoiceDraw------------*/
/*	Description: draw the TextChoice 
                        绘制TextChoice
	Input:pointer to the TextChoice ,the style of TextChoice 
	Output:void
*/
void MidControlTextChoiceDraw(ControlItem_t *pTextChoiceItem, int nStyle);

/*-------------MidControlTextChoiceDrawFrame-------------*/
/*	Descrpation : Display TextChoice's basic frame according special model
	Input ：Control pointer，Display model
	Output：void
	*/

/*-------------MidControlTextChoiceIsAcceptingFocus-----------*/
/*	
	Descrpition : Determinant if TextChoice is accepting focus at current status
	                   判断是否已获得焦点
	Input : Pointer to TextChoice 
	Initial condition : TextChoice object has exist and has been enroled
	Ouptut: if permit get focus then return true.else return false.
*/
bool MidControlTextChoiceIsAcceptingFocus(ControlItem_t *pTextChoiceItem);

/*-------------MidControlTextChoiceRestoreSetting-----------*/
/*	Descrpition：restore the setting of text field control to original setting
	Input： Pointer to TextChoice
	Output：void
	*/
void MidControlTextChoiceRestoreSetting(ControlItem_t *pTextChoiceItem);

bool MidControlTextChoiceInit(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t* pControlList);
HRESULT MidControlTextChoiceSetCallback(struct ControlItem *this, void *pFunc, int nType);

HRESULT MidControlTextChoiceUpdate(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t *pcontrolList);

HRESULT MidControlTextChoiceSetText(ControlItem_t *pItem, char *pNull, int siCurrent, ControlList_t* pControlList);

HRESULT MidControlTextChoiceRegGetText(ControlItem_t *pChoiceItem, void *pFunction);



#endif

