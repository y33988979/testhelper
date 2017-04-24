/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysoslist.h

    Description : 

    Others:      

    History:      1. llh creat 2004-3-24


*******************************************************************************/

#ifndef _YSOSLIST_H_
#define _YSOSLIST_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "drv_os.h"
#include "ys_typedef.h"


#define OPERATION_SUCCEED	0
#define OPERATION_FAIL		1

/*=========================================================================
    Global Variables
=========================================================================== */


/*-------------------------------------------------------------------------
 * Structure definitions
 *-------------------------------------------------------------------------*/
/*The function prototype for deleting the data part of the list node*/
typedef void (*ListDataDelete)(void *pData, DRVOS_Partition_t *pPtn);
/*The function prototype for converting the list node to nvm data structure.
  Can be neglected when the data is of constant length and no pointer is included*/
typedef void (*ListData2NVM)(void *pData, S8_YS *pBuffer, U32_YS *nLength);
/*The function prototype for converting the nvm data structure to list node
  Can be neglected when the data is of constant length and no pointer is included*/
typedef void (*ListNVM2Data)(void *pData, S8_YS *pBuffer, U32_YS *nLength);


typedef struct IndexItem {
    void *      pItem;
} IndexItem_t;

/*The node composing the list*/
struct stbnode_t {
    struct stbnode_t   *pPrev;
    struct stbnode_t   *pNext;
    void            *pData;
};

/*The head of the list*/
struct stbhead_t {
  /*???系统可能对信号量有个数要求*///DRVOS_Sem_t lock;
  struct stbnode_t   *pFirst;
  struct stbnode_t   *pLast;
  struct stbnode_t  *pCur;
  int             nLength;
  DRVOS_Partition_t     *pPtn;
  int               nSize;      
  ListDataDelete    fnDataDelete;
  ListData2NVM  fnData2NVM;
  ListNVM2Data  fnNVM2Data;
};

/*The head of the list when it is saved to nvm*/
typedef struct NMVHead {
    char                cValidity;      //0:false; 1:valid
    unsigned short      nLength;        //The length of the contents, exclusive
    unsigned short      nItem;          //The number of items
} NVMHead_t;

/*The node of the list when it is saved to nvm*/
typedef struct NVMNode {
    char                cValidity;      //0:false; 1:valid
    unsigned short      nLength;        //The length of the node
} NVMNode_t;


typedef struct stbnode_t STBNODE_T;
typedef struct stbhead_t STBHEAD_T;
typedef struct stbhead_t list_t;


IndexItem_t * IndexItemCreate(DRVOS_Partition_t *pPtn, void *pItem);

int ListGetCount(list_t *pList);
list_t* ListCreate(int nLength, ListDataDelete fnDataDelete, ListData2NVM fnData2NVM, ListNVM2Data fnNVM2Data, DRVOS_Partition_t *pPtn);
S32_YS ListInit ( list_t * pList, int nSize, ListDataDelete fnDataDelete, ListData2NVM fnData2NVM, ListNVM2Data fnNVM2Data, DRVOS_Partition_t *pPtn);
S32_YS ListDelete ( list_t * pList);
S32_YS ListTerm ( list_t * pList);
STBNODE_T *ListCreateNode( list_t *pList, void *pData );
STBNODE_T *ListAddBegin( list_t *pList, void *pData );
STBNODE_T *ListAddEnd( list_t *pList, void *pData );
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListInsertBefore
  Description     :  insert node before specified node
  Input           : 
	   		
  Outut           :  
  			Pointer to the new node
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
STBNODE_T *ListInsertCurNodeBefore( list_t *pList,void *pData );
S32_YS ListTermDataNotFree ( list_t * pList);

STBNODE_T *ListInsertBefore( list_t *pList, void *pTheNode, void *pData ); 
STBNODE_T *ListInsertAfter( list_t *pList, void *pTheNode, void *pData );
void ListDeleteBegin( list_t* pList );
void ListDeleteEnd( list_t* pList );
void ListDeleteItem( list_t * pList, void* pData );
void ListDeleteCur( list_t * pList );
void *ListMoveHead( list_t *pList );
void *ListMoveEnd( list_t *pList );
void *ListMoveNext( list_t *pList );
void *ListMovePrev( list_t *pList );
void *ListMoveTo( list_t *pList, void *pData );
void *ListGetAt( list_t *pList );

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
