/*****************************************************************************
*
* Description: 实现一个频点所有节目加密流录制成清流
*  
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2013-01-25    whyu       创建
*
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "drv_basic.h"
#include "ysfilter.h"
#include "yspti.h"
#include "ysostime.h"
#include "drv_os.h"
#include "stbtype.h"
#include <driver.h>
#include "pvrallts.h"
#include "dvb.h"
#include "CDCASa.h"
#include "dvbtablereceive.h"
#define MAX_USER_PRIORITY 255
#define TS_STD_PID_NUM    9
#define MAX_SHARE_CHANNEL_NUM DMX_TOTALCHAN_CNT
#define MAX_DESC_CHANNEL_NUM    16
#define MAX_STD_CHANNEL_NUM    34
#define HI_MAX_TS_NUM 512
#define USE_PRINTTABLE

DRVOS_Sem_t *aquarebufferlock;

BOOL mFinishPidParseState=FALSE;
FILE  *MuxTsFileHandle;
static U8 mDescChannelNum=0;
U32 pvrallts_start = 0;
U32 countstdPidNum = 0;


U32  g_AVKeyArrzy[MAX_DESC_CHANNEL_NUM]={0};   
STPTI_Pid_t  g_DescSlotHandle[MAX_DESC_CHANNEL_NUM]={0};
STPTI_Pid_t  g_StdSlotHandle[MAX_STD_CHANNEL_NUM]={0};

DRV_ParsePatTable_t ParsePatTable;
DRV_ParsePmtTable_t ParsePmtTable;
DRV_AssortCurrentPid_t AssortParsePid;

PAT_PmtPid_t PmtPidData[12];
PAT_PatTableInfo_t PatTableInfo;
PMT_ProgramPid_t ProgramPid[12];

STPTI_Pid_t mStdPid[9]={PAT_PID,CAT_PID,NIT_PID,SDT_PID,EIT_PID,RST_PID,TDT_PID,DIT_PID,SIT_PID};//需要应用传送过来PID的数组列表
STPTI_Pid_t mNotEncryptPid[34];
STPTI_Pid_t mEncryptPid[16];
PMT_ProgramPid_t g_ProgramKyePid[30];
DRV_ProgramPidCount_t g_ProgramPidCount;  
DRV_DescRawPacketTaskState_t DescRawPacketTaskState;

static void drv_MultiRawPacketReceiveMuxTask(void *pvParam)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    HI_UNF_DMX_DATA_S pDestination0[HI_MAX_TS_NUM];
    HI_UNF_DMX_DATA_S pDestination1[HI_MAX_TS_NUM];
	HI_U32 j,k,kk,mm;
	HI_U32 pu32AcquiredNum0;
	HI_U32 pu32AcquiredNum1;
    
    DescRawPacketTaskState.DescRawPacketTaskRun=TRUE;

    while(!DescRawPacketTaskState.StartTsReceive)
    {
        DRV_OS_TaskDelay_MS(150);
    }
    
    while (DescRawPacketTaskState.DescRawPacketTaskRun)
    {
        if(!DescRawPacketTaskState.StartTsReceive)
        {
            printf("@@@@@@@@@@@record TS is close!!!!!!!!!!\n\n");
            break;
        }

#if 1//接收不加密的数据，包括标准表和PMT等
        for(j = 0; j < countstdPidNum; j++)
        {
            pu32AcquiredNum0 = 0;
            ErrCode = HI_UNF_DMX_AcquireBuf(g_StdSlotHandle[j], HI_MAX_TS_NUM, &pu32AcquiredNum0, pDestination0, 15);
            if (ErrCode != HI_SUCCESS)
            { 
                usleep(10);
                //printf("----0000----HI_UNF_DMX_AcquireBuf error ErrCode = %x  \n",ErrCode);
                HI_UNF_DMX_ReleaseBuf(g_StdSlotHandle[j], pu32AcquiredNum0, pDestination0);
                continue;
            }
            
            //printf("----------pu32AcquiredNum0 = %d,    \n", pu32AcquiredNum0);
            for(kk=0; kk <pu32AcquiredNum0; kk++)
            {
                fwrite(pDestination0[kk].pu8Data,1,pDestination0[kk].u32Size,MuxTsFileHandle);
                //DRV_VfsWriteFile(MuxTsFileHandle,pDestination0[kk].pu8Data,pDestination0[kk].u32Size)
            }
          
            ErrCode = HI_UNF_DMX_ReleaseBuf(g_StdSlotHandle[j], pu32AcquiredNum0, pDestination0);

        }
#endif
#if 1//接收两个通道的加密音视就是一个节目
        for(k = 0; k < 2; k++)
        {
            pu32AcquiredNum1 = 0;
            ErrCode = HI_UNF_DMX_AcquireBuf(g_DescSlotHandle[k], HI_MAX_TS_NUM, &pu32AcquiredNum1, pDestination1, 15);
            if (ErrCode != HI_SUCCESS)
            { 
                usleep(10);
                //printf("----1111----HI_UNF_DMX_AcquireBuf error ErrCode = %x  \n",ErrCode);
                HI_UNF_DMX_ReleaseBuf(g_DescSlotHandle[k], pu32AcquiredNum1, pDestination1);
                continue;
            }
            
            //printf("----------pu32AcquiredNum1 = %d,    \n", pu32AcquiredNum1);
            for(mm=0; mm <pu32AcquiredNum1; mm++)
            {
                fwrite(pDestination1[mm].pu8Data,1,pDestination1[mm].u32Size,MuxTsFileHandle);
                //DRV_VfsWriteFile(MuxTsFileHandle,pDestination0[mm].pu8Data,pDestination0[mm].u32Size)
            }
          
            ErrCode = HI_UNF_DMX_ReleaseBuf(g_DescSlotHandle[k], pu32AcquiredNum1, pDestination1);

        }
#endif

    }
}

DRV_ErrCode drv_DescTsPacketBasicInit(void)  
{
    DRV_ErrCode ErrCode;
    memset(g_DescSlotHandle,0,MAX_DESC_CHANNEL_NUM);
    memset(g_AVKeyArrzy,0,MAX_DESC_CHANNEL_NUM);
    memset(g_StdSlotHandle,0,MAX_STD_CHANNEL_NUM);

    DescRawPacketTaskState.DescRawPacketTaskId = DRV_OS_TaskCreate("descRawPacket",drv_MultiRawPacketReceiveMuxTask,(MAX_USER_PRIORITY - 10),1024*1024,NULL,0);
    if(DescRawPacketTaskState.DescRawPacketTaskId==NULL)
    {
        STTBX_Print(("%s,%d:  Unable to create desc_raw_packet_task\n", __FUNCTION__, __LINE__));
		return 1;
    }
    return ErrCode;
}

void AssortPidByIsNotEncrypt(PMT_ProgramPid_t *ProgramPid, 
                                       U8 ProgNum,
                                       DRV_ProgramPidCount_t *ProgramDescrip,
                                       PMT_ProgramPid_t *ProgramKyePid_p,
                                       STPTI_Pid_t *NotEncryptPid)
{
       U8 i=0;
       U8 j=0;
       U8 Cnt=0;
       U8 PcrCnt=0; 
       U8 PcrCntIndex=0;
       unsigned short  szPcrPid[ProgNum];
       DRV_ProgramPidCount_t ProgramDespInfo;
       memset(&ProgramDespInfo,0,sizeof(DRV_ProgramPidCount_t));
       for(i=0;i<ProgNum;i++)
       {
           if((ProgramPid[i].AudEcmPid!=8191)||(ProgramPid[i].VidEcmPid!=8191))
           {
               ProgramPid[i].EncryptState=TRUE;
               ProgramDespInfo.EncryptProgramNum++;
               {
                   if(ProgramPid[i].AudEcmPid!=8191)
                   {
                       ProgramDespInfo.EncryptPidNum+=1;
                       ProgramKyePid_p[j].AudPid=ProgramPid[i].AudPid;
                       ProgramKyePid_p[j].AudEcmPid=ProgramPid[i].AudEcmPid;
                   }
                   else
                   {
                       ProgramKyePid_p[j].AudEcmPid=8191;
                       ProgramKyePid_p[j].AudPid=8191;
                       NotEncryptPid[Cnt++]=ProgramPid[i].AudPid;
                       ProgramDespInfo.NotEncryptPidNum+=1;
                   }
                   if(ProgramPid[i].VidEcmPid!=8191)
                   {
                       ProgramDespInfo.EncryptPidNum+=1;
                       ProgramKyePid_p[j].VidPid=ProgramPid[i].VidPid;
                       ProgramKyePid_p[j].VidEcmPid=ProgramPid[i].VidEcmPid;
                   }
                   else
                   {
                       ProgramKyePid_p[j].VidEcmPid=8191;
                       ProgramKyePid_p[j].VidPid=8191;
                       NotEncryptPid[Cnt++]=ProgramPid[i].VidPid;
                       ProgramDespInfo.NotEncryptPidNum+=1;
                   }
                   j++;
               }
           }
           else
           {
               ProgramPid[i].EncryptState=FALSE;
               ProgramDespInfo.NotEncryptProgramNum++;
               ProgramDespInfo.NotEncryptPidNum+=2;
               NotEncryptPid[Cnt++]=ProgramPid[i].AudPid;
               NotEncryptPid[Cnt++]=ProgramPid[i].VidPid;
           }
           
       }
   
       for(i=0;i<PatTableInfo.ProgramMaxNum;i++)
       {
           NotEncryptPid[Cnt++]=PmtPidData[i].PmtPid;
       }
       ProgramDespInfo.NotEncryptPidNum+=PatTableInfo.ProgramMaxNum;
       for(i=0;i<TS_STD_PID_NUM;i++)
       {
           NotEncryptPid[Cnt++]=mStdPid[i];
       }
       ProgramDespInfo.NotEncryptPidNum+=TS_STD_PID_NUM;
   
       memcpy(ProgramDescrip, &ProgramDespInfo,sizeof(DRV_ProgramPidCount_t));
}

   void drv_AssortCurrentParsePidTask(void *args)
{
       U8 pidcnt=0;
       AssortParsePid.AssortPidTaskRunStatus=TRUE;
       AssortParsePid.AssortPidFinishState=FALSE;
       while(AssortParsePid.AssortPidTaskRunStatus)
       {
           if(mFinishPidParseState==TRUE)
           {
               AssortPidByIsNotEncrypt(ProgramPid,PatTableInfo.ProgramMaxNum,&g_ProgramPidCount,g_ProgramKyePid,mNotEncryptPid);
#ifdef USE_PRINTTABLE
               STTBX_Print(("ProgramDescrip.EncryptProgramNum     = 0x%x \n",g_ProgramPidCount.EncryptProgramNum));
               STTBX_Print(("ProgramDescrip.EncryptPidNum         = 0x%x \n",g_ProgramPidCount.EncryptPidNum));
               STTBX_Print(("ProgramDescrip.NotEncryptProgramNum  = 0x%x \n",g_ProgramPidCount.NotEncryptProgramNum));
               STTBX_Print(("ProgramDescrip.NotEncryptPidNum      = 0x%x \n",g_ProgramPidCount.NotEncryptPidNum));  
               for(pidcnt=0;pidcnt<g_ProgramPidCount.EncryptProgramNum;pidcnt++)
               {
                   STTBX_Print(("Program is [%d]     \n ",pidcnt));
                   STTBX_Print(("g_ProgramKyePid.AudPid     = 0x%x \n",g_ProgramKyePid[pidcnt].AudPid));
                   STTBX_Print(("g_ProgramKyePid.VidPid     = 0x%x \n",g_ProgramKyePid[pidcnt].VidPid));
                   STTBX_Print(("g_ProgramKyePid.AudEcmPid  = 0x%x \n",g_ProgramKyePid[pidcnt].AudEcmPid));
                   STTBX_Print(("g_ProgramKyePid.VidEcmPid  = 0x%x \n",g_ProgramKyePid[pidcnt].VidEcmPid));
               }
               for(pidcnt=0;pidcnt<g_ProgramPidCount.NotEncryptPidNum;pidcnt++)
               {
                   STTBX_Print(("pidcnt=%d,mNotEncryptPid=0x%x \n",pidcnt,mNotEncryptPid[pidcnt]));
               }
#endif               
               mFinishPidParseState=FALSE;
               AssortParsePid.AssortPidFinishState=TRUE;
               
           }
           else
           {
               DRV_OS_TaskDelay_MS(100);
               continue;
           }
       }     
}

   S32 AdjustPMTTable( TS_PmtTable_t *PmtTable, 
                                  unsigned char *TableBuffer ,
                                  PMT_ProgramPid_t   *ProgramPid)
{ 
      unsigned char CaIdStart=0;
      unsigned char CaIdStartCnt=0;
      //unsigned short CurrsStreamPID=0;
      unsigned short CA_PID=8191;
      unsigned short Private_PID=8191;
      unsigned int CA_SYS_ID=0;
      TS_PmtTableInfo_t PmtTableInfo;
      U8 PES_ECM_STATE=0;
      U8 ES_ECM_STATE=0;
      ProgramPid->AudEcmPid=8191;
      ProgramPid->VidEcmPid=8191;
      
      PmtTable->table_id                            = TableBuffer[0];
      PmtTable->section_syntax_indicator            = TableBuffer[1] >> 7;
      PmtTable->zero                                = TableBuffer[1] >> 6 & 0x01; 
      PmtTable->reserved_1                          = TableBuffer[1] >> 4 & 0x03;
      PmtTable->section_length                      = (TableBuffer[1] & 0x0F) << 8 | TableBuffer[2];    
      PmtTable->program_number                      = TableBuffer[3] << 8 | TableBuffer[4];
      PmtTable->reserved_2                          = TableBuffer[5] >> 6;
      PmtTable->version_number                      = TableBuffer[5] >> 1 & 0x1F;
      PmtTable->current_next_indicator              = (TableBuffer[5] << 7) >> 7;
      PmtTable->section_number                      = TableBuffer[6];
      PmtTable->last_section_number                 = TableBuffer[7];
      PmtTable->reserved_3                          = TableBuffer[8] >> 5;
      PmtTable->PCR_PID                             = ((TableBuffer[8] << 8) | TableBuffer[9]) & 0x1FFF;
      PmtTable->reserved_4                          = TableBuffer[10] >> 4;
      PmtTable->program_info_length                 = (TableBuffer[10] & 0x0F) << 8 | TableBuffer[11]; 
      // Get CRC_32
      int len = 0;
      len = PmtTable->section_length + 3;    
      PmtTable->CRC_32     = (TableBuffer[len-4] & 0x000000FF) << 24
      | (TableBuffer[len-3] & 0x000000FF) << 16
      | (TableBuffer[len-2] & 0x000000FF) << 8
      | (TableBuffer[len-1] & 0x000000FF); 
      ProgramPid->PcrPid= PmtTable->PCR_PID;
      U16 pos_mov = 12;
  
      if ( PmtTable->program_info_length != 0 )
      {
          for( ;pos_mov < (12+PmtTable->program_info_length); )//22
          {
              PmtTableInfo.DescriptorTag=TableBuffer[pos_mov];
              PmtTableInfo.DescriptorLength=TableBuffer[pos_mov+1];
              if(PmtTableInfo.DescriptorTag==0x09)
              {
                 pos_mov+=2;
                 CA_SYS_ID=TableBuffer[pos_mov] << 8 | TableBuffer[pos_mov+1]; 
                 pos_mov+=2;
                 CA_PID=(TableBuffer[pos_mov]&0x1F) << 8 | TableBuffer[pos_mov+1];
                 ProgramPid->AudEcmPid=CA_PID;
                 ProgramPid->VidEcmPid=CA_PID;
                 pos_mov+=2;
                 CaIdStart=0;
                 for(;CaIdStart<(PmtTableInfo.DescriptorLength-4);) 
                 {
                     pos_mov+=1;
                     CaIdStart+=1;
                 }
                 PES_ECM_STATE=1;
                 ES_ECM_STATE=0;
              }
              else
              {
                  pos_mov += (PmtTableInfo.DescriptorLength+2);
                  if(PES_ECM_STATE==0)
                  {
                      ProgramPid->AudEcmPid=8191;
                      ProgramPid->VidEcmPid=8191;
                  }
              }
          }
      }
      else
      {
         ProgramPid->AudEcmPid=8191;
         ProgramPid->VidEcmPid=8191;
      }
      // Get stream type and PID    
      for( ; pos_mov <= (PmtTable->section_length + 2 ) -  4; )
      {
          static U8 i=0;
          PmtTableInfo.StreamType = TableBuffer[pos_mov];
          PmtTable->reserved_5  =   TableBuffer[pos_mov+1] >> 5;
          PmtTableInfo.StreamPid = ((TableBuffer[pos_mov+1] << 8) | TableBuffer[pos_mov+2]) & 0x1FFF;
  
        
          switch(PmtTableInfo.StreamType)
          {
              case 0x01://
              case 0x02://
              case 0x1b://
                   ProgramPid->VidPid=PmtTableInfo.StreamPid;
              break;
              case 0x03://
              case 0x04://
              case 0x06://  
              case 0x0F://
              case 0x11://
              case 0x81://
              case 0x86://
              case 0x8A://
                   ProgramPid->AudPid=PmtTableInfo.StreamPid;
              break;
              default:
                  Private_PID=PmtTableInfo.StreamPid;
                  STTBX_Print(("This is a private PID[%d]\n",Private_PID));
              break;
          }
          i++;
          PmtTable->reserved_6 = TableBuffer[pos_mov+3] >> 4;
          PmtTableInfo.EsInfoLength = (TableBuffer[pos_mov+3] & 0x0F) << 8 | TableBuffer[pos_mov+4];
  
          if (PmtTableInfo.EsInfoLength != 0)
          {
              pos_mov += 5;
              CaIdStartCnt=pos_mov;
              for( ;pos_mov < (CaIdStartCnt+PmtTableInfo.EsInfoLength); )//16
              {
                  PmtTableInfo.DescriptorTag=TableBuffer[pos_mov];
                  PmtTableInfo.DescriptorLength=TableBuffer[pos_mov+1];
                  if(PmtTableInfo.DescriptorTag==0x09)
                  {
                     pos_mov+=2;
                     CA_SYS_ID=TableBuffer[pos_mov] << 8 | TableBuffer[pos_mov+1]; 
                     pos_mov+=2;
                     switch(PmtTableInfo.StreamType)
                     {
                         case 0x01://
                         case 0x02://
                         case 0x1b://
                              ProgramPid->VidEcmPid=(TableBuffer[pos_mov]&0x1F) << 8 | TableBuffer[pos_mov+1];
                         break;
                         case 0x03://
                         case 0x04://
                         case 0x06:// 
                         case 0x0F://
                         case 0x11://
                         case 0x81:// 
                         case 0x86://
                         case 0x8A://
                              ProgramPid->AudEcmPid=(TableBuffer[pos_mov]&0x1F) << 8 | TableBuffer[pos_mov+1];
                         break;
                         default:
                             Private_PID=(TableBuffer[pos_mov]&0x1F) << 8 | TableBuffer[pos_mov+1];
                             STTBX_Print(("This is a private PID[%d]\n",Private_PID));
                         break;
                     }
                     pos_mov+=2;
                     CaIdStart=0;
                     for(;CaIdStart<(PmtTableInfo.DescriptorLength-4);) 
                     {
                         pos_mov+=1;
                         CaIdStart+=1;
                     }
                     PES_ECM_STATE=0;
                     ES_ECM_STATE=1;
                  }
                  else
                  {
                      pos_mov += (PmtTableInfo.DescriptorLength+2);
                  }
              }
          }
          else
          {
              pos_mov += 5;
          }
          if(pos_mov > (PmtTable->section_length + 2 ) -  4)
          {
              STTBX_Print(("PmtTable.elem_pid_num  =  %d \n", i));
              i = 0;
          }
      }
   return 0;
}

  S32 ParsePMTTable( unsigned char *SectionBuffer ,
                                 unsigned int SectionLength ,
                                 PMT_ProgramPid_t *ProgramPid)
{
     DRV_ErrCode ST_ErrorCode;
     TS_PmtTable_t *TSPMT_TableData_p;
     static U8  *pPmtSecBuffer = NULL;
     U16 PmtTableLength=0;
     pPmtSecBuffer = DRV_OSMalloc(SystemPartition, SectionLength + 16); 
     memcpy(pPmtSecBuffer, SectionBuffer, SectionLength );
     
     PmtTableLength= ((pPmtSecBuffer[1] & 0x0F) << 8 |  pPmtSecBuffer[2])+ 3; 
     STTBX_Print(("SectionSize                 = 0x%x \n",SectionLength));
     STTBX_Print(("PmtTable.pmt_data_length    = 0x%x \n",PmtTableLength));
     
     TSPMT_TableData_p = DRV_OSMalloc(SystemPartition, sizeof(TSPMT_TableData_p));           
     AdjustPMTTable(TSPMT_TableData_p,pPmtSecBuffer,ProgramPid);
     U8 ProgramPidCnt = 0;
     
#ifdef USE_PRINTTABLE
     STTBX_Print(("ts_pmt_table->table_id                 = 0x%x \n",TSPMT_TableData_p->table_id));
     STTBX_Print(("ts_pmt_table->current_next_indicator   = 0x%x \n",TSPMT_TableData_p->current_next_indicator));
     STTBX_Print(("ts_pmt_table->section_length           = 0x%x \n",TSPMT_TableData_p->section_length));
     STTBX_Print(("ts_pmt_table->program_number           = 0x%x \n",TSPMT_TableData_p->program_number));
     STTBX_Print(("ts_pmt_table->version_number           = 0x%x \n",TSPMT_TableData_p->version_number));
     STTBX_Print(("ts_pmt_table->current_next_indicator   = 0x%x \n",TSPMT_TableData_p->current_next_indicator));
     STTBX_Print(("ts_pmt_table->section_number           = 0x%x \n",TSPMT_TableData_p->section_number));
     STTBX_Print(("ts_pmt_table->last_section_number      = 0x%x \n",TSPMT_TableData_p->last_section_number));
     STTBX_Print(("ts_pmt_table->PCR_PID                  = 0x%x \n",TSPMT_TableData_p->PCR_PID));
     STTBX_Print(("ts_pmt_table->program_info_length      = 0x%x \n",TSPMT_TableData_p->program_info_length));
     STTBX_Print(("ts_pmt_table->CRC_32                   = 0x%x \n",TSPMT_TableData_p->CRC_32));
     for(ProgramPidCnt = 0;ProgramPidCnt < 1;ProgramPidCnt++)
     {
        STTBX_Print(("ProgramPid->AudPid    = 0x%x \n",ProgramPid->AudPid)); 
        STTBX_Print(("ProgramPid->VidPid    = 0x%x \n",ProgramPid->VidPid)); 
        STTBX_Print(("ProgramPid->AudEcmPid = 0x%x \n",ProgramPid->AudEcmPid)); 
        STTBX_Print(("ProgramPid->VidEcmPid = 0x%x \n",ProgramPid->VidEcmPid)); 
        STTBX_Print(("ProgramPid->PcrPid    = 0x%x \n",ProgramPid->PcrPid)); 
     }
#endif     
     DRV_OSFree( SystemPartition, TSPMT_TableData_p ); 
     DRV_OSFree( SystemPartition, pPmtSecBuffer );
     return 0;
}

void drv_ParsePmtTask(void *args)
{
     ParsePmtTable.ParsePmtTaskRunStatus=TRUE;
     U8 PmtCnt=0;
     while(ParsePmtTable.ParsePmtTaskRunStatus)
     {
         if(ParsePmtTable.CurrentPmtTableValid)
         {
             ParsePMTTable(ParsePmtTable.PmtTableBuffer,ParsePmtTable.PmtTableSize,&ProgramPid[PmtCnt++]);
             if(PmtCnt==PatTableInfo.ProgramMaxNum)
             {
                 PmtCnt=0;
                 mFinishPidParseState=TRUE;
             }   
             ParsePmtTable.CurrentPmtTableValid=FALSE;
             //break;
             
             printf("%s:%s\n", __FILE__, __func__);
         }
         else
         {
             DRV_OS_TaskDelay_MS(100);
             continue;
         }
     }
}

 S32 AdjustPATTable ( TS_PatTable_t *PatTable, 
                                unsigned char *TableBuffer ,
                                PAT_PatTableInfo_t *PatTableInfo, 
                                PAT_PmtPid_t *PmtPidData)
{
    PatTable->table_id                    = TableBuffer[0];
    PatTable->section_syntax_indicator    = TableBuffer[1] >> 7;
    PatTable->zero                        = TableBuffer[1] >> 6 & 0x1;
    PatTable->reserved_1                  = TableBuffer[1] >> 4 & 0x3;
    PatTable->section_length              =(TableBuffer[1] & 0x0F) << 8 |  TableBuffer[2]; 
    PatTable->transport_stream_id         = TableBuffer[3] << 8 | TableBuffer[4];
    PatTable->reserved_2                  = TableBuffer[5] >> 6;
    PatTable->version_number              = TableBuffer[5] >> 1 &  0x1F;
    PatTable->current_next_indicator      =(TableBuffer[5] << 7) >> 7;
    PatTable->section_number              = TableBuffer[6];
    PatTable->last_section_number         = TableBuffer[7]; 

    int len = 0;
    len = 3 + PatTable->section_length;
    PatTable->CRC_32 = (TableBuffer[len-4] & 0x000000FF) << 24
    | (TableBuffer[len-3] & 0x000000FF) << 16
    | (TableBuffer[len-2] & 0x000000FF) << 8 
    | (TableBuffer[len-1] & 0x000000FF); 
    PatTableInfo->NetworkPidFlag=0;
    PatTable->network_PID = 8191;
    int n = 0;
    for( n = 0; n < PatTable->section_length - 12; n += 4 )
    {
        static U8 i=0;
        static U8 j=0;
        unsigned short  program_num;
        program_num = TableBuffer[8 + n ] << 8 | TableBuffer[9 + n ];  
        PatTable->reserved_3  = TableBuffer[10 + n ] >> 5; 
        if( program_num == 0x00)
        {  
            PatTable->network_PID = (TableBuffer[10 + n ] & 0x1F) << 8 | TableBuffer[11 + n ];
            PatTableInfo->NetworkPidFlag = 1;    
            STTBX_Print(("PatTable->network_PID %0x \n\n", PatTable->network_PID));
        }
        else
        {  
            PmtPidData[i].ProgramNum  =  program_num; 
            PmtPidData[i].PmtPid = (TableBuffer[10 + n] & 0x1F) << 8 | TableBuffer[11 + n];
            i++; j=n;
            if((j+4)>(PatTable->section_length - 12))
            {
                PatTableInfo->ProgramMaxNum = i;
                STTBX_Print(("pat_info.program_num_max =  %d \n",PatTableInfo->ProgramMaxNum));
                i=0;
            }
        }         
    }
    return 0;
}


S32 ParsePATTable( unsigned char *DestBuffer ,
                               unsigned int SectionSize,
                               PAT_PatTableInfo_t *PatTableInfo,
                               PAT_PmtPid_t *PmtPidData )
{

    DRV_ErrCode ST_ErrorCode;
    DRV_Handle  ts_file_handle;
    U8 i;
    static U8  *pPatSecBuffer = NULL;
    pPatSecBuffer = DRV_OSMalloc(SystemPartition, SectionSize + 16); 
    memcpy(pPatSecBuffer, DestBuffer, SectionSize );

    PatTableInfo->PatTableLength = ((pPatSecBuffer[1] & 0x0F) << 8 |  pPatSecBuffer[2])+ 3;   

    STTBX_Print(("SectionSize               = 0x%x \n",SectionSize));
    STTBX_Print(("pat_info.pat_data_length  = 0x%x \n",PatTableInfo->PatTableLength));

    TS_PatTable_t *TSPAT_TableData_p;
    TSPAT_TableData_p = DRV_OSMalloc(SystemPartition, sizeof(TSPAT_TableData_p));
    
    AdjustPATTable(TSPAT_TableData_p,pPatSecBuffer,PatTableInfo,PmtPidData); 
#ifdef USE_PRINTTABLE
    STTBX_Print(("ts_pat_table->table_id                 = 0x%x \n",TSPAT_TableData_p->table_id));
    STTBX_Print(("ts_pat_table->section_syntax_indicator = 0x%x \n",TSPAT_TableData_p->section_syntax_indicator));
    STTBX_Print(("ts_pat_table->section_length           = 0x%x \n",TSPAT_TableData_p->section_length));
    STTBX_Print(("ts_pat_table->transport_stream_id      = 0x%x \n",TSPAT_TableData_p->transport_stream_id));
    STTBX_Print(("ts_pat_table->version_number           = 0x%x \n",TSPAT_TableData_p->version_number));
    STTBX_Print(("ts_pat_table->current_next_indicator   = 0x%x \n",TSPAT_TableData_p->current_next_indicator));
    STTBX_Print(("ts_pat_table->section_number           = 0x%x \n",TSPAT_TableData_p->section_number));
    STTBX_Print(("ts_pat_table->last_section_number      = 0x%x \n",TSPAT_TableData_p->last_section_number));
    STTBX_Print(("ts_pat_table->CRC_32                   = 0x%x \n",TSPAT_TableData_p->CRC_32));                              
    if(PatTableInfo->NetworkPidFlag== 1)
    {
        STTBX_Print(("ts_pat_packet->network_PID = %x \n",TSPAT_TableData_p->network_PID));
    }
    for(i=0;i<PatTableInfo->ProgramMaxNum;i++)
    {
        STTBX_Print(("pat_program_data[%d].program_number  = %d \n",i,PmtPidData[i].ProgramNum));
        STTBX_Print(("pat_program_data[%d].program_map_PID = 0x%x\n\n",i,PmtPidData[i].PmtPid));
    }
#endif    
    DRV_OSFree( SystemPartition, TSPAT_TableData_p );
    DRV_OSFree( SystemPartition, pPatSecBuffer );
    return 0;
}

void PMT_TableDataCallBack(int FilterNo, BOOLEAN isTimeOut, unsigned char *pSecData, unsigned short SecLength)
{
    static U8 PmtTableCnt=0;
    if(isTimeOut)	
	{
		STTBX_Print(("[Test] TestFilterCallback: section filter Timeout\n"));
	}
    else
    {
        if(ParsePmtTable.CurrentPmtTableValid==FALSE)
        {
            memcpy(ParsePmtTable.PmtTableBuffer, pSecData, SecLength );
            ParsePmtTable.PmtTableSize=SecLength;
            ParsePmtTable.CurrentPmtTableValid=TRUE;
            PmtTableCnt++;
            DRV_FILTFreeFilterReq(FilterNo);
            STTBX_Print(("#####TableDataCallBack->PmtTableCnt[%d]\n",PmtTableCnt));
        }
    }
}

void drv_ParsePatTask(void *args)
{
    U8 PmtCnt=0;
    int PmtSlotID[10]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    ParsePatTable.ParsePatTaskRunStatus=TRUE;
    while(ParsePatTable.ParsePatTaskRunStatus)
    {
        if(ParsePatTable.CurrentPatTableValid)
        {
            ParsePATTable(ParsePatTable.PatTableBuffer,ParsePatTable.PatTableSize,&PatTableInfo,PmtPidData);
            ParsePatTable.CurrentPatTableValid=FALSE;
            ParsePmtTable.CurrentPmtTableValid=FALSE;
            for(PmtCnt=0;PmtCnt<PatTableInfo.ProgramMaxNum;PmtCnt++)
            {
                PmtSlotID[PmtCnt]=DRV_FILTReqFilterForStandard( NULL, PMT_TableDataCallBack, ONE_KILO_SECTION,
                                                           PmtPidData[PmtCnt].PmtPid, 
                                                           PMT_TABLE_ID, 
                                                           -1, 
                                                           0, 
                                                           -1, 
                                                           10000, 
                                                           -1, 
                                                           -1 );
                if(PmtSlotID[PmtCnt] <0)
                {
                    STTBX_Print(("%s,%d: *DRV_FILTReqFilterForStandard Failed!* PmtPid[%d],PmtCnt[%d]\n", __FUNCTION__, __LINE__,PmtPidData[PmtCnt].PmtPid,PmtCnt));
                } 
            }
            
        }
        else
        {
            DRV_OS_TaskDelay_MS(100);
            continue;
        }
    }
}

DRV_ErrCode drv_PIDParseConfigInit(void)
{

    DRV_ErrCode ErrCode=DRV_NO_ERROR;
    DRVOS_Task_t *ParsePatTaskId;
    DRVOS_Task_t *ParsePmtTaskId;
    DRVOS_Task_t *AssortPidTaskId;
    ParsePatTable.CurrentPatTableValid=FALSE;
    ParsePmtTable.CurrentPmtTableValid=FALSE; 
    AssortParsePid.AssortPidFinishState=FALSE;
    ParsePatTable.PatTableBuffer=DRV_OSMalloc(SystemPartition,4*1024);
	if(ParsePatTable.PatTableBuffer== NULL)
    {
       STTBX_Print(("%s,%d:  Failed!\n", __FUNCTION__, __LINE__));
	}	
    ParsePatTable.PatTableBufferHeader=ParsePatTable.PatTableBuffer;
    memset(ParsePatTable.PatTableBuffer,0,4*1024);

    ParsePmtTable.PmtTableBuffer=DRV_OSMalloc(SystemPartition,4*1024);
	if(ParsePmtTable.PmtTableBuffer== NULL)
    {
       STTBX_Print(("%s,%d:  Failed!\n", __FUNCTION__, __LINE__));
	}	
    ParsePmtTable.PmtTableBufferHeader=ParsePmtTable.PmtTableBuffer;
    memset(ParsePmtTable.PmtTableBuffer,0,4*1024);

    ParsePatTaskId= DRV_OS_TaskCreate("ParsePatTask",drv_ParsePatTask,(MAX_USER_PRIORITY - 15),128*1024,NULL,0);
    if(ParsePatTaskId==NULL)
    { 
        STTBX_Print(("%s,%d:  Unable to create ParsePatTask!!!\n", __FUNCTION__, __LINE__));
		return 1;
    }
    ParsePmtTaskId= DRV_OS_TaskCreate("ParsePmtTask",drv_ParsePmtTask,(MAX_USER_PRIORITY - 15),128*1024,NULL,0);
    if(ParsePmtTaskId==NULL)
    {
        STTBX_Print(("%s,%d:  Unable to create ParsePmtTask!!!\n", __FUNCTION__, __LINE__));
		return 1;
    }
     AssortPidTaskId= DRV_OS_TaskCreate("AssortPidTask",drv_AssortCurrentParsePidTask,(MAX_USER_PRIORITY - 15),128*1024,NULL,0);
    if(AssortPidTaskId==NULL)
    {
        STTBX_Print(("%s,%d:  Unable to create AssortPidTask!!\n", __FUNCTION__, __LINE__));
		return 1;
    }
    return ErrCode;
}

DRV_ErrCode DRV_DescPidParseSysConfigInit(void)
{
    DRV_ErrCode ErrCode=DRV_NO_ERROR;
    ErrCode=drv_PIDParseConfigInit();
    drv_DescTsPacketBasicInit();
    return ErrCode;
}


 raw_channel_set_pid_std(STPTI_Slot_t *StdSlotHandle,STPTI_Pid_t *PidList, U8 PidNum)
{
    DRV_ErrCode	ErrCode;
    U8 i,j=0;
    STPTI_Pid_t stdpidlist[17];
    if((StdSlotHandle==NULL)||(PidList==NULL)||(PidNum==0))
    {
        STTBX_Print(("%s,%d: *warning* pid num donot match slot num !\n", __FUNCTION__, __LINE__));
        return SECTION_ERROR_ST_ERROR;
    }   
    else
    {
        for(j = 0;j <= 9 + PatTableInfo.ProgramMaxNum; j++)
        {
            if (j < 9)
            {
                stdpidlist[j] = mStdPid[j];
            }
            else
            {
                stdpidlist[j] = PmtPidData[j-9].PmtPid;
            }
        }

        countstdPidNum = PatTableInfo.ProgramMaxNum + 9;
        
        for(i=0;i<countstdPidNum;i++)
        {
             ErrCode = HI_UNF_DMX_SetChannelPID(StdSlotHandle[i],stdpidlist[i]);
             ErrCode |= HI_UNF_DMX_OpenChannel(StdSlotHandle[i]);
             if (ErrCode != ST_NO_ERROR)
             {
                 STTBX_Print(("%s,%d: *HI_UNF_DMX_SetChannelPID Failed!* ErrCode[%d]\n", __FUNCTION__, __LINE__,ErrCode));
        	     return (SECTION_ERROR_ST_ERROR);
        	 }
        	 printf("-----HI_UNF_DMX_SetChannelPID StdSlotHandle[%d] = %d,Pid = 0x%x\n",i,StdSlotHandle[i],stdpidlist[i]);
        }

    }
    return ErrCode;
}

 raw_channel_set_pid_des(STPTI_Slot_t *DescSlotHandle,STPTI_Pid_t *PidList, U8 PidNum)
{
    extern  DRV_Pid m_curVidPid;
    extern  DRV_Pid m_curAudPid;
    DRV_ErrCode	ErrCode;

    if((DescSlotHandle==NULL)||(PidList==NULL)||(PidNum==0))
    {
        STTBX_Print(("%s,%d: *warning* pid num donot match slot num !\n", __FUNCTION__, __LINE__));
        return SECTION_ERROR_ST_ERROR;
    }   
    else
    {
        ErrCode = HI_UNF_DMX_SetChannelPID(DescSlotHandle[0],m_curVidPid);
        ErrCode |= HI_UNF_DMX_OpenChannel(DescSlotHandle[0]);
        printf("-----HI_UNF_DMX_SetChannelPID DescSlotHandle[0] = %d,Pid = 0x%x\n",DescSlotHandle[0],m_curVidPid);
        ErrCode = HI_UNF_DMX_SetChannelPID(DescSlotHandle[1],m_curAudPid);
        ErrCode |= HI_UNF_DMX_OpenChannel(DescSlotHandle[1]);
        printf("-----HI_UNF_DMX_SetChannelPID DescSlotHandle[1] = %d,Pid = 0x%x\n",DescSlotHandle[1],m_curAudPid);
    }
    return ErrCode;
}

void drv_MultiRawChannelSetPID(STPTI_Slot_t *DescSlotHandle,STPTI_Slot_t *StdSlotHandle,STPTI_Pid_t *DescPidList,STPTI_Pid_t *StdPidList,U8 DescPidNum,U8 StdPidNum)
{
    DRV_ErrCode	ErrCode;
    
    DescRawPacketTaskState.StartTsReceive=FALSE;
    
    ErrCode=raw_channel_set_pid_des(DescSlotHandle,DescPidList,DescPidNum);
    
    ErrCode=raw_channel_set_pid_std(StdSlotHandle,StdPidList,StdPidNum);
    
    DescRawPacketTaskState.StartTsReceive=TRUE;
    return ErrCode;
}

void drv_DescKeyLinkDescChannel(U32 *DescramblerHandle, STPTI_Slot_t *SlotHandle, U8 LinkNum)
{
    DRV_ErrCode ErrCode;
    U8 i=0;
    if((DescramblerHandle==NULL)||(SlotHandle==NULL)||(LinkNum==0))
    {
        STTBX_Print(("%s,%d: *error parameter!* \n", __FUNCTION__, __LINE__));
        return;
    }
    if(LinkNum!=mDescChannelNum)
    {
        STTBX_Print(("%s,%d: *warning* Descrambler num donot match channel num !\n", __FUNCTION__, __LINE__));
    }
    for(i=0;i<LinkNum;i++)
    {
        if(ErrCode=HI_UNF_DMX_AttachDescrambler(DescramblerHandle[i],SlotHandle[i] ))
        {
            STTBX_Print(("------%s faild,ErrCode is %d\n",__func__,ErrCode));
            return ErrCode;
        }
    }
}

void drv_SetEcmPidForCa(PMT_ProgramPid_t *ProgramKyePid,U8 EncryptProgramNum)//PCR_PID不需要设置
{
    extern  DRV_Pid m_curVidPid;
    extern  DRV_Pid m_curAudPid;
    int i =0;
    SCDCASServiceInfo astServiceInfo;

    CDCASTB_SetEcmPid(CDCA_LIST_FIRST,NULL);
    CDCASTB_SetEcmPid(CDCA_LIST_OK, NULL);
    CA_StopPrivateDataReceive(); /*释放换台前申请的filter*/
        
    CDCASTB_SetEcmPid(CDCA_LIST_FIRST, NULL);
    for(i =0;i<EncryptProgramNum;i++)
    {
        if ((ProgramKyePid[i].AudPid == m_curAudPid)&&(ProgramKyePid[i].VidPid == m_curVidPid))
        {
            if(ProgramKyePid[i].AudEcmPid!=8191)
            {
                astServiceInfo.m_byServiceNum = 1;
                astServiceInfo.m_wEcmPid = ProgramKyePid[i].AudEcmPid;
                astServiceInfo.m_wServiceID[0] = 1;
                CDCASTB_SetEcmPid(CDCA_LIST_ADD, &astServiceInfo);
                printf("11111111111111111drv_SetEcmPidForCa\n");
            }
            if(ProgramKyePid[i].VidEcmPid!=8191)  
            {
                astServiceInfo.m_byServiceNum = 1;
                astServiceInfo.m_wEcmPid = ProgramKyePid[i].VidEcmPid;
                astServiceInfo.m_wServiceID[0] = 1;
                CDCASTB_SetEcmPid(CDCA_LIST_ADD, &astServiceInfo);
                printf("22222222222222222drv_SetEcmPidForCa\n");
            }
        }
    }
    CDCASTB_SetEcmPid(CDCA_LIST_OK, NULL);
}


void drv_CopyKeyto(PMT_ProgramPid_t *ProgramKyePid,U32 *DescKeyArrzy,U8 EncryptProgramNum,STPTI_Pid_t *EncryptPid)
{
    U8 ProgCnt=0;
    U8 PidCnt=0;
    U8 DescKeyCnt=0;

    for(ProgCnt=0;ProgCnt<EncryptProgramNum;ProgCnt++)
    {
        if(ProgramKyePid[ProgCnt].AudEcmPid!=8191)
        {
            ProgramKyePid[ProgCnt].AudeoKey=DescKeyArrzy[DescKeyCnt++];
            EncryptPid[PidCnt++]=ProgramKyePid[ProgCnt].AudPid;
        }
        if(ProgramKyePid[ProgCnt].VidEcmPid!=8191)
        {
            ProgramKyePid[ProgCnt].VideoKey=DescKeyArrzy[DescKeyCnt++];
            EncryptPid[PidCnt++]=ProgramKyePid[ProgCnt].VidPid;
        }  
    }  

}

void drv_CreateMultiKeyForDescChannel(U32 *DescramblerHandle,U8 DescramblerNum)
{
    DRV_ErrCode ErrCode;
    U8 i=0;  
    if((DescramblerHandle==NULL)||(DescramblerNum==0))
    {
        STTBX_Print(("%s,%d: *error parameter!* DescramblerNum=[%d]\n", __FUNCTION__, __LINE__,DescramblerNum));
        return;
    }
    for(i=0;i<DescramblerNum;i++)
    {
       if(ErrCode=pti_allocate_dynamic_key(0, &DescramblerHandle[i]))
       {
            STTBX_Print(("------%s faild,ErrorCode is %d\n",__func__,ErrCode));
       }
    }
    return ErrCode;
}

create_raw_channel_link_single_buffer_std (STPTI_Slot_t *StdSlotHandle,U32 SlotNum)
{
    DRV_ErrCode	ErrCode;
    DRV_ErrCode	ErrCodeAux;
    U32 i;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
	
    if((SlotNum>=MAX_STD_CHANNEL_NUM)||(StdSlotHandle==NULL))
    {
        STTBX_Print(("%s,%d: create_raw_channel_link_single_buffer *error parameter!* \n", __FUNCTION__, __LINE__));
        return SECTION_ERROR_ST_ERROR;
    }
    for(i=0;i<SlotNum;i++)
    {
        ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
        if (ErrCode != HI_SUCCESS)
        {
            STTBX_Print((" HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
            return (SECTION_ERROR_ST_ERROR);
        }
        stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_POST;
        stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        stChnAttr.u32BufSize = 512*1024;
        stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&StdSlotHandle[i]);
        if (ErrCode != HI_SUCCESS)
        {
            STTBX_Print((" HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
            return (SECTION_ERROR_ST_ERROR);
        }
        printf("-----HI_UNF_DMX_CreateChannel success, StdSlotHandle[%d] = %d\n",i,StdSlotHandle[i]);
        
    }
   
    return 0;
}

create_raw_channel_link_single_buffer_des (STPTI_Slot_t *DescSlotHandle,U32 SlotNum)
{
    DRV_ErrCode	ErrCode;
    DRV_ErrCode	ErrCodeAux;
    U32 i;
	HI_UNF_DMX_CHAN_ATTR_S stChnAttr;
	memset(&stChnAttr,0,sizeof(HI_UNF_DMX_CHAN_ATTR_S));
	
    if((SlotNum>=MAX_DESC_CHANNEL_NUM)||(DescSlotHandle==NULL))
    {
        STTBX_Print(("%s,%d: create_raw_channel_link_single_buffer *error parameter!* \n", __FUNCTION__, __LINE__));
        return SECTION_ERROR_ST_ERROR;
    }
    for(i=0;i<SlotNum;i++)
    {
        ErrCode = HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr);
        if (ErrCode != HI_SUCCESS)
        {
            STTBX_Print((" HI_UNF_DMX_GetChannelDefaultAttr ErrCode 0x%08x\n",ErrCode));
            return (SECTION_ERROR_ST_ERROR);
        }
        stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_POST;
        stChnAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
        stChnAttr.u32BufSize = 512*1024;
        stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
        ErrCode = HI_UNF_DMX_CreateChannel(0,&stChnAttr,&DescSlotHandle[i]);
        if (ErrCode != HI_SUCCESS)
        {
            STTBX_Print((" HI_UNF_DMX_CreateChannel ErrCode 0x%08x\n",ErrCode));
            return (SECTION_ERROR_ST_ERROR);
        }
        printf("-----HI_UNF_DMX_CreateChannel success, DescSlotHandle[%d] = %d\n",i,DescSlotHandle[i]);
        
    }
   
    return 0;
}

void drv_CreateRawChannelLinkSingleBuffer(STPTI_Slot_t *DescSlotHandle,STPTI_Slot_t *StdSlotHandle,U8 DescSlotNum,U8 StdSlotNum)
{
    DRV_ErrCode	ErrCode = 0;
    ErrCode=create_raw_channel_link_single_buffer_des(DescSlotHandle,DescSlotNum);
    
    ErrCode=create_raw_channel_link_single_buffer_std(StdSlotHandle,StdSlotNum);
    mDescChannelNum=DescSlotNum;
    return ErrCode;
}



void PAT_TableDataCallBack(int FilterNo, BOOLEAN isTimeOut, unsigned char *pSecData, unsigned short SecLength)
{
    if(isTimeOut)	
	{
		STTBX_Print(("[Test] TestFilterCallback: section filter Timeout\n"));
	}
    else
    {
        memcpy(ParsePatTable.PatTableBuffer, pSecData, SecLength );
        ParsePatTable.PatTableSize=SecLength;
        ParsePatTable.CurrentPatTableValid=TRUE;
        DRV_FILTFreeFilterReq(FilterNo);
    }
}

DRV_ErrCode drv_StartGetParseTableData(void)
{
    int PatSlotID=32;

    PatSlotID=DRV_FILTReqFilterForStandard( NULL, PAT_TableDataCallBack, ONE_KILO_SECTION,
                                               PAT_PID, 
                                               PAT_TABLE_ID, 
                                               -1, 
                                               0, 
                                               -1, 
                                               10000, 
                                               -1, 
                                               -1 );
    if(PatSlotID < 0)
    {
        STTBX_Print(("%s,%d: *DRV_FILTReqFilterForStandard Failed!*PatSlotID[%d]\n", __FUNCTION__, __LINE__,PatSlotID));
    } 
}

DRV_ErrCode DRV_StartDescCurrentTs(void)
{
    DRV_ErrCode	ErrCode;
    MuxTsFileHandle = fopen("/mnt/pvrallts.ts","w+");
    //MuxTsFileHandle=DRV_VfsOpenFile("c:/MuxPt.ts","w+");
    if (MuxTsFileHandle==-1)
    {
        STTBX_Print(("%s,%d: * DRV_VfsOpenFile Failed!!*\n", __FUNCTION__, __LINE__));
        return(ST_ERROR_BAD_PARAMETER);
    }
        
    drv_StartGetParseTableData();
#if 1    
AGAIN:
    if(AssortParsePid.AssortPidFinishState==TRUE)
    {
        AssortParsePid.AssortPidFinishState=FALSE;
        drv_CreateRawChannelLinkSingleBuffer(g_DescSlotHandle,g_StdSlotHandle,g_ProgramPidCount.EncryptPidNum,g_ProgramPidCount.NotEncryptPidNum);

        drv_CreateMultiKeyForDescChannel(g_AVKeyArrzy,g_ProgramPidCount.EncryptPidNum);
        drv_CopyKeyto(g_ProgramKyePid,g_AVKeyArrzy,g_ProgramPidCount.EncryptProgramNum,mEncryptPid);
        drv_SetEcmPidForCa(g_ProgramKyePid,g_ProgramPidCount.EncryptProgramNum);
        drv_DescKeyLinkDescChannel(g_AVKeyArrzy,g_DescSlotHandle,mDescChannelNum);
        
        drv_MultiRawChannelSetPID(g_DescSlotHandle,g_StdSlotHandle,mEncryptPid,mNotEncryptPid,g_ProgramPidCount.EncryptPidNum,g_ProgramPidCount.NotEncryptPidNum);
        
        pvrallts_start = 1;
    }
    else
    {
        DRV_OS_TaskDelay_MS(500);
        goto AGAIN;
    }
#endif    
}







DRV_ErrCode destroy_raw_channel_unlink_single_buffer(STPTI_Slot_t *SlotHandle,U8 SlotNum)
{
    DRV_ErrCode	ErrCode;
    U8 i=0;
    
    if((SlotNum>=MAX_DESC_CHANNEL_NUM)||(SlotHandle==NULL) ||(SlotNum==0))
    {
        STTBX_Print(("%s,%d: *error parameter!* \n", __FUNCTION__, __LINE__));
        return SECTION_ERROR_ST_ERROR;
    }
    for(i=0;i<SlotNum;i++)
    {
        ErrCode = HI_UNF_DMX_CloseChannel(SlotHandle[i]);
        if (ErrCode != ST_NO_ERROR)
        {
            STTBX_Print(("%s,%d: *HI_UNF_DMX_CloseChannel Failed!* ErrCode[%d]\n", __FUNCTION__, __LINE__,ErrCode));
        }
        ErrCode = HI_UNF_DMX_DestroyChannel(SlotHandle[i]);
    	if (ErrCode != ST_NO_ERROR)
    	{
            STTBX_Print(("%s,%d: *HI_UNF_DMX_DestroyChannel Failed!* ErrCode[%d]\n", __FUNCTION__, __LINE__,ErrCode));
    	}   
    }
    return ErrCode;
}

DRV_ErrCode drv_destroyRawChannelUnlinkSingleBuffer(STPTI_Slot_t *DescSlotHandle,STPTI_Slot_t *StdSlotHandle,U8 DescSlotNum,U8 StdSlotNum)
{
    DRV_ErrCode	ErrCode;
    ErrCode=destroy_raw_channel_unlink_single_buffer(DescSlotHandle,DescSlotNum);
    ErrCode=destroy_raw_channel_unlink_single_buffer(StdSlotHandle,StdSlotNum);
    return ErrCode;
}

U32 drv_DeallocateMultiKeyForDescChannel(U32 *DescramblerHandle,U8 DescramblerNum)
{
    DRV_ErrCode ErrCode;
    U8 i=0;  
    if((DescramblerHandle==NULL)||(DescramblerNum==0))
    {
        STTBX_Print(("%s,%d: *error parameter!* \n", __FUNCTION__, __LINE__));
        return;
    }
    for(i=0;i<DescramblerNum;i++)
    {
       if(ErrCode=HI_UNF_DMX_DestroyDescrambler(DescramblerHandle[i]))
       {
            STTBX_Print(("------%s faild,ErrorCode is %d\n",__func__,ErrCode));
       }
    }
    return ErrCode;
}

U32 drv_DescKeyUnlinkChannel(U32 *DescramblerHandle,STPTI_Slot_t *SlotHandle,U8 UnlinkNum)
{
    DRV_ErrCode ErrCode;
    U8 i=0;
    if((DescramblerHandle==NULL)||(SlotHandle==NULL)||(UnlinkNum==0))
    {
        STTBX_Print(("%s,%d: *error parameter!* \n", __FUNCTION__, __LINE__));
        return;
    }
    for(i=0;i<UnlinkNum;i++)
    {
        if(ErrCode=HI_UNF_DMX_DetachDescrambler(DescramblerHandle[i], SlotHandle[i]))
        {
            STTBX_Print(("------%s faild,ErrCode is %d\n",__func__,ErrCode));
            return ErrCode;
        }
    }
}
void drv_StopCACallback(void)/*通知CA库释放上一个频点的ECM，filter*/
{
    CDCASTB_SetEcmPid(CDCA_LIST_FIRST,NULL);
    CDCASTB_SetEcmPid(CDCA_LIST_OK, NULL);
    CA_StopPrivateDataReceive(); /*释放换台前申请的filter*/
}
DRV_ErrCode DRV_StopDescCurrentTs(void)
{
    DRV_ErrCode	ErrCode; 
    DRV_VfsFflush(MuxTsFileHandle);
    DRV_VfsCloseFile(MuxTsFileHandle);
    pvrallts_start = 0;

    DescRawPacketTaskState.StartTsReceive=FALSE;
    
    drv_StopCACallback();

    drv_DescKeyUnlinkChannel(g_AVKeyArrzy,g_DescSlotHandle,g_ProgramPidCount.EncryptPidNum);
    drv_DeallocateMultiKeyForDescChannel(g_AVKeyArrzy,g_ProgramPidCount.EncryptPidNum);
    drv_destroyRawChannelUnlinkSingleBuffer(g_DescSlotHandle,g_StdSlotHandle,g_ProgramPidCount.EncryptPidNum,g_ProgramPidCount.NotEncryptPidNum);
}

void DRV_CA_ScrSetCW(unsigned short wEcmPID,const unsigned char* OddKey, const unsigned char* EvenKey, 
								unsigned char byKeyLen,unsigned char RecordFlag)

{
    int i = 0;
    unsigned char  PcrCnt = 0;
    unsigned char szOdd[8];
    unsigned char szEven[8];  

    if((OddKey == NULL) || (EvenKey == NULL))
    {
        printf("CDSTBCA_ScrSetCW Failed Because Input Para Is NULL!\n");
        return;
    }
    
    memcpy(szOdd,OddKey,8);
    memcpy(szEven,EvenKey,8);
    szOdd[7]=(szOdd[6]+szOdd[5]+szOdd[4])&0xFF;
    szOdd[3]=(szOdd[2]+szOdd[1]+szOdd[0])&0xFF;
    szEven[7]=(szEven[6]+szEven[5]+szEven[4])&0xFF;
    szEven[3]=(szEven[2]+szEven[1]+szEven[0])&0xFF;
	 STTBX_Print(("[CA] CDSTBCA_ScrSetCW: wEcmPID:%u CW:\n\
	 0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x\n\
	 0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x  0X%02x\n",
			 wEcmPID,szOdd[0], szOdd[1], szOdd[2], szOdd[3], szOdd[4], szOdd[5], szOdd[6], szOdd[7],
			 szEven[0], szEven[1], szEven[2], szEven[3], szEven[4], szEven[5], szEven[6], szEven[7]));
    for(i = 0;i<g_ProgramPidCount.EncryptProgramNum;i++)
    {
        if(g_ProgramKyePid[i].AudEcmPid == wEcmPID)
        {
    		 pti_set_descramble_key(g_ProgramKyePid[i].AudeoKey, odd_parity, 2, (unsigned char*)szOdd); 
    		 pti_set_descramble_key(g_ProgramKyePid[i].AudeoKey, even_parity, 2, (unsigned char*)szEven);
    		 printf("=================audecm input ok !!!\n");
        }
        if(g_ProgramKyePid[i].VidEcmPid == wEcmPID)
        {
    		 pti_set_descramble_key(g_ProgramKyePid[i].VideoKey, odd_parity, 2, (unsigned char*)szOdd); 
    		 pti_set_descramble_key(g_ProgramKyePid[i].VideoKey, even_parity, 2, (unsigned char*)szEven);
    		 printf("=================videcm input ok !!!\n");
        }
    }

}

