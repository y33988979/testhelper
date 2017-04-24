/**
 \file
 \brief udiskҵ��ģ������ͷ�ļ�
 \copyright Hisilicon Tech. Co., Ltd.
 \date 2009-2050
 \version main\2
 \author r00112701/z42136
 \date 2009-11-9
 */


#ifndef __HI_UDISK_H__
#define __HI_UDISK_H__

#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */


        /*--------------------------------------------------------------
        Error Code
        ----------------------------------------------------------------*/
        enum
        {
                HI_UDISK_ERR_NULL           = HI_FAILURE,
                HI_UDISK_ERR_INVALIDPARA    = -2,
                HI_UDISK_ERR_NOMEM          = -3,
                HI_UDISK_ERR_EXIST          = -4,
                HI_UDISK_ERR_FULL           = -5,
                HI_UDISK_ERR_UNKNOWN_TYPE   = -6,
                HI_UDISK_ERR_NOTEXIST       = -7,
                HI_UDISK_ERR_BUTT
        };


        /*--------------------------------------------------------------
        ���ݽṹ����
        ----------------------------------------------------------------*/

#define MAX_UDISK_NAME_LEN  256

        /** ������Ϣ */
        typedef struct
        {
                //HI_U32  u32Port;        /**< �������ڵ�USB�˿�, Ŀǰû��ʵ�� */
                HI_U64  u64TotalSize;        /**< ��������*/
                HI_U64  u64AvailSize;    /* ���̿������� */
                HI_CHAR szVendor[MAX_UDISK_NAME_LEN];     /**< ���̣�ֻ�� */
                HI_CHAR szModel[MAX_UDISK_NAME_LEN];      /**< �ͺţ�ֻ�� */

                HI_CHAR szDevPath[MAX_UDISK_NAME_LEN];     /**< �����豸�ڵ�����Ŀ¼��ֻ�� */
                HI_CHAR szMntPath[MAX_UDISK_NAME_LEN];     /**< ����ӳ����Ŀ¼�����еķ������ڸ�Ŀ¼�£���ͬ����Ŀ¼���ܲ�һ�£�ֻ�� */
        } HI_UDISK_DISKINFO_S;

        /** ���̷�����ʽ */
        typedef enum
        {
                HI_UDISK_FSTYPE_FAT = 0x0,
                HI_UDISK_FSTYPE_FAT32,
                HI_UDISK_FSTYPE_NTFS,
                HI_UDISK_FSTYPE_EXT2,
                HI_UDISK_FSTYPE_EXT3,

                HI_UDISK_FSTYPE_BUT,
        } HI_UDISK_FSTYPE_E;


        /** ���̷�����Ϣ */
        typedef struct
        {
                HI_BOOL bMounted;           /**< �Ƿ���ȷ���ء����û�й��أ���������Զ���Ч */
                HI_UDISK_FSTYPE_E enFSType; /**< �����ļ���ʽ */
                HI_U64  u64TotalSize;       /**< �������� */
                HI_U64  u64AvailSize;       /**< ������������ */

                HI_CHAR szDevPath[MAX_UDISK_NAME_LEN];  /**< �����豸�ڵ�Ŀ¼��ֻ�� */
                HI_CHAR szMntPath[MAX_UDISK_NAME_LEN];  /**< ��������Ŀ¼��ֻ�� */
        } HI_UDISK_PARTITIONINFO_S;


        /** �ļ��б� */
        typedef struct
        {
                HI_U32  u32Index;        /**< �ļ����ļ��б��е������ţ���0��ʼ��� */
                HI_U32  u32Type;         /**< 0:DIR, 1:FILE */
                HI_U64  u32Size;         /**< �ļ���С, Byte */
                HI_U32  u32CTime;        /**< �ļ�����ʱ�� */
                HI_CHAR* pszShortName;   /**< �ļ���������·����ֻ�� */
                HI_CHAR* pszFullName;    /**< ��·�����ļ���, ֻ�� */
        } HI_UDISK_FILEINFO_S;

        /** Ŀ¼�������ʽ */
        typedef enum
        {
                HI_UDISK_SORT_DEFAULT       = 0x00000011, /**< Ĭ������ */

                HI_UDISK_SORT_ASC           = 0x00000001, /**< ����������δָ��ASC/DESC����£�Ĭ��ΪASC��ASC/DESC����NAME/TIME/SIZE/TYPE������� */
                HI_UDISK_SORT_DESC          = 0x00000002, /**< ���������� */

                HI_UDISK_SORT_NAME          = 0x00000010,    /**< ����������δָ��NAME/TIME/SIZE/TYPE������£�Ĭ��ΪNAME */
                HI_UDISK_SORT_TIME          = 0x00000020,    /**< ��ʱ������ */
                HI_UDISK_SORT_SIZE          = 0x00000040,    /**< ����С���� */
                HI_UDISK_SORT_EXT           = 0x00000080,    /**< ���������򣬼����ļ���չ������ */

                HI_UDISK_SORT_BUTT
        } HI_UDISK_SORT_MODE_E;

        /** Ŀ¼����������� */
        typedef enum
        {
                HI_UDISK_FLT_DEFAULT    = 0x00000005,    /**< Ĭ�Ϸ�ʽ: UP+SUB, ��Сд������ */
                HI_UDISK_FLT_NODIR      = 0x00000000,    /**< �������κ�Ŀ¼ */

                HI_UDISK_FLT_UPDIR      = 0x00000001,    /**< ������������ϼ�Ŀ¼����".." */
                HI_UDISK_FLT_CURDIR     = 0x00000002,    /**< �������������ǰĿ¼����"." */
                HI_UDISK_FLT_SUBDIR     = 0x00000004,    /**< �����������Ŀ¼ */

                HI_UDISK_FLT_CASECARE   = 0x00000008,    /**< ����ʱ���Ĵ�Сд�����ļ���Ŀ¼����Ч */

                HI_UDISK_FLT_BUTT
        } HI_UDISK_FILTER_E;

        /** Ŀ¼���� */
        typedef struct
        {
                HI_BOOL bRecursive;                 /**< �Ƿ�ݹ�������Ŀ¼�е��ļ�/Ŀ¼ */
                HI_UDISK_SORT_MODE_E enSortMode;    /**< Ŀ¼�������ʽ */
                HI_UDISK_FILTER_E enFilterType;     /**< Ŀ¼����������� */
                HI_CHAR * pFileFilter;              /**< �÷�: ".*":(�����ļ�)��"": �������κ��ļ���".mp3": ֻȡmp3�ļ���".jpg,.png": ȡjpg��png�ļ� */
        } HI_UDISK_SCAN_S;

        typedef struct
        {
                HI_UDISK_FILEINFO_S ** ppFileList;
                HI_U32 u32FileNum;
        } HI_UDISK_FILELIST_S;

        /** �¼����� */
        typedef enum
        {
                HI_UDISK_EVENT_PLUGIN = 0x0, /**< ���̲��� */
                HI_UDISK_EVENT_PLUGOUT,      /**< ���̰γ� */
                HI_UDISK_EVENT_MOUNTED,      /**< �������� */
                HI_UDISK_EVENT_UMOUNTED,    /**< ����ж�� */
                HI_UDISK_EVENT_FULL,         /**< ������ */
                HI_UDISK_EVENT_ALLPARTMOUNTED, /* �ô��������з����Ѽ���*/
                HI_UDISK_EVENT_BUTT
        } HI_UDISK_EVENT_E;

        /**
        \brief  �¼��ص�����
        \attention \n
        ��
        \param[in] HI_UDISK_EVENT_E : �¼�����
        \param[in] pParam : �¼���������ͬ���¼����ͣ���Ӧ��ͬ�Ĳ����ṹ���μ�HI_UDISK_EVENT_PARAM_xxxx_S

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_EVENT_PARAM_PLUG_S \n
        ::HI_UDISK_EVENT_PARAM_MOUNT_S \n
        ::HI_UDISK_EVENT_PARAM_FULL_S
        */
        typedef void (*HI_UDISK_EVENT_CALLBACK)(HI_UDISK_EVENT_E enEvent, HI_VOID * pParam, HI_VOID * pPrivate);

        /** �¼�����: for PLUGIN/PLUGOUT */
        typedef struct
        {
                HI_U32 u32DiskIndex;
        } HI_UDISK_EVENT_PARAM_PLUG_S;

        /** �¼�����: for MOUNTED/UNMOUNTED */
        typedef struct
        {
                HI_U32 u32DiskIndex;
                HI_U32 u32PartitionIndex;
        } HI_UDISK_EVENT_PARAM_MOUNT_S;

        /** �¼�����: for FULL */
        typedef struct
        {
                HI_U32 u32DiskIndex;
                HI_U32 u32PartitionIndex;
        } HI_UDISK_EVENT_PARAM_FULL_S;


        /******************************* API declaration *****************************/
        /** @defgroup  UDISK�������ӿ�ģ��
         *  @ingroup H1
         *  @brief ��ϸ����UDISK�������ӿڵ�API����
         *  @{  */


        /**
        \brief  UDISK��ʼ��
        \attention \n
        ��
        \param[in] pUdiskRoot : UDISK����Ŀ¼�����뱣֤�ǿ�Ŀ¼����"/tmp"
        \param[in] u32MaxDisk : ϵͳ֧�ֵ������̸�����Ϊ0��ʹ��ϵͳĬ��ֵ(32��)
        \param[in] u32MaxPartition : ϵͳ֧�ֵ�ÿ��������������������Ϊ0��ʹ��ϵͳĬ��ֵ(32��)

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_Deinit
        */
        HI_S32 HI_UDISK_Init(const HI_CHAR * pUdiskRoot);

        /**
        \brief  UDISKȥ��ʼ��
        \attention \n
        ��
        \param[in] ��

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_Init
        */
        HI_S32 HI_UDISK_Deinit(HI_VOID);

        /**
        \brief  ��ȡUdisk����Ŀ¼
        \attention \n
        ��
        \param[in] ��

        \retval  ::Udisk����Ŀ¼�����ΪHI_NULL����˵��UDISKδ��ʼ��
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_Init
        */
        HI_CHAR * HI_UDISK_GetRoot(HI_VOID);


        /**
        \brief  ��ȡ���̸���
        \attention \n
        ��
        \param[out] pu32DiskNum  : ���̸���

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetDiskInfo
        */
        HI_S32 HI_UDISK_GetDiskNum(HI_U32 * pu32DiskNum);

        /**
        \brief  ��ȡ������Ϣ
        \attention \n
        ��
        \param[in] u32DiskIndex : ����Index�����ڼ������̣�ֵΪ0��[DiskNum-1]
        \param[out] pDiskInfo   : ������Ϣ

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetDiskNum
        */
        HI_S32 HI_UDISK_GetDiskInfo(HI_U32 u32DiskIndex, HI_UDISK_DISKINFO_S * pDiskInfo);

        /**
        \brief  ��ȡָ�������µķ�������
        \attention \n
        ��
        \param[in] u32DiskIndex  : ����Index
        \param[out] pu32PartitionNum  : �����·�������

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetPartitionInfo
        */
        HI_S32 HI_UDISK_GetPartitionNum(HI_U32 u32DiskIndex, HI_U32 * pu32PartitionNum);

        /**
        \brief  ��ȡָ�������µ�ָ����������Ϣ
        \attention \n
        ��
        \param[in] u32DiskIndex  : ����Index
        \param[in] u32PartitionIndex  : ����Index
        \param[out] pPartitionInfo  : ������Ϣ

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetPartitionNum
        */
        HI_S32 HI_UDISK_GetPartitionInfo(HI_U32 u32DiskIndex, HI_U32 u32PartitionIndex, HI_UDISK_PARTITIONINFO_S * pPartitionInfo);


        /**
        \brief  ������ʽ��
        \attention \n
        ��
        \param[in] u32DiskIndex  : ����Index
        \param[in] u32PartitionIndex  : ����Index
        \param[in] enFSType  : ��������ʽ�������ָ�ʽ

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetDiskNum \n
        ::HI_UDISK_GetPartitionNum
        */
        HI_S32 HI_UDISK_FormatPartition(HI_U32 u32DiskIndex, HI_U32 u32PartitionIndex, HI_UDISK_FSTYPE_E enFSType);

        /**
        \brief  ��ȡ�ļ��б�
        \attention \n
        ��
        \param[in] pszDir  : �ļ�������Ŀ¼
        \param[in] pScanOpt  : ��ȡ��ʽ
        \param[out] pFileList  : �ļ�/Ŀ¼�б��ڴ���UDISK������ʹ���������HI_UDISK_FreeFileList�ͷŸ��ڴ�
        \param[out] pFileNum  : �ļ�/Ŀ¼����

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_FreeFileList
        */
        HI_S32 HI_UDISK_GetFileList(
                HI_CHAR *pszDir,
                HI_UDISK_SCAN_S * pScanOpt,
                HI_UDISK_FILELIST_S * pFileList);

        /**
        \brief  �ͷ��ļ��б���ռ�ڴ�ռ�
        \attention \n
        ��
        \param[in] pFileList  : �ļ��б�ָ��

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_GetFileList
        */
        HI_S32 HI_UDISK_FreeFileList(HI_UDISK_FILELIST_S * pFileList);


        /**
        \brief  ע���¼��ص�
        \attention \n
        ��
        \param[in] enEvent  : �¼�����
        \param[in] pCallbackFunc  : �¼����Ͷ�Ӧ�Ļص�����
        \param[in] pPrivate  : �û�˽�����ݣ��¼�����ʱ����ͨ���ص�����͸�����û����ڴ����û�����/�ͷţ����û���֤�ڴ����Ч��

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_UnRegisterEventCallback
        */
        HI_S32 HI_UDISK_RegisterEventCallback(
                HI_UDISK_EVENT_E enEvent,
                HI_UDISK_EVENT_CALLBACK pCallbackFunc,
                HI_VOID * pPrivate);


        /**
        \brief  ע���¼��ص�
        \attention \n
        ��
        \param[in] enEvent  : �¼�����

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ::HI_UDISK_RegisterEventCallback
        */
        HI_S32 HI_UDISK_UnRegisterEventCallback(HI_UDISK_EVENT_E enEvent);

        /**
        \brief  ���÷����Ƿ��ϱ�FULL�¼���Ĭ������£����еķ��������ϱ�
        \attention \n
        ��
        \param[in] u32DiskIndex  : ����Index
        \param[in] u32PartitionIndex  : ����Index
        \param[in] bReport  : �Ƿ��ϱ�
        \param[in] u64WaterLine  : ����ʣ��ռ����ڸ�ˮ�߾��ϱ�FULL�¼�, ��λByte

        \retval  ::HI_SUCCESS
        \retval  ::HI_FAILURE

        \see \n
        ��
        */
        HI_S32 HI_UDISK_SetFullReport(HI_U32 u32DiskIndex, HI_U32 u32PartitionIndex, HI_BOOL bReport, HI_U64 u64WaterLine);

        HI_VOID HI_UDISK_VfatParaSet(HI_BOOL bLinuxVfatType);


        /** @} */  /*! <!-- API declaration end */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UDISK_H__ */


