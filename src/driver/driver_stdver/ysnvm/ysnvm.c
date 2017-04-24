/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysnvm.c

    Description : e2prom read and write file

    Others:      

    History:      1. yzb creat 2004-3-16
    		  2. wwqing modify 2013-8-28


*******************************************************************************/

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#ifndef STTBX_PRINT
#define STTBX_PRINT
#endif
#include "drv_basic.h"
#include "ysflash.h"

#include "ysnvm.h"
#include "yse2p.h"
/*----------------------------------------------------------------------------
 *	Debug
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *	External variable
 *----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Private Constant & Enum & Macro
 *----------------------------------------------------------------------------*/

/* flash模拟e2prom文件 */
#ifdef FILE_E2Prom
enum FLASH_OP_STATUS
{
	FLASH_OP_STATUS_FAILED = -1,
	FLASH_OP_STATUS_OK = 0,		
	FLASH_OP_STATUS_BAD_BLOCK = 1
};

enum BLOCK_e
{
	BLOCK_MAIN = 0,
	BLOCK_BAK = 1
};

static FILE *E2PROM_fd = NULL;
#define E2PROM_FILE_NAME  "/FlashData/file_e2prom"
#define NVM_FLASH_BLOCK_SIZE 128*1024   //flash块大小
#define NVM_FLASH_PAGE_SIZE  2*1024     //flash页大小
#define NVM_FLASH_BLOCK_PAGE_NUM  64    //block的页数
#define NVM_STBINFO_SIZE     300        //保存在flash stbinfo分区的数据大小
#define NVM_PAGE_FLAG_SIZE 4            //flash页读写标记长度
#define FLASH_DEV_NAME "stbinfo"

static S8_YS mPageNum = 0xFF;  			//应该读写flash 的page
static U8_YS mBakIsUse = BLOCK_MAIN;    //是否使用了备份数据
static U8_YS mPageFlag[4] = {0xbe,0xbe,0xbe,0xbe};  //flash是否读写当前page的标志
#endif

/* 24C64, 8K E2PROM */
#define NVM_END		0x2000   

#define NVM_FACT_CFG_HWCFG_OFFSET (NVM_FACT_CFG_ADDR+4) 
#define NVM_FACT_CFG_HWCFG_SIZE 4


/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Private Module Variable
 *----------------------------------------------------------------------------*/
static U8_YS m_NVM_Image[NVM_END];
static BOOL_YS IsFactCfgInited = FALSE;

/*----------------------------------------------------------------------------
 *	Private Function Prototype
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Function
 *----------------------------------------------------------------------------*/

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMReadtoImage
  Description     : 将E2P数据拷贝到内存
  Input           : 
  Outut           : 成功返回0; 失败返回1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
S32_YS DRV_NVMReadtoImage(void)
{
#ifdef FILE_E2Prom
    S32_YS ret;
	S32_YS PageIndex = NVM_FLASH_BLOCK_PAGE_NUM;
	unsigned char *tmp_flash_buff = NULL;

	DRV_OS_SemWait(m_StbInfoRWMutex,DRVOS_WAIT_INFINITY,0); 

    E2PROM_fd = fopen(E2PROM_FILE_NAME, "r");
    if(NULL == E2PROM_fd)
    {
        E2PROM_fd = fopen(E2PROM_FILE_NAME, "w");
        if(NULL == E2PROM_fd)
        {
            printf("> %d Line: %s, make file error\n",__LINE__, __func__);
			DRV_OSSemSignal(m_StbInfoRWMutex);
            return -1;
        }
        memset(m_NVM_Image, 0xff, NVM_END);
        /* 创建NVM_END大小文件 */
        fwrite(m_NVM_Image, 1, NVM_END, E2PROM_fd);        
        fclose(E2PROM_fd);
        /* 第一次开机也要读取flash内容 */
        goto read_flash;
    }
    
    ret = fread(m_NVM_Image, 1, NVM_END, E2PROM_fd);
    if(NVM_END != ret)
    {
        printf("> %d Line: %s, read file e2prom error ret= % d\n",__LINE__, __func__, ret);
		DRV_OSSemSignal(m_StbInfoRWMutex);
		fclose(E2PROM_fd);
        return -1;
    }
    
    fclose(E2PROM_fd);

read_flash:/* 将前300字节读取到m_NVM_Image中*/

	//Open stbinfo
    ret = DRV_FLASH_OPEN(FLASH_DEV_NAME);
    if(FLASH_OP_STATUS_FAILED == ret)
    {
        printf("> %d Line: %s, DRV_FLASH_OPEN error ret= % d\n",__LINE__, __func__, ret);
		DRV_OSSemSignal(m_StbInfoRWMutex); 
        return -1;
    }

	//Mallc a Flash Page
	tmp_flash_buff = (unsigned char *)DRV_OSMalloc(SystemPartition, NVM_FLASH_PAGE_SIZE);
	if(NULL == tmp_flash_buff)
	{
		printf("> %d Line: %s, DRV_OSMalloc failed!!\n",__LINE__, __func__);
		DRV_OSSemSignal(m_StbInfoRWMutex);
        return -1;
	}
	
	memset(tmp_flash_buff, 0xFF, NVM_FLASH_PAGE_SIZE);

	//Get Read or Wite Page index, Search from the Main Block and the MAX page(63)
	PageIndex = NVM_FLASH_BLOCK_PAGE_NUM - 1;
	while(PageIndex >= 0)
	{
		ret = DRV_FLASH_READ(PageIndex * NVM_FLASH_PAGE_SIZE + mBakIsUse * NVM_FLASH_BLOCK_SIZE, tmp_flash_buff, NVM_FLASH_PAGE_SIZE);
	    if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_MAIN == mBakIsUse)) //主数据块碰到坏块 read flash退出，使用备份block中的数据
			||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_MAIN == mBakIsUse)))//主数据块碰到其它错误 read flash退出，使用备份block中的数据
		{
			mBakIsUse = BLOCK_BAK;
			PageIndex = NVM_FLASH_BLOCK_PAGE_NUM - 1;
			printf("[WARRING]> %d Line: %s, DRV_FLASH_READ BLOCK_MAIN is error! ret[%d]\n", __LINE__, __func__, ret);
			
			continue;
	    }
		else if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_BAK == mBakIsUse)) //备用数据块碰到坏块 read flash退出，读取失败返回-1
			||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_BAK == mBakIsUse)))		 //备用数据块碰到其它错误 read flash退出，读取失败返回-1
		{
			printf("[ERROR]> %d Line: %s, DRV_FLASH_READ BLOCK_MAIN and BLOCK_BAK is all error!! ret[%d]\n",__LINE__, __func__, ret);

			DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
    		tmp_flash_buff = NULL;
			DRV_FLASH_CLOSE();
			DRV_OSSemSignal(m_StbInfoRWMutex); 
			return -1;
	    }

		//Check the 'PageFlag' find valid data
		if(0 == (memcmp(tmp_flash_buff + NVM_FLASH_PAGE_SIZE - NVM_PAGE_FLAG_SIZE, mPageFlag, NVM_PAGE_FLAG_SIZE)))
		{
			mPageNum = PageIndex;
			printf("[INFO]> %d Line: %s, DRV_FLASH_READ block[%d] is page[%d]...\n", __LINE__, __func__, mBakIsUse, mPageNum);
			break;
		}	

		PageIndex --;
	}

	if (PageIndex < 0)
    {
		printf("[ERROR]> %d Line: \n\n[ERROR!!!]%s, DRV_FLASH_READ error，no mPageFlag\n",__LINE__ , __func__);
		DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
	    tmp_flash_buff = NULL;
		DRV_FLASH_CLOSE();
		DRV_OSSemSignal(m_StbInfoRWMutex); 
        return -1;
    }
			

    /* 将有用的300字节数据拷贝到m_NVM_Image中 */
    memcpy(m_NVM_Image, tmp_flash_buff, NVM_STBINFO_SIZE);
	
	DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
	tmp_flash_buff = NULL;
    DRV_FLASH_CLOSE();
	DRV_OSSemSignal(m_StbInfoRWMutex); 

    return 0;
#else
    S32_YS  returnword;

    returnword = HI_UNF_E2PROM_Read(0, m_NVM_Image, NVM_END);
    if(returnword != ST_NO_ERROR)
    {
        STTBX_Print(("DRV_NVMReadtoImage ErrorCode is 0x%x\n",returnword));
		return TRUE_YS;
    }
	return FALSE_YS;    
#endif
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMRead
  Description     : 读取e2p指定位置、长度的数据
  Input           : 
  Outut           : 成功返回0; 失败返回1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
HI_S32 DRV_NVMRead (HI_S32 iOffset, HI_S32 nLength, HI_U8 *data)
{
   	if ((iOffset + nLength) > NVM_END)
   	{
		Err_Print("\n\n-->>DRV_NVMRead failed, iOffset[%d] + nLength[%d] > 0x2000\n\n", iOffset, nLength);
        return TRUE_YS;
   	}
   	else
   	{
		memcpy((HI_U8*)data, m_NVM_Image + iOffset, nLength);
        return FALSE_YS;    
   	}
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMWrite
  Description     : 向e2p写入指定位置、长度的数据
  Input           : 
  Outut           : 成功返回0; 失败返回1 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 采用多次写，防止写失败
*****************************************************************************/ 
HI_S32 DRV_NVMWrite ( HI_S32 iOffset, HI_S32 nLength, HI_U8 *data )
{
    S32_YS returnword = 0;
    U32_YS indexCont = 0;    
    S32_YS ret = 0;

    if ((iOffset + nLength) > NVM_END)
    {
		Err_Print("\n\n-->>DRV_NVMWrite failed, iOffset[%d] + nLength[%d] > 0x2000\n\n", iOffset, nLength);
        return TRUE_YS;
    }
    else
    {   
		DRV_OS_SemWait(m_StbInfoRWMutex,DRVOS_WAIT_INFINITY,0); 

		//save data in memory first
        memcpy(m_NVM_Image + iOffset, (U8_YS*)data, nLength);
        
#ifdef FILE_E2Prom
		S32_YS PageIndex;
	    unsigned char *tmp_flash_buff = NULL;
		U32_YS indexCont2 = 0; 

		/* 将flash数据保存到stbinfo分区flash */
        if(iOffset < NVM_STBINFO_SIZE)
        {	
			//Open stbinfo
            ret = DRV_FLASH_OPEN(FLASH_DEV_NAME);
            if(FLASH_OP_STATUS_FAILED == ret)
            {
                printf("> %d Line: %s, DRV_FLASH_OPEN error ret= % d\n",__LINE__ , __func__, ret);
				DRV_OSSemSignal(m_StbInfoRWMutex); 
                return -1;
            }

			//Mallc a Flash Page
			tmp_flash_buff = (unsigned char *)DRV_OSMalloc(SystemPartition, NVM_FLASH_PAGE_SIZE);
			if(NULL == tmp_flash_buff)
			{
				printf("> %d Line: %s, DRV_OSMalloc failed!!\n",__LINE__ , __func__);
				DRV_OSSemSignal(m_StbInfoRWMutex); 
		        return -1;
			}
			memset(tmp_flash_buff, 0xFF, NVM_FLASH_PAGE_SIZE);

			//遍历block中的页的数据，确定下一次从哪个页开始写
			{	
				PageIndex = NVM_FLASH_BLOCK_PAGE_NUM - 1;
				while(PageIndex >= 0)
				{
				    ret = DRV_FLASH_READ(PageIndex * NVM_FLASH_PAGE_SIZE + mBakIsUse * NVM_FLASH_BLOCK_SIZE, tmp_flash_buff, NVM_FLASH_PAGE_SIZE);
				    if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_MAIN == mBakIsUse)) //主数据块碰到坏块 read flash退出，使用备份block中的数据
						||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_MAIN == mBakIsUse)))//主数据块碰到其它错误 read flash退出，使用备份block中的数据
					{
						mBakIsUse = BLOCK_BAK;
						PageIndex = NVM_FLASH_BLOCK_PAGE_NUM - 1;
						printf("[WARRING]> %d Line: %s, DRV_FLASH_READ BLOCK_MAIN is bad!!! ret[%d]\n", __LINE__, __func__, ret);
						
						continue;
				    }
					else if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_BAK == mBakIsUse)) //备用数据块碰到坏块 read flash退出，读取失败返回-1
						||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_BAK == mBakIsUse)))	 //备用数据块碰到其它错误 read flash退出，读取失败返回-1
					{
						printf("[ERROR]> %d Line: %s, DRV_FLASH_READ BLOCK_MAIN and BLOCK_BAK is all bad!!! ret[%d]\n", __LINE__, __func__, ret);

						DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
						tmp_flash_buff = NULL;
						DRV_FLASH_CLOSE();
						DRV_OSSemSignal(m_StbInfoRWMutex);
						return -1;
				    }
					
					//Check the 'PageFlag' find valid data
					if(0 == (memcmp(tmp_flash_buff + NVM_FLASH_PAGE_SIZE - NVM_PAGE_FLAG_SIZE, mPageFlag, NVM_PAGE_FLAG_SIZE)))
					{
						mPageNum = PageIndex + 1;
						printf("[INFO]> %d Line: %s, DRV_FLASH_READ Get Write Block[%d], Page[%d]!!!\n", __LINE__, __func__, mBakIsUse, mPageNum);
						
						break;
					}	

					PageIndex --;
				}
			}

			/*如果没有数据或整个block都写满的，就erase flash*/
			while((PageIndex < 0) || (PageIndex == 63))
			{
				printf("[INFO]> %d Line: %s, DRV_FLASH_READ Do Not Find Write Page in Block[%d], Clean the whole Block!!!\n", __LINE__, __func__, mBakIsUse);
						
		        ret = DRV_FLASH_ERASE(mBakIsUse * NVM_FLASH_BLOCK_SIZE, NVM_FLASH_BLOCK_SIZE);
		        if((FLASH_OP_STATUS_OK == ret) && (BLOCK_MAIN == mBakIsUse)) //主数据块擦除成功后，erase备份块
		        {
					printf("[INFO]> %d Line: %s, Clean the Main Block ok!!!\n", __LINE__, __func__);
					
					ret = DRV_FLASH_ERASE(NVM_FLASH_BLOCK_SIZE, NVM_FLASH_BLOCK_SIZE);
					if(FLASH_OP_STATUS_OK == ret)
					{
						printf("[INFO]> %d Line: %s, Clean the BAK Block ok!!!\n", __LINE__, __func__);
					}
					else
					{
						printf("[WARRING]> %d Line: %s, Clean the BAK Block Failed!!! ret[%d]\n", __LINE__, __func__, ret);
					}
		        }
				else if((FLASH_OP_STATUS_OK == ret) && (BLOCK_BAK == mBakIsUse)) //备用数据块擦除成功
		        {
					printf("[INFO]> %d Line: %s, Clean the BAK Block ok!!!\n", __LINE__, __func__);
		        }
				else if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_MAIN == mBakIsUse))
					||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_MAIN == mBakIsUse)))
				{
					mBakIsUse = BLOCK_BAK;
					printf("[WARRING]> %d Line: %s, DRV_FLASH_ERASE BLOCK_MAIN is failed, ret[%d]!!! \n",__LINE__, __func__, ret);
					continue;
				}
				else if(((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_BAK == mBakIsUse))
					||((FLASH_OP_STATUS_FAILED == ret) && (BLOCK_BAK == mBakIsUse)))
				{
					printf("[ERROR]> %d Line: %s, DRV_FLASH_ERASE BLOCK_BAK is failed, ret[%d]!!! \n", __LINE__, __func__, ret);

					DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
					tmp_flash_buff = NULL;
					DRV_FLASH_CLOSE();
					DRV_OSSemSignal(m_StbInfoRWMutex); 
					return -1;	
				}

				//将当前可操作页号置为当前块的0
				PageIndex = 0;
				mPageNum = 0;
			}

			/*拷贝写入数据，并把标志置为读写区域*/
			memset((HI_U8*)tmp_flash_buff, 0xFF, NVM_FLASH_PAGE_SIZE);
			memcpy((HI_U8*)tmp_flash_buff, m_NVM_Image, NVM_FLASH_PAGE_SIZE);
			memcpy((HI_U8*)tmp_flash_buff + NVM_FLASH_PAGE_SIZE - NVM_PAGE_FLAG_SIZE, mPageFlag, NVM_PAGE_FLAG_SIZE);
		
			//写flash, 多次写，防止写失败
			for(indexCont = 0; indexCont < 6; indexCont ++)
			{
				printf("[INFO]> %d Line: %s, Write Data in Block[%d] Page[%d].\n", __LINE__, __func__, mBakIsUse, mPageNum);
		        ret = DRV_FLASH_WRITE(mPageNum * NVM_FLASH_PAGE_SIZE + mBakIsUse * NVM_FLASH_BLOCK_SIZE, tmp_flash_buff, NVM_FLASH_PAGE_SIZE);
		        if((FLASH_OP_STATUS_OK == ret) && (BLOCK_MAIN == mBakIsUse))
		        {
					printf("[INFO]> %d Line: %s, Write Data in Block[%d] Page[%d] OK.\n", __LINE__, __func__, mBakIsUse, mPageNum);
					//将数据写入备用数据块
		        	ret = DRV_FLASH_WRITE(mPageNum * NVM_FLASH_PAGE_SIZE + NVM_FLASH_BLOCK_SIZE, tmp_flash_buff , NVM_FLASH_PAGE_SIZE);
					if(FLASH_OP_STATUS_OK == ret)
					{
						printf("[INFO]> %d Line: %s, Write Data in Block[bak] Page[%d] OK.\n", __LINE__, __func__, mPageNum);
						break;
					}
					else
					{
						printf("[WARRING]> %d Line: %s, DRV_FLASH_WRITE BLOCK_BAK is failed!!! ret[%d], try again[%d]...\n", __LINE__, __func__, ret, indexCont+1);
					}
		        }
				else if((FLASH_OP_STATUS_OK == ret) && (BLOCK_BAK == mBakIsUse))
		        {
					printf("[INFO]> %d Line: %s, Write Data in Block[%d] Page[%d] OK.\n", __LINE__, __func__, mBakIsUse, mPageNum);
					break;
		        }
				else if((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_MAIN == mBakIsUse))//主数据块碰到坏块，使用备份block
				{
					mBakIsUse = BLOCK_BAK;
					indexCont = 0; 
		            printf("> %d Line: %s, DRV_FLASH_READ BLOCK_MAIN is bad!!! ret[%d]\n", __LINE__, __func__, ret);
		        }
				else if((FLASH_OP_STATUS_BAD_BLOCK == ret) && (BLOCK_BAK == mBakIsUse))     //两份数据block都坏块，退出
				{
					printf("> %d Line: %s, DRV_FLASH_WRITE BLOCK_MAIN and BLOCK_BAK is bad!!! ret[%d]\n", __LINE__, __func__, ret);
					DRV_OSFree(SystemPartition,(U32)tmp_flash_buff);
					tmp_flash_buff = NULL;
					DRV_FLASH_CLOSE();
					DRV_OSSemSignal(m_StbInfoRWMutex); 
					return -1;	
				}
				else
				{
					printf("> %d Line: %s, DRV_FLASH_WRITE error ret = %d, try again[%d]...\n", __LINE__, __func__, ret, indexCont+1);
				}
			}
			
			if (indexCont != 0)
			{
				if (indexCont == 6)
				{
					printf("> %d Line: \n\n[ERROR!!!]%s, DRV_FLASH_WRITE error ret= % d\n",__LINE__ , __func__, ret);
				}
				else
				{
					printf("> %d Line: \n\n[warning!!!]%s, DRV_FLASH_WRITE error ret= % d, and retry %d time\n",__LINE__ , __func__, ret, indexCont);
				}
			}

            DRV_FLASH_CLOSE();
			
			DRV_OSFree(SystemPartition, (U32)tmp_flash_buff);
			tmp_flash_buff = NULL;
        }		
		 
		//将数据写入file_e2prom
        E2PROM_fd = fopen(E2PROM_FILE_NAME, "r+");
        if(NULL == E2PROM_fd)
        {
            printf("> %d Line: %s  open file e2prom error\n",__LINE__ , __func__);
			DRV_OSSemSignal(m_StbInfoRWMutex); 
            return -1;
        }
		
        ret = fseek(E2PROM_fd, iOffset, SEEK_SET);
        if(-1 == ret)
        {
            printf("> %d Line: %s fseek error\n",__LINE__ , __func__);
			DRV_OSSemSignal(m_StbInfoRWMutex); 
            return -1;
        }
        
        ret = fwrite(data, 1, nLength, E2PROM_fd);
        if(ret != nLength)
        {
            printf("> %d Line: %s write file e2prom error ret = %d, nLength = %d iOffset = %d \n",__LINE__ , __func__, ret, nLength, iOffset);
			DRV_OSSemSignal(m_StbInfoRWMutex); 
            return -1;
        }
		
        fclose(E2PROM_fd);
		DRV_OSSemSignal(m_StbInfoRWMutex); 

		return 0;
#else
        for(indexCont = 0; indexCont < 6; indexCont++)
        {
            returnword = (S32_YS)HI_UNF_E2PROM_Write(iOffset, data, nLength);
            if(returnword == ST_NO_ERROR)
            {
                break;
            }
        }

        if(indexCont != 0)
        {
            if(indexCont == 6)
            {
                STTBX_Print(("DRV E2P:wirte e2p error is:%d----\n",returnword));
            }
            else
            {
                STTBX_Print(("warning!!!DRV E2P:wirte e2p error,and retry %d time\n",indexCont));	
            }
        }
		
		DRV_OSSemSignal(m_StbInfoRWMutex); 
		return returnword;
#endif
    }

    
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_NVMFlush
  Description     : 将E2P数据区域置为0xFF
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_NVMFlush(void)
{
	U32 Index;
	U8  Data = 0xFF;
	
	for(Index=0;Index<8*1024;Index++)
	{
		DRV_NVMWrite(Index,1, &Data);
	}
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_FactInited
  Description     : 获取E2p中FactCfg区域是否已经被初始化过
  Input           : 
  Outut           : 被初始化过，返回1，否则返回0
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_FactInited(void)
{
    return IsFactCfgInited;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadFactCfg
  Description     : 获取E2p中FactCfg区域的数据
  Input           : 
  Outut           : 如果是有效数据，返回0，否则返回1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_ReadFactCfg(U8_YS *pData)
{
	DRV_NVMRead(NVM_FACT_CFG_ADDR, NVM_FACT_CFG_SIZE,(U8_YS *)pData);
	if(*(U32_YS *)pData != 0xbeebebee)	/*出厂设置数据的初始化标记*/
    {
        return TRUE_YS;	/* 无初始化出厂设置数据*/
    }
	else 
    {
        IsFactCfgInited = TRUE_YS;
        return FALSE_YS;
	}
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadFactCfg
  Description     : 向E2p中FactCfg区域写入factcfg数据
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 如果不是第一次写入FactCfg，需要写入0xbeebebee作为初始化标记
*****************************************************************************/ 
void DRV_WriteFactCfg(U8_YS *pData)
{
	S32_YS i;
	U32_YS Word;
	U8_YS * pHWCFG;
	U32_YS checksum = 0;
	
	Word = *(U32_YS *)pData;
	DRV_NVMRead(NVM_FACT_CFG_ADDR, 4, (U8_YS*)&Word);

	if(Word == 0xbeebebee)//不是第一次初始化
	{
		pHWCFG=pData+4;
		DRV_NVMRead (NVM_FACT_CFG_HWCFG_OFFSET,NVM_FACT_CFG_HWCFG_SIZE,pHWCFG);
		DRV_NVMWrite(NVM_FACT_CFG_ADDR, NVM_FACT_CFG_SIZE-4, (U8_YS *)pData);
		for(i=0; i<(NVM_FACT_CFG_SIZE-4)/4; i++)
			checksum += *(&Word + i);
		DRV_NVMWrite(NVM_FACT_CFG_ADDR+ NVM_FACT_CFG_SIZE-4, 4, (U8_YS *)&checksum);
	}
	else//是第一次初始化
	{
		Word = 0xbeebebee;		/* 初始化标记*/
		DRV_NVMWrite(NVM_FACT_CFG_ADDR, NVM_FACT_CFG_SIZE-4, (U8_YS *)pData);
		for(i=0; i<(NVM_FACT_CFG_SIZE-4)/4; i++)
			checksum += *(&Word + i);
		DRV_NVMWrite(NVM_FACT_CFG_ADDR+ NVM_FACT_CFG_SIZE-4, 4, (U8_YS *)&checksum);
	}		
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadAppCfg
  Description     : 获取E2p中AppCfg区域数据
  Input           : 
  Outut           : 如果是有效数据，返回0，否则返回1
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_ReadAppCfg(U8_YS *pData)
{
	DRV_NVMRead(NVM_APP_CFG_ADDR, NVM_APP_CFG_SIZE,(U8_YS *)pData);
	if(*(U32_YS *)pData != 0xbeebebee)	/*出厂设置数据的初始化标记*/
		return TRUE_YS;	/* 无初始化出厂设置数据*/
	else 
		return FALSE_YS;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteAppCfg
  Description     : 写入E2p中AppCfg区域数据
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteAppCfg(U8_YS *pData)
{
	U32_YS *pWord;
	
	pWord = (U32_YS *)pData;
	*pWord = 0xbeebebee;		/* 初始化标记*/
	DRV_NVMWrite(NVM_APP_CFG_ADDR, NVM_APP_CFG_SIZE, (U8_YS *)pData);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeMark
  Description     : 写入强制升级标识符0x51DB
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeMark()
{
	U16_YS data =0x51DB;	/* 强制升级标识*/
	DRV_NVMWrite(NVM_UPGRADE_MARK_ADDR, NVM_UPGRADE_MARK_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearUpgradeMark
  Description     : 擦除强制升级标识符
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearUpgradeMark()
{
	U16_YS data =0xFFFF;	/* 强制升级标识*/
	DRV_NVMWrite(NVM_UPGRADE_MARK_ADDR, NVM_UPGRADE_MARK_SIZE,(U8_YS *)&data);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPUpgrade
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteIPUpgrade(void)
{
	U16_YS data = 0xa622;	

	DRV_NVMWrite(NVM_UPGRADE_MARK_ADDR,NVM_UPGRADE_MARK_SIZE, (U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPUpgradeNoAppMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteIPUpgradeNoAppMark(void)
{
	U16_YS data =0xB05A;	/* 强制升级标识*/
	
	DRV_NVMWrite(NVM_UPGRADE_MARK_ADDR, NVM_UPGRADE_MARK_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeMark2
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeMark2(void)
{
	U16 data =0x40CA;	/* 强制升级标识2*/
	
	DRV_NVMWrite(NVM_UPGRADE_MARK_ADDR, NVM_UPGRADE_MARK_SIZE,( U8_YS *)&data);
	STTBX_Print(("\n DRV_WriteUpgradeMark2() !\n"));

}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeMark2
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U16 DRV_ReadUpgradeMark2(void)
{
	U16 data = 0;	
	
	DRV_NVMRead(NVM_UPGRADE_MARK_ADDR, NVM_UPGRADE_MARK_SIZE,( U8_YS *)&data);
	STTBX_Print(("\n DRV_ReadUpgradeMark2()=0x%x !\n",data));
	return data;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeType
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
U16_YS DRV_ReadUpgradeType(void)
{
	U16_YS data = 0;	

	DRV_NVMRead(NVM_UPGRADE_MARK_ADDR,NVM_UPGRADE_MARK_SIZE, (U8_YS *)&data);
    return data;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteApplicationMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteApplicationMark(void)
{
	U16_YS data =0x51DB;	/* 强制升级标识*/
	DRV_NVMWrite(NVM_APPLICATION_MARK_ADDR, NVM_APPLICATION_MARK_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearApplicationMark
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearApplicationMark(void)
{
	U16_YS data =0xFFFF;	/* 强制升级标识*/
	DRV_NVMWrite(NVM_APPLICATION_MARK_ADDR, NVM_APPLICATION_MARK_SIZE,(U8_YS *)&data);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeTS
  Description     : 写入升级频点信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeTS(U32_YS Fre,U16_YS Sym, U16_YS Qam)
{
	U32_YS data[2];

	data[0] = Fre;
	data[1] = (Sym << 16) + Qam;
	DRV_NVMWrite(NVM_UPGRADE_TS_ADDR, /*NVM_UPGRADE_TS_SIZE*/8,(U8_YS *)data);

	return;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeTS
  Description     : 读取升级频点信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ReadUpgradeTS(U32* pFre,U16* pSym, U8* pQam)
{
    U32 data[2];
    DRV_NVMRead(NVM_UPGRADE_TS_ADDR, 8/*NVM_UPGRADE_TS_SIZE*/,(U8_YS *)data);
    *pFre = data[0];
    *pSym = data[1] >> 16;
    *pQam = data[1] & 0xFFFF;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUpgradeUMT
  Description     : 写入升级UMT PID和TableID信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteUpgradeUMT(U16_YS pid,U16_YS tableid)
{
	U32_YS data;

	data = (pid << 16) + tableid;
	DRV_NVMWrite(NVM_UPGRADE_UMT_ADDR, NVM_UPGRADE_UMT_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUpgradeUMT
  Description     : 读取升级UMT PID和TableID信息
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ReadUpgradeUMT(U16* pPid, U16* pTableid)
{
	U32 data;

	DRV_NVMRead(NVM_UPGRADE_UMT_ADDR, NVM_UPGRADE_UMT_SIZE,(U8_YS *)&data);
	*pPid = data >> 16;
	*pTableid = data & 0xFFFF;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDefaultSettingTag
  Description     : 写入默认配置tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_WriteDefaultSettingTag()
{
	U16_YS data =0x51DB;	/* 默认设置标识*/
	
	DRV_NVMWrite(NVM_DEFAULT_SETTING_TAG_ADDR, NVM_DEFAULT_SETTING_TAG_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ClearDefaultSettingTag
  Description     : 清除默认配置tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
void DRV_ClearDefaultSettingTag()
{
	U16_YS data =0xFFFF;	/* 默认设置标识*/
	
	DRV_NVMWrite(NVM_DEFAULT_SETTING_TAG_ADDR, NVM_DEFAULT_SETTING_TAG_SIZE,(U8_YS *)&data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_CheckDefaultSettingTag
  Description     : 检测默认配置tag
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/ 
BOOL_YS DRV_CheckDefaultSettingTag()
{
	U16_YS data;	/* 默认设置标识*/
	
	DRV_NVMRead(NVM_DEFAULT_SETTING_TAG_ADDR, NVM_DEFAULT_SETTING_TAG_SIZE,(U8_YS *)&data);

	if( data == 0x51db)
		return TRUE_YS;
	else
		return FALSE_YS;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannel
  Description     : 读取关机前的频道(当前频道)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般开机调用，获取关机前的频道
*****************************************************************************/ 
U16_YS DRV_DTVReadChannel(void)
{
	U16_YS chn;

	DRV_NVMRead(DTV_NVM_CHANNEL_ADDR,DTV_NVM_CHANNEL_SIZE, (U8_YS*)&chn);
	return chn;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannel
  Description     : 写入关机前的频道(当前频道)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般是切台后调用，保存当前频道
*****************************************************************************/ 
void DRV_DTVWriteChannel(U16_YS chn)
{
	DRV_NVMWrite(DTV_NVM_CHANNEL_ADDR,DTV_NVM_CHANNEL_SIZE,(U8_YS *)&chn);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannelLockPassword
  Description     : 读取频道锁密码
  Input           : 
  Outut           : 返回密码
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般是用户密码
*****************************************************************************/ 
U32_YS DRV_DTVReadChannelLockPassword(void)
{
	U32_YS uiPassword;

	DRV_NVMRead(NVM_CHANNEL_LOCK_PASSWORD_ADDR,NVM_CHANNEL_LOCK_PASSWORD_SIZE, (U8_YS *)&uiPassword);
	return uiPassword;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannelLockPassword
  Description     : 写入频道锁密码
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般是用户密码
*****************************************************************************/ 
void DRV_DTVWriteChannelLockPassword(U32_YS uiPassword)
{
	DRV_NVMWrite(NVM_CHANNEL_LOCK_PASSWORD_ADDR,NVM_CHANNEL_LOCK_PASSWORD_SIZE,(U8_YS *)&uiPassword);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadChannelLockPasswordModifiedTag
  Description     : 读取频道锁密码修改标识
  Input           : 
  Outut           : 返回频道锁密码修改标识
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般是用户密码
*****************************************************************************/ 
S8_YS DRV_DTVReadChannelLockPasswordModifiedTag()
{
	S8_YS scTag;

	DRV_NVMRead(NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_ADDR,NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_SIZE, (U8_YS *)&scTag);
	return scTag;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteChannelLockPasswordModifiedTag
  Description     : 写入频道锁密码修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 一般是用户密码
*****************************************************************************/ 
void DRV_DTVWriteChannelLockPasswordModifiedTag(S8_YS  scTag)
{
	DRV_NVMWrite(NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_ADDR,NVM_CHANNEL_LOCK_PASSWORD_MODIFIED_TAG_SIZE,(U8_YS *)&scTag);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadChnlOrderChangedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadChnlOrderChangedTag(void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_CHNL_ORDER_CHANGED_TAG_ADDR,NVM_CHNL_ORDER_CHANGED_TAG_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteChnlOrderChangedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteChnlOrderChangedTag(U8_YS vol)
{
	DRV_NVMWrite(NVM_CHNL_ORDER_CHANGED_TAG_ADDR,NVM_CHNL_ORDER_CHANGED_TAG_SIZE, &vol);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNetworkID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNetworkID(void)
{
	U16_YS chn;

	DRV_NVMRead(NVM_REGION_NETWORK_ID_ADDR,NVM_REGION_NETWORK_ID_SIZE, (U8_YS *)&chn);
	return chn;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNetworkID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNetworkID(U16_YS chn)
{
	DRV_NVMWrite(NVM_REGION_NETWORK_ID_ADDR,NVM_REGION_NETWORK_ID_SIZE,(U8_YS *)&chn);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadPDTVersionNO
  Description     : 获取九州广告的版本号
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadPDTVersionNO(void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_PDT_VERSION_NO_ADDR,NVM_PDT_VERSION_NO_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WritePDTVersionNO
  Description     : 写入九州广告的版本号
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WritePDTVersionNO(U8_YS vol)
{
	DRV_NVMWrite(NVM_PDT_VERSION_NO_ADDR,NVM_PDT_VERSION_NO_SIZE, &vol);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStockChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadStockChannelNO(void)
{
	U16_YS	usChannelNO;

	DRV_NVMRead( NVM_STOCK_CHANNEL_NO_ADDR, NVM_STOCK_CHANNEL_NO_SIZE, (U8_YS *)&usChannelNO);
	return usChannelNO;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteStockChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteStockChannelNO(U8_YS usChannelNO)
{
	DRV_NVMWrite(NVM_STOCK_CHANNEL_NO_ADDR,NVM_STOCK_CHANNEL_NO_SIZE, (U8_YS *)&usChannelNO);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadLanguageSetting
  Description     : 获取语言设置
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadLanguageSetting()
{
	U8_YS	vol;

	DRV_NVMRead(NVM_LANGUAGE_SETTING_ADDR,NVM_LANGUAGE_SETTING_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteLanguageSetting
  Description     : 写入语言设置
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteLanguageSetting(U8_YS vol)
{
	DRV_NVMWrite(NVM_LANGUAGE_SETTING_ADDR,NVM_LANGUAGE_SETTING_SIZE, &vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCARegionCode
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadCARegionCode(void)
{
	U32_YS	vol;

	DRV_NVMRead(NVM_CA_REGION_CODE_ADDR,NVM_CA_REGION_CODE_SIZE,(U8_YS *)&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCARegionCode
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCARegionCode(U32_YS vol)
{
	DRV_NVMWrite(NVM_CA_REGION_CODE_ADDR,NVM_CA_REGION_CODE_SIZE, (U8_YS *)&vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDBCTS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadDBCTS(U32_YS *Fre,U16_YS *Sym, U8 *Qam)
{
	U32_YS data[2];

	
	DRV_NVMRead(NVM_DBC_TS_ADDR, NVM_DBC_TS_SIZE,(U8_YS *)data);

	*Fre = data[0];
	*Sym = data[1] >>16;
	*Qam = data[1]&0xff; 
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDBCTS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDBCTS(U32_YS Fre,U16_YS Sym, U8 Qam)
{
	U32_YS data[2];

	data[0] = Fre;
	data[1] = (Sym << 16) + Qam;
	DRV_NVMWrite(NVM_DBC_TS_ADDR, NVM_DBC_TS_SIZE,(U8_YS *)data);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCityEyesChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadCityEyesChannelNO()
{
	U16_YS	usChannelNO;

	DRV_NVMRead( NVM_CITY_EYES_CHANNEL_NO_ADDR, NVM_CITY_EYES_CHANNEL_NO_SIZE, (U8_YS *)&usChannelNO);
	return usChannelNO;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCityEyesChannelNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCityEyesChannelNO(U8_YS usChannelNO)
{
	DRV_NVMWrite(NVM_CITY_EYES_CHANNEL_NO_ADDR,NVM_CITY_EYES_CHANNEL_NO_SIZE, (U8_YS *)&usChannelNO);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDtviaTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadDtviaTag(void)
{
	U8_YS	ucTag;
	DRV_NVMRead(NVM_DTVIA_TAG_ADDR,NVM_DTVIA_TAG_SIZE,&ucTag);
	return ucTag;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDtviaTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDtviaTag(U8_YS ucTag)
{
	DRV_NVMWrite(NVM_DTVIA_TAG_ADDR,NVM_DTVIA_TAG_SIZE, &ucTag);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadApplicationType
  Description     : 读取机顶盒软件类型
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadApplicationType(void)
{
	U8_YS	ucTag;

	DRV_NVMRead(NVM_APPLICATION_TYPE_ADDR,NVM_APPLICATION_TYPE_SIZE,&ucTag);
	return ucTag;
}
/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteApplicationType
  Description     : 写入机顶盒软件类型
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteApplicationType(U8_YS ucTag)
{
	DRV_NVMWrite(NVM_APPLICATION_TYPE_ADDR,NVM_APPLICATION_TYPE_SIZE, &ucTag);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadSYSSETPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
S32_YS DRV_DTVReadSYSSETPassword(void)
{
	S32_YS uiPassword;

	DRV_NVMRead(NVM_SYSSET_PASSWORD_ADDR,NVM_SYSSET_PASSWORD_SIZE, (U8_YS *)&uiPassword);
	return uiPassword;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVReadSYSSETPasswordModifiedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
S8_YS DRV_DTVReadSYSSETPasswordModifiedTag(void)
{
	S8_YS scTag;

	DRV_NVMRead(NVM_SYSSET_PASSWORD_MODIFIED_TAG_ADDR,NVM_SYSSET_PASSWORD_MODIFIED_TAG_SIZE, (U8_YS *)&scTag);
	return scTag;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteSYSSETPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
void DRV_DTVWriteSYSSETPassword(S32_YS uiPassword)
{
	DRV_NVMWrite(NVM_SYSSET_PASSWORD_ADDR,NVM_SYSSET_PASSWORD_SIZE,(U8_YS *)&uiPassword);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_DTVWriteSYSSETPasswordModifiedTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
void DRV_DTVWriteSYSSETPasswordModifiedTag(S8_YS  scTag)
{
	DRV_NVMWrite(NVM_SYSSET_PASSWORD_MODIFIED_TAG_ADDR,NVM_SYSSET_PASSWORD_MODIFIED_TAG_SIZE,(U8_YS *)&scTag);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteSYSSETShowTime
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
void DRV_WriteSYSSETShowTime(S8_YS ShowTime)
{
	DRV_NVMWrite(NVM_SYSSET_MENU_SHOW_TIME_TAG_ADDR,NVM_SYSSET_MENU_SHOW_TIME_TAG_SIZE,(U8_YS *)&ShowTime);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadSYSSETShowTime
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :未使用
*****************************************************************************/ 
S8_YS DRV_ReadSYSSETShowTime(void)
{
	S8_YS scTag;

	DRV_NVMRead(NVM_SYSSET_MENU_SHOW_TIME_TAG_ADDR,NVM_SYSSET_MENU_SHOW_TIME_TAG_SIZE, (U8_YS *)&scTag);
	return scTag;
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNVODServiceNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNVODServiceNO()
{
	U16_YS	usChannelNO;

	DRV_NVMRead( NVM_NVOD_SERVICE_NO_ADDR, NVM_NVOD_SERVICE_NO_SIZE, (U8_YS *)&usChannelNO);
	return usChannelNO;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNVODServiceNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNVODServiceNO(U8_YS usChannelNO)
{
	DRV_NVMWrite(NVM_NVOD_SERVICE_NO_ADDR,NVM_NVOD_SERVICE_NO_SIZE, (U8_YS *)&usChannelNO);
}


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadNVODEventID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadNVODEventID()
{
	U16_YS	usChannelNO;

	DRV_NVMRead( NVM_NVOD_EVENT_ID_ADDR, NVM_NVOD_EVENT_ID_SIZE, (U8_YS *)&usChannelNO);
	return usChannelNO;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteNVODEventID
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteNVODEventID(U8_YS usChannelNO)
{
	DRV_NVMWrite(NVM_NVOD_EVENT_ID_ADDR,NVM_NVOD_EVENT_ID_SIZE, (U8_YS *)&usChannelNO);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZone
  Description     : 读取时区
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadTimeZone(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_TIME_ZONE_ADDR, NVM_TIME_ZONE_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZone
  Description     : 写入时区
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteTimeZone(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_TIME_ZONE_ADDR,NVM_TIME_ZONE_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadTimeZoneTag
  Description     : 读取时区修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadTimeZoneTag(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_TIME_ZONE_TAG_ADDR, NVM_TIME_ZONE_TAG_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteTimeZoneTag
  Description     : 写入时区修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteTimeZoneTag(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_TIME_ZONE_TAG_ADDR,NVM_TIME_ZONE_TAG_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTime
  Description     : 读取显示时间
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadShowTime(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_SHOW_TIME_ADDR, NVM_SHOW_TIME_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteShowTime
  Description     : 写入显示时间
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteShowTime(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_SHOW_TIME_ADDR,NVM_SHOW_TIME_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTimeTag
  Description     : 读取显示时间修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadShowTimeTag(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_SHOW_TIME_TAG_ADDR, NVM_SHOW_TIME_TAG_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadShowTimeTag
  Description     : 写入显示时间修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteShowTimeTag(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_SHOW_TIME_TAG_ADDR,NVM_SHOW_TIME_TAG_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVideoMode
  Description     : 读取视频输出制式(N/P)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVideoMode(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_VIDEO_MODE_ADDR, NVM_VIDEO_MODE_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVideoMode
  Description     : 写入视频输出制式(N/P)
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVideoMode(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_VIDEO_MODE_ADDR,NVM_VIDEO_MODE_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVideoModeTag
  Description     : 读取视频输出制式(N/P)修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVideoModeTag(void)
{
	U8_YS	ucTimeZone;

	DRV_NVMRead( NVM_VIDEO_MODE_TAG_ADDR, NVM_VIDEO_MODE_TAG_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVideoModeTag
  Description     : 写入视频输出制式(N/P)修改标识
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVideoModeTag(U8_YS ucTimeZone)
{
	DRV_NVMWrite(NVM_VIDEO_MODE_TAG_ADDR,NVM_VIDEO_MODE_TAG_SIZE, (U8_YS *)&ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUseCAPINTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadUseCAPINTag (void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_USE_CA_PIN_TAG_ADDR,NVM_USE_CA_PIN_TAG_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUseCAPINTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteUseCAPINTag (U8_YS vol)
{
	DRV_NVMWrite(NVM_USE_CA_PIN_TAG_ADDR,NVM_USE_CA_PIN_TAG_SIZE, &vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadUsePasswordTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadUsePasswordTag(void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_USE_PASSWORD_TAG_ADDR,NVM_USE_PASSWORD_TAG_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteUsepasswordTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteUsepasswordTag(U8_YS vol)
{
	DRV_NVMWrite(NVM_USE_PASSWORD_TAG_ADDR,NVM_USE_PASSWORD_TAG_SIZE, &vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStartFre
  Description     : 获取分频段搜索的开始搜索频点
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadStartFre(void)
{
	U16_YS	vol;

	DRV_NVMRead(NVM_START_FRE_ADDR,NVM_START_FRE_SIZE,(U8_YS *)&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadStartFre
  Description     : 写入分频段搜索的开始搜索频点
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteStartFre(U16_YS vol)
{
	DRV_NVMWrite(NVM_START_FRE_ADDR,NVM_START_FRE_SIZE, (U8_YS *)&vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadEndFre
  Description     : 获取分频段搜索的结束搜索频点
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16_YS DRV_ReadEndFre(void)
{
	U16_YS	vol;

	DRV_NVMRead(NVM_END_FRE_ADDR,NVM_END_FRE_SIZE,(U8_YS *)&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteEndFre
  Description     : 写入分频段搜索的结束搜索频点
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteEndFre(U16_YS vol)
{
	DRV_NVMWrite(NVM_END_FRE_ADDR,NVM_END_FRE_SIZE, (U8_YS *)&vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadCAPINPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
S32_YS DRV_ReadCAPINPassword(void)
{
	S32_YS	vol;

	DRV_NVMRead(NVM_CA_PIN_PASSWORD_ADDR,NVM_CA_PIN_PASSWORD_SIZE,(U8_YS *)&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteCAPINPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteCAPINPassword(S32_YS vol)
{
	DRV_NVMWrite(NVM_CA_PIN_PASSWORD_ADDR,NVM_CA_PIN_PASSWORD_SIZE, (U8_YS *)&vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadInversionTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadInversionTag(void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_INVERSION_TAG_ADDR,NVM_INVERSION_TAG_SIZE,(U8_YS *)&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteInversionTag
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteInversionTag(U8_YS vol)
{
	DRV_NVMWrite(NVM_INVERSION_TAG_ADDR,NVM_INVERSION_TAG_SIZE, (U8_YS *)&vol);
}

#ifndef NVM_USE_FIXED_SYSTEM_OFFSET
/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDtviaDataVersion
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32 DRV_ReadDtviaDataVersion(U8_YS ucFiletype )
{
	U32 ucVersion;
	DRV_NVMRead( NVM_DTVIA_DATA_VERSION_ADDR+(ucFiletype-1)*4, 4, (U8_YS *)&ucVersion);
	return ucVersion;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDtviaDataVersion
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDtviaDataVersion(U8_YS ucFiletype, U32 ucTimeZone)
{
	DRV_NVMWrite(NVM_DTVIA_DATA_VERSION_ADDR+(ucFiletype-1)*4,4, (U8_YS *)&ucTimeZone);
}
#endif


/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadBootPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U16 DRV_ReadBootPassword(void)
{
	U16	ucTimeZone;

	DRV_NVMRead( NVM_PASSWORD_1_ADDR, NVM_PASSWORD_1_SIZE, (U8_YS *)&ucTimeZone);
	return ucTimeZone;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteBootPassword
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteBootPassword(U16 ucTimeZone)
{
	DRV_NVMWrite(NVM_PASSWORD_1_ADDR,NVM_PASSWORD_1_SIZE, (U8_YS *)&ucTimeZone);
}

#ifndef NVM_USE_FIXED_SYSTEM_OFFSET
/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadChnlChngTimingSet
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadChnlChngTimingSet(U8_YS *ucTimeZone)
{
	DRV_NVMRead( NVM_CC_TIMING_ADDR, NVM_CC_TIMING_SIZE, (U8_YS *)ucTimeZone);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteChnlChngTimingSet
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteChnlChngTimingSet(U8_YS *ucTimeZone)
{
	DRV_NVMWrite(NVM_CC_TIMING_ADDR,NVM_CC_TIMING_SIZE, (U8_YS *)ucTimeZone);
}
#endif

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadRadioPDTVersionNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadRadioPDTVersionNO(void)
{
	U8_YS	vol;

	DRV_NVMRead(NVM_RADIO_BACKGROUND_VERSION_NO_ADDR,NVM_RADIO_BACKGROUND_VERSION_NO_SIZE,&vol);
	return vol;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteRadioPDTVersionNO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteRadioPDTVersionNO(U8_YS vol)
{
	DRV_NVMWrite(NVM_RADIO_BACKGROUND_VERSION_NO_ADDR,NVM_RADIO_BACKGROUND_VERSION_NO_SIZE, &vol);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPAddr(U32_YS ip)
{
	DRV_NVMWrite(NVM_LWIP_IPADDR_ADDR,NVM_LWIP_IPADDR_SIZE, (U8_YS *)&ip);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPMask
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPMask(U32_YS mask)
{
	DRV_NVMWrite(NVM_LWIP_MASK_ADDR,NVM_LWIP_MASK_SIZE, (U8_YS *)&mask);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPGW
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPGW(U32_YS gw)
{
	DRV_NVMWrite(NVM_LWIP_GW_ADDR,NVM_LWIP_GW_SIZE, (U8_YS *)&gw);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPDNS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPDNS(U32_YS dns)
{
	DRV_NVMWrite(NVM_LWIP_DNS_ADDR,NVM_LWIP_DNS_SIZE, (U8_YS *)&dns);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPAddr(void)
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_LWIP_IPADDR_ADDR,NVM_LWIP_IPADDR_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPMask
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPMask()
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_LWIP_MASK_ADDR,NVM_LWIP_MASK_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPGW
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPGW()
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_LWIP_GW_ADDR,NVM_LWIP_GW_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPDNS
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPDNS()
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_LWIP_DNS_ADDR,NVM_LWIP_DNS_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteIPAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteIPAUTO(U8_YS ipauto)
{
	DRV_NVMWrite(NVM_LWIP_IP_AUTO_ADDR,NVM_LWIP_IP_AUTO_SIZE, &ipauto);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadIPAUTO()
{
	U8_YS ipauto;

	DRV_NVMRead(NVM_LWIP_IP_AUTO_ADDR,NVM_LWIP_IP_AUTO_SIZE, &ipauto);
	return ipauto;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteDNSAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteDNSAUTO(U8_YS dnsauto)
{
	DRV_NVMWrite(NVM_LWIP_DNS_AUTO_ADDR,NVM_LWIP_DNS_AUTO_SIZE, &dnsauto);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadDNSAUTO
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadDNSAUTO()
{
	U8_YS dnsauto;

	DRV_NVMRead(NVM_LWIP_DNS_AUTO_ADDR,NVM_LWIP_DNS_AUTO_SIZE, &dnsauto);
	return dnsauto;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVODServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVODServerAddr(U32_YS ip)
{
	DRV_NVMWrite(NVM_VOD_SERVER_ADDR,NVM_VOD_SERVER_SIZE, (U8_YS *)&ip);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVODServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadVODServerAddr()
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_VOD_SERVER_ADDR,NVM_VOD_SERVER_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteHTTPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteHTTPServerAddr(U32_YS ip)
{
	DRV_NVMWrite(NVM_HTTP_SERVER_ADDR,NVM_HTTP_SERVER_SIZE, (U8_YS *)&ip);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadHTTPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadHTTPServerAddr()
{
	U32_YS	IPAddr;

	DRV_NVMRead(NVM_HTTP_SERVER_ADDR,NVM_HTTP_SERVER_SIZE,(U8_YS *)&IPAddr);
	return IPAddr;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteMACAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteMACAddr(U8_YS * mac_addr)
{
	DRV_NVMWrite(NVM_MAC_ADDR,NVM_MAC_SIZE, mac_addr);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadMACAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadMACAddr(U8_YS * mac_addr)
{
	DRV_NVMRead(NVM_MAC_ADDR,NVM_MAC_SIZE,mac_addr);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadIPServerAddr
  Description     : 
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32_YS DRV_ReadIPServerAddr(void)
{
	 U32_YS ipUpServerIp;
	 
	 DRV_NVMRead(NVM_IPUPDATE_SERVER_ADDR,NVM_IPUPDATE_SERVER_SIZE,(U8_YS *)&ipUpServerIp);
	 return ipUpServerIp;
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_WriteVoutMode
  Description     : 写入视频输出接口
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteVoutMode(U8_YS vout_mode)
{
	DRV_NVMWrite(NVM_VOUT_MODE,VOUT_MODE_SIZE, &vout_mode);
}

/*****************************************************************************
  Date&Time    :    2004-03-16
  Author          :    yzb
  Function Name   : DRV_ReadVoutMode
  Description     : 获取视频输出接口
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U8_YS DRV_ReadVoutMode(void)
{
	U8_YS VoutMode;
	DRV_NVMRead(NVM_VOUT_MODE,VOUT_MODE_SIZE, &VoutMode);
	return VoutMode;
}

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_ReadSTBParamsMark
  Description     : 读取进后门的标记
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
U32 DRV_ReadSTBParamsMark(void)
{
    U16 STBParamsMark = 0;

    DRV_NVMRead(NVM_BACKDOOR_TAG_ADDR,NVM_BACKDOOR_TAG_SIZE, (U8 *)&STBParamsMark);
    return STBParamsMark;
}

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_WriteSTBParamsMark
  Description     : 写进后门的标记位，0xABCD是进入的标记，需要与boot一致
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteSTBParamsMark(HI_U16 STBParamsMark)
{
    DRV_NVMWrite(NVM_BACKDOOR_TAG_ADDR,NVM_BACKDOOR_TAG_SIZE, (HI_U8 *)&STBParamsMark);
}

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_WriteJzPrivateData
  Description     : 写入后门程序的私有数据，
  					一般为地方特有的设置或参数，由应用自己定义并解析使用
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_WriteJzPrivateData(unsigned char *pData, unsigned short usLen)
{
	if(NVM_JZ_PRIVATE_DATA_SIZE >= (usLen+2))
	{
		DRV_NVMWrite(NVM_JZ_PRIVATE_DATA_ADDR, 2, (U8_YS *)&usLen);
		DRV_NVMWrite(NVM_JZ_PRIVATE_DATA_ADDR+2, usLen, (U8_YS *)pData);
	}
}

/*****************************************************************************
  Date&Time       : 2013-07-23
  Author          :    cwde
  Function Name   : DRV_ReadJzPrivateData
  Description     : 读取后门程序的私有数据，
  					一般为地方特有的设置或参数，由应用自己定义并解析使用
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          :
*****************************************************************************/ 
void DRV_ReadJzPrivateData(unsigned char *pData, unsigned short *usLen)
{
	unsigned short usLen1 = 0, usLen2 = 0;
	DRV_NVMRead(NVM_JZ_PRIVATE_DATA_ADDR, 2, (U8_YS *)usLen);
	usLen1 = *usLen;
	usLen2 = NVM_JZ_PRIVATE_DATA_SIZE-2;
	if(usLen2 >= usLen1)
	{
		DRV_NVMRead(NVM_JZ_PRIVATE_DATA_ADDR+2, usLen1, (U8_YS *)pData);
	}
}

/********************地市编码**********************/
void DRV_WriteWasuCityCode(HI_U8* pCityCode)
{
    DRV_NVMWrite(NVM_WASU_CITYCODE_ADDR, NVM_WASU_CITYCODE_SIZE, pCityCode);
}
void DRV_ReadWasuCityCode(HI_U8* pCityCode)
{
    DRV_NVMRead(NVM_WASU_CITYCODE_ADDR, NVM_WASU_CITYCODE_SIZE, pCityCode);
}

/********************机顶盒号**********************/
void DRV_WriteWasuSTBID(HI_U8* pStbId)
{
    DRV_NVMWrite(NVM_WASU_STBID_ADDR, NVM_WASU_STBID_SIZE, pStbId);
}
void DRV_ReadWasuSTBID(HI_U8* pStbId)
{
    DRV_NVMRead(NVM_WASU_STBID_ADDR, NVM_WASU_STBID_SIZE, pStbId);
}


