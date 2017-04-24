/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosdecompress.c

    Description : 

    Others:      

    History:      1. llh creat 2004-3-24


*******************************************************************************/


/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include "ysosdecompress.h"





/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *	External variable
 *----------------------------------------------------------------------------*/
#if 0

#ifdef STBCFG_ENABLE_LOGO  
	extern CompressData_t ys_logo_compress_data_struct; 
#endif

//extern CompressData_t ys_chinese_compress_data_struct   ; 
extern CompressData_t ys_stbbg_compress_data_struct;
extern  CompressData_t ys_stbbgd_compress_data_struct    ;
extern  CompressData_t ys_stbbgu_compress_data_struct    ;
extern  CompressData_t ys_stbbgm_compress_data_struct    ;
extern  CompressData_t ys_vod480bmp_compress_data_struct    ;
extern  CompressData_t ys_vodbmp_compress_data_struct    ;
extern  CompressData_t ys_vodenter_compress_data_struct    ;
extern  CompressData_t ys_vodinfo_compress_data_struct    ;

extern  CompressData_t ys_hpbmp_compress_data_struct;

extern  CompressData_t ys_hpbut_compress_data_struct;

extern  CompressData_t ys_hpbutf_compress_data_struct;

extern  CompressData_t ys_hpdown_compress_data_struct;

#ifdef TFCAS21
	extern  CompressData_t ys_ca21buy_compress_data_struct;

	extern  CompressData_t ys_ca21buyf_compress_data_struct;
#endif

extern  CompressData_t ys_catbmp_compress_data_struct;

extern  CompressData_t ys_catlable_compress_data_struct;

extern  CompressData_t ys_cattab1bg_compress_data_struct;

extern  CompressData_t ys_cattab1row_compress_data_struct;

extern  CompressData_t ys_cattab1rowf_compress_data_struct;

extern  CompressData_t ys_cattab2row_compress_data_struct;

extern  CompressData_t ys_cattab2rowf_compress_data_struct;

extern  CompressData_t ys_chnlbmp_compress_data_struct;

extern  CompressData_t ys_chnllable_compress_data_struct;

extern  CompressData_t ys_chnltab1bg_compress_data_struct;

extern  CompressData_t ys_chnltab1row_compress_data_struct;

extern  CompressData_t ys_chnltab1rowf_compress_data_struct;

extern  CompressData_t ys_chnltab2row_compress_data_struct;

extern  CompressData_t ys_chnltab2rowf_compress_data_struct;

extern  CompressData_t ys_pobmp_compress_data_struct;

extern  CompressData_t ys_poord_compress_data_struct;

extern  CompressData_t ys_poordf_compress_data_struct;

extern  CompressData_t ys_porec_compress_data_struct;

extern  CompressData_t ys_porecf_compress_data_struct;

extern  CompressData_t ys_potab1bg_compress_data_struct;

extern  CompressData_t ys_potab1row_compress_data_struct;

extern  CompressData_t ys_potab1rowf_compress_data_struct;

extern  CompressData_t ys_potab2row_compress_data_struct;

extern  CompressData_t ys_potab2rowf_compress_data_struct;

extern  CompressData_t ys_stbblue_compress_data_struct;

extern  CompressData_t ys_stbgreen_compress_data_struct;

extern  CompressData_t ys_stbred_compress_data_struct;

extern  CompressData_t ys_stbyellow_compress_data_struct;



/*频道列表*/

extern CompressData_t ys_chnllist_table_bg3_compress_data_struct;
extern CompressData_t ys_chnllist_table_bg5_compress_data_struct;
extern CompressData_t ys_chnllist_table_bg7_compress_data_struct;
extern CompressData_t ys_chnllist_downbanner_compress_data_struct;
extern CompressData_t ys_chnllist_upbanner_compress_data_struct;
extern CompressData_t ys_chnllist_top_compress_data_struct;


extern CompressData_t ys_chnllist_table_f_compress_data_struct;

#endif
#if 0
 /*--------------------------------------------------------------------------
 * Statics
 *--------------------------------------------------------------------------*/

static BOOL_YS  decompresscompleted = FALSE;
static U32  uiFontIndex = 0;  /* 字库解压索引 */
#ifdef STBCFG_ENABLE_LOGO  
static U32  uiLogoIndex = 0; /* logo解压索引 */
#endif

/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/
DRVOS_Task_t*						ptidDecompressTaskId = NULL;


CompressDataIndex_t g_STBDecompressIndex[COMPRESSINDEXLENGTH];

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DecompressProcess
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

static   void 	DRV_DecompressProcess ( void *pvParam )
{
	
	InitDecompressIndex();
	DecompressAllData();
	
}


/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECDecompressInit
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL DRV_DECDecompressInit ( void ) 
{
	
	if ( ( ptidDecompressTaskId = task_create (  DRV_DecompressProcess, 
											NULL,
											EXTRACT_PROCESS_WORKSPACE,
											EXTRACT_PROCESS_PRIORITY,
											"DRV_DecompressProcess", 
											0 ) ) == NULL )
	{

		return   TRUE;
	}
	return FALSE;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECDecompressTerm
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void  DRV_DECDecompressTerm ( void ) 
{
#ifdef EXTRACT_DEBUG	
#endif

	if (ptidDecompressTaskId != NULL)
	{
		task_delete(ptidDecompressTaskId);
		ptidDecompressTaskId = NULL;
	}
	
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : InitDecompressIndex
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 

void InitDecompressIndex(void)
{
	int Index = 0;
	
	for( Index = 0; Index < COMPRESSINDEXLENGTH; Index ++ )
	{
		g_STBDecompressIndex[Index].ucStatus = INDEX_FREE;
		g_STBDecompressIndex[Index].ucDataStatus = DATA_NO_DECOMPRESS;
		g_STBDecompressIndex[Index].pCompressData = NULL;
	}
    Index = 0;






#ifdef STBCFG_ENABLE_LOGO  /* 开机logo  */
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_logo_compress_data_struct;
    uiLogoIndex = Index;
    Index++;
#endif

#if 0
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chinese_compress_data_struct;
    uiFontIndex = Index;  /* 设置字库索引 */
    Index++;
#endif

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_stbbg_compress_data_struct;
    Index++;

	/*频道列表*/
	
    	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_downbanner_compress_data_struct;
    Index++;
    
    	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_upbanner_compress_data_struct;
    Index++;
    
    	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_top_compress_data_struct;
    Index++;
	
    	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_table_bg3_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_table_bg5_compress_data_struct;
    Index++;

        g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_table_bg7_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData= &ys_chnllist_table_f_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_hpbmp_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_hpbut_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_hpbutf_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_hpdown_compress_data_struct;
    Index++;

#ifdef TFCAS21
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_ca21buy_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_ca21buyf_compress_data_struct;
    Index++;
#endif

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_catbmp_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_catlable_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_cattab1bg_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_cattab1row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_cattab1rowf_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_cattab2row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_cattab2rowf_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnlbmp_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnllable_compress_data_struct;
    Index++;

	
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnltab1bg_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnltab1row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnltab1rowf_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnltab2row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_chnltab2rowf_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_pobmp_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_poord_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_poordf_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_porec_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_porecf_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_potab1bg_compress_data_struct;
    Index++;

	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_potab1row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_potab1rowf_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_potab2row_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_potab2rowf_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_stbblue_compress_data_struct;
    Index++;

	
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_stbgreen_compress_data_struct;
    Index++;


	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_stbred_compress_data_struct;
    Index++;

	
	g_STBDecompressIndex[Index].ucStatus = INDEX_IN_USE;
	g_STBDecompressIndex[Index].pCompressData = &ys_stbyellow_compress_data_struct;
    Index++;


	

}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DecompressAllData
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DecompressAllData(void)
{
	
	int iNo;
	for( iNo = 0; iNo < COMPRESSINDEXLENGTH; iNo ++ )
	{
        drv_FlashClaimFlashAccess();
        if( ( g_STBDecompressIndex[iNo].ucStatus == INDEX_IN_USE)
			&&( g_STBDecompressIndex[iNo].ucDataStatus == DATA_NO_DECOMPRESS)
			&&( g_STBDecompressIndex[iNo].pCompressData != NULL))
		{
			g_STBDecompressIndex[iNo].ucDataStatus = DATA_IN_DECOMPRESSING;
			DecompressData( g_STBDecompressIndex[iNo].pCompressData->pCompressDataPointer, 
				      g_STBDecompressIndex[iNo].pCompressData->pDataPointer, 
				      g_STBDecompressIndex[iNo].pCompressData->uiCompressDataLength);
			g_STBDecompressIndex[iNo].ucDataStatus = DATA_DECOMPRESSED;
		}
        drv_FlashReleaseFlashAccess();
	}
	decompresscompleted = TRUE;
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECDecompressPic
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_DECDecompressPic( STOSD_Bitmap_t  DecompressVerify)
{
	int iNo;
	for( iNo = 1; iNo < COMPRESSINDEXLENGTH; iNo ++ )
	{
		if( ( g_STBDecompressIndex[iNo].ucStatus == INDEX_IN_USE)
			&&( g_STBDecompressIndex[iNo].pCompressData != NULL)
			&&( DecompressVerify.Data_p ==  g_STBDecompressIndex[iNo].pCompressData->pDataPointer))
		{
			/*if( g_STBDecompressIndex[iNo].ucDataStatus == DATA_DECOMPRESSED)
				break;*/
			while( ( g_STBDecompressIndex[iNo].ucDataStatus == DATA_IN_DECOMPRESSING)
			    ||( g_STBDecompressIndex[iNo].ucDataStatus == DATA_NO_DECOMPRESS))
			{
				MidPause(300);
			}
			/*
			if( g_STBDecompressIndex[iNo].ucDataStatus == DATA_NO_DECOMPRESS)
			{
				g_STBDecompressIndex[iNo].ucDataStatus = DATA_IN_DECOMPRESSING;
				DecompressData( g_STBDecompressIndex[iNo].pCompressData->pCompressDataPointer, 
					      g_STBDecompressIndex[iNo].pCompressData->pDataPointer, 
					      g_STBDecompressIndex[iNo].pCompressData->uiCompressDataLength);
				g_STBDecompressIndex[iNo].ucDataStatus = DATA_DECOMPRESSED;
				break;
			}*/
			break;

		}
	}
	
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECDecompressCompleted
  Description     : 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_DECDecompressCompleted(void)
{
	return decompresscompleted;
}

/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECFontDecompressCompleted
  Description     : 字库解压完毕
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
BOOL_YS DRV_DECFontDecompressCompleted(void)
{
    return (g_STBDecompressIndex[uiFontIndex].ucDataStatus == DATA_DECOMPRESSED ? TRUE : FALSE);
}
/*****************************************************************************
  Date&Time    :    2004-03-24
  Author          :    llh
  Function Name   : DRV_DECLogoDecompressCompleted
  Description     : logo解压完毕 
  Input           : 
  Outut           :  
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
#ifdef STBCFG_ENABLE_LOGO
BOOL_YS DRV_DECLogoDecompressCompleted(void)
{
#ifdef TFSTB
    return (g_STBDecompressIndex[uiLogoIndex].ucDataStatus == DATA_DECOMPRESSED ? TRUE : FALSE);
#else
    return TRUE;
#endif
}
#endif

#endif


static unsigned char text_buf[RING_BUFFER_SIZE + UPPER_LIMIT - 1];    /* ring buffer of size ring_buffer_size, with extra UPPER_LIMIT-1 bytes to facilitate string comparison */




                     
void DecompressData( const unsigned char *pIndata, unsigned char *pOutdata, int iDataLength)       
{
       int  i, j, k, r, c;
       unsigned int  flags;
       int iReadDataLength = 0;
      

       for (i = 0; i < RING_BUFFER_SIZE - UPPER_LIMIT; i++)
      		text_buf[i] = ' ';
       r = RING_BUFFER_SIZE - UPPER_LIMIT;  
       flags = 0;
       for ( ; ; ) 
       {
               if (((flags >>= 1) & 256) == 0) 
               {
               	c = *pIndata;
               	pIndata ++;
                       if ( iReadDataLength >= iDataLength) 
                       	break;
                       iReadDataLength ++;
                       flags = c | 0xff00;             /* uses higher byte cleverly */
               }                                                       /* to count eight */
               if (flags & 1) 
               {
               	c = *pIndata;
               	pIndata ++;
                       if ( iReadDataLength >= iDataLength) 
                       	break;
                       iReadDataLength ++;
                       
                       *pOutdata = c;
                       pOutdata ++;
                       text_buf[r++] = c;  
                       r &= (RING_BUFFER_SIZE - 1);
               } 
               else 
               {
               	i= *pIndata;
               	pIndata ++;
                       if ( iReadDataLength >= iDataLength) 
                       	break;
                       iReadDataLength ++;
                       
                       	
                       j = *pIndata;
               	pIndata ++;
                       if ( iReadDataLength >= iDataLength) 
                       	break;
                       iReadDataLength ++;
                       
                       	
                       i |= ((j & 0xf0) << 4); 
                       j = (j & 0x0f) + THRESHOLD;
                       for (k = 0; k <= j; k++) 
                       {
                               c = text_buf[(i + k) & (RING_BUFFER_SIZE - 1)];
                               *pOutdata = c;
                     	   pOutdata ++;
                               text_buf[r++] = c; 
                               r &= (RING_BUFFER_SIZE - 1);
                       }
               }
       }
      
}






