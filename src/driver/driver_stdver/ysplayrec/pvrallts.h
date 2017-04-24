


typedef struct DRV_ParsePatTable_s
{
    U8 *PatTableBuffer;
    U8 *PatTableBufferHeader;
    U16 PatTableSize;
    DRVOS_Task_t *ParsePatTaskId;
    BOOL CurrentPatTableValid;
    BOOL ParsePatTaskRunStatus;
    BOOL TaskExitStatus;
}DRV_ParsePatTable_t;
typedef struct DRV_ParsePmtTable_s
{
    U8 *PmtTableBuffer;
    U8 *PmtTableBufferHeader;
    U16 PmtTableSize;
    DRVOS_Task_t *ParsePmtTaskId;
    BOOL CurrentPmtTableValid;
    BOOL ParsePmtTaskRunStatus;
    BOOL TaskExitStatus;
}DRV_ParsePmtTable_t;

typedef struct DRV_AssortCurrentPid_s
{
    DRVOS_Task_t *AssortPidTaskId;
    BOOL AssortPidFinishState;
    BOOL AssortPidTaskRunStatus;
    BOOL TaskExitStatus;
}DRV_AssortCurrentPid_t;  //AssortCurrentParsePidTask



typedef struct PAT_PmtPid_s
{
 unsigned short ProgramNum ; 
 unsigned short PmtPid ;   
}PAT_PmtPid_t;

typedef struct PAT_PatTableInfo_s
{ 
    unsigned char NetworkPidFlag;
    unsigned char ProgramMaxNum;
    unsigned char PatTableLength ;  
}PAT_PatTableInfo_t;

typedef struct TS_PmtTableInfo_s
{
     unsigned char StreamType  ;     
     unsigned short StreamPid ;     
     unsigned short EsInfoLength ;  
     unsigned char DescriptorTag ;   
     unsigned char DescriptorLength;  
}TS_PmtTableInfo_t;



typedef struct TS_PatTable_s
{
    unsigned table_id                     : 8; 
    unsigned section_syntax_indicator     : 1; 
    unsigned zero                         : 1; 
    unsigned reserved_1                   : 2; 
    unsigned section_length               : 12; 
    unsigned transport_stream_id          : 16; 
    unsigned reserved_2                   : 2;
    unsigned version_number               : 5; 
    unsigned current_next_indicator       : 1; 
    unsigned section_number               : 8;
    unsigned last_section_number          : 8; 
    unsigned reserved_3                   : 3; 
    unsigned network_PID                  : 13; 
    unsigned CRC_32                       : 32; 
} TS_PatTable_t; 

typedef struct TS_PmtTable_S 
{
    unsigned table_id                        : 8; 
    unsigned section_syntax_indicator        : 1; 
    unsigned zero                            : 1; 
    unsigned reserved_1                      : 2; 
    unsigned section_length                  : 12;
    unsigned program_number                  : 16;
    unsigned reserved_2                      : 2; 
    unsigned version_number                  : 5; 
    unsigned current_next_indicator          : 1; 
    unsigned section_number                  : 8; 
    unsigned last_section_number             : 8; 
    unsigned reserved_3                      : 3; 
    unsigned PCR_PID                         : 13;
    unsigned reserved_4                      : 4; 
    unsigned program_info_length             : 12;
    unsigned reserved_5                      : 3; 
    unsigned reserved_6                      : 4; 
    unsigned CRC_32                          : 32; 
}TS_PmtTable_t; 


typedef struct PMT_ProgramPid_s
{
    unsigned short  AudPid;
    unsigned short  VidPid;
    unsigned short  PcrPid;
    unsigned short  AudEcmPid;
    unsigned short  VidEcmPid;
    BOOL EncryptState;
#if 1
    U32 VideoKey;
    U32 AudeoKey;
    U32 AVKey;
#endif
}PMT_ProgramPid_t;

typedef struct DRV_ProgramPidCount_s
{
    U8   EncryptProgramNum;
    U8   NotEncryptProgramNum;
    U8   EncryptPidNum;
    U8   NotEncryptPidNum;
}DRV_ProgramPidCount_t;

typedef struct DRV_DescRawPacketTaskState_s
{
    BOOL DescRawPacketTaskRun;
    BOOL StartTsReceive;
    DRVOS_Task_t *DescRawPacketTaskId;   
}DRV_DescRawPacketTaskState_t;
typedef void (*DescDataCallBack)(unsigned char *pDescRawPacketData, unsigned short DescDataLength);


