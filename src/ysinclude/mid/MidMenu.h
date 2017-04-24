/*
  MidMenu.h
 */
 


#ifndef	_MIDMENU_H_
#define	_MIDMENU_H_

#include "ctrlst.h"

#define COLORREF				int

#define	SOLID					0

#define ALIGNMENT_MID			0
#define ALIGNMENT_LEFT			1
#define ALIGNMENT_RIGHT			2

#define NORMAL_VIEW				0
#define GRAY_VIEW				1

#define SHOW_ON_SINGAL_PANEL	0
#define HIDE_PARENT_ON_VISIBLE	1

//#define ITEM_WITHOUTFOCUS		0

#define HAVE_CHILD				1
#define NO_CHILD				0

#define PREV					0
#define NEXT					1

#define UP						0
#define DOWN					1

#define NO_FOCUS				0
#define FOCUS					1


//{{{----------add by    ----------
/*
typedef struct MenuItemFeature {
//	Rect_t				rcPanel;	
//	BOOLEAN				bFocus;	       //是否为活动状态
//	BOOLEAN				bVisible;      //是否正在显示
//	BOOLEAN				bChild;
//	int					nViewMode;		//菜单项显示模式：正常显示、变灰。
//	UI32				nMsgCode;		//菜单项点击后发出的消息代码
}MenuItemFeature_t;
*/

typedef struct MenuFeature {
	ControlItem_t		*pMenuItems;
	int					nMenuItems;
	ControlItem_t		*pEnd;			//链表尾指针
}MenuFeature_t;

//----------add by    ----------}}}
/*
void DrawRect(Rect_t rcRect);
void DrawTriagle(Rect_t rcRect);
void DrawShadow(Rect_t rcRect);
void DrawUNShadow(Rect_t rcRect);
*/

/*
void MidControlMenuDraw (ControlItem_t* pMenuItem,int nStyle);
void MidControlMenuItemDraw(ControlItem_t *pMenuItem,int nStyle);
HRESULT MidControlMenuItemDestroy(ControlItem_t *pMenuItem,ControlList_t* pControlList);
ControlItem_t* MidControlMenuItemCopy(ControlItem_t *pMenuItem ,ControlList_t* pControlList);
void MidControlMenuItemClear(ControlItem_t *pMenuItem, COLORREF color);
void MidControlMenuItemClearFocus(ControlItem_t *pMenuItem);
void ControlListCreate(ControlList_t* pControlList);
HRESULT MidControlMenuItemGetFocus(ControlItem_t* pMenuItem);
HRESULT MidControlMenuItemLoseFocus(ControlItem_t* pMenuItem);
ControlItem_t* MidControlMenuLocateSibling(ControlItem_t *pMenuItem, BOOLEAN bPrev);
ControlItem_t* MidControlMenuLocateParent(ControlItem_t *pMenuItem);
ControlItem_t* MidControlMenuLocateSon(ControlItem_t *pMenuItem);
ControlItem_t* MidControlMenuGetFocusItem(MenuFeature_t* pMenuInfo);
void MidControlMenuItemRollSib(MenuFeature_t *pMenuInfo, BOOLEAN bUp); 
void MidControlMenuItemRollParent(MenuFeature_t *pMenuInfo);
void MidControlMenuItemRollSon(MenuFeature_t *pMenuInfo);
void MidControlMenuShowSub(ControlItem_t *pMenuItem, BOOLEAN bMoveFocus);
void MidControlMenuHideSub(ControlItem_t *pMenuItem);
void MidControlMenuRetreatToRoot(ControlItem_t *pMenuItem);


//{{{----------add by    ----------
void MidControlMenuItemSetCat(ControlCatInfo_t *pCatInfo);
HRESULT MidControlMenuItemLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t *pControlList);
ControlItem_t * MidControlMenuItemCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);
bool MidControlMenuItemProcess(ControlItem_t *pMenuItem,  MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);
void MidControlMenuSetCat(ControlCatInfo_t *pCatInfo);
ControlItem_t * MidControlMenuCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);
HRESULT MidControlMenuLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t *pControlList);
HRESULT MidControlMenuDestroy(ControlItem_t *pMenuItem ,ControlList_t* pControlList);
ControlItem_t *MidControlMenuCopy(ControlItem_t *pMenuItem,ControlList_t *pControlList);
HRESULT MidControlMenuRoll (ControlItem_t *pMenu, bool bUp) ;
void MidControlMenuClear(ControlItem_t *pMenuItem, COLORREF color);
HRESULT MidControlMenuLoseFocus (ControlItem_t *pMenu) ;
HRESULT MidControlMenuGetFocus (ControlItem_t *pMenu) ;
bool MidControlMenuIsAcceptingFocus(ControlItem_t *pMenuItem);
bool MidControlMenuProcess (ControlItem_t *pMenu,  MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList) ;
HRESULT MidControlMenuAddItem(ControlItem_t*pMenu,ControlItem_t *pMenuItem);
//----------add by    ----------}}}
*/
/*****************************************************************************
 *****                            MenuItem                               *****
 ****************************************************************************/

//{{{----------add by    ----------
void MidControlMenuItemSetCat(ControlCatInfo_t *pCatInfo);

/*MidControlMenuItemCreate
说明：构造MenuItem
输入：控件列表指针、控件信息字符串、控件信息字符串长度;
输出：如果创建成功 返回控件指针；否则返回NULL；*/
ControlItem_t * MidControlMenuItemCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);

/*MidControlMenuItemLoadFromString
说明：从字符串中读取控件信息并构造MenuItem，控件信息结构由外部构造后送入，但结构内部其它指针所指向的空间的分配需要函数自己完成。各属性之间用特殊符号隔开。
输入：MenuItem信息结构指针、属性字符串、字符串长度
输出：错误代码*/
HRESULT MidControlMenuItemLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t *pControlList);

/* Name			: MidControlMenuItemDestroy
 * Description	: Destroy the menuitem by releasing the memory allocated for it
 * Paramters	:
 *			pMenuItem	:[In] pointer to the menuitem ot delete
 *			pControlList:[In] pointer to the list the item belong*/
HRESULT MidControlMenuItemDestroy(ControlItem_t *pMenuItem,ControlList_t* pControlList);

/* Name			: MidControlMenuItemProcess
 * Description	: Prcoessing the input message
 * Parameters	:
 * Return		:	*/
bool MidControlMenuItemProcess(ControlItem_t *pMenuItem,  MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);

/* Name			: MidControlMenuItemGetFocus
 * Description	: The specified menuitem getfocus
 * Parameters	: 
 *			pMenuItem	:[In] pointer to the menuitem
 *			bshowSub	:[In] show subs when the item get focus*/
HRESULT MidControlMenuItemGetFocus(ControlItem_t* pMenuItem);

/* Name			: MidControlMenuItemLoseFocus
 * Description	: The specified menu item loses focus
 * Parameters	:
 *			pMenuItem	:[In] pointer to the menuitem*/
HRESULT MidControlMenuItemLoseFocus(ControlItem_t* pMenuItem);

/* Name			: MidControlMenuItemShowSub
 * Description	: Show the subs of the menuitem
 * Parameters	:
 *				pMenuItem	:[In] pointer to the menuitem
 *				bMoveFocus	:[In] move focus after showing the subs
 * Return		: err code*/
HRESULT MidControlMenuItemShowSub(ControlItem_t *pMenuItem, bool bMoveFocus);

/* Name			: MidControlMenuItemHideSub
 * Description	: Hide the sub of the menuitem
 * Parameters	:
 *				pMenuItem	:[In] pointer to the menuitem*/
void MidControlMenuItemHideSub(ControlItem_t *pMenuItem);

/* Name			: MidControlMenuItemShowSib
 * Description	: Show the siblings of the menuitem
 * Parameters	:
 *				pMenuItem	:[In] pointer to the menuitem
 *				bMoveFocus	:[In] set focus the specified item after showing the items*/
void MidControlMenuItemShowSib(ControlItem_t *pMenuItem, bool bMoveFocus);

/* Name			: MidControlMenuItemHideSib
 * Description	: Hide the siblings of the menuitem, inclusive
 * Parameters	:
 *				pMenuItem	:[In] pointer to the menuitem*/
void MidControlMenuItemHideSib(ControlItem_t *pMenuItem);

/* Name			: MidControlMenuItemShow
 * Description	: show the menuitem
 * Parameters	:
 *			pMenuItem	:[In] pointer to the menuitem*/
void MidControlMenuItemShow(ControlItem_t *pMenuItem);

/* Name			: MidControlMenuItemHide
 * Description	: Hide the display of the menuitem
 * Parameters	:
 *			pMenuItem	:[In] pointer to the menuitem
 *			bSub		:[In] operation applying to subs*/
void MidControlMenuItemHide(ControlItem_t *pMenuItem, bool bSub);

/* Name			: MidControlMenuItemClear
 * Description	: Clear the display of the menuitem with color
 * Parameters	:
 *			pMenuItem	:[In] pointer to the menuitem
 *			color		:[In] color to redraw the area*/
void MidControlMenuItemClear(ControlItem_t *pMenuItem, COLORREF color);

/*
void DrawMenuItemName(char* cMenuItemName, Rect_t rcRect, int iFontSize, int iStyle, COLORREF crName);
*/
void MidControlMenuItemDraw(ControlItem_t *pMenuItem,int nStyle);

/* Name			: MidControlMenuItemLocateSibling
 * Description	: Locate the sibling of the specified item according to the set direction
 * Parameters	:
 * Return		:
 */
ControlItem_t* MidControlMenuItemLocateSibling(ControlItem_t *pMenuItem, bool bPrev);

/* Name			: MidControlMenuItemLocateSibling
 * Description	: Locate the first sibling of the specified item and return its pointer
 * Return		:
 */
ControlItem_t* MidControlMenuItemLocateFirstSibling(ControlItem_t *pMenuItem);

/*
 * Name			:  MidControlMenuItemLocateLastSibling
 * DESCRIPTION	:  Locate the last sibling of the specified item and return its pointer.
 * Parameters	:
 * Return		:
 */ 
ControlItem_t* MidControlMenuItemLocateLastSibling(ControlItem_t *pMenuItem);

/* Name			: MidControlMenuItemLocateParent
 * Description	: Locate the parent menu item of the specified menuitem
 * Parameters	:
 * Return		:*/
ControlItem_t* MidControlMenuItemLocateParent(ControlItem_t *pMenuItem);

/* Name			: MidControlMenuItemGetFocusItem
 * Description	: Locate the focused menuitem in the subs of the specified menuitem
 * Input		: Pointer to the menuitem
 * Output		: Pointer to the menuitem located */
ControlItem_t* MidControlMenuItemGetFocusItem(ControlItem_t* pMenuItem);

ControlItem_t* MidControlMenuItemCopy(ControlItem_t *pMenuItem,ControlList_t *pControlList );


/****************************************************************************
 *******                           Menu                               *******
 ****************************************************************************/

/* Name			:MidControlMenuSetCat 
 * Description	:Set the category information of the menu control
 * Input		:the pointer to the category structure
 * Output		:the category information*/
void MidControlMenuSetCat(ControlCatInfo_t *pCatInfo);

/* Name			:MidControlMenuCreate
 * Description	:Create menu from the input string
 * Input		:pointer to the control list, input string, input string length
 * Output		:poniter to the new menu control created*/
ControlItem_t * MidControlMenuCreate(ControlList_t *pcontrolList, char *pstrFeature, int nLen);

/*MidControlMenuLoadFromString
说明：从字符串中读取控件信息并构造menu，控件信息结构由外部构造后送入，但结构内部其它指针所指向的空间的分配需要函数自己完成。各属性之间用特殊符号隔开。
输入：menu信息结构指针、属性字符串、字符串长度
输出：错误代码*/
HRESULT MidControlMenuLoadFromString(ControlItem_t *pItem, char *pstrItem, int nLen,ControlList_t *pControlList);

/* Name			:MidControlMenuDestroy
 * Description	:Destroy the menu control
 * Input		:pointer to the menu control and the controlList
 * Output		:err code*/
HRESULT MidControlMenuDestroy(ControlItem_t *pMenu ,ControlList_t* pControlList);

/* Name			: MidControlMenuCopy
 * Description	: this function is no used here
 * Input		: pointer to the original control, the list the control is belonging
 * Output		: new pointer to menu control*/
ControlItem_t *MidControlMenuCopy(ControlItem_t *pMenuItem,ControlList_t *pControlList);


/* Name			: MidControlMenuRoll
 * Description	: scroll the menu up and down
 * Input		: pointer to the menu control, scroll up or down
 * Output		: err code*/
HRESULT MidControlMenuRoll (ControlItem_t *pMenu, bool bUp);

/* Name			: MidControlMenuProcess
 * Description	: Processing the incoming message
 * Parameters	:
 *					pMenu	: In, pointer to the control
 *					pMsgIn	: In, pointer to the input message
 *					pMsgOut	: In, pointer to the output message buffer
 *							  Out,output messagebody
 *					pControlList: In*/
bool MidControlMenuProcess (ControlItem_t *pMenu,  MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);

/* Name			: MidControlMenuGetFocusItem
 * Description	: Locate the focused menuitem in the menu
 * Input			: Pointer to the menu
 * Output		: Pointer to the menuitem located */
ControlItem_t* MidControlMenuGetFocusItem(ControlItem_t* pMenu);


/* Name			: MidControlMenuLoseFocus
 * Description	: The Menu lose focus
 * Parameters	:
 *				pMenu	:[In] pointer to the menu
 * Return		: Err code	*/
HRESULT MidControlMenuLoseFocus (ControlItem_t *pMenu);

/* Name			: MidControlMenuGetFocus
 * Description	: The Menu get focus
 * Parameters	:
 *				pMenu	:[In] pointer to the menu
 * Return		: Err code	*/
HRESULT MidControlMenuGetFocus (ControlItem_t *pMenu);

/* Name			: MidControlMenuIsAcceptingFocus
 * Description	: Determine if the current control get focus
 * Parameters	:
 *			pMenu	:[In] Pointer to the menu
 * Return		: true or false*/
bool MidControlMenuIsAcceptingFocus(ControlItem_t *pMenu);

/* Name			: MidControlMenuDraw
 * Description	: Draw the menu control
 * Status		: TOCHECK
 * Parameters	:
 *			pMenuItem	:[In] Pointer to the menu
 *			nStyle		:[In] The style of which the menu is drawn
 * Return		: nothing*/
void MidControlMenuDraw(ControlItem_t *pMenu, int nStyle);
	
/* Name			: MidControlMenuAddItem
 * Description	: Appned the menu item to the menu control
 * Parameters	:
 *			pMenu		:[In] Pointer to the menu
 *			pMenuItem	:[In] Pointer to the menu item
 * Return		: true or false*/
HRESULT MidControlMenuAddItem(ControlItem_t*pMenu, ControlItem_t *pMenuItem);
HRESULT MidControlMenuSetCallback(struct ControlItem *this, void *pFunc, int nType);
HRESULT MidControlMenuItemSetCallback(struct ControlItem *this, void *pFunc, int nType);


#endif





