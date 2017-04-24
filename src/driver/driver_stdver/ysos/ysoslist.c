/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysoslist.c

    Description : 

    Others:      

    History:      1. llh creat 2004-3-24


*******************************************************************************/

/* Includes --------------------------------------------------------------- */
#include <string.h>

#include "ysoslist.h"


/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : IndexItemCreate
  Description     : Create an index item for the item access
  Input           : 
	   		pPtn		: [In] The partition in which creation is performed
	 		pItem		: [In] pointer to the item to create index for
  Outut           :  
  			pointer to the index item created
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
IndexItem_t * IndexItemCreate(DRVOS_Partition_t *pPtn, void *pItem)
{
	IndexItem_t *pIndex = NULL;
	pIndex = (IndexItem_t *)DRV_OSMalloc(pPtn, sizeof(IndexItem_t));
	if (pIndex != NULL)
		pIndex->pItem = pItem;
	return pIndex;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListCreate
  Description     : Create List
  Input           : 
	   		
  Outut           :  
  			pointer to the list
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

list_t* ListCreate ( int nSize, ListDataDelete fnDataDelete, ListData2NVM fnData2NVM, ListNVM2Data fnNVM2Data, DRVOS_Partition_t *pPtn)
{/*应对创建的链表加互斥*/
  list_t *pList = NULL;
  if (pPtn != NULL)
  {
    pList = (list_t *)DRV_OSMalloc(pPtn, sizeof(list_t));
    if (pList != NULL)
    {
      pList->nSize = nSize;/*???*/
      pList->nLength = 0;
      pList->pFirst = NULL;
      pList->pLast = NULL;
      pList->pCur = NULL;
      pList->pPtn = pPtn;
      pList->fnDataDelete = fnDataDelete;
      pList->fnData2NVM = fnData2NVM;
      pList->fnNVM2Data = fnNVM2Data;
    
      /*???系统可能对信号量有个数要求*///DRV_OSSemInit(NULL, &(pList->lock), 1, FIFO_WAIT);
    }
  }
  return pList;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteNode
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void ListDeleteNode ( list_t * pList, STBNODE_T *pNode)
{
	if ((pNode != NULL) && (pList != NULL))
	{
		if (pNode->pData != NULL)
		{
			if (pList->fnDataDelete)
				pList->fnDataDelete(pNode->pData, pList->pPtn);
			else
				/*pData 所分配空间必须与list中的part一致*/DRV_OSFree(pList->pPtn, pNode->pData);
		}
		DRV_OSFree(pList->pPtn, pNode);
	}
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteNode
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void ListDeleteNodeDataNotFree ( list_t * pList, STBNODE_T *pNode)
{
	if ((pNode != NULL) && (pList != NULL))
	{
		if (pNode->pData != NULL)
		{
		}
		DRV_OSFree(pList->pPtn, pNode);
	}
}

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDelete
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

S32_YS ListDelete ( list_t * pList)
{
	STBNODE_T * pNode = NULL;
	if (pList != NULL)
	{
	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
		while (pList->pFirst != NULL)
		{
			pNode = pList->pFirst;
			pList->pFirst = pNode->pNext;
			ListDeleteNode(pList, pNode);
		}
   /*????*///DRV_OSSemSignal(pList->lock);
		DRV_OSFree(pList->pPtn, pList);
	}
	return OPERATION_SUCCEED;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListInit
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/

S32_YS ListInit ( list_t * pList, int nSize, ListDataDelete fnDataDelete, ListData2NVM fnData2NVM, ListNVM2Data fnNVM2Data, DRVOS_Partition_t *pPtn)
{/*应对创建的链表加互斥*/
	if ((pList != NULL) && (pPtn != NULL))
	{
		pList->nLength = 0;
		pList->nSize = nSize;/*???*/
		pList->pFirst = NULL;
		pList->pLast = NULL;
		pList->pCur = NULL;
		pList->pPtn = pPtn;
		pList->fnDataDelete = fnDataDelete;
		pList->fnData2NVM = fnData2NVM;
		pList->fnNVM2Data = fnNVM2Data;

        /*???系统可能对信号量有个数要求*///DRV_OSSemInit(NULL, &(pList->lock), 1, FIFO_WAIT);
	}

	return OPERATION_SUCCEED;	
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListTerm
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

S32_YS ListTerm ( list_t * pList)
{
	STBNODE_T * pNode = NULL;

  if (NULL != pList)
  {
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

	while (pList->pFirst != NULL)
	{
		pNode = pList->pFirst;
		pList->pFirst = pNode->pNext;
		ListDeleteNode(pList, pNode);
	}
	pList->pCur = NULL;
	pList->pLast = NULL;
	pList->nLength = 0;
   //DRV_OSSemSignal(pList->lock);
 }
	return OPERATION_SUCCEED;
}

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListTerm
  Description     : 
  Input           : 
	   		list_t * pList
  Outut           :  
  			error code
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

S32_YS ListTermDataNotFree ( list_t * pList)
{
	STBNODE_T * pNode = NULL;

  if (NULL != pList)
  {
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

	while (pList->pFirst != NULL)
	{
		pNode = pList->pFirst;
		pList->pFirst = pNode->pNext;
		ListDeleteNodeDataNotFree(pList, pNode);
	}
	pList->pCur = NULL;
	pList->pLast = NULL;
	pList->nLength = 0;
   //DRV_OSSemSignal(pList->lock);
 }
	return OPERATION_SUCCEED;
}

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListCreateNode
  Description     : 
  Input           : 
	   		void *Data_p
  Outut           :  
  			Pointer to the new node
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 


STBNODE_T *ListCreateNode( list_t *pList, void *pData )
{
    STBNODE_T *pNewNode = NULL;

    pNewNode = DRV_OSMalloc( pList->pPtn, sizeof( STBNODE_T ));
    if ( pNewNode == NULL )
        return pNewNode;

    pNewNode->pPrev = NULL;
    pNewNode->pNext = NULL;
    pNewNode->pData = pData;

    return pNewNode;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListAddBegin
  Description     : add node to beginning of list
  Input           : 
	   		HEAD_T *Head_p, void *data_p
  Outut           :  
  			Pointer to the new node
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

STBNODE_T *ListAddBegin( list_t *pList, void *pData )
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNewNode = NULL;

  if (NULL != pList)
  {
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

    pNewNode = ListCreateNode( pList, pData );
    if ( pNewNode != NULL )
    {

      if ( pList->pFirst == NULL )
      {
          pList->pFirst = pNewNode;
          pList->pLast  = pNewNode;
  		pList->pCur = pNewNode;
          pList->nLength  = 1;
      }
      else
      {
          pList->pFirst->pPrev = pNewNode;
          pNewNode->pNext = pList->pFirst;
          pList->pFirst = pNewNode;
  
          pList->nLength ++;
      }
    }
   //DRV_OSSemSignal(pList->lock);
  }

    return pNewNode;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListAddEnd
  Description     : add node to end of list
  Input           : 
	   		HEAD_T *Head_p, void *data_p
  Outut           :  
  			Pointer to the new node
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

STBNODE_T *ListAddEnd( list_t *pList, void *pData )
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNewNode;

  if (NULL != pList)
  {
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
    pNewNode = ListCreateNode( pList, pData );
    if ( pNewNode != NULL )
    {  
      if ( pList->pFirst == NULL )
      {
          pList->pFirst = pNewNode;
          pList->pLast  = pNewNode;
  		pList->pCur = pNewNode;
          pList->nLength = 1;
      }
      else
      {
          pList->pLast->pNext = pNewNode;
          pNewNode->pPrev = pList->pLast;
          pList->pLast = pNewNode;
  
          pList->nLength ++;
      }
    }
   //DRV_OSSemSignal(pList->lock);
  }

    return pNewNode;
}
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

STBNODE_T *ListInsertBefore( list_t *pList, void *pTheNode, void *pData ) 
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNode=NULL;
    STBNODE_T *pNewNode=NULL;

	if ((pList == NULL) || (pData == NULL))
		return NULL;
  

	/*Insert at the begin if list empty or no specific node specified*/
	if (pTheNode == NULL)
		return ListAddBegin(pList, pData);
  
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
   
    for ( pNode = pList->pFirst; pNode != NULL; pNode = pNode->pNext )
    {
        if ( pNode->pData == pTheNode )
        {
            break;
        }
    }
   //DRV_OSSemSignal(pList->lock);

	/*Insert at the begin if no node meets condition found*/
	if (pNode == NULL)
		return ListAddBegin(pList, pData);

    if ( pNode->pData != pTheNode )
        return ListAddBegin(pList, pData);
    else
    {
        if ( pNode == pList->pFirst )
            return ListAddBegin( pList, pData );
        else
        {
            pNewNode = ListCreateNode( pList, pData );
            if ( pNewNode == NULL )
                return pNewNode;

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
            pNode->pPrev->pNext = pNewNode;    /* link to Previous node */
            pNewNode->pPrev = pNode->pPrev;

            pNode->pPrev = pNewNode;            /* link to this node */
            pNewNode->pNext = pNode;

            pList->nLength ++;
   //DRV_OSSemSignal(pList->lock);
        }
    }

    return pNewNode;
}

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
STBNODE_T *ListInsertNodeBefore( list_t *pList, STBNODE_T *pTheNode, void *pData ) 
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNode= pTheNode;
    STBNODE_T *pNewNode=NULL;

	if ((pList == NULL) || (pData == NULL))
		return NULL;
  

	/*Insert at the begin if list empty or no specific node specified*/
	if (pTheNode == NULL)
		return ListAddBegin(pList, pData);
  
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
   
	if (pNode == NULL)
		return ListAddBegin(pList, pData);
    else
    {
        if ( pNode == pList->pFirst )
            return ListAddBegin( pList, pData );
        else
        {
            pNewNode = ListCreateNode( pList, pData );
            if ( pNewNode == NULL )
                return pNewNode;

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
            pNode->pPrev->pNext = pNewNode;    /* link to Previous node */
            pNewNode->pPrev = pNode->pPrev;

            pNode->pPrev = pNewNode;            /* link to this node */
            pNewNode->pNext = pNode;

            pList->nLength ++;
   //DRV_OSSemSignal(pList->lock);
        }
    }

    return pNewNode;
}

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
STBNODE_T *ListInsertCurNodeBefore( list_t *pList,void *pData ) 
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNode = NULL;
    STBNODE_T *pNewNode=NULL;

	if ((pList == NULL) || (pData == NULL))
		return NULL;
  

	/*Insert at the begin if list empty or no specific node specified*/
	if (pList->pCur == NULL)
	{
		return ListAddEnd(pList, pData);
	}
  
	if (pList->pCur == pList->pFirst)
	{
		return ListAddBegin(pList, pData);
	}
	
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
   pNode = pList->pCur;
    {
        {
            pNewNode = ListCreateNode( pList, pData );
            if ( pNewNode == NULL )
                return pNewNode;

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
            pNode->pPrev->pNext = pNewNode;    /* link to Previous node */
            pNewNode->pPrev = pNode->pPrev;

            pNode->pPrev = pNewNode;            /* link to this node */
            pNewNode->pNext = pNode;

            pList->nLength ++;
   //DRV_OSSemSignal(pList->lock);
        }
    }

    return pNewNode;
}

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListInsertAfter
  Description     :  insert node after specified node
  Input           : 
	   		
  Outut           :  
  			Pointer to the new node
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

STBNODE_T *ListInsertAfter( list_t *pList, void *pTheNode, void *pData )
{/*pData 所分配空间必须与list中的part一致*/
    STBNODE_T *pNode = NULL;
    STBNODE_T *pNewNode = NULL;

	if ((pList == NULL) || (pData == NULL))
		return NULL;

	/*Insert at the end if list empty or no specific node specified*/
	if (pTheNode == NULL)
		return ListAddEnd(pList, pData);

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
    for ( pNode = pList->pFirst; pNode != NULL; pNode = pNode->pNext )
    {
        if ( pNode->pData == pTheNode )
        {
            break;
        }
    }
   //DRV_OSSemSignal(pList->lock);

	/*Insert at the end if no node meets condition found*/
	if (pNode == NULL)
		return ListAddEnd(pList, pData);

    if ( pNode->pData != pTheNode )
        return ListAddEnd(pList, pData);
    else
    {
        if ( pNode == pList->pLast )
            return ListAddEnd( pList, pData );
        else
        {
            pNewNode = ListCreateNode( pList, pData );
            if ( pNewNode == NULL )
                return pNewNode;

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
            pNode->pNext->pPrev = pNewNode;    /* link to next node */
            pNewNode->pNext = pNode->pNext;

            pNode->pNext = pNewNode;            /* link to this node */
            pNewNode->pPrev = pNode;

            pList->nLength ++;
   //DRV_OSSemSignal(pList->lock);
   
        }
    }

    return pNewNode;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteBegin
  Description     :   delete the First node in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 


void ListDeleteBegin( list_t* pList )
{
    STBNODE_T *pNode;

	if (pList == NULL)   
		return;
  
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

    pNode = pList->pFirst;                     /* get pointer to node to delete */
    if ( pNode != NULL )
    {
		/*Move the current to the previous if it is one to delete*/
		if ( pList->pCur == pNode )
			pList->pCur = pNode->pNext;
	
        pList->pFirst = pNode->pNext;         /* move head pointer */
        if ( pList->pFirst == NULL )            /* Last item in list */
            pList->pLast = NULL;
        else
            pList->pFirst->pPrev = NULL;       /* reset Previous pointer */

		ListDeleteNode(pList, pNode);
        pList->nLength --;
    }
    
   //DRV_OSSemSignal(pList->lock);
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteEnd
  Description     :   delete the Last node in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void ListDeleteEnd( list_t* pList )
{
    STBNODE_T *pNode;
    
	if (pList == NULL)
		return;
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

    pNode = pList->pLast;					/* get pointer to node to delete */
    if ( pNode != NULL )
    {
		/*Move the current to the previous if it is one to delete*/
		if ( pList->pCur == pNode )
			pList->pCur = pNode->pPrev;

        pList->pLast = pNode->pPrev;		/* move head Last pointer */
        if ( pList->pLast == NULL )			/* Last item in list */
            pList->pFirst = NULL;
        else
            pList->pLast->pNext = NULL;		/* reset Previous pointer */

		ListDeleteNode(pList, pNode);
        pList->nLength --;
    }
   //DRV_OSSemSignal(pList->lock);
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteItem
  Description     :   delete the specified item in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void ListDeleteItem( list_t * pList, void* pData )
{
    STBNODE_T *pNode;

	if ((pList == NULL) || (pData == NULL))
		return;
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);
  
	pNode = pList->pFirst;
	while (pNode != NULL)
	{
		if (pNode->pData == pData)
		{
			break;
		}
		pNode = pNode->pNext;
	}

	/*Delete the specified item if it exists*/
	if (pNode != NULL)
	{
		if (pNode == pList->pFirst)
			ListDeleteBegin(pList);
		else if (pNode == pList->pLast)
			ListDeleteEnd(pList);
		else
		{
			/*Move the current to the next if it is one to delete*/
			if (pNode == pList->pCur)
				pList->pCur = pNode->pNext;
			/*Delete the the located node*/
			pNode->pPrev->pNext = pNode->pNext;
			pNode->pNext->pPrev = pNode->pPrev;
			ListDeleteNode(pList, pNode);
	        pList->nLength --;
		}
	}
   //DRV_OSSemSignal(pList->lock);
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListDeleteCur
  Description     :   delete the current item in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void ListDeleteCur( list_t * pList )
{
	STBNODE_T *pNode = NULL;

  	if (pList == NULL)
		return;
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);


	if (pList->pCur != NULL)
	{
		if (pList->pCur == pList->pFirst)
			ListDeleteBegin(pList);
		else if (pList->pCur == pList->pLast)
			ListDeleteEnd(pList);
		else
		{
			/*Move the current to the next if it is one to delete*/
			pNode = pList->pCur;
			pList->pCur = pNode->pNext;
			/*Delete the the located node*/
			pNode->pPrev->pNext = pNode->pNext;
			pNode->pNext->pPrev = pNode->pPrev;
			ListDeleteNode(pList, pNode);
	        pList->nLength --;
		}
	}
   //DRV_OSSemSignal(pList->lock);
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListMoveHead
  Description     :   move the current to the first in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListMoveHead( list_t *pList )
{
	if (pList != NULL)
	{
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

		pList->pCur = pList->pFirst;
    
   //DRV_OSSemSignal(pList->lock);
   
		if (pList->pCur != NULL)
			return pList->pCur->pData;
	}
	return NULL;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListMoveEnd
  Description     :    move the current to the end in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListMoveEnd( list_t *pList )
{
	if (pList != NULL)
	{

  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

		pList->pCur = pList->pLast;
    
   //DRV_OSSemSignal(pList->lock);
   
		if (pList->pCur != NULL)
			return pList->pCur->pData;
	}
	return NULL;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListMoveNext
  Description     :   move the current to the next in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListMoveNext( list_t *pList )
{
	if (pList != NULL)
	{
	
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

		if (pList->pCur != NULL)
			pList->pCur = pList->pCur->pNext;
		else
			pList->pCur = pList->pFirst;
    
   //DRV_OSSemSignal(pList->lock);

		if (pList->pCur != NULL)
			return pList->pCur->pData;
	}
	return NULL;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListMovePrev
  Description     :   move the current to the previous one in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListMovePrev( list_t *pList )
{
	if (pList != NULL)
	{
	
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

		if (pList->pCur != NULL)
			pList->pCur = pList->pCur->pPrev;
		else
			pList->pCur = pList->pLast;
    
   //DRV_OSSemSignal(pList->lock);

		if (pList->pCur != NULL)
			return pList->pCur->pData;
	}
	return NULL;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListMoveTo
  Description     :  move the current to the specific one in the list
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListMoveTo( list_t *pList, void *pData )
{
  void *Result = NULL;
  
	if (pList != NULL)
   {
   
  	//DRV_OSSemWait(pList->lock, INFINITY_WAIT, 0);

     if ((pList->pCur == NULL)||(pData != pList->pCur->pData))
     {
     	pList->pCur = pList->pFirst;
     	while (pList->pCur)
     	{
     		if (pList->pCur->pData == pData)
     		{     		
             Result = pData;
     			break;
     		}
     		else
     			pList->pCur = pList->pCur->pNext;
     	}
     }
     else
       Result = pData;
     
   //DRV_OSSemSignal(pList->lock);
	}
	return Result;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListGetAt
  Description     :  return the current
  Input           : 
	   		
  Outut           :  
  			None
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void *ListGetAt( list_t *pList )
{
	if (pList != NULL)
	{
		if (pList->pCur != NULL)
			return pList->pCur->pData;
	}
	return NULL;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : ListGetCount
  Description     :  
  Input           : 
	   		
  Outut           :  
  			
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
int ListGetCount(list_t *pList)
{
	return pList->nLength;
}



