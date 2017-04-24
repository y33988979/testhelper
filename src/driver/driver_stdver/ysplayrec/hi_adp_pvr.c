/**
\file
\brief common function of PVR
\copyright Shenzhen Hisilicon Co., Ltd.
\date 2008-2018
\version draft
\author QuYaxin 46153
\date 2010-3-26
*/

#include "hi_adp.h"
#include "hi_adp_pvr.h"
//#include "hi_adp_mpi.h"
//#define PVR_ADVCA_MODE /*if use ADVCA mode,define PVR_ADVCA_MODE*/
#include "ysav.h"
#include "HA.AUDIO.AC3PASSTHROUGH.decode.h"
#include "HA.AUDIO.DOLBYPLUS.decode.h"

/***************************** Macro Definition ******************************/



/*************************** Structure Definition ****************************/
typedef struct hiTS_SEND_ARGS_S
{
    HI_U32 u32DmxId;
    HI_U32 u32PortId;
    FILE   *pTsFile;
} TS_SEND_ARGS_S;

typedef struct tagPVREventType
{
    HI_U8 szEventTypeName[128];
    HI_UNF_PVR_EVENT_E eEventID;
}PVR_EVENT_TYPE_ST;
/********************** Global Variable declaration **************************/
static HI_S32       g_s32PvrRecChnNum[DMX_CNT]; /* number of record channel for each demux */
static HI_HANDLE    g_hPvrRecChns[DMX_CNT][8];  /* handle of record channel for each demux */

HI_BOOL             g_bIsRecStop = HI_FALSE;
static HI_BOOL      g_bStopTsThread = HI_FALSE;
HI_HANDLE           g_hTsBufForPlayBack;


static PVR_EVENT_TYPE_ST g_stEventType[] = {
    {"HI_UNF_PVR_EVENT_PLAY_EOF",       HI_UNF_PVR_EVENT_PLAY_EOF},
    {"HI_UNF_PVR_EVENT_PLAY_SOF",       HI_UNF_PVR_EVENT_PLAY_SOF},
    {"HI_UNF_PVR_EVENT_PLAY_ERROR",     HI_UNF_PVR_EVENT_PLAY_ERROR},
    {"HI_UNF_PVR_EVENT_PLAY_REACH_REC", HI_UNF_PVR_EVENT_PLAY_REACH_REC},
    {"HI_UNF_PVR_EVENT_PLAY_RESV",      HI_UNF_PVR_EVENT_PLAY_RESV},
    {"HI_UNF_PVR_EVENT_REC_DISKFULL",   HI_UNF_PVR_EVENT_REC_DISKFULL},
    {"HI_UNF_PVR_EVENT_REC_ERROR",      HI_UNF_PVR_EVENT_REC_ERROR},
    {"HI_UNF_PVR_EVENT_REC_OVER_FIX",   HI_UNF_PVR_EVENT_REC_OVER_FIX},
    {"HI_UNF_PVR_EVENT_REC_REACH_PLAY", HI_UNF_PVR_EVENT_REC_REACH_PLAY},
    {"HI_UNF_PVR_EVENT_REC_DISK_SLOW",  HI_UNF_PVR_EVENT_REC_DISK_SLOW},
    {"HI_UNF_PVR_EVENT_REC_RESV",       HI_UNF_PVR_EVENT_REC_RESV},
    {"HI_UNF_PVR_EVENT_BUTT",           HI_UNF_PVR_EVENT_BUTT}
};

HI_U8* PVR_GetEventTypeStringByID(HI_UNF_PVR_EVENT_E eEventID);

/******************************* API declaration *****************************/
#if 1
HI_VOID SearchFileTsSendThread(HI_VOID *args)
{
    HI_UNF_STREAM_BUF_S   StreamBuf;
    HI_U32            Readlen;
    HI_S32            Ret;
    HI_HANDLE         g_TsBuf;
    TS_SEND_ARGS_S    *pstPara = args;

    Ret = HI_UNF_DMX_AttachTSPort(pstPara->u32DmxId, pstPara->u32PortId);
    if (HI_SUCCESS != Ret)
    {
        printf("call VoInit failed.\n");
        return;
    }

    Ret = HI_UNF_DMX_CreateTSBuffer(pstPara->u32PortId, 0x200000, &g_TsBuf);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_CreateTSBuffer failed.\n");
        return;
    }

    while (!g_bStopTsThread)
    {
        Ret = HI_UNF_DMX_GetTSBuffer(g_TsBuf, 188*50, &StreamBuf, 0);
        if (Ret != HI_SUCCESS )
        {
            usleep(10 * 1000) ;
            continue;
        }

        Readlen = fread(StreamBuf.pu8Data, sizeof(HI_S8), 188*50, pstPara->pTsFile);
        if(Readlen <= 0)
        {
            printf("read ts file error!\n");
            rewind(pstPara->pTsFile);
            continue;
        }

        Ret = HI_UNF_DMX_PutTSBuffer(g_TsBuf, Readlen);
        if (Ret != HI_SUCCESS )
        {
            printf("call HI_UNF_DMX_PutTSBuffer failed.\n");
        }
    }

    Ret = HI_UNF_DMX_DestroyTSBuffer(g_TsBuf);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_DMX_DestroyTSBuffer failed.\n");
    }
    HI_UNF_DMX_DetachTSPort(pstPara->u32DmxId);

    return;
}

static HI_S32 PVR_SearchFile(HI_U32 u32DmxId, HI_U32 u32PortId, const HI_CHAR *pszFileName, PMT_COMPACT_TBL **ppProgTbl)
{
    HI_S32 Ret;
    pthread_t   TsThd;
    TS_SEND_ARGS_S    stPara;
    FILE *pTsFile;

    pTsFile = fopen(pszFileName, "rb");

    if(pTsFile == NULL)
    {
        printf("PVR_SearchFile open TS file failed.\n");
        return HI_FAILURE;
    }

    stPara.u32DmxId = u32DmxId;
    stPara.u32PortId = u32PortId;
    stPara.pTsFile = (FILE *)pTsFile;

    g_bStopTsThread = HI_FALSE;
    pthread_create(&TsThd, HI_NULL, (HI_VOID *)SearchFileTsSendThread, &stPara);


    sleep(1);

    HIADP_Search_Init();
    Ret = HIADP_Search_GetAllPmt(u32DmxId, ppProgTbl);
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_Search_GetAllPmt failed.\n");
    }
    HIADP_Search_DeInit();

    g_bStopTsThread =HI_TRUE;
    pthread_join(TsThd, HI_NULL);
    fclose(pTsFile);

    return Ret;
}

#endif
static HI_S32 PVR_RecAddPid(HI_U32 u32DmxId, int pid, HI_UNF_DMX_CHAN_TYPE_E chnType)
{
    HI_U32 ret      = HI_SUCCESS;
    HI_S32 chnIdx   = 0;
    HI_HANDLE       hPidChn;

    HI_UNF_DMX_CHAN_ATTR_S stChnAttr;

    if(u32DmxId >= DMX_CNT)
    {
        PRINT_SMP("IN PVR_RecAddPid the u32DmxId:%d is over than DMX_CNT:(%d)!!!\n", u32DmxId, DMX_CNT);
        return HI_FAILURE;
    }

    chnIdx   = g_s32PvrRecChnNum[u32DmxId];

    //HI_MPI_DMX_SetRecodeType(u32DmxId, 1); /* 1== DESCRAM TS */

    ret = HI_UNF_DMX_GetChannelHandle(u32DmxId, pid, &hPidChn);
    if (HI_SUCCESS == ret)
    {
        SAMPLE_RUN(HI_UNF_DMX_GetChannelAttr(hPidChn, &stChnAttr), ret);
        if (HI_SUCCESS != ret)
        {
            return ret;
        }

        if (stChnAttr.enChannelType != HI_UNF_DMX_CHAN_OUTPUT_MODE_REC)
        {
            PRINT_SMP("Pid %d already open for PLAY, can NOT recorde\n", pid);
            return ret;
        }
    }


    /*demux get attribution of default channel *//*CNcomment:demux��ȡͨ��Ĭ������*/
    SAMPLE_RUN(HI_UNF_DMX_GetChannelDefaultAttr(&stChnAttr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    stChnAttr.enChannelType = chnType;
    //stChnAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
    stChnAttr.enCRCMode    = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    stChnAttr.u32BufSize   = 64;
    stChnAttr.enOutputMode = HI_UNF_DMX_CHAN_OUTPUT_MODE_REC;


    /*demux create channel *//*CNcomment: demux�½�ͨ��*/
    SAMPLE_RUN(HI_UNF_DMX_CreateChannel(u32DmxId, &stChnAttr, &hPidChn), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    /*demux set channel PID*//*CNcomment:demux ����ͨ��PID*/
    SAMPLE_RUN(HI_UNF_DMX_SetChannelPID(hPidChn, pid), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_DMX_DestroyChannel(hPidChn);
        return ret;
    }

    /*demux open channel*//*CNcomment:demux��ͨ��*/
    SAMPLE_RUN(HI_UNF_DMX_OpenChannel(hPidChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_DMX_DestroyChannel(hPidChn);
        return ret;
    }

    chnIdx %= 8; //by g_hPvrRecChns definition, the max column index should be less than 8
    
    g_hPvrRecChns[u32DmxId][chnIdx] = hPidChn;
    g_s32PvrRecChnNum[u32DmxId]++;

    printf("DMX:%d add PID OK, pid:%d/%#x, Type:%d\n", u32DmxId, pid, pid, chnType);

    return HI_SUCCESS;
}

static HI_S32 PVR_RecDelAllPid(HI_U32 u32DmxId)
{
    HI_S32 i;
    HI_S32 ret = HI_SUCCESS;
    HI_HANDLE hPidChn;

    for (i = 0; i < g_s32PvrRecChnNum[u32DmxId]; i++)
    {
        hPidChn = g_hPvrRecChns[u32DmxId][i];
        SAMPLE_RUN(HI_UNF_DMX_CloseChannel(hPidChn),ret);
        SAMPLE_RUN(HI_UNF_DMX_DestroyChannel(hPidChn), ret);
        printf("DMX_DestroyChannel: %d.\n", hPidChn);
    }

    g_s32PvrRecChnNum[u32DmxId]= 0;

    return ret;
}

HI_S32 PVR_SavePorgInfo(PVR_PROG_INFO_S *pstProgInfo, HI_CHAR *pszPvrRecFile)
{
    HI_S32 ret;
    PVR_PROG_INFO_S userData;

    SAMPLE_CheckNullPTR(pstProgInfo);

	memcpy(&userData, pstProgInfo, sizeof(PVR_PROG_INFO_S));
	userData.u32MagicNumber = PVR_PROG_INFO_MAGIC;

    ret = HI_UNF_PVR_SetUsrDataInfoByFileName(pszPvrRecFile, (HI_U8*)&userData, sizeof(PVR_PROG_INFO_S));
    if (HI_SUCCESS != ret)
    {
        printf("PVR_SetUsrDataInfoByFileName ERR:%#x.\n", ret);
        return ret;
    }

    usleep(10*1000);
    printf("\n------------------\n");
    printf("File Info:\n");
    printf("Pid:  A=%d/%#x, V=%d/%#x.\n",pstProgInfo->stProgInfo.AElementPid,
                                         pstProgInfo->stProgInfo.AElementPid,
                                         pstProgInfo->stProgInfo.VElementPid,
                                         pstProgInfo->stProgInfo.VElementPid);
    printf("Type: A=%d, V=%d.\n", pstProgInfo->stProgInfo.AudioType, pstProgInfo->stProgInfo.VideoType);
    printf("isClearStream: %d, isEncrypt: %d.\n", pstProgInfo->stRecAttr.bIsClearStream, pstProgInfo->stRecAttr.stEncryptCfg.bDoCipher);
    printf("indexType: %d, bRewind: %d. maxSize: 0x%llx\n", pstProgInfo->stRecAttr.enIndexType, pstProgInfo->stRecAttr.bRewind, pstProgInfo->stRecAttr.u64MaxFileSize);
    printf("------------------\n\n");
    usleep(10*1000);


    return HI_SUCCESS;
}

HI_S32 PVR_GetPorgInfo(PVR_PROG_INFO_S *pstProgInfo, const HI_CHAR *pszPvrRecFile)
{
    HI_S32 ret;
    HI_U32 dataRead;

    PVR_PROG_INFO_S userData;

    SAMPLE_CheckNullPTR(pstProgInfo);
    SAMPLE_CheckNullPTR(pszPvrRecFile);

    ret = HI_UNF_PVR_GetUsrDataInfoByFileName(pszPvrRecFile, (HI_U8*)&userData,
                                sizeof(PVR_PROG_INFO_S), &dataRead);
    if (HI_SUCCESS != ret)
    {
        printf("PVR_SetUsrDataInfoByFileName ERR:%#x.\n", ret);
        return ret;
    }

	if (PVR_PROG_INFO_MAGIC == userData.u32MagicNumber)
	{
        memcpy(pstProgInfo, &(userData),  sizeof(PVR_PROG_INFO_S));
	}
    else
    {
        //HI_U32 temp1 = 0;
        printf("Can  only play the program record by sample. \n");

        return HI_FAILURE;

    }

    usleep(10*1000);
    printf("\n------------------\n");
    printf("File Info:\n");
    if (pstProgInfo->stProgInfo.AElementNum > 0)
    {
        printf("Audio:\n");
        printf("   PID = %#x\n",pstProgInfo->stProgInfo.AElementPid);
        printf("   Type= %x\n", pstProgInfo->stProgInfo.AudioType);
    }
    else
    {
        printf("Audio: none\n");
    }
    if (pstProgInfo->stProgInfo.VElementNum > 0)
    {
        printf("Video:\n");
        printf("   PID = %#x\n",pstProgInfo->stProgInfo.VElementPid);
        printf("   Type= %d\n", pstProgInfo->stProgInfo.VideoType);
    }
    else
    {
        printf("Video: none\n\n");
    }

    printf("isClearStream: %d, isEncrypt: %d.\n", pstProgInfo->stRecAttr.bIsClearStream, pstProgInfo->stRecAttr.stEncryptCfg.bDoCipher);
    printf("indexType: %d, bRewind: %d. maxSize: 0x%llx\n", pstProgInfo->stRecAttr.enIndexType, pstProgInfo->stRecAttr.bRewind, pstProgInfo->stRecAttr.u64MaxFileSize);
    printf("------------------\n");
    usleep(10*1000);

    return HI_SUCCESS;
}

HI_S32 PVR_checkIdx(char *pfileName)
{
#if 0
    int  i;
    PVR_INDEX_ENTRY_S entry;
    HI_U64 lastOffset = 0;
    HI_U32 sizeCalc;
    HI_U32 scbuf[10];
    HI_U32 LastSize=0;
#endif
    int ret = 0;
    int pos;
    int readNum;
    PVR_IDX_HEADER_INFO_S headInfo;
    FILE *fpIndex;
    int fileTs;

    char indexName[256];
    sprintf(indexName, "%s.idx", pfileName);

    fpIndex = fopen(indexName, "rb");
    if (NULL == fpIndex)
    {
        printf("can't open file %s to read!\n", indexName);
        return 2;
    }
//    fileTs = open(pfileName, O_RDONLY | O_LARGEFILE);
    fileTs = open(pfileName, O_RDONLY);
    if (fileTs == -1)
    {
        printf("can't open ts file %s to read!\n", indexName);
        fclose(fpIndex);
        return 2;
    }
    fseek(fpIndex, 0, SEEK_END);
    pos = ftell(fpIndex);
    rewind(fpIndex);

    readNum = fread(&headInfo, 1, sizeof(PVR_IDX_HEADER_INFO_S), fpIndex);
    if (readNum != sizeof(PVR_IDX_HEADER_INFO_S))
    {
            perror("read failed:");
            printf("read head failed: want%d, get:%d\n", sizeof(PVR_IDX_HEADER_INFO_S), readNum);
            return 3;
    }
    else
    {
        if (headInfo.u32StartCode == 0x5A5A5A5A)
        {
            printf("This index file has head info: head size=%u, fileSize=%llu\n", headInfo.u32HeaderLen, headInfo.u64ValidSize);
            printf("IndexEntrySize: %d, index file size:%d, headifo len:%d\n",
                     sizeof(PVR_INDEX_ENTRY_S), pos, headInfo.u32HeaderLen);
            fseek(fpIndex, headInfo.u32HeaderLen, SEEK_SET);
        }
        else
        {
            printf("This index file has NO head info\n");
            return 3;
        }
    }
    printf("\nframe info:\n");
    printf("====frame start:%d\n", headInfo.stCycInfo.u32StartFrame);
    printf("====frame end:  %d\n", headInfo.stCycInfo.u32EndFrame);
    printf("====frame last: %d\n", headInfo.stCycInfo.u32LastFrame);

#if 0
    if (headInfo.stCycInfo.u32StartFrame > headInfo.stCycInfo.u32EndFrame)
    {
        fseek(fpIndex, (headInfo.stCycInfo.u32StartFrame * sizeof(PVR_INDEX_ENTRY_S) + headInfo.u32HeaderLen), SEEK_SET);

        for (i = headInfo.stCycInfo.u32StartFrame; i <= headInfo.stCycInfo.u32LastFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                printf("read failed: i =%u offset:%d want%d, get:%d\n", i, (headInfo.stCycInfo.u32StartFrame + i) * sizeof(PVR_INDEX_ENTRY_S), sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    printf("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                    //return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        printf("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset,entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                    //    printf("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }

                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if ((sizeCalc != LastSize) && (0 != LastSize))
                {
                    printf("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset, lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }

        fseek(fpIndex, headInfo.u32HeaderLen, SEEK_SET);
        for (i = 0; i < headInfo.stCycInfo.u32EndFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                printf("read failed: want%d, get:%d\n", sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    printf("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                //    return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        printf("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset, entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                    //    printf("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }

                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if (sizeCalc != LastSize)
                {
                    //printf("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset , lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }

    }
    else
    {
        for (i = headInfo.stCycInfo.u32StartFrame; i < headInfo.stCycInfo.u32EndFrame; i++)
        {
            readNum = fread(&entry, 1, sizeof(PVR_INDEX_ENTRY_S), fpIndex);
            if (readNum != sizeof(PVR_INDEX_ENTRY_S))
            {
                perror("read failed:");
                printf("read failed: want%d, get:%d\n", sizeof(PVR_INDEX_ENTRY_S), readNum);
                getchar();
                return 3;
            }
            else
            {
                if (4 != pread(fileTs, scbuf, 4, entry.u64Offset))
                {
                    printf("F%04d: can NOT read TS, offset=0x%llx.\n", i, entry.u64Offset);
                //    return ret + 1;
                }
                else
                {
                    if ((scbuf[0] & 0xffffff) != 0x010000)
                    {
                        printf("F%04d: sc is %#x @ %llx/%llx, size=%d\n", i, scbuf[0], entry.u64Offset,entry.u64SeqHeadOffset, entry.u32FrameSize);
                        //return -1;
                        ret++;
                    }
                    else
                    {
                //        printf("F%04d:  Offset=%llx, size=%d\n", i,   entry.u64Offset, entry.u32FrameSize);
                    }
                }
                sizeCalc = (HI_U32)(entry.u64SeqHeadOffset - lastOffset);
                if (sizeCalc != LastSize)
                {
                    //printf("--F%04d: Size %u != %llx - %llx(%u)\n",i, entry.u32FrameSize, entry.u64SeqHeadOffset, lastOffset, sizeCalc);
                }
                lastOffset = entry.u64SeqHeadOffset;
                LastSize = entry.u32FrameSize;
            }
        }
    }
#endif

    close(fileTs);
    fclose(fpIndex);
    if (ret)
    {
        printf("\n------------End of file. index check failed, err:%d-------\n", ret);
    }
    else
    {
        printf("\n------------End of file. index check ok-------\n");
    }
    return ret;
}
HI_S32 PVR_RecStart(char *path, PMT_COMPACT_PROG *pstProgInfo, HI_U32 u32DemuxID,
            HI_BOOL bRewind, HI_BOOL bDoCipher, HI_U64 maxSize, HI_U32 *pRecChn)
{
    HI_U32 recChn;
    HI_S32 ret = HI_SUCCESS;
    HI_UNF_PVR_REC_ATTR_S   attr;
    HI_U32                  VidPid;
	HI_U32                  AudPid = 0;
    PVR_PROG_INFO_S         fileInfo;
    PVR_PROG_INFO_S         *pfileInfo;
    HI_CHAR                 szFileName[PVR_MAX_FILENAME_LEN];
    attr.u32DemuxID    = u32DemuxID;

    PVR_RecAddPid(attr.u32DemuxID, 0, HI_UNF_DMX_CHAN_TYPE_SEC);
    PVR_RecAddPid(attr.u32DemuxID, pstProgInfo->PmtPid, HI_UNF_DMX_CHAN_TYPE_SEC);
    //PVR_RecAddPid(attr.u32DemuxID, 0x120, HI_UNF_DMX_CHAN_TYPE_SEC);
    //PVR_RecAddPid(attr.u32DemuxID, 0x140, HI_UNF_DMX_CHAN_TYPE_SEC);

	if (pstProgInfo->AElementNum > 0)
	{
		AudPid  = pstProgInfo->AElementPid;
		PVR_RecAddPid(attr.u32DemuxID, AudPid, HI_UNF_DMX_CHAN_TYPE_AUD);
		//PVR_RecAddPid(attr.u32DemuxID, 0x124, HI_UNF_DMX_CHAN_TYPE_AUD);
		//PVR_RecAddPid(attr.u32DemuxID, 0x144, HI_UNF_DMX_CHAN_TYPE_AUD);

		//��������¼�ƽ�����
		CA_AllocateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE);
		CA_AssociateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE,AudPid);

        //CA_AllocateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE);
		//CA_AssociateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE,124);
	}

    if (pstProgInfo->VElementNum > 0 )
	{
		VidPid = pstProgInfo->VElementPid;
        PVR_RecAddPid(attr.u32DemuxID, VidPid, HI_UNF_DMX_CHAN_TYPE_VID);
        //PVR_RecAddPid(attr.u32DemuxID, 0x121, HI_UNF_DMX_CHAN_TYPE_VID);
        //PVR_RecAddPid(attr.u32DemuxID, 0x141, HI_UNF_DMX_CHAN_TYPE_VID);
        attr.u32IndexPid   = VidPid;
        attr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_VIDEO;
        attr.enIndexVidType = pstProgInfo->VideoType;

		//��������¼�ƽ�����
		CA_AllocateDescrambleKey(REC_VIDEO_DESCRAMBLE_TYPE);
		CA_AssociateDescrambleKey(REC_VIDEO_DESCRAMBLE_TYPE,VidPid);
	}
	else
	{
	    attr.u32IndexPid   = AudPid;
        attr.enIndexType   = HI_UNF_PVR_REC_INDEX_TYPE_AUDIO;
        attr.enIndexVidType = HI_UNF_VCODEC_TYPE_BUTT;
	}
#if 1//add by jyshan ��ʱ���汣֤Ŀ¼��Ч����·��Ϊ����·��
    sprintf(attr.szFileName, "%s", path);
#else
    sprintf(szFileName, "rec_v%d_a%d.ts",
                        pstProgInfo->VElementPid,
                        pstProgInfo->AElementPid);

    sprintf(attr.szFileName, "%s/", path);

    strcat(attr.szFileName, szFileName);
#endif
    printf("record file name:%s\n", attr.szFileName);

    attr.u32FileNameLen = strlen(attr.szFileName);
    attr.u32ScdBufSize = PVR_STUB_SC_BUF_SZIE;
    attr.u32DavBufSize = PVR_STUB_TSDATA_SIZE;
    attr.enStreamType  = HI_UNF_PVR_STREAM_TYPE_TS;
    attr.bRewind = bRewind;
    attr.u64MaxFileSize= maxSize;//source;
    attr.u64MaxTimeInMs= 0;
    attr.bIsClearStream = HI_TRUE;
    attr.u32UsrDataInfoSize = sizeof(PVR_PROG_INFO_S) + 100;/*the one in index file is a multipleit of 40 bytes*//*CNcomment:�����ļ�����40���ֽڶ����*/

    attr.stEncryptCfg.bDoCipher = bDoCipher;
    attr.stEncryptCfg.enType = HI_UNF_CIPHER_ALG_AES;
    attr.stEncryptCfg.u32KeyLen = 16;          /*strlen(PVR_CIPHER_KEY)*/
    SAMPLE_RUN(HI_UNF_PVR_RecCreateChn(&recChn, &attr), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    SAMPLE_RUN(HI_UNF_PVR_RecStartChn(recChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_RecDestroyChn(recChn);
        return ret;
    }
    memcpy(&(fileInfo.stProgInfo), pstProgInfo, sizeof(fileInfo.stProgInfo));
    memcpy(&(fileInfo.stRecAttr), &attr, sizeof(fileInfo.stRecAttr));
    pfileInfo = &fileInfo;

    SAMPLE_RUN(PVR_SavePorgInfo(&fileInfo, attr.szFileName), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_RecStopChn(recChn);
        HI_UNF_PVR_RecDestroyChn(recChn);
        return ret;
    }

    *pRecChn = recChn;

    return HI_SUCCESS;
}

HI_S32 PVR_RecStop(HI_U32 u32RecChnID)
{
    HI_S32 ret;
    HI_S32 ret2;
    HI_UNF_PVR_REC_ATTR_S recAttr;


    SAMPLE_RUN(HI_UNF_PVR_RecGetChn(u32RecChnID, &recAttr), ret2) ;
    SAMPLE_RUN(HI_UNF_PVR_RecStopChn(u32RecChnID), ret) ;
    SAMPLE_RUN(HI_UNF_PVR_RecDestroyChn(u32RecChnID), ret) ;
    if (HI_SUCCESS == ret2)
    {
        if (!recAttr.stEncryptCfg.bDoCipher)
        {
            //SAMPLE_RUN(PVR_checkIdx(recAttr.szFileName), ret);
        }
    }

	//����ͷŽ�����
	CA_DisAssociateDescrambleKey(REC_VIDEO_DESCRAMBLE_TYPE);
	CA_DeallocateDescrambleKey(REC_VIDEO_DESCRAMBLE_TYPE);
	CA_DisAssociateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE);
	CA_DeallocateDescrambleKey(REC_AUDIO_DESCRAMBLE_TYPE);

    PVR_RecDelAllPid(PVR_DMX_ID_REC) ;

    return ret;
}


HI_S32 PVR_SwitchDmxSource(HI_U32 dmxId, HI_U32 protId)
{
    HI_S32 ret;

    HI_UNF_DMX_DetachTSPort(dmxId);

    SAMPLE_RUN(HI_UNF_DMX_AttachTSPort(dmxId, protId), ret);

    return ret;
}

HI_S32 PVR_SetAvplayPidAndCodecType(HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    //HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    HI_U32                  VidPid;
	HI_U32                  AudPid;

    HI_UNF_VCODEC_ATTR_S        VdecAttr;
    HI_UNF_ACODEC_ATTR_S        AdecAttr;
//    HI_U32                  i;
    HI_S32                  Ret;
    HI_UNF_VCODEC_TYPE_E    enVidType;
	HI_U32                  u32AudType;

    /*
    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_Stop failed.\n");
        //return Ret;
    }
    */
    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
    HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_ADEC, &AdecAttr);

    if (pProgInfo->VElementNum > 0 )
    {
        VidPid = pProgInfo->VElementPid;
        enVidType = pProgInfo->VideoType;
    }
    else
    {
        VidPid = INVALID_TSPID;
        enVidType = HI_UNF_VCODEC_TYPE_BUTT;
    }

    if (pProgInfo->AElementNum > 0)
    {
        AudPid  = pProgInfo->AElementPid;
        u32AudType = pProgInfo->AudioType;
    }
    else
    {
        AudPid = INVALID_TSPID;
        u32AudType = 0xffffffff;
    }

    if (VidPid != INVALID_TSPID)
    {
        VdecAttr.enType = enVidType;
        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VDEC, &VdecAttr);
        if (Ret != HI_SUCCESS)
        {
            printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
            return Ret;
        }

        Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_VID_PID,&VidPid);
        if (Ret != HI_SUCCESS)
        {
            printf("call HIADP_AVPlay_SetVdecAttr failed.\n");
            return Ret;
        }
    }

    if (AudPid != INVALID_TSPID)
    {
        if (u32AudType == HA_AUDIO_ID_AC3PASSTHROUGH)
        {
            u32AudType = HA_AUDIO_ID_DOLBY_PLUS;
        }
        Ret = HIADP_AVPlay_SetAdecAttr(hAvplay,u32AudType,HD_DEC_MODE_RAWPCM,1);
        Ret |= HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_AUD_PID,&AudPid);
        if (HI_SUCCESS != Ret)
        {
            printf("HIADP_AVPlay_SetAdecAttr failed:%#x\n",Ret);
            return Ret;
        }
    }

    return HI_SUCCESS;
}



/*set audio and video PID attribution,start to play*//*CNcomment:��������ƵPID����,��ʼ����*/
HI_S32 PVR_StartLivePlay(HI_HANDLE hAvplay, const PMT_COMPACT_PROG *pProgInfo)
{
    HI_U32 ret = HI_SUCCESS;

    PVR_SetAvplayPidAndCodecType(hAvplay, pProgInfo);

    /*start to play audio and video*//*CNcomment:��ʼ����Ƶ����*/
    SAMPLE_RUN(HI_UNF_AVPLAY_Start(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD|HI_UNF_AVPLAY_MEDIA_CHAN_VID , NULL), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    return HI_SUCCESS;
}

HI_S32 PVR_StopLivePlay(HI_HANDLE hAvplay)
{
    HI_UNF_AVPLAY_STOP_OPT_S option;

    option.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    option.u32TimeoutMs = 0;

    printf("stop live play ...\n");

    /*stop playing audio and video*//*CNcomment:ֹͣ����Ƶ�豸*/
    return HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &option);
}


/*start playback*//*CNcomment:��ʼ�ط�*/
HI_S32 PVR_StartPlayBack(const HI_CHAR *pszFileName, HI_U32 *pu32PlayChn, HI_HANDLE hAvplay)
{
    HI_S32 ret;
    HI_U32 playChn;
    PVR_PROG_INFO_S        fileInfo;
    HI_UNF_PVR_PLAY_ATTR_S attr;
    PMT_COMPACT_TBL        *pPmttal;
    SAMPLE_CheckNullPTR(pszFileName);
    SAMPLE_CheckNullPTR(pu32PlayChn);

    SAMPLE_RUN(PVR_GetPorgInfo(&fileInfo, pszFileName), ret);
    if (HI_SUCCESS != ret)
    {
        PRINT_SMP("Can NOT get prog INFO, can't play.\n");
        ret = PVR_SearchFile(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK, pszFileName, &pPmttal);
        if(ret != HI_SUCCESS)
        {
            printf("PVR_StartPlayBack PVR_SearchFile fail!:\n");
            return ret;
        }
        PVR_SetAvplayPidAndCodecType(hAvplay, &(pPmttal->proginfo[0]));

        memcpy(attr.szFileName, pszFileName, strlen(pszFileName) + 1);
        attr.u32FileNameLen = strlen(pszFileName);
        attr.enStreamType = HI_UNF_PVR_STREAM_TYPE_TS;
        attr.bIsClearStream = HI_TRUE;
    }
    else
    {
        PVR_SetAvplayPidAndCodecType(hAvplay, &(fileInfo.stProgInfo));

        memcpy(attr.szFileName, pszFileName, strlen(pszFileName) + 1);
        attr.u32FileNameLen = strlen(pszFileName);
        attr.enStreamType = fileInfo.stRecAttr.enStreamType;
        attr.bIsClearStream = fileInfo.stRecAttr.bIsClearStream;
    }

    if (1 == fileInfo.stRecAttr.stEncryptCfg.bDoCipher&&ret==HI_SUCCESS)
    {
        memcpy(&(attr.stDecryptCfg), &(fileInfo.stRecAttr.stEncryptCfg), sizeof(HI_UNF_PVR_CIPHER_S));

        printf("cipher info:\n");
        printf("  enType:%d\n", attr.stDecryptCfg.enType);
    }
    else
    {
        printf("cipher info: not encrypt\n");
        attr.stDecryptCfg.bDoCipher = HI_FALSE;
    }

#if 1//add by jyshan
    ret = HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_DetachTSPort failed.\n");
        HI_UNF_DMX_DeInit();
        return ret;
    }
#endif
    ret = HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE, PVR_DMX_PORT_ID_PLAYBACK);
    if (ret != HI_SUCCESS)
    {
        printf("call HI_UNF_DMX_AttachTSPort failed.\n");
        HI_UNF_DMX_DeInit();
        return ret;
    }

    SAMPLE_RUN(HI_UNF_DMX_CreateTSBuffer(PVR_DMX_PORT_ID_PLAYBACK, 2*1024*1024, &g_hTsBufForPlayBack), ret);
    if (ret != HI_SUCCESS)
    {
        HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE);
        HI_UNF_DMX_DeInit();
        return ret;
    }
    /*create new play channel*//*CNcomment:�����µĲ���ͨ��*/
    SAMPLE_RUN(HI_UNF_PVR_PlayCreateChn(&playChn, &attr, hAvplay, g_hTsBufForPlayBack), ret);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }
    SAMPLE_RUN(HI_UNF_PVR_PlayStartChn(playChn), ret);
    if (HI_SUCCESS != ret)
    {
        HI_UNF_PVR_PlayDestroyChn(playChn);
        return ret;
    }

    *pu32PlayChn = playChn;
    return HI_SUCCESS;
}

/*stop playback*//*CNcomment: ֹͣ�ط� */
HI_VOID PVR_StopPlayBack(HI_U32 playChn)
{
    HI_S32 ret;
    HI_UNF_AVPLAY_STOP_OPT_S stopOpt;
    HI_UNF_PVR_PLAY_ATTR_S PlayAttr;

    stopOpt.enMode = HI_UNF_AVPLAY_STOP_MODE_STILL;
    stopOpt.u32TimeoutMs = 0;
    SAMPLE_RUN(HI_UNF_PVR_PlayGetChn(playChn,&PlayAttr), ret);
    HI_UNF_PVR_PlayStopChn(playChn, &stopOpt);
    HI_UNF_PVR_PlayDestroyChn(playChn);
    SAMPLE_RUN(HI_UNF_DMX_DestroyTSBuffer(g_hTsBufForPlayBack), ret);
    SAMPLE_RUN(HI_UNF_DMX_DetachTSPort(PVR_DMX_ID_LIVE), ret);
#if 1//add by jyshan
    SAMPLE_RUN(HI_UNF_DMX_AttachTSPort(PVR_DMX_ID_LIVE,PVR_DMX_PORT_ID_DVB), ret);
#endif
}

HI_S32 PVR_AvplayInit(HI_HANDLE hWin, HI_HANDLE *phAvplay)
{
    HI_S32                  Ret;
    HI_HANDLE               hAvplay;
    HI_UNF_AVPLAY_ATTR_S        AvplayAttr;
    HI_UNF_SYNC_ATTR_S          SyncAttr;

    Ret = DRV_AvplayerRegADecLib();
    if (Ret != HI_SUCCESS)
    {
        printf("call HIADP_AVPlay_RegADecLib failed.\n");
        return Ret;
    }

  
    Ret = HI_UNF_AVPLAY_Init();
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Init failed.\n");
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetDefaultConfig(&AvplayAttr, HI_UNF_AVPLAY_STREAM_TYPE_TS);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_GetDefaultConfig failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    AvplayAttr.u32DemuxId = PVR_DMX_ID_LIVE;
    Ret = HI_UNF_AVPLAY_Create(&AvplayAttr, &hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_Create failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_GetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_GetAttr failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    SyncAttr.enSyncRef = HI_UNF_SYNC_REF_AUDIO;
    SyncAttr.stSyncStartRegion.s32VidPlusTime = 100;
    SyncAttr.stSyncStartRegion.s32VidNegativeTime = -100;
    SyncAttr.stSyncStartRegion.bSmoothPlay = HI_TRUE;
    SyncAttr.bQuickOutput = HI_TRUE;
    SyncAttr.u32PreSyncTimeoutMs = 600;

    Ret = HI_UNF_AVPLAY_SetAttr(hAvplay, HI_UNF_AVPLAY_ATTR_ID_SYNC, &SyncAttr);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_SetAttr failed.\n");
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_AVPLAY_ChnOpen(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD, HI_NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_AVPLAY_ChnOpen failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        return Ret;
    }

    Ret = HI_UNF_VO_AttachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_VO_AttachWindow failed.\n");
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_VO_SetWindowEnable(hWin, HI_TRUE);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_VO_SetWindowEnable failed.\n");
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    Ret = HI_UNF_SND_Attach(HI_UNF_SND_0, hAvplay, HI_UNF_SND_MIX_TYPE_MASTER, 100);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_SND_Attach failed.\n");
        HI_UNF_VO_SetWindowEnable(hWin, HI_FALSE);
        HI_UNF_VO_DetachWindow(hWin, hAvplay);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
        HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
        HI_UNF_AVPLAY_Destroy(hAvplay);
        HI_UNF_AVPLAY_DeInit();
        return Ret;
    }

    *phAvplay = hAvplay;
    return HI_SUCCESS;
}

HI_S32  PVR_AvplayDeInit(HI_HANDLE hAvplay, HI_HANDLE hWin)
{
    HI_S32                      Ret;
    HI_UNF_AVPLAY_STOP_OPT_S    Stop;

    Stop.enMode = HI_UNF_AVPLAY_STOP_MODE_BLACK;
    Stop.u32TimeoutMs = 0;

    Ret = HI_UNF_AVPLAY_Stop(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID | HI_UNF_AVPLAY_MEDIA_CHAN_AUD, &Stop);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_Stop failed.\n");
    }

    Ret = HI_UNF_SND_Detach(HI_UNF_SND_0, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_SND_Detach failed.\n");
    }

    Ret = HI_UNF_VO_DetachWindow(hWin, hAvplay);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_VO_DetachWindow failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_VID);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_ChnClose(hAvplay, HI_UNF_AVPLAY_MEDIA_CHAN_AUD);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_ChnClose failed.\n");
    }

    Ret = HI_UNF_AVPLAY_Destroy(hAvplay);
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_Destroy failed.\n");
    }

    Ret = HI_UNF_AVPLAY_DeInit();
    if (Ret != HI_SUCCESS )
    {
        printf("call HI_UNF_AVPLAY_DeInit failed.\n");
    }

    return HI_SUCCESS;
}
extern DRV_PlayRecCallback mPlayRecCallback;
HI_VOID PVR_CallBack(HI_U32 u32ChnID, HI_UNF_PVR_EVENT_E EventType, HI_S32 s32EventValue, HI_VOID *args)
{
	extern BOOL mIsTimeShiftStart;
    printf("\n==============call back================\n");
    
    if (EventType > HI_UNF_PVR_EVENT_REC_RESV)
    {
        printf("====callback error!!!\n");
        return;
    }

    printf("====channel     %d\n", u32ChnID);
    printf("====event:%s    %d\n", PVR_GetEventTypeStringByID(EventType), EventType);
    printf("====event value %d\n", s32EventValue);


    if (EventType == HI_UNF_PVR_EVENT_PLAY_EOF)
    {
        printf("==========play to end of file======\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_REACH_END_OF_PLAYBACK_EVT,NULL);	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_SOF)
    {
        printf("==========play to start of file======\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_REACH_BEGIN_OF_PLAYBACK_EVT,NULL);	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_ERROR)
    {
        printf("==========play internal error, check if the disk is insert to the box======\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_PLAY_FILE_NOT_FOUND_EVT,NULL);	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_PLAY_REACH_REC)
    {
        printf("==========play reach to record ======\n");
        if(mPlayRecCallback != NULL)
        {        	
        	mPlayRecCallback(DRV_PLAYREC_PLAY_REACH_RECORD_EVT,NULL);
        }
    }

    if (EventType == HI_UNF_PVR_EVENT_REC_DISKFULL)
    {
        printf("\n====disk full,  stop record=====\n\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_DISK_LOW_LEVEL_EVT,NULL);	
        }

        HI_UNF_PVR_RecStopChn(u32ChnID);
        g_bIsRecStop = HI_TRUE;
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_ERROR)
    {
        printf("======disk write error, please check if the disk is insert to the box.====\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_RECORD_WRITE_ERROR_EVT,NULL);	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_OVER_FIX)
    {
        printf("\n======reach the fixed size.==========\n\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_REACH_END_OF_FILE_EVT,NULL);
        	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_REACH_PLAY)
    {
        printf("\n======record reach to play.==========\n\n");
        if((mIsTimeShiftStart)&&(mPlayRecCallback != NULL))
        {
        	//mPlayRecCallback(DRV_PLAYREC_REACH_END_OF_FILE_EVT,NULL);	
        }
    }
    if (EventType == HI_UNF_PVR_EVENT_REC_DISK_SLOW)
    {
        printf("======disk is too slow, the stream record would be error.====\n");
        if(mPlayRecCallback != NULL)
        {
        	mPlayRecCallback(DRV_PLAYREC_RECORD_WRITE_ERROR_EVT,NULL);	
        }
    }

    printf("=======================================\n\n");

    return;
}

HI_S32 PVR_RegisterCallBacks(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_EOF, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_SOF, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_ERROR, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISKFULL, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_OVER_FIX, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_DISK_SLOW, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY, PVR_CallBack, NULL);
    Ret |= HI_UNF_PVR_RegisterEvent(HI_UNF_PVR_EVENT_REC_ERROR, PVR_CallBack, NULL);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_RegisterEvent failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}
HI_S32 PVR_UnRegisterCallBacks(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_PLAY_EOF);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_PLAY_SOF);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_PLAY_ERROR);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_PLAY_REACH_REC);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_DISKFULL);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_OVER_FIX);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_DISK_SLOW);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_REACH_PLAY);
    Ret |= HI_UNF_PVR_UnRegisterEvent(HI_UNF_PVR_EVENT_REC_ERROR);
    if (Ret != HI_SUCCESS)
    {
        printf("call HI_UNF_PVR_UnRegisterEvent failed.\n");
        return Ret;
    }

    return HI_SUCCESS;
}


HI_U8* PVR_GetEventTypeStringByID(HI_UNF_PVR_EVENT_E eEventID)
{
    HI_U32 nNum = sizeof(g_stEventType)/sizeof(g_stEventType[0]);
    HI_U32 nIndex = 0;
    HI_U8* pszRet = HI_NULL;

    for(nIndex=0; nIndex<nNum; nIndex++)
    {
        if( eEventID == g_stEventType[nIndex].eEventID)
        {
            pszRet = g_stEventType[nIndex].szEventTypeName;
            break;
        }
    }

    if(nIndex == nNum)
    {
       pszRet = g_stEventType[nNum-1].szEventTypeName;
    }

    return pszRet;
}



