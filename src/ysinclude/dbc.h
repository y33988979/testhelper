/*******************************************************************************
    Copyright (C), 2004-2007, JIUZHOU. Co., Ltd.

    File name :dbc.h

    Description : 跨模块接口调用，用于衔接dbc  与应用，也有dbc  内
                       部的公用接口
    

    Others:       // 其它内容的说明

    History:      // 修改历史记录列表，每条修改记录应包括修改日期、修改
                  // 者及修改问题简述和修改内容简述；所有修改还要反映到
                  // 版本历史记录中  

    ======================================================================
    时    间：2008-05-20
    作    者：wsyu
    说    明：1. 创建
    ======================================================================
    时    间：2004-03-01
    作    者：XXX
    说    明：1. 问题简述
              2. 修改内容简述
              3. 其他

*******************************************************************************/

#ifndef DBC_H__
#define DBC_H__
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
#include "midware.h"
#include "digitv.h"
 /*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Enum & Macro 
 *----------------------------------------------------------------------------*/
typedef	enum
{
    DBC_NO_PRINT = 1,          /*不打印*/
    DBC_PRINT_ALL,              /*打印所有除了第三方库*/
    DBC_PRINT,                      /*强制打印*/
    DBC_PRINT_CORE,            /*第三方库打印*/
    DBC_PRINT_APP,              /*打印应用相关*/
    DBC_PRINT_OS,                /*打印os  相关*/
    DBC_PRINT_OSD,             /*打印osd  相关*/
    DBC_PRINT_AV,                /*打印音视频 相关*/
    DBC_PRINT_DEMUX,         /*打印数据接收  相关*/
    DBC_PRINT_NVM,             /*打印flash  相关*/
    DBC_PRINT_SOCKET,        /*打印socket  相关*/ 
    DBC_PRINT_SYS,               /*打印system相关*/ 
    DBC_PRINT_TUNER,            /*打印tuner相关*/ 
    DBC_PRINT_CA,                    /*打印ca相关*/ 
    DBC_PRINT_FILE                    /*打印file 相关*/ 
} Dbc_Print_Level;

typedef	enum
{
    DBC_DX_BROWSER,       /*单向浏览器*/
    DBC_SX_BROWSER,       /*双向浏览器*/
    DBC_DX_STOCK,            /*单向股票*/
    DBC_SX_STOCK,            /*双向股票*/
    DBC_DX_VOD,                /*单向VOD*/
    DBC_SX_VOD,                /*双向VOD*/
    DBC_SX_DELAYTV,         /*双向时移*/
    DBC_DX_SPECIAL,          /*单向固定页面*/
    DBC_SX_CM                    /*cm  设置页面*/
} Dbc_Service_Type;

typedef enum
{
    APP_QUIT_TV=0x08002001,
    APP_QUIT_Radio=0x08002002,
    APP_QUIT_NVOD=0x08002003,
    APP_QUIT_MENU=0x08002004,
    APP_QUIT_EPG = 0x08002005,
    APP_QUIT_DTV = 0x08002006,
    APP_QUIT_DIGIT = 0x08002007,
    DBC_TUNER_SINGLE = 0x08002008,
    DBC_TUNER_NSINGLE = 0x08002009,
    DBC_TASK_DEL = 0x0800200a,
    DBC_IP_LINK = 0x0800200b,
    DBC_IP_ULINK = 0x0800200c
}Ipanel_Exit_Msg_e;

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

/*************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 输入参数说明，包括每个参数的作
                  // 用、取值说明及参数间关系。
* Output:         // 对输出参数的说明。
* Return:         // 函数返回值的说明
* Others:         // 其它说明
*************************************************/
/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  获取当前浏览器的版本号(字符串形式)
* Input:buff:版本号字符串地址，预留长度不要小于20字节
* Output:   
* Return:len:字符串长度     
                0 获取失败 
                >0成功 
* Others:   
*******************************************************************************/
unsigned char DBC_GetBrowserVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  获取当前浏览器的版本号(字符串形式)
* Input:buff:版本号字符串地址，预留长度不要小于20字节
* Output:   
* Return:len:字符串长度     
                0 获取失败 
                >0成功 
* Others:   
*******************************************************************************/
unsigned char DBC_GetStockVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  获取当前浏览器的版本号(字符串形式)
* Input:buff:版本号字符串地址，预留长度不要小于20字节
* Output:   
* Return:len:字符串长度     
                0 获取失败 
                >0成功 
* Others:   
*******************************************************************************/
unsigned char DBC_GetVodVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  获取当前浏览器的版本号(字符串形式)
* Input:buff:版本号字符串地址，预留长度不要小于20字节
* Output:   
* Return:len:字符串长度     
                0 获取失败 
                >0成功 
* Others:   
*******************************************************************************/
unsigned char DBC_GetADVersion(char *buff);

/********************************************************************************
* Function: Dbc_Print
* Description:  dbc 打印函数
* Input:
            control:打印控制          
* Output:   
* Return:   
* Others:   仅供dbc  内部调用
********************************************************************************/
void Dbc_Print(Dbc_Print_Level control,char *Format_p, ...);

/********************************************************************************
* Function: DBC_Print_LevelGet
* Description:  获取打印等级
* Input:        
* Output:   
* Return:   
* Others:   仅供dbc  内部调用
********************************************************************************/
Dbc_Print_Level DBC_Print_LevelGet(void);

/********************************************************************************
* Function:      DBC_is_mem_valid 
* Description:  dbc 内存是否已经分配
* Input:
* Output:   
* Return:        TRUE: 已分配
                      FALSE: 未分配
* Others:   该内存是指可共用的dbc  的内存
*******************************************************************************/
BOOL DBC_is_mem_valid(void);

/*******************************************************************************
* Function:      DBC_get_Mem_Size 
* Description:  获取dbc  创建的partition  的大小
* Input:
* Output:   
* Return:  partition  的大小
* Others:   
*******************************************************************************/
unsigned int DBC_get_Mem_Size(void);

/*******************************************************************************
* Function:      DBC_get_TempMem 
* Description:  获取dbc  内存的地址
* Input:
* Output:   
* Return:   dbc  内存的地址
* Others:   该内存是指可共用的dbc  的内存，这时已经在该内存上
                创建了dbc  的Partition
*******************************************************************************/
char *DBC_get_TempMem(void);

/*******************************************************************************
* Function:      DBC_get_Mem_Part 
* Description:  获取dbc  创建的partition  
* Input:
* Output:   
* Return:  该 partition  的地址     
* Others:   该partition是指可共用的dbc  的partition
*******************************************************************************/
DRVOS_Partition_t *DBC_get_Mem_Part(void);

/*******************************************************************************
* Function:      DBC_MemInit 
* Description:  dbc 内存初始化,从SystemPartition  分配内存并创建partition
* Input:
* Output:   
* Return:        TRUE: 成功
                      FALSE: 失败
* Others:  
*******************************************************************************/
BOOL DBC_MemInit(void);

/*******************************************************************************
* Function:      DBC_ReInitMemory 
* Description:  dbc 内存reset，初始化内存，重新建立partition
* Input:
* Output:   
* Return:        
* Others:   
*******************************************************************************/
void DBC_ReInitMemory(void);

/*******************************************************************************
* Function:      DBC_RegionSize_Get 
* Description:  获取dbc  需要的region  大小
* Input:    
* Output:  
                outWide: 宽，只要region  的宽大于等于该值即可
                outHight: 高，只要region  的高大于等于该值即可
* Return:        
* Others:   应用调用获取dbc  的region  需求，全部都是16  位色
*******************************************************************************/
void DBC_RegionSize_Get(int *outWide,int *outHight);

/*******************************************************************************
* Function:      DBC_Region_Set 
* Description:  设置dbc 使用的region
* Input:    inRegine:应用设置的dbc  需要的region  ，目前一般为16  位色
* Output:  
* Return:        
* Others:   
*******************************************************************************/
void DBC_Region_Set(OSD_Region_t *inRegine);

/*******************************************************************************
* Function:      DBC_Region_Get 
* Description:  获取dbc 使用的region
* Input:   
* Output:  
* Return:        
* Others:   
*******************************************************************************/
OSD_Region_t * DBC_Region_Get(void);

/********************************************************************************
* Function: DBC_Show_info
* Description:  显示提示信息
* Input:1       字符串
* Output:   
* Return:   
* Others:   
********************************************************************************/
void DBC_Show_info(char *message1, char *message2);

/*******************************************************************************
* Function:      DBC_Delivery_Set 
* Description:  获取单向浏览器、股票或者vod  的频点参数
* Input:          inFre:频点(eg:614000)
                      inSym:符号率(eg:6875)
                      inQam:调制方式(eg:3)
* Output:   
* Return:        
* Others:   供应用调用
*******************************************************************************/
void DBC_Delivery_Set( unsigned int inFre, unsigned short inSym, unsigned char inQam);

/*******************************************************************************
* Function:      DBC_Service_Set 
* Description:  dbc  业务id  设置
* Input:          inServiceid: 业务id 
* Output:   
* Return:        
* Others:   供应用调用
*******************************************************************************/
void DBC_Service_Set(unsigned short inServiceid);

/*******************************************************************************
* Function:      DBC_Type_Set 
* Description:  设置dbc  的业务模式:浏览器、股票还是vod  等
* Input:          inType: 类型
* Output:   
* Return:        
* Others:   供应用调用
*******************************************************************************/
void DBC_Type_Set(Dbc_Service_Type inType);

/********************************************************************************
* Function: DBC_StbID_Get
* Description:  dbc 获取机顶盒序列号
* Input:
* Output:   
* Return:   stbid字符串地址
* Others:   
*********************************************************************************/
unsigned char DBC_StbID_Get(char* buf,unsigned char buflen);

/********************************************************************************
* Function: DBC_SCId_Get
* Description:  dbc 获取smart 卡号
* Input:inlag:1:获取中间8位卡号
                    2:获取前15为卡号,没有校验位
                    3:获取整个16位卡号
* Output:   
* Return:   false:失败 true:成功
* Others:   
*********************************************************************************/
BOOL_YS DBC_SCId_Get(unsigned char inlag, char *outCardId);

/********************************************************************************
* Function: DBC_SCStatus_Get
* Description:  dbc 主动获取smart card 的状态
* Input:
* Output:   
* Return:  0:获取失败
               1:in
               2:out
               3:invalid 
* Others:   
*********************************************************************************/
unsigned char DBC_SCStatus_Get(void );

/********************************************************************************
* Function: DBC_SCStatus_Callback
* Description:  ca设置smart card 的状态
* Input:inStatus:1:in
                        2:out
                        3:invalid
* Output:   
* Return:   
* Others:   ca  调用
*********************************************************************************/
void DBC_SCStatus_Callback(unsigned char inStatus);

/********************************************************************************
* Function: DBC_Stop_CARecv
* Description:  进dbc 停止CA 数据接收
* Input:
* Output:   
* Return:   
* Others:   以后有可能由CA 做一统一接口供DBC 调用
*********************************************************************************/
void DBC_Stop_CARecv(void);

/*******************************************************************************
* Function:       DBC_EncryptAV_Play
* Description:   播放加密音视频时调用
* Input:  
* Output:   
* Return:        
* Others:   目前支持同方和数码视讯
*******************************************************************************/
void DBC_EncryptAV_Play(STBDATAServiceInfo_t *pSTBServiceInfo);

/*******************************************************************************
* Function:       DBC_Transform_AudioType
* Description:   转化ts流中音频类型为驱动层可识别的类型
* Input:  
            AudioType:ts流中音频类型
* Output:   
* Return:    
            驱动层的音频类型
* Others: 暂时把没有转化的类型全当做mpeg1处理
*******************************************************************************/
DRV_AudioType DBC_Transform_AudioType(unsigned char AudioType);

/*******************************************************************************
* Function:       DBC_Transform_VideoType
* Description:   转化ts流中视频类型为驱动层可识别的类型
* Input:  
            AudioType:ts流中视频类型
* Output:   
* Return:    
            驱动层的视频类型
* Others: 暂时把没有转化的类型全当做mpeg2处理
*******************************************************************************/
DRV_VideoType DBC_Transform_VideoType(unsigned char VideoType);

/*******************************************************************************
* Function:       DBC_InitCommand
* Description:   dbc 串口调试定义
* Input:  
* Output:   
* Return:        
* Others:   
*******************************************************************************/
BOOL DBC_InitCommand( void );

/*******************************************************************************
*佳创数据广播mindware专用接口
********************************************************************************/
/*******************************************************************************
* Function:       DBC_AVIT_AddService
* Description:    midware调用该函数用于佳创service搜索 
* Input:  inTsInfo
          inServiceInfo
* Output:   
* Return: 0 :成功 其它:失败       
* Others:   
*******************************************************************************/
int DBC_AVIT_AddService(STBDATATsInfo_t *inTsInfo,DvbService_t *inServiceInfo);

/*******************************************************************************
* Function:       DBC_AVIT_SaveService
* Description:    midware调用该函数用于佳创保存service
* Input:  
* Output:   
* Return:      
* Others:   
*******************************************************************************/
void DBC_AVIT_SaveService(void);

/*******************************************************************************
* Function:       DBC_AVIT_ClearService
* Description:    midware调用该函数用于佳创清除service
* Input:  
* Output:   
* Return:      
* Others:   
*******************************************************************************/
void DBC_AVIT_ClearService(void);
#endif //DBC_H__

