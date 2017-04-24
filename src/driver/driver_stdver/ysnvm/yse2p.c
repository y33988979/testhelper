/****************************************************************************

File Name   : e2p.c

Description : Testtool EEPROM Commands

Copyright (C) 1999, ST Microelectronics

 ****************************************************************************/

/* Includes --------------------------------------------------------------- */

#include <string.h>
#include <hi_unf_ecs.h>

#include "drv_os.h"
#include "drv_basic.h"
#include "yse2p.h"

/* external data ---------------------------------------------------------- */

/* Definitions ------------------------------------------------------------ */

/* the Offsets into the	following two segments point to	the same memory
   area	in the EEPROM, supporting segment & offset positional testing.	*/


/* Global Variables ------------------------------------------------------- */
/*保护stbinfo分区读写，stbinfo分区只能在一次操作完后在进行下一次操作*/
DRVOS_Sem_t  *m_StbInfoRWMutex = NULL;

/* Local Variables -------------------------------------------------------- */

/* Private types/constants ------------------------------------------------ */

/* Functions -------------------------------------------------------------- */

/* Testtool Functions --------------------------------------------------- */


/* Global Functions ------------------------------------------------------- */

/* 复用寄存器基地址 */
#define BASE_ADDR           0x10203000

/* 复用寄存器实际地址 */
#define TSI0_VI0_GPIO_AO    BASE_ADDR+0x158

/* I2C0复用 */
#define LED_GPIO_SPI_ARMEN  BASE_ADDR+0x084
#define KEY_GPIO            BASE_ADDR+0x088


/*-------------------------------------------------------------------------
 * Function : DRV_E2pSetup
 *	      DRV_E2pSetup	EEPROM
 * Input    : ForceTerminate
 * Output   :
 * Return   : TRUE if error, FALSE if success
 * ----------------------------------------------------------------------*/
DRV_ErrCode DRV_E2pSetup(void)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;
#ifndef FILE_E2Prom
    /* 设置此复用为gpio，默认情况此为I2C0模式 */
    //HI_SYS_WriteRegister(TSI0_VI0_GPIO_AO, 0x0); /*store old pin function define */

    /* 设置I2C0模式，此引脚与E2PROM连接 */
    //HI_SYS_WriteRegister(LED_GPIO_SPI_ARMEN, 0x2);
    //HI_SYS_WriteRegister(KEY_GPIO, 0x2);


    /*open e2prom module*/
	errCode = HI_UNF_E2PROM_Open();
	if(errCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_E2pSetup HI_UNF_E2PROM_Open is error.\n"));
		return errCode;
	}

    /*set e2prom mode 、、*/
	errCode = HI_UNF_E2PROM_SetChipType(HI_UNF_E2PROM_CHIP_AT24C64); 
	if(errCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_E2pSetup HI_UNF_E2PROM_SetChipType is error.\n"));
		return errCode;
	}

    /*set E2PROM i2c device address*/
    errCode = HI_UNF_E2PROM_SetAddr(0xA0);
	if(errCode != DRV_NO_ERROR)
    {
		STTBX_Print(("DRV_E2pSetup HI_UNF_E2PROM_SetAddr is error.\n"));
		return errCode;
    }

    /*set E2PROM use i2c  which channel*/
    errCode = HI_UNF_E2PROM_SetI2cNum(E2PROM_I2C_NUM);
	if(errCode != DRV_NO_ERROR)
    {
		STTBX_Print(("DRV_E2pSetup HI_UNF_E2PROM_SetI2cNum is error.\n"));
		return errCode;
    }

	STTBX_Print(("DRV_NvmInit is OK.\n"));
#endif

	m_StbInfoRWMutex = DRV_OS_SemCreate(NULL,1,DRVOS_INIT_FIFO_WAIT);

	return (DRV_NO_ERROR);
} 
DRV_ErrCode DRV_E2pTerm(void)
{
	DRV_ErrCode errCode = DRV_NO_ERROR;
#ifndef FILE_E2Prom
    /*Close e2prom module*/
	errCode = HI_UNF_E2PROM_Close();
	if(errCode != DRV_NO_ERROR)
	{
		STTBX_Print(("DRV_E2pTerm HI_UNF_E2PROM_Close is error.\n"));
		return errCode;
	}

	STTBX_Print(("DRV_E2pTerm is OK.\n"));
#endif
	return (DRV_NO_ERROR);
} 

/* EOF */

