#ifndef CM_CTRL_H
#define CM_CTRL_H

#include <string.h>

#define OS21_STB  0
#define Linux_STB  1

#define SNMP_PORT 161
#define CM_ENABLE_SCANNING     1
#define CM_DISABLE_SCANNING     2

#define CM_CMD_STOP_SCANNING     2
#define CM_CMD_START_SCANNING     1

#define CM_CMD_PASSWORD     3
#define CM_CMD_FACTORY        4
#define CM_CMD_RETRY_NUM    4
#define CM_CMD_DOWNLINKFREQUENCY_FOURBYTES   5
#define CM_CMD_DOWNLINKFREQUENCY_THREEBYTES  6
#define CM_CMD_GET_DS_CHANNEL 7
#define CM_CMD_DIAGNOSTIC  8
#define CM_CMD_PRIVATE_MIB_FACTORY 9
#define CM_CMD_TUNNER_INDEX  10
#define CM_CMD_DS_OFFSET_ZERO  11
#define CM_CMD_CAL_MODULATION  12
#define CM_CMD_LOCK_NOW             13
#define CM_CMD_CAL_QAM_LOCKED   14
#define CM_CMD_SNR                          15
#define CM_CMD_DOWNCHANNEL_POWER  16
#define CM_CMD_CAL_OFFSET_POWER    17
#define CM_CMD_DISABLE_FACTORY    18
#define CM_CMD_OPER_MODE     20
#define CM_CMD_COMMIT_SETTING      21
#define CM_CMD_CONFIGURE_SELECTED_FREQUENCY 22
#define CM_CMD_GET_OFFSET_SETTING  23

/*----------------------------------------------------------------------------*/
#define CM_CMD_GET_MACADDR  24
#define CM_CMD_PRIVATE_MIBENABLE 25

#define CM_CMD_GET_UP_FREQUENCY   27
#define CM_CMD_GET_UP_CHANNEL_ID   28
#define CM_CMD_GET_UP_STRENGTH   29
#define CM_CMD_GET_UP_SNR  30
#define CM_CMD_GET_UP_MODULATION 31



#define CM_CMD_GET_DOWN_FREQUENCY   41
#define CM_CMD_GET_DOWN_CHANNEL_ID   42
#define CM_CMD_GET_DOWN_STRENGTH   43
#define CM_CMD_GET_DOWN_SNR  44
#define CM_CMD_GET_DOWN_MODULATION 45
#define CM_CMD_GET_UP_STRENGHT   46

#define CM_CMD_GET_STAT_VALUE   50
#define CM_CMD_GET_STAT_CODE   51
#define CM_CMD_GET_STAT_RESET   52
#define CM_CMD_GET_STAT_LOST_SYNC   53
#define CM_CMD_GET_STAT_INVALID_MAP   54
#define CM_CMD_GET_STAT_INVALID_UCD   55
#define CM_CMD_GET_STAT_INVALID_RANG_RES   56
#define CM_CMD_GET_STAT_INVALID_REG_RES   57
#define CM_CMD_GET_STAT_T1_TIMEOUT   58
#define CM_CMD_GET_STAT_T2_TIMEOUT   59
#define CM_CMD_GET_STAT_T3_TIMEOUT   60
#define CM_CMD_GET_STAT_T4_TIMEOUT   61
#define CM_CMD_GET_STAT_RANG_ABORT   62
#define CM_CMD_GET_STAT_DOCSIS_OPER_MODE    63
#define CM_CMD_GET_STAT_MUDO_TYPE   64
#define CM_CMD_GET_STAT_EQ_DATA   65


/*----------------CM channel extend info define----------------------------*/
#define CM_CMD_GET_UP_WIDTH 70
#define CM_CMD_GET_UP_SLOT_SIZE 71
#define CM_CMD_GET_UP_TX_TIM_OFFSET 72
#define CM_CMD_GET_UP_RANG_BACK_START 73
#define CM_CMD_GET_UP_RANG_BACK_END 74
#define CM_CMD_GET_UP_TX_BACK_START 75
#define CM_CMD_GET_UP_TX_BACK_END 76
#define CM_CMD_GET_UP_SCDMA_ACT_CODE 77
#define CM_CMD_GET_UP_SCDMA_CODE_PER_SLOT 78
#define CM_CMD_GET_UP_SCDMA_FRAME_SIZE 79
#define CM_CMD_GET_UP_SCDMA_HOP_SEED 80
#define CM_CMD_GET_UP_CHANNEL_TYPE 81
#define CM_CMD_GET_UP_CLONE_FROM 82
#define CM_CMD_GET_UP_UPDATE 83
#define CM_CMD_GET_UP_STATUS 84
#define CM_CMD_GET_UP_PRE_EQ_ENABLE 85
#define CM_CMD_GET_DOWN_WIDTH 86
#define CM_CMD_GET_DOWN_INTER_LEAVE 87

/*----------------设置CM参数命令定义----------------------------*/
#define CM_CMD_SET_MACADDR  90 //设置CM的MAC地址
#define CM_MACADDR_OFFSET   52 //MAC地址字段偏移


/*---------------------------request id 偏移----------------------------------*/

#define CM_PASSWORD_OFFSET      19
#define CM_FACTORY_OFFSET         19
#define CM_SCANNING_OFFSET       19
#define CM_FREQUENCY_OFFSET     19
#define CM_LENGHT_OFFSET           1
#define CM_DSCHANNEL_OFFSET   52

#define CM_REQUEST_ID_OFFSET        19
#define CM_REQUEST_ID_EQDATA_OFFSET        21

/*------------response报文中绑定变量的结果字段偏移----------------------------*/
/*------一般绑定变量的结果字段偏移为48，一些特殊类型的变量偏移单独定义--------*/
#define CM_BIND_VALUE_OFFSET     48 //常用绑定变量的结果字段偏移

#define CM_UP_CHANNEL_ID_OFFSET     48
#define CM_UP_MODULATION_OFFSET     48
#define CM_UP_STRENTH_OFFSET     48

#define CM_DOWN_FREQUENCY_OFFSET    48
#define CM_DOWN_CHANNEL_ID_OFFSET     48
#define CM_DOWN_STRENTH_OFFSET     48
#define CM_DOWN_SNR_OFFSET     48

#define CM_STAT_VALUE_OFFSET    48
#define CM_STAT_CODE_OFFSET    48
#define CM_STAT_RESET_OFFSET    48
#define CM_STAT_LOST_SYNC_OFFSET    48
#define CM_STAT_INVALID_MAP_OFFSET    48
#define CM_STAT_INVALID_UCD_OFFSET    48
#define CM_STAT_INVALID_RING_RES_OFFSET    48
#define CM_STAT_INVALID_REG_RES_OFFSET    48
#define CM_STAT_T1TIMEOUT_OFFSET    48
#define CM_STAT_T2TIMEOUT_OFFSET    48
#define CM_STAT_T3TIMEOUT_OFFSET    48
#define CM_STAT_T4TIMEOUT_OFFSET    48
#define CM_STAT_RANG_ABORT_OFFSET    48
#define CM_STAT_DOC_OPER_MODE_OFFSET    48
#define CM_STAT_MODU_TYPE_OFFSET    48
#define CM_STAT_EQ_DATA_OFFSET    50
/*----------------------------------------------------------------------------*/

/*供构造snmp数据包使用，数据类型常用*/
#define SNMP_DATA_TYPE_INTEGER               0x02
#define SNMP_DATA_TYPE_OCTET_STRING     0x04
#define SNMP_DATA_TYPE_OBJECT_IDENTIFIER  0x06
#define SNMP_DATA_TYPE_NULL                           0x05
#define SNMP_DATA_TYPE_SEQUENCE                  0x30
#define SNMP_DATA_TYPE_IPADDRESS                0x40
#define SNMP_DATA_TYPE_COUNTER                    0x41
#define SNMP_DATA_TYPE_GAUGE                        0x42
#define SNMP_DATA_TYPE_TIME_TICKS               0x43
#define SNMP_DATA_TYPE_OPAQUE                      0x44
#define SNMP_GET_REQUEST                                0xa0
#define SNMP_SET_REQUEST                                 0xa3
#define SNMP_DATA_LEN_ONE_BYTE                     0x01

/*----------------------------------------------------------------------------*/

#define CM_SNMP_DEBUG 0
#define CM_SNMP_DEBUG_PREFIX    "CM-SNMP-DEBUG"
#define CM_MAC_LENGTH  6

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef short int16_t;
typedef int int32_t;

typedef struct _STB_CM_Status_info_t_  //CM状态信息
{
    int32_t  CmStatValue;  //docsIfCmStatusValue
    uint32_t CmStatResets; //docsIfCmStatusResets
    uint32_t CmStatLostSyncs;//docsIfCmStatusLostSyncs
    uint32_t CmStatInvMaps;//docsIfCmStatusInvalidMaps
    uint32_t CmStatInvUcds;//docsIfCmStatusInvalidUcds
    uint32_t CmStatInvRangRes; //docsIfCmStatusInvalidRangingResponses
    uint32_t CmStatInvRegRes; //docsIfCmStatusInvalidRegistrationResponses

    uint32_t CmStatT1Timeout; //docsIfCmStatusT1Timeouts
    uint32_t CmStatT2Timeout; //docsIfCmStatusT2Timeouts
    uint32_t CmStatT3Timeout; //docsIfCmStatusT3Timeouts
    uint32_t CmStatT4Timeout; //docsIfCmStatusT4Timeouts
    uint32_t CmStatRangAbr; //docsIfCmStatusRangingAborteds
    int32_t  CmStatDocOperMode; //docsIfCmStatusDocsisOperMode
    int32_t  CmStatModuType;    //docsIfCmStatusModulationType
    uint8_t  CmStatusEqData[101]; //docsIfCmStatusEqualizationData
    uint8_t  CmStatCode[10]; //docsIfCmStatusCode
    
#if _IS_ADD_ATTRIBUTE_
}__attribute__((packed)) STB_CM_Status_info_t, *pSTB_CM_Status_info_t;
#else 
} STB_CM_Status_info_t, *pSTB_CM_Status_info_t;
#endif

typedef struct _STB_CM_info_ext_t_  //CM扩展信息
{
    /*********UP***************/
    int32_t  UpWidth; //docsIfUpChannelWidth
    uint32_t UpSlotSize; //docsIfUpChannelSlotSize
    uint32_t UpTxTimOffset; //docsIfUpChannelTxTimingOffset
    int32_t  UpRangBackStart; //docsIfUpChannelRangingBackoffStart
    int32_t  UpRangBackEnd; //docsIfUpChannelRangingBackoffEnd
    int32_t  UpTxBackStart; //docsIfUpChannelTxBackoffStart
    int32_t  UpTxBackEnd; //docsIfUpChannelTxBackoffEnd
    uint32_t UpScdmaActCodes; //docsIfUpChannelScdmaActiveCodes
    int32_t  UpScdmaCodesPerSlot; //docsIfUpChannelScdmaCodesPerSlot
    uint32_t UpScdmaFraSize; //docsIfUpChannelScdmaFrameSize
    uint32_t UpScdmaHopSeed; //docsIfUpChannelScdmaHoppingSeed
    int32_t  UpType; //docsIfUpChannelType
    int32_t  UpCloFrom; //docsIfUpChannelCloneFrom
    int32_t  UpUpdate; //docsIfUpChannelUpdate
    int32_t  UpStatus; //docsIfUpChannelStatus
    int32_t  UpPreEqEnable; //docsIfUpChannelPreEqEnable

    /*********DOWN***************/
    int32_t  DownWidth; //docsIfDownChannelWidth
    int32_t  DownInterleave; //docsIfDownChannelInterleave      
#if _IS_ADD_ATTRIBUTE_
}__attribute__((packed)) STB_CM_info_ext_t, *pSTB_CM_info_ext_t;
#else 
} STB_CM_info_ext_t, *pSTB_CM_info_ext_t;
#endif

typedef struct _STB_CM_info_t_  //CM信号参数信息
{
    uint32_t cur_frequency; //当前频率
    int16_t cm_signal_strength; //接收电平
    uint16_t cm_modulation; //调制方式
    uint32_t cm_snr; //信噪比
    uint32_t  channel_id; //通道ID
#if _IS_ADD_ATTRIBUTE_
}__attribute__((packed)) STB_CM_info_t, *pSTB_CM_info_t;
#else 
} STB_CM_info_t, *pSTB_CM_info_t;
#endif

typedef struct _STB_CM_info_desc_t_     //CM描述信息
{
    uint8_t desc_tag; //Cable Modem参数信息描述子（AMEP_CM）
    uint8_t desc_len; //sizeof(STB_CM_info_desc_t) - 2
    //time_info_t time; //实际发生时间
    STB_CM_info_t cm_down; //CM 下行信号参数
    STB_CM_info_t cm_up; //CM 上行信号参数
    uint8_t cm_mac[CM_MAC_LENGTH];//CM 的MAC地址
    uint8_t send_mode; //发送模式,0-report sending,1-query sending,2-warning sending
    STB_CM_Status_info_t cm_status; //CM状态信息
    STB_CM_info_ext_t cm_extend;   //CM扩展信息
#if _IS_ADD_ATTRIBUTE_
}__attribute__((packed)) STB_CM_info_desc_t, *pSTB_CM_info_desc_t;
#else 
} STB_CM_info_desc_t, *pSTB_CM_info_desc_t;
#endif
 
/*END: add by whkuan*/

struct snmp_pdu
{
    /*
     * Protocol-version independent fields
     */
    long    version;
    int command;    /* Type of this PDU */
    long    reqid;  /* Request id - note: not incremented on retries */
    long    msgid;      /* Message id for V3 messages
                         * note: incremented for each retry */
    long    transid;    /* Unique ID for incoming transactions */
    long    sessid;     /* Session id for AgentX messages */
    long    errstat;    /* Error status (non_repeaters in GetBulk) */
    long    errindex;   /* Error index (max_repetitions in GetBulk) */

    //struct variable_list *variables;
};

struct variable
{
    //现在先写成单个值的形式，以后如果要扩展，则以指针的形式加进来
    int  value_type;
    int  value_len;
    char *value;
};
struct object_identifier
{
    int object_name_type;
    int object_name_len;
    char *object_name;
};

struct variable_binding_value
{
    //有可能有多个变量值进行绑定，此处仅绑定一个值
    int var_binding_type;
    int var_binding_len;
    struct object_identifier oid;
    struct variable var_list;
};
struct pdu_data_value
{
    int request_id_type;
    int request_id_len;
    int request_id_value;

    int errstat_type;
    int errstat_len;
    int errstat_value;

    int errindex_type;
    int errindex_len;
    int errindex_value;

    int variable_bindings_type;
    int variable_bindings_len;
    struct variable_binding_value p_var_binding;//绑定一个值


};
struct snmp_set_request_pdu
{
    int sequence;
    int  all_length;

    int  version;
    int version_type;
    int version_len;

    char *command;
    int command_type;
    int command_len;

    int pdu_data_type;
    int pdu_data_len;
    struct pdu_data_value p_pdu_data;
};

/*
int pdu_packet()
{

}
*/
struct snmp_get_request_pdu
{

};




int cm_stop_scanning();
int cm_restart_scanning();

int cm_set_DS_Channel(int DSFrequency);
int cm_get_DS_Channel();

int cm_Downstream_Calibration();
int cm_Get_DownStream_Offset();

/*-----------Get up channel information -----------------*/
extern uint32_t cm_Get_Up_ChannelId();
uint32_t cm_Get_Up_ChannelFrequency();
uint16_t cm_Get_Up_ChannelModulation();
float cm_Get_Up_ChannelStrenth();
int32_t cm_Get_Up_ChannelWidth();
int32_t cm_Get_Up_ChannelSlotSize();
uint32_t cm_Get_Up_ChannelTxTimOffset();
int32_t cm_Get_Up_ChannelRangBackStart();
int32_t cm_Get_Up_ChannelRangBackEnd();
int32_t cm_Get_Up_ChannelTxBackStart();
int32_t cm_Get_Up_ChannelTxBackEnd();
uint32_t cm_Get_Up_ChannelScdmaActCode();
int32_t cm_Get_Up_ChannelScdmaCodePerSlot();
uint32_t cm_Get_Up_ChannelScdmaFrameSize();
uint32_t cm_Get_Up_ChannelScdmaHopSeed();
int32_t cm_Get_Up_ChannelCloneFrom();
int32_t cm_Get_Up_ChannelUpdate();
int32_t cm_Get_Up_ChannelStatus();
int32_t cm_Get_Up_ChannelPreEqEnable();

/*--------------Get down channel information-----------------*/

uint32_t cm_Get_DownChannelId();
int32_t cm_Get_DownChannelFrequency();
int32_t cm_Get_DownChannelWidth();
int32_t cm_Get_DownChannelModulation();
float cm_Get_DownChannelStrenth();
float cm_Get_DownChannelSNR();
int32_t cm_Get_DownChannelInterLeave();

/*----------Get CM Status information-------------------------*/
int32_t cm_Get_StatusValue();
void cm_Get_StatusCode(uint8_t *);
uint32_t cm_Get_StatusReset();
uint32_t cm_Get_StatusLostSync();
uint32_t cm_Get_StatusInvalidMap();
uint32_t cm_Get_StatusInvalidUcd();
uint32_t cm_Get_StatusInvalidRangRes();
uint32_t cm_Get_StatusInvalidRegRes();
uint32_t cm_Get_StatusT1Timeout();
uint32_t cm_Get_StatusT2Timeout();
uint32_t cm_Get_StatusT3Timeout();
uint32_t cm_Get_StatusT4Timeout();
uint32_t cm_Get_StatusRangAbort();
int32_t cm_Get_StatusDocsisOperMode();
int32_t cm_Get_StatusModulationType();
void cm_Get_StatusEqualizationData(uint8_t * );
/*------------------------------------------------------------*/    

void cm_Get_MAC_Address(uint8_t *);

//struct STB_CM_info_t* cm_Get_Up_Info();
//struct STB_CM_info_t* cm_Get_Down_Info();

#endif



