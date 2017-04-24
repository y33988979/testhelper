/*****************************************************************************

File Name  : lists.h

Description: export transponder lists.

Copyright (C) 2004 STMicroelectronics

*****************************************************************************/

/* Define to prevent recursive inclusion */
#ifndef __LISTS_H
#define __LISTS_H

/* Includes --------------------------------------------------------------- */

#include "stddefs.h"    /* STAPI includes */

#define MAX_NUM_CHANNELS  40
/* Exported Types ------------------------------------------------------ */
typedef enum SERVICE_Mode_e
{
    SERVICE_MODE_NONE,
    SERVICE_MODE_DVB
} SERVICE_Mode_t;


typedef enum SERVICE_Display_e
{
    SERVICE_DISPLAY_NONE,
    SERVICE_DISPLAY_PAL,
    SERVICE_DISPLAY_NTSC,
    SERVICE_DISPLAY_SECAM
} SERVICE_Display_t;
typedef enum
{
    BAND_US = 0,
    BAND_EU_LO,
    BAND_EU_HI,
    BAND_C,
    BAND_TER,
    BAND_CABLE,
    MAX_BANDS
} TUNER_BandNum_t;

/* Exported Types --------------------------------------------------------- */
typedef enum LISTS_StreamType_e
{
    NOPID   = -1,
    MP1V    =  1,
    MP2V,
    MP1A,
    MP2A,
    PRIVATE,
    TTXT,       /* TTXT & SUBT use same stream type (added to distinguish from TTXT) */
    SUBT,
    PCR,
    DSMCC,
    AC3     = 0x81,
    OTHER   = 0xff
} LISTS_StreamType_t;



typedef struct LISTS_Channel_s
{
    int                     Index;
    int                     Transponder;    /* Index into transponder list */
    SERVICE_Display_t       Display;        /* display mode (PAL/NTSC) */

    U16                     VideoPid;       /* channel pid information */
    U16                     AudioPid;
    U16                     PcrPid;
    U16                     TtxPid;
    U16                     SubtPid;

    LISTS_StreamType_t      AudioType;

    char                   *ChannelName;    /* channel name to display */
} LISTS_Channel_t;

/* Exported Constants ----------------------------------------------------- */

/* Exported Variables ----------------------------------------------------- */



/* Exported Macros -------------------------------------------------------- */
/* shorthand */

#define MODE_NONE   SERVICE_MODE_NONE
#define MODE_DVB    SERVICE_MODE_DVB


#define DISP_NONE   SERVICE_DISPLAY_NONE
#define DISP_PAL    SERVICE_DISPLAY_PAL
#define DISP_NTSC   SERVICE_DISPLAY_NTSC
#define ENDOFLIST   -1

/* Exported Functions ----------------------------------------------------- */

#endif /* __LISTS_H */

/* EOF -------------------------------------------------------------------- */
