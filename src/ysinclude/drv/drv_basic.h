#ifndef _DRV_BASIC_H_
#define _DRV_BASIC_H_
/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <stddefs.h>
#include "drv_typedef.h"
#include "drv_memory.h"
#include "drv_debug.h"


/***************************** Macro Definition ******************************/
/* CHIP_TYPE */
//#define CHIP_TYPE_hi3716mv100 
//#define CHIP_TYPE_hi3720v100
//#define CHIP_TYPE_hi3716hv100
//#define CHIP_TYPE_hi3716cv100

/* BOARD_TYPE */
//#define BOARD_TYPE_hi3716mstb
//#define BOARD_TYPE_hi3716mdmo 
//#define BOARD_TYPE_hi3716lstb
//#define BOARD_TYPE_hi3716hdmo
//#define BOARD_TYPE_hi3716cdmo

/* 3716MV200 */
#if  defined (BOARD_TYPE_hi3716mstb) 
/* TUNER */
#define TUNER_USE    0
#define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_CD1616
#define DEMOD_TYPE   HI_UNF_DEMOD_DEV_TYPE_3130I
#define I2C_CHANNEL  HI_UNF_I2C_CHANNEL_QAM
/* DAC */
#define DACMODE0 HI_UNF_DISP_DAC_MODE_HD_Y
#define DACMODE1 HI_UNF_DISP_DAC_MODE_HD_PR
#define DACMODE2 HI_UNF_DISP_DAC_MODE_HD_PB
#define DACMODE3 HI_UNF_DISP_DAC_MODE_SVIDEO_C
#define DACMODE4 HI_UNF_DISP_DAC_MODE_SVIDEO_Y
#define DACMODE5 HI_UNF_DISP_DAC_MODE_CVBS
/* SIO (only BOARD_TYPE_hi3716mstbverc) */
#define SLAC_RESET_GPIO_GNUM	11
#define SLAC_RESET_GPIO_PNUM	5
/* E2PROM */
#define E2PROM_I2C_NUM          2

/* 3716C*/
#elif defined (BOARD_TYPE_hi3716cdmoverb)
/* TUNER */
#define TUNER_USE    0
 #define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_CD1616
 #define DEMOD_TYPE   HI_UNF_DEMOD_DEV_TYPE_3130I
 #define I2C_CHANNEL  HI_UNF_I2C_CHANNEL_QAM

/* DAC */
#define DACMODE0 HI_UNF_DISP_DAC_MODE_HD_PR
#define DACMODE1 HI_UNF_DISP_DAC_MODE_HD_Y
#define DACMODE2 HI_UNF_DISP_DAC_MODE_HD_PB
#define DACMODE3 HI_UNF_DISP_DAC_MODE_SVIDEO_C
#define DACMODE4 HI_UNF_DISP_DAC_MODE_SVIDEO_Y
#define DACMODE5 HI_UNF_DISP_DAC_MODE_CVBS
/* SIO */
#define SLAC_RESET_GPIO_GNUM	1
#define SLAC_RESET_GPIO_PNUM	4
/* E2PROM */
#define E2PROM_I2C_NUM          3

/* 3716MV300 */
#elif defined (BOARD_TYPE_hi3716mdmo3avera)
/* TUNER */
#define TUNER_USE    0
#define TUNER_TYPE   HI_UNF_TUNER_DEV_TYPE_TDA18250
#define DEMOD_TYPE   HI_UNF_DEMOD_DEV_TYPE_3130I
#define I2C_CHANNEL  HI_UNF_I2C_CHANNEL_QAM
/* DAC */
#define DACMODE0 HI_UNF_DISP_DAC_MODE_CVBS
#define DACMODE1 HI_UNF_DISP_DAC_MODE_SVIDEO_Y
#define DACMODE2 HI_UNF_DISP_DAC_MODE_SVIDEO_C
#define DACMODE3 HI_UNF_DISP_DAC_MODE_HD_PB 
#define DACMODE4 HI_UNF_DISP_DAC_MODE_HD_Y
#define DACMODE5 HI_UNF_DISP_DAC_MODE_HD_PR
/* SIO (only BOARD_TYPE_hi3716mtst3avera) */
#define SLAC_RESET_GPIO_GNUM	8
#define SLAC_RESET_GPIO_PNUM	5
/* E2PROM */
#define E2PROM_I2C_NUM          2
#else
    #error YOU MUST DEFINE  BOARD_TYPE!
#endif


/* 3716系列硬件没有E2P需要flash和文件模拟*/
#define FILE_E2Prom

/* Exported Functions -------------------------------------------------- */

enum
{
    DRV_INIT_ERROR = DRV_INIT_BASE + 1,
    DRV_INIT_ERROR_BOOT_INIT,
    DRV_INIT_ERROR_PARTITION_CREATE,   
    DRV_INIT_ERROR_PIO_INIT,
    DRV_INIT_ERROR_PIO_TERM,
    DRV_INIT_ERROR_SPI_INIT,
    DRV_INIT_ERROR_MBX_INIT,
    DRV_INIT_ERROR_CLOCK_INFO,
    DRV_INIT_ERROR_UART_INIT,
    DRV_INIT_ERROR_UART_OPEN,
    DRV_INIT_ERROR_TBX_INIT,
    DRV_INIT_ERROR_TESTTOOL_INIT,
    DRV_INIT_ERROR_TESTTOOL_TERM,
    DRV_INIT_ERROR_EVT_INIT,
    DRV_INIT_ERROR_EVT_OPEN,
    DRV_INIT_ERROR_EVT_TERM,
    DRV_INIT_ERROR_FDMA_INIT,
    DRV_INIT_ERROR_AVMEM_INIT,
    DRV_INIT_ERROR_AVMEM_CREATE,
    DRV_INIT_ERROR_CLKRV_INIT,
    DRV_INIT_ERROR_CLKRV_OPEN,
    DRV_INIT_ERROR_CLKRV_ENABLE,
    DRV_INIT_ERROR_I2C_CLOCK,
    DRV_INIT_ERROR_I2C_INIT,
    DRV_INIT_ERROR_E2P_INIT,
    DRV_INIT_ERROR_E2P_OPEN,
    DRV_INIT_ERROR_E2P_TERM,
    DRV_INIT_ERROR_E2P_CLOSE,
    DRV_INIT_ERROR_AUDIO_INIT,
    DRV_INIT_ERROR_AUDIO_TERM,
    DRV_INIT_ERROR_AUDIO_OPEN,
    DRV_INIT_ERROR_AUDIO_CLOSE,
    DRV_INIT_ERROR_SMART_CLOCKINFO,
    DRV_INIT_ERROR_SMART_UART_INIT,
    DRV_INIT_ERROR_SMART_UART_TERM,
    DRV_INIT_ERROR_SMART_INIT,
    DRV_INIT_ERROR_SMART_TERM,
    DRV_INIT_ERROR_SMART_OPEN,
    DRV_INIT_ERROR_SMART_CLOSE,
    DRV_INIT_ERROR_DENC_INIT,
    DRV_INIT_ERROR_DENC_TERM,
    DRV_INIT_ERROR_DENC_OPEN,
    DRV_INIT_ERROR_DENC_CLOSE,
    DRV_INIT_ERROR_VOUT_INIT,
    DRV_INIT_ERROR_VOUT_TERM,
    DRV_INIT_ERROR_VOUT_OPEN,
    DRV_INIT_ERROR_VOUT_CLOSE,
    DRV_INIT_ERROR_VTG_INIT,
    DRV_INIT_ERROR_VTG_TERM,
    DRV_INIT_ERROR_VTG_OPEN,
    DRV_INIT_ERROR_VTG_CLOSE,
    DRV_INIT_ERROR_VMIX_INIT,
    DRV_INIT_ERROR_VMIX_TERM,
    DRV_INIT_ERROR_VMIX_OPEN,
    DRV_INIT_ERROR_VMIX_CLOSE,
    DRV_INIT_ERROR_LAYER_INIT,
    DRV_INIT_ERROR_LAYER_TERM,
    DRV_INIT_ERROR_LAYER_OPEN,
    DRV_INIT_ERROR_LAYER_CLOSE,
    DRV_INIT_ERROR_BLIT_INIT,
    DRV_INIT_ERROR_BLIT_OPEN,
    DRV_INIT_ERROR_HDMI_INIT,
    DRV_INIT_ERROR_HDMI_OPEN,
    DRV_INIT_ERROR_MERGE_INIT,
    DRV_INIT_ERROR_MERGE_CLOCKINFO,
    DRV_INIT_ERROR_MERGE_SET_PARAMS,
    DRV_INIT_ERROR_MERGE_CONNECT,
    DRV_INIT_ERROR_PTI_INIT,
    DRV_INIT_ERROR_PTI_OPEN,
    DRV_INIT_ERROR_VIDEO_INIT,
    DRV_INIT_ERROR_VIDEO_TERM,
    DRV_INIT_ERROR_VIDEO_OPEN,
    DRV_INIT_ERROR_VIDEO_CLOSE,
	DRV_INIT_ERROR_INJ_INT
};


/**/
extern yspartition_t *SystemPartition;

/**/

typedef enum PIO_Mode_s
{
 DRV_PIO_NONPIO,    /* Input, weak pull-up (default) */
 DRV_PIO_BIDIR,     /* Bidirectional, open drain     */
 DRV_PIO_OUT,       /* Output, push-pull             */
 DRV_PIO_IN,        /* Input, high-Z                 */
 DRV_PIO_ALT_OUT,   /* Alternate output, push-pull   */
 DRV_PIO_ALT_BIDIR  /* Alternate bidir, open drain   */
} DRV_PIO_Mode_t;


/**/
/**/

/**/

/**/

/**/

/**/

/**/
typedef enum
{
    I2C_NIM,
    I2C_HDMI,
    I2C_E2P,
    I2C_MAXDEVICE   /* For range test */
} I2C_DeviceId_t;


#define I2C_MAX_NUMBER I2C_MAXDEVICE

#define DRV_LOW_POWER_CALLBACK_CHECK     {\
         extern BOOL LowerPowerEnable;\
         if(LowerPowerEnable == TRUE)\
         {\
             return DRV_NO_ERROR;\
         }\
     }

#define DRV_LOW_POWER_CALLBACK_CHECK_NULL     {\
         extern BOOL LowerPowerEnable;\
         if(LowerPowerEnable == TRUE)\
         {\
             return;\
         }\
     }

#define DRV_LOW_POWER_TASK_CHECK     {\
         extern BOOL LowerPowerEnable;\
         if(LowerPowerEnable == TRUE)\
         {\
             DRV_OS_TaskDelay(312500);\
             DRV_LED_DisplayNULL();\
             continue;\
         }\
     }
/**/
U32 DRV_ChipVersion(void);
DRV_ErrCode DRV_PioBitConfigure(U32 PIO_Index,U32 PIO_Bit,DRV_PIO_Mode_t PIO_Mode);
DRV_ErrCode DRV_PioBitSet(U32 PIO_Index,U32 PIO_Bit,BOOL OnOff);
DRV_ErrCode DRV_ShowDriverVersion();
DRV_ErrCode DRV_InitBasic(void);
/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
