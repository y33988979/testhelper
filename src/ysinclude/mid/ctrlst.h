
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



/*=================��������=======================*/
/*----------�ؼ��趨---------*/
#define ITEM_ENABLED					0x00000001	/*�ؼ�������*/
#define ITEM_VISIBLE						0x00000002	/*�ؼ��ɼ�*/
#define ITEM_INFOCUS					0x00000004	/*�ؼ�ӵ�н���*/
#define HIDEPARENT_ON_VISIBLE			0x00000008	/*��ʾ�ӿؼ������ظ��ؼ�*/

#define SHOW_SON						0x00000010	/*��ʾ���ؼ��������ӿؼ�*/
#define SHOWSON_ON_SINGLEPANEL		0x00000020	/*��ͬһ���������ʾ�����ӿؼ�*/
#define SHOW_ALL_ABOVE_LEVEL			0x00000040	/*��ʾ�����ϲ�εĿؼ�*/
#define ITEM_COMMAND					0x00000080	/*�ؼ���������Ŀ��*/

#define ITEM_STATUS						0x00000100	/*�ؼ����ڱ�ʾ״̬*/
#define ITEM_CHECKED					0x00000200	/*�ؼ���ѡ��*/
#define ITEM_SUBVISIBLE					0x00000400	/*�ؼ����ӿؼ��ɼ�*/
#define ITEM_LEAF						0x00000800	/*�ؼ���һ��Ҷ�ӿؼ�*/

//#define ITEM_SUBINFOCUS				0x00001000	/*�ؼ����ӿؼ�ӵ�е�ǰ����*/
#define ITEM_PASSWORD					0x00001000	/*�������ı����������Ϊ����*/
#define ITEM_SHOWSONONFOCUS			0x00002000	/*���ؼ�ӵ�н���ʱ��ʾ���ӿؼ�*/
#define ITEM_HORIZON					0x00004000	/*�ؼ���ˮƽ���г�*/
#define ITEM_WITHOUTFOCUS				0x00008000	/*�ؼ�������ӵ�н���*/

#define ITEM_RECORD						0x00010000	/*the items are selected to be recorded*/
#define ITEM_RING						0x00020000  /*�ؼ��������ܹ�ѭ����ʾ(table�ؼ�ר��)*/
#define ITEM_NOBG						0x00040000  /*����ʾ�ؼ��ı���(��Ϊtextlabel�ؼ�ר��)*/
#define ITEM_PAGE_TABLE				0x00080000  /*ҳ�׽ڵ�̶�ģʽ(table�ؼ�ר��)*/

#define ITEM_PAGE_TABLE_FOCUS_0		0x00100000  /*ҳ�׽ڵ�̶�ģʽ�·�ҳ�Ľ����Ƿ�̶��ڵ�0��(table�ؼ�ר��)*/

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

#define REFRESH_SHOW			0x00001000	/*ˢ����ʾ�����ؼ���Setting�����ı�ʱ�ػ�*/


/*---------�ؼ����ͳ���---------*/
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

/*�ؼ��������*/
enum ControlErrorCode{
	CTRL_MEMORY_ERR = CONTROL_BASE + 1,
	CTRL_POINTER_ERR,
	CTRL_FUNC_STYLE_ERR,
	INVALID_ITEM,
	INVALID_LIST,
	MSGBOX_CREATE_ERROR,
	MSGBOX_ERROR
};

/*�ؼ���������*/
enum ControlFuncType{
	ENTER = 1,
	LEAVE,
        DRAWICON,//table,txtareaʹ�ã��ڿɷ�ҳʱ����ͷ
	PREVPROC,
	FOLLOWPROC
};




typedef struct Language
{
  void *Language_Type[2];
}Language_t;


/*----------����------------*/
#define FIELD_SEP			"[/]"	/*�ַ����и�������ķָ���*/
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

/*һЩ�����ȡֵ������������ĺ��壬������
nLeft��
	-1: Always meets the left border of the parent display
	-2: Always meets the left control in the same panel
	other: Locate the contorl at the specified position of the grid
nTop��
	-1: Always meets the top border of the parent display
	-2: Always meets the top control in the same panel
	other: Locate the contorl at the specified position of the grid 
nRight��
	-1: Always meets the right border of the parent display
	-2: Always meets the right control in the same panel
	other: Locate the contorl at the specified position of the grid
nBottom��
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
	char					*pFileName;		//ͼƬ���ļ���
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

/*�ؼ��ķ���*/
/*
 * Function:	MidControlSetCallback
 * Description:	���ÿؼ���Ӧ�¼�����������nStyle�����ͣ���pFuncָ��ĺ���������Ӧ����Ӧ�¼��ĺ���ָ��
 * Calls:       ��
 * Called By:      
 * Input:       struct ControlItem *this	�˿ؼ������ָ��
 *              void *pFunc					Ӧ����Ӧ�¼�����ָ��
 *				int nType					��������
 * Output:      ��
 * Return:      �����Ƿ�ɹ�
 * Others:         	
 */
typedef HRESULT (*MidControlSetCallback)(struct ControlItem *this, void *pFunc, int nType);

/*�ؼ���Ӧ�¼��Ļص�����*/
/*
 * Function:	MidControlEnter
 * Description:	�������ƶ����˿ؼ�ʱ�������¼�
 * Calls:       ��
 * Called By:      
 * Input:       struct ControlItem *this	�˿ؼ������ָ��
 * Output:      ��
 * Return:      ��
 * Others:         	
 */
typedef HRESULT (*MidControlEnter)(struct ControlItem *this);

/*
 * Function:	MidControlLeave
 * Description:	�������뿪�˿ؼ�ʱ�������¼�
 * Calls:       ��
 * Called By:      
 * Input:       struct ControlItem *this	�˿ؼ������ָ��
 * Output:      ��
 * Return:      ��
 * Others:         	
 */
typedef HRESULT (*MidControlLeave)(struct ControlItem *this);

/*
 * Function:	MidControlNonModelDialog
 * Description:	���ڷ�ģ̬�Ի������Ϣ�Ĵ���
 * Calls:       ��
 * Called By:      
 * Input:       pMsg    ������Ϣ
 * Output:      ��
 * Return:      ��ģ̬�Ի����Ƿ����Ϣ������.
 * Others:         	
 */
typedef BOOL (*MidControlNonModelDialog)(MessageInfo_t *pMsg);



/*----------------------ControlItem-------------------*/
/*�ؼ������е�ÿ���ؼ�����һ��"�ؼ�����������ṹ"����*/

typedef bool (*MidControlUserProcess)(struct ControlItem *pItem, MessageInfo_t *pMsgIn, MessageInfo_t *pMsgOut, bool *pFlag);

typedef struct ControlItem {
	CONTROL_ID				nId;			//�ؼ���Ψһ��ʶ��
	struct ControlCatInfo	*pCatInfo;		//�ؼ�������Ϣָ��
	ControlInfo_t			*pInfo;			//�ؼ���Ϣָ��
	KEY_CODE				nKeyCode;		//�ؼ���ݼ���
	int						iLevel ;		//�ؼ������Ĳ��;        
	Rect_t					rcControl;		//�ؼ���λ��
	UI32					nSetting;		//�ؼ���ǰ״̬
	UI32					nPreSetting;	//�ؼ���ǰһ״̬
	UI32					nOrgSetting;	//�ؼ�ȱʡ״̬
	int						iRadius;
	int					iTextCount;
	union Textbody{
		char				*pLabel;		//��ʾ���ַ���
		char				*pBody;			//�ؼ��ı�����
	}*TextInfo;
	int						nLen;			//�ı�����������
	MidMsgSettingList_t		msgList;		//�ؼ���Ӧ��Ϣ�趨 
	MidTextSettingList_t	TextSetting;	//�ı�����趨
	MidLineSettingList_t	LineSetting;	//�߿����趨	
	MidDrawSettingList_t	*pDrawSetting;	//�ؼ������⻭��
	MidLineStyle_t			*pFocusLine;	//�ؼ��Ľ����߷���趨
	MidShadowStyle_t		*pShadow;		//�ؼ�����Ӱ����趨
	struct ControlItem		*pNext;			//ָ����һ���ؼ���ָ��
	struct ControlItem		*pPrev;			//ָ����һ���ؼ���ָ��

	BOOL					bGraphic;		//�ؼ���ʾ��ʽ(�Ƿ���ͼ�η�ʽ)				
	MidGraphicSettingList_t GraphicSetting;	//�ؼ���ʾͼƬ�б�
	MidControlSetCallback	SetCallback;	

	MidControlEnter			Enter;		
	MidControlLeave			Leave;

   MidControlUserProcess PrevProcess;
   MidControlUserProcess FollowProcess;
	
} ControlItem_t;

/*------------ControlList---------------*/
/*�����������Ϣ��"�ؼ�������Ϣ�ṹ"����*/
typedef struct ControlList {
	int					nID;				 
	UI32				nSetting;			
	UI32				nPrevID;			
	struct ControlList	*pNext;				 
	struct ControlList	*pParent;			
	UI32				nItem;				//�����е���������Ŀ
	DRVOS_Partition_t			*ptnApp;	//��������ڴ����÷���
	struct ControlCatList *pControlCatList; //�ؼ���������ָ��
	ControlItem_t		*pHead;				//������ָ��
	ControlItem_t		*pCurrent;			//��ǰ������ָ��
	ControlItem_t		*pEnd;				//����βָ��
	ControlItem_t		*pFocus;			//��ǰ����ؼ�ָ��
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
/*ÿ�����͵Ŀؼ���ͨ��"�ؼ�������Ϣ�ṹ"������*/
typedef struct ControlCatInfo {
	CONTROL_CAT_ID	nCatId;				//Ψһ�ؼ����ͱ�ʶ��
	struct ControlCatInfo *pNext;		//ָ����һ���ṹ��ָ��
	struct ControlCatInfo *pPrev;		//ָ��ǰһ���ṹ��ָ��
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
/*���У�
	MidControlCreate()�������ؼ�ʵ������ʼ������洢����ע��ؼ�ʵ������������ɫ�塣��ÿ���ؼ����д�����ɾ�������������������ڴ���䡢��ɫ���һ�����⡣��
	MidControlDestroy()��ɾ���ؼ�ʵ�������ռ�ö����������ע����Ŀ���ָ���ɫ�塣
	MidControlProcess()�����������Ϣ��������ͬ������Ϣ�ķ�ʽ������������ڵĻ�����
	MidControlDraw()����ָ���ķ�ʽ��ʾ�ؼ���
	MidControlLoadFromString()����pstrFeatureָ��ĳ�ΪnLen���ַ����ж�ȡ�ؼ�������Ϣ������pFeaturesָ������ݽṹ
	MidControlRestoreSetting()���ָ��ؼ����趨Ϊȱʡ�趨
	MidControlLoseFocus()���ؼ�ʧȥ����
	MidControlGetFocus()���ؼ���ý���
	MidControlCopy()���ƿؼ���Ϣ������һ����ͬ�Ŀؼ�
*/


/*---------------------------ControlCatList-------------------------*/
/*��Щ�ؼ�������Ϣ�ṹ��ϵͳ��Ҳͨ�������ڿؼ�ʵ������ķ�ʽ������֯*/
typedef struct ControlCatList {
	ControlCatInfo_t		*pCats;			//������ָ��
	int 					nCat;			//��Ŀ
} ControlCatList_t;

/*----------------AppGui----------------*/
/*Ӧ�ý������ݽṹ*/
typedef struct AppGui{
	ControlCatList_t		*pCatList;				//�ؼ������б�
	ControlList_t			*pControlList;			//�ؼ��б�
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
���룺�ؼ��б�,Ҫɾ���Ŀؼ��ṹָ��
�����������Ϣ
˵����ɾ����ǰ�����ͬʱ��������ָ�롣�����ǰ�ؼ�Ϊ����ĩβ����ǰָ��ָ���������ĩβ��
*/
HRESULT ControlListDeleteItem(ControlList_t* pControlList, ControlItem_t *pDeleteControl);
HRESULT ControlListDeleteCurrent(ControlList_t* pControlList);
ControlItem_t* ControlListSetCurrentFocus(ControlList_t * pControlList);

/*�����ؼ�����ĺ���*/
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
˵�����ƶ���ָ���ؼ������ѡ���ƶ����㣬�����ƶ���ǰָ���ͬʱ�ƶ��ؼ�����
���룺�ؼ��б��ؼ��ṹָ�룬�Ƿ��ƶ�����
�����
����: �ؼ�ָ��
����������ƶ����㣬��ÿؼ�������Եõ����㣬���ÿؼ����ܵõ����㣬���ƶ�
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
˵��������һ���ؼ���
���룺�ؼ�������Ϣ�ṹ����ָ��
�����ָ��ؼ���ָ��
*/

ControlItem_t* ControlItemCreate(ControlList_t * pControlList);

/*----------ControlListSetCat----------*/
/*
˵�����趨�ؼ���������Ϣ
���룺�ؼ���������ָ�룬�ؼ��ṹָ�롢�ؼ�����
������������
*/
HRESULT ControlListSetCat(ControlList_t* pControlList,ControlItem_t* pControlItem, UI16 nCat);
/*----------ControlListSetID----------*/
/*
˵�����趨�ؼ���ID
���룺�ؼ���������ָ�룬�ؼ��ṹָ��
������������
*/
HRESULT ControlListSetID(ControlList_t* pControlList, ControlItem_t* pControlItem);
/*----------ControlCat----------*/
/*
˵�������ؼ������ַ���ת���ɿؼ�����
���룺�ؼ������ַ���
������ؼ�����
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
 * Description	: �жϸÿؼ��Ƿ��ܹ��õ�����
 * Parameters	: ControlItem_t *pControlItem �ؼ����
 * Return		: false:���ܵõ�����;true:���Եõ�����
 * Other		: ͨ���жϿؼ�������ITEM_VISIBLE��ITEM_WITHOUTFOCUS
 */
BOOL ControlItemIsCanGetFocus(ControlItem_t *pControlItem);

/* Name			: ControlItemGetFocus
 * Description	: ���øÿؼ���GetFocus��������Ӧ�ÿؼ���Leave�¼�
 * Parameters	: ControlItem_t *pControlItem �ؼ����
 * Return		: ��
 * Other		: 
 */
void ControlItemGetFocus(ControlItem_t *pControlItem);

/* Name			: ControlItemLoseFocus
 * Description	: ���øÿؼ���GetFocus��������Ӧ�ÿؼ���Leave�¼�
 * Parameters	: ControlItem_t *pControlItem �ؼ����
 * Return		: ��
 * Other		: 
 */
void ControlItemLoseFocus(ControlItem_t *pControlItem);

/* Name			: ControlListSetSetting
 * Description	: ����ControlList��nSettingֵ
 * Parameters	: ControlItem_t *pControlItem 	�ؼ�����ָ��
 *				  UI32			nSetting		Ҫ���õ�����
 * Return		: ��
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
