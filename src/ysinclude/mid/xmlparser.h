/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : Xmlparser.h

    Description : XML��������

    Others:      

    History:     1. llh creat 2004-3-29
                 2. xyan Modify 2011-11-15

*******************************************************************************/
#ifndef _XMLPARSER_H
#define _XMLPARSER_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stbtype.h"
#include "driver.h"

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
/*��������������Ҫ�ӿں���*/
typedef void(*xmlStartElement)(void *userData, const char *name, const char **atts);
typedef void(*xmlCharacterData)(void *userData,const char *s,int len);
typedef void(*xmlEndElement)(void *userData, const char *name);

/*�������ṹ*/
typedef struct __XMLParser
{
	void *userData;
	char *pParserName;
	xmlStartElement  pStartElement;
	xmlCharacterData pCharacterData;
	xmlEndElement    pEndElement;

}XMLParser;

typedef struct xmlElement
{
	char *pText;
	struct xmlElement *pNext;
}xmlElement_t;

/*�ڵ����ݽṹ*/
typedef struct NodeAttribute
{
	char  *pucName;/*�ڵ�����*/
	char  *pucData;/*�ڵ���������*/
}NodeAttribute_t;

typedef struct xmlElememtNode
{
	unsigned short uiNodeID;				/*�ý���ID��,��һ�����У�ID��Ψһ��*/
	unsigned short ucCountOfAtts;	    	/*�ڵ����Ը���*/
	char *pucName;                  		/*�ýڵ������*/
	char *pucData;		      				/*�ýڵ����������*/
	NodeAttribute_t *pNodeAttribute;    	/*�ڵ���������*/
	struct xmlElememtNode *pParentNode; 	/*���ڵ�*/
	struct xmlElememtNode *pFirstChildNode;	/*��һ���ӽڵ�*/
	struct xmlElememtNode *pBrotherNode;	/*�ֵܽڵ�*/
}xmlNode_t;

/*�ڵ�����ṹ*/
typedef struct BrotherList
{
	xmlNode_t *pFirstBrother;
	xmlNode_t *pLastBrother;
	xmlNode_t *pCurrentBrother;	
}BrotherList_t;

/*���Ķ��뷽ʽ*/
typedef struct ColAtts
{
	unsigned char  ucColWidth;
	unsigned char	ucColAlignType;/*�еĶ��뷽ʽ:0-����1-�����2-�Ҷ���*/
}ColAtts_t;

typedef struct TableInitPara
{
	unsigned char	ucCountOfCol;
	unsigned char   ucTitleAlignType;/*����Ķ��뷽ʽ:0-����1-�����2-�Ҷ���*/
	ColAtts_t *pColAtts;/*�е���������*/
}TableInitPara_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :    xyan
  Function Name   : XML_ParseData
  Description     : ����XML����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL XML_ParseData(char *pData,unsigned int uiDataLen,xmlNode_t **pRootNode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :    xyan
  Function Name   : GetXMLNodeFromID
  Description     : ͨ��ID��ȡXML�ڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void GetXMLNodeFromID(xmlNode_t *pRootNode,unsigned int uiID,xmlNode_t **ppNode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNodeAttribute
  Description     : ��ȡ�ڵ�����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void GetNodeAttribute(xmlNode_t *pXmlNode,const char *AttsName,char *AttsValue);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetCountOfBrothers
  Description     : ��ȡ�ֵܽڵ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/	
int GetCountOfBrothers(xmlNode_t *pXmlNode);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetCountOfYoungBrother
  Description     : ��ȡ�ֵܽڵ���ֵܽڵ����
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int GetCountOfYoungBrother(xmlNode_t *pXmlNode);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetCountOfOldBrother
  Description     : ��ȡ�ֵܽڵ���Ŀ
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
int GetCountOfOldBrother(xmlNode_t *pXmlNode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetFirstBrother
  Description     : ��ȡ��һ���ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
xmlNode_t * GetFirstBrother(BrotherList_t  *pBrotherList);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetCurrentBrother
  Description     : ��ȡ��ǰ�ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
xmlNode_t *GetCurrentBrother(BrotherList_t  *pBrotherList);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : SetCurrentBrother
  Description     : ���õ�ǰ�ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void SetCurrentBrother(BrotherList_t  *pBrotherList,xmlNode_t *pBrotherNode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetNextBrother
  Description     : ��ȡ��һ���ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
xmlNode_t * GetNextBrother(BrotherList_t  *pBrotherList);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetPreBrother
  Description     : ��ȡǰһ���ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
xmlNode_t * GetPreBrother(BrotherList_t  *pBrotherList);
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : GetLastBrother
  Description     : ��ȡ���һ���ֵܽڵ�
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
xmlNode_t * GetLastBrother(BrotherList_t  *pBrotherList);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : BrotherListInit
  Description     : �ֵܽڵ������ʼ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void BrotherListInit(BrotherList_t  *pBrotherList,xmlNode_t *pXmlNode);

/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :   xyan
  Function Name   : XML_PartionSet
  Description     : ���ý���XML�ڴ��
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void XML_PartionSet(DRVOS_Partition_t *Part);

#endif 
