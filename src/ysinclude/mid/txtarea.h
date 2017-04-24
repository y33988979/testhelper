
#ifndef _TXTAREA_H_
#define _TXTAREA_H_

#include "ctrlst.h"
#include "stberror.h"
//Basic struct definition

#if 0
typedef struct MidLine{
    int	 iHead ;		// Row head index
	int  iEnd  ;		// Row tail index
}MidLine_t ;

typedef void (*MidTextAreaDrawIcon)(struct ControlItem *this);
typedef struct TextAreaFeature {
	int nLine ;					//total row number of text
	int nLinePerPage  ;			//row number per page
	int nSpace ;				//Distance between Hz and Hz -- Asc code are set as 2 pixels
	int nLineSpace    ;			//Space between rows
	int nXBorderSpace ;			//Space of page border
	int nYBorderSpace ;			//Space of page head and page food 
	int iPage;					//Current page No
	int iPageHeadLine  ;		//First row no of current page
	MidLine_t  **pLines;		//Row struct array
	MidTextAreaDrawIcon DrawIcon;
}TextAreaFeature_t;
#endif

#if 1
/*结合texter的area控件结构*/
typedef struct
{
  int Font;
  int Zoom;
  int FgColor;
  int BgColor;
}TextAreaTitle_t;

typedef void (*MidTextAreaDrawIcon)(struct ControlItem *this);
typedef struct TextAreaFeature 
{
  TEXT_TexterStyle_t style;
  unsigned int CurrentPage;
  //unsigned int PrevPage;
  //unsigned int NextPage;
  unsigned int MaxPage;
  unsigned int TotalPage;
  unsigned int *PageIndex;/*eg. PageIndex[3] = 135*/
  TextAreaTitle_t *Title;
  MidTextAreaDrawIcon DrawIcon;
}TextAreaFeature_t;
#endif

/* *****************************************************
*				TEXTAREA FUNCTION DECLARATION          
* ***************************************************** */
/*---------MidControlTextAreaSetCat----------*/
/*	Description:set the type information of control
	input:pointer to the control item category information
	output: void 
*/
void MidControlTextAreaSetCat(ControlCatInfo_t *pCatInfo) ;
/*----------MidControlTextAreaCreate-----------*/
/*	Description:Create a instance of TextArea,insert the information of grid into the control list
	Input: pointer to the control list,the string of control item, the string length of control 
	Output:pointer to the control item. if create error , return null.
*/
ControlItem_t * MidControlTextAreaCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen) ;
/*-----------MidControlTextAreaDestroy------------*/
/*	Destription:Destroy the instance of Textarea
                           清除Textarea的ControlItem结构
	Input: pointer to the Textarea
	Output:error code
*/	
HRESULT MidControlTextAreaDestroy(ControlItem_t *pTextAreaItem,ControlList_t* pControlList);
/*-----------MidControlTextAreaUpdate------------*/
/*
	Description:	update the TextArea by reconnecting itself and its subs with the ubderlying members
                             更新TextArea
	Input:			pointer to the TextArea, pointer to the TextArea object, pointer to the TextArea category object
	Output:			error code
*/
HRESULT MidControlTextAreaUpdate(ControlItem_t *pTextAreaItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t *pcontrolList);
/*----------MidControlTextAreaLoadFromString-------------*/
/*	Description:load the layout setting from the string
                        从字符串填充TextLabel的ControlItem结构
	Input:		Pointer to the subindexblock, pointer to the string, string length
	Output:		error code
*/
HRESULT MidControlTextAreaLoadFromString(ControlItem_t *pTextAreaItem, char *pstrFeature, int nLen,ControlList_t *pControlList);
/*----------MidControlTextAreaDraw------------*/
/*	Description: draw the TextArea 
                            绘制TextArea
	Input:pointer to the TextArea,the style of TextArea
	Output:void
*/
void MidControlTextAreaDraw(ControlItem_t *pTextAreaItem, int nStyle) ;
/*--------------MidControlTextAreaProcess---------------*/
/*	Description:function processing the input message for the the cell and return message in pMsgOut
                        消息处理
	Input:	pointer to the cell, pointer to the input and output message
	Output:	processed or not
	*/
bool MidControlTextAreaProcess (ControlItem_t *pTextAreaItem,MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList) ;
/*--------------MidControlTextAreaRoll---------------*/
/*Descrpation :Roll page of text according Input parameter
  Input  :Control pointer,roll direction
  Output :void
*/
void MidControlTextAreaRoll(ControlItem_t *pTextAreaItem, bool bRollDown, ControlList_t *pControlList) ;

/*--------------MidControlTextAreaCopy-----------------*/
/*	Description:Copy the information of a TextArea to a new TextArea.It is not implemented here.
                        复制控件
	Input:pointer to the TextArea,pointer to the control list
	Output:pointer to the new TextArea
*/
ControlItem_t *MidControlTextAreaCopy(ControlItem_t *pTextAreaItem,ControlList_t* pcontrolList);
/*------------------MidControltextarea_set_status------------------*/
/*	Description:Set the focus value of the control
	Input:		Pointer to the control item, with focus of not
	Output:		void
*/
void MidControltextarea_set_status (ControlItem_t *pTextAreaItem, bool bFocus) ;
void MidControlTextAreaLocate(ControlItem_t *pTextAreaItem) ;
void MidControlTextAreaRollForward(ControlItem_t *pTextAreaItem, ControlList_t *pControlList) ;
void MidControlTextAreaRollBack(ControlItem_t *pTextAreaItem, ControlList_t *pControlList) ;

/*Deal with focus*/
/*-----------------MidControlTextAreaLoseFocus-----------------*/
/*	Description:Let control lost focus
	Input:	pointer to the control
	Output:	succeed or not*/
HRESULT  MidControlTextAreaLoseFocus (ControlItem_t *pTextAreaItem) ;

/*------------------MidControlTextAreaGetFocus------------------*/
/*	Description:The subindexblock get focus, set the tag and redraw the item
	Input:		Pointer to the subindexblock
	Output:		succeed or not*/
HRESULT  MidControlTextAreaGetFocus(ControlItem_t *pTextAreaItem);
/*------------------MidControlTextAreaRestoreSetting------------------*/
/*	Description:restore the original setting of grid
	Input:pointer to the grid
	Output: void 
*/
void MidControlTextAreaRestoreSetting(ControlItem_t *pTextAreaItem);

/*-------------MidControlTextLabelSetText-----------*/
/*
Descrpation :Evalute a string to TextArea control.
Input :  Point to TextLabel, string, string's length, Control list
Output : if operation success then return true else return error
*/
HRESULT MidControlTextAreaSetText(ControlItem_t *pTextAreaItem,char *pText, int nLen, ControlList_t *pControlList);

HRESULT MidControlTextAreaSetCallback(struct ControlItem *this, void *pFunc, int nType);

bool MidControlTextAreaIsRoll(ControlItem_t *pTextAreaItem, int iRoll);

bool MidControlTextAreaIsRollUp(ControlItem_t *pTextAreaItem);

bool MidControlTextAreaIsRollDown(ControlItem_t *pTextAreaItem);


/*-----------------MidControlTextAreaGetPageInfo-----------------*/
/*
Description:	get txtarea page information
Input:			pTextAreaItem  ControlItem_t pointer
Output:			siCurrentPage  return current page from 1 to n
				siTotalPage    return total page
*/
void MidControlTextAreaGetPageInfo(ControlItem_t *pTextAreaItem, int *siCurrentPage, int *siTotalPage);

/*-----------------MidControlTextAreaPageShow-----------------*/
/*
Description:  turn to page siPageIndexShow
Input:		  pTextAreaItem ControlItem_t pointer
			  siPageIndexShow index of page from 0 to n-1
Output:		  NULL
return:		  true:success  false:fail
*/
bool MidControlTextAreaPageShow(ControlItem_t *pTextAreaItem, int siPageIndexShow);
#endif

