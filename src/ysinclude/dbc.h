/*******************************************************************************
    Copyright (C), 2004-2007, JIUZHOU. Co., Ltd.

    File name :dbc.h

    Description : ��ģ��ӿڵ��ã������ν�dbc  ��Ӧ�ã�Ҳ��dbc  ��
                       ���Ĺ��ýӿ�
    

    Others:       // �������ݵ�˵��

    History:      // �޸���ʷ��¼�б�ÿ���޸ļ�¼Ӧ�����޸����ڡ��޸�
                  // �߼��޸�����������޸����ݼ����������޸Ļ�Ҫ��ӳ��
                  // �汾��ʷ��¼��  

    ======================================================================
    ʱ    �䣺2008-05-20
    ��    �ߣ�wsyu
    ˵    ����1. ����
    ======================================================================
    ʱ    �䣺2004-03-01
    ��    �ߣ�XXX
    ˵    ����1. �������
              2. �޸����ݼ���
              3. ����

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
    DBC_NO_PRINT = 1,          /*����ӡ*/
    DBC_PRINT_ALL,              /*��ӡ���г��˵�������*/
    DBC_PRINT,                      /*ǿ�ƴ�ӡ*/
    DBC_PRINT_CORE,            /*���������ӡ*/
    DBC_PRINT_APP,              /*��ӡӦ�����*/
    DBC_PRINT_OS,                /*��ӡos  ���*/
    DBC_PRINT_OSD,             /*��ӡosd  ���*/
    DBC_PRINT_AV,                /*��ӡ����Ƶ ���*/
    DBC_PRINT_DEMUX,         /*��ӡ���ݽ���  ���*/
    DBC_PRINT_NVM,             /*��ӡflash  ���*/
    DBC_PRINT_SOCKET,        /*��ӡsocket  ���*/ 
    DBC_PRINT_SYS,               /*��ӡsystem���*/ 
    DBC_PRINT_TUNER,            /*��ӡtuner���*/ 
    DBC_PRINT_CA,                    /*��ӡca���*/ 
    DBC_PRINT_FILE                    /*��ӡfile ���*/ 
} Dbc_Print_Level;

typedef	enum
{
    DBC_DX_BROWSER,       /*���������*/
    DBC_SX_BROWSER,       /*˫�������*/
    DBC_DX_STOCK,            /*�����Ʊ*/
    DBC_SX_STOCK,            /*˫���Ʊ*/
    DBC_DX_VOD,                /*����VOD*/
    DBC_SX_VOD,                /*˫��VOD*/
    DBC_SX_DELAYTV,         /*˫��ʱ��*/
    DBC_DX_SPECIAL,          /*����̶�ҳ��*/
    DBC_SX_CM                    /*cm  ����ҳ��*/
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
* Function:       // ��������
* Description:    // �������ܡ����ܵȵ�����
* Input:          // �������˵��������ÿ����������
                  // �á�ȡֵ˵�����������ϵ��
* Output:         // �����������˵����
* Return:         // ��������ֵ��˵��
* Others:         // ����˵��
*************************************************/
/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  ��ȡ��ǰ������İ汾��(�ַ�����ʽ)
* Input:buff:�汾���ַ�����ַ��Ԥ�����Ȳ�ҪС��20�ֽ�
* Output:   
* Return:len:�ַ�������     
                0 ��ȡʧ�� 
                >0�ɹ� 
* Others:   
*******************************************************************************/
unsigned char DBC_GetBrowserVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  ��ȡ��ǰ������İ汾��(�ַ�����ʽ)
* Input:buff:�汾���ַ�����ַ��Ԥ�����Ȳ�ҪС��20�ֽ�
* Output:   
* Return:len:�ַ�������     
                0 ��ȡʧ�� 
                >0�ɹ� 
* Others:   
*******************************************************************************/
unsigned char DBC_GetStockVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  ��ȡ��ǰ������İ汾��(�ַ�����ʽ)
* Input:buff:�汾���ַ�����ַ��Ԥ�����Ȳ�ҪС��20�ֽ�
* Output:   
* Return:len:�ַ�������     
                0 ��ȡʧ�� 
                >0�ɹ� 
* Others:   
*******************************************************************************/
unsigned char DBC_GetVodVersion(char *buff);

/*******************************************************************************
* Function:      DBC_GetBrowserVersion 
* Description:  ��ȡ��ǰ������İ汾��(�ַ�����ʽ)
* Input:buff:�汾���ַ�����ַ��Ԥ�����Ȳ�ҪС��20�ֽ�
* Output:   
* Return:len:�ַ�������     
                0 ��ȡʧ�� 
                >0�ɹ� 
* Others:   
*******************************************************************************/
unsigned char DBC_GetADVersion(char *buff);

/********************************************************************************
* Function: Dbc_Print
* Description:  dbc ��ӡ����
* Input:
            control:��ӡ����          
* Output:   
* Return:   
* Others:   ����dbc  �ڲ�����
********************************************************************************/
void Dbc_Print(Dbc_Print_Level control,char *Format_p, ...);

/********************************************************************************
* Function: DBC_Print_LevelGet
* Description:  ��ȡ��ӡ�ȼ�
* Input:        
* Output:   
* Return:   
* Others:   ����dbc  �ڲ�����
********************************************************************************/
Dbc_Print_Level DBC_Print_LevelGet(void);

/********************************************************************************
* Function:      DBC_is_mem_valid 
* Description:  dbc �ڴ��Ƿ��Ѿ�����
* Input:
* Output:   
* Return:        TRUE: �ѷ���
                      FALSE: δ����
* Others:   ���ڴ���ָ�ɹ��õ�dbc  ���ڴ�
*******************************************************************************/
BOOL DBC_is_mem_valid(void);

/*******************************************************************************
* Function:      DBC_get_Mem_Size 
* Description:  ��ȡdbc  ������partition  �Ĵ�С
* Input:
* Output:   
* Return:  partition  �Ĵ�С
* Others:   
*******************************************************************************/
unsigned int DBC_get_Mem_Size(void);

/*******************************************************************************
* Function:      DBC_get_TempMem 
* Description:  ��ȡdbc  �ڴ�ĵ�ַ
* Input:
* Output:   
* Return:   dbc  �ڴ�ĵ�ַ
* Others:   ���ڴ���ָ�ɹ��õ�dbc  ���ڴ棬��ʱ�Ѿ��ڸ��ڴ���
                ������dbc  ��Partition
*******************************************************************************/
char *DBC_get_TempMem(void);

/*******************************************************************************
* Function:      DBC_get_Mem_Part 
* Description:  ��ȡdbc  ������partition  
* Input:
* Output:   
* Return:  �� partition  �ĵ�ַ     
* Others:   ��partition��ָ�ɹ��õ�dbc  ��partition
*******************************************************************************/
DRVOS_Partition_t *DBC_get_Mem_Part(void);

/*******************************************************************************
* Function:      DBC_MemInit 
* Description:  dbc �ڴ��ʼ��,��SystemPartition  �����ڴ沢����partition
* Input:
* Output:   
* Return:        TRUE: �ɹ�
                      FALSE: ʧ��
* Others:  
*******************************************************************************/
BOOL DBC_MemInit(void);

/*******************************************************************************
* Function:      DBC_ReInitMemory 
* Description:  dbc �ڴ�reset����ʼ���ڴ棬���½���partition
* Input:
* Output:   
* Return:        
* Others:   
*******************************************************************************/
void DBC_ReInitMemory(void);

/*******************************************************************************
* Function:      DBC_RegionSize_Get 
* Description:  ��ȡdbc  ��Ҫ��region  ��С
* Input:    
* Output:  
                outWide: ��ֻҪregion  �Ŀ���ڵ��ڸ�ֵ����
                outHight: �ߣ�ֻҪregion  �ĸߴ��ڵ��ڸ�ֵ����
* Return:        
* Others:   Ӧ�õ��û�ȡdbc  ��region  ����ȫ������16  λɫ
*******************************************************************************/
void DBC_RegionSize_Get(int *outWide,int *outHight);

/*******************************************************************************
* Function:      DBC_Region_Set 
* Description:  ����dbc ʹ�õ�region
* Input:    inRegine:Ӧ�����õ�dbc  ��Ҫ��region  ��Ŀǰһ��Ϊ16  λɫ
* Output:  
* Return:        
* Others:   
*******************************************************************************/
void DBC_Region_Set(OSD_Region_t *inRegine);

/*******************************************************************************
* Function:      DBC_Region_Get 
* Description:  ��ȡdbc ʹ�õ�region
* Input:   
* Output:  
* Return:        
* Others:   
*******************************************************************************/
OSD_Region_t * DBC_Region_Get(void);

/********************************************************************************
* Function: DBC_Show_info
* Description:  ��ʾ��ʾ��Ϣ
* Input:1       �ַ���
* Output:   
* Return:   
* Others:   
********************************************************************************/
void DBC_Show_info(char *message1, char *message2);

/*******************************************************************************
* Function:      DBC_Delivery_Set 
* Description:  ��ȡ�������������Ʊ����vod  ��Ƶ�����
* Input:          inFre:Ƶ��(eg:614000)
                      inSym:������(eg:6875)
                      inQam:���Ʒ�ʽ(eg:3)
* Output:   
* Return:        
* Others:   ��Ӧ�õ���
*******************************************************************************/
void DBC_Delivery_Set( unsigned int inFre, unsigned short inSym, unsigned char inQam);

/*******************************************************************************
* Function:      DBC_Service_Set 
* Description:  dbc  ҵ��id  ����
* Input:          inServiceid: ҵ��id 
* Output:   
* Return:        
* Others:   ��Ӧ�õ���
*******************************************************************************/
void DBC_Service_Set(unsigned short inServiceid);

/*******************************************************************************
* Function:      DBC_Type_Set 
* Description:  ����dbc  ��ҵ��ģʽ:���������Ʊ����vod  ��
* Input:          inType: ����
* Output:   
* Return:        
* Others:   ��Ӧ�õ���
*******************************************************************************/
void DBC_Type_Set(Dbc_Service_Type inType);

/********************************************************************************
* Function: DBC_StbID_Get
* Description:  dbc ��ȡ���������к�
* Input:
* Output:   
* Return:   stbid�ַ�����ַ
* Others:   
*********************************************************************************/
unsigned char DBC_StbID_Get(char* buf,unsigned char buflen);

/********************************************************************************
* Function: DBC_SCId_Get
* Description:  dbc ��ȡsmart ����
* Input:inlag:1:��ȡ�м�8λ����
                    2:��ȡǰ15Ϊ����,û��У��λ
                    3:��ȡ����16λ����
* Output:   
* Return:   false:ʧ�� true:�ɹ�
* Others:   
*********************************************************************************/
BOOL_YS DBC_SCId_Get(unsigned char inlag, char *outCardId);

/********************************************************************************
* Function: DBC_SCStatus_Get
* Description:  dbc ������ȡsmart card ��״̬
* Input:
* Output:   
* Return:  0:��ȡʧ��
               1:in
               2:out
               3:invalid 
* Others:   
*********************************************************************************/
unsigned char DBC_SCStatus_Get(void );

/********************************************************************************
* Function: DBC_SCStatus_Callback
* Description:  ca����smart card ��״̬
* Input:inStatus:1:in
                        2:out
                        3:invalid
* Output:   
* Return:   
* Others:   ca  ����
*********************************************************************************/
void DBC_SCStatus_Callback(unsigned char inStatus);

/********************************************************************************
* Function: DBC_Stop_CARecv
* Description:  ��dbc ֹͣCA ���ݽ���
* Input:
* Output:   
* Return:   
* Others:   �Ժ��п�����CA ��һͳһ�ӿڹ�DBC ����
*********************************************************************************/
void DBC_Stop_CARecv(void);

/*******************************************************************************
* Function:       DBC_EncryptAV_Play
* Description:   ���ż�������Ƶʱ����
* Input:  
* Output:   
* Return:        
* Others:   Ŀǰ֧��ͬ����������Ѷ
*******************************************************************************/
void DBC_EncryptAV_Play(STBDATAServiceInfo_t *pSTBServiceInfo);

/*******************************************************************************
* Function:       DBC_Transform_AudioType
* Description:   ת��ts������Ƶ����Ϊ�������ʶ�������
* Input:  
            AudioType:ts������Ƶ����
* Output:   
* Return:    
            ���������Ƶ����
* Others: ��ʱ��û��ת��������ȫ����mpeg1����
*******************************************************************************/
DRV_AudioType DBC_Transform_AudioType(unsigned char AudioType);

/*******************************************************************************
* Function:       DBC_Transform_VideoType
* Description:   ת��ts������Ƶ����Ϊ�������ʶ�������
* Input:  
            AudioType:ts������Ƶ����
* Output:   
* Return:    
            ���������Ƶ����
* Others: ��ʱ��û��ת��������ȫ����mpeg2����
*******************************************************************************/
DRV_VideoType DBC_Transform_VideoType(unsigned char VideoType);

/*******************************************************************************
* Function:       DBC_InitCommand
* Description:   dbc ���ڵ��Զ���
* Input:  
* Output:   
* Return:        
* Others:   
*******************************************************************************/
BOOL DBC_InitCommand( void );

/*******************************************************************************
*�Ѵ����ݹ㲥mindwareר�ýӿ�
********************************************************************************/
/*******************************************************************************
* Function:       DBC_AVIT_AddService
* Description:    midware���øú������ڼѴ�service���� 
* Input:  inTsInfo
          inServiceInfo
* Output:   
* Return: 0 :�ɹ� ����:ʧ��       
* Others:   
*******************************************************************************/
int DBC_AVIT_AddService(STBDATATsInfo_t *inTsInfo,DvbService_t *inServiceInfo);

/*******************************************************************************
* Function:       DBC_AVIT_SaveService
* Description:    midware���øú������ڼѴ�����service
* Input:  
* Output:   
* Return:      
* Others:   
*******************************************************************************/
void DBC_AVIT_SaveService(void);

/*******************************************************************************
* Function:       DBC_AVIT_ClearService
* Description:    midware���øú������ڼѴ����service
* Input:  
* Output:   
* Return:      
* Others:   
*******************************************************************************/
void DBC_AVIT_ClearService(void);
#endif //DBC_H__

