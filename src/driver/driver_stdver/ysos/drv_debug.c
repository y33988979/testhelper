#include <stddefs.h>
#include <stdarg.h>
#include <errno.h>
#include "drv_debug.h"
#include "drv_basic.h"


static U32 mAllow_print = 1;

#define DEBUG_MASK (0xFFFFFFFE)
#define PRINT_MASK (0xFFFFFFFD)
#define ENABLE_DEBUG (~DEBUG_MASK)
#define ENABLE_PRINT (~PRINT_MASK)
#define ENABLE_ALL (ENABLE_PRINT|ENABLE_DEBUG)

#define CURRENT_MODULE DRV_MODULE_DEBUG

typedef struct ModuleControl_t
{
    char Name[8];
    BOOL_YS Enable;
}ModuleControl;

 
static ModuleControl m_ModuleControl[DRV_MODULE_MAX_NB] =
{
    {
        "  APP  ",
        ENABLE_ALL,
    },
    {
        " BASIC ",
        ENABLE_ALL,
    },
    {
        "  OS   ",
        ENABLE_ALL,
    },
    {
        " AUDIO ",
        ENABLE_ALL,
    },
    {
        "  CAS  ",
         ENABLE_ALL,
    },
    {    
        " DEBUG ",
         ENABLE_ALL,
    },
    {    
        "DISPLAY",
         ENABLE_ALL,
    },
    {    
        "FILTER ",
         ENABLE_ALL,
    },
    {    
        " INIT  ",
         ENABLE_ALL,
    },
    {    "  NVM  ",
         ENABLE_ALL,
    },
    {    "  OSD  ",
         ENABLE_ALL,
    },
    {    " PANEL ",
         ENABLE_ALL,
    },
    {
        "TURNER ",
         ENABLE_ALL,
    },
    {
        " VIDEO ",
        ENABLE_ALL,
    },
    {
        "  NET  ",
        ENABLE_ALL,
    },
    {
        "  DVM  ",
        ENABLE_ALL,
    },
    {
        "PLAYER ",
        ENABLE_ALL,
    },
	{
		"MISC ",
		ENABLE_ALL,
	},
	{
		"DISK ",
		ENABLE_ALL,
	},
	{
		"MPLAYER ",
		ENABLE_ALL,
	},
};

    

void drv_print(report_severity_t Severity, char *Format, ...)
{
    va_list list;

    if(mAllow_print == 0)
        return;

    switch(Severity)
    {
        case severity_fatal:
            fprintf(stdout,"***Fatal>");
            break;
        case severity_error:
            fprintf(stdout,"###Error>");
            break;
        case severity_warn:
            fprintf(stdout,"+++Warn >");
            break;
        case severity_info:
            fprintf(stdout,"---Info >");
            break;
    }
    va_start(list, Format);
    vfprintf(stdout, Format, list);
    va_end(list);
}

void do_report(report_severity_t Severity, char *Format, ...)
{
    va_list list;

    if(mAllow_print == 0)
        return;

    switch(Severity)
    {
        case severity_fatal:
            fprintf(stdout,"***Fatal>[ APP ]");
            break;
        case severity_error:
            fprintf(stdout,"###Error>[ APP ]");
            break;
        case severity_warn:
            if(drv_DebugIsEnabelDebug(DRV_MODULE_APP) == FALSE)
                return;
            fprintf(stdout,"+++Warn >[ APP ]");
            break;
        case severity_info:
            if(drv_DebugIsEnabelPrint(DRV_MODULE_APP) == FALSE)
                return;
            fprintf(stdout,"---Info >[ APP }");
            break;
    }
    va_start(list, Format);
    vfprintf(stdout, Format, list);
    va_end(list);
}   


BOOL_YS drv_DebugIsEnabelPrint(DRV_Module Module)
{
    if(m_ModuleControl[Module].Enable & ~(PRINT_MASK))
        return 1;

    else
        return 0;
    
}

BOOL_YS drv_DebugIsEnabelDebug(DRV_Module Module)
{
    if(m_ModuleControl[Module].Enable & ~(DEBUG_MASK))
        return 1;

    else
        return 0;
    
}

/*****************************************************************************
* Function    : DRV_DebugGetRevision
* Description :
*   获得Debug模块的驱动版本号.
* Note        :
*  
******************************************************************************/
DRV_Revision DRV_DebugGetRevision(void)
{
    return "1.0";
}

DRV_ErrCode DRV_DebugGetPlatformId(U32_YS *PlatformId_p)
{
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugSetPlatformErrorCode(U32_YS ErrCode)
{
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugGetPlatformErrorCode(U32_YS *ErrCode_p)
{
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugEnablePrint(DRV_Module Module)
{
    int i;
    
    if(Module > DRV_MODULE_MAX_NB)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }

    if(Module == DRV_MODULE_MAX_NB)
    {
        for(i = 0; i< DRV_MODULE_MAX_NB; i++)
            m_ModuleControl[i].Enable |= ENABLE_PRINT;
    }
    else
    {
        m_ModuleControl[Module].Enable |= ENABLE_PRINT;
    }
    
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugDisablePrint(DRV_Module Module)
{
    int i;
    
    if(Module > DRV_MODULE_MAX_NB)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }
    if(Module == DRV_MODULE_MAX_NB)
    {
        for(i = 0; i< DRV_MODULE_MAX_NB; i++)
            m_ModuleControl[i].Enable &= PRINT_MASK;
    }
    else
    {
        m_ModuleControl[Module].Enable &= PRINT_MASK;
    }
    
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugEnableDebug(DRV_Module Module)
{
    int i;
    
    if(Module > DRV_MODULE_MAX_NB)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }

    if(Module == DRV_MODULE_MAX_NB)
    {
        for(i = 0; i< DRV_MODULE_MAX_NB; i++)
            m_ModuleControl[i].Enable |= ENABLE_DEBUG;
    }
    else
    {
        m_ModuleControl[Module].Enable |= ENABLE_DEBUG;
    }
    
    return DRV_NO_ERROR;
}


DRV_ErrCode DRV_DebugDisableDebug(DRV_Module Module)
{
    int i;
    
    if(Module > DRV_MODULE_MAX_NB)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_BAD_PARAMETER);
    }

    if(Module == DRV_MODULE_MAX_NB)
    {
        for(i = 0; i< DRV_MODULE_MAX_NB; i++)
            m_ModuleControl[i].Enable &= DEBUG_MASK;
    }
    else
    {
        m_ModuleControl[Module].Enable &= DEBUG_MASK;
    }
    
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_DebugPrintRevision(DRV_Module Module)
{
    return DRV_NO_ERROR;
}

U8_YS * DRV_DebugGetModuleName(DRV_Module Module)
{
    if(Module > DRV_MODULE_MAX_NB)
    {
        YSDRIVER_ERRORCODE(DRV_ERROR_BAD_PARAMETER);
        return NULL;
    }
    return (U8*)(m_ModuleControl[Module].Name);
}


/*-------------------------------------------------------------------------
 * Function : GetErrorText
 * Input    : ST_ErrorCode
 * Output   :
 * Return   : Error String
 * ----------------------------------------------------------------------*/
char *ST_GetErrorText(ST_ErrorCode_t ST_ErrorCode)
{

    return("UNKNOWN ERROR");
}


char *YSDRIVER_GetErrorText(DRV_ErrCode ErrorCode)
{
    switch(ErrorCode)
    {
        case DRV_ERROR_BAD_PARAMETER:
            return "DRV_ERROR_BAD_PARAMETER";
        case DRV_ERROR_DEVICEFILE_PATH:
            return "DRV_ERROR_DEVICEFILE_PATH";
        case DRV_ERROR_NO_MEMORY:
            return "DRV_ERROR_NO_MEMORY";
        case DRV_PANEL_ERROR_HANDLE:
            return "DRV_PANEL_ERROR_HANDLE";
        case DRV_PANEL_ERROR_INIT:
            return "DRV_PANEL_ERROR_INIT";
        case DRV_PANEL_ERROR_PIO_OPEN:
            return "DRV_PANEL_ERROR_PIO_OPEN";
        case DRV_PANEL_ERROR_PIO_COMPARE:
            return "DRV_PANEL_ERROR_PIO_COMPARE";
        case DRV_NO_ERROR:
            return "DRV_NO_ERROR";
        default:
            break;
    }
    return("UNKNOWN ERROR");
}



