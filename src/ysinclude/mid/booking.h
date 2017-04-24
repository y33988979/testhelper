/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : booking.h

    Description : ԤԼ����

    Others:      

    History:  1. yzb creat 2005-10-18
			  2. xyan Modify 2011-11-16

*******************************************************************************/
#ifndef BOOKING_H_
#define BOOKING_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define BOOK_DATA_VERSION  					0x00000001
#define BOOK_DATA_MAGIC_WORD  				0x19800508
#define BOOK_DATA_MAX_COUNT  				120//���ԤԼ��120*68+8+24(RESERVED) = 8192

#define BOOK_CH_NAME_MAX_LENGTH  			16
#define BOOK_PRG_NAME_MAX_LENGTH  			32
#define BOOK_NOBUTTON_DLG_TIMEOUT 			2
#define BOOK_WITHBUTTON_DLG_TIMEOUT 		15
#define BOOK_DLG_NOTIFY_INFO_BYTES			150

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
typedef enum
{
	BOOK_INFO_INVALID = 0,
	BOOK_INFO_VALID = 1,
	BOOK_INFO_FIRST_ALARMED,
	BOOK_INFO_SECOND_ALARMED,
	BOOK_INFO_ACTION,
	BOOK_INFO_COUNT
}BOOK_InfoState_e;

typedef enum
{
	BOOK_INFO_BOOKING_TV = 1,
	BOOK_INFO_BOOKING_RADIO = 2,
	BOOK_INFO_BOOKING_NVOD = 3,
    BOOK_INFO_MAX_COUNT
}BOOK_InfoType_e;

typedef enum
{
    BOOK_TYPE_NONE = 0,
	BOOK_TYPE_WATCH = 1,
	BOOK_TYPE_PVR = 2,
    BOOK_TYPE_COUNT
}BOOK_Type_e;

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/* ��flash��ע���ֽڶ���:���ݽṹ���� 68 �ֽ� */
typedef struct 
{
	unsigned short NetworkID;
	unsigned short TsID;
	unsigned short ServiceID;
	unsigned short EventID;				
	unsigned int TimeBegin;
	unsigned int TimeEnd;
	unsigned char State;
	unsigned char Type;
	unsigned short ChannelID;
	char ChanName[BOOK_CH_NAME_MAX_LENGTH];
	char PrgName[BOOK_PRG_NAME_MAX_LENGTH];
}BOOK_BookingInfo_t;

/*���ݽṹ����8�ֽ�*/
typedef struct
{
	int MagicWord;/*ԤԼ���ݱ�־*/
	unsigned char DataCount;/*256*68>16K*/
	char Version;/*ԤԼ���ݽṹ�汾*/
	char Reserved[2];
}BOOK_BookingTable_t;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_GetBookingList
  Description     : ��ȡԤԼ����ָ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
list_t *BOOK_GetBookingList(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_Disable
  Description     : ֹͣԤԼ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_Disable(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_Enable
  Description     : ��ԤԼ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_Enable(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_SaveData
  Description     : ��ԤԼ���ݴ���FLASH��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : �ڴ���Listʱ�Ͱ�ʱ�����򣬼�ȡ������
*****************************************************************************/ 
BOOL BOOK_SaveData(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_BookingItemDel
  Description     : ɾ��ָ����ԤԼ���ݽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_BookingItemDel(BOOK_BookingInfo_t *Item);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_ClearBooking
  Description     : �������ԤԼ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_ClearBooking(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_CheckData
  Description     : ���ָ��Event�Ƿ����ԤԼ������,����Event���ԤԼ��ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_CheckData(STBDATAServiceInfo_t *pServiceInfo,STBDATAEvent_t *pEventInfo,StbTime_t CurrentTime);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_BookingAdd
  Description     : ��ӵ���/�㲥ԤԼ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
BOOK_BookingInfo_t *BOOK_BookingAdd(STBDATAChannelInfo_t *ChannelInfo,STBDATAEvent_t *Prog,unsigned char Type);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_NvodBookingAdd
  Description     : ���NVODԤԼ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
BOOK_BookingInfo_t *BOOK_NvodBookingAdd(STBDATAServiceInfo_t *ServiceInfo,STBDATAEvent_t *TimeshiftProg,STBDATAEvent_t *ReferenceProg,unsigned char Type);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_BookingDel
  Description     : ɾ��ָ����ԤԼ���ݽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_BookingDel(STBDATAServiceInfo_t *pServiceInfo,STBDATAEvent_t *pEventInfo,unsigned char BookingType);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_BookingDelByBookInfoAndCheckData
  Description     : ɾ��ָ��ԤԼ����,��ȥ����ӦEventԤԼ��ʾ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_BookingDelByBookInfoAndCheckData(BOOK_BookingInfo_t *BookingInfo);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :   xyan
  Function Name   : BOOK_Process
  Description     : ԤԼ����봥��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : ����ԤԼ���ݶ��ǰ�ʱ���Ⱥ�˳�����������,���,ֻ��Ҫ�����һ��
*****************************************************************************/ 
int BOOK_Process(DRVOS_MsgQueue_t *DealBookAppMsgQueue);

/*****************************************************************************
  Date&Time    	  : 2004-03-29
  Author          :  xyan
  Function Name   : BOOK_Init
  Description     : ԤԼ��ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void BOOK_Init(void);

#endif 

