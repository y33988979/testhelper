/******************************************************************************

Copyright (C), 2004-2008, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     : hi_unf_demux.h
Version       : Initial
Author        : HiSilicon multimedia software group
Created       : 2008-06-10
Last Modified :
Description   : HiSilicon DEMUX API declaration
Function List :
History       :
* Version   Date         Author                           Defect No.     Description
* main\1    2008-06-10   Jiang Lei (employee ID: 40671)     NULL         Created this file.
* main\2    2010-04-02   Jiang Lei (employee ID: 40671)     NULL         modified for HD series.
******************************************************************************/
/**
 * \file
 * \brief Describes the information about the DEMUX module.CNcomment:�ṩDEMUX�������Ϣ
 */

#ifndef __HI_UNF_DEMUX_H__
#define __HI_UNF_DEMUX_H__

#include "hi_unf_common.h"
#include "hi_error_mpi.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

/*************************** Structure Definition ****************************/
/** \addtogroup      H_2_2_2 */
/** @{ */  /** <!-- ��DEMUX�� */
/**Number of DEMUX devices.*/
/**DMX�豸��Ŀ*/
#define DMX_CNT                         5

/**Number of digital video broadcasting (DVB) ports.These ports are used to receive data from the tuner */
/**CNcomment:DVB�˿�����,ͨ��tuner��������*/
#define DMX_DVBPORT_CNT                 3

/**Number of RAM ports. These ports are used to receive data from the transport stream (TS) buffer.  */
/**CNcomment:RAM�˿�����,ʹ��TS BUFFER��������*/
#define DMX_RAMPORT_CNT                 4

/**Number of DEMUX ports.*/
/**CNcomment:DMXģ��˿�����*/
#define DMX_TOTALPORT_CNT               7

/**Number of audio/video channels of the DEMUX module. */
/**CNcomment:DMXģ������Ƶͨ������ */
#define DMX_AVCHAN_CNT                  32

/**Number of DEMUX channels. */
/**CNcomment:DMXͨ������Ŀ*/
#define DMX_TOTALCHAN_CNT               96

/**Number of DEMUX filters. */
/**CNcomment:DMX����������Ŀ*/
#define DMX_TOTALFILTER_CNT             96

/**Maximum depth of a filter. */
/**CNcomment:filter������*/
#define DMX_FILTER_MAX_DEPTH            16

/**Number of program clock reference (PCR) channels. */
/**CNcomment:PCRͨ������*/
#define DMX_PCRCHAN_CNT                 16

/**Default size of the general-purpose channel buffer, 16 KB. */
/**CNcomment:Ĭ��ͨ��ͨ����������С 16K Byte*/
#define DMX_DEFAULT_CHANBUF_SIZE        (16*1024)

/**Definition of the DEMUX ID */
/**CNcomment:DEMUX ID����*/
#define HI_UNF_DMX_ID_0_PLAY            0
#define HI_UNF_DMX_ID_1_REC             1
#define HI_UNF_DMX_ID_2_REC             2
#define HI_UNF_DMX_ID_3_REC             3
#define HI_UNF_DMX_ID_4_PLAY            4


/*************************** Structure Definition ****************************/
/**Defines the capability of the DEMUX module*/
/**CNcomment:����DEMUXģ��ҵ���ܽṹ��*/
typedef struct hiUNF_DMX_CAPABILITY_S
{
    HI_U32 u32PortNum;          /**<Number of ports. */ /**< CNcomment:�˿�����     */
    HI_U32 u32DmxNum;           /**<Number of DEMUX devices*/ /**< CNcomment:�豸����     *///AI7D04684
    HI_U32 u32CommonChannelNum; /**<Number of general-purpose channels*/ /**< CNcomment:ͨ��ͨ������ */
    HI_U32 u32AudioChannelNum;  /**<Number of audio channels*/ /**< CNcomment:��Ƶͨ������ */
    HI_U32 u32VideoChannelNum;  /**<Number of video channels*/ /**< CNcomment:��Ƶͨ������ */
    HI_U32 u32FilterNum;        /**<Number of filters*/ /**< CNcomment:����������   */
    HI_U32 u32KeyNum;           /**<Number of keys*/ /**< CNcomment:��Կ������   */
}HI_UNF_DMX_CAPABILITY_S;

/**<TS port ID*/ 
/**CNcomment:TS�˿�ID */
typedef enum hiUNF_DMX_PORT_E
{
    HI_UNF_DMX_PORT_0_TUNER,     /**<Port0, tuner mode*/ /**< CNcomment:port0, Tunerģʽ */
    HI_UNF_DMX_PORT_1_TUNER,     /**<Port1, tuner mode*/ /**< CNcomment:port1, Tunerģʽ */
    HI_UNF_DMX_PORT_2_TUNER,     /**<Port2, tuner mode*/ /**< CNcomment:port2, Tunerģʽ */
    HI_UNF_DMX_PORT_3_RAM,       /**<Port3, RAM mode*/ /**< CNcomment:port3, RAMģʽ */
    HI_UNF_DMX_PORT_4_RAM,       /**<Port4, RAM mode*/ /**< CNcomment:port4, RAMģʽ */
    HI_UNF_DMX_PORT_5_RAM,       /**<Port5, RAM mode*/ /**< CNcomment:port5, RAMģʽ */
    HI_UNF_DMX_PORT_6_RAM,       /**<Port6, RAM mode*/ /**< CNcomment:port6, RAMģʽ */

    HI_UNF_DMX_PORT_BUTT
}HI_UNF_DMX_PORT_E;

/**<TS port mode*/ 
/**CNcomment:TS�˿�ģʽ */
typedef enum hiUNF_DMX_PORT_MODE_E
{
    HI_UNF_DMX_PORT_MODE_EXTERNAL,   /**<External TS input mode*/ /**< CNcomment:�ⲿTS����*/
    HI_UNF_DMX_PORT_MODE_INTERNAL,   /**<Internal TS input mode*/ /**< CNcomment:�ڲ�TS����*/
    HI_UNF_DMX_PORT_MODE_RAM,        /**<Memory input mode*/ /**< CNcomment:���ڴ�����*/

    HI_UNF_DMX_PORT_MODE_BUTT
}HI_UNF_DMX_PORT_MODE_E;

/**<TS port type*/ 
/**CNcomment:TS�˿����� */
typedef enum hiUNF_DMX_PORT_TYPE_E
{
    HI_UNF_DMX_PORT_TYPE_PARALLEL_BURST,          /**<Parallel burst mode*/ /**< CNcomment:����BURSTģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_VALID,          /**<Parallel valid mode*/ /**< CNcomment:����VALIDģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188,     /**<Self-sync 188 mode*/ /**< CNcomment:��ͬ��188ģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_204,     /**<Self-sync 204 mode*/ /**< CNcomment:��ͬ��204ģʽ             */
    HI_UNF_DMX_PORT_TYPE_PARALLEL_NOSYNC_188_204, /**<self-sync 188/204 auto-identification mode */ /**< CNcomment:��ͬ��188/204�Զ�ʶ��ģʽ */
    HI_UNF_DMX_PORT_TYPE_SERIAL,                  /**<Serial mode*/ /**< CNcomment:����ģʽ                  */
    HI_UNF_DMX_PORT_TYPE_USER_DEFINED,            /**<User defined mode,just for mv300 ip mode*//**< CNcomment:�û��Զ���ģʽ��ֻ��mv300 IP�˿������� */

    HI_UNF_DMX_PORT_TYPE_BUTT
}HI_UNF_DMX_PORT_TYPE_E;

/**<TS port attributes*/ 
/**TS�˿�����*/
typedef struct hiUNF_DMX_PORT_ATTR_S
{
    HI_UNF_DMX_PORT_MODE_E enPortMod;  /**<Port mode*/ /**< CNcomment:�˿�ģʽ */
    HI_UNF_DMX_PORT_TYPE_E enPortType; /**<Port type*/ /**< CNcomment:�˿����� */
    HI_U32 u32SyncLostTh;              /**<Sync loss threshold.The default value is recommended.*/ /**< CNcomment:ͬ����ʧ���ޣ��Ƽ�ʹ��Ĭ��ֵ */
    HI_U32 u32SyncLockTh;              /**<Sync lock threshold.The default value is recommended.*/ /**< CNcomment:ͬ���������ޣ��Ƽ�ʹ��Ĭ��ֵ */
    HI_U32 u32TunerInClk;              /**<Whether to reverse the phase of the clock input from the tuner*/ /**< CNcomment:Tuner����ʱ���Ƿ���.0: ͬ��(Ĭ��); 1: ����*/
    HI_U32 u32SerialBitSelector;       /**<Port line sequence select In parallel mode: 0: cdata[7] is the most significant bit (MSB) (default). 1: cdata[0] is the MSB.
                                           In serial mode: 1: cdata[0] is the data line (default). 0: cdata[7] is the data line.*/
                                        /**< CNcomment:�˿�����ѡ�񡣲���ʱ:0: ��ʾcdata[7]���λ(Ĭ��);1: ��ʾcdata[0]���λ��
                                           ����ʱ:1: ��ʾcdata[0]Ϊ������(Ĭ��); 0: ��ʾcdata[7]Ϊ������*/

    HI_U32 u32TunerErrMod;             /**<Level mode of the cerr_n line from the tuner to a DEMUX.
                                           0: A data error occurs when the cerr_n line is high.
                                           1: A data error occurs when the cerr_n line is low (default).*/
										    /**< CNcomment:tuner��DEMUX��cerr_n�ߵ�ƽģʽ:
                                           0: ��ʾcerr_n��Ϊ��ʱ��ʾ���ݴ���;
                                           1: ��ʾcerr_n��Ϊ��ʱ��ʾ���ݴ��� (Ĭ��) */
    HI_U32 u32UserDefLen1;            /**<User defined length1,valid when enPortType is HI_UNF_DMX_PORT_TYPE_USER_DEFINED,188~255 *//**< CNcomment:�û��Զ��峤�ȼ�����1��ֻ�ж˿�ģʽΪ�û��Զ���ʱ��Ч����Ч��Χ188~255 */
    HI_U32 u32UserDefLen2;            /**<User defined length2,valid when enPortType is HI_UNF_DMX_PORT_TYPE_USER_DEFINED,188~255 *//**< CNcomment:�û��Զ��峤�ȼ�����2��ֻ�ж˿�ģʽΪ�û��Զ���ʱ��Ч����Ч��Χ188~255*/

} HI_UNF_DMX_PORT_ATTR_S;

/**<Status of the TS port*/ 
/**CNcomment:TS�˿ڵ�״̬*/
typedef struct hiUNF_DMX_PORT_PACKETNUM_S
{
	HI_U32 				   u32TsPackCnt;	/**<Number of TS packets received from the TS port*/ /**< CNcomment:�˿��յ���TS������*/
	HI_U32  			   u32ErrTsPackCnt;	/**<Number of error TS packets received from the TS port*/ /**< CNcomment:�˿��ϴ���TS���ļ���*/
}HI_UNF_DMX_PORT_PACKETNUM_S;

/**<Status of a TS buffer of a DEMUX*/ 
/**CNcomment:DEMUX��TS Buffer״̬ */
typedef struct hiUNF_DMX_TSBUF_STATUS_S
{
    HI_U32 u32BufSize;    /**<Buffer size*/ /**< CNcomment:��������С       */
    HI_U32 u32UsedSize;   /**<Used buffer size*/ /**< CNcomment:��������ʹ�ô�С,��ֵ��ʵ��ʹ��ֵ��0x100�ֽ� */
} HI_UNF_DMX_TSBUF_STATUS_S;

/**<Channel type*/ 
/**CNcomment:ͨ������ */
typedef enum hiUNF_DMX_CHAN_TYPE_E
{
    HI_UNF_DMX_CHAN_TYPE_SEC = 0, /**<Channel that receives sections data such as program specific information (PSI) or service information (SI) data*/ /**< CNcomment:���ڽ��� PSI/SI��secitons���ݵ�ͨ��*/
    HI_UNF_DMX_CHAN_TYPE_PES,     /**<Channel that receives packetized elementary stream (PES) data*/ /**< CNcomment:���ڽ���PES�����ݵ�ͨ��*/
    HI_UNF_DMX_CHAN_TYPE_AUD,     /**<Channel that receives audio data*/ /**< CNcomment:���ڽ�����Ƶ���ݵ�ͨ��*/
    HI_UNF_DMX_CHAN_TYPE_VID,     /**<Channel that receives video data*/ /**< CNcomment:���ڽ�����Ƶ���ݵ�ͨ��*/
    HI_UNF_DMX_CHAN_TYPE_POST,      /**<Entire-packet posting channel that receives an entire TS packet with a specific packet identifier (PID). */ /**< CNcomment:��������ͨ�������ڽ���ĳPID������TS��*/
    HI_UNF_DMX_CHAN_TYPE_ECM_EMM,   /**<Channel that receives entitlement control message (ECM) or entitlement management message (EMM) data*/ /**< CNcomment:���ڽ���ECM��EMM���ݵ�ͨ��*/

    HI_UNF_DMX_CHAN_TYPE_BUTT
}HI_UNF_DMX_CHAN_TYPE_E;

/**<Cyclic redundancy check (CRC) mode of a channel*/ 
/**CNcomment:ͨ����CRCģʽ */
typedef enum hiUNF_DMX_CHAN_CRC_MODE_E
{
    HI_UNF_DMX_CHAN_CRC_MODE_FORBID = 0,                /**<The CRC check is disabled*/ /**< CNcomment:CRCУ���ֹ */
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_DISCARD = 1,     /**<The CRC check is enabled, and the error Section data is discarded*/ /**< CNcomment:CRCУ��ʹ�ܣ�����������Section */
    HI_UNF_DMX_CHAN_CRC_MODE_FORCE_AND_SEND = 2,        /**<The CRC check is enabled, and the error Section data is received*/ /**< CNcomment:CRCУ��ʹ�ܣ������ʹ���Section */
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD = 3, /**<Whether the CRC check is performed depends on the syntax, and the error Section data is discarded*/ /**< CNcomment:�����﷨�ж��Ƿ����CRCУ�飬����������Section */
    HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_SEND = 4,    /**<Whether the CRC check is performed depends on the syntax, and the error Section data is received*/ /**< CNcomment:�����﷨�ж��Ƿ����CRCУ�飬�����ʹ���Section */

    HI_UNF_DMX_CHAN_CRC_MODE_BUTT
}HI_UNF_DMX_CHAN_CRC_MODE_E;

/**<Output mode of a channel*/ 
/**CNcomment:ͨ�������ģʽ */
typedef enum hiUNF_DMX_CHAN_OUTPUT_MODE_E
{
    HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY = 0x1,     /**<Mode of playing audios/videos or receiving data*/ /**< CNcomment:����Ƶ���Ż����ݽ��� */
    HI_UNF_DMX_CHAN_OUTPUT_MODE_REC  = 0x2,     /**<Recording mode*/ /**< CNcomment:¼�� */
    HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY_REC = 0x3, /**<Mode of recording and playing data or receiving data*/ /**< CNcomment:ͬʱ¼�ƺͲ��Ż����ݽ��� */
    HI_UNF_DMX_CHAN_OUTPUT_MODE_BUTT = -1
}HI_UNF_DMX_CHAN_OUTPUT_MODE_E;

/**<Channel attribute*/ 
/**CNcomment:ͨ������*/
typedef struct hiUNF_DMX_CHAN_ATTR_S
{
    HI_U32 u32BufSize;                            /**<Buffer size used by channels*/ /**< CNcomment:ͨ��Ӳ��ʹ��buffer��С*/
    HI_UNF_DMX_CHAN_TYPE_E enChannelType;         /**<Channel type*/ /**< CNcomment:ͨ������ */
    HI_UNF_DMX_CHAN_CRC_MODE_E enCRCMode;         /**<CRC mode.It is valid for the DEMUX_CHAN_SEC channel.*/ /**< CNcomment:CRCģʽ��ͨ������ΪDMX_CHAN_SEC ʱ��Ч */
    HI_UNF_DMX_CHAN_OUTPUT_MODE_E enOutputMode;   /**<Output mode of the channel data*/ /**< CNcomment:ͨ���������ģʽ */
}HI_UNF_DMX_CHAN_ATTR_S;

/**<Scrambled flag of the channel data*/ 
/** CNcomment:ͨ�����ݵļ��ű�־ */
typedef enum hiUNF_DMX_SCRAMBLED_FLAG_E
{
    HI_UNF_DMX_SCRAMBLED_FLAG_TS,  /**<TS data is scrambled*/ /**< CNcomment:���ݱ�TS������*/
    HI_UNF_DMX_SCRAMBLED_FLAG_PES, /**<PES data is scrambled*/ /**< CNcomment:���ݱ�PES������*/
    HI_UNF_DMX_SCRAMBLED_FLAG_NO,  /**<Data is not scrambled*/ /**< CNcomment:����δ������*/

    HI_UNF_DMX_SCRAMBLED_FLAG_BUTT
}HI_UNF_DMX_SCRAMBLED_FLAG_E;

/**<Channel status*/ 
/** CNcomment:ͨ��״̬ */
typedef enum hiUNF_DMX_CHAN_STATUS_E
{
    HI_UNF_DMX_CHAN_CLOSE = 0x0,        /**<The channel is stopped.*/ /**< CNcomment:ͨ�����ڹر�״̬*/
    HI_UNF_DMX_CHAN_PLAY_EN = 0x1,      /**<The channel is playing audios/videos or receiving data.*/ /**< CNcomment:ͨ����������Ƶ���Ż����ݽ���״̬*/
    HI_UNF_DMX_CHAN_REC_EN = 0X2,       /**<The channel is recording data.*/ /**< CNcomment:ͨ����������¼��״̬*/
    HI_UNF_DMX_CHAN_PLAY_REC_EN = 0X3,  /**<The channel is recording and receiving data.*/ /**< CNcomment:ͨ������ͬʱ¼�ƺ����ݽ���״̬*/
}HI_UNF_DMX_CHAN_STATUS_E;

/**<Defines the channel status*/ 
/** CNcomment:����ͨ��״̬�ṹ��*/
typedef struct hiUNF_DMX_CHAN_STATUS_S
{
    HI_UNF_DMX_CHAN_STATUS_E enChanStatus;   /**<Channel status*/ /**< CNcomment:ͨ��״̬*/
}HI_UNF_DMX_CHAN_STATUS_S;

/******************  Filter Attr  ***********************/

/**<Filter attribute*/ 
/** CNcomment:����������*/
typedef struct hiUNF_DMX_FILTER_ATTR_S
{
    HI_U32 u32FilterDepth;                    /**<Depth of a filter.*/ /**< CNcomment:���������*/
    HI_U8 au8Match[DMX_FILTER_MAX_DEPTH];     /**<Matched bytes of a filter.The data is compared by bit.*/ /**< CNcomment:������ƥ���ֽ�,��bit�Ƚ�*/
    HI_U8 au8Mask[DMX_FILTER_MAX_DEPTH] ;     /**<Masked bytes of a filter. The conditions are set by bit. 0: no mask. Comparison is required. 1: mask. Comparison is not required.*/ /**< CNcomment:�����������ֽ�,��bit����, 0:û��mask��Ҫ���бȽ�, 1:mask�����ã������бȽ�*/
    HI_U8 au8Negate[DMX_FILTER_MAX_DEPTH] ;   /**<Negated bytes of a filter. 0: not negated; 1: negated*/ /**< CNcomment:������ȡ���ֽ�,��byte����, 0:��ȡ��,1:ȡ��*/
}HI_UNF_DMX_FILTER_ATTR_S;

/**<Conditional access (CA) type, indicating whether advanced CA is used.*/ 
/** CNcomment:CA���ͣ��Ƿ�ʹ�ø߰�ȫCA*/
typedef enum hiUNF_DMX_CA_TYPE_E
{
    HI_UNF_DMX_CA_NORMAL = 0,    /**<Common CA*/ /**< CNcomment:��ͨCA*/
    HI_UNF_DMX_CA_ADVANCE,       /**<Advanced CA*/ /**< CNcomment:�߰�ȫCA*/

    HI_UNF_DMX_CA_BUTT
}HI_UNF_DMX_CA_TYPE_E;

/**<CA Entropy reduction mode*/ 
/** CNcomment:�ؼ���ģʽ*/
typedef enum hiUNF_DMX_CA_ENTROPY_REDUCTION_E
{
    HI_UNF_DMX_CA_ENTROPY_REDUCTION_CLOSE = 0,  /**<64bit*/
    HI_UNF_DMX_CA_ENTROPY_REDUCTION_OPEN,       /**<48bit*/

    HI_UNF_DMX_CA_ENTROPY_REDUCTION_BUTT
}HI_UNF_DMX_CA_ENTROPY_E;


/**<Type of the descrambler protocol.*/ 
/** CNcomment:������Э������*/
typedef enum hiUNF_DMX_DESCRAMBLER_TYPE_E
{
    HI_UNF_DMX_DESCRAMBLER_TYPE_CSA2      = 0,   /**<CSA2.0*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_CSA3      ,      /**<CSA3.0*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_AES_IPTV  ,      /**<AES IPTV of SPE*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_AES_ECB   ,      /**<SPE AES ECB*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_AES_CI    ,      /**<SPE AES CIPLUS*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_DES_CI    ,      /**<DES CIPLUS*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_DES_CBC   ,      /**<DES CBC*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_AES_NS    ,      /**<AES NS-Mode*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_NS   ,      /**<SMS4 NS-Mode*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_IPTV ,      /**<SMS4 IPTV*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_ECB  ,      /**<SMS4 ECB*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_SMS4_CBC  ,      /**<SMS4 CBC*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_AES_CBC   ,      /**<AES CBC*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_BUTT
}HI_UNF_DMX_DESCRAMBLER_TYPE_E;

/**<Attribute of the key area.*/ 
/** CNcomment:��Կ������*/
typedef struct hiUNF_DMX_DESCRAMBLER_ATTR_S
{
    HI_UNF_DMX_CA_TYPE_E enCaType;                    /**<Whether the descrambler adopts advanced CA.*/ /**< CNcomment:�������Ƿ�ʹ�ø߰�ȫCA*/
    HI_UNF_DMX_DESCRAMBLER_TYPE_E enDescramblerType;  /**<Descrambling protocol type of the descrambler*/ /**< CNcomment:����������Э������*/
    HI_UNF_DMX_CA_ENTROPY_E enEntropyReduction;       /**<CA Entropy reduction mode*/ /**< CNcomment:0:48bitģʽ��1:64 bitģʽ��cas2.0ʱ��Ч*/
}HI_UNF_DMX_DESCRAMBLER_ATTR_S;

/**<Type of the DEMUX data packet.*/ 
/** DEMUX���ݰ�������*/
typedef enum hiUNF_DMX_DATA_TYPE_E
{
    HI_UNF_DMX_DATA_TYPE_WHOLE = 0,  /**<The data segment contains a complete data packet*/ /**< CNcomment:�˶����ݰ������������ݰ�, ����SECTIONÿ��������������*/
    HI_UNF_DMX_DATA_TYPE_HEAD,       /**<The data segment contains the head of a data packet, but the data packet may not be complete*/ /**< CNcomment:�˶����ݰ������ݰ�����ʼ�����ǲ�һ���������İ�, ֻ����PES����*/
    HI_UNF_DMX_DATA_TYPE_BODY,       /**<This type is valid only for the PES data.The data segment contains the body of a data packet. */ /**< CNcomment:�˶����ݰ������ݰ������ݣ���������ʼ�������н�β, ֻ����PES����*/
    HI_UNF_DMX_DATA_TYPE_TAIL,       /**<This type is valid only for the PES data.The data segment contains the tail of a data packet, and is used to identify the end of a data packet.*/ /**< CNcomment:�˶����ݰ������ݰ��Ľ�β������ָʾ��ʶ��İ�����, ֻ����PES����*/

    HI_UNF_DMX_DATA_TYPE_BUTT
}HI_UNF_DMX_DATA_TYPE_E;

/**<DEMUX data packet*/ 
/** CNcomment:DEMUX���ݰ��ṹ*/
typedef struct hiUNF_DMX_DATA_S
{
    HI_U8   *pu8Data;                   /**<Data pointer*/ /**< CNcomment:����ָ�� */
    HI_U32  u32Size;                    /**<Data length*/ /**< CNcomment:���ݳ��� */
    HI_UNF_DMX_DATA_TYPE_E enDataType;  /**<Data packet type*/ /**< CNcomment:���ݰ������� */
}HI_UNF_DMX_DATA_S;

/**<DEMUX buffer status*/ 
/** CNcomment:DEMUX������״̬�ṹ*/
typedef struct hiUNF_DMX_BUF_STATUS_S
{
    HI_U32 u32BufSize;  /**<Used buffer*/ /**< CNcomment:��������С */
    HI_U32 u32UsedSize; /**<Buffer size*/ /**< CNcomment:��������ʹ�ô�С */
} HI_UNF_DMX_BUF_STATUS_S;


/** @} */  /** <!-- ==== Structure Definition end ==== */


/******************************* API Declaration *****************************/
/** \addtogroup      H_1_2_1 */
/** @{ */  /** <!-- ��DEMUX�� */

/**
\brief Initializes the DEMUX module.CNcomment:��ʼ��DEMUXģ��
\attention \n
Before using DEMUXs, you must call this application programming interface (API).\n
Before using the personal video recorder (PVR) or audio/video player (AVPLAY), you need to initialize the DEMUX module.\n
The error code HI_SUCCESS is returned if this API is called repeatedly. 
CNcomment:�ڽ���DEMUX��ز���ǰӦ�����ȵ��ñ��ӿ�\n
��ʹ��PVR��AVPLAYǰ����ǰ����DEMUXģ��ĳ�ʼ��\n
�ظ����ñ��ӿڣ��᷵�سɹ�
\param  N/A.CNcomment:��
\retval ::HI_SUCCESS Success. CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\see \n
N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_Init(HI_VOID);


/**
\brief Deinitializes the DEMUX module. CNcomment:ȥ��ʼ��DEMUXģ��
\attention \n
After this API is called, the DEMUX module is stopped, and the DEMUX resources used by the process are released.\n
This API is valid when it is called for the first time. If this API is called repeatedly, the error code HI_SUCCESS is returned.
CNcomment:���ñ��ӿ�ֹͣʹ��DEMUXģ�飬���ͷű�������ռ�õ�DEMUX��Դ\n
���ӿڵ�һ�ε���������,�ظ����÷��سɹ�
\param  N/A.CNcomment:��
\retval ::HI_SUCCESS Success CNcomment:�ɹ�
\retval ::HI_FAILURE Calling this API fails. CNcomment:APIϵͳ����ʧ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DeInit(HI_VOID);


/**
\brief Obtains the information about the service capabilities of the DEMUX module.The information includes the numbers of channels, filters, keys, ports, DEMUXs, and audio/video channels. 
CNcomment:��ѯDEMUXģ��ҵ��������ҵ���ܰ���ͨ��ͨ����Ŀ����������Ŀ����Կ��Ŀ���˿���Ŀ��DEMUX�豸��Ŀ������Ƶͨ����Ŀ��
\attention \n
N/A.CNcomment:��
\param[out] pstCap   Pointer to the capability of the DEMUX module (output).CNcomment:ָ�����ͣ����DEMUXģ��������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null.CNcomment:ָ�����Ϊ��
\see \n
::HI_UNF_DMX_CAPABILITY_S
*/
HI_S32 HI_UNF_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *pstCap);


/**
\brief Obtains the attributes of a DEMUX port.CNcomment:��ȡDEMUX�˿�����
\attention \n
 N/A.CNcomment:��
\param[in] enPortId  u32PortId  Port ID.CNcomment:�˿ں�
\param[out] pstAttr   Pointer to the port attributes (output).CNcomment:ָ�����ͣ�����˿����ԡ�
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid.CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null.CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr);


/**
\brief Sets the attributes of a DEMUX port.CNcomment:����DEMUX�˿�����
\attention \n
It is recommended to query the default attributes of a DEMUX port by calling HI_UNF_DMX_GetTSPortAttr before modifying the attributes of the port.\n
Port 0 to port 2 are HI_UNF_DMX_PORT_MODE_EXTERNAL ports, and port 3 to port 6 are HI_UNF_DMX_PORT_MODE_RAM ports.\n
The port mode can be queried only, but other attributes can be modified dynamically.\n
The port in memory input mode can be set to self-sync parallel mode only; otherwise, the port type fails to be set.\n
If the sync loss threshold and sync lock threshold are used, their default values are recommended.\n
For details on how to set the input clock of the tuner, select the line sequence of the port, and set the level of the tuner error line, contact hardware engineers of HiSilicon\n
and refer to tuner user manuals. 
CNcomment:������ͨ��HI_UNF_DMX_GetTSPortAttr��ȡĬ�����ԣ�Ȼ���Ҫ�ı�����Խ�������\n
�˿�0~2ΪHI_UNF_DMX_PORT_MODE_EXTERNAL���ͣ��˿�3~6ΪHI_UNF_DMX_PORT_MODE_RAM���Ͷ˿�\n
�˿�ģʽֻ֧�ֲ�ѯ����֧���޸ģ��������Ծ�֧����ʱ��̬�޸�\n
�ڴ�ģʽ�Ķ˿ڽ�֧������Ϊ��ͬ������ģʽ�����������˿����ͻ�ʧ��\n
ͬ����ʧ��ͬ���������ޣ��Ƽ�ʹ��Ĭ��ֵ\n
tuner����ʱ�ӷ��򡢶˿�����ѡ���tuner error�ߵ�ƽģʽ����������ѯӲ������ʦ\n
���ο�tuner���û��ֲᣬȻ������Ϊ���ʵ�ֵ
\param[in] enPortId    Port ID.CNcomment:�˿ںš�
\param[in] pstAttr    Pointer to port attributes.CNcomment:ָ�����ͣ��˿�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails. CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr);


/**
\brief Attaches a DEMUX to a port.CNcomment:��DEMUX���˿���
\attention \n
A maximum of five DEMUXs are supported. Each DEMUX can be attached to only one port, but the data from a port can be delivered to multiple DEMUXs.\n
After a DEMUX is attached to a port, the DEMUX starts to receive the data input from this port.\n
If the DEMUX is not used, you can detach it by calling HI_UNF_DMX_DetachTSPort.\n
The DEMUXs can be attached to different ports dynamically. That is, you can attach the DEMUXs without detaching them.\n
If you call this API to attach a DEMUX to the same channel, the error code HI_SUCCESS is returned. 
CNcomment:֧��5·DEMUX��ÿ·DEMUXֻ�ܰ󶨵�һ��PORT������һ��PORT�����ݿ��Էַ������DEMUX\n
DEMUX�󶨵��˿���֮��Ϳ�ʼ��������˿������������\n
����ʹ����·DEMUX�������ݺ�ʹ��HI_UNF_DMX_DetachTSPort�ӿڽ��DEMUX\n
���Զ�̬�İ󶨵���ͬ�Ķ˿��ϣ������Ƚ��\n
�ظ����ô˽ӿڰ󶨵���ͬͨ���Ϸ��سɹ�
\param[in] u32DmxId  DEMUX ID.
\param[in] enPortId  Port ID.CNcomment:�˿ںš�
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
::HI_UNF_DMX_DetachTSPort
*/
HI_S32 HI_UNF_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId);


/**
\brief Detaches a DEMUX from a port.CNcomment:�Ӷ˿��Ͻ��DEMUX
\attention \n
If a DEMUX is not used, you can detach it from the corresponding port by calling this API.\n
This API can be called dynamically, and can be used to disable the input sources of a DEMUX.\n
If this API is called repeatedly, the error code HI_SUCCESS is returned. 
CNcomment:����ʹ��DEMUX�����κ�����ʱʹ�ô˽ӿڽ�DEMUX�Ӷ˿��Ͻ��\n
���Զ�̬ʹ�ý�󶨽ӿڣ�����ͨ����󶨽ӿ��ж�DEMUX������Դ\n
�ظ���󶨷��سɹ�
\param[in] u32DmxId  DEMUX ID.
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
::HI_UNF_DMX_AttachTSPort
*/
HI_S32 HI_UNF_DMX_DetachTSPort(HI_U32 u32DmxId);


/**
\brief Obtains the ID of the port that is attached to a DEMUX. CNcomment:��ȡDEMUX���󶨵Ķ˿ڵĶ˿ں�
\attention \n
If a DEMUX is not attached to any port, an error code is returned.CNcomment:���DEMUXû�а󶨵��κζ˿��ϻ᷵�ش�����
\param[in] u32DmxId  DEMUX ID.
\param[out] penPortId  Pointer to the ID of the port that is attached to a DEMUX(output).CNcomment:ָ�����ͣ����DEMUX�󶨵Ķ˿ںš�
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOATTACH_PORT  The DEMUX is not attached to any port.CNcomment:��·DEMUXû�а󶨵��κζ˿���
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSPortId(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E *penPortId);


/**
\brief Creates a TS buffer for the RAM port to receive the TS data from Internet or local memory. CNcomment:ΪRAM�˿ڴ���TS Buffer���ڽ��մ�����򱾵ش洢�������TS����
\attention \n
A TS buffer can be created for the RAM port only.\n
The buffer size must range from 4 KB to 16 MB.\n
It is recommended to set the buffer size to an integral multiple of 4 KB. Otherwise, bottom alignment is performed inside.\n
The TS buffer cannot be created for the same port for multiple times; otherwise, an error code is returned. 
CNcomment:ֻ��RAM���Ͷ˿ڿ��Դ���TS Buffer\n
buffer��С������4K��16M֮�䣬��λΪ�ֽ�\n
buffer��С���Ϊ4K��������������������������ڲ���������¶������\n
ͬһ���˿ڲ����ظ�����TS Buffer,���򷵻ش�����
\param[in] enPortId  [in]  Port ID.CNcomment:�˿ںš�
\param[in] u32TsBufSize Size of a TS buffer.CNcomment:TS Buffer��С
\param[out] phTsBuffer  Pointer to the handle of a created TS buffer (output).CNcomment:ָ�����ͣ����������TS Buffer �����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_RECREAT_TSBUFFER  The TS buffer corresponding to the port is created.CNcomment:�ظ�����TS Buffer
\see \n
N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_E enPortId, HI_U32 u32TsBufSize, HI_HANDLE *phTsBuffer);


/**
\brief Destroys an existing TS buffer. CNcomment:���ٴ�����TS Buffer
\attention \n
A TS buffer cannot be destroyed repeatedly. If a TS buffer is destroyed, its handle is also destroyed.\n
In addition, if a buffer is destroyed, its data is cleared. In this case, the get and put operations are not available.\n
However, the data in channels is retained when a TS buffer is destroyed. To switch streams, you need disable the channel, and then enable the channel again. 
CNcomment:�����ظ�����һ��TS Buffer,����֮��handle�Ͳ�������\n
����֮��buffer�е����ݾͻᱻ��գ������ٽ���Get��Put����\n
����TS buffer���������ͨ�������ݣ�Ҫ�л�������Ҫע��ر�ͨ�������´�
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DestroyTSBuffer(HI_HANDLE hTsBuffer);


/**
\brief Obtains a TS buffer to input data.CNcomment:��ȡTS Buffer�ռ䣬������������
\attention \n
If you call this API repeatedly, the address of the same buffer is returned.\n
The input data length must be appropriate. If the data length is too large, data cannot be input in time.\n
If the data length is too small, the buffer is scheduled frequently, which causes the system performance to deteriorate.\n
If the size of the available space is smaller than the requested data length, an error code is returned.\n
To be specific, the requested data length must be smaller than the size of the available space, that is, the buffer cannot be full.\n
In addition, the requested data length cannot be 0; otherwise, the error code HI_ERR_DMX_INVALID_PARA is returned.\n
If u32TimeOutMs is set to 0, it indicates that the waiting time is 0; if u32TimeOutMs is set to 0XFFFFFFFF, it indicates infinite wait; if u32TimeOutMs is set to other values, it indicates that the waiting time is u32TimeOutMs ms.\n
If no buffer can be applied for during the block period, the error code HI_ERR_DMX_TIMEOUT is returned.\n
If u32TimeOutMs is set to 0, and no buffer can be applied for, it indicates that the internal TS buffer is full. In this case, you need to call the usleep(10000) function to release the CPU. Therefore, other threads can be scheduled. 
CNcomment:�ظ�get����ͬһ��buffer��ַ\n
��������ʱע��ÿ����������ݳ��ȣ�̫�����ܻᵼ���������ݲ���ʱ\n
̫�̿��ܻᵼ�µ���̫Ƶ���������½�\n
��ʣ��ռ�С�ڵ�������ĳ���ʱ�����ش�����\n
ÿ������ĳ���ҪС��ʣ��ʣ�೤�ȣ���buffer�����ܱ�����\n
���󳤶Ȳ�����Ϊ0�����򷵻ز����Ƿ�������\n
u32TimeOutMs����Ϊ0��ʾ���ȴ�������Ϊ0xffffffff��ʾһֱ�ȴ�������Ϊ����ֵ��ʾ�ȴ�u32TimeOutMs���롣\n
����������ʱ�䣬���޷����뵽Buffer���򷵻�HI_ERR_DMX_TIMEOUT������\n
u32TimeOutMs����Ϊ0ʱ��������벻��Buffer��˵����ʱ�ڲ�TS Buffer�ռ���������Ҫͨ��usleep(10000)�ͷ�cpu
��ʹ�����߳��ܹ��õ����ȡ�
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\param[in] u32ReqLen Requested data length.CNcomment:�������ݵĳ���
\param[out] pstData  Data buffer.CNcomment:����buffer�ṹ
\param[in] u32TimeOutMs      Wait timeout, in ms.CNcomment:�ȴ���ʱʱ�䣬��λms
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_TIMEOUT  The wait times out. CNcomment:�ȴ���ʱ
\retval ::HI_ERR_DMX_NOAVAILABLE_BUF  The size of the available buffer is smaller than the requested data length.CNcomment:ʣ��buffer�ռ�С�����󳤶�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ReqLen,
            HI_UNF_STREAM_BUF_S *pstData, HI_U32 u32TimeOutMs);


/**
\brief Updates the write pointer of a TS buffer after the TS data is input.CNcomment:TS����������ϣ����ڸ���TS Bufferдָ��
\attention \n
This API must work with HI_UNF_DMX_GetTSBuffer. That is, if you call HI_UNF_DMX_PutTSBuffer without calling HI_UNF_DMX_GetTSBuffer, no operation takes effect, but the error code HI_SUCCESS is returned.\n
If the valid data length is 0, no operation takes effect, and the error code HI_SUCCESS.\n
The valid data length cannot be greater than data length queried by calling HI_UNF_DMX_GetTSBuffer. Otherwise, the error code HI_ERR_DMX_INVALID_PARA is returned.\n
If the valid data length is smaller than the data length queried by calling HI_UNF_DMX_GetTSBuffer, it indicates that a part of data is stored.
CNcomment:�˽ӿڱ����Get�ӿ����ʹ�ã����û��Get�����Put�������򲻻�����κβ����������سɹ�\n
Put�ĳ������Ϊ0��Ҳ��������κβ���ֱ�ӷ��سɹ�\n
������Put���ȴ���ʵ��Get���ĳ��ȣ����򷵻ز����Ƿ�������\n
����Put����С��Get���ȣ�����ֻ������һ��������\n
Ϊ���Ч�ʣ�Put�ĳ��Ⱦ�����Ҫ̫�̣�����������10��ts������Putһ�Σ�����̫�̻ᵼ�������½�\n
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\param[in] u32ValidDataLen Valid data length.CNcomment:��Ч���ݵĳ���
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_PutTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ValidDataLen);


/**
\brief Resets a TS buffer to clear its data.CNcomment:��λTS Buffer�����TS Buffer�е�����
\attention \n
If a TS buffer is reset, you can call HI_UNF_DMX_PutTSBuffer only after calling HI_UNF_DMX_GetTSBuffer.CNcomment:TS Buffer��λ����Ҫ����get����ִ��put����
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_ResetTSBuffer(HI_HANDLE hTsBuffer);


/**
\brief Obtains the status of a TS buffer. CNcomment:��ȡTS Buffer״̬
\attention \n
You can query the size of a TS buffer and the used buffer size by calling this API. CNcomment:���Բ�ѯTS Buffer��С�����õĿռ��С
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\param[out] pStatus  TS buffer status.CNcomment:TS Buffer״̬�ṹ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSBufferStatus(HI_HANDLE hTsBuffer, HI_UNF_DMX_TSBUF_STATUS_S *pStatus);

/**
\brief Obtains the ID of the port corresponding to a TS buffer.CNcomment:��ȡTS Buffer��Ӧ��Port ID
\attention \n
If you query the ID of the port corresponding to a TS buffer that is not applied for successfully, an error code is returned. 
CNcomment:����û�������TS buffer,��ѯ�᷵�ش���
\param[in] hTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\param[out] penPortId  ID of the port.corresponding to a TS buffer  CNcomment:TS Buffer��Ӧ��Port ID
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSBufferPortId(HI_HANDLE hTsBuffer, HI_UNF_DMX_PORT_E *penPortId);

/**
\brief Obtains the handle of the TS buffer corresponding to a port ID.CNcomment:��ȡPort ID��Ӧ��TS Buffer handle
\attention \n
If you obtain the handle of a TS buffer that is not created, an error code is returned.CNcomment:����û�������TS buffer,��ѯ�᷵�ش���
\param[in] enPortId   Port ID
\param[out] phTsBuffer Handle of a TS buffer.CNcomment:TS Buffer���
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetTSBufferHandle(HI_UNF_DMX_PORT_E enPortId, HI_HANDLE *phTsBuffer);

/**
\brief Obtains the information about TS packets of a port.CNcomment:��ȡ�˿ڵ�TS����Ϣ
\attention \n
If you query the information about an unattached port, an error code is returned. 
CNcomment:����û�а󶨵Ķ˿�,��ѯ�᷵�ش���
\param[in] u32PortId   Port ID
\param[out] sPortStat   Number of TS packets .CNcomment:TS��������Ϣ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/

HI_S32 HI_UNF_DMX_GetTSPortPacketNum(HI_U32 u32PortId, HI_UNF_DMX_PORT_PACKETNUM_S *sPortStat);

/**
\brief Obtains the default attributes of a channel.CNcomment:��ȡͨ��Ĭ������
\attention \n
It is recommended to query the default channel attributes before creating a channel, and then modify the attributes.\n
By default, the attributes of the Section channel are returned. If other channels are used, you need to modify the attributes.
CNcomment:���������ͨ��֮ǰʹ�ô˽ӿڻ�ȡĬ�����ԣ�Ȼ��ı���Ҫ�޸ĵ����Լ���\n
Ĭ�����԰�һ���sectionͨ�����صģ����Ҫʹ���������͵�ͨ��ע���޸�����ֵ��
\param[out] pstChAttr Pointer to the structure of the channel attributes.CNcomment:ͨ�����Խṹ��ָ��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetChannelDefaultAttr(HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);


/**
\brief Creates a PID channel based on channel attributes.CNcomment:����ͨ�����Դ���һ��PIDͨ��
\attention \n
It is recommended to call HI_UNF_DMX_GetChannelDefaultAttr to query default channel attributes before calling HI_UNF_DMX_CreateChannel. 
For the Section channel and ECM/EMM channel, you can set the CRC mode, and dynamically change the CRC mode by calling HI_UNF_DMX_SetChannelAttr.\n
For other channels, the CRC check is always disabled. If you select other CRC modes, the CRC disable mode takes effect by default.\n
You need to set the buffer size when creating a channel. The buffer size is in the unit of byte and the default size is 16 KB.\n
It is recommended to set the buffer size to a value greater than 16 KB for the Section channel, ECM/EMM channel, or post channel. If the buffer size is smaller than 16 KB, the size of 16 KB is used by default.\n
It is recommended to set the buffer size to a value greater than 64 KB for the PES channel. If the buffer size is smaller than 64 KB, the size of 64 KB is used by default.\n
Because DEMUX_1, DEMUX_2, and DEMUX_3 support recording, they allow you to allocate audio/video recording channels for the PVR.\n
The playing channel and playing/recording channel cannot be created at the same time./n
The audio/video playing channels are allocated and managed by the player. 
CNcomment:������ô˽ӿ�֮ǰ�������ȵ���HI_UNF_DMX_GetChannelDefaultAttr��ȡͨ��Ĭ������\n
ֻ��section���͵�ͨ����ecm emm���͵�ͨ��֧������CRCģʽ�����ҿ���ͨ���������ýӿڶ�̬�޸�CRCģʽ\n
��������ͨ��ֻ֧������ΪCRC��ֹ�����ѡ�����������͵�CRC�ᱻĬ���޸�ΪCRC��ֹ\n
������ͨ��ʱע��Buffer��С�����ã�Ĭ�ϴ�СΪ16K��buffer��С�ĵ�λΪ�ֽ�\n
Section��ecm emm��POSTͨ������ʹ��16K���ϵ�buffer��С�����С��16K�ᱻĬ���޸�Ϊ16K\n
PESͨ������ʹ��64K���ϵ�buffer��С�����С��64K�ᱻĬ���޸�Ϊ64K\n
DEMUX_1��DEMUX_2��DEMUX_3֧��¼�ƹ��ܣ��������¼�����͵�����Ƶͨ������PVRģ��ʹ�ã�\n
�������ͺ�ͬʱ������¼�����͵�����Ƶͨ���ǲ��������\n
����Ƶ�Ĳ���ͨ����Playerͳһ����͹���
\param[in] u32DmxId  DEMUX ID
\param[in] pstChAttr  Pointer to the attributes of an input channel. For details, see the description of HI_UNF_DMX_CHAN_ATTR_S.CNcomment:ָ�����ͣ�����ͨ�����ԡ���μ�::HI_UNF_DMX_CHAN_ATTR_S
\param[out] phChannel  Pointer to the handle of an allocated channel.CNcomment:ָ�����ͣ���������ͨ��Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOFREE_CHAN  There is no available channel.CNcomment:û�п���ͨ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
            HI_HANDLE *phChannel);


/**
\brief Destroys an existing PID channel.CNcomment:���ٴ�����PIDͨ��
\attention \n
If you release an enabled channel, the channel is disabled automatically. If you release a channel that is attached to a filter or key area, the channel is detached from the filter or key area automatically.\n
However, the filter or key area is not released.\n
It is recommended to explicitly detach channels by calling corresponding detach APIs. In this way, resources are managed in a unified manner. 
CNcomment:�ͷ�ͨ��ʱ���ͨ�����ڴ�״̬���Զ��ر�ͨ�������ͨ���ϰ��˹���������Կ�������Զ����\n
���ǲ������ͷŹ���������Կ��
����ʹ�ý�󶨽ӿڽ�����ʽ�Ľ�󶨲���������Դ��ʹ�ý��������Ĺ���
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\retval none
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DestroyChannel(HI_HANDLE hChannel);


/**
\brief Obtains the attributes of a channel.CNcomment:��ȡͨ������
\attention \n
The API can be called only after a channel is allocated.CNcomment:���ӿڱ�����ͨ�������Ժ�ſ��Ե���
\param[in] hChannel   Channel handle.CNcomment:ͨ��Handle��
\param[out] pstChAttr  Pointer to the attributes of a specified channel. For details, see the description of HI_UNF_DMX_CHAN_ATTR_S.CNcomment:ָ�����ͣ����ָ��ͨ�����ԡ���μ�::HI_UNF_DMX_CHAN_ATTR_S
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid.CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);


/**
\brief Sets the attributes of a channel.CNcomment:����ͨ������
\attention \n
The API can be called only after a channel is allocated.\n
You cannot dynamically modify all attributes except the CRC mode by calling HI_UNF_DMX_SetChannelAttr.\n
To be specific, the CRC mode can be changed dynamically after a channel is created.\n
If you modify other attributes, the error code HI_ERR_DMX_INVALID_PARA is returned.
CNcomment:���ӿڱ�����ͨ�������Ժ�ſ��Ե���\n
����CRCģʽ�������Զ�������ʹ��HI_UNF_DMX_SetChannelAttr�ӿڽ��ж�̬�޸�\n
CRCģʽ������ͨ������������ʱ���޸ģ���̬��Ч\n
Ҫ�޸��������Ի᷵�ز����Ƿ�������
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\param[in] pstChAttr    Pointer to the attributes of an input channel. For details, see the description of HI_UNF_DMX_CHAN_ATTR_S.  CNcomment:ָ�����ͣ�����ͨ�����ԡ���μ� ::HI_UNF_DMX_CHAN_ATTR_S
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetChannelAttr(HI_HANDLE hChannel, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr);


/**
\brief Sets the PID of a channel.CNcomment:����ͨ��PID
\attention \n
If you set the same PID for two channels of a DEMUX, the PID of the second channel is valid, and the PID of the first channel is changed to 0x1FFF.\n
You must set the PID of a channel after the channel is disabled.\n
If you set the PID of a channel to 0x1FFF, the channel cannot receive data. That is, the channel is disabled. 
CNcomment:ͬһ·DEMUX������ͨ��������ͬ��PID�������õ�ͨ����Ч�������õ�ͨ����PID���޸�Ϊ0x1fff\n
������ͨ���رյ�״̬������ͨ����PID\n
����ͨ����PID����Ϊ0x1fff,����Ϊ0x1fff�󲻽����κ����ݣ��൱�ڹر�ͨ��
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\param[in] u32Pid     Channel PID.CNcomment:ͨ��PID
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_OPENING_CHAN  The channel is enabled.CNcomment:ͨ�����ڴ�״̬
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid);


/**
\brief Obtains the PID of a channel.CNcomment:��ȡͨ�����õ�PID
\attention \n
If no PID is set for a channel, the obtained value is 0x1FFF.\n
You can obtain the channel PID after a channel is created. 
CNcomment:û������PIDǰ����ȡ����PIDΪ0x1fff\n
������ͨ������������ʱ�̻�ȡͨ��PID
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\param[out] pu32Pid   Pointer to the PID of a channel (output).CNcomment:ָ�����ͣ����ͨ����PID
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid);


/**
\brief Enables a channel.CNcomment:��ͨ��
\attention \n
If you call this API to enable a PID channel that has been enabled, no error occurs.\n
If you call this API to enable a Section channel that is not attached to a filter, the channel cannot receive data.\n
If you want to receive data without filter, you can create a filter and then set its filter depth to 0.
CNcomment:��PIDͨ����״̬�£����ñ��ӿڴ�ͨ�����ᷢ������\n
����sectionͨ������ͨ��û�а󶨹�����ʱ���ñ��ӿڣ�ͨ�����ܽ�������\n
�������ʹ��filter�������ݣ���������һ��filter�������������Ϊ0���������ݽ���
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_OpenChannel(HI_HANDLE hChannel);


/**
\brief Disables a channel.CNcomment:�ر�ͨ��
\attention \n
If you this API to disable a PID channel that has been disabled, no error occurs.CNcomment:��PIDͨ���ر�״̬�£����ñ��ӿڹر�ͨ�����ᷢ������
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CloseChannel(HI_HANDLE hChannel);


/**
\brief Obtains the status of a channel.CNcomment:��ȡͨ��״̬
\attention \n
You can call this API after a channel is created.CNcomment:������ͨ������������ʱ�̵���
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\param[out] pstStatus  Pointer to the status of a channel (output). For details, see the description of ::HI_UNF_DMX_CHAN_STATUS_S.CNcomment:ָ�����ͣ����ͨ��״̬����μ�::HI_UNF_DMX_CHAN_STATUS_S
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus);

/**
\brief Obtains the ts count of a channel.CNcomment:��ȡͨ��ts������
\attention \n
You can call this API after a channel is created.CNcomment:������ͨ������������ʱ�̵���
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\param[out] pu32TsCount  Pointer to the count of a channel (output).CNcomment:ָ�����ͣ����ͨ��ts��������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
 */
HI_S32 HI_UNF_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32 *pu32TsCount);



/**
\brief Obtains the handle of the channel with a specific PID.CNcomment:��ȡ����ĳPID��ͨ�����
\attention \n
You must specify the DEMUX ID when calling this API, because the channel PIDs of different DEMUXs can be the same.\n
You cannot query the PID that is equal to or greater than 0x1FFF; otherwise, the error code HI_ERR_DMX_INVALID_PARA is returned.\n
If no channel with a specific PID is found, an error code is returned.
CNcomment:��ͬDEMUX�豸��ͨ������������ͬ��PID�����ñ��ӿ���Ҫָ��DEMUX ID\n
�������ѯ0x1fff�����ϵķǷ�PIDֵ�����򷵻ز����Ƿ�������\n
���û�в�ѯ���κ�ͨ��������Ҫ��ѯ��PID�������ش�����
\param[in] u32DmxId    DEMUX ID
\param[in] u32Pid      Channel PID.CNcomment:ͨ��PID
\param[out] phChannel Pointer to channel handle.CNcomment: ָ�����ͣ����ͨ��Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_UNMATCH_CHAN  No matched channel is found.CNcomment:û�в�ѯ��ƥ��ͨ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetChannelHandle(HI_U32 u32DmxId , HI_U32 u32Pid, HI_HANDLE *phChannel);


/**
\brief Obtains the number of available channels of a DEMUX.CNcomment:��ȡDEMUX�豸���õĿ���ͨ��������
\attention \n
The returned number is the number of all remaining channels including the used audio/video channels.\n
Because channels are allocated in a unified manner, the parameter u32DmxId is invalid.
CNcomment:���ص�����Ϊ����ͨ��ʣ��������������ѱ�ʹ�õ�����Ƶͨ��\n
����ͨ����Դͳһ�������Բ���u32DmxId�Ѿ�û�����塣
\param[in] u32DmxId         DEMUX ID��
\param[out] pu32FreeCount   Pointer to the number of available channels of a DEMUX.CNcomment:ָ�����ͣ����DEMUX����ͨ����Ŀ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFreeChannelCount (HI_U32 u32DmxId, HI_U32 *pu32FreeCount);


/**
\brief Obtains the scrambled flag of the channel data.CNcomment:��ȡͨ�����ݼ��ű�־
\attention \n
You can query the scrambled PES data and ES data.CNcomment:֧��PES�����ź�TS�����ŵĲ�ѯ
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\param[out] penScrambleFlag   Pointer to the scrambled flag of the channel data (output). For details, see the description of ::HI_UNF_DMX_SCRAMBLED_FLAG_E.CNcomment:ָ�����ͣ����ͨ�����ݼ��ű�־����μ�::HI_UNF_DMX_SCRAMBLED_FLAG_E
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag);


/**
\brief Creates a data filter.CNcomment:����һ�����ݹ�����
\attention \n
The maximum depth of a filter is 16 bytes.\n
For details on how to set the attributes of a filter, see the description of ::HI_UNF_DMX_FILTER_ATTR_S.\n
You can set a filter when creating it. If you do not want to filter data, you can set the filter depth to 0.\n
To modify the attributes of a filter, call HI_UNF_DMX_SetFilterAttr.\n
The attributes can be modified dynamically.\n
If a filter is used to filter the Section data, the valid byte is byte 1 and byte 4 to byte 18.\n
If a filter is used to filter the PES data, the valid byte is byte 4 and byte 7 to byte 21.\n
The filter can be used to filter the data of general-purpose channels only, including the Section channel, ECM/EMM channel, and PES channel.\n
If a filter is allocated successfully, phFilter returns the handle of the created filter.
CNcomment:������������Ϊ16\n
����������������ο�::HI_UNF_DMX_FILTER_ATTR_S\n
����������ʱ��ͬʱʵ�ֶԹ����������ã�������Ҫ���ݹ��ˣ����ù��������Ϊ0����\n
����Ҫ�޸Ĺ��������ԣ��ɵ���HI_UNF_DMX_SetFilterAttr���������޸�\n
֧�����ԵĶ�̬�޸�\n
���������ڹ���Section����ʱ����Ч�����ֽ�Ϊ��1�ֽڼ���4�ֽڡ���18�ֽ�\n
���������ڹ���PES����ʱ����Ч�����ֽ�Ϊ��4�ֽڼ���7�ֽڡ���21�ֽ�\n
������ֻ������ͨ��ͨ�������ݹ��ˣ�����sectionͨ����ECM/EMMͨ����PESͨ��\n
������ɹ���phFilter���ػ�ȡ�Ĺ�����Handle
\param[in] u32DmxId    DEMUX ID��
\param[in] pstFilterAttr   Pointer to the attributes of a filter (input).CNcomment:ָ�����ͣ�������������ԡ�
\param[out] phFilter   Pointer to the handle of the created filter (output).CNcomment:ָ�����ͣ��������Ĺ�����Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOFREE_FILTER There is no available filter.CNcomment:û�п���filter
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CreateFilter(HI_U32 u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S  *pstFilterAttr,
            HI_HANDLE *phFilter);


/**
\brief Destroys an existing data filter.CNcomment:���ٴ��������ݹ�����
\attention \n
If a filter is attached to a general-purpose channel, you can call this API to release the filter when the channel is enabled or disabled.\n
If no filter is attached to a Section channel, the channel cannot receive data. This restriction is not applicable to other channels.
CNcomment:����������ͨ��ͨ���󶨣�������ͨ���򿪻��߹ر�״̬�£����ñ��ӿ��ͷŹ�����\n
����sectionͨ����ͨ����û���κι�������ʱ�����ٽ������ݣ�����ͨ��û�д�����
\param[in] hFilter  Filter handle.CNcomment:�����������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DestroyFilter(HI_HANDLE hFilter);


/**
\brief Releases all the filters attached to a channel.CNcomment:�ͷŰ���ĳ��ͨ���ϵ����й�����
\attention \n
You can call this API to release all filters attached to a channel when the channel is enabled or disabled.CNcomment:������ͨ���򿪻��߹ر�״̬�£����ñ��ӿ��ͷ�ͨ���ϰ󶨵����й�����
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is valid.CNcomment:��������Ƿ�
\see \n
N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DeleteAllFilter(HI_HANDLE hChannel);


/**
\brief Sets the filter criteria of a filter.CNcomment:���ù������Ĺ�������
\attention \n
The maximum filter depth u32FilterDepth is 16.\n
For details on how to set the attributes of a filter, see the description of ::HI_UNF_DMX_FILTER_ATTR_S.\n
If a filter is used to filter the Section data, the valid byte is byte 1 and byte 4 to byte 18.\n
If a filter is used to filter the PES data, the valid byte is byte 4 and byte 7 to byte 21.\n
The attributes can be modified dynamically.
CNcomment:���������������u32FilterDepth���Ϊ16\n
����������������ο�::HI_UNF_DMX_FILTER_ATTR_S\n
���������ڹ���Section����ʱ����Ч�����ֽ�Ϊ��1�ֽڼ���4�ֽڡ���18�ֽ�\n
���������ڹ���PES����ʱ����Ч�����ֽ�Ϊ��4�ֽڼ���7�ֽڡ���21�ֽ�\n
֧�����ԵĶ�̬�޸�
\param[in] hFilter   Filter handle.CNcomment:�����������
\param[in] pstFilterAttr   Pointer to the attributes of a filter (input).CNcomment:ָ�����ͣ�������������ԡ�
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_ATTACHED_FILTER  The attributes cannot be set dynamically, because the filter is attached to a channel.CNcomment:����������ͨ���ϣ����ܶ�̬����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetFilterAttr(HI_HANDLE hFilter, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr );


/**
\brief Obtains the filter criteria of a filter.CNcomment:��ȡ�������Ĺ�������
\attention \n
If you call the API before a filter is allocated, an error code is returned.
CNcomment:�ڹ�����δ����ʱ���øýӿڣ����ش���
\param[in] hFilter   Filter handle.CNcomment:�����������
\param[out] pstFilterAttr   Pointer to the attributes of a filter (output). For details, see the description of ::HI_UNF_DMX_FILTER_ATTR_S.CNcomment:ָ�����ͣ�������������ԡ���μ�::HI_UNF_DMX_FILTER_ATTR_S
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFilterAttr(HI_HANDLE hFilter, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr );


/**
\brief  Attaches filters to a specific channel.CNcomment:�󶨹�������ָ����ͨ����
\attention \n
One or more filters can be attached to a general-purpose channel. All filters are ORed.\n
That is, if the data meets the filter criteria of a filter, the data is transmitted to the DMA buffer.\n
Filters cannot be attached to the audio/video channel and post channel.\n
If a filter is used, it cannot be attached to one or more channels repeatedly.\n
If a filter and a channel do not belong to the same DEMUX, the filter cannot be detached from the channel.\n
A filter can be attached to a general-purpose channel when the channel is enabled or disabled.
CNcomment:һ��ͨ��ͨ�����԰�һ�����߶�����ݹ����������й�����֮��Ĺ��˹�ϵ�ǻ�\n
��ֻҪ���������κ�һ���������Ĺ�����������ǰ���˵����ݶ���ͨ�����˶�������DMA������\n
��֧������Ƶͨ����POSTͨ���Ĺ�������\n
���ܽ�һ���������ظ��󶨵�һ������ͨ����\n
��������ͨ��������ͬһ·DEMUXʱ�����ܽ��а󶨲���\n
֧��ͨ��ͨ���򿪻��߹ر�״̬�µĹ�������
\param[in] hFilter  Filter handle.CNcomment:�����������
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_ATTACHED_FILTER  The filter is attached.CNcomment:�������Ѿ����ڰ�״̬
\retval ::HI_ERR_DMX_NOT_SUPPORT   No filter can be attached to the channel.CNcomment:������ͨ����֧�ְ󶨹�����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);



/**
\brief Detaches a filter from a specific channel.CNcomment:����������ָ��ͨ���Ͻ��
\attention \n
If no filter is attached to a channel, the error code HI_FAILURE is returned when you call this API.\n
If a filter and a channel do not belong to the same DEMUX, an error code is returned.\n
A filter can be detached from a general-purpose channel when the channel is enabled or disabled.
CNcomment:ͨ����û�а󶨹��������н�󶨷���ʧ��\n
��������ͨ��������ͬһ·DEMUXά��ʱ�����ش���\n
֧��ͨ��ͨ���򿪻��߹ر�״̬�µĹ��������

\param[in] hFilter   Filter handle.CNcomment:�����������
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NOATTACH_FILTER  No filter is attached to any channel.CNcomment:filterû�а����κ�ͨ����
\retval ::HI_ERR_DMX_UNMATCH_FILTER  No filter is attached to the channel.CNcomment:filterû�а��ڴ�ͨ����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel);


/**
\brief Obtains the handle of the channel to which a filter is attached.CNcomment:��ȡ���������󶨵�ͨ�����
\attention \n
Each filter corresponds to a channel. If a filter is not attached to a channel, the error code HI_ERR_DMX_NOATTACH_FILTER is returned.CNcomment:ÿ��������Ψһ��Ӧһ��ͨ������������û�а��κε�ͨ���ϣ��򷵻�HI_ERR_DMX_NOATTACH_FILTER������
\param[in] hFilter    Filter handle.CNcomment:�����������
\param[out] phChannel   Pointer to the handle of the channel to which a filter is attached (output).CNcomment:ָ�����ͣ�����������󶨵�ͨ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOATTACH_FILTER  No filter is attached to any channel.CNcomment:filterû�а����κ�ͨ����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFilterChannelHandle(HI_HANDLE hFilter, HI_HANDLE *phChannel);


/**
\brief Obtains the number of available filters of a DEMUX.CNcomment:��ȡDEMUX�豸���õĿ��й���������
\attention \n
The number of unallocated filters is returned. The filters that are allocated but not attached to channels are excluded.
CNcomment:���ص�ǰδ����Ĺ�������Ŀ���ѷ��䵫δ�󶨵�ͨ���ϵĹ�������ͳ��
\param[in] u32DmxId   DEMUX ID��
\param[out]  pu32FreeCount  Pointer to the number of allocated filters (output).CNcomment:ָ�����ͣ������ǰδ����Ĺ�������Ŀ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFreeFilterCount(HI_U32 u32DmxId ,  HI_U32 * pu32FreeCount);


/**
\brief Creates a key area.CNcomment:����һ����Կ��
\attention \n
When creating a key area, you can ignore the DUMUX to which the key area belongs, because all DEMUXs share all key areas.
CNcomment:������Կ�������ù���������·DEMUX������DEMUX����������Կ��
\param[in] u32DmxId   DEMUX ID��
\param[out] phKey     Pointer to the handle of a created key area.CNcomment:ָ�����ͣ�������뵽����Կ��Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOFREE_KEY There is no available key area. CNcomment:û�п��е���Կ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CreateDescrambler(HI_U32 u32DmxId, HI_HANDLE *phKey);


/**
\brief Creates a key area. The key area type and descrambling protocol type can be selected.CNcomment:����һ����Կ��,֧��ѡ��߰�ȫCA�ͽ���Э������
\attention \n
When an advanced CA key area is created, the descrambling protocol depends on the hardware and interface settings are ignored.\n
The number of advanced CA key areas also depends on the hardware. Therefore, before creating an advanced key area, you need to call HI_UNF_DMX_GetFreeDescramblerCountExt to check the number of available CA key areas.
CNcomment:����Ǹ߰�ȫCA������Э���Ѿ���Ӳ���������ӿڵ����ñ�����\n
�߰�ȫCA�ĸ���Ҳ����Ӳ�������ģ�ʹ��ǰ���ʹ��HI_UNF_DMX_GetFreeDescramblerCountExt��һ��ʣ��ĸ���
\param[in] u32DmxId   DEMUX ID
\param[in] pstDesramblerAttr  Pointer to the attributes of a key area.CNcomment:��Կ������ָ��  
\param[out] phKey      Pointer to the handle of a created key area.CNcomment:ָ�����ͣ�������뵽����Կ��Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOFREE_KEY  There is no available key area.CNcomment:û�п��е���Կ��
\retval ::HI_ERR_DMX_NOT_SUPPORT  Not support HI_UNF_DMX_DESCRAMBLER_ATTR_S type.CNcomment:��֧�ֵ�HI_UNF_DMX_DESCRAMBLER_ATTR_S����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_CreateDescramblerExt(HI_U32 u32DmxId, const HI_UNF_DMX_DESCRAMBLER_ATTR_S *pstDesramblerAttr, HI_HANDLE *phKey);

/**
\brief Destroys an existing key area.CNcomment:���ٴ�������Կ��
\attention \n
If a key area is attached to a channel, the key area needs to be detached from the channel first, but the channel is not disabled.\n
If a key area is detached or destroyed before the attached channel is disabled, an error may occur during data receiving.
CNcomment:�����Կ������ͨ���ϣ����ȴ�ͨ���Ͻ����Կ��������ע�ⲻ��ر�ͨ��\n
���û�йر�ͨ���������Կ���Ľ�󶨻����ٲ��������ܵ������ݽ��յĴ���
\param[in] hKey  Handle of the key area to be destroyed.CNcomment:��ɾ������Կ��Handle��
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DestroyDescrambler(HI_HANDLE hKey);


/**
\brief Sets the even keys of a key area. This API is used to configure the DEMUX descrambler based on even keys after the CA system obtains control words.CNcomment:������Կ����ż��Կ��CAϵͳ�õ������ֺ󣬿ɵ��ñ��ӿڽ�ż��Կ���õ�DEMUX����ģ��
\attention \n
pEvenKey points to the even key data to be set. The data consists of 16 bytes: CW1, CW2, ?-, and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.\n
The initial value of each key is 0, which indicates that data is not descrambled.
CNcomment:pEvenKeyָ��Ҫ���õ�ż��Կ���ݡ����ݹ�16byte��byte������CW1��CW2��������CW16\n
֧����Կ���Ķ�̬���ã���������Կ������������ʱ��������Կֵ\n
��������Կ֮ǰ����Կ�ĳ�ʱֵ����0����ʾ��������
\param[in] hKey  Handle of the key area to be set.CNcomment:�����õ���Կ�������
\param[in] pu8EvenKey  Pointer to the 16-byte even key data to be set.CNcomment:ָ�����ͣ�ָ��Ҫ���õ�ż��Կ���ݣ�������16���ֽڵ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetDescramblerEvenKey(HI_HANDLE hKey, const HI_U8 *pu8EvenKey);


/**
\brief Sets the odd keys of a key area. This API is used to configure the DEMUX descrambler based on odd keys after the CA system obtains control words.CNcomment:������Կ��������Կ��CAϵͳ�õ������ֺ󣬿ɵ��ñ��ӿڽ�����Կ���õ�DEMUX����ģ��
\attention \n
pOddKey points to the odd key data to be set. The data consists of 16 bytes: CW1, CW2, ?-, and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.\n
The initial value of each key is 0, which indicates that data is not descrambled.
CNcomment:pOddKeyָ��Ҫ���õ�����Կ���ݡ�����Կ���ݹ�16byte��byte������CW1��CW2��������CW16\n
֧����Կ���Ķ�̬���ã���������Կ������������ʱ��������Կֵ\n
��������Կ֮ǰ����Կ�ĳ�ʱֵ����0����ʾ��������
\param[in] hKey  Handle of the key area to be set.CNcomment:�����õ���Կ�������
\param[in] pu8OddKey   Pointer to the 16-byte odd key data to be set.CNcomment:ָ�����ͣ�ָ��Ҫ���õ�����Կ���ݣ�������16���ֽڵ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetDescramblerOddKey(HI_HANDLE hKey, const HI_U8 *pu8OddKey);


/**
\brief Set Even IV.For algs do not use IV,do not care.CNcomment:����ż��Կ���ĳ�ʼ�����������ڲ��漰��ʼ�������Ľ����㷨���Բ���ע��
\attention \n
pu8IVKey points to the iv key data to be set.The data consists of 16 bytes: CW1, CW2, ?-, and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.
CNcomment:pu8IVKeyָ��Ҫ���õĳ�ʼ���������ݡ�����Կ���ݹ�16byte��byte������CW1��CW2��������CW16\n
֧����Կ���Ķ�̬���ã���������Կ������������ʱ������
\param[in] hKey  Handle of the key area to be set.CNcomment:�����õ���Կ�������
\param[in] pu8IVKey   Pointer to the 16-byte IV key data to be set.CNcomment:ָ�����ͣ�ָ��Ҫ���õ�����Կ���ݣ�������16���ֽڵ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetDescramblerEvenIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);
/**
\brief Set Odd IV.For algs do not use IV,do not care.CNcomment:��������Կ���ĳ�ʼ�����������ڲ��漰��ʼ�������Ľ����㷨���Բ���ע��
\attention \n
pu8IVKey points to the iv key data to be set.The data consists of 16 bytes: CW1, CW2, ?-, and CW16.\n
The key value can be set dynamically, that is, the key value can be set at any time after a key area is created.
CNcomment:pu8IVKeyָ��Ҫ���õĳ�ʼ���������ݡ�����Կ���ݹ�16byte��byte������CW1��CW2��������CW16\n
֧����Կ���Ķ�̬���ã���������Կ������������ʱ������
\param[in] hKey  Handle of the key area to be set.CNcomment:�����õ���Կ�������
\param[in] pu8IVKey    Pointer to the 16-byte IV key data to be set.CNcomment:ָ�����ͣ�ָ��Ҫ���õ�����Կ���ݣ�������16���ֽڵ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_SetDescramblerOddIVKey(HI_HANDLE hKey, const HI_U8 *pu8IVKey);

/**
\brief Attaches a key area to a specific channel.CNcomment:����Կ����ָ��ͨ��
\attention \n
A key area can be attached to multiple channels that belong to different DEMUXs.\n
The static loading data in the key areas that are attached to all types of channels can be descrambled.\n
The same key area or different key areas cannot be attached to the same channel.
CNcomment:һ����Կ�����԰󶨵����ͨ���ϣ�ͨ���������ڲ�·��DEMUX\n
���Զ��������͵�ͨ������Կ�����о������ݵĽ���\n
�������ظ�����ͬ��ͬ����Կ����ͬһ��ͨ����
\param[in] hKey    Handle of the key area to be attached.CNcomment:���󶨵���Կ�������
\param[in] hChannel   Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_ATTACHED_KEY  A key area is attached to the channel.CNcomment:ͨ�����Ѿ���һ����Կ����������
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_AttachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);


/**
\brief Detaches a key area from a channel.CNcomment:����Կ����ͨ���Ͻ��
\attention \n
The key area used by a channel can be detached dynamically. That is, you can call this API to detach a key area at any time after it is attached.\n
The scrambled data, however, may not be descrambled after the key area is detached, which causes data error.\n
The value of a key area retains even after it is detached. If the key area is attached again, its value is still the previously configured value.\n
If you do not want to descramble data, you can detach the corresponding key area or set all key values to 0.
CNcomment:���Զ�̬�Ľ��ͨ��ʹ�õ���Կ���������ڰ󶨺������ʱ��ʹ�ô˽ӿڽ����Կ��\n
���ǽ�󶨺���ܵ��¼�������û�б����ţ��������ݴ���\n
�����Կ�������ܸı���Կ����ֵ��������°���Կ������Կֵ��Ȼ���ϴ����õ�ֵ\n
���������н��ţ����˽����Կ��֮�⣬Ҳ����ֱ�ӽ���Կֵȫ������Ϊ0��ʵ��
\param[in] hKey    Handle of the key area to be detached.CNcomment:����󶨵���Կ�������
\param[in] hChannel  Channel handle.CNcomment:ͨ�������
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NOATTACH_KEY  No key areas are attached to the channel.CNcomment:ͨ����û�а��κ���Կ��
\retval ::HI_ERR_DMX_UNMATCH_KEY  The specified key area is not attached to the specified channel.CNcomment:ָ������Կ��û�а���ָ����ͨ����
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_DetachDescrambler(HI_HANDLE hKey, HI_HANDLE hChannel);


/**
\brief Obtains the handle of the key area that is attached to a channel.CNcomment:��ȡͨ���󶨵���Կ�����
\attention \n
If no key area is attached to the channel, the error code HI_ERR_DMX_NOATTACH_KEY is returned when you call this API.
CNcomment:��ͨ��û�а���Կ��ʱ�����ñ��ӿڷ���HI_ERR_DMX_NOATTACH_KEY������
\param[in] hChannel  Handle of the channel to be queried.CNcomment:Ҫ��ѯ��ͨ�������
\param[out] phKey     Pointer to the handle of the key area that is attached to a channel (output).CNcomment:ָ�����ͣ����ͨ���󶨵���Կ�����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOATTACH_KEY  No key areas are attached to the channel.CNcomment:ͨ����û�а��κ���Կ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetDescramblerKeyHandle(HI_HANDLE hChannel, HI_HANDLE *phKey);


/**
\brief Obtains the number of available key areas.CNcomment:��ȡ������Կ������
\attention \n
Because key areas are shared by multiple DEMUXs, the first parameter is meaningless. Therefore, you only need to enter a valid value.CNcomment:��Կ���ڼ�·DEMUX֮�乲�ã����Ե�һ������û�����壬ֻҪ����һ���Ϸ�ֵ�Ϳ����ˡ�
\param[in] u32DmxId   DEMUX ID
\param[out]  pu32FreeCount   Pointer to the number of available key areas (output).CNcomment:ָ�����ͣ����������Կ����Ŀ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFreeDescramblerKeyCount(HI_U32 u32DmxId , HI_U32 * pu32FreeCount);

/**
\brief  Obtains the number of available key areas, including advanced CA key areas and common CA key areas.CNcomment:��ȡ������Կ�������������߰�ȫCA����ͨCA
\attention \n
This API can be used to query the numbers of advanced CA key areas and common CA key areas.
CNcomment:������Ϊ�߰�ȫCA����ͨCA������ѯ�Ľӿ�ʹ�á�
\param[out]  pu32FreeNormal  Pointer to the number of available common CA key areas.CNcomment:ָ�����ͣ����ͨ��CA������Կ����Ŀ
\param[out]  pu32FreeAdvance   Pointer to the number of available advanced CA key areas.CNcomment:ָ�����ͣ�����߰�ȫCA������Կ����Ŀ
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32 HI_UNF_DMX_GetFreeDescramblerCountExt(HI_U32 * pu32FreeNormal, HI_U32 * pu32FreeAdvance);



/**
\brief Obtains the handles of all channels that receive new data.CNcomment:��ȡ�������ݵ��������ͨ���ľ��
\attention \n
When you call this API, the parameter pu32ChNum serves as the input parameter and is used to specify the data length pointed by phChannel. pu32ChNum indicates the number of channels to be queried.\n
The number of channels must be initialized, and the initial value cannot be 0.\n
It is recommended to set the number of channels to the total number. For details about the total number of channels, see the description of the macro definition ::DMX_TOTALCHAN_CNT.\n
After you call this API successfully, the parameter pu32ChNum serves as the output parameter and is used to specify the valid data length pointed by phChannel.\n
The block time is configurable (in ms). If u32TimeOutMs is set to 0, it indicates that the block time is 0; if u32TimeOutMs is set to 0xFFFFFFFF, it indicates infinite wait.\n
If no data is received when the block time is exceeded, the error code HI_ERR_DMX_TIMEOUT is returned.\n
If the wait time is 0, and no data is received in any channel, ::HI_ERR_DMX_NOAVAILABLE_DATA is returned.
CNcomment:���øýӿ�ʱ��pu32ChNum��Ϊ�������������ָ��phChannelָ������ĳ��ȣ���ʾҪ��ѯ��ͨ��������\n
ͨ�����������ʼ�����ҳ�ʼֵ����Ϊ��0ֵ��\n
���齫ͨ����������Ϊͨ�����ܸ���, ͨ���ܸ����ο��궨�� ::DMX_TOTALCHAN_CNT\n
���ýӿڵ��óɹ�ʱ��pu32ChNum��Ϊ���������ָ��phChannelָ���������Ч����\n
����ʱ��������ã�ʱ�䵥λΪ���룬����Ϊ0���ȴ�������Ϊ0xffffffffһֱ�ȴ���\n
����������ʱ�䣬����ͨ����û�����ݵ���򷵻صȴ���ʱ����\n
����ȴ�ʱ��Ϊ0������û���κ�ͨ���������򷵻�::HI_ERR_DMX_NOAVAILABLE_DATA
\param[out] phChannel      Pointer to the number of channels. The number of channels depends on the value of pu32ChNum.CNcomment:ָ�����ͣ�ָ��ͨ��������顣���鳤����pu32ChNum����
\param[in] pu32ChNum   	Pointer type
                           Input: indicates the number of channel handles to which pChannelHandle points.
                           Output: indicates the number of lower-layer channels that receive new data. The maximum value cannot be greater than the input value.
						   CNcomment:ָ�����͡�ע��:�ò���ͬʱ��Ϊ����������
                           ���룺pChannelHandleָ������ĳ��ȡ�
                           ������ײ���������ͨ������Ŀ����಻�����������ֵ
\param[in] u32TimeOutMs      Wait timeout, in ms.CNcomment:�ȴ���ʱʱ�䣬��λms
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOAVAILABLE_DATA  No data is received in any channel.CNcomment:û���κ�ͨ�������ݵ���
\retval ::HI_ERR_DMX_TIMEOUT  The wait times out.CNcomment:�ȴ���ʱ
\see \n
 N/A.CNcomment:��
*/
/*#define HI_UNF_DMX_GetDataHandle(phChannel, pu32ChNum, u32TimeOutMs) \
            HI_UNF_DMX_SelectDataHandle(NULL, *pu32ChNum, phChannel, pu32ChNum, u32TimeOutMs)*/
HI_S32  HI_UNF_DMX_GetDataHandle(HI_HANDLE *phChannel, HI_U32 *pu32ChNum,
            HI_U32 u32TimeOutMs);

/**
\brief Obtains whether data is received in some channels.CNcomment:��ѯ����ͨ���Ƿ������ݵ���
\attention \n
This API is a supplementary interface of ::HI_UNF_DMX_GetDataHandle. You can call this API when you only want to check whether data is received in some channels.
The handles of the channels to be queried are specified by phWatchChannel, and the number of channels to be queried is specified by u32WatchNum.\n
If phWatchChannel is NULL, all channels are queried. The value of u32WatchNum cannot be 0.\n
The block time is configurable (in ms). If u32TimeOutMs is set to 0, it indicates that the block time is 0; if u32TimeOutMs is set to 0xFFFFFFFF, it indicates infinite wait.\n
If no data is received when the block time is exceeded, the error code HI_ERR_DMX_TIMEOUT is returned.\n
If the wait time is 0, and no data is received in any channel, ::HI_ERR_DMX_NOAVAILABLE_DATA is returned.
CNcomment:������Ϊ::HI_UNF_DMX_GetDataHandle�Ĳ��䣬��Ӧ����ֻ��ע����ͨ�������ݵ������ʱʹ�ô˽ӿ�\n
������ͨ���ľ����phWatchChannelָ����������u32WatchNumָ��\n
phWatchChannelΪNULL,��ѯ����ͨ�������ݵ��������u32WatchNum����Ϊ��0ֵ\n
����ʱ��������ã�ʱ�䵥λΪ���룬����Ϊ0���ȴ�������Ϊ0xffffffffһֱ�ȴ���\n
����������ʱ�䣬����ͨ����û�����ݵ���򷵻صȴ���ʱ����\n
����ȴ�ʱ��Ϊ0������û���κ�ͨ���������򷵻�::HI_ERR_DMX_NOAVAILABLE_DATA

\param[in] phWatchChannel   Pointer to the number of handles of the channels to be queried. The number depends on the value of u32WatchNum.CNcomment:ָ�����ͣ�ָ�򱻼���ͨ���ľ�����顣���鳤����u32WatchNum����
\param[in] u32WatchNum      Number of channels to be queried.CNcomment:����ͨ���ĸ���
\param[in] phDataChannel    Pointer to the number of handles of the channels that contain data The number depends on the value of pu32ChNum.CNcomment:ָ�����ͣ�ָ��������ݵ�ͨ���ľ�����顣���鳤����pu32ChNum����
\param[out] pu32ChNum       Pointer to the number of lower-layer channels that receive new data. The maximum value cannot be greater than the input value.CNcomment:ָ�����͡��ײ���������ͨ������Ŀ����಻���������ӵ�ͨ������ֵ
\param[in] u32TimeOutMs     Wait timeout, in ms.CNcomment:�ȴ���ʱʱ�䣬��λms
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR   The pointer is null.CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOAVAILABLE_DATA   No data is received in any channel.CNcomment:û���κ�ͨ�������ݵ���
\retval ::HI_ERR_DMX_TIMEOUT   The wait times out.CNcomment:�ȴ���ʱ
\see \n
 N/A.CNcomment:��
*/
HI_S32  HI_UNF_DMX_SelectDataHandle(HI_HANDLE *phWatchChannel, HI_U32 u32WatchNum,
            HI_HANDLE *phDataChannel, HI_U32 *pu32ChNum, HI_U32 u32TimeOutMs);


/**
\brief Obtains the received data packets from a specific channel.CNcomment:��ָ��ͨ���л�ȡ��������ݰ�
\attention \n
Before calling this API, you need to set the value of u32AcquireNum to specify the maximum number of buffers that can be queried. The number of buffers depends on the value of u32AcquireNum.\n
The value of pu32AcquiredNum is not greater than that of u32AcquireNum.\n
For the Section channel, ECM channel, or EMM channel, each data packet contains a complete Section.\n
For the PES channel, each data packet may contain a complete PES. If the PES is too large, it is divided into multiple PES packets to transfer.\n
The enDataType field determines whether a transferred data packet is complete.\n
For the post channel, each data packet may contain one or more complete TS packets. Each TS packet consists of 188 bytes.\n
For the audio/video channel, data cannot be obtained by calling this API. The audio data and video data are transferred to the decoder through internal interfaces.
You cannot call this API repeatedly, but you can call this API once and release buffers for several times. Note that you must release buffers in sequence, and the address and size of the buffer to be released must be consistent with those obtained by calling HI_UNF_DMX_AcquireBuf.
In addition, you can call HI_UNF_DMX_AcquireBuf again only after all data packets are released. Otherwise, the error code HI_ERR_DMX_REACQUIRE_BUF is returned.
CNcomment:�ڵ��ñ��ӿ�֮ǰ����u32AcquireNum��ֵ��ָ�������ѯ�Ļ�������Ϣ�����Ŀ����Ϣ����������ĳ�����u32AcquireNum����\n
ʵ�ʻ�ȡ�Ļ�������Ϣ��Ŀpu32AcquiredNum��ֵ���ᳬ��u32AcquireNum\n
����sectionͨ����ECM/EMMͨ����ÿ�����ݰ�����һ��������section\n
����PESͨ����ÿ�����ݰ���������������pes���������pes̫�󣬿���Ҫ�ֶ��pes�������\n
����������Ƿ�����ͨ�����ݰ��ṹ��enDataType�ֶ�ָ��\n
����POST���͵�ͨ����ÿ�����ݰ�����һ������������TS����TS����188�ֽ�\n
��������Ƶͨ���ǲ���ͨ���˽ӿڻ�ȡ���ݵģ�����Ƶ���ݻ�ͨ���ڲ��ӿ�ֱ���͸����������н���\n
�������ظ����ô˽ӿڣ�����һ���������ͷţ������ͷű��밴˳�򣬶����ͷŵĵ�ַ�ͳ��ȱ���������һ��\n
����ֻ���ͷ������е����ݰ������ٴ����󣬷��򷵻��ظ����������
\param[in] hChannel            Channel handle.CNcomment:ͨ�������
\param[in] u32AcquireNum       Maximum number of obtained data packets.CNcomment:��������ݰ���������
\param[out] pu32AcquiredNum    Pointer to the actual number of obtained data packets.CNcomment:ָ�����ͣ����ʵ�����󵽵����ݰ�����
\param[out] pstBuf             Pointer to the number of data packets. The number depends on the value of pu32AcquiredNum.CNcomment:ָ�����ͣ�ָ�����ݰ���Ϣ���飬���������pu32AcquiredNum����
\param[in] u32TimeOutMs        Wait timeout, in ms.CNcomment:�ȴ���ʱʱ�䣬��λΪms
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\retval ::HI_ERR_DMX_NOAVAILABLE_DATA  There is no valid data in the channel.CNcomment:ͨ����û����Ч����
\retval ::HI_ERR_DMX_TIMEOUT  The wait times out.CNcomment:�ȴ���ʱ
\retval ::HI_ERR_DMX_REACQUIRE_BUF  The data packet has been obtained.CNcomment:�ظ��������ݰ�
\see \n
 N/A.CNcomment:��
*/
HI_S32  HI_UNF_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
            HI_U32 * pu32AcquiredNum, HI_UNF_DMX_DATA_S *pstBuf,
            HI_U32 u32TimeOutMs);


/**
\brief Releases the buffers for storing data packets after data packets are processed.CNcomment:���������ݰ������ݺ��ͷ����ݰ�ռ�õĻ���ռ�
\attention \n
If you release buffers by calling this API, the buffers must be those obtained by calling ::HI_UNF_DMX_AcquireBuf, and the addresses and sizes of these buffers cannot be changed.\n
The number of released buffers can be less than the number of obtained buffers.
CNcomment:���ӿ��ͷŵĻ�����һ����ͨ��::HI_UNF_DMX_AcquireBuf��ȡ�Ļ��������������ĵ�ַ�ͳ��Ȳ��ܸ���\n
�ͷŵĻ�������������С�ڻ�ȡ�Ļ���������
\param[in] hChannel        Channel handl.CNcomment:ͨ�������
\param[in] u32ReleaseNum   Number of released buffers.CNcomment:�ͷ����ݰ�����
\param[in] pstBuf          Pointer to the number of released buffers. The number depends on the value of u32ReleaseNum.CNcomment:ָ�����ͣ�ָ���ͷŻ�������Ϣ���飬������u32ReleaseNum����
\retval ::HI_SUCCESS Success.CNcomment:�ɹ�
\retval ::HI_FAILURE  Calling this API fails.CNcomment:APIϵͳ����ʧ��
\retval ::HI_ERR_DMX_NOT_INIT  The DEMUX module is not initialized.CNcomment:ģ��û�г�ʼ��
\retval ::HI_ERR_DMX_INVALID_PARA  The input parameter is invalid. CNcomment:��������Ƿ�
\retval ::HI_ERR_DMX_NULL_PTR  The pointer is null. CNcomment:ָ�����Ϊ��
\see \n
 N/A.CNcomment:��
*/
HI_S32  HI_UNF_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum,
            HI_UNF_DMX_DATA_S *pstBuf);

/** @} */  /** <!-- ==== API Declaration End ==== */



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_UNF_DEMUX_H__ */

