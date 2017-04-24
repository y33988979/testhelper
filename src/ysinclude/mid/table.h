
#ifndef _TABLE_H_
#define _TABLE_H_

#include "sttbx.h"  

//#include "common.h"
#include "ctrlst.h"

#define INVALID_COL		TABLE_BASE+1

#define ROW_MOVE    BUTTON_PRESS+100
/**************************************************************************/
typedef void MidTableRowObj_t;

/*The function for retrieving the text for the item member*/
typedef HRESULT (*MidTableRowObjGetText)(MidTableRowObj_t *pRowObj, int iCol, char *pText, int *pnLen, UI32 nSetting, MidTextStyle_t *pTextStyle);
/*The function for retrieving the status from underlying object*/
typedef HRESULT (*MidTableRowObjGetSetting)(MidTableRowObj_t *pRowObj, UI32 *pnSetting);
/*The function for saving the status from underlying object*/
typedef HRESULT (*MidTableRowObjSaveSetting)(MidTableRowObj_t *pRowObj, UI32 nSetting);
/*The function for retrieving the next underlying object*/
typedef HRESULT (*MidTableRowObjGetNext)(MidTableRowObj_t *pRowObj);
/*The function called to set message for the object connect to the selected row*/
typedef bool (*MidTableRowObjSetMsg)(MidTableRowObj_t* pObj, MessageInfo_t* pMsg, UI32 nOperation);
/*The function for getting the access to underlying object*/
typedef bool (*MidTableRowObjBlock)(MidTableRowObj_t* pObj);
/*The function for releasing the access to underlying object*/
typedef bool (*MidTableRowObjUnBlock)(MidTableRowObj_t* pObj);
 
typedef struct MidTableRowObjCat {
    MidTableRowObjGetText       GetText;
    MidTableRowObjGetSetting    GetSetting;
    MidTableRowObjSaveSetting   SaveSetting;
    MidTableRowObjGetNext       GetNext;
    MidTableRowObjSetMsg        SetMsg;
    MidTableRowObjBlock         Block;
    MidTableRowObjUnBlock       UnBlock;
} MidTableRowObjCat_t;


/* Name         : MidTableColSetting
 * Description  : This is the structure holding all the necessary infomation for columns*/
typedef struct MidTableColSetting {
    U8                    nColNum;            /*The number of column*/
    U8                    nScrollColNum;            /*The scroll column*/
    UI16                    *pnWidths;          /*the array of column width*/
    int *pnStyle;
    MidLineSettingList_t    LineSettings;       /*the pointer to the line setting list for column*/
} MidTableColSetting_t;

/* Name         : MidTableRowSetting
 * Description  : This is the structure holding all the necessary infomation for Rows*/
typedef struct MidTableRowSetting {
    UI16                    nHeight;
    MidTextSettingList_t    TextSettings;
    MidLineSettingList_t    LineSettings;
} MidTableRowSetting_t;

/* Name         : MidTableRowSetting
 * Description  : This is the structure holding all the necessary infomation for Rows*/
typedef struct MidTableRow {
//  UI16                    nRowId;
    Rect_t                  rcRow;
    UI32                    nSetting;           /*the displaying style of the row */
    MidTableRowSetting_t    *pRowSetting;       /*The general setting of the rows*/
    MidTableColSetting_t    *pColSetting;       /*The setting for the columns shared by all*/
    MidTableRowObj_t        *pRowObj;           /*pointer to the underlieing item*/ 
    MidTableRowObjCat_t     *pRowObjCat;
    /*TableRow的图形属性*/
    BOOL					bRowGraphic;		//tablerow显示方式(是否以图形方式)				
    MidGraphicSettingList_t *pRowGraphicSetting;	//tablerow显示图片列表    
    int Num;
}MidTableRow_t;


#define	SCROLL_BMP_NAME_MAX_LEN 8
//滚动条阴影设置
typedef struct MidTableScrollShadowSetting
{
	UI16							nShadowWidth;
	int							UpLeftLineColor;
	int							DownRightColor;
}MidTableScrollShadowSetting_t;
//滚动条头尾的设置
typedef struct MidTableScrollHeadSetting
{
	int							TrangelMargin;
	int							BackColor;
	int							ForeColor;
	Rect_t						HeadRect;
	Rect_t						EndRect;
	char							HeadBmp[SCROLL_BMP_NAME_MAX_LEN];
	char							EndBmp[SCROLL_BMP_NAME_MAX_LEN];
}MidTableScrollHeadSetting_t;

//滚动条中间部分设置
typedef struct MidTableScrollBarSetting
{
	int							BackColor;
	int							ForeColor;
	int							BarLength;
	int							BarPosition;
	Rect_t						BarBackRect;
	char							BarBmp[SCROLL_BMP_NAME_MAX_LEN];
	char							BarBackBmp[SCROLL_BMP_NAME_MAX_LEN];
}MidTableScrollBarSetting_t;

typedef struct MidTableScroll
{
	BOOL						bRowGraphic;		//是否贴图
	UI16							nDistance;			//滚动条离table的距离
	UI16							nWidth;				//滚动条的宽度
	MidTableScrollShadowSetting_t	ShadowSetting;		//阴影设置，对贴图模式无意义
	MidTableScrollHeadSetting_t		ScrollHeadSetting;	
	MidTableScrollBarSetting_t		ScrollBarSetting;		
}MidTableScroll_t;

/*******************************************************************************
 *****                           MidTableRow                               *****
 *******************************************************************************/
/* Name         : MidTableRowUpdate
 * Description  : update the table row features;
 * Parameters   :
 *          pRow        :[In] the row to initialize
 *          pRowObj     :[In] the underlying object
 *          pRowObjCat  :[In] the category of underlying object
 * Return       : succeed or not*/
bool MidTableRowUpdate( MidTableRow_t *pRow, MidTableRowObj_t *pRowObj, MidTableRowObjCat_t *pRowObjCat);

/* Name         : MidTableRowDraw
 * Description  : draw the table row according to the row feature
 * Parameters   :
 *          pRow    :[In] the row to draw
 * Return       : nothing*/
void MidTableRowDraw(MidTableRow_t *pRow);
        
/* Name         : MidTableRowGetFocus
 * Description  : the current row get the operation focus;
 * Parameters   :
 *          pRow    :[In]The row
 * Return       :succeed or not*/
bool MidTableRowGetFocus ( MidTableRow_t *pRow );

/* Name         : MidTableRowLoseFocus
 * Description  : the current row lose the operation focus;
 * Parameters   :
 *          pRow    :[In]The row
 * Return       :succeed or not*/
bool    MidTableRowLoseFocus ( MidTableRow_t *pRow );

/* Name         : MidTableRowSelected
 * Description  : the current row get checked;
 * Parameters   :
 *          pRow    :[In]The row
 * Return       :succeed or not*/
bool MidTableRowSelected( MidTableRow_t *pRow );

/* Name         : MidTableRowDeselected
 * Description  : the current row lose selected mark;
 * Parameters   :
 *          pRow    :[In]The row
 * Return       :succeed or not*/
bool MidTableRowDeselected( MidTableRow_t *pRow );

/* Name         : MidTableRowProcess
 * Description  : Processing the input message
 * Parameters   :
 *          pRow    :[In] the objective row
 *          pMsgIn  :[In] the incoming message
 *          pMsgOut :[In] the buffer for outgoing message
                     [Out]the outgoing message
 * Return       : processed or not*/
bool MidTableRowProcess (MidTableRow_t *pRow, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut, ControlList_t* pControlList );


/*******************************************************************************************/
typedef void    MidTableObj_t;
typedef void    MidTableInitParam_t;

/*The function for retrieving the title for the column*/
typedef HRESULT (*MidTableObjGetTitle)(UI32 nId, MidTableObj_t *pObj, int iCol, char *pText, int *pnLen);
/*The function for registering the access to underlying object, return the id in pnId*/
typedef HRESULT (*MidTableObjRegister)(MidTableObj_t *pObj, UI32 *pnId, MidTableInitParam_t* pInitParam);
/*The function for releasing the access to underlying object*/
typedef HRESULT (*MidTableObjRelease)(UI32 nId, MidTableObj_t *pObj);
/*The function for setting the index item for underlying object, 
  nId is not understandable by table, 0 is the default*/
typedef HRESULT (*MidTableObjIndexBy)(UI32 nId, MidTableObj_t *pObj, int n2Id);
/*The function for getting the number of object meeting condition after pRowObj(exclusive)*/
typedef int     (*MidTableObjGetCount)(UI32 nId, MidTableObj_t *pObj, MidTableRowObj_t * pRowObj, bool bNext);
/*The function for getting the sub objects' category*/
typedef MidTableRowObjCat_t* (*MidTableObjGetSubCat)();
/*The function for getting the current object meeting condition, 
  if pRowObj == NULL, return the current; if pRowObj != NULL and is valid, set it as the current*/
typedef MidTableRowObj_t* (*MidTableObjGetAt)(UI32 nId, MidTableObj_t *pObj, MidTableRowObj_t *pRowObj);
/*The function for getting the first object meeting condition*/
typedef MidTableRowObj_t* (*MidTableObjGetFirst)(UI32 nId, MidTableObj_t *pObj);
/*The function for getting the first object meeting condition*/
typedef MidTableRowObj_t* (*MidTableObjGetLast)(UI32 nId, MidTableObj_t *pObj);
/*The function for retrieving the next underlying object of pObj*/
typedef MidTableRowObj_t* (*MidTableObjGetNext)(UI32 nId, MidTableObj_t *pObj);
/*The function for retrieving the previous underlying object for pObj*/
typedef MidTableRowObj_t* (*MidTableObjGetPrev)(UI32 nId, MidTableObj_t *pObj);
/* 将pRowObj加入到pObj中iIndex的位置，若iIndex为-1，则插入到当前项之前；首项是0 */
typedef MidTableRowObj_t *(*MidTableObjAddItem)(UI32 nId, MidTableObj_t *pObj, MidTableRowObj_t *pRowObj, int iIndex, bool bDirector);
/* 从pObj中第iIndex项，如果iIndex为-1，则删除当前项；首项是0 */
typedef bool (*MidTableObjRemoveItem)(UI32 nId, MidTableObj_t *pObj, int iIndex);

typedef struct MidTableObjCat {
    MidTableObjGetTitle     GetTitle;
    MidTableObjRegister     Register;
    MidTableObjRelease      Release;
    MidTableObjIndexBy      IndexBy;
    MidTableObjGetCount     GetCount;
    MidTableObjGetSubCat    GetSubCat;
    MidTableObjGetAt        GetAt;
    MidTableObjGetFirst     GetFirst;
    MidTableObjGetLast     GetLast;
    MidTableObjGetNext      GetNext;
    MidTableObjGetPrev      GetPrev;
    MidTableObjAddItem		AddItem;
    MidTableObjRemoveItem   RemoveItem;
}MidTableObjCat_t;


/*******************************************************************************************/
typedef void (*MidTableDrawIcon)(struct ControlItem *this);
typedef struct MidTable {
    UI32                    nId;            /*The id for accessing underlying object*/
    MidTableRowSetting_t    RowSetting;     /*The generic setting of the table rows*/
    MidTableColSetting_t    ColSetting;     /*The generic col setting of the table*/
    UI16                    nRow;           /*The number of visible rows*///绑定数据的总数
    UI16                    nRowInTable;    /*The number of rows in one screen displaying
    										 *注意:取值范围是1-N */
    UI16                    nRowValid;      /*The number of rows valid in the view
    										 *注意:取值范围是1-nRowInTable*/
    MidTableRow_t           *pRows;         /*The pointer to the array of row information */
    int                     iFocusRow;      /*index of the focus row in visible region
    										 *注意:取值范围是0-(nRowValid-1) */
    MidTableObj_t           *pObj;          /*pointer to the underlieing item*/ 
    MidTableObjCat_t        *pObjCat;       /*The underlying category of the item*/
    MidTableRowObjCat_t     *pRowObjCat;    /*the underlying object category of the row item*/
    MidTableRowSetting_t    TitleSetting;   /*The setting of the title row*/
    MidTableScroll_t		*ScrollSetting;/*滚动条设置*/
    /*TableRow的图形属性*/
    BOOL					bRowGraphic;		//tablerow显示方式(是否以图形方式)				
    MidGraphicSettingList_t RowGraphicSetting;	//tablerow显示图片列表    
    UI16 RowSpace;//the space between rows
    
    MidTableDrawIcon DrawIcon;
} MidTable_t ;


UI16 MidTableGetVisibleRowNum(ControlItem_t *pTable);

bool MidTableRollUp(ControlItem_t *pTable);
bool MidTableRollDown(ControlItem_t *pTable);

bool MidTablePageUp(ControlItem_t *pTable);
bool MidTablePageDown(ControlItem_t *pTable);

void MidTableDrawTitle(ControlItem_t *pTable);
void MidTableDrawCol(ControlItem_t *pTable);
void MidTableInitScroll(ControlItem_t *pTable);
int MidTableGetRowNO(ControlItem_t *pTable, int iIndex);

/* 下列函数已失去作用 */
void MidControlTableHide(ControlItem_t *pTable);

void MidControlTableSetCat ( ControlCatInfo_t *pCatInfo );
/* Name         : MidControlTableCreate
 * Description  : create a table object;
 * Parameters   : 
 *          pControlList    :[In] the pointer to the control list,
 *          pstrTable       :[In] the string of control information
 *          nLen            :[In] the length of the information string
 * Return       : the pointer to the control item if create successfully, else NULL*/
ControlItem_t* MidControlTableCreate (ControlList_t* pControlList, char *pstrTable, int nLen );

/* Name         : MidTableDestroy 
 * Description  : Delete the table object, free the space occupied by the object;
 * Parameters   : 
 *      pTable      :[In] the table to destroy
 *      pControlList:[In] 
 * Return       : err code*/
HRESULT MidControlTableDestroy(ControlItem_t *pTable, ControlList_t* pControlList );

/* Name         : MidControlTableLoadFromString
 * Description  : load the generic information of control object from the string pointed by
                  pstrTable to the object pointed by pTable;
 * Parameters   : 
 *          pTable      :[In] pointer to the table to initiate
 *          pstrTable   :[In] the input string
 *          nLen        :[In] the stringlength
 *          pControlList:[In] the controllist for inserting the control
 * Return       :err code*/
HRESULT MidControlTableLoadFromString(ControlItem_t* pTable, char *pstrTable, int nLen, ControlList_t *pControlList );

/* Name         : MidControlTableInit
 * Description  : Initiate the table
 * Parameters   : 
 *          pTable      :[In]the table to initiate
 *          pObj        :[In]the underlying object
 *          pObjCat     :[In]the underlying object category
 *          pControlList:[In]
 * Return       : Succeed or not*/
bool MidControlTableInit( ControlItem_t* pTable, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam,  ControlList_t *pControlList);

/* Name         : MidControlTableUpdate
 * Description  : Update the contents of the table when the underlying object changed
 * Parameters   : 
 *          pTable      :[In]the table to initiate
 *          pObj        :[In]the underlying object
 *          pObjCat     :[In]the underlying object category, kept simply for compatibilty
 *          pControlList:[In]
 * Return       : error code*/
HRESULT MidControlTableUpdate(ControlItem_t* pTable, void *pObj, void *pObjCat, MidControlInitParam_t *pInitParam,  ControlList_t *pControlList);



/* Name         :MidControlTableGetFocus
 * Description  :the current table get the operating focus;
 * Parameters   :
 *          pTable      :[In] the table gaining focus
 * Return       :err code*/
HRESULT MidControlTableGetFocus (ControlItem_t *pTable);

/* Name         : MidControlTableLoseFocus
 * Description  : the table lose the operating focus;
 * Parameters   :
 *          pTable  :[In] the table
 * Return       : err code*/
HRESULT MidControlTableLoseFocus(ControlItem_t *pTable);

/* Name         : MidTableUpdateRows
 * Description  : Update the rows in the table
 * Parameters   :
 *          pTable      :[In] the table whose rows to initiate
 * Return       :nothing*/
void MidTableUpdateRows(ControlItem_t *pTable, int iNewFocusRow );

/* Name         : MidControlTableProcess
 * Description  : Processing the incoming message
 * Parameters   :
 *          pTable      :[In] the table to process the incoming message
 *          pMsgIn      :[In] the incoming message
 *          pMsgOut     :[In] the buffer to hold the outgoing message
 *                       [Out]the outgoing message
 * Return       : processed or not*/
bool MidControlTableProcess(ControlItem_t *pTable, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut, ControlList_t *pControlList );

/* Name         : MidControlTableDraw
 * Description  : draw the table with the table feature
 * Parameters   :
 *          pTable      :[In]the table to draw
 *          nStyle      :[In]the style for drawing
 * Return       : nothing*/
void MidControlTableDraw(ControlItem_t *pTable, int nStyle);

HRESULT MidTableRemoveItem(ControlItem_t *pTable, int iIndex);
HRESULT MidTableRemoveEndItem(ControlItem_t *pTable);
HRESULT MidTableAddItem(ControlItem_t *pTable, void *pRowObjData, int iIndex, bool bNext);
HRESULT MidTableRowRefresh(ControlItem_t *pTable, int iIndex);
HRESULT MidTableSetFocus(ControlItem_t *pTable, int iIndex);

HRESULT MidControlTableSetCallback(struct ControlItem *this, void *pFunc, int nType);

bool    MidTableRowDisableFocus ( MidTableRow_t *pRow );
void MidTableUpdateRowsNew(ControlItem_t *pTable);
void MidTableInitRows(ControlItem_t *pTable);

void MidTableGetCurrentPageStatus(ControlItem_t *pTable, MidTableRowObj_t **pPageFirstRowObj, int *pPageFocus);
void MidTableSetCurrentPageStatus(ControlItem_t *pTable, MidTableRowObj_t *pPageFirstRowObj, int PageFocus);
bool MidTableIsRollUp(ControlItem_t *pTable);
bool MidTableIsRollDown(ControlItem_t *pTable);
void MidTableSetRollforTooLong(BOOL value);

#endif
