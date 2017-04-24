
#include "hisi_util.h"

#ifdef __cplusplus
extern "C"
{
#endif

        /**�����ļ�������󳤶�*/
#define LOADER_FILE_NAME_LEN       128
#define HI_SYSCFG_STBID_LEN        26   /**< �������ⲿ���к� */
#define HI_SYSCFG_STBSN_LEN        15   /**< �������ڲ����к� */

        /**IP��ַ����*/
        typedef HI_U32 HI_IPAddr;
        /**IP�˿ںŶ���*/
        typedef HI_U16 HI_IPPort;

        /**�������Ͷ���*/
        typedef enum tagUPDATE_TYPE_E {
                HI_UPDATE_OTA  = 0,/**< CABLE ���� */
                HI_UPDATE_IP,      /**< IP���� */
                HI_UPDATE_NULL,    /**< ������ */
        } HI_UPDATE_TYPE_E;

        /** ota�������Ͷ���,���ϲ���չʹ��.Ӧ�ð��չ淶д��(����1����ǿ�ơ�2�����ѡ) */
        typedef enum tagOTA_TYPE_E
        {
                HI_OTA_BUTT,
        } HI_OTA_TYPE_E;

        /**IP����ʱʹ��IP��ַ���Ͷ���*/
        typedef enum tagIP_TYPE_E {
                HI_IP_STATIC = 0,/**< ʹ�þ�̬IP */
                HI_IP_DHCP,      /**< ʹ�ö�̬IP */
                HI_IP_BUTT,
        } HI_IP_TYPE_E;

#if 0
        /**���������������ӿڽṹ��*/
        typedef struct tagLoaderOtaInfo
        {
                /* Ӧ�ú�loader �佻�� �ı��� */
                HI_U32      u32MagicNumber;     /**< Loader DB Magic Number ���� */
                HI_UPDATE_TYPE_E eUpdateType;   /**< ����, �������� , 0 :OTA, 1:IP,2:������*/
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

                HI_U8       au8AppUse[20];      /**< �ϲ�Ӧ��˽�� */

                HI_UPDATE_TYPE_E eLastUpdType;  /**< ˽�� */
                HI_U32      u32FailedCnt;       /**< ����ʧ�ܴ���,�û��������.*/
                HI_U32      u32DestroyFlag;     /**< �û��������.*/
                HI_U8       au8LoaderReserved[8];  /**<  ˽�� */
                HI_U32      u32Crc;                  /**< crc У��ֵ */
        } HI_LOADER_OTA_INFO_S;
#else
        typedef enum
        {
                OUTPUT_INTERFACE_NONE=0x0,
                OUTPUT_INTERFACE_CVBS2_SVIDEO,           //0x1
                OUTPUT_INTERFACE_CVBS2,                  //0x2
                OUTPUT_INTERFACE_CVBS3,                  //0x3
                OUTPUT_INTERFACE_CVBS4,                  //0x4
                OUTPUT_INTERFACE_CVBS_SVIDEO,            //0x5
                OUTPUT_INTERFACE_CVBS_YUV,               //0x6
                OUTPUT_INTERFACE_CVBS_RGB,               //0x7
                OUTPUT_INTERFACE_SVIDEO_SVIDEO,          //0x8
        } OUTPUT_INTERFACE_E;
        typedef enum
        {
                TV_MODE_AUTO = 0,
                TV_MODE_PAL,
                TV_MODE_PAL_M,//  PAL3.58
                TV_MODE_PAL_N,
                TV_MODE_NTSC358,//NTSC3.58
                TV_MODE_NTSC443,
                TV_MODE_SECAM,
                TV_MODE_480I,//���������
                TV_MODE_480P,
                TV_MODE_576I,//���������
                TV_MODE_576P,
                TV_MODE_720P_50,
                TV_MODE_720P_60,
                TV_MODE_1080I_50,
                TV_MODE_1080I_60,
                TV_MODE_1080P_24,
                TV_MODE_1080P_25,
                TV_MODE_1080P_30,
                TV_MODE_1080P_50,
                TV_MODE_1080P_60,
        } TV_MODE_TYPE;
        typedef enum otaloader_type
        {
                Single_Loader = 0,
                Double_Loader   = 1,
                Sounth_Loader  = 2,
                Irdeto_Loader = 3,
        } Loader_Type;
        /** ����汾���Ͷ��� */
        typedef struct LDR_SoftwareVersion
        {
                HI_U16  SolutionId;            /**< ������������ʶ */
                HI_U16  MajorVersion;          /**< ���汾�� */
                HI_U16  MinorVersion;          /**< �ΰ汾�� */
        } LDR_SoftwareVersion ;
        typedef struct _mce_db
        {
                HI_U64 u8AdvMinTime;
                HI_U64 u8AdvMaxTime;
                HI_U64 resolve2;
                HI_U64 u16ServiceId;
                HI_U64 u16TsId;
                HI_U64 u32Freq;
                HI_U64 u16SymboRate;
                HI_U64 u16VideoPid;
                HI_U64 u16AudioPid;
                HI_U64 u8ChannelNum;
                HI_U64 u8Qam;
                HI_U64 u8Volume;
        } MCE_DB_S, *PTR_MCE_DB_S;
        /**���������������ӿڽṹ��*/
        typedef struct tagLoaderOtaInfo
        {
                /* Ӧ�ú�loader �佻�� �ı��� */
                HI_U32 u32MagicNumber; /* Loader DB Magic Number ���� */
                HI_U32 db_len; /* Loader DB ���ܳ��ȣ���magic_number���𣬵�u32Crc��ֹ */
                HI_U32 ota_type; /* ��������*/
                OUTPUT_INTERFACE_E enOutputInterface;    /* ��Ƶ���ģʽ*/
                TV_MODE_TYPE enTvModetype;          /* ��Ƶ�����ʽ*/
                TV_MODE_TYPE enHdTvModetype;          /* ��Ƶ�����ʽ*/
                HI_U32 u32LoaderDestroyFlag;         /*LOADER�ƻ���־*/
                HI_U32 u32FailedCnt; /* ����ʧ�ܴ��� */
                HI_U32 u32DestroyFlag;  /* �����𻵱�־ */

                HI_U32 enAspectRatio;
                HI_U32 enAspectConversionMode;
                HI_U32 enEncodeFormat;

                Loader_Type loader_type;
                HI_U32 u32OtaPid; /* ������pid */
                HI_U32 u32OtaFreq; /* ������Ƶ��, ��λΪ10KHz */
                HI_U32 u32OtaSymbRate; /* ������ */
                HI_U32 u32OtaModulation; /* ����ģʽ */
                HI_U32 u32NewSoftware; /* ������������汾�� */
                HI_U32 u32CurSoftware; /* ��ǰ����汾�� */
                HI_U32 u32HardwareVersion; /* Ӳ���汾�� */

                HI_U32 u32NewMajorSwVersion; /* ��������汾 */
                HI_U32 u32NewMinorSwVersion; /* ������ΰ汾 */
                HI_U32 u32MajorSwVersion; /* ������汾 */
                HI_U32 u32MinorSwVersion; /* ����ΰ汾 */
                HI_U32 u32MajorHardwareVersion; /* Ӳ�����汾�� */
                HI_U32 u32MinorHardwareVersion; /* Ӳ���ΰ汾�� */
                HI_U32 u32Regioncode; /* ������ */
                HI_U32 u32HwBatchNo; /*���� */
                HI_U32 stb_type; /* ��Ӫ�̹�Կ */
                HI_U32 time; /* ����Ԥ��ʱ�� */

#if 1 //TF_LOADER
                HI_U32 u32HardwareSubVersion; /*Ӳ���Ӱ汾��*/
                LDR_SoftwareVersion appVersion;  /*ͬ�������Ҫ������汾*/
                LDR_SoftwareVersion ldVersion;  /*ͬ�������Ҫ��loader�汾*/
                LDR_SoftwareVersion lgVersion;
                LDR_SoftwareVersion MCEVersion;
                LDR_SoftwareVersion KerVersion;
                LDR_SoftwareVersion RBVersion;
#endif

                HI_U32 download_data; /* �������� */

                /* AMLOGICƽ̨LoaderҪ�õ������� */
                HI_U32 oui_code; /*  ����ID */
                HI_U32 stb_sn; /* ���������к�*/
                HI_U32 upgrade_type;  /*  ��������*/
                HI_U32 manual_upgrade_flag; /* �ֶ�������ʶ*/
                HI_U32 table_id; /* ������table id */
                HI_U32 u32Reserved6;
                HI_U32 u32Reserved7;
                HI_U32 u32Reserved8;
                MCE_DB_S stMCEDb;
                HI_U8 au8AppUse[24];      /**< �ϲ�Ӧ��˽�� */
                HI_U32 RESERVED;
                HI_U32 u32Crc;                  /**< crc У��ֵ */
        } HI_LOADER_OTA_INFO_S;

#endif

        /** ��������Ϣ */
        typedef struct hiLOADER_STBINFO_S
        {
                HI_U32 u32OUI;       /**< ����id */
                HI_U32 u32AreaCode;  /**< ������ */
                HI_U32 u32HWVersion; /**< Ӳ���汾�� */
                HI_U32 u32HWBatch;   /**< Ӳ������ */
                HI_U8  au8OutSn[HI_SYSCFG_STBID_LEN];   /**< �������ⲿ���к� */
                HI_U8  au8Sn[HI_SYSCFG_STBSN_LEN];   /**< �������ڲ����к� */
        } HI_LOADER_STB_INFO_S;

        HI_U32 HI_APP_LOADER_ReadLoaderInfo(HI_LOADER_OTA_INFO_S* info);
        HI_S32 HI_APP_LOADER_WriteLoaderInfo(HI_LOADER_OTA_INFO_S* info);
        HI_S32 HI_APP_LOADER_ReadSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);
        HI_S32 HI_APP_LOADER_WriteSTBInfo(HI_LOADER_STB_INFO_S* pstStbinfo);

#ifdef __cplusplus
}
#endif

