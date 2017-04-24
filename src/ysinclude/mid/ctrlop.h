
#ifndef _CONTROL_OP_H_
#define _CONTROL_OP_H_

#include "stbtype.h"
//#include "common.h"
#include "ctrlst.h"
#include "stberror.h"
#include "guitk.h"
#include "driver.h"

/*=======Load variable for specified string========*/
/*---------------LoadUI32---------------------*/
/*	说明：从指定长度的字符串中读取UI32类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadUI32(UI32 *pUI, char *pstr, int nLen);

/*---------------LoadUI16---------------------*/
/*	说明：从指定长度的字符串中读取UI16类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadUI16(UI16 *pUI, char *pstr, int nLen);

/*----------------LoadInt---------------------*/
/*	说明：从指定长度的字符串中读取int类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadInt(int *pInt, char *pstr, int nLen);

/*---------------LoadRect---------------------*/
/*	说明：从指定长度的字符串中读取Rect类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadRect(Rect_t *pRect, char *pstr, int nLen);


/*---------------LoadSize---------------------*/
/*	说明：从指定长度的字符串中读取Size类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadSize(Size_t *pSize, char *pstr, int nLen);

/*---------------LoadString---------------------*/
/*	说明：从指定长度的字符串中读取string类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadString(char *pstrDest, int nDest, char *pstr, int nLen);

/*---------------IDtoChar-------------------*/
/*	Description:Convert the id of input message to character
	input:message id
	Output:corresponding character
	*/
char IDtoChar(unsigned int nId, bool *bTextInputMode, char *InputMap[]);
void MidLoadUIChar(BOOLEAN *pChar, char *pstr, int nLen);

/*---------------LoadLineStyle---------------------*/
/*	说明：从指定长度的字符串中读取MidLineStyle类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadLineSetting(MidLineSetting_t *pSetting, char *pstr, int nLen);
void MidLoadLineSettingList(MidLineSettingList_t *pList, char *pstr, int nLen, ControlList_t *pControlList);

/*---------------LoadTextStyle---------------------*/
/*	说明：从指定长度的字符串中读取MidTextStyle类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadTextSetting(MidTextSetting_t *pSetting, char *pstr, int nLen);
void MidLoadTextSettingList(MidTextSettingList_t *pList, char *pstr, int nLen, ControlList_t *pControlList);

/*---------------LoadMsgStyle---------------------*/
/*	说明：从指定长度的字符串中读取MidMsgStyle类型的变量
	输入：目标变量指针、字符串指针、字符串长度
	输出：无
*/
void MidLoadMsgSetting(MidMsgSetting_t *pSetting, char *pstr, int nLen);
void MidLoadMsgSettingList(MidMsgSettingList_t *pList, char *pstr, int nLen, ControlList_t *pControlList);
void MidLoadScrollSetting(MidTable_t *pFeature, char *pstr, int nLen,ControlList_t *pControlList);
void MidLoadScrollBmp(char *pBmpName, char *pstr, int nLen);

/*---------------LoadDefaultStyle---------------------*/
/*	说明：手动设定Default style的值
	输入：目标变量指针、目标类型
	输出：无
*/
void MidLoadDefaultTextSettingList(MidTextSettingList_t *pList, CONTROL_CAT_ID	nCatId, ControlList_t *pControlList);
void MidLoadDefaultLineSettingList(MidLineSettingList_t *pList, CONTROL_CAT_ID	nCatId, ControlList_t *pControlList);
void MidLoadDefaultMsgSettingList(MidMsgSettingList_t *pList, CONTROL_CAT_ID	nCatId, ControlList_t *pControlList);

/* Name			: MidResetLineSetting
 * Description	: Reset the line setting list of the control
 * Input		: pointer to the list
 * Output		: nothing*/
void MidResetLineSetting(MidLineSettingList_t *pLineSetting);

/* Name			: MidResetTextSetting
 * Description	: Reset the text setting list of the control
 * Input		: pointer to the list
 * Output		: nothing*/
void MidResetTextSetting(MidTextSettingList_t *pTextSetting);

/* Name			: MidResetMsgSetting
 * Description	: Reset the message setting list of the control
 * Input		: pointer to the list
 * Output		: nothing*/
void MidResetMsgSetting(MidMsgSettingList_t *pMsgSetting);

/* Name			: MidResetSetting
 * Description	: Reset the setting lists of the control
 * Input		: pointer to the control item 
 * Output		: nothing*/
void MidResetSetting(ControlItem_t *pItem);

/* Name			: MidDestroySetting
 * Description	: Delete the setting lists of the control
 * Input		: pointer to the control
 * Output		: void*/
void MidDestroySetting(ControlItem_t *pItem, DRVOS_Partition_t *ptnApp);

//void MidLoadIntArray(int *pIntArray, char *pstr, int nLen);

void MidLoadLineStyle(MidLineStyle_t *pLineStyle, char *pstr, int nLen);
void MidLoadTriangleStyle(TriangleStyle_t *pTriangleStyle, char *pstr, int nLen);
void MidLoadTxterStyle(TEXT_TexterStyle_t *pTxterStyle, char *pstr, int nLen);

/* Name			: MidControlGetTextStyle
 * Description	: Get the text output style for the current control setting
 * Parameters	:
 *			pItem		:[In] pointer to the control item
 * Return		: pointer to the style structure */
MidTextStyle_t * MidControlGetTextStyle(ControlItem_t*pItem);

/* Name			: MidControlGetLineStyle
 * Description	: Get the Line output style for the current control setting
 * Parameters	:
 *			pItem		:[In] pointer to the control item
 * Return		: pointer to the style structure */
MidLineStyle_t * MidControlGetLineStyle(ControlItem_t*pItem);

/* Name			: MidControlSetOutMsg
 * Description	: Set the output message when nEvent happens on pItem
 * Parameters	:
 *			pItem	:[In] pointer to the control item
 *			pMsgOut	:[In] pointer to the output message structure
 *					 [Out]output message
 *			nEvent	:[In] the id of the event happens
 * Return		: Valid message generated or not*/
BOOLEAN MidControlSetOutMsg(ControlItem_t *pItem, MessageInfo_t *pMsgOut, UI32 nEvent);

void MidLoadTextStyle(MidTextStyle_t *pTextStyle, char *pstr, int nLen);


/* Name			: MidGetTextStyle
 * Description	: Get the matching text style for the specified setting
 * Parameters	:
 *			pTextSettings	:[In] pointer to the setting list
 *			nSetting		:[In] item status
 * Return		: the textstyle got*/
MidTextStyle_t* MidGetTextStyle(MidTextSettingList_t *pTextSettings, UI32 nSetting);

/* Name			: MidGetLineStyle
 * Description	: Get the matching line style for the specified setting
 * Parameters	:
 *			pLineSettings	:[In] pointer to the setting list
 *			nSetting		:[In] item status
 * Return		: the linestyle got*/
MidLineStyle_t* MidGetLineStyle(MidLineSettingList_t *pLineSettings, UI32 nSetting);

/* Name			:MidLoadUI16Array
 * Description	:Load UI16 array from the string
 * Input		:pointer to the array, pointer to the header of string, string length, number of ui16
 * Output		:array loaded	*/
void MidLoadUI16Array(UI16 *pArray, char *pstr, int nLen, int nUI);

/* Name			:MidLoadStringArray
 * Description	:Load String array from the string
 * Input		:pointer to the array, pointer to the header of string, string length, number of string
 * Output		:array loaded	*/
void MidLoadStringArray(char **pArray, char *pstr, int nLen, int nUI);

/* Description	:Load GraphicSettingList from the string
 * Input		:pointer to the list, pointer to the header of string, string length
 * Output		:structure loaded	
 */
void MidLoadGraphicSettingList(MidGraphicSettingList_t *pList, char *pstr, int nLen,ControlList_t *pControlList);

/* Description	:Load GraphicSetting structure from the string
 * Input		:pointer to the structure, pointer to the header of string, string length
 * Output		:structure loaded	*/
void MidLoadGraphicSetting(MidGraphicSetting_t *pSetting, char *pstr, int nLen);

char* MidControlGetGraphicName(ControlItem_t *pItem, int nStyle);
#endif
