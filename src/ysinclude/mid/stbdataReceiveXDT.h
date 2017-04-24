/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbdataReceiveXDT.h

    Description : ���ݵ�����վ����

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSstbdataReceiveXDT_
#define _YSstbdataReceiveXDT_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "dvb.h"
#include "midware.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define XDT_TABLE_ID 						0XEA //234
#define XDT_PID 							0X1EA1 //7841
#define SECTION_WAIT_TIMEOUT_XDT_MS   		10000
#define SECTION_WAIT_TIMEOUT_INDEX_MS  		16000

#define XML_PARSE_DATA_LENGTH 				( 1024 * 128)
#define XML_PARSE_DATA_INDEX_LENGTH 		( 1024 * 155)
#define XML_PARSE_DATA_LINK_LENGTH 			( 1024 * 155)

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct XMLFile
{
    unsigned int 	uiSourceFileLength;
    unsigned int 	uiCompressDataLength;
    unsigned short 	usMaxDataBlockNumber;
    unsigned short 	usDataBlockReceivedNumber;
    unsigned char 	*pBlockReceived;
    unsigned char 	*pCompressFileData;
    BOOL	 		bReceiveComplete;
    BOOL	 		bFirstReceived;
}XMLFile_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : LockDefaultDelivery
  Description     : ����Ƶ��
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void LockDefaultDelivery(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : StartReceiveXDT
  Description     : ��ʼ����XDT ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void StartReceiveXDT(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : FinishReceiveXDT
  Description     : ��������XDT ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void FinishReceiveXDT(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : GetXDTReceiveStatus
  Description     : ��ȡ����XDT ����״̬
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL GetXDTReceiveStatus(void);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : RequestXMLDataWaitting
  Description     : �ȴ�������ȡXML ����
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void RequestXMLDataWaitting(unsigned short StartRequestStatus);

/*****************************************************************************
  Date&Time    	  : 2004-03-30
  Author          :   xyan
  Function Name   : RequestXMLLinkageWaitting
  Description     : �ȴ�������ȡXML �����ļ�
  Input           : 
  Outut           :   
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void RequestXMLLinkageWaitting(DRVOS_Partition_t *pPtn,unsigned short usXDTExtentionTableID);


#endif


