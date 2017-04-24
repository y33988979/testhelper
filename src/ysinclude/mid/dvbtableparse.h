/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : dvbtableparse.h

    Description : 

    Others:      

    History:    1. llh creat 2004-3-29
				2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _YSDVBTABLEPARSE_
#define _YSDVBTABLEPARSE_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h"
#include "dvb.h"
#include "dvbtablemgr.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define MAX_PID_VALUE             			0x1FFF   
#define	NIT_PROGRAM_NO						0
#define	MAX_NO_OF_PROGRAMS					100
#define	NO_OF_CRC_DATA_BYTES				4

#define EIT_COMMON_SCHEDULE                 1		//��ͨService��Schedule		
#define EIT_NVOD_TIMESHIFT_SCHEDULE			2		//Nvodʱ��Service��Schedule
#define EIT_PRESENTANDFOLLOWING       		3		//��ͨService��PF
#define EIT_PF_AS_SCHEDULE                  4		//Nvod�ο�Service��PF

#define NIT_PROGRAM_NO 						0
#define MAX_EVENT_TYPE_DESCRIPTION_LENGTH	100
#define NO_OF_CRC_DATA_BYTES 				4
#define SEGMENT_SECTION_NO 					8

#define INVALID_SECTION_NO 					-1
#define PRIVATE_SETTING_DESC 				0x80

#define LOGICAL_SCREEN_DESC 				0x81	//��ͨ������
#define CHANNEL_ORDER_DESC 					0x82	//ͬ��BAT
#define CHANNEL_VOLUME_COMPENSATE_DESC		0x83	//ͬ��BAT
#define BATID_CHANNEL_ORDER 				0x7011  //ͬ��bat�����batid

#define CHANNEL_NUMBER_DESC 				0x90	//��� Ƶ����������
#define SERVICE_INFORMATION_DESC      		0x92    //Ƶ��˵��������
#define SERVICE_ENTRANCE_DESC 				0X96    //˼�� vod��Ƶ��������

#define PRIVATE_SPECIFER_SUANTONG 			0x0000001F	//��ͨ˽���������I
//#define PRIVATE_SPECIFER_TONGFANG 		0x5f040000	//ͬ��˽���������R

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef enum Table_Descriptor_To_Parse
{	
	NIT_FIRST_DESCRIPTOR_LOOP,						
	NIT_SECOND_DESCRIPTOR_LOOP,						
	BAT_FIRST_DESCRIPTOR_LOOP,		
	BAT_SECOND_DESCRIPTOR_LOOP,
	PMT_FIRST_DESCRIPTOR_LOOP,
	PMT_SECOND_DESCRIPTOR_LOOP,							
	SDT_DESCRIPTOR_LOOP,												
	EIT_DESCRIPTOR_LOOP,			
	CAT_DESCRIPTOR_LOOP,				
	TOT_DESCRIPTOR_LOOP,												
	PRIVATE_DESCRIPTOR_TABLE								
}Table_Descriptor_To_Parse_e;

/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DVB_TransferTextCharacters
  Description     : ת���ַ��ı�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DVB_TransferTextCharacters(unsigned char *pDestText,int DestTextMaxBytes,unsigned char *pSrcText,int SrcTextLength,DRVOS_Partition_t *pPtn);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ConvertUnicodeToGBK
  Description     : ��Unicodeת��ΪGBK
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
unsigned char *ConvertUnicodeToGBK(unsigned char* hzInput,int nLen);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DVB_ParseNIT
  Description     : ����NIT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
HRESULT	DVB_ParseNIT(unsigned char *pucSectionData,int *psNextSectionNumber,DvbSI_t* pDvbSiInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ParseBAT
  Description     : ���� BAT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
HRESULT	DVB_ParseBAT (unsigned char * pucSectionData, int* psNextSectionNumber,DvbSI_t* pDvbSiInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ParsePAT
  Description     : ���� PAT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
HRESULT DVB_ParsePAT(unsigned char *pucSectionData,int *psNextSectionNumber,DvbTs_t *pDvbTsInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ParsePMT
  Description     : ���� PMT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
HRESULT DVB_ParsePMT(unsigned char *pucSectionBuffer, int* psNextSectionNumber,DvbService_t *pServiceInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : parseCAT
  Description     : ���� CAT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
 		
*****************************************************************************/ 
HRESULT DVB_ParseCAT(unsigned char *pucSectionData,int *psNextSectionNumber,DvbTs_t *pTsInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ParseSDT
  Description     : ���� SDT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
 		
*****************************************************************************/ 
HRESULT DVB_ParseSDT(unsigned char *pucSectionData,int *psNextSectionNumber,DvbTs_t *pTsInfo);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : ParseTDT
  Description     : ����TDT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
 		
*****************************************************************************/ 
HRESULT DVB_ParseTDT(unsigned char *pucSectionData);

/*****************************************************************************
  Date&Time       : 2004-03-29
  Author          :   xyan
  Function Name   : DVB_ParseTOT
  Description     : ���� TOT
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
 		
*****************************************************************************/ 
HRESULT DVB_ParseTOT(unsigned char *pucSectionData);


#endif

