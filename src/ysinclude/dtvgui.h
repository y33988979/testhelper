/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dtvgui.h

    Description : DTV��ʾ����

    Others:      

    History: 	1. 2004-3-15 yzb create
    			2. 2011-11-11 xyan Modify

*******************************************************************************/
#ifndef _DTVGUI_H
#define _DTVGUI_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "digitv.h"    

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define DTV_RGN_MIX_WEIGHT              	63

/*DTVRegion��С����*/
#define DTV_RGN_POS_X						0
#define DTV_RGN_POS_Y                   	0
#define DTV_RGN_WIDTH                   	1920
#define DTV_RGN_HEIGHT                  	1080

/*DTVRegion��������ƫ��������*/
#define DTV_RGN_X_OFFSET                  	0
#define DTV_RGN_Y_OFFSET                  	0

/*����Ƶ����λ�ü���С*/
#define DTV_CHNL_NUM_WIDTH					400
#define DTV_CHNL_NUM_HEIGHT             	160
#define DTV_CHNL_NUM_X                  	(DTV_RGN_WIDTH - DTV_CHNL_NUM_WIDTH) 
#define DTV_CHNL_NUM_Y                  	(120 - DTV_RGN_Y_OFFSET)
#define DTV_NULL_CHNL_Y_OFFSET				70

/*����DTV��ʾ��Ϣλ�ü���С*/
#define DTV_NOTE_X							580
#define DTV_NOTE_Y                     		364
#define DTV_NOTE_WIDTH                     	750 
#define DTV_NOTE_HEIGHT                    	423
#define DTV_NOTE_CONTENT_X					DTV_NOTE_X
#define DTV_NOTE_CONTENT_Y                  DTV_NOTE_Y+82
#define DTV_NOTE_CONTENT_WIDTH              740 
#define DTV_NOTE_CONTENT_HEIGHT             331

/*����DTV������λ�ü���С*/
#define DTV_VOLUME_X      		            180
#define DTV_VOLUME_Y      		            800
#define DTV_VOLUME_WIDTH      		        1560
#define DTV_VOLUME_HEIGHT     		        200

//����������ͼƬ��λ��
#define DTV_VOLUME_PIC_X                    (DTV_VOLUME_X + 140)
#define DTV_VOLUME_PIC_Y                    (DTV_VOLUME_Y +23)
#define DTV_VOLUME_PIC_WIDTH                70
#define DTV_VOLUME_PIC_HEIGHT               70

//����������ģʽ
#define DTV_VOLUME_MODE_X                   (DTV_VOLUME_PIC_X+100)
#define DTV_VOLUME_MODE_Y                   (DTV_VOLUME_Y+48)
#define DTV_VOLUME_MODE_WIDTH               150
#define DTV_VOLUME_MODE_HEIGHT              40

//���������ڵ�λ��
#define DTV_VOLUME_WORD_X                   (DTV_VOLUME_MODE_X+980) 
#define DTV_VOLUME_WORD_Y                   (DTV_VOLUME_Y+48)
#define DTV_VOLUME_WORD_WIDTH               40
#define DTV_VOLUME_WORD_HEIGHT              40

//������С���ֵ���ʾλ��
#define DTV_VOLUME_DIGIT_X                  (DTV_VOLUME_WORD_X +100)
#define DTV_VOLUME_DIGIT_Y                  (DTV_VOLUME_Y+50)
#define DTV_VOLUME_DIGIT_WIDTH              80
#define DTV_VOLUME_DIGIT_HEIGHT             40

//��������ʼλ��
#define DTV_VOLUME_START_X                  (DTV_VOLUME_X+213)
#define DTV_VOLUME_START_Y                  (DTV_VOLUME_PIC_Y+83)
#define DTV_VOLUME_BAR_WIDTH                27
#define DTV_VOLUME_BAR_HEIGHT               61

/*����DTV��Ϣ��λ�ü���С*/
#define DTV_BANNER_POS_X                     180
#define DTV_BANNER_POS_Y                     795
#define DTV_BANNER_WIDTH                     1560
#define DTV_BANNER_HEIGHT                    260

//BannerƵ����λ��
#define DTV_PRGINFO_CHNLNUM_POS_X   	    (DTV_BANNER_POS_X + 40)
#define DTV_PRGINFO_CHNLNUM_POS_Y   	    (DTV_BANNER_POS_Y + 30)
#define DTV_PRGINFO_CHNLNUM_WIDTH   	     105
#define DTV_PRGINFO_CHNLNUM_HEIGHT          52

//Banner�Ͻ�Ŀ����λ��
#define DTV_PRGINFO_SERVICENAME_POS_X  	    (DTV_PRGINFO_CHNLNUM_POS_X + DTV_PRGINFO_CHNLNUM_WIDTH)
#define DTV_PRGINFO_SERVICENAME_POS_Y  	    (DTV_BANNER_POS_Y + 30)
#define DTV_PRGINFO_SERVICENAME_WIDTH  	    375
#define DTV_PRGINFO_SERVICENAME_HEIGHT       40

//Banner������λ��
#define DTV_BANNER_PIC_X                    (DTV_PRGINFO_SERVICENAME_POS_X+DTV_PRGINFO_SERVICENAME_WIDTH)
#define DTV_BANNER_PIC_Y                    (DTV_BANNER_POS_Y + 29)
#define DTV_BANNER_PIC_WIDTH                40
#define DTV_BANNER_PIC_HEICHT               40

//Banner������λ��
#define DTV_BANNER_MODE_X                   (DTV_BANNER_PIC_X+50)
#define DTV_BANNER_MODE_Y                   (DTV_BANNER_POS_Y + 30)
#define DTV_BANNER_MODE_WIDTH               40
#define DTV_BANNER_MODE_HEICHT              40

//Banner�ϼ���ͼƬλ��
#define DTV_BANNER_LOCK_X                   (DTV_BANNER_MODE_X+255)
#define DTV_BANNER_LOCK_Y                   (DTV_BANNER_MODE_Y-1)
#define DTV_BANNER_LOCK_WIDTH                90
#define DTV_BANNER_LOCK_HEIGHT               41

//Banner��ϲ��ͼƬλ��
#define DTV_BANNER_FAVORITE_X               (DTV_BANNER_LOCK_X+100)
#define DTV_BANNER_FAVORITE_Y               (DTV_BANNER_LOCK_Y+2)
#define DTV_BANNER_FAVORITE_WIDTH            90
#define DTV_BANNER_FAVORITE_HEIGHT           39

//Banner�ϵ�ǰʱ��λ��
#define DTV_PRGINFO_TIME_POS_X  		    (DTV_BANNER_FAVORITE_X+209)
#define DTV_PRGINFO_TIME_POS_Y  		    (DTV_BANNER_POS_Y + 30)
#define DTV_PRGINFO_TIME_WIDTH   		    500
#define DTV_PRGINFO_TIME_HEIGHT  		    40

//Banner�Ͽ�ʼ�ͽ���ʱ���λ��
#define DTV_BANNER_START_TIME_X             (DTV_BANNER_POS_X + 40)
#define DTV_BANNER_START_TIME_Y             (DTV_PRGINFO_CHNLNUM_POS_Y+DTV_PRGINFO_CHNLNUM_HEIGHT)
#define DTV_BANNER_START_TIME_WIDTH         120
#define DTV_BANNER_START_TIME_HEIGHT        63
#define DTV_BANNER_END_TIME_X               (DTV_BANNER_START_TIME_X+1269)
#define DTV_BANNER_END_TIME_Y               (DTV_PRGINFO_CHNLNUM_POS_Y+DTV_PRGINFO_CHNLNUM_HEIGHT)
#define DTV_BANNER_START_END_WIDTH          106
#define DTV_BANNER_START_END_HEIGHT         63

//Banner�Ͻ�������ɫ������λ��
#define DTV_BANNER_PROCESS_X                (DTV_BANNER_START_TIME_X+275)
#define DTV_BANNER_PROCESS_Y                (DTV_PRGINFO_CHNLNUM_POS_Y+DTV_PRGINFO_CHNLNUM_HEIGHT-12)
#define DTV_BANNER_PROCESS_WIDTH             930
#define DTV_BANNER_PROCESS_HEIGHT            65

//Banner�Ͻ�����λ��
#define DTV_BANNER_PROCESS_START_X          (DTV_BANNER_PROCESS_X+15)
#define DTV_BANNER_PROCESS_START_Y          (DTV_PRGINFO_CHNLNUM_POS_Y+DTV_PRGINFO_CHNLNUM_HEIGHT+15)
#define DTV_BANNER_PROCESS_START_WIDTH       900
#define DTV_BANNER_PROCESS_START_HEIGHT      11

//Banner�ϵ�ǰ��Ŀ
#define DTV_PRGINFO_PRG_TIME_POS_X  	    (DTV_BANNER_POS_X + 40)
#define DTV_PRGINFO_PRG_TIME_POS_Y  	    (DTV_BANNER_START_TIME_Y + DTV_BANNER_START_TIME_HEIGHT)
#define DTV_PRGINFO_PRG_TIME_WIDTH   	    480
#define DTV_PRGINFO_PRG_TIME_HEIGHT  	     45

#define DTV_PRGINFO_PRG_NAME_POS_X  	     (DTV_PRGINFO_PRG_TIME_POS_X + DTV_PRGINFO_PRG_TIME_WIDTH)
#define DTV_PRGINFO_PRG_NAME_POS_Y  	    (DTV_BANNER_START_TIME_Y + DTV_BANNER_START_TIME_HEIGHT)
#define DTV_PRGINFO_PRG_NAME_WIDTH  	     500
#define DTV_PRGINFO_PRG_NAME_HEIGHT  	    40

//Banner�Ϻ�����Ŀ
#define DTV_PRGINFO_FLW_TIME_POS_X  	    (DTV_BANNER_POS_X + 40)
#define DTV_PRGINFO_FLW_TIME_POS_Y  	    (DTV_PRGINFO_PRG_TIME_POS_Y+DTV_PRGINFO_PRG_TIME_HEIGHT)
#define DTV_PRGINFO_FLW_TIME_WIDTH   	    320
#define DTV_PRGINFO_FLW_TIME_HEIGHT  	     45

//ԤԼ��ʾ��λ��
#define DTV_PRGINFO_BOOK_POS_X  	        (DTV_PRGINFO_FLW_TIME_POS_X+DTV_PRGINFO_FLW_TIME_WIDTH)
#define DTV_PRGINFO_BOOK_POS_Y  	        (DTV_PRGINFO_PRG_TIME_POS_Y+DTV_PRGINFO_PRG_TIME_HEIGHT-10)
#define DTV_PRGINFO_BOOK_WIDTH  	        160
#define DTV_PRGINFO_BOOK_HEIGHT  	        40

#define DTV_PRGINFO_FLW_NAME_POS_X  	    (DTV_PRGINFO_BOOK_POS_X+DTV_PRGINFO_BOOK_WIDTH)
#define DTV_PRGINFO_FLW_NAME_POS_Y  	    (DTV_PRGINFO_PRG_NAME_POS_Y+DTV_PRGINFO_PRG_NAME_HEIGHT)
#define DTV_PRGINFO_FLW_NAME_WIDTH  	     500
#define DTV_PRGINFO_FLW_NAME_HEIGHT  	     40

//��Ŀ�����ʾ��λ��
#define DTV_PROGRAM_INFO_POS_X  	        60
#define DTV_PROGRAM_INFO_POS_Y  	        310
#define DTV_PROGRAM_INFO_WIDTH  	        500
#define DTV_PROGRAM_INFO_HEIGHT  	        600

//��Ŀ�������λ��
#define DTV_PROGRAM_INFO_TITLE_X  	        (DTV_PROGRAM_INFO_POS_X+10)
#define DTV_PROGRAM_INFO_TITLE_Y  	        (DTV_PROGRAM_INFO_POS_Y+20)
#define DTV_PROGRAM_INFO_TITLE_WIDTH  	    480
#define DTV_PROGRAM_INFO_TITLE_HEIGHT  	    68

//��Ŀ������ݵ�λ��
#define DTV_PROGRAM_INFO_CONTENT_X  	    (DTV_PROGRAM_INFO_TITLE_X+30)
#define DTV_PROGRAM_INFO_CONTENT_Y  	    (DTV_PROGRAM_INFO_TITLE_Y+DTV_PROGRAM_INFO_TITLE_HEIGHT)
#define DTV_PROGRAM_INFO_CONTENT_WIDTH  	420
#define DTV_PROGRAM_INFO_CONTENT_HEIGHT  	472

/*����IC��ID����ʾλ�ü���С*/
#define DTV_CA_FINGER_X						120
#define DTV_CA_FINGER_Y                     160
#define DTV_CA_FINGER_WIDTH                 400
#define DTV_CA_FINGER_HEIGHT                50

/*CA ������ϢTop OSD��ʾλ�ü���С*/
#define DTV_CA_SCROLL_TOP_POS_X             0
#define DTV_CA_SCROLL_TOP_POS_Y             0
#define DTV_CA_SCROLL_TOP_WIDTH             DTV_RGN_WIDTH
#define DTV_CA_SCROLL_TOP_HEIGHT            40

/*CA ������ϢBottom OSD��ʾλ�ü���С*/
#define DTV_CA_SCROLL_BOTTOM_POS_X          0
#define DTV_CA_SCROLL_BOTTOM_POS_Y          0
#define DTV_CA_SCROLL_BOTTOM_WIDTH          DTV_RGN_WIDTH
#define DTV_CA_SCROLL_BOTTOM_HEIGHT         40
#define DTV_CA_OSD_ASC_HEIGHT               50
#define DTV_CA_OSD_ASC_WIDTH                40

/*�ʼ���ʾСͼ����ʾλ�ü���С*/
#define DTV_CA_EMAIL_MSG_POS_X              120
#define DTV_CA_EMAIL_MSG_POS_Y              100
#define DTV_CA_EMAIL_MSG_WIDTH              60
#define DTV_CA_EMAIL_MSG_HEIGHT             40

/*CA����Ȩ��Ϣ��ʾλ�ü���С*/
#define DTV_CA_DETITLE_MSG_POS_X            1620
#define DTV_CA_DETITLE_MSG_POS_Y            (DTV_CHNL_NUM_Y + DTV_CHNL_NUM_HEIGHT + 5)
#define DTV_CA_DETITLE_MSG_WIDTH            120
#define DTV_CA_DETITLE_MSG_HEIGHT           50

/*����DTV��ɫ����ɫ����*/
#define DTV_TRANS							0
#define DTV_WHITE           				1
#define DTV_BLACK           				2
#define DTV_YELLOW          				3
#define DTV_GRAY            				4
#define DTV_LOW_YELLOW          			5
#define DTV_RED             				6
#define DTV_GREEN           				7

#define DTV_ORANGE          8
#define DTV_TRANSBLACK      9
#define DTV_BRIGHT_RED      10
#define DTV_TRANSWHITE_LOW  11
#define DTV_TRANS_BLUE_NEW  12
#define DTV_COLOR13   		13
#define DTV_BLUE_TRANS      14
#define DTV_CYAN_TRANS      15


/*����APP��ɫ����ɫ����*/
#define APP_TRANS							0
#define APP_WHITE           				253
#define APP_BLACK           				255
#define APP_YELLOW          				241
#define APP_GRAY            				123
#define APP_RED             				250
#define APP_GREEN           				205
#define APP_BLUE                            234 

#define DTV_GET_BIT(byte,yy)     	(((byte) >> (yy)) & 0x01)
#define DTV_OSD_IS_CHINESE(x)       ((x) & 0x80)
#define DTV_OSD_IS_CR(x)            ((x) == '\n')

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*����DTV����ʾ��ʾ���ȼ�*/
typedef enum
{
    DTV_INVALID_NOTIFY_PRIORITY,
    DTV_CHANNEL_LOCK_NOTIFY_PRIORITY,   /*Ƶ��������ʾ���ȼ�Ϊ1*/
    DTV_CA_NOTIFY_PRIORITY,             /*CA��ʾ���ȼ�Ϊ2*/ 
    DTV_NO_PROG_PRIORITY,				/*�޽�Ŀ��ʾ���ȼ�Ϊ3*/
    DTV_SINGLE_LOST_NOTIFY_PRIORITY,    /*�ź��ж����ȼ�Ϊ4*/  
    DTV_COUNT_PRIORITY
}DTV_Notify_Priority_e;

/*����DTV��Ϣ����ʾ��ʾ*/
typedef struct DTV_BannerShow
{
	BOOL 	bProgNameShow;
	BOOL 	bProgTimeShow;
	BOOL 	bPresentShow;
	BOOL 	bFollowShow;
	BOOL 	bAdShow;
}DTV_BannerShow_t;

/*����CA������Ϣ�ṹ����*/
typedef struct DTV_CAScrollInfo
{
	BOOL IsRunning;
	OSD_Bitmap_t  *stBitmap;
	OSD_Region_t * hOSDHandle;
	SizeRect_t stMsgRect;
	unsigned int width;
	unsigned int height;
	int siStart;
}DTV_CAScrollInfo_t;
/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_OsdRegionCreate
  Description     : ����DTVRegion
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_OsdRegionCreate(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_Cls
  Description     : ���DTVRegion
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_Cls(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_EnableDisplay
  Description     : ��DTVRegion����Ϊ��ǰRegion������ʾ
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : Ӧ�����˳���DTV����Ҫ����
*****************************************************************************/ 
void DTV_EnableDisplay(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_DisableDisplay
  Description     : ��DTVRegion����Ϊ����ʾ
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ��DTV�½���Ӧ����Ҫ���� 
*****************************************************************************/ 
void DTV_DisableDisplay(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_OSDOutHz
  Description     : DTV����ʾ�ı��ַ�
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_OSDOutHz(OSD_Region_t *OsdHandle,char *pucHZ,int x,int y,int palette,int zoom);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_OSDOutHzNew
  Description     : �µĺ�����ʾ����    
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 1. ��ӻس�����֧��;2. ��Ӵ��ڴ�С֧��(col*row)
*****************************************************************************/
void DTV_OSDOutHzNew(OSD_Region_t *OsdHandle,char *pucHZ,int x,int y,int palette,int zoom,unsigned int uiWinWidth,unsigned int uiWinHeight);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetStringWidth
  Description     : �����ִ���ռ�Ŀ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned int DTV_GetStringWidth(char *pcString,int Width);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_FillStrNumber
  Description     : ���������ַ�����λ��
  Input           : *pStringOfNumer:�ַ���ָ��,NumOfWord:����,FillNum:���ֵ
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_FillStrNumber(char *pStringOfNumer,int NumOfWord,int FillNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurDataTime
  Description     : ��ȡ��ǰ�����ں�ʱ��
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
StbTm_t *DTV_GetCurDataTime(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetText
  Description     : ��ȡ��Ӧ�ı���Ϣ
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
char *DTV_GetText(char *TextList[][2],int StringNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetDataTimeString
  Description     : ��ȡ���ں�ʱ���Ӧ�ַ���Ϣ
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_GetDataTimeString(char *StringDisplay,StbTm_t *pTime);

/*****************************************************************************
  Date&Time       : 2011-12-02
  Author          :   zhubo
  Function Name   : DTV_GetDataString
  Description     : ��ȡ���ڶ�Ӧ�ַ���Ϣ,��ʽ����:11��11��������
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_GetDataString(char *StringData,StbTm_t *pTime);

/*****************************************************************************
  Date&Time       : 2011-12-02
  Author          :   zhubo
  Function Name   : DTV_GetTimeString
  Description     : ��ȡʱ���Ӧ�ַ���Ϣ,��ʽ����:17:29
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_GetTimeString(char *StringTime,StbTm_t *pTime);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ClearChnlNum
  Description     : ���DTV��Ƶ������ʾ 
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ClearChnlNum(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_DisplayChnlNum
  Description     : ��ָ���ַ�����ʾ��Ƶ����λ��
  Input           : ָ���ַ���ָ��
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_DisplayChnlNum(char *pucChar);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_ShowNullChnlNum
  Description     : DTV����ʾ��ЧƵ���� 
  Input           : 
  Outut           :
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DTV_ShowNullChnlNum(unsigned short usNum);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_InitNotifyInfo
  Description     : ��ʼ�����ȼ���ʾ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_InitNotifyInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_GetAppVideoRectFlag
  Description     : ��ȡAPPС��Ƶ���ڱ�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL DTV_GetAppVideoRectFlag(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_SetAppVideoRectFlag
  Description     : ����APPС��Ƶ���ڱ�ʶ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_SetAppVideoRectFlag(BOOL bFlag);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : DTV_GetCurrentDTVNotifyPriority
  Description     : ��ȡ��ǰDTV��ʾ��ʾ�����ȼ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned int DTV_GetCurrentDTVNotifyPriority(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_SetCurrentDTVNotifyPriority
  Description     : ���õ�ǰDTV��ʾ��ʾ�����ȼ�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_SetCurrentDTVNotifyPriority(unsigned int uiPriority);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_GetCurrentDTVShowCANotifyNo
  Description     : ��ȡ��ǰDTV��ʾCA��ʾ���±�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
unsigned char DTV_GetCurrentDTVShowCANotifyNo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan    
  Function Name   : DTV_RefreshNotifyInfo
  Description     : ˢ��DTV��ʾ��Ϣ��ʾ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_RefreshNotifyInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan    
  Function Name   : DTV_RefreshNoticeInAppVideoRect
  Description     : ˢ��APPС��������ʾ��ʾ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_RefreshNoticeInAppVideoRect(void);  

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ShowChannelLock
  Description     : DTV����ʾ��Ŀ����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ShowChannelLock(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_HideChannelLock
  Description     : DTV�������Ŀ������ʾ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_HideChannelLock(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ShowCAMessage
  Description     : DTV����ʾCA��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ShowCAMessage(unsigned char uiCANotifyNo);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_HideCAMessage
  Description     : DTV�����CA��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_HideCAMessage(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ShowNoProgramme
  Description     : DTV����ʾ�޽�Ŀ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ShowNoProgramme(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_HideNoProgramme
  Description     : DTV������޽�Ŀ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_HideNoProgramme(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ShowSingleLost       
  Description     : DTV����ʾ�ź��ж�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ShowSingleLost(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_HideSingleLost
  Description     : DTV������ź��ж�
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_HideSingleLost(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_VolumeBackgroundShow
  Description     : ��ʾDTV��������
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_VolumeBackgroundShow(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_VolumeShow
  Description     : ��ʾDTV����
  Input           : 
  Outut           :
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_VolumeShow(int Volume,BOOL bMute);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ClearBanner
  Description     : ���DTV��Ϣ����ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ClearBanner(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_HideBanner
  Description     : ����DTV��Ϣ����ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_HideBanner(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_ShowPFInfo
  Description     : ��ʾDTV��Ϣ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_ShowPFInfo(void);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan 
  Function Name   : DTV_RefreshBanner
  Description     : ˢ��DTV��Ϣ����ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_RefreshBanner(void);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDHideFingerMsg
  Description     : DTV region������CAָ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDHideFingerMsg(void);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDShowFingerMsg
  Description     : DTV region����ʾCAָ����Ϣ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDShowFingerMsg(unsigned int uiID);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDScrollInfoStart
  Description     : CA Osd������ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL DTV_CA_OSDScrollInfoStart(unsigned int uiScrollType,char *pString);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDScrollInfoRun
  Description     : CAS Osd��Ϣ����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDScrollInfoRun(unsigned int uiScrollType);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo  
  Function Name   : DTV_CA_OSDScrollInfoStop
  Description     : ֹͣCAS OSD��Ϣ����
  Input           : 
  Outut           : 
  Calls           : 
  Called by       : 
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDScrollInfoStop(unsigned int uiScrollType);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo 
  Function Name   : DTV_CA_OSDHideEmailMsg
  Description     : �����ʼ���ʾСͼ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDHideEmailMsg(void);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDShowEmailMsg
  Description     : ��ʾ�ʼ���ʾСͼ��
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDShowEmailMsg(void);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDHideDetitleMsg
  Description     : ���ط���Ȩ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDHideDetitleMsg(void);

/*****************************************************************************
  Date&Time       : 2012-01-05
  Author          :   zhubo
  Function Name   : DTV_CA_OSDShowDetitleMsg
  Description     : ��ʾ����Ȩ��Ϣ
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void DTV_CA_OSDShowDetitleMsg(void);

#endif
