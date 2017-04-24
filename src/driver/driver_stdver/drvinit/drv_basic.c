/*****************************************************************************
*
* Description: 

*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------

*
*****************************************************************************/
#define CURRENT_MODULE DRV_MODULE_OS
#include <hi_struct.h>
#include <sttbx.h>
#include "drv_basic.h"
#include "drv_os.h"
#define SYSTEM_MEMORY_SIZE (0x03000000) /* 48 Mbytes */

/*-------------------------- BOOT START ------------------------------------*/
//static ST_DeviceName_t BOOT_DeviceName = "BOOT";
/*-------------------------- BOOT END ------------------------------------*/

/*-------------------------- BOOT START ------------------------------------*/
//ST_ClockInfo_t  CLOCK_Info;
//ST_DeviceName_t   CLOCK_DeviceName[CLOCK_MAX_NUMBER]={"CLOCK"};
//STCLOCK_Handle_t  CLOCK_Handle[CLOCK_MAX_NUMBER];
/*-------------------------- BOOT END ------------------------------------*/


/*-------------------------- PIO START ------------------------------------*/
//ST_DeviceName_t PIO_DeviceName[] = {"PIO0","PIO1","PIO2","PIO3","PIO4","PIO5","PIO6","PIO7","PIO8","PIO9","PIO10","PIO11","PIO12","PIO13","PIO14","PIO15","PIO16"};
/*-------------------------- PIO END ------------------------------------*/


/*-------------------------- UART START ------------------------------------*/
/* For linux, there is no UART support in STSDK, this is done inside the kernel */
/*-------------------------- UART END ------------------------------------*/


/*-------------------------- TBX START ------------------------------------*/
/*-------------------------- TBX END ------------------------------------*/


/*-------------------------- PARTITION START ------------------------------------*/
//ST_Partition_t *cache_partition_sdk[CACHE_PARTITION_MAX_NUMBER];
//ST_Partition_t *ncache_partition_sdk[NCACHE_PARTITION_MAX_NUMBER];
yspartition_t *SystemPartition;
void* m_SystemMemory=NULL;
/*-------------------------- AVMEM END ------------------------------------*/


/*-------------------------- EVT START ------------------------------------*/
//STEVT_Handle_t  EVT_Handle[EVT_MAX_NUMBER];
//ST_DeviceName_t EVT_DeviceName[EVT_MAX_NUMBER] = {"EVT"};
/*-------------------------- EVT END ------------------------------------*/


/*-------------------------- FDMA START ------------------------------------*/
//ST_DeviceName_t FDMA_DeviceName[] = {"FDMA0","FDMA1"};
/*-------------------------- FDMA END ------------------------------------*/

/* For linux, stspi is not yet officially supported */

/*-------------------------- I2C START ------------------------------------*/
//STI2C_Handle_t I2C_Handle[I2C_MAX_NUMBER];
//ST_DeviceName_t I2C_DeviceName[I2C_MAX_NUMBER] = {"I2CNIM","I2CHDMI","I2CE2P"};

//I2C_DeviceHandle_t I2CDeviceHandle[I2C_MAXDEVICE] =
//{
//   /* DeviceName    Handles table    I2cType */
//   { "I2CNIM",      I2C_NIM  },
//   { "I2CHDMI",     I2C_HDMI  },
//   { "I2CE2P",      I2C_E2P  }
//};
/*-------------------------- I2C END ------------------------------------*/

/*-------------------------- PWM START ------------------------------------*/
//ST_DeviceName_t PWM_DeviceName[PWM_MAX_NUMBER]={"PWM0"};
/*-------------------------- PWM END ------------------------------------*/

/* ========================================================================
   Name:        DRV_ChipVersion
   Description: Return the chip version
   ======================================================================== */

U32 DRV_ChipVersion(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	HI_SYS_VERSION_S pstVersion;
	ErrCode = HI_SYS_GetVersion(&pstVersion);

	return(pstVersion.enChipTypeHardWare);
}

static DRV_ErrCode InitBootInit(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
    /** initial */
	ErrCode = HI_SYS_Init();
	if(ErrCode != DRV_NO_ERROR)
	{
		printf("InitBootInit HI_SYS_Init ErrCode 0x%08x\n",ErrCode);
	}
    ErrCode = HI_GO_Init();
    if (HI_SUCCESS != ErrCode) 
    {
        printf("InitBootInit HI_GO_Init ErrCode 0x%08x\n",ErrCode); 
        return ErrCode;       
    }
	return ErrCode;
}

static DRV_ErrCode drv_InitEvtSetup(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}
static DRV_ErrCode drv_InitEvtTerm(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}

static DRV_ErrCode drv_InitPioInit(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
} 

static DRV_ErrCode drv_InitPioTerm(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}
DRV_ErrCode DRV_PioBitConfigure(U32 PIO_Index,U32 PIO_Bit,DRV_PIO_Mode_t PIO_Mode)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}

DRV_ErrCode DRV_PioBitSet(U32 PIO_Index,U32 PIO_Bit,BOOL OnOff)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}
DRV_ErrCode DRV_PioBitGet(U32 PIO_Index,U32 PIO_Bit,BOOL *OnOff)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}

static DRV_ErrCode InitPwmInit(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
} 

DRV_ErrCode drv_InitFdmaInit(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}

DRV_ErrCode DRV_ShowDriverVersion(void)
{
#if defined(CHIP_HI3716MV300)
	STTBX_Print(("|---------------TYPE:HI3716MV300---------------|\n"));
#elif defined(CHIP_HI3716MV300TFADV)
	STTBX_Print(("|---------------TYPE:HI3716MV300TFADV---------------|\n"));
#elif defined(CHIP_HI3716MV300SMADV)
	STTBX_Print(("|---------------TYPE:HI3716MV300SMADV---------------|\n"));
#elif defined(CHIP_HI3716M)
	STTBX_Print(("|---------------TYPE:HI3716MV200---------------|\n"));
#elif defined(CHIP_HI3716C)
	STTBX_Print(("|---------------TYPE:HI3716C---------------|\n"));
#else
#error you should select the chip !!!
#endif	
	STTBX_Print(("|------Kernel Version: 0x%x-----------|\n",KERNELVER));
	STTBX_Print(("|------Rootfsware Version: 0x%x-----------|\n",ROOTWVER));
	STTBX_Print(("|------Hardware Version: 0x%x-----------|\n",HWVER));
	STTBX_Print(("|------Software Version: 0x%x-----------|\n",SWVER));
	STTBX_Print(("|------Ca Type: %s-----------|\n",CATPYE));
	STTBX_Print(("|------File Type Type: %s-----------|\n",FILETYPE));
	STTBX_Print(("|---Version Created @%s---|\n",BUILDER));
	STTBX_Print(("|---1:END---|\n"));
	STTBX_Print(("|---1:END---|\n"));
	return DRV_NO_ERROR;
}

static DRV_ErrCode drv_InitClockSetup(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	return ErrCode;
}

static DRV_ErrCode drv_InitPartitionInit(void)
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR; 
	U32 size = 0;
	void *check;
    partition_initialize();

    size = SYSTEM_MEMORY_SIZE;
    printf("SIZE OF SYS MEM IS  %d M;",size /(1024*1024));
    m_SystemMemory = malloc( SYSTEM_MEMORY_SIZE );
#if 1// check memory
    memset(m_SystemMemory+SYSTEM_MEMORY_SIZE-0x10000,0xff,0x10000);//如果这步操作没死机，则说明内存是够的
    printf(" MALLOC OK;");
	check = malloc(0x100000);
    memset(check+0x100000-0x10000,0xff,0x10000);//如果这步操作没死机，则说明内存预留1M是可以的
    free(check);
#endif
    printf(" AND CHECK OK!\n");
    SystemPartition = DRV_OSPartCreate(NULL,m_SystemMemory,SYSTEM_MEMORY_SIZE);
	return ErrCode;
}

DRV_ErrCode DRV_InitBasic(void)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;	
    DRV_CHECK( InitBootInit() );
    DRV_ShowDriverVersion();
    DRV_CHECK( drv_InitPartitionInit() );
   	drv_OsTimeInit();
    YSDRIVER_ERRORCODE_RETURN(errCode);
}

