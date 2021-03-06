#ifndef CDCAS_CALIB_APIEX_H
#define CDCAS_CALIB_APIEX_H

#ifdef  __cplusplus
extern "C" {
#endif


/*-------------------------------------基本数据类型---------------------------------------*/
typedef unsigned  long  CDCA_U32;
typedef unsigned  short CDCA_U16;
typedef unsigned  char  CDCA_U8;

typedef CDCA_U8 CDCA_BOOL;

#define CDCA_TRUE    ((CDCA_BOOL)1)
#define CDCA_FALSE   ((CDCA_BOOL)0)

/*----------------------------------------宏定义--------------------------------------*/

/*--------- FLASH类型 -------*/
#define CDCA_FLASH_BLOCK_A        1     /* BLOCK A */
#define CDCA_FLASH_BLOCK_B        2     /* BLOCK B */

/*--------- 智能卡相关限制 --------*/
#define CDCA_MAXLEN_SN            16U   /* 智能卡序列号的长度 */
#define CDCA_MAXLEN_PINCODE       6U    /* PIN码的长度 */
#define CDCA_MAXLEN_TVSPRIINFO    32U   /* 运营商私有信息的长度 */
#define CDCA_MAXNUM_OPERATOR      4U    /* 最多的运营商个数 */
#define CDCA_MAXNUM_ACLIST        18U   /* 智能卡内保存的每个运营商的用户特征个数 */
#define CDCA_MAXNUM_SLOT          20U   /* 卡存储的最大钱包数 */

#define CDCA_MAXNUM_CURTAIN_RECORD	80U /*窗帘节目记录的最大个数*/


#define CDCA_MAXNUM_IPPVP         300U  /* 智能卡保存最多IPPV节目的个数 */
#define CDCA_MAXNUM_PRICE         2U    /* 最多的IPPV价格个数 */

#define CDCA_MAXNUM_ENTITLE       300U  /* 授权产品的最大个数 */

/*---------- 机顶盒相关限制 ----------*/
#define CDCA_MAXNUM_PROGRAMBYCW   4U    /* 一个控制字最多解的节目数 */
#define CDCA_MAXNUM_ECM           8U    /* 同时接收处理的ECMPID的最大数量 */

#define CDCA_MAXNUM_DETITLE       5U    /* 每个运营商下可保存的反授权码个数 */

/*---------- CAS提示信息 ---------*/
#define CDCA_MESSAGE_CANCEL_TYPE      0x00  /* 取消当前的显示 */
#define CDCA_MESSAGE_BADCARD_TYPE     0x01  /* 无法识别卡 */
#define CDCA_MESSAGE_EXPICARD_TYPE    0x02  /* 智能卡过期，请更换新卡 */
#define CDCA_MESSAGE_INSERTCARD_TYPE  0x03  /* 加扰节目，请插入智能卡 */
#define CDCA_MESSAGE_NOOPER_TYPE      0x04  /* 卡中不存在节目运营商 */
#define CDCA_MESSAGE_BLACKOUT_TYPE    0x05  /* 条件禁播 */
#define CDCA_MESSAGE_OUTWORKTIME_TYPE 0x06  /* 当前时段被设定为不能观看 */
#define CDCA_MESSAGE_WATCHLEVEL_TYPE  0x07  /* 节目级别高于设定的观看级别 */
#define CDCA_MESSAGE_PAIRING_TYPE     0x08  /* 智能卡与本机顶盒不对应 */
#define CDCA_MESSAGE_NOENTITLE_TYPE   0x09  /* 没有授权 */
#define CDCA_MESSAGE_DECRYPTFAIL_TYPE 0x0A  /* 节目解密失败 */
#define CDCA_MESSAGE_NOMONEY_TYPE     0x0B  /* 卡内金额不足 */
#define CDCA_MESSAGE_ERRREGION_TYPE   0x0C  /* 区域不正确 */
#define CDCA_MESSAGE_NEEDFEED_TYPE    0x0D  /* 子卡需要和母卡对应，请插入母卡 */
#define CDCA_MESSAGE_ERRCARD_TYPE     0x0E  /* 智能卡校验失败，请联系运营商 */
#define CDCA_MESSAGE_UPDATE_TYPE      0x0F  /* 智能卡升级中，请不要拔卡或者关机 */
#define CDCA_MESSAGE_LOWCARDVER_TYPE  0x10  /* 请升级智能卡 */
#define CDCA_MESSAGE_VIEWLOCK_TYPE    0x11  /* 请勿频繁切换频道 */
#define CDCA_MESSAGE_MAXRESTART_TYPE  0x12  /* 智能卡暂时休眠，请5分钟后重新开机 */
#define CDCA_MESSAGE_FREEZE_TYPE      0x13  /* 智能卡已冻结，请联系运营商 */
#define CDCA_MESSAGE_CALLBACK_TYPE    0x14  /* 智能卡已暂停，请回传收视记录给运营商 */
#define CDCA_MESSAGE_CURTAIN_TYPE	  0x15 /*窗帘节目，不可预览阶段*/
#define CDCA_MESSAGE_CARDTESTSTART_TYPE 0x16 /*升级测试卡测试中...*/
#define CDCA_MESSAGE_CARDTESTFAILD_TYPE 0x17 /*升级测试卡测试失败，请检查机卡通讯模块*/
#define CDCA_MESSAGE_CARDTESTSUCC_TYPE  0x18 /*升级测试卡测试成功*/
#define CDCA_MESSAGE_NOCALIBOPER_TYPE    0x19/*卡中不存在移植库定制运营商*/

#define CDCA_MESSAGE_STBLOCKED_TYPE   0x20  /* 请重启机顶盒 */
#define CDCA_MESSAGE_STBFREEZE_TYPE   0x21  /* 机顶盒被冻结 */



/*---------- 功能调用返回值定义 ----------*/
#define CDCA_RC_OK                    0x00  /* 成功 */
#define CDCA_RC_UNKNOWN               0x01  /* 未知错误 */
#define CDCA_RC_POINTER_INVALID       0x02  /* 指针无效 */
#define CDCA_RC_CARD_INVALID          0x03  /* 智能卡无效 */
#define CDCA_RC_PIN_INVALID           0x04  /* PIN码无效 */
#define CDCA_RC_DATASPACE_SMALL       0x06  /* 所给的空间不足 */
#define CDCA_RC_CARD_PAIROTHER        0x07  /* 智能卡已经对应别的机顶盒 */
#define CDCA_RC_DATA_NOT_FIND         0x08  /* 没有找到所要的数据 */
#define CDCA_RC_PROG_STATUS_INVALID   0x09  /* 要购买的节目状态无效 */
#define CDCA_RC_CARD_NO_ROOM          0x0A  /* 智能卡没有空间存放购买的节目 */
#define CDCA_RC_WORKTIME_INVALID      0x0B  /* 设定的工作时段无效 */
#define CDCA_RC_IPPV_CANNTDEL         0x0C  /* IPPV节目不能被删除 */
#define CDCA_RC_CARD_NOPAIR           0x0D  /* 智能卡没有对应任何的机顶盒 */
#define CDCA_RC_WATCHRATING_INVALID   0x0E  /* 设定的观看级别无效 */
#define CDCA_RC_CARD_NOTSUPPORT       0x0F  /* 当前智能卡不支持此功能 */
#define CDCA_RC_DATA_ERROR            0x10  /* 数据错误，智能卡拒绝 */
#define CDCA_RC_FEEDTIME_NOT_ARRIVE   0x11  /* 喂养时间未到，子卡不能被喂养 */
#define CDCA_RC_CARD_TYPEERROR        0x12  /* 子母卡喂养失败，插入智能卡类型错误 */

#define CDCA_RC_CAS_FAILED                  0x20 //发卡cas指令执行失败
#define CDCA_RC_OPER_FAILED                0x21 //发卡运营商指令执行失败


/*-- 读卡器中的智能卡状态 --*/
#define CDCA_SC_OUT         0x00    /* 读卡器中没有卡          */
#define CDCA_SC_REMOVING    0x01    /* 正在拔卡，重置状态      */
#define CDCA_SC_INSERTING   0x02    /* 正在插卡，初始化        */
#define CDCA_SC_IN          0x03    /* 读卡器中是可用的卡      */
#define CDCA_SC_ERROR       0x04    /* 读卡器的卡不能识别      */
#define CDCA_SC_UPDATE      0x05    /* 读卡器的卡可升级 */
#define CDCA_SC_UPDATE_ERR  0x06    /* 读卡器的卡升级失败      */

/*---------- ECM_PID设置的操作类型 ---------*/
#define CDCA_LIST_OK          0x00
#define CDCA_LIST_FIRST       0x01
#define CDCA_LIST_ADD         0x02


/*------------ 邮件大小及数量限制 ------------*/
#define CDCA_MAXNUM_EMAIL         100U  /* 机顶盒保存的最大邮件个数 */
#define CDCA_MAXLEN_EMAIL_TITLE   30U   /* 邮件标题的长度 */
#define CDCA_MAXLEN_EMAIL_CONTENT 160U  /* 邮件内容的长度 */

/*------------ 邮件图标显示方式 ------------*/
#define CDCA_Email_IconHide       0x00  /* 隐藏邮件通知图标 */
#define CDCA_Email_New            0x01  /* 新邮件通知，显示新邮件图标 */
#define CDCA_Email_SpaceExhaust   0x02  /* 磁盘空间以满，图标闪烁。 */

/*------------ OSD的长度限制 -----------*/
#define CDCA_MAXLEN_OSD           180U  /* OSD内容的最大长度 */

/*------------ OSD显示类型 ------------*/
#define CDCA_OSD_TOP              0x01  /* OSD风格：显示在屏幕上方 */
#define CDCA_OSD_BOTTOM           0x02  /* OSD风格：显示在屏幕下方 */
#define CDCA_OSD_FULLSCREEN       0x03  /* OSD风格：整屏显示 */
#define CDCA_OSD_HALFSCREEN       0x04  /* OSD风格：半屏显示 */


/*------------ IPPV/IPPT不同购买阶段提示 -------------*/
#define CDCA_IPPV_FREEVIEWED_SEGMENT  0x00  /* IPPV免费预览阶段，是否购买 */
#define CDCA_IPPV_PAYVIEWED_SEGMENT   0x01  /* IPPV收费阶段，是否购买 */
#define CDCA_IPPT_PAYVIEWED_SEGMENT   0x02  /* IPPT收费段，是否购买 */

/*------------ IPPV价格类型 ------------*/
#define CDCA_IPPVPRICETYPE_TPPVVIEW       0x0  /* 不回传，不录像类型 */
#define CDCA_IPPVPRICETYPE_TPPVVIEWTAPING 0x1  /* 不回传，可录像类型 */

/*------------ IPPV节目的状态 -----------*/
#define CDCA_IPPVSTATUS_BOOKING   0x01  /* 预定 */
#define CDCA_IPPVSTATUS_VIEWED    0x03  /* 已看 */


/*---------- 频道锁定应用相关定义 ---------*/
#define CDCA_MAXNUM_COMPONENT     5U    /* 节目组件最大个数 */
#define CDCA_MAXLEN_LOCKMESS      40U


/*---------- 反授权确认码应用相关定义 --------*/
#define CDCA_Detitle_All_Read     0x00  /* 所有反授权确认码已经被读，隐藏图标 */
#define CDCA_Detitle_Received     0x01  /* 收到新的反授权码，显示反授权码图标 */
#define CDCA_Detitle_Space_Small  0x02  /* 反授权码空间不足，改变图标状态提示用户 */
#define CDCA_Detitle_Ignore       0x03  /* 收到重复的反授权码，可忽略，不做处理 */


/*---------- 进度条提示信息 ---------*/
#define CDCA_SCALE_RECEIVEPATCH   1     /* 升级数据接收中 */
#define CDCA_SCALE_PATCHING       2     /* 智能卡升级中 */



/*-------------------------------------end of 宏定义--------------------------------------*/



/*----------------------------------------数据结构----------------------------------------*/

/*-- 系统时间 --*/
typedef CDCA_U32  CDCA_TIME;
typedef CDCA_U16  CDCA_DATE;

/*-- 信号量定义（不同的操作系统可能不一样）--*/
typedef CDCA_U32  CDCA_Semaphore;

/*-- 运营商信息 --*/
typedef struct {
    char     m_szTVSPriInfo[CDCA_MAXLEN_TVSPRIINFO+1];  /* 运营商私有信息 */
    CDCA_U8  m_byReserved[3];    /* 保留 */
}SCDCAOperatorInfo;

/*-- 节目信息 --*/
/* Y10_update : 只需要ECMPID和ServiceID即可 */
typedef struct {
    CDCA_U16  m_wEcmPid;         /* 节目相应控制信息的PID */
    CDCA_U8   m_byServiceNum;    /* 当前PID下的节目个数 */
    CDCA_U8   m_byReserved;      /* 保留 */
    CDCA_U16  m_wServiceID[CDCA_MAXNUM_PROGRAMBYCW]; /* 当前PID下的节目ID列表 */
}SCDCASServiceInfo;

/*-- 授权信息 --*/
typedef struct {
    CDCA_U32  m_dwProductID;   /* 普通授权的节目ID */    
    CDCA_DATE m_tBeginDate;    /* 授权的起始时间 */
    CDCA_DATE m_tExpireDate;   /* 授权的过期时间 */
    CDCA_U8   m_bCanTape;      /* 用户是否购买录像：1－可以录像；0－不可以录像 */
    CDCA_U8   m_byReserved[3]; /* 保留 */
}SCDCAEntitle;

/*-- 授权信息集合 --*/
typedef struct {
    CDCA_U16      m_wProductCount;
    CDCA_U8       m_m_byReserved[2];    /* 保留 */
    SCDCAEntitle  m_Entitles[CDCA_MAXNUM_ENTITLE]; /* 授权列表 */
}SCDCAEntitles;

/*-- 钱包信息 --*/
typedef struct {
    CDCA_U32  m_wCreditLimit; /* 信用度（点数）*/
    CDCA_U32  m_wBalance;     /* 已花的点数 */
}SCDCATVSSlotInfo;

/*-- IPPV/IPPT节目的价格 --*/
typedef struct {
    CDCA_U16  m_wPrice;       /* 节目价格（点数）*/
    CDCA_U8   m_byPriceCode;  /* 节目价格类型 */
    CDCA_U8   m_byReserved;   /* 保留 */
}SCDCAIPPVPrice; 

/*-- IPPV/IPPT节目购买提示信息 --*/
typedef struct {
    CDCA_U32        m_dwProductID;          /* 节目的ID */
    CDCA_U16        m_wTvsID;               /* 运营商ID */
    CDCA_U8         m_bySlotID;             /* 钱包ID */
    CDCA_U8         m_byPriceNum;           /* 节目价格个数 */
    SCDCAIPPVPrice  m_Price[CDCA_MAXNUM_PRICE]; /* 节目价格 */
    union {
        CDCA_DATE   m_wExpiredDate;         /* 节目过期时间,IPPV用 */
        CDCA_U16    m_wIntervalMin;         /* 时间间隔，单位分钟,IPPT 用 */
    }m_wIPPVTime;
    CDCA_U8         m_byReserved[2];        /* 保留 */
}SCDCAIppvBuyInfo;

/*-- IPPV节目信息 --*/
typedef struct {
    CDCA_U32   m_dwProductID;   /* 节目的ID */
    CDCA_U8    m_byBookEdFlag;  /* 产品状态：BOOKING，VIEWED */ 
    CDCA_U8    m_bCanTape;      /* 是否可以录像：1－可以录像；0－不可以录像 */
    CDCA_U16   m_wPrice;        /* 节目价格 */
    CDCA_DATE  m_wExpiredDate;  /* 节目过期时间,IPPV用 */
    CDCA_U8    m_bySlotID;      /* 钱包ID */
    CDCA_U8    m_byReserved;    /* 保留 */
}SCDCAIppvInfo;


/*-- 邮件头 --*/
typedef struct {
    CDCA_U32   m_dwActionID;                 /* Email ID */
    CDCA_U32   m_tCreateTime;                /* EMAIL创建的时间 */
    CDCA_U16   m_wImportance;                /* 重要性： 0－普通，1－重要 */
    CDCA_U8    m_byReserved[2];              /* 保留 */
    char       m_szEmailHead[CDCA_MAXLEN_EMAIL_TITLE+1]; /* 邮件标题，最长为30 */    
    CDCA_U8    m_bNewEmail;                  /* 新邮件标记：0－已读邮件；1－新邮件 */
}SCDCAEmailHead;

/*-- 邮件内容 --*/
typedef struct {
    char     m_szEmail[CDCA_MAXLEN_EMAIL_CONTENT+1];      /* Email的正文 */
    CDCA_U8  m_byReserved[3];              /* 保留 */
}SCDCAEmailContent;


/*-- 频道锁定信息 --*/
/*-- 节目组件信息 --*/
typedef struct {    /* 组件用于通知机顶盒节目类型及PID等信息，一个节目可能包含多个组件 */
    CDCA_U16   m_wCompPID;     /* 组件PID */
    CDCA_U16   m_wECMPID;      /* 组件对应的ECM包的PID，如果组件是不加扰的，则应取0。 */
    CDCA_U8    m_CompType;     /* 组件类型 */
    CDCA_U8    m_byReserved[3];/* 保留 */
}SCDCAComponent;

/*-- 频道参数信息 --*/
typedef struct {    
    CDCA_U32   m_dwFrequency;              /* 频率，BCD码 */
    CDCA_U32   m_symbol_rate;              /* 符号率，BCD码 */
    CDCA_U16   m_wPcrPid;                  /* PCR PID */
    CDCA_U8    m_Modulation;               /* 调制方式 */
    CDCA_U8    m_ComponentNum;             /* 节目组件个数 */
    SCDCAComponent m_CompArr[CDCA_MAXNUM_COMPONENT];       /* 节目组件列表 */
    CDCA_U8    m_fec_outer;                /* 前项纠错外码 */
    CDCA_U8    m_fec_inner;                /* 前项纠错内码 */
    char       m_szBeforeInfo[CDCA_MAXLEN_LOCKMESS+1]; /* 保留 */
    char       m_szQuitInfo[CDCA_MAXLEN_LOCKMESS+1];   /* 保留 */
    char       m_szEndInfo[CDCA_MAXLEN_LOCKMESS+1];    /* 保留 */
}SCDCALockService;

/*-- 窗帘节目信息 --*/
typedef struct {
    CDCA_U16   m_wProgramID;        /* 节目的ID */
    CDCA_TIME  m_dwStartWatchTime;   /* 起始观看时间 */
    CDCA_U8    m_byWatchTotalCount;  /* 累计观看次数 */
    CDCA_U16   m_wWatchTotalTime;    /* 累计观看时长,(cp周期数) */
}SCDCACurtainInfo;


/*-----------------------------------------------------------------------------------
a. 本系统中，参数m_dwFrequency和m_symbol_rate使用BCD码，编码前取MHz为单位。
   编码时，前4个4-bit BCD码表示小数点前的值，后4个4-bit BCD码表示小数点后的值。
   例如：
        若频率为642000KHz，即642.0000MHz，则对应的m_dwFrequency的值应为0x06420000；
        若符号率为6875KHz，即6.8750MHz，则对应的m_symbol_rate的值应为0x00068750。

b. 本系统中，m_Modulation的取值如下：
    0       Reserved
    1       QAM16
    2       QAM32
    3       QAM64
    4       QAM128
    5       QAM256
    6～255  Reserved
------------------------------------------------------------------------------------*/ 


/*------------------------------------end of 数据结构-------------------------------------*/



/*---------------------------以下接口是CA_LIB提供给STB------------------------*/

/*------ CA_LIB调度管理 ------*/

/* CA_LIB初始化 */
extern CDCA_BOOL CDCASTB_Init( CDCA_U8 byThreadPrior );

/* 关闭CA_LIB，释放资源 */
extern void CDCASTB_Close( void );

/* CDCAS同密判断 */
extern CDCA_BOOL  CDCASTB_IsCDCa(CDCA_U16 wCaSystemID);


/*------ Flash管理 ------ */

/* 存储空间的格式化 */
extern void CDCASTB_FormatBuffer( void );

/* 屏蔽对存储空间的读写操作 */
extern void CDCASTB_RequestMaskBuffer(void);

/* 打开对存储空间的读写操作 */
extern void CDCASTB_RequestUpdateBuffer(void);


/*------ TS流管理 ------*/

/* 设置ECM和节目信息 */
extern void CDCASTB_SetEcmPid( CDCA_U8 byType,
                               const SCDCASServiceInfo* pServiceInfo );

/* 设置EMM信息 */
extern void  CDCASTB_SetEmmPid(CDCA_U16 wEmmPid);

/* 私有数据接收回调 */
extern void CDCASTB_PrivateDataGot( CDCA_U8        byReqID,
								  	CDCA_BOOL      bTimeout,
									CDCA_U16       wPid,
									const CDCA_U8* pbyReceiveData,
									CDCA_U16       wLen            );

/*------- 智能卡管理 -------*/

/* 插入智能卡 */
extern CDCA_BOOL CDCASTB_SCInsert( void );

/* 拔出智能卡*/
extern void CDCASTB_SCRemove( void );

/* 读取智能卡外部卡号 */
extern CDCA_U16 CDCASTB_GetCardSN( char* pCardSN );

/* PIN码管理 */
extern CDCA_U16 CDCASTB_ChangePin( const CDCA_U8* pbyOldPin,
                                   const CDCA_U8* pbyNewPin);

/* 设置用户观看级别 */
extern CDCA_U16 CDCASTB_SetRating( const CDCA_U8* pbyPin,
                                   CDCA_U8 byRating );

/* 查询用户观看级别 */
extern CDCA_U16 CDCASTB_GetRating( CDCA_U8* pbyRating );

/* 设置智能卡工作时段 */
extern CDCA_U16 CDCASTB_SetWorkTime( const CDCA_U8* pbyPin,
									 CDCA_U8        byStartHour,
									 CDCA_U8        byStartMin,
									 CDCA_U8        byStartSec,
									 CDCA_U8        byEndHour,
									 CDCA_U8        byEndMin,
									 CDCA_U8        byEndSec    );

/* 查询智能卡当前工作时段 */
extern CDCA_U16 CDCASTB_GetWorkTime( CDCA_U8* pbyStartHour,
									 CDCA_U8* pbyStartMin,
									 CDCA_U8* pbyStartSec,
									 CDCA_U8* pbyEndHour,
									 CDCA_U8* pbyEndMin,
									 CDCA_U8* pbyEndSec   );

/*------- 基本信息查询 -------*/

/* 查询CA_LIB版本号 */
extern CDCA_U32 CDCASTB_GetVer( void );

/* 查询运营商ID列表 */
extern CDCA_U16 CDCASTB_GetOperatorIds( CDCA_U16* pwTVSID );

/* 查询运营商信息 */
extern CDCA_U16 CDCASTB_GetOperatorInfo( CDCA_U16           wTVSID,
                                         SCDCAOperatorInfo* pOperatorInfo );

/* 查询用户特征 */
extern CDCA_U16 CDCASTB_GetACList( CDCA_U16 wTVSID, CDCA_U32* pACArray );

/* 查询钱包ID列表 */
extern CDCA_U16 CDCASTB_GetSlotIDs( CDCA_U16 wTVSID, CDCA_U8* pbySlotID );

/* 查询钱包的详细信息 */
extern CDCA_U16 CDCASTB_GetSlotInfo( CDCA_U16          wTVSID,
                                     CDCA_U8           bySlotID,
                                     SCDCATVSSlotInfo* pSlotInfo );

/* 查询普通授权节目购买情况 */
extern CDCA_U16 CDCASTB_GetServiceEntitles( CDCA_U16       wTVSID,
                                            SCDCAEntitles* pServiceEntitles );



/*-------- 授权信息管理 --------*/

/* 查询授权ID列表 */
extern CDCA_U16 CDCASTB_GetEntitleIDs( CDCA_U16  wTVSID,
                                       CDCA_U32* pdwEntitleIds );


/* 查询反授权确认码 */
extern CDCA_U16 CDCASTB_GetDetitleChkNums( CDCA_U16   wTvsID,
                                           CDCA_BOOL* bReadFlag,
                                           CDCA_U32*  pdwDetitleChkNums);

/* 查询反授权信息读取状态 */
extern CDCA_BOOL CDCASTB_GetDetitleReaded( CDCA_U16 wTvsID );

/* 删除反授权确认码 */
extern CDCA_BOOL CDCASTB_DelDetitleChkNum( CDCA_U16 wTvsID,
                                           CDCA_U32 dwDetitleChkNum );



/*------- 机卡对应 -------*/

/* 查询机卡对应情况 */
extern CDCA_U16 CDCASTB_IsPaired( CDCA_U8* pbyNum,
                                  CDCA_U8* pbySTBID_List );

/* 查询机顶盒平台编号 */
extern CDCA_U16 CDCASTB_GetPlatformID( void );


/*-------- IPPV应用 -------*/

/* IPPV节目购买 */
extern CDCA_U16 CDCASTB_StopIPPVBuyDlg( CDCA_BOOL       bBuyProgram,
                                        CDCA_U16        wEcmPid,
                                        const CDCA_U8*  pbyPinCode,
                                        const SCDCAIPPVPrice* pPrice );

/* IPPV节目购买情况查询 */
extern CDCA_U16 CDCASTB_GetIPPVProgram( CDCA_U16       wTvsID,
                                        SCDCAIppvInfo* pIppv,
                                        CDCA_U16*      pwNumber );



/*-------- 邮件管理 --------*/

/* 查询邮件头信息 */
extern CDCA_U16 CDCASTB_GetEmailHeads( SCDCAEmailHead* pEmailHead,
                                       CDCA_U8*        pbyCount,
                                       CDCA_U8*        pbyFromIndex );

/* 查询指定邮件的头信息 */
extern CDCA_U16 CDCASTB_GetEmailHead( CDCA_U32        dwEmailID,
                                      SCDCAEmailHead* pEmailHead );

/* 查询指定邮件的内容 */
extern CDCA_U16 CDCASTB_GetEmailContent( CDCA_U32           dwEmailID,
                                         SCDCAEmailContent* pEmailContent );

/* 删除邮件 */
extern void CDCASTB_DelEmail( CDCA_U32 dwEmailID );

/* 查询邮箱使用情况 */
extern CDCA_U16 CDCASTB_GetEmailSpaceInfo( CDCA_U8* pbyEmailNum,
                                           CDCA_U8* pbyEmptyNum );



/*-------- 子母卡应用 --------*/

/* 读取子母卡信息 */
extern CDCA_U16 CDCASTB_GetOperatorChildStatus( CDCA_U16   wTVSID,
                                                CDCA_U8*   pbyIsChild,
                                                CDCA_U16*   pwDelayTime,
                                                CDCA_TIME* pLastFeedTime,
                                                char*      pParentCardSN,
                                                CDCA_BOOL *pbIsCanFeed );

/* 读取母卡喂养数据 */
extern CDCA_U16 CDCASTB_ReadFeedDataFromParent( CDCA_U16 wTVSID,
                                                CDCA_U8* pbyFeedData,
                                                CDCA_U8* pbyLen     );

/* 喂养数据写入子卡 */
extern CDCA_U16 CDCASTB_WriteFeedDataToChild( CDCA_U16       wTVSID,
                                              const CDCA_U8* pbyFeedData,
                                              CDCA_U8        byLen    );



/*-------- 显示界面管理 --------*/

/* 刷新界面 */
extern void CDCASTB_RefreshInterface( void );


/*-------- 双向模块接口 -------*/
    

/*------------------------以上接口是CA_LIB提供给STB---------------------------*/

/******************************************************************************/

/*------------------------以下接口是STB提供给CA_LIB---------------------------*/

/*-------- 线程管理 --------*/

/* 注册任务 */
extern CDCA_BOOL CDSTBCA_RegisterTask( const char* szName,
                                       CDCA_U8     byPriority,
                                       void*       pTaskFun,
                                       void*       pParam,
                                       CDCA_U16    wStackSize  );

/* 线程挂起 */
extern void CDSTBCA_Sleep(CDCA_U16 wMilliSeconds);


/*-------- 信号量管理 --------*/

/* 初始化信号量 */
extern void CDSTBCA_SemaphoreInit( CDCA_Semaphore* pSemaphore,
                                   CDCA_BOOL       bInitVal );

/* 信号量给予信号 */
extern void CDSTBCA_SemaphoreSignal( CDCA_Semaphore* pSemaphore );

/* 信号量获取信号 */
extern void CDSTBCA_SemaphoreWait( CDCA_Semaphore* pSemaphore );


/*-------- 内存管理 --------*/

/* 分配内存 */
extern void* CDSTBCA_Malloc( CDCA_U32 byBufSize );

/* 释放内存 */
extern void  CDSTBCA_Free( void* pBuf );

/* 内存赋值 */
extern void  CDSTBCA_Memset( void*    pDestBuf,
                             CDCA_U8  c,
                             CDCA_U32 wSize );

/* 内存复制 */
extern void  CDSTBCA_Memcpy( void*       pDestBuf,
                             const void* pSrcBuf,
                             CDCA_U32    wSize );


/*--------- 存储空间（Flash）管理 ---------*/

/* 读取存储空间 */
extern void CDSTBCA_ReadBuffer( CDCA_U8   byBlockID,
                                CDCA_U8*  pbyData,
                                CDCA_U32* pdwLen );

/* 写入存储空间 */
extern void CDSTBCA_WriteBuffer( CDCA_U8        byBlockID,
                                 const CDCA_U8* pbyData,
                                 CDCA_U32       dwLen );


/*-------- TS流管理 --------*/

/* 设置私有数据过滤器 */
extern CDCA_BOOL CDSTBCA_SetPrivateDataFilter( CDCA_U8        byReqID,  
											   const CDCA_U8* pbyFilter,  
											   const CDCA_U8* pbyMask, 
											   CDCA_U8        byLen, 
											   CDCA_U16       wPid, 
											   CDCA_U8        byWaitSeconds );


/* 释放私有数据过滤器 */
extern void CDSTBCA_ReleasePrivateDataFilter( CDCA_U8  byReqID,
                                              CDCA_U16 wPid );

/* 设置CW给解扰器 */
extern void CDSTBCA_ScrSetCW( CDCA_U16       wEcmPID,  
							  const CDCA_U8* pbyOddKey,  
							  const CDCA_U8* pbyEvenKey, 
							  CDCA_U8        byKeyLen, 
							  CDCA_BOOL      bTapingEnabled );


/*--------- 智能卡管理 ---------*/

/* 智能卡复位 */
extern CDCA_BOOL CDSTBCA_SCReset( CDCA_U8* pbyATR, CDCA_U8* pbyLen );

/* 智能卡通讯 */
extern CDCA_BOOL CDSTBCA_SCPBRun( const CDCA_U8* pbyCommand, 
								  CDCA_U16       wCommandLen,  
								  CDCA_U8*       pbyReply,  
								  CDCA_U16*      pwReplyLen  );


/*-------- 授权信息管理 -------*/

/* 通知授权变化 */
extern void CDSTBCA_EntitleChanged( CDCA_U16 wTvsID );


/* 反授权确认码通知 */
extern void CDSTBCA_DetitleReceived( CDCA_U8 bstatus );



/*-------- 安全控制 --------*/

/* 读取机顶盒唯一编号 */
extern void CDSTBCA_GetSTBID( CDCA_U16* pwPlatformID,
                              CDCA_U32* pdwUniqueID);

/* 安全芯片接口 */
extern CDCA_U16 CDSTBCA_SCFunction( CDCA_U8* pData);


/*-------- IPPV应用 -------*/

/* IPPV节目通知 */
extern void CDSTBCA_StartIppvBuyDlg( CDCA_U8                 byMessageType,
                                     CDCA_U16                wEcmPid,
                                     const SCDCAIppvBuyInfo* pIppvProgram  );

/* 隐藏IPPV对话框 */
extern void CDSTBCA_HideIPPVDlg(CDCA_U16 wEcmPid);


/*------- 邮件/OSD显示管理 -------*/

/* 邮件通知 */
extern void CDSTBCA_EmailNotifyIcon( CDCA_U8 byShow, CDCA_U32 dwEmailID );

/* 显示OSD信息 */
extern void CDSTBCA_ShowOSDMessage( CDCA_U8     byStyle,
                                    const char* szMessage );

/* 隐藏OSD信息*/
extern void CDSTBCA_HideOSDMessage( CDCA_U8 byStyle );



/*-------- 子母卡应用 --------*/

/* 请求提示读取喂养数据结果 */
extern void  CDSTBCA_RequestFeeding( CDCA_BOOL bReadStatus );



/*-------- 强制切换频道 --------*/

/* 频道锁定 */
extern void CDSTBCA_LockService( const SCDCALockService* pLockService );

/* 解除频道锁定 */
extern void CDSTBCA_UNLockService( void );


/*-------- 显示界面管理 --------*/

/* 不能正常收看节目的提示 */
/*wEcmPID==0表示与wEcmPID无关的消息，且不能被其他消息覆盖*/
extern void CDSTBCA_ShowBuyMessage( CDCA_U16 wEcmPID,
                                    CDCA_U8  byMessageType );

/* 指纹显示 */
extern void CDSTBCA_ShowFingerMessage( CDCA_U16 wEcmPID,
                                       CDCA_U32 dwCardID );


/* 安全窗帘显示*/


/* 进度显示 */
extern void CDSTBCA_ShowProgressStrip( CDCA_U8 byProgress,
                                       CDCA_U8 byMark );

/*--------- 机顶盒通知 --------*/

/* 机顶盒通知 */
extern void  CDSTBCA_ActionRequest( CDCA_U16 wTVSID,
                                    CDCA_U8  byActionType );


/*--------- 双向模块接口 --------*/


/*-------- 其它 --------*/

/* 获取字符串长度 */
extern CDCA_U16 CDSTBCA_Strlen(const char* pString );

/* 调试信息输出 */
extern void CDSTBCA_Printf(CDCA_U8 byLevel, const char* szMesssage );

/*---------------------------以上接口是STB提供给CA_LIB------------------------*/

#ifdef  __cplusplus
}
#endif
#endif
/*EOF*/

