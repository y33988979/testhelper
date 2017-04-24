/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stberror.h

    Description : 数字电视的应用
 
    Others:      
    
    History: 	1.  2004-3-15 yzb create
    			2.

*******************************************************************************/
#ifndef _ERR_CODE_
#define _ERR_CODE_

#define COMMON_BASE			0x00100000		/*The base code for generic error*/
#define APPMGR_BASE			0x00210000		/*the base code for application manager*/
#define APPMSG_BASE			0x00220000		/*The base code for messager engine*/
#define FILE_BASE			0x00230000		/*the base code for file system*/
#define SI_BASE				0x00240000		/*the base code for si processor*/
#define IC_BASE				0x00250000		/*the base code for ic card driver*/

#define CTRLMGR_BASE		0x00300000		/*the base code for control manager */
#define INVALID_INPUT		0x00300001		/*input data is invalid*/
#define END_ITEM			0x00300002      /*move to the end control list item*/

#define CONTROL_BASE		0x00400000
#define BUTTON_BASE			0x00410000		/*the base code for button control*/
#define CHECKBOX_BASE		0x00420000		/*the base code for checkbox control*/
#define RADIO_BASE			0x00430000		/*the base code for radio control*/
#define RADIOGROUP_BASE		0x00440000		/*the base code for radiogroup control*/
#define MENU_BASE			0x00450000		/*the base code for menu control*/
#define MENUITEM_BASE		0x00460000		/*the base code for menuitem control*/
#define TEXTAREA_BASE		0x00470000		/*the base code for textarea control*/
#define TEXTFIELD_BASE		0x00480000		/*the base code for textfield control*/
#define TEXTLABEL_BASE		0x00490000		/*the base code for textlabel control*/
#define GRID_BASE			0x00500000		/*the base code for grid control*/
#define TABLE_BASE			0x00510000		/*the base code for table control*/
#define BITMAP_BASE			0x00520000		/*the base code for bitmap control*/
#define CARET_BASE			0x00530000		/*the base code for caret control*/

#define EPG_BASE			0x00700000		/*the base code for epg application*/
#define EMAIL_BASE			0x00710000		/*the base code for email application*/
#define NEWS_BASE			0x00720000		/*the base code for news application*/
#define STBSET_BASE			0x00730000		/*the base code for tuner parameter setting*/
#define ACCOUNT_BASE		0x00740000		/*the base code for account management*/
#define ACCESS_MGR_BASE		0x00750000		/*the base code for access management*/
#define IMMESSAGE_BASE                  0x00760000      /*the base code for immessage application*/
#define REC_BASE                  0x00770000      /*the base code for immessage application*/
#define CSS_BASE                  0x00780000      /*the base code for immessage application*/


#define PARSER_BASE			0x00800000
#define FILTER_BASE			0x00820000

#define CHNLSET_BASE			0x00830000

#define UPGRADE_BASE			0x00840000


/*----------Common error code-------------*/
#define OPERATION_SUCCEED	0
#define OPERATION_FAIL		1
#define MEMORY_ERR			COMMON_BASE+1
//#define INVALID_ITEM		COMMON_BASE+2
#define INSUFFICIENT_MEM	COMMON_BASE+3

#define FOCUS_SWITCHED		CONTROL_BASE+1


#endif	/* _ERR_CODE_ */


