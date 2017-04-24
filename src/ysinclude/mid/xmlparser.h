/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : Xmlparser.h

    Description : XML解析功能

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
/*解析器的三个主要接口函数*/
typedef void(*xmlStartElement)(void *userData, const char *name, const char **atts);
typedef void(*xmlCharacterData)(void *userData,const char *s,int len);
typedef void(*xmlEndElement)(void *userData, const char *name);

/*解析器结构*/
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

/*节点数据结构*/
typedef struct NodeAttribute
{
	char  *pucName;/*节点名称*/
	char  *pucData;/*节点属性内容*/
}NodeAttribute_t;

typedef struct xmlElememtNode
{
	unsigned short uiNodeID;				/*该结点的ID号,在一颗树中，ID是唯一的*/
	unsigned short ucCountOfAtts;	    	/*节点属性个数*/
	char *pucName;                  		/*该节点的名称*/
	char *pucData;		      				/*该节点的数据内容*/
	NodeAttribute_t *pNodeAttribute;    	/*节点属性数组*/
	struct xmlElememtNode *pParentNode; 	/*父节点*/
	struct xmlElememtNode *pFirstChildNode;	/*第一孩子节点*/
	struct xmlElememtNode *pBrotherNode;	/*兄弟节点*/
}xmlNode_t;

/*节点链表结构*/
typedef struct BrotherList
{
	xmlNode_t *pFirstBrother;
	xmlNode_t *pLastBrother;
	xmlNode_t *pCurrentBrother;	
}BrotherList_t;

/*表格的对齐方式*/
typedef struct ColAtts
{
	unsigned char  ucColWidth;
	unsigned char	ucColAlignType;/*列的对齐方式:0-居中1-左对齐2-右对齐*/
}ColAtts_t;

typedef struct TableInitPara
{
	unsigned char	ucCountOfCol;
	unsigned char   ucTitleAlignType;/*标题的对齐方式:0-居中1-左对齐2-右对齐*/
	ColAtts_t *pColAtts;/*列的属性数组*/
}TableInitPara_t;

/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time       : 2009-04-24
  Author          :    xyan
  Function Name   : XML_ParseData
  Description     : 解析XML数据
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
  Description     : 通过ID获取XML节点
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
  Description     : 获取节点属性
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
  Description     : 获取兄弟节点个数
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
  Description     : 获取兄弟节点的兄弟节点个数
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
  Description     : 获取兄弟节点数目
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
  Description     : 获取第一个兄弟节点
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
  Description     : 获取当前兄弟节点
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
  Description     : 设置当前兄弟节点
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
  Description     : 获取下一个兄弟节点
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
  Description     : 获取前一个兄弟节点
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
  Description     : 获取最后一个兄弟节点
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
  Description     : 兄弟节点链表初始化
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
  Description     : 设置解析XML内存块
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
void XML_PartionSet(DRVOS_Partition_t *Part);

#endif 
