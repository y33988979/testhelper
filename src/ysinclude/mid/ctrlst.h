
#ifndef _CONTROL_ST_H_
#define _CONTROL_ST_H_

#include "stlite.h"
#include "stbtype.h"
//#include "text8.h"
#include "stberror.h"
#include "driver.h"


#define COLOR					int			
#define MSGBOXID				10000		
typedef unsigned short			CONTROL_ID;
typedef unsigned short			CONTROL_CAT_ID;
typedef void					ControlInfo_t;
#define ROUNDCORNER				10000		



/*=================常量定义=======================*/
/*----------控件设定---------*/
#define ITEM_ENABLED					0x00000001	/*控件被允许*/
#define ITEM_VISIBLE						0x00000002	/*控件可见*/
#define ITEM_INFOCUS					0x00000004	/*控件拥有焦点*/
#define HIDEPARENT_ON_VISIBLE			0x00000008	/*显示子控件后隐藏父控件*/

#define SHOW_SON						0x00000010	/*显示本控件的所有子控件*/
#define SHOWSON_ON_SINGLEPANEL		0x00000020	/*在同一个面板上显示所有子控件*/
#define SHOW_ALL_ABOVE_LEVEL			0x00000040	/*显示所有上层次的控件*/
#define ITEM_COMMAND					0x00000080	/*控件用于命令目的*/

#define ITEM_STATUS						0x00000100	/*控件用于表示状态*/
#define ITEM_CHECKED					0x00000200	/*控件被选中*/
#define ITEM_SUBVISIBLE					0x00000400	/*控件的子控件可见*/
#define ITEM_LEAF						0x00000800	/*控件是一个叶子控件*/

//#define ITEM_SUBINFOCUS				0x00001000	/*控件的子控件拥有当前焦点*/
#define ITEM_PASSWORD					0x00001000	/*仅用于文本输入框，输入为密码*/
#define ITEM_SHOWSONONFOCUS			0x00002000	/*当控件拥有焦点时显示其子控件*/
#define ITEM_HORIZON					0x00004000	/*控件被水平的列出*/
#define ITEM_WITHOUTFOCUS				0x00008000	/*控件不可以拥有焦点*/

#define ITEM_RECORD						0x00010000	/*the items are selected to be recorded*/
#define ITEM_RING						0x00020000  /*控件的数据能够循环显示(table控件专用)*/
#define ITEM_NOBG						0x00040000  /*不显示控件的背景(现为textlabel控件专用)*/
#define ITEM_PAGE_TABLE				0x00080000  /*页首节点固定模式(table控件专用)*/

#define ITEM_PAGE_TABLE_FOCUS_0		0x00100000  /*页首节点固定模式下翻页的焦点是否固定在第0项(table控件专用)*/

#define ORIGINAL_SHOW			0x00000001	
#define GET_FOCUS				0x00000002	
#define WITH_FOCUS				0x00000004	
#define LOSE_FOCUS				0x00000008	
#define WITHOUT_FOCUS			0x00000010	
#define GET_CHECK				0x00000020	
#define WITH_CHECK				0x00000040	
#define LOSE_CHECK				0x00000080	
#define WITHOUT_CHECK			0x00000100	
#define CONTENT_ONLY			0x00000200
#define FRAME_ONLY				0x00000400	
#define REFRESH_CLIENT			0x00000800	

#define REFRESH_SHOW			0x00001000	/*刷新显示，当控件的Setting发生改变时重画*/


/*---------控件类型常量---------*/
enum ControlType {
	BUTTON = 1,
	CHECKBOX,
	RADIO,
	RADIOGROUP,
	MENUITEM,
	MENU,
	TEXTLABEL,
	TEXTAREA,
	TEXTFIELD,
	BITMAP,
	CARET,
	PANEL,
	DIALOG,
	GRID,
	TABLE,
	FRAME,
	TEXTCHOICE,
	CONTROL_CAT_NUM
};

#define LIST_START		CONTROL_CAT_NUM+1	 
#define	LIST_END		CONTROL_CAT_NUM+2	 
#define	APP_END			CONTROL_CAT_NUM+3	 

/*控件错误代码*/
enum ControlErrorCode{
	CTRL_MEMORY_ERR = CONTROL_BASE + 1,
	CTRL_POINTER_ERR,
	CTRL_FUNC_STYLE_ERR,
	INVALID_ITEM,
	INVALID_LIST,
	MSGBOX_CREATE_ERROR,
	MSGBOX_ERROR
};

/*控件函数类型*/
enum ControlFuncType{
	ENTER = 1,
	LEAVE,
        DRAWICON,//table,txtarea使用，在可翻页时画箭头
	PREVPROC,
	FOLLOWPROC
};




typedef struct Language
{
  void *Language_Type[2];
}Language_t;


/*----------其它------------*/
#define FIELD_SEP			"[/]"	/*字符串中各数据项的分隔符*/
#define SUB_FIELD_SEP		";"

#ifndef NULL
#define NULL				0
#endif
#ifndef true
#define true				1
#define	false				0
#endif

/*------------------------------------------*/
/*structure for the coordinates of rectangle*/

#if 0/*move to middraw.h*/
typedef struct Rect{
	int nLeft;				//the cordinate of the left border
	int nTop;				//the cordinate of the top border
	int nRight;				//the cordinate of the right border
	int nBottom;			//the cordinate of the bottom border
} Rect_t;
#endif

/*一些特殊的取值将被赋予特殊的含义，包括：
nLeft：
	-1: Always meets the left border of the parent display
	-2: Always meets the left control in the same panel
	other: Locate the contorl at the specified position of the grid
nTop：
	-1: Always meets the top border of the parent display
	-2: Always meets the top control in the same panel
	other: Locate the contorl at the specified position of the grid 
nRight：
	-1: Always meets the right border of the parent display
	-2: Always meets the right control in the same panel
	other: Locate the contorl at the specified position of the grid
nBottom：
	-1: Always meets the bottom border of the parent display
	-2: Always meets the bottom control in the same panel
	other: Locate the contorl at the specified position of the grid
*/

/*------------------------------------------*/
/*structure for the size of rectangle*/
typedef struct Size{
	int nX;				//the cordinate on x
	int nY;				//the cordinate on y
} Size_t;

typedef struct MidLineStyle
{
	UI16			nWidth;		/*The width of the line*/
	int				nStyle;		/*The style of the line, solid, dash, etc.*/
	COLOR			crNormal;	/*he color for normal status*/
	COLOR			crSpecial;	/*The color for special status*/
}MidLineStyle_t;

#if 0/*move to middraw.h*/
typedef struct MidTextStyle
{
	COLOR			crFore;		//the foreground color of text
	COLOR			crBack;		//the background color of text
	UI16			nFontSize;  //the size of the text
	UI16			nFontStyle;	//the style of th text
	int				nStyle;		//the alignment of the text
}MidTextStyle_t;
#endif

typedef struct MidTextSetting
{
	UI32			nMask;		//the mask to match setting of controlitem
	MidTextStyle_t	stTextStyle;//the style of controlitem text
}MidTextSetting_t;

typedef struct MidTextSettingList 
{
	UI16					nItem;		//number of setting in the list
	MidTextSetting_t*		pSettings;	//pointer to the header of the array	
} MidTextSettingList_t;

typedef struct MidLineSetting
{
	UI32			nMask;		//the mask to match setting of controlitem
	MidLineStyle_t	stLineStyle;//the style of controlitem line
}MidLineSetting_t;

typedef struct MidLineSettingList 
{
	UI16					nItem;		//number of setting in the list
	MidLineSetting_t*		pSettings;	//pointer to the header of the array
} MidLineSettingList_t;



//typedef void (*MidDrawFunction)(ControlItem_t * pItem, int nStyle);
typedef void (*MidDrawFunction)(void *,int nStyle);

typedef struct MidDrawSetting
{
	UI32					nMask;			//the mask to match setting of controlitem
	MidDrawFunction			DrawFunction;	//the draw function of controlitem
}MidDrawSetting_t;

typedef struct MidDrawSettingList 
{
	UI16					nItem;			//number of setting in the list
	MidDrawSetting_t		*pDrawSettings;	//pointer to the header of the array
} MidDrawSettingList_t;

typedef struct MidMsgSetting
{
	UI32					nMask;			//the mask to match setting of controlitem
	MessageInfo_t			msgOut;	//the draw function of controlitem	
}MidMsgSetting_t;

typedef struct MidMsgSettingList 
{
	UI16					nItem;			//number of setting in the list
	MidMsgSetting_t		*pSettings;	//pointer to the header of the array
} MidMsgSettingList_t;


typedef struct MidGraphicSetting {
	UI32					nMask;			//the mask to match setting of controlitem
	char					*pFileName;		//图片的文件名
}MidGraphicSetting_t;

typedef struct MidGraphicSettingList {
	UI16					nItem;				//number of setting in the list
	MidGraphicSetting_t		*pSettings;	//pointer to the header of the array
} MidGraphicSettingList_t;

typedef struct MidShadowStyle
{
	MidLineStyle_t			stUpShadow;
	MidLineStyle_t			stDownShadow;
}MidShadowStyle_t;

typedef void		MidControlInitParam_t;

struct ControlItem;

/*控件的方法*/
/*
 * Function:	MidControlSetCallback
 * Description:	设置控件响应事件函数，根据nStyle的类型，将pFunc指向的函数赋给相应的响应事件的函数指针
 * Calls:       无
 * Called By:      
 * Input:       struct ControlItem *this	此控件自身的指针
 *              void *pFunc					应用响应事件函数指针
 *				int nType					设置类型
 * Output:      无
 * Return:      设置是否成功
 * Others:         	
 */
typedef HRESULT (*MidControlSetCallback)(struct ControlItem *this, void *pFunc, int nType);

/*控件响应事件的回调函数*/
/*
 * Function:	MidControlEnter
 * Description:	当焦点移动到此控件时发生此事件
 * Calls:       无
 * Called By:      
 * Input:       struct ControlItem *this	此控件自身的指针
 * Output:      无
 * Return:      无
 * Others:         	
 */
typedef HRESULT (*MidControlEnter)(struct ControlItem *this);

/*
 * Function:	MidControlLeave
 * Description:	当焦点离开此控件时发生此事件
 * Calls:       无
 * Called By:      
 * Input:       struct ControlItem *this	此控件自身的指针
 * Output:      无
 * Return:      无
 * Others:         	
 */
typedef HRESULT (*MidControlLeave)(struct ControlItem *this);

/*
 * Function:	MidControlNonModelDialog
 * Description:	用于非模态对话框对消息的处理
 * Calls:       无
 * Called By:      
 * Input:       pMsg    按键消息
 * Output:      无
 * Return:      非模态对话框是否对消息处理了.
 * Others:         	
 */
typedef BOOL (*MidControlNonModelDialog)(MessageInfo_t *pMsg);



/*----------------------ControlItem-------------------*/
/*控件链表中的每个控件均有一个"控件链表数据项结构"描述*/

typedef bool (*MidControlUserProcess)(struct ControlItem *pItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut, bool *pFlag);

typedef struct ControlItem {
	CONTROL_ID				nId;			//控件的唯一标识符
	struct ControlCatInfo	*pCatInfo;		//控件类型信息指针
	ControlInfo_t			*pInfo;			//控件信息指针
	KEY_CODE				nKeyCode;		//控件快捷键码
	int						iLevel ;		//控件所处的层次;        
	Rect_t					rcControl;		//控件的位置
	UI32					nSetting;		//控件当前状态
	UI32					nPreSetting;	//控件的前一状态
	UI32					nOrgSetting;	//控件缺省状态
	int						iRadius;
	int					iTextCount;
	union Textbody{
		char				*pLabel;		//显示的字符串
		char				*pBody;			//控件文本内容
	}*TextInfo;
	int						nLen;			//文本缓冲区长度
	MidMsgSettingList_t		msgList;		//控件响应消息设定 
	MidTextSettingList_t	TextSetting;	//文本风格设定
	MidLineSettingList_t	LineSetting;	//边框风格设定	
	MidDrawSettingList_t	*pDrawSetting;	//控件的特殊画法
	MidLineStyle_t			*pFocusLine;	//控件的焦点线风格设定
	MidShadowStyle_t		*pShadow;		//控件的阴影风格设定
	struct ControlItem		*pNext;			//指向下一个控件的指针
	struct ControlItem		*pPrev;			//指向上一个控件的指针

	BOOL					bGraphic;		//控件显示方式(是否以图形方式)				
	MidGraphicSettingList_t GraphicSetting;	//控件显示图片列表
	MidControlSetCallback	SetCallback;	

	MidControlEnter			Enter;		
	MidControlLeave			Leave;

   MidControlUserProcess PrevProcess;
   MidControlUserProcess FollowProcess;
	
} ControlItem_t;

/*------------ControlList---------------*/
/*整个链表的信息由"控件链表信息结构"描述*/
typedef struct ControlList {
	int					nID;				 
	UI32				nSetting;			
	UI32				nPrevID;			
	struct ControlList	*pNext;				 
	struct ControlList	*pParent;			
	UI32				nItem;				//链表中的数据项数目
	DRVOS_Partition_t			*ptnApp;	//链表分配内存所用分区
	struct ControlCatList *pControlCatList; //控件类型链表指针
	ControlItem_t		*pHead;				//链表首指针
	ControlItem_t		*pCurrent;			//当前数据项指针
	ControlItem_t		*pEnd;				//链表尾指针
	ControlItem_t		*pFocus;			//当前焦点控件指针
} ControlList_t;

typedef ControlItem_t* (*MidControlCreate)(ControlList_t *pControlList, char *pstrFeature, int nLen);
typedef	HRESULT (*MidControlDestroy)(ControlItem_t *pItem,ControlList_t* pControlList);
typedef bool (*MidControlProcess)(ControlItem_t *pItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut,ControlList_t *pControlList);
typedef void (*MidControlDraw)(ControlItem_t * pItem, int nStyle);
typedef HRESULT (*MidControlLoadFromString)(ControlItem_t *pItem, char *pstrFeature, int nLen,ControlList_t *pControlList);
typedef void (*MidControlRestoreSetting)(ControlItem_t *pItem);
typedef HRESULT (*MidControlLoseFocus)(ControlItem_t *pItem);
typedef HRESULT (*MidControlGetFocus)(ControlItem_t *pItem);
typedef ControlItem_t* (*MidControlCopy)(ControlItem_t *pItem,ControlList_t* pControlList);
typedef bool (*MidControlInit)(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t* pControlList); 
typedef char* (*MidControlGetText)(ControlItem_t *pItem); 
typedef HRESULT (*MidControlSetText)(ControlItem_t *pItem, char *pText, int nLen, ControlList_t* pControlList); 
typedef HRESULT (*MidControlGetStatus)(ControlItem_t *pItem, void *pStatus); 
typedef HRESULT (*MidControlSetStatus)(ControlItem_t *pItem, void *pStatus); 
typedef HRESULT (*MidControlUpdate)(ControlItem_t *pItem, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam, ControlList_t *pcontrolList); 




/*--------------ControlCatInfo----------------*/
/*每种类型的控件均通过"控件类型信息结构"来描述*/
typedef struct ControlCatInfo {
	CONTROL_CAT_ID	nCatId;				//唯一控件类型标识符
	struct ControlCatInfo *pNext;		//指向下一个结构的指针
	struct ControlCatInfo *pPrev;		//指向前一个结构的指针
	MidControlCreate			Create; 
	MidControlDestroy			Destroy;
	MidControlProcess			Process;
	MidControlDraw				Draw;
	MidControlLoadFromString	LoadFromString;
	MidControlRestoreSetting	RestoreSetting;
	MidControlLoseFocus			LoseFocus;
	MidControlGetFocus			GetFocus;
	MidControlCopy				Copy;
	
	MidControlInit				Init;		 
	MidControlGetText			GetText;	 
	MidControlSetText			SetText;	 
	MidControlGetStatus			GetStatus;	 
	MidControlSetStatus			SetStatus;	 
	MidControlUpdate			Update;		 
} ControlCatInfo_t;
/*其中：
	MidControlCreate()：创建控件实例，初始化对象存储区，注册控件实例，并保留调色板。（每个控件都有创建、删除两个基本函数处理内存分配、调色板等一般问题。）
	MidControlDestroy()：删除控件实例，清除占用对象存贮区及注册项目，恢复调色板。
	MidControlProcess()：处理入口消息，处理结果同样用消息的方式发出（如果存在的话）。
	MidControlDraw()：以指定的方式显示控件。
	MidControlLoadFromString()：从pstrFeature指向的长为nLen的字符串中读取控件参数信息，放入pFeatures指向的数据结构
	MidControlRestoreSetting()：恢复控件的设定为缺省设定
	MidControlLoseFocus()：控件失去焦点
	MidControlGetFocus()：控件获得焦点
	MidControlCopy()复制控件信息，创建一个相同的控件
*/


/*---------------------------ControlCatList-------------------------*/
/*这些控件类型信息结构在系统中也通过类似于控件实例链表的方式进行组织*/
typedef struct ControlCatList {
	ControlCatInfo_t		*pCats;			//链表首指针
	int 					nCat;			//数目
} ControlCatList_t;

/*----------------AppGui----------------*/
/*应用界面数据结构*/
typedef struct AppGui{
	ControlCatList_t		*pCatList;				//控件类型列表
	ControlList_t			*pControlList;			//控件列表
	ControlList_t			*pCurrentList;			 
} AppGui_t;
		

//function declaration 
HRESULT ControlListInit(ControlList_t* pControlList, CONTROL_ID nID,void *pObj,void *pObjCat); 
char* MidControlListGetText(ControlList_t* pControlList, CONTROL_ID nID); 
HRESULT MidControlListSetText(ControlList_t* pControlList, CONTROL_ID nID, char *pText, int nLen); 
HRESULT MidControlListGetStatus(ControlList_t* pControlList, CONTROL_ID nID, void *pStatus); 
HRESULT MidControlListSetStatus(ControlList_t* pControlList, CONTROL_ID nID, void *pStatus); 

ControlList_t* ControlListCreateFromFile(char *ConfigName[],DRVOS_Partition_t *ptnApp,ControlCatList_t *pControlCatList);
HRESULT ControlListDestroy(ControlList_t* pControlList);
HRESULT ControlListInsertBefore(ControlList_t* pControlList, ControlItem_t *pNewControl);
HRESULT ControlListInsertAfter(ControlList_t* pControlList, ControlItem_t *pNewControl);
HRESULT ControlListAppend(ControlList_t* pControlList, ControlItem_t *pNewControl);

/*----------ControlListDeleteItem----------*/
/*
输入：控件列表,要删除的控件结构指针
输出：错误信息
说明：删除当前索引项，同时调整链表指针。如果当前控件为链表末尾，则当前指针指向新链表的末尾。
*/
HRESULT ControlListDeleteItem(ControlList_t* pControlList, ControlItem_t *pDeleteControl);
HRESULT ControlListDeleteCurrent(ControlList_t* pControlList);
ControlItem_t* ControlListSetCurrentFocus(ControlList_t * pControlList);

/*遍历控件链表的函数*/
ControlItem_t* ControlListMoveHead(ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMoveEnd(ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMoveParent(ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMoveSon(ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMovePrev(ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMoveNext (ControlList_t * pControlList, bool bFocus);
ControlItem_t* ControlListMoveNextOfLevel (ControlList_t * pControlList, bool bFocus, int iLevel);
ControlItem_t* ControlListMoveSibling(ControlList_t * pControlList, bool bPrev, bool bFocus);
/*----------ControlListMoveTo----------*/
/*
说明：移动到指定控件；如果选择移动焦点，则在移动当前指针的同时移动控件焦点
输入：控件列表，控件结构指针，是否移动焦点
输出：
返回: 控件指针
其它：如果移动焦点，则该控件必须可以得到焦点，若该控件不能得到焦点，则不移动
*/
ControlItem_t* ControlListMoveTo(ControlList_t * pControlList, ControlItem_t *pControlItem, bool bFocus);
ControlItem_t* ControlListGetCurrent(ControlList_t * pControlList);
ControlItem_t* ControlListGetFocusItem(ControlList_t *pControlList);
ControlCatInfo_t* ControlListGetCatInfo(ControlList_t * pControlList);
ControlItem_t* ControlListLocateById(ControlList_t * pControlList, CONTROL_ID nId);
ControlItem_t* ControlListLocateByKey(ControlList_t * pControlList, KEY_CODE nKeyCode);
CONTROL_ID ControlListGetNewCtlId(ControlList_t * pControlList) ;
HRESULT ControlListMoveFocus(ControlList_t * pControlList);
bool ControlListProcess(ControlList_t * pControlList, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut);
void ControlListLoseFocus(ControlList_t * pControlList);
ControlItem_t * ControlListGetFocus(ControlList_t * pControlList);
void ControlListDraw(ControlList_t * pControlList, int nStyle);
HRESULT ControlListCatAppend (ControlCatList_t *pCatList, ControlCatInfo_t *pCatItem);
HRESULT ControlListCatDestroy(ControlCatList_t *pCatList,DRVOS_Partition_t *ptnApp);
ControlCatList_t * ControlListCatInit(DRVOS_Partition_t *ptnApp);
AppGui_t * AppGuiInit(char *ConfigName[], DRVOS_Partition_t *ptnApp);
HRESULT AppGuiDestroy(AppGui_t *pGui);


/*-----------ControlItemCreate-----------*/
/*
说明：建立一个控件。
输入：控件类型信息结构链表指针
输出：指向控件的指针
*/

ControlItem_t* ControlItemCreate(ControlList_t * pControlList);

/*----------ControlListSetCat----------*/
/*
说明：设定控件的类型信息
输入：控件类型链表指针，控件结构指针、控件类型
输出：错误代码
*/
HRESULT ControlListSetCat(ControlList_t* pControlList,ControlItem_t* pControlItem, UI16 nCat);
/*----------ControlListSetID----------*/
/*
说明：设定控件的ID
输入：控件类型链表指针，控件结构指针
输出：错误代码
*/
HRESULT ControlListSetID(ControlList_t* pControlList, ControlItem_t* pControlItem);
/*----------ControlCat----------*/
/*
说明：将控件类型字符串转换成控件类型
输入：控件类型字符串
输出：控件类型
*/
UI16 ControlCat(char *pCat); 


bool MidControlGetOutMessage(ControlItem_t *pControl, MessageInfo_t *pMsgOut);
void MidControlSetDrawFunction(CONTROL_ID nId,MidDrawSettingList_t *pDrawList); 


ControlList_t* ControlListGetCurrentList(ControlList_t *pList);
HRESULT	ControlListAddList(ControlList_t *pList,ControlList_t *pNewList);
HRESULT	ControlListDeleteList(ControlList_t *pList,UI32 nListID);
HRESULT MidControlMsgBox(ControlList_t *pList,char *pText,UI32 nType,int iX0,int iY0);
ControlList_t* ControlListCreateFromString(char **pstrInfo ,DRVOS_Partition_t *ptnApp,ControlCatList_t *pControlCatList);
ControlList_t* ControlListGetListByID(ControlList_t *pList,UI32 nID);


/* Name			: ControlItemIsCanGetFocus
 * Description	: 判断该控件是否能够得到焦点
 * Parameters	: ControlItem_t *pControlItem 控件句柄
 * Return		: false:不能得到焦点;true:可以得到焦点
 * Other		: 通过判断控件的属性ITEM_VISIBLE、ITEM_WITHOUTFOCUS
 */
BOOL ControlItemIsCanGetFocus(ControlItem_t *pControlItem);

/* Name			: ControlItemGetFocus
 * Description	: 调用该控件的GetFocus函数，响应该控件的Leave事件
 * Parameters	: ControlItem_t *pControlItem 控件句柄
 * Return		: 无
 * Other		: 
 */
void ControlItemGetFocus(ControlItem_t *pControlItem);

/* Name			: ControlItemLoseFocus
 * Description	: 调用该控件的GetFocus函数，响应该控件的Leave事件
 * Parameters	: ControlItem_t *pControlItem 控件句柄
 * Return		: 无
 * Other		: 
 */
void ControlItemLoseFocus(ControlItem_t *pControlItem);

/* Name			: ControlListSetSetting
 * Description	: 设置ControlList的nSetting值
 * Parameters	: ControlItem_t *pControlItem 	控件链表指针
 *				  UI32			nSetting		要设置的属性
 * Return		: 无
 * Other		: 
 */
void ControlListSetSetting(ControlList_t *pControlList, UI32 nSetting);
void ControlListItemDraw(ControlList_t * pList, int nStyle);
void ControlListNewDraw(ControlList_t	*pHeadControlList, int nStyle);
void ControlListNewLoseFocus(ControlList_t * pControlList);
void  ControlListNewGetFocus(ControlList_t * pControlList);
void ControlListNewHide(ControlList_t * pControlList);
char* ControlItemChooseText(const ControlItem_t *pControlItem);
void ControlListNoFocusDraw(ControlList_t * pList, int nStyle);
#endif
