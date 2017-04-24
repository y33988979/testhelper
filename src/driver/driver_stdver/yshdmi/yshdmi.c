#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "hi_unf_hdmi.h"
#include "hi_unf_disp.h"
#include "drv_disp.h"
#include "yshdmi.h"

#ifdef DRV_HdmiSwitch
#define ITE6633E_T
#define HdmiSwitchi2cnum	1
#define SWADR	 0x94	//JohnYen set for HDMI switch I2C device address

#define PORT_A 0x00
#define PORT_B 0x05
#define PORT_C 0x0A
#define SEL_PORT_REG		0x06

/* HDMISwitchHotplugin detcet引脚 */
#define GPIO_HDMISwitchHotplogin (1*8+5)
#define uart1_gpio_usb1_i2c1_IT6633 0x28
#define uart1_gpio_usb0 0x2c

DRV_HDMI_SwitchCallback	HDMI_Switchplug_cb=NULL;
HI_U32	g_uart1_gpio_usb1_i2c1_IT6633RegValue=0;
HI_U32	g_uart1_gpio_usb0_DETECTIONRegValue=0;
#endif

typedef struct hiHDMI_ARGS_S
{
    HI_UNF_HDMI_ID_E  enHdmi;
    HI_UNF_ENC_FMT_E  enWantFmt;
    HI_UNF_SAMPLE_RATE_E AudioFreq;
    HI_U32            AduioBitPerSample;
    HI_U32            AudioChannelNum;
    HI_UNF_SND_INTERFACE_E AudioInputType;
    HI_U32            ForceHDMIFlag;
}HDMI_ARGS_S;

static HDMI_ARGS_S g_stHdmiArgs;
HI_U32 g_HDCPFlag         = HI_FALSE;
HI_U32 g_AudioFreq        = HI_UNF_SAMPLE_RATE_48K;
HI_U32 g_AduioBitPerSample= 16;
HI_U32 g_AudioChannelNum  = 2;
HI_U32 g_AudioInputType   = HI_UNF_SND_INTERFACE_I2S;//HI_UNF_SND_INTERFACE_I2S;//HI_UNF_SND_INTERFACE_I2S;//HI_UNF_SND_INTERFACE_SPDIF;
HI_U32 g_Audiotype        = 0;
HI_U32 g_ForceDVIFlag     = HI_FALSE;
HI_U32 g_HDMI_Bebug       = HI_FALSE;
HI_U32 g_HDMIUserCallbackFlag = HI_FALSE;
HI_U32 g_HDMIForceMode    = HI_UNF_HDMI_FORCE_HDMI;//HI_UNF_HDMI_FORCE_NULL;
HI_U32 g_CompressFlag     = HI_FALSE;

User_HDMI_CallBack pfnHdmiUserCallback = NULL;


static HI_CHAR *g_pDispFmtString[HI_UNF_ENC_FMT_BUTT+1] = {
    "1080P_60", 
    "1080P_50", 
    "1080P_30", 
    "1080P_25",

    "1080P_24",        
    "1080i_60",        
    "1080i_50",        

    "720P_60",         
    "720P_50",         

    "576P_50",         
    "480P_60",         

    "PAL",             
    "PAL_N",           
    "PAL_Nc",          

    "NTSC",            
    "NTSC_J",          
    "NTSC_PAL_M",      

    "SECAM_SIN",       
    "SECAM_COS",
    
    "BUTT"
};

HI_UNF_ENC_FMT_E stringToUnfFmt(HI_CHAR *pszFmt)
{
    HI_S32 i;
    HI_UNF_ENC_FMT_E fmtReturn = HI_UNF_ENC_FMT_BUTT;

    if (NULL == pszFmt)
    {
        return HI_UNF_ENC_FMT_BUTT;
    }

    for (i = 0; i < HI_UNF_ENC_FMT_BUTT; i++)
    {
        if (strcasestr(pszFmt, g_pDispFmtString[i]))
    	{
    		fmtReturn = i;        
    		break;
    	}
    }

    if (i >= HI_UNF_ENC_FMT_BUTT)
    {
        i = HI_UNF_ENC_FMT_720P_50;
        fmtReturn = i;   
        printf("\n!!! Can NOT match format, set format to is '%s'/%d.\n\n", g_pDispFmtString[i], i);
    }
    else
    {
	    printf("\n!!! The format is '%s'/%d.\n\n", g_pDispFmtString[i], i);
    }
	return fmtReturn;
}

HI_VOID HDMI_PrintSinkCap(HI_UNF_HDMI_SINK_CAPABILITY_S *pCapbility)
{
    HI_S32 i;

    if (!pCapbility->bConnected)
    {
        printf("\nHDMI Sink disconnected!\n");
        return;
    }

    
    printf("\nHDMI Sink connected!  Sink type is '%s'!\n", pCapbility->bSupportHdmi ? "HDMI" : "DVI");
    
    printf("\nHDMI Sink video cap :\n");
        
    for (i = 0; i < HI_UNF_ENC_FMT_BUTT; i++)
    {
        printf("%d, ", pCapbility->bVideoFmtSupported[i]);
    }
    printf("---END.\n");
    
    printf("Video native format: %d\n", pCapbility->enNativeVideoFormat);
    
    printf("\nHDMI Sink audio cap :\n");
        
    for (i = 0; i < HI_UNF_HDMI_MAX_AUDIO_CAP_COUNT; i++)
    {
        printf("%d, ", pCapbility->bAudioFmtSupported[i]);
    }
    printf("---END.\n");
    
    printf("Max Audio PCM channels: %d\n", pCapbility->u32MaxPcmChannels);
}

HI_VOID HDMI_SetAudioCompress(HI_U32 CompressFlag)
{
    if(CompressFlag == HI_TRUE)
    {
        g_CompressFlag = HI_TRUE;
        g_stHdmiArgs.AudioInputType = HI_UNF_SND_INTERFACE_SPDIF;
    }
    else
    {
        g_CompressFlag = HI_FALSE;
        g_stHdmiArgs.AudioInputType = HI_UNF_SND_INTERFACE_I2S;
    }
}

static HI_VOID HDMI_PrintAttr(HI_UNF_HDMI_ATTR_S *pstHDMIAttr)
{
    if (HI_TRUE != g_HDMI_Bebug)
    {
        return;
    }
    
    printf("=====HI_UNF_HDMI_SetAttr=====\n"
           "bEnableHdmi:%d\n"
           "bEnableVideo:%d\n"
           "enVideoFmt:%d\n\n"
           "enVidOutMode:%d\n"
           "enDeepColorMode:%d\n"
           "bxvYCCMode:%d\n\n"     
           "bEnableAudio:%d\n"   
           "eSoundIntf;%d\n"    
           "bIsMultiChannel:%d\n"
           "enSampleRate:%d\n\n"
           "enBitDepth:%d\n"
           "u8I2SCtlVbit:%d\n\n"
           "bEnableAviInfoFrame:%d\n"
           "bEnableAudInfoFrame:%d\n"
           "bEnableSpdInfoFrame:%d\n"
           "bEnableMpegInfoFrame:%d\n\n"
           "bDebugFlag:%d\n"
           "==============================\n",
           pstHDMIAttr->bEnableHdmi,
           pstHDMIAttr->bEnableVideo,pstHDMIAttr->enVideoFmt,
           pstHDMIAttr->enVidOutMode,pstHDMIAttr->enDeepColorMode,pstHDMIAttr->bxvYCCMode,
           pstHDMIAttr->bEnableAudio,pstHDMIAttr->enSoundIntf,pstHDMIAttr->bIsMultiChannel,pstHDMIAttr->enSampleRate,
           pstHDMIAttr->enBitDepth,pstHDMIAttr->u8I2SCtlVbit,
           pstHDMIAttr->bEnableAudInfoFrame,pstHDMIAttr->bEnableAudInfoFrame,
           pstHDMIAttr->bEnableSpdInfoFrame,pstHDMIAttr->bEnableMpegInfoFrame,
           pstHDMIAttr->bDebugFlag);
    return;
}

void HDMI_HotPlug_Proc(HI_VOID *pPrivateData)
{
    HI_S32          ret = HI_SUCCESS;
    HDMI_ARGS_S     *pArgs  = (HDMI_ARGS_S*)pPrivateData;
    HI_HANDLE       hHdmi   =  pArgs->enHdmi;
    HI_UNF_HDMI_ATTR_S             stHdmiAttr;
    HI_UNF_HDMI_INFOFRAME_S        stInfoFrame;
    HI_UNF_HDMI_SINK_CAPABILITY_S  stSinkCap;
    HI_UNF_ENC_FMT_E               enTargetFmt;
    HI_UNF_HDMI_COLORSPACE_E       enColorimetry;
    HI_UNF_HDMI_VIDEO_MODE_E       enVidOutMode;
    HI_UNF_ASPECT_RATIO_E          enAspectRate;
    HI_U32                         Audio_Freq = 0;
    
    HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *pstAVIInfoframe;
    HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *pstAUDInfoframe;
    static HI_U8 u8FirstTimeSetting = HI_TRUE;
   
    printf("\n --- Get HDMI event: HOTPLUG. --- \n"); 

    ret = HI_UNF_HDMI_GetSinkCapability(hHdmi, &stSinkCap); 
    //HDMI_PrintSinkCap(&stSinkCap);
    if (HI_FALSE == stSinkCap.bConnected)
    {
        printf("No Connect\n");
        return;
    }
    
    ret = HI_UNF_HDMI_GetAttr(hHdmi, &stHdmiAttr);
    HI_UNF_DISP_GetFormat(HI_UNF_DISP_HD0, &enTargetFmt); /* set display's format to targetFormat */
    stHdmiAttr.enVideoFmt = enTargetFmt;

    if (HI_TRUE != stSinkCap.bVideoFmtSupported[enTargetFmt])
    {
        printf("\n***Warrring:From EDID, Sink CAN NOT receive this format:%d***\n", enTargetFmt);
    }
  
    if (HI_TRUE == stSinkCap.bSupportYCbCr)
    {
        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
    }
    else
    {
        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }

    //enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;printf("force to RGB444\n");
    //enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;printf("force to YCBCR444\n");
    //enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR422;printf("force to YCBCR422\n");
    
    enColorimetry = HDMI_COLORIMETRY_ITU709;
    enAspectRate  = HI_UNF_ASPECT_RATIO_16TO9;
    
    if (HI_TRUE == stSinkCap.bSupportHdmi)
    {
        /* New function to set AVI Infoframe */
        //printf("*** SetInfoFrame for AVI Infoframe\n");        
        memset(&stInfoFrame, 0, sizeof(stInfoFrame));
        HI_UNF_HDMI_GetInfoFrame(hHdmi, HI_INFOFRAME_TYPE_AVI, &stInfoFrame);
        stInfoFrame.enInfoFrameType = HI_INFOFRAME_TYPE_AVI;
        pstAVIInfoframe = (HI_UNF_HDMI_AVI_INFOFRAME_VER2_S *)&(stInfoFrame.unInforUnit.stAVIInfoFrame);

        pstAVIInfoframe->u32PixelRepetition      = HI_FALSE;

        if(HI_UNF_ENC_FMT_1080P_60 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080P_60;
        }
        else if(HI_UNF_ENC_FMT_1080P_50 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080P_50;
        }
        else if(HI_UNF_ENC_FMT_1080P_30 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080P_30;
        }
        else if(HI_UNF_ENC_FMT_1080P_25 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080P_25;
        }
        else if(HI_UNF_ENC_FMT_1080P_24 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080P_24;
        }
        else if(HI_UNF_ENC_FMT_1080i_60 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080i_60;
        }
        else if(HI_UNF_ENC_FMT_1080i_50 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_1080i_50;
        }
        else if(HI_UNF_ENC_FMT_720P_60 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_720P_60;
        }
        else if(HI_UNF_ENC_FMT_720P_50 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_720P_50;
        }
        else if(HI_UNF_ENC_FMT_576P_50 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_576P_50;
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            if (pstAVIInfoframe->enAspectRatio == HI_UNF_ASPECT_RATIO_16TO9)
            {
                enAspectRate = HI_UNF_ASPECT_RATIO_16TO9;
            }
        }
        else if(HI_UNF_ENC_FMT_480P_60 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_480P_60;
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            if (pstAVIInfoframe->enAspectRatio == HI_UNF_ASPECT_RATIO_16TO9)
            {
                enAspectRate = HI_UNF_ASPECT_RATIO_16TO9;
            }
        }
        else if(HI_UNF_ENC_FMT_PAL == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_PAL;
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            if (pstAVIInfoframe->enAspectRatio == HI_UNF_ASPECT_RATIO_16TO9)
            {
                enAspectRate = HI_UNF_ASPECT_RATIO_16TO9;
            }
            pstAVIInfoframe->u32PixelRepetition = HI_TRUE;
        }
        else if(HI_UNF_ENC_FMT_NTSC == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_NTSC;
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            if (pstAVIInfoframe->enAspectRatio == HI_UNF_ASPECT_RATIO_16TO9)
            {
                enAspectRate = HI_UNF_ASPECT_RATIO_16TO9;
            }
            pstAVIInfoframe->u32PixelRepetition = HI_TRUE;
        }
        else if(HI_UNF_ENC_FMT_861D_640X480_60 == enTargetFmt)
        {
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_861D_640X480_60;
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            enVidOutMode  = HI_UNF_HDMI_VIDEO_MODE_RGB444;//force to use RGB444 Format
        }
        else if ((HI_UNF_ENC_FMT_VESA_800X600_60 <= enTargetFmt) && (HI_UNF_ENC_FMT_VESA_2048X1152_60 >= enTargetFmt))
        {
            printf("Warring:DVI timing mode enTimingMode:0x%x\n", pstAVIInfoframe->enTimingMode);
            pstAVIInfoframe->enTimingMode = HI_UNF_ENC_FMT_861D_640X480_60; //AVI is useless in DVI format
            enColorimetry = HDMI_COLORIMETRY_ITU601;
            enAspectRate  = HI_UNF_ASPECT_RATIO_4TO3;
            enVidOutMode  = HI_UNF_HDMI_VIDEO_MODE_RGB444;//force to use RGB444 Format       
        }
        else
        {
            printf("***Error: Unsupport Video timing:0x%x\n***", enTargetFmt);
        }
        /* Deal with xvYCC MODE */
        if (HI_TRUE == stHdmiAttr.bxvYCCMode)
        {
            if (HDMI_COLORIMETRY_ITU601 == enColorimetry)
            {
                enColorimetry = HDMI_COLORIMETRY_XVYCC_601;
            }
            else if (HDMI_COLORIMETRY_ITU709 == enColorimetry)
            {
                enColorimetry = HDMI_COLORIMETRY_XVYCC_709;
            }
        }
        pstAVIInfoframe->enOutputType            = enVidOutMode;
        pstAVIInfoframe->bActive_Infor_Present   = HI_TRUE;
        pstAVIInfoframe->enBarInfo               = HDMI_BAR_INFO_NOT_VALID;
        pstAVIInfoframe->enScanInfo              = HDMI_SCAN_INFO_NO_DATA;//HDMI_SCAN_INFO_OVERSCANNED;
        pstAVIInfoframe->enColorimetry           = enColorimetry;
        pstAVIInfoframe->enAspectRatio           = enAspectRate;
        pstAVIInfoframe->enActiveAspectRatio     = enAspectRate;
        pstAVIInfoframe->enPictureScaling        = HDMI_PICTURE_NON_UNIFORM_SCALING;
        pstAVIInfoframe->enRGBQuantization       = HDMI_RGB_QUANTIZATION_DEFAULT_RANGE;
        pstAVIInfoframe->bIsITContent            = HI_FALSE;
        pstAVIInfoframe->u32LineNEndofTopBar     = 0;  /* We can determine it in hi_unf_hdmi.c */
        pstAVIInfoframe->u32LineNStartofBotBar   = 0;  /* We can determine it in hi_unf_hdmi.c */
        pstAVIInfoframe->u32PixelNEndofLeftBar   = 0;  /* We can determine it in hi_unf_hdmi.c */
        pstAVIInfoframe->u32PixelNStartofRightBar= 0;  /* We can determine it in hi_unf_hdmi.c */
        ret = HI_UNF_HDMI_SetInfoFrame(hHdmi, &stInfoFrame);
        
        /* New function to set Audio Infoframe */
        /* HDMI requires the CT, SS and SF fields to be set to 0 ("Refer to Stream Header") 
           as these items are carried in the audio stream.*/
        //printf("*** SetInfoFrame for AUDIO Infoframe\n");
        memset(&stInfoFrame, 0, sizeof(stInfoFrame));
        stInfoFrame.enInfoFrameType = HI_INFOFRAME_TYPE_AUDIO;
        pstAUDInfoframe = (HI_UNF_HDMI_AUD_INFOFRAME_VER1_S *)&(stInfoFrame.unInforUnit.stAUDInfoFrame);
        pstAUDInfoframe->u32ChannelCount      = g_stHdmiArgs.AudioChannelNum; //maybe 2 or 8;
        pstAUDInfoframe->enCodingType         = HDMI_AUDIO_CODING_REFER_STREAM_HEAD;
        pstAUDInfoframe->u32SampleSize        = HI_UNF_HDMI_DEFAULT_SETTING;
        //pstAUDInfoframe->u32SamplingFrequency = HI_UNF_HDMI_DEFAULT_SETTING;
        
        HI_UNF_SND_GetSampleRate(HI_UNF_SND_0, &Audio_Freq);
        //printf("SPDIF Audio_Freq:%d\n", Audio_Freq);
        if(g_stHdmiArgs.AudioInputType == HI_UNF_SND_INTERFACE_I2S)
        {
            pstAUDInfoframe->u32SamplingFrequency = HI_UNF_HDMI_DEFAULT_SETTING;
        }
        else
        {
            pstAUDInfoframe->u32SamplingFrequency = Audio_Freq;
        }
        pstAUDInfoframe->u32ChannelAlloc      = 0;
        pstAUDInfoframe->u32LevelShift        = 0;
        pstAUDInfoframe->u32DownmixInhibit    = HI_FALSE;
        
        if(g_CompressFlag == HI_TRUE)
        {
            //Only for special test
            printf("special compress test\n");
            HI_UNF_HDMI_GetInfoFrame(hHdmi, HI_INFOFRAME_TYPE_AUDIO, &stInfoFrame);
        }
        
        ret = HI_UNF_HDMI_SetInfoFrame(hHdmi, &stInfoFrame);

        stHdmiAttr.bEnableHdmi         = HI_TRUE;
        stHdmiAttr.bEnableAviInfoFrame = HI_TRUE;
        stHdmiAttr.bEnableAudInfoFrame = HI_TRUE;
        
        /* Default we set audio below parm */
        stHdmiAttr.bEnableVideo       = HI_TRUE;
        stHdmiAttr.bEnableAudio       = HI_TRUE;
        stHdmiAttr.bIsMultiChannel    = HI_FALSE;
        stHdmiAttr.enSampleRate       = Audio_Freq;//g_stHdmiArgs.AudioFreq;
        stHdmiAttr.enBitDepth         = g_stHdmiArgs.AduioBitPerSample;
        stHdmiAttr.enSoundIntf        = g_stHdmiArgs.AudioInputType;
        stHdmiAttr.u8I2SCtlVbit       = HI_FALSE;
    }
    else
    {
        stHdmiAttr.bEnableHdmi         = HI_FALSE;
        stHdmiAttr.bEnableVideo        = HI_TRUE;
        stHdmiAttr.bEnableAudio        = HI_FALSE;
        stHdmiAttr.bEnableAviInfoFrame = HI_FALSE;
        stHdmiAttr.bEnableAudInfoFrame = HI_FALSE;      
        printf("DVI use output format:RGB444 \n");
        enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
    }
    
    //stHdmiAttr.bEnableHdmi        = HI_TRUE;//HI_FALSE;//HI_TRUE;  // HI_FALSE means DVI 

    /* Default we set video below parm */
    stHdmiAttr.enVideoFmt         = enTargetFmt;
    stHdmiAttr.enVidOutMode       = enVidOutMode;
    //DeepColor should use former setting!
    //stHdmiAttr.enDeepColorMode    = HI_UNF_HDMI_DEEP_COLOR_OFF;
    /* Other inforframe Flag */
    stHdmiAttr.bEnableSpdInfoFrame = HI_FALSE;
    stHdmiAttr.bEnableMpegInfoFrame= HI_FALSE;
    stHdmiAttr.bDebugFlag          = g_HDMI_Bebug;
    
    if (u8FirstTimeSetting == HI_TRUE)
    {
        u8FirstTimeSetting = HI_FALSE;
        if (g_HDCPFlag == HI_TRUE)
        {
            stHdmiAttr.bHDCPEnable = HI_TRUE;//Enable HDCP
        }
        else
        {
            stHdmiAttr.bHDCPEnable= HI_FALSE;
        }
    }
    else
    {
        //HDCP Enable use default setting!!
    }
    
    HDMI_PrintAttr(&stHdmiAttr);
    ret = HI_UNF_HDMI_SetAttr(hHdmi, &stHdmiAttr);
    /* HI_UNF_HDMI_SetAttr must before HI_UNF_HDMI_Start! */
    ret = HI_UNF_HDMI_Start(hHdmi);
    
    return;
}

HI_VOID HDMI_UnPlug_Proc(HI_VOID *pPrivateData)
{
    HDMI_ARGS_S     *pArgs  = (HDMI_ARGS_S*)pPrivateData;
    HI_HANDLE       hHdmi   =  pArgs->enHdmi;
    
    printf("\n --- Get HDMI event: UnPlug. --- \n");
    HI_UNF_HDMI_Stop(hHdmi);
    return;
}
static HI_U32 HDCPFailCount = 0;
HI_VOID HDMI_HdcpFail_Proc(HI_VOID *pPrivateData)
{
    printf("\n --- Get HDMI event: HDCP_FAIL. --- \n");
    
    HDCPFailCount ++ ;
    if(HDCPFailCount >= 50)
    {
        HDCPFailCount = 0;
        printf("\nWarrning:Customer need to deal with HDCP Fail!!!!!!\n");
    }
    return;
}

HI_VOID HDMI_HdcpSuccess_Proc(HI_VOID *pPrivateData)
{
    printf("\n --- Get HDMI event: HDCP_SUCCESS. --- \n");
    return;
}

HI_VOID HDMI_Event_Proc(HI_UNF_HDMI_EVENT_TYPE_E event, HI_VOID *pPrivateData)
{
    
    switch ( event )
    {
        case HI_UNF_HDMI_EVENT_HOTPLUG:
            HDMI_HotPlug_Proc(pPrivateData);
            break;
        case HI_UNF_HDMI_EVENT_NO_PLUG:
            HDMI_UnPlug_Proc(pPrivateData);
            break;
        case HI_UNF_HDMI_EVENT_EDID_FAIL:
            break;
        case HI_UNF_HDMI_EVENT_HDCP_FAIL:
            HDMI_HdcpFail_Proc(pPrivateData);
            break;
        case HI_UNF_HDMI_EVENT_HDCP_SUCCESS:
            HDMI_HdcpSuccess_Proc(pPrivateData);
            break;
        default:
            break;
    }
    /* Private Usage */
    if ((g_HDMIUserCallbackFlag == HI_TRUE) && (pfnHdmiUserCallback != NULL))
    {
        pfnHdmiUserCallback(event, NULL);
    }
    
    return;
}

/*
hotplug:0x10203030 0x2 or 0x10203060 0x3
cec：0x1020303c 0x2 or 0x1020305c 0x3
scl&sda:0x10203044 0x1
*/
HI_VOID HDMI_PinConfig(HI_VOID)
{
     return ;
    //Pilot
    HI_SYS_WriteRegister(0x10203014, 0x2);
    HI_SYS_WriteRegister(0x10203018, 0x2);
    HI_SYS_WriteRegister(0x10203020, 0x1);        

}

HI_S32 HIADP_HDMI_SetAdecAttr(HI_UNF_SND_INTERFACE_E enInterface, HI_UNF_SAMPLE_RATE_E enRate)
{
    if (enRate)
    {
        g_AudioFreq = enRate;
    }

    if (enInterface)
    {
        g_AudioInputType = enInterface;
    }
    return 0;
}

HI_S32 HIADP_HDMI_Init(HI_UNF_HDMI_ID_E enHDMIId, HI_UNF_ENC_FMT_E enWantFmt)
{
    HI_U32 Ret = HI_FAILURE;
    HI_UNF_HDMI_INIT_PARA_S stHdmiInitParam;
    
    HDMI_PinConfig();
    
    g_stHdmiArgs.enHdmi       = enHDMIId;
    g_stHdmiArgs.enWantFmt    = enWantFmt;
    if(g_AudioFreq == HI_UNF_SAMPLE_RATE_UNKNOWN)
    {
        g_stHdmiArgs.AudioFreq         = HI_UNF_SAMPLE_RATE_48K;
        g_stHdmiArgs.AduioBitPerSample = 16;
        g_stHdmiArgs.AudioChannelNum   = 2;
        g_stHdmiArgs.AudioInputType    = HI_UNF_SND_INTERFACE_I2S;//HI_UNF_SND_INTERFACE_I2S;//HI_UNF_SND_INTERFACE_SPDIF;
        g_stHdmiArgs.ForceHDMIFlag     = HI_FALSE;
    }
    else
    {
        g_stHdmiArgs.AudioFreq         = g_AudioFreq;
        g_stHdmiArgs.AduioBitPerSample = g_AduioBitPerSample;
        g_stHdmiArgs.AudioChannelNum   = g_AudioChannelNum;
        g_stHdmiArgs.AudioInputType    = g_AudioInputType;
        g_stHdmiArgs.ForceHDMIFlag     = HI_FALSE;
    }
    
    stHdmiInitParam.pfnHdmiEventCallback = HDMI_Event_Proc;
    stHdmiInitParam.pCallBackArgs        = &g_stHdmiArgs;
    stHdmiInitParam.enForceMode          = g_HDMIForceMode;//HI_UNF_HDMI_FORCE_NULL;
    printf("HDMI Init Mode:%d\n", stHdmiInitParam.enForceMode);
    Ret = HI_UNF_HDMI_Init(&stHdmiInitParam);
    if (HI_SUCCESS != Ret)
    {
        printf("HI_UNF_HDMI_Init failed:%#x\n",Ret);
        return HI_FAILURE;
    }

    Ret = HI_UNF_HDMI_Open(enHDMIId);
    if (Ret != HI_SUCCESS)
    {
        printf("HI_UNF_HDMI_Open failed:%#x\n",Ret);
        HI_UNF_HDMI_DeInit();
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}


HI_S32 HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_E enHDMIId)
{
    HI_UNF_HDMI_Stop(enHDMIId);
    HI_UNF_HDMI_Close(enHDMIId);
    HI_UNF_HDMI_DeInit();

    return HI_SUCCESS;
}
DRV_ErrCode DRV_HdmiInit(void)
{
	DRV_ErrCode ErrCode;
	ErrCode = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0,mEncFormat);
	return ErrCode;
}
DRV_ErrCode DRV_HdmiTerm(void)
{
	DRV_ErrCode ErrCode;
	ErrCode = HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_0);
	return ErrCode;
}

 DRV_ErrCode DRV_HdmiEdidSurpportDD(void)
{
	HI_UNF_HDMI_SINK_CAPABILITY_S  sinkCap;
	HI_U32                         RetError = HI_SUCCESS;

	//printf("hdmi_display_edid enter with HI_UNF_HDMI_GetSinkCapability\n");
	RetError = HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0, &sinkCap);
	if (RetError != HI_SUCCESS)
	{
		printf("can not get HdmiEdid sink Attribute\n");
		return HI_FAILURE;
	}

	if(sinkCap.bIsRealEDID)
	{
		if(sinkCap.bAudioFmtSupported[10])
		{
			printf("hdmi_display_edid sinkCap.bAudioFmtSupported[10][%d] support DD+\n",sinkCap.bAudioFmtSupported[10]);
			return HI_SUCCESS;
		}
		else
			printf("hdmi_display_edid sinkCap.bAudioFmtSupported[10][%d] not support DD+\n",sinkCap.bAudioFmtSupported[10]);
	}
	else
		printf("HdmiEdid  sink get edid erro,sinkCap.bIsRealEDID[%d],\n",sinkCap.bIsRealEDID);
	
	return HI_FAILURE;
}

#ifdef DRV_HdmiSwitch
/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void ConfigGpioUart1_Gpio_Usb1_I2c1PinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + uart1_gpio_usb1_i2c1_IT6633), &g_uart1_gpio_usb1_i2c1_IT6633RegValue); /*store old pin function define */
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb1_i2c1_IT6633), 0x03);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}
static void RestoreGpioUart1_Gpio_Usb1_I2c1PinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb1_i2c1_IT6633), g_uart1_gpio_usb1_i2c1_IT6633RegValue);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

static void ConfigGpioUart1_Gpio_Usb0PinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + uart1_gpio_usb0), &g_uart1_gpio_usb0_DETECTIONRegValue); /*store old pin function define */
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb0), 0x00);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}
static void RestoreGpioUart1_Gpio_Usb0PinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb0), g_uart1_gpio_usb0_DETECTIONRegValue);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

/****************************************************************************/
/*                       I2C write & read 8 bits data                       */
/****************************************************************************/
// PC_SCL = P1_4	--I2C1_SCL
// PC_SDA = P1_2	--I2C1_SDA
unsigned char HDMI_ReadI2C_Byte(unsigned char RegAddr)
{
	DRV_ErrCode ErrCode;
	unsigned char d;
	/* write I2C */
	ErrCode = HI_UNF_I2C_Read(HdmiSwitchi2cnum, SWADR, RegAddr, 1, &d, 1);
	if (HI_SUCCESS != ErrCode)
	{
	    printf("HI_UNF_I2C_Read() fail,RegAddr[%x],d[%x]\n",RegAddr,d);
	    return HI_FAILURE;
	}
	
	return d;
}

DRV_ErrCode HDMI_WriteI2C_Byte(unsigned char  RegAddr, unsigned char d)
{

	DRV_ErrCode ErrCode;
	/* write I2C */
	ErrCode = HI_UNF_I2C_Write(HdmiSwitchi2cnum, SWADR, RegAddr, 1, &d, 1);
	if (HI_SUCCESS != ErrCode)
	{
	    printf("HI_UNF_I2C_Write() fail,RegAddr[%x],d[%x]\n",RegAddr,d);
	    return HI_FAILURE;
	}
	
	return ErrCode;
}
void ITESwitchInitial(void)
{

#ifdef ITE6633E_T								 
	HDMI_WriteI2C_Byte (0x15, 0x0c);
#else
	HDMI_WriteI2C_Byte (0x15, 0xcc);
#endif 	

	HDMI_WriteI2C_Byte (0x29, 0x21);
	HDMI_WriteI2C_Byte (0x06, 0x80);
	HDMI_WriteI2C_Byte (0x0c, 0x07);

	HDMI_WriteI2C_Byte (0x04, 0x00);
	//Delay1ms(50);
	DRV_OS_TaskDelay_MS(50);
	HDMI_WriteI2C_Byte (0x04, 0x80);
	//Delay1ms(50);
	DRV_OS_TaskDelay_MS(50);
	HDMI_WriteI2C_Byte (0x04, 0x00);

	HDMI_WriteI2C_Byte (0x05, 0x0a);

	HDMI_WriteI2C_Byte (0x0F, 0x80);

	#ifdef WriteEDID
		EDIDInitial();			   
	#endif

	HDMI_WriteI2C_Byte (0x29, 0x01);

	//SwitchLED (PORT_1);

	HDMI_WriteI2C_Byte(0x06, 0xB0);  // TOE enable

	//end of initial

}

DRV_ErrCode ChangePort(unsigned char port)
{
	DRV_ErrCode errCode = HI_SUCCESS;
	unsigned char uc,uc3;
	uc=HDMI_ReadI2C_Byte(SEL_PORT_REG);		//Read Reg0x06
	uc &= 0xF0;							//Disable ITE6633 & Select Channel
	uc |= port;							//Enable software mode
	errCode=HDMI_WriteI2C_Byte (SEL_PORT_REG, uc);	//Configure	REG0x06[1:0] select channel
	if(HI_SUCCESS != errCode)
	{
		printf("%s:%s: %d  HI_UNF_GPIO_WriteBit ErrorCode=0x%08x\n", __FILE__, __FUNCTION__, __LINE__, errCode);
		return errCode;
	}
	uc3=HDMI_ReadI2C_Byte(SEL_PORT_REG);
	printf("Now Channel %X\n", port);
	return errCode;
}

void drv_HDIMSwitchHotplugTask(void)
{
	DRV_ErrCode errCode = HI_SUCCESS;
	HI_U32 u32PressStatus, u32KeyId;
	static HI_U8 KeyData;
	static HI_BOOL KeyValid;
	HI_U32 p32GpioNo;
	HI_BOOL bHighVolt;

	ConfigGpioUart1_Gpio_Usb0PinMux();//I/O复用为GPIO1_5

	errCode = HI_UNF_GPIO_Open();
	if (HI_SUCCESS != errCode)
	{
	    printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, errCode);
	    return errCode;
	}
	
	/* 输入 */
	errCode = HI_UNF_GPIO_SetDirBit(GPIO_HDMISwitchHotplogin, 1);
	errCode |= HI_UNF_GPIO_SetIntType(GPIO_HDMISwitchHotplogin, HI_UNF_GPIO_INTTYPE_UPDOWN);
	errCode |= HI_UNF_GPIO_SetIntEnable(GPIO_HDMISwitchHotplogin, 1);
	if(HI_SUCCESS != errCode)
	{
		printf("%s: %d  HI_UNF_GPIO_WriteBit ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
		HI_UNF_GPIO_Close();
		return errCode;
	}
	HI_UNF_GPIO_ReadBit(GPIO_HDMISwitchHotplogin, &bHighVolt);
	if(1 == bHighVolt)
	{
	    printf("debug : init GPIO_HDMISwitchHotplogin in \n");
	    if(HDMI_Switchplug_cb!=NULL)
			HDMI_Switchplug_cb(DRV_HDMI_Switch_IN);
	}

	while(1)
	{
		p32GpioNo = 0xffffffff;        
		u32PressStatus = 0;        
		u32KeyId = 0xffffffff;
		errCode = HI_UNF_GPIO_QueryInt(&p32GpioNo, 0);
		if(HI_SUCCESS != errCode)
		{
		    //printf("HI_UNF_GPIO_QueryInt timeout\n");
		    usleep(1*1000);
		    continue;
		}
		if(GPIO_HDMISwitchHotplogin == p32GpioNo)
		{
		    printf("debug : GPIO_HDMISwitchHotplogin int \n");
		    HI_UNF_GPIO_ReadBit(GPIO_HDMISwitchHotplogin, &bHighVolt);
		    if(1 == bHighVolt)
		    {
		        printf("debug : GPIO_HDMISwitchHotplogin in \n");
		        if(HDMI_Switchplug_cb!=NULL)
					HDMI_Switchplug_cb(DRV_HDMI_Switch_IN);
		    }
		    else if(0 == bHighVolt)
		    {
		        printf("debug : GPIO_HDMISwitchHotplogin out \n");
		        if(HDMI_Switchplug_cb!=NULL)
					HDMI_Switchplug_cb(DRV_HDMI_Switch_REMOVE);
		    }
		}
	}
}
	
void register_hdmi_switch_callback(DRV_HDMI_SwitchCallback cb)
{
	HDMI_Switchplug_cb=cb;
}

void unregister_hdmi_switch_callback(void)
{
	HDMI_Switchplug_cb=NULL;
}

 DRV_ErrCode DRV_HdmiSwitchInit(void)
{
	DRV_ErrCode ErrCode;
	DRVOS_Task_t * pHDMISwitchTaskid;
	
	ConfigGpioUart1_Gpio_Usb1_I2c1PinMux();
		
	/* Open I2C */
	ErrCode = HI_UNF_I2C_Open();
	if (HI_SUCCESS != ErrCode)
	{
	    printf("--HI_UNF_I2C_Open() fail.\n");
	    return HI_FAILURE;
	}
	ErrCode = HI_UNF_I2C_SetRate(HdmiSwitchi2cnum, HI_UNF_I2C_RATE_100K);
	ITESwitchInitial();
	ErrCode=DRV_HdmiSwitchSet_STB();
	if (HI_SUCCESS != ErrCode)
	{
	    printf("--INIT DRV_HdmiSwitchSet_STB() fail.\n");
	    //return HI_FAILURE;
	}
	/*create a thread for detect hdmiswitch hotplugin*/
	pHDMISwitchTaskid = DRV_OS_TaskCreate("drv_HDIMSwitchHotplugTask",drv_HDIMSwitchHotplugTask,3,1024*2,NULL,0);
	if (pHDMISwitchTaskid == NULL)
	{
	    printf("--%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
	    return ErrCode;
	}
	
	return ErrCode;
}


 DRV_ErrCode DRV_HdmiSwitchSet_STB(void)
{
	DRV_ErrCode ErrCode;
	
	ErrCode=ChangePort(PORT_B);
	if (HI_SUCCESS != ErrCode)
	{
	    printf("DRV_HdmiSwitchSet_STB() fail.\n");
	    return HI_FAILURE;
	}
	
	return ErrCode;
}

 DRV_ErrCode DRV_HdmiSwitchSet_UnSTB(void)
{
	DRV_ErrCode ErrCode;

	ErrCode=ChangePort(PORT_A);
	if (HI_SUCCESS != ErrCode)
	{
	    printf("DRV_HdmiSwitchSet_UnSTB() fail.\n");
	    return HI_FAILURE;
	}
	
	return ErrCode;
}

 DRV_ErrCode DRV_HdmiSwitchSet_Un2STB(void)
{
	DRV_ErrCode ErrCode;

	ErrCode=ChangePort(PORT_C);
	if (HI_SUCCESS != ErrCode)
	{
	    printf("DRV_HdmiSwitchSet_Un2STB() fail.\n");
	    return HI_FAILURE;
	}
	
	return ErrCode;
}
#endif
 

