/*******************************************************************************
 *             Copyright 2004 - 2050, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: hi_loader_info.h
 * Description:
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main1     2008-11-26   c47559
 ******************************************************************************/
#ifndef __UPGRD_INFO_H__
#define __UPGRD_INFO_H__


#include "hi_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

        /*************************** Structure Definition ****************************/
        /** \addtogroup      H_2_2*/
        /** @{ */  /** <!-- ��loader�������������ݽṹ�� */

        /**LOADER DB��OTA����ħ��,����У�����ݵı�ʶ*/
#define LOADER_MAGIC_NUMBER 0x4C4F4144
        /**�����ļ�������󳤶�*/
#define LOADER_FILE_NAME_LEN       128

        /**�û�������󳤶� */
#define LOADER_USER_NAME_LEN        32

        /**�������󳤶� */
#define LOADER_PASSWORD_LEN         32


        /**�������к��Ƿ����Ŀ���*/
#define LOADER_INNER_STB_SN
        /**IP��ַ����*/
        typedef HI_U32 HI_IPAddr;
        /**IP�˿ںŶ���*/
        typedef HI_U16 HI_IPPort;

#if 0
        typedef enum UPG_UpgradeType
        {
                UPGRADE_TYPE_NONE,
                UPGRADE_TYPE_APP_FORCE,/*0x51DB*/
                UPGRADE_TYPE_IP_FORCE,
                UPGRADE_TYPE_USB,
                UPGRADE_TYPE_SERIAL,
                UPGRADE_TYPE_BIOS_FORCE,
                UPGRADE_TYPE_PIC_FORCE,
                UPGRADE_TYPE_APP_AUTO,/*0xA00A*/
                UPGRADE_TYPE_APP_FORCE2,    /* TEST CODE 0x40CA,�����������ʧ�ܲ����ظ�����*/
                UPGRADE_TYPE_IP_AUTO/*0xa622 or 0xB05A(No App)*/
        } UPG_UpgradeType;
        Ҫ����һһ��Ӧ
#endif
#if 1/* zqyuan jyshan */
        /**�������Ͷ���*/
        typedef enum tagUPDATE_TYPE_E {
                HI_UPDATE_NULL,    /**< ������ ��ӦUPGRADE_TYPE_NONE*/
                HI_UPDATE_OTA     ,/**< CABLE ���� ��ӦUPGRADE_TYPE_APP_FORCE*/
                HI_UPDATE_IP_FORCE,
                HI_UPDATE_USB,
                HI_UPDATE_SERIAL,
                HI_UPDATE_BIOS_FORCE,
                HI_UPDATE_PIC_FORCE,
                HI_UPDATE_APP_AUTO,/*0xA00A*/
                HI_UPDATE_FORCE2,    /* TEST CODE 0x40CA,�����������ʧ�ܲ����ظ�����*/
                HI_UPDATE_IP      /**< IP���� ��ӦUPGRADE_TYPE_IP_AUTO*/
        } HI_UPDATE_TYPE_E;
#else
        /**�������Ͷ���*/
        typedef enum tagUPDATE_TYPE_E {
                HI_UPDATE_OTA  = 0,/**< CABLE ���� */
                HI_UPDATE_IP,      /**< IP���� */
                HI_UPDATE_NULL,    /**< ������ */
        } HI_UPDATE_TYPE_E;
#endif

        /**����ʱʹ������Э�鶨��*/
        typedef enum tagProto_TYPE_E {
                HI_PROTO_TFTP = 0, /**< TFTPЭ������ */
                HI_PROTO_FTP,      /**< FTPЭ������ */
                HI_PROTO_HTTP,     /**< HTTPЭ������*/

                HI_PROTO_BUTT
        } HI_PROTO_TYPE_E;

        /**IP����ʱʹ��IP��ַ���Ͷ���*/
        typedef enum tagIP_TYPE_E {
                HI_IP_STATIC = 0,/**< ʹ�þ�̬IP */
                HI_IP_DHCP,      /**< ʹ�ö�̬IP */
                HI_IP_BUTT,
        } HI_IP_TYPE_E;

        /** ota�������Ͷ���,���ϲ���չʹ��.Ӧ�ð��չ淶д��(����1����ǿ�ơ�2�����ѡ) */
        typedef enum tagOTA_TYPE_E
        {
                HI_OTA_BUTT,
        } HI_OTA_TYPE_E;


        /**���������������ӿڽṹ��*/
        typedef struct tagLoaderOtaInfo
        {
                /* Ӧ�ú�loader �佻�� �ı��� */
                HI_U32      u32MagicNumber;     /**< Loader DB Magic Number ���� */
                HI_UPDATE_TYPE_E eUpdateType;   /**< ����, �������� , 0 :OTA, 1:IP,2:������*/
                HI_PROTO_TYPE_E  eProtoType;    /**< ��������Э�� 0:TFTP, 1:FTP, 2 HTTP >*/
                HI_S8       as8UserName[LOADER_USER_NAME_LEN]; /**<FTP����ʹ�ã��û��� >*/
                HI_S8       as8Password[LOADER_USER_NAME_LEN]; /**<FTP����ʹ�ã����� >*/
                HI_U32      u32NewSoftware;     /**< ������汾��NIT������������Ҫ�����İ汾�� */
                HI_U32      u32CurSoftware;     /**< ��ǰ����汾��Ӧ�ó����Լ���APP�ж����˺꣬����ʱ��Ҫд�뵽DB�� */
                HI_U32      u32Hardware;        /**< ����, Ӳ���汾�� */
                HI_U32      u32Loaderversion;

                HI_OTA_TYPE_E eOTAType;         /**< ������־��Ӧ�ð��չ淶д��(����1����ǿ�ơ�2�����ѡ)*/
                HI_U32      u32OtaPid;          /**< ������������TS PID  ��������ݷǳ���Ҫ,һ��Ҫ��ȷ.*/
                HI_U32      u32OtaFreq;         /**< ������������Ƶ��,��λΪKHz,����403000 */
                HI_U32      u32OtaSymbRate;     /**< �����������ķ�����, ��λΪKbps, ����6875 */
                HI_U32      u32OtaModulation;   /**< ota loaderʹ��, ������������������ 0:16QM 1:32QAM 2:64QAM 3:128QAM 4:256QAM */

                HI_IP_TYPE_E eIpType;           /**< ip  loaderʹ��, ip��ȡ����*/
                HI_U8       u8EthAddr[6];       /**< ip  loaderʹ��, mac��ַ*/
                HI_U8       Reserved1[2];

                /*����ip��ַֻ��ʹ�þ�̬ipʱ��Ҫ����*/
                HI_IPAddr   ipOurIp;            /**< ip  loaderʹ��, ����ip*/
                HI_IPAddr   ipGateway;          /**< ip  loaderʹ��, ����*/
                HI_IPAddr   ipSubnetMask;       /**< ip  loaderʹ��, ��������*/
                HI_IPAddr   ipServer;           /**< ip  loaderʹ��, ������ip*/
                HI_IPPort   ipServerPort;       /**< ip  loaderʹ��, ������port*/
                HI_U8       Reserved2[2];
                HI_S8       as8FileName[LOADER_FILE_NAME_LEN]; /**<IP ip  loaderʹ��, �����ļ���*/
                HI_S32      u32FileSize;                       /**<IP ip  loaderʹ��, �����ļ���С */

                HI_U8       au8AppUse[20];      /**< �ϲ�Ӧ��˽�� */

                HI_UPDATE_TYPE_E eLastUpdType;  /**< ˽�� */
                HI_U32      u32FailedCnt;       /**< ����ʧ�ܴ���,�û��������.*/
                HI_U32      u32DestroyFlag;     /**< �û��������.*/
                HI_U8       au8LoaderReserved[8];  /**<  ˽�� */
                HI_U32      u32Crc;                  /**< crc У��ֵ */
        } HI_LOADER_OTA_INFO_S;

#ifdef HI_SD_LOADER   /** ֻ�б���汾ʱ������HI_LOADER_DB_S,�ڸ���ƽ̨�ϲ�֧��,��صĽӿ����Ƶ�FastPlayģ��.*/

        /**��Ƶ���ÿ�߱ȶ���*/
        typedef enum hiUNF_ASPECT_RATIO_E
        {
                HI_UNF_ASPECT_RATIO_UNKNOWN,
                HI_UNF_ASPECT_RATIO_4TO3,
                HI_UNF_ASPECT_RATIO_16TO9,
                HI_UNF_ASPECT_RATIO_SQUARE,
                HI_UNF_ASPECT_RATIO_14TO9,
                HI_UNF_ASPECT_RATIO_221TO1,

                HI_UNF_ASPECT_RATIO_BUTT
        } HI_UNF_ASPECT_RATIO_E;

        /** Video aspect mode */
        typedef enum hiUNF_ASPECT_CVRS_E
        {
                HI_UNF_ASPECT_CVRS_IGNORE = 0x0,
                HI_UNF_ASPECT_CVRS_LETTERBOX,
                HI_UNF_ASPECT_CVRS_PANANDSCAN,
                HI_UNF_ASPECT_CVRS_COMBINED,
                HI_UNF_ASPECT_CONVERSION_MODE_BUTT
        } HI_UNF_ASPECT_CVRS_E;

        /**��Ƶ��ʽ����*/
        typedef enum hiUNF_ENC_FMT_E
        {
                HI_UNF_ENC_FMT_UNKNOWN = 0,
                HI_UNF_ENC_FMT_PAL,         /**< B D G H I PAL */
                HI_UNF_ENC_FMT_PAL_N,       /**< (N)PAL        */
                HI_UNF_ENC_FMT_PAL_Nc,      /**< (Nc)PAL       */
                HI_UNF_ENC_FMT_SECAM_SIN,
                HI_UNF_ENC_FMT_SECAM_COS,
                HI_UNF_ENC_FMT_NTSC,        /**< (M)NTSC       */
                HI_UNF_ENC_FMT_NTSC_J,      /**< NTSC-J        */
                HI_UNF_ENC_FMT_NTSC_PAL_M,  /**< (M)PAL        */
                HI_UNF_ENC_FMT_AUTO,        /**< ���������������Ӧ�����������Ч */
                HI_UNF_ENC_FMT_BUTT
        } HI_UNF_ENC_FMT_E;


        /********************************DISPLAY STRUCT********************************/
#define MAX_DAC_NUM (4)

        /**DAC�������*/
        typedef enum hiUNF_OUTPUT_DAC_MODE_E
        {
                HI_UNF_DAC_MODE_SILENCE = 0,
                HI_UNF_DAC_MODE_CVBS,
                HI_UNF_DAC_MODE_Y,
                HI_UNF_DAC_MODE_PB,
                HI_UNF_DAC_MODE_PR,
                HI_UNF_DAC_MODE_R,
                HI_UNF_DAC_MODE_G,
                HI_UNF_DAC_MODE_B,
                HI_UNF_DAC_MODE_SVIDEO_Y,
                HI_UNF_DAC_MODE_SVIDEO_C,
                HI_UNF_DAC_MODE_BUTT
        } HI_UNF_OUTPUT_DAC_MODE_E;

        /**�������*/
        typedef struct  hiUNF_OUTPUT_INTERFACE_S
        {
                HI_BOOL                          bScartEnable;
                HI_UNF_OUTPUT_DAC_MODE_E         enDacMode[MAX_DAC_NUM];
        } HI_UNF_OUTPUT_INTERFACE_S ;


        /** ��Ƶ���� */
        typedef struct hiSYSCFG_VIDEO_ATTR_S
        {
                HI_S8                     s8VideoTrans;      /**< ��Ƶ��͸���� */
                HI_UNF_ASPECT_RATIO_E     enAspectRatio ;    /**< ��Ļ��߱� */
                HI_UNF_ASPECT_CVRS_E      enScreenMode ;     /**< ��Ļģʽ */
                HI_UNF_ENC_FMT_E          enTV_Mode ;        /**< ������ʽ */
                HI_UNF_OUTPUT_INTERFACE_S stVO_Mode ;        /**< ��Ƶ�ź������ʽ */
        } HI_SYSCFG_VIDEO_ATTR_S, *HI_SYSCFG_VIDEO_ATTR_S_PTR;

        /** MCE���� */
        typedef struct hiSYSCFG_MCE_S
        {
                HI_U32 u32MceFreq;         /**< MCE��Ŀ����Ƶ�� */
                HI_U32 u32MceSymbRate;   /**< MCE��Ŀ���ķ����� */
                HI_U32 u32MceModulation; /**< MCE��Ŀ���ĵ��Ʋ��� */
                HI_U16 u16MceVideoPid;   /**< MCE��Ŀ������ƵPID */
                HI_U16 u16MceAudioPid;   /**< MCE��Ŀ������ƵPID */
                HI_U32 u32Volume;        /**< ����-MCE��Ŀ������Ƶ */
        } HI_SYSCFG_MCE_INFO_S;

        /** ca���� */
        typedef enum hiSYSCFG_CA_TYPE_E
        {
                HI_CA_TYPE_NONE = 0,
                HI_CA_TYPE_TF,       /**< ͬ�� */
                HI_CA_TYPE_DVT,      /**< ������Ѷ */
                HI_CA_TYPE_IRDETO,  /**< ���ϵ� */
                HI_CA_TYPE_BUTT
        } HI_SYSCFG_CA_TYPE_E;

        /** Loader DB�������ݽṹ */
        typedef struct hiLoader_DB_S
        {
                HI_U32 u32MagicNumber;                      /**< Loader DB����ħ�� */
                HI_SYSCFG_MCE_INFO_S   stSysMceInfo;        /**< MCE��Ŀ�������� */
                HI_SYSCFG_VIDEO_ATTR_S stSysVideoInfo;     /**< ��Ƶ����*/
                HI_SYSCFG_CA_TYPE_E eSysCaType;             /**< ca smart card type */
                HI_U32 u32Crc;                              /**< Loader DB����CRC32ֵ    */
        } HI_LOADER_DB_S;
#else
        typedef struct hiLoader_DB_S
        {
                HI_U32 u32MagicNumber;                      /**< Loader DB����ħ�� */
                HI_U8   aresove[16];
                HI_U32 u32Crc;                              /**< Loader DB����CRC32ֵ    */
        } HI_LOADER_DB_S;
#endif

#define HI_SYSCFG_STBID_LEN 26   /**< �������ⲿ���к� */
#define HI_SYSCFG_STBSN_LEN 15   /**< �������ڲ����к� */

        /** ��������Ϣ */
        typedef struct hiLOADER_STBINFO_S
        {
                HI_U32 u32OUI;       /**< ����id */
                HI_U32 u32AreaCode;  /**< ������ */
                HI_U32 u32HWVersion; /**< Ӳ���汾�� */
                HI_U32 u32HWBatch;   /**< Ӳ������ */
                HI_U8  au8OutSn[HI_SYSCFG_STBID_LEN];   /**< �������ⲿ���к� */
#ifdef LOADER_INNER_STB_SN
                HI_U8  au8Sn[HI_SYSCFG_STBSN_LEN];   /**< �������ڲ����к� */
#endif
        } HI_LOADER_STB_INFO_S;

        /**ȫ��LOADER������������*/
        extern HI_LOADER_OTA_INFO_S g_struLoaderOTAInfo;
        /**ȫ��LOADER DB��������*/
        extern HI_LOADER_DB_S       g_stLoaderDbinfo;
        /**ȫ�����кŲ�������*/
        extern HI_LOADER_STB_INFO_S g_stLoaderStbInfo ;

        /** @} */  /** <!-- ==== Structure Definition end ==== */

        /******************************* API declaration *****************************/
        /** \addtogroup      H_1_2 */
        /** @{ */  /** <!-- ��loader����������ؽӿڡ� */

        /**
        \brief ��FLASH��E2ROM�л�ȡ������������.
        \attention \n
        �����������ĵ�ַ�ɶ�������CFG_LOADER_INFOBASIC_ADDR������\n
        \param[out] info  loader��������,�ⲿ�����ڴ�.
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_OTA_INFO_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_OTA_INFO_S����.
        */
        HI_U32 HI_LOADER_ReadOTAInfo(HI_LOADER_OTA_INFO_S* info) ;

        /**
        \brief ���������������ݵ�FLASH��E2ROM.
        \attention \n
        �����������ĵ�ַ�ɶ�������CCFG_LOADER_INFOBASIC_ADDR������.\n
        \param[in]  info Ҫ���µ���������.
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_OTA_INFO_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_OTA_INFO_S����.
        */
        HI_U32 HI_LOADER_WriteOTAInfo(HI_LOADER_OTA_INFO_S* info) ;

        /**
        \brief ��FLASH��E2ROM�л�ȡLOADER DB ��������.
        \attention \n
        �����������ĵ�ַ�ɶ�������CCFG_LOADER_INFOBASIC_ADDR������.����OTA����������ͬһ��,��LOADER_OFFESET(0x100)��ʼ\n
        LOADER DB������Ҫ�������ϲ��Ӧ�ó������ͨѶ,Ŀǰ��Ҫ����MCE���ٿ���������,��Ƶ���ò�����.
        \param[out] pstLoaderDb  loader DB ����.�ⲿ�����ڴ�.
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_DB_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_DB_S����.
        */
        HI_S32 HI_LOADER_ReadDBInfo(HI_LOADER_DB_S* pstLoaderDb) ;

        /**
        \brief ����LOADER DB �������ݵ�FLASH��E2ROM��.
        \attention \n
        �����������ĵ�ַ�ɶ�������CFG_LOADER_INFOBASIC_ADDR������.����OTA����������ͬһ��,��LOADER_OFFESET(0x100)��ʼ\n
        LOADER DB������Ҫ�������ϲ��Ӧ�ó������ͨѶ,Ŀǰ��Ҫ����MCE���ٿ���������,��Ƶ���ò�����.
        \param[in] pstLoaderDb  Ҫ���µ�loader DB ����.
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_DB_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_DB_S����.
        */
        HI_S32 HI_LOADER_WriteDBInfo(HI_LOADER_DB_S* pstLoaderDb) ;

        /**
        \brief ��FLASH��E2ROM�л�ȡ���������к���Ϣ.
        \attention \n
        ���������к���Ϣ���ĵ�ַ�ɶ�������CCFG_LOADER_INFOBASIC_ADDR������.
        ���кź�������ʱ��Ҫ�Ƚϵ�Ӳ���汾��,���̱�š���ͬ����Ӫ�̺ͳ����в�ͬ�Ĺ������е����к�.\n
        ���кŵĸ�ʽ�������:\n
                      �������к���:  15010300101010001c1d000021 \n
                      ע��:�������кſ�ѡ. \n
                      ���кŸ�ʽ����:\n
                ��ʽ��AAAA BB CCC DD EE X FFFF FFFF FFFF\n
                  AAAA  ��λ����ָʾ�� \n
                      BB        �ն˲�Ʒ����������֤���\n
                      CCC       �ն˲�Ʒ��������������\n
                      DD        �ն�����\n
                      EE        Ӳ���汾��\n
                      X         ʶ����\n
                      FFFFFFFFFFFF      ����ǰһλ�Ķ���ľ�����ֵ��    MAC ��ַ .\n
        \param[out] pstStbinfo  ���������к���Ϣ.�ⲿ�����ڴ�.
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_STB_INFO_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_STB_INFO_S����.
        */
        HI_S32 HI_LOADER_ReadSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);

        /**
        \brief ����FLASH��E2ROM���������к���Ϣ.
        \attention \n
        ���������к���Ϣ���ĵ�ַ�ɶ�������CFG_LOADER_STBINFO_ADDR������.\n
        ���кź�������ʱ��Ҫ�Ƚϵ�Ӳ���汾��,���̱�š�\n
        ��ͬ����Ӫ�̺ͳ����в�ͬ�Ĺ������е����кţ�\n
        ���ô˽ӿں���Զ������кŵ����12λ���ó�mac��ַ,\n
        �û��ɸ���ʵ����Ҫ���޸����кŹ���.\n
        �ýӿ�ֻ���au8OutSn(�������ⲿ���к�)������\n
        au8Sn����(����������ڲ����к�LOADER_INNER_STB_SN�궨��Ļ�).\n
        ���кŵĸ�ʽ�������:\n
                      �������к���:  15010300101010001c1d000021      \n
                      ע��:�������кſ�ѡ.            \n
                      ���кŸ�ʽ����:\n
                ��ʽ��AAAA BB CCC DD EE X FFFF FFFF FFFF\n
                  AAAA  ��λ����ָʾ��  \n
                      BB        �ն˲�Ʒ����������֤��� \n
                      CCC       �ն˲�Ʒ��������������\n
                      DD        �ն�����\n
                      EE        Ӳ���汾��\n
                      X         ʶ����\n
                      FFFFFFFFFFFF      ����ǰһλ�Ķ���ľ�����ֵ��    MAC ��ַ .\n
        \param[out] pstStbinfo  ���������к���Ϣ.�ⲿ�����ڴ�.\n
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_STB_INFO_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_STB_INFO_S����.
        */
        HI_S32 HI_LOADER_WriteSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);

        /**
        \brief ���FLASH��E2ROM�е�������ʶ.
        \attention \n
        �����������ĵ�ַ�ɶ�������CFG_LOADER_STBINFO_ADDR������.\n
        \retval ::HI_SUCCESS �ɹ�
        \retval ::HI_FAILURE  APIϵͳ����ʧ��
        \see ::HI_LOADER_OTA_INFO_S \n
        ��ϸ��Ϣ,��ο�HI_LOADER_OTA_INFO_S����.
        */
        HI_S32  HI_LOADER_ClearRequest() ;

        /** @} */  /** <!-- ==== API declaration end ==== */

#ifdef __cplusplus
#if __cplusplus
}

#endif
#endif

#endif /*__UPGRD_INFO_H__*/
