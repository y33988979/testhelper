/***************************************************************************** 
Copyright 2012, AltoBeam Inc. All rights reserved.
     
File Name: atbm886x.c
Description: Software development source codes for ATBM886x DTMB/DVBC demodulator
Version: 1.0.2
Released 2012-09-17

===============================================================================
Brief introduction for software porting

--------------------------------------------------------------------------------
[Basic functions for porting]

There are four basic functions which are called by other predefined APIs. 
Developer needs to implement them according to certain SoC/OS platform.
All functions for porting are listed at below:

SemodHardwareReset()
	This function is to control the output of SoC GPIO which is connected to the RESETN pin of demodulator.
DelayMS()
	This function is to generate time delay as required.
I2CRead()
	This function is to conduct a single read transaction on I2C bus.
I2CWrite()
	This function is to conduct a single write transaction on I2C bus.
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
[Configuration according to hardware environment]

Some environment varibles need to be specified according to a certain hardware design.
The configuration for these varibles is included in function ATBMPowerOnInit().
All items for customization and the allowed values for each are listed at below:

MPEG_TS_mode_t
	ui8TSTransferType: TS_PARALLEL_MODE / TS_SERIAL_MODE	
	ui8OutputEdge: TS_OUTPUT_FALLING_EDGE / TS_OUTPUT_RISING_EDGE	
	ui8TSSPIMSBSelection: TS_SPI_MSB_ON_DATA_BIT7 / TS_SPI_MSB_ON_DATA_BIT0	
	ui8TSSSIOutputSelection: TS_SSI_OUTPUT_ON_DATA_BIT7 / TS_SSI_OUTPUT_ON_DATA_BIT0	
	ui8SPIClockConstantOutput: TS_CLOCK_CONST_OUTPUT / TS_CLOCK_VALID_OUTPUT	
DVBC_Params_t
	ui8InputMode: DVBC_IF_INPUT / DVBC_IQ_INPUT	
	i32SymbolRate: (1000k ~ 8000k symbol/sec)
tuner_config_t
	ui8IQmode: SWAP_IQ / NO_SWAP_IQ	
	ui8DTMBBandwithMHz: (6MHz, 7MHz and 8MHz bandwidth)
	dbIFFrequency: (4MHz~11MHz, 36MHz~37MHz)
custom_config_t
	ui8ATBMSlaveAddr: 0x80 / 0x88
	ui8CrystalOrOscillator:CRYSTAL / OSCILLATOR	
	ui8DtmbDvbcMode: ATBM_DTMB_MODE / ATBM_DVBC_MODE 

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
[Predefined API functions to be called]
There are a set of predefined functions provided in the SDK source codes.
Simply call some top-level functions to make a basic DTV application, including:

ATBMPowerOnInit()
       This function is to initialize demodulator after power up, this is the first function 
       must be called before any other demodulator functions.    
ATBMSetConfigParas(custom_config_t stCustomConfigp)
	This function config the parameter to the global variable ,it should be called 
	before ATBMSetDTMBMode() or ATBMSetDVBCMode()
ATBMSetDTMBMode()
	This function sets the demodulator working in DTMB mode. 
ATBMSetDVBCMode()
        This function sets the demodulator working in DVB-C mode. 
--------------------------------------------------------------------------------

For more information, please refer to the complete SDK user munal.
===============================================================================
******************************************************************************/


#include <stdio.h>
#include <math.h> 
#include <unistd.h> 
#include "ystuner.h"
#include "atbm886x.h"
#include "drv_os.h"
#include "hi_unf_ecs.h"

//#define JZ_ATBM_TUNER_DEBUG
#ifdef JZ_ATBM_TUNER_DEBUG
#define TUNER_DEBUG	printf
#else
#define TUNER_DEBUG(...)	do{}while(0)
#endif

#define TUNER_I2C_NUM       0
#define TUNER_TDAC_SELECT	1
#define TUNER_UNLOCK_COUNT  2   //宏定义失锁多长时间发送LOST消息 (n+1)*2.5s左右

DRV_TunerCallback mTunerCallback = NULL;
DRV_TunerCallback mTunerCallback_user = NULL;
BOOL g_TunerLock;

void *pvParam;

HI_BOOL g_SelTuner=0;

HI_U8  Modulation ;
HI_U32 Frequence;
HI_U8 LDPCeff;
HI_U8 FrameHead;
HI_U8 CarrierQuan;
HI_U32 NewTSFlag;


unsigned char cQamSize ;   
int 		iSymbolRate;
int 		iTransponderFreq;


//信号量，互斥锁
//static pthread_mutex_t IICMutex;
static DRVOS_Sem_t  semNewTS;
static DRVOS_Sem_t  semSetNewParam;
static DRVOS_Sem_t  mutexI2C;
static DRVOS_Sem_t  mutexI2Ctunersignal;

DRVOS_Task_t *pTask;

HI_U8 ATBM_ChipID;
struct custom_config_t custom_config;
TUNER_Type_t TunerType = Tuner_TDAC7;




HI_U8           ui8ATBMSlaveAddr         = 0x80;   /*either 0x80 or 0x88 */
HI_U8           ui8ATBMSoftwareVersion = 10;     /*SW guide version = ui8ATBMSoftwareVersion/10 */
double          dbDvbcOscFreq;
double          dbDvbcSymbolRate;
custom_config_t stCustomConfig;  /*global config parameter*/


/******************************************************************************
I2CRead:  Read one register by one step I2C format, parameters: 
   I2CSlaveAddr:  I2C slave address;   
   addr_length:   address length, here is 2 bytes; 
   addr_dat:      pointer to 2 bytes register address;  
   data_length:   Data length (in byte) to read, here is 1 byte;
   reg_dat:       pointer to retuned 1 byte register value;
   
   Reading register 0x0004 the I2C sequences is:   
   S_0x80_A_0x00_A_0x04_A_S_0x81_A_0x01_N_P   
   
   S:I2C start;
   A:ACK;
   N:NACK;
   P:stop        
****************************************************************************/
extern ATBM_I2CREADWRITE_STATUS I2CReadOneStep(HI_U8 ui8I2CSlaveAddr, HI_U16 addr_length, HI_U8 *addr_dat,  HI_U16 data_length, HI_U8 *reg_dat)
{
 /*I2C read in one step format, please port this function if needed*/	
    return ATBM_I2CREADWRITE_OK;
}

/****************************************************************************** 
I2CWriteWithRestart: Write one register by one step I2C format, parameters: 
   I2CSlaveAddr:  I2C slave address;   
   addr_length:   address length number, here is 2 bytes; 
   addr_dat:      pointer to 2 bytes register address;  
   data_length:   Data length (in byte) to write, here is 1 byte;
   reg_dat:       pointer to 1 byte register value;
   
   Writing register 0x0004 with 0x01 the I2C sequences is:   
   S_0x80_A_0x00_A_0x04_A_S_0x80_A_0x01_A_P
      
   S:I2C start; A:ACK; N:NACK; P:stop    
   ******Attention a start is needed after sending the register's address.     
****************************************************************************/
extern ATBM_I2CREADWRITE_STATUS I2CWriteWithRestart(HI_U8 ui8I2CSlaveAddr, HI_U8 addr_length, HI_U8 *addr_dat,  HI_U8 data_length, HI_U8 *reg_dat)
{
 /*I2C write in one step format, please port this function if needed*/	
    return ATBM_I2CREADWRITE_OK;
}

/****************************************************************************

   The following are APIs prededined by AltoBeam.
   You should change the values of the variables for configuration in function
   ATBMPowerOnInit() according to hardware design.
                
****************************************************************************/



/****************************************************************************
Function:     ATBMPowerOnInit
Parameters:   none
Return:       power_on_status  
Description:           
          This function close the reference clock and set  I2C gate off from demod to tuner.     
******************************************************************************/
int ATBMPowerOnInit()
{
	int power_on_status = 0;
	HI_U8 ui8pll, reset_status;

	HI_U8 ui8ChipID;

	stCustomConfig.stTunerConfig.ui8IQmode              = NO_SWAP_IQ;        					     
	stCustomConfig.stTunerConfig.ui8DTMBBandwithMHz     = 8; //unit: MHz
	stCustomConfig.stTunerConfig.dbIFFrequency          = 36; //unit: MHz

	//demod config
	stCustomConfig.stTsMode.ui8TSTransferType           = TS_SERIAL_MODE;	  
	stCustomConfig.stTsMode.ui8OutputEdge               = TS_OUTPUT_RISING_EDGE;
	stCustomConfig.stTsMode.ui8SPIClockConstantOutput   = TS_CLOCK_CONST_OUTPUT;
	stCustomConfig.stTsMode.ui8TSSPIMSBSelection        = TS_SPI_MSB_ON_DATA_BIT7;
	stCustomConfig.stTsMode.ui8TSSSIOutputSelection     = TS_SSI_OUTPUT_ON_DATA_BIT0;
	stCustomConfig.ui8CrystalOrOscillator               = CRYSTAL;  // when dbSampleClkFrequency == 30.4 then use CRYSTAL macro   and dbSampleClkFrequency == 24.0 then use OSCILLATOR
	stCustomConfig.dbSampleClkFrequency                 = 30.4; //unit: MHz

	//dvbc config
	stCustomConfig.stDvbcParams.i32SymbolRate           = 6875;//unit: KSymbol/sec, the default value is 6875K for China DVBC broadcasting
	stCustomConfig.stDvbcParams.ui8InputMode            = DVBC_IF_INPUT;

	ui8ChipID = ATBMChipID();	
	if(ui8ChipID == 0x40)
	{
		ATBMWrite(0x01, 0x0c, 0x00);
		ATBMWrite(0x06, 0x06, 0x00);
		ATBMWrite(0x01, 0x03, 0x00);
		ATBMRead(0x06, 0x0e, &ui8pll);
		ATBMWrite(0x06, 0x04, 0x01); 
		ui8pll |= 0x01;
		ATBMWrite(0x06, 0x0e,ui8pll);	
		ui8pll &= 0xfe;
		ATBMWrite(0x06, 0x0e,ui8pll);
		ATBMWrite(0x06, 0x04, 0x00);
	}
	else if(ui8ChipID == 0x30)
	{
		reset_status = ATBMReset(stCustomConfig.ui8CrystalOrOscillator); //Check hardware state
		if(reset_status ==0)
		{
			printf("please check Crystal or oscillator setting\n");			
		}
		ATBMWrite(0x06, 0x06, 0x00); 
		ATBMWrite(0x01, 0x03, 0x00);
	}
	else
	{
		printf("power on init failed, chip not correctly detected\n");
	}
	Delayms(1);/*1 ms*/ 
	return power_on_status;
}


/****************************************************************************
Function:     ATBMChipID
Parameters:   none
Return:       ChipID  
Description: This function checks ATBM886x chip ID,  The chip ID of ATBM886x is 0x40.     
******************************************************************************/
HI_U8 ATBMChipID()
{
	HI_U8 ui8ChipID;
	
	if(ATBM_I2CREADWRITE_OK == ATBMRead(0x00, 0x00,&ui8ChipID))
	{
		return ui8ChipID;
	}
	
	return 0xff;
}

 /****************************************************************************
Function:    ATBMCheckDemodStatus
Parameters:  none
Return:      PLL lock status  

Description:  
     Call this function when crystal is used. if PLL locked, hardware reset is not needed.  
     This function is called before configuring demodulator registers.
	 This function check demodulator status after power on or hardware reset 
     If 1 is returned, demodulator in ready status.
     If 0 is returned, demodulator need hardware reset or power on.
 ******************************************************************************/
HI_U8 ATBMCheckDemodStatus()
{	
	HI_U8 ui8CheckTimes = 0;
	HI_U8 ui8Analog, ui8PLLState, ui8Tmp, ui8HardwareState, ui8ChipID;
	HI_U8 ui8Step = 0x20; 

	ATBMRead(0x06, 0x07, &ui8HardwareState);
	if(ui8HardwareState!=5)
	{
		return 0;
	}
	ATBMRead(0x00, 0x00, &ui8ChipID);
	if(ui8ChipID == 0x20) 
	{
		return 1; 
	}
	ui8PLLState = ATBMCheckPLLStatus();  //check state 0, no register change
	if(ui8PLLState == 1)
	{         
		return 1; 
	}	

	ATBMRead(0x02, 0x14, &ui8Tmp);	
	while(ui8CheckTimes<5)
	{
		if((ui8Tmp&0x02) == 0x02) 
		{		
			ATBMWrite(0x06, 0x06, 0x02); //open		
		}else
		{
			ATBMWrite(0x06, 0x06, 0x00); 
		}
		ui8Analog = ui8CheckTimes*ui8Step;	
		if(ui8CheckTimes == 4)
		{
			ui8Analog = 0x60;	
		}
		ATBMWrite(0x06, 0x0d, ui8Analog);
		if(ui8CheckTimes == 4)
		{
			ATBMWrite(0x06, 0x05, 0x00);
		}
		Delayms(10);			
		ui8PLLState = ATBMCheckPLLStatus();  
		if(ui8PLLState == 1)
		{         
			return 1; 
		}	
		ui8CheckTimes = ui8CheckTimes + 1;
	}	
	return 0;
} 

/****************************************************************************
Function:    PLLLock_Flag
Parameters:  none
Return:      demodulator PLL lock status  

Description: 
	 when internal PLL is locked, the returned value is 1.
******************************************************************************/
 HI_U8 PLLLockFlag()
 {
	 HI_U8 ui8Flag;
	 int i32Iter = 0;
	 while(i32Iter<20)
	 {
		 ATBMRead(0x06, 0x11, &ui8Flag);
		 if((ui8Flag&0x01) != 0x01)
		 {
			 return 0;
		 }
		 i32Iter++;
	 }
	 return 1;
 }

 /****************************************************************************
 Function:    Digtal_PLL_Lock_Flag
 Parameters:  none
 Return:      demodulator PLL lock status  

 Description: 
 when DVB-C register set a value different from the default value,
 if PLL unlocked, the register will be cleared to default value.
 If PLL aways locked, the register will not be changed.
 ******************************************************************************/
 unsigned char Digtal_PLL_Lock_Flag()
 {
	 int check_times = 0;
	 HI_U8 locked_flag = 0;
	 HI_U32 cla_in = 0, cla_out = 0x04;

	 ATBMWrite(0x00, 0x13, 0x01);
	 ATBMDVBCWrite(0x348, cla_out);
	 while(check_times<3)
	 {
		 ATBMDVBCRead(0x348, &cla_in);
		 if(cla_in != cla_out)
		 {		

			 locked_flag = 0;
			 break;
		 }
		 else
		 {
			 locked_flag = 1;
		 }
		 Delayms(10);
		 check_times = check_times + 1;
	 }	
	 ATBMWrite(0x00, 0x13, 0x00);
	 return locked_flag;
 }

/****************************************************************************
Function:    ATBMCheckPLLStatus
Parameters:  none
Return:      PLL status  
Description: 
	 When demodulator PLL locked and stable, the returned value is 1.
******************************************************************************/
 HI_U8 ATBMCheckPLLStatus(void)
 {
	 HI_U8 ui8PllState,pll_digtal_state, ui8Tmp;	

	 ui8PllState = PLLLockFlag(); 
	 if(ui8PllState == 1)
	 {
		 ATBMRead(0x02, 0x14, &ui8Tmp);
		 if((ui8Tmp&0x02) != 0x02)
		 {		
			 ATBMWrite(0x06, 0x06, 0x02); 
		 }
		 else
		 {
			 ATBMWrite(0x06, 0x06, 0x00); 
		 }
	 }
	 else
	 {
		 return 0;
	 }
	 Delayms(10);
	 ui8PllState = PLLLockFlag();
	 if(ui8PllState == 1)
	 {
		 pll_digtal_state = Digtal_PLL_Lock_Flag();
		 if(pll_digtal_state == 1)	 
		 {	 
			 return 1;
		 }

	 }
	 return 0;
 }

 /****************************************************************************
 Function:    ATBMReset
 Parameters:  ui8CryOrOsc
 Return:      1, succed. 0:  failed  
 Description:
 This function resets ATBM8869T demodulator if PLL is not locked. this is a
 compatible function used in ATBMInit or be called after suspend.
 for example, transfer from  suspend mode to standby mode.
 ******************************************************************************/
 int ATBMReset(HI_U8 ui8CryOrOsc)
 {

	 HI_U8 ui8HardwareState, ui8CheckResult = 0;
	 HI_U8 ui8Loop, ui8Tmp;	
	 /* After hardware power on properly or reset correctly,	  
	 * ui8HardwareState value should be 0x05 when using crystal, 3.3V PLL 
	 * ui8HardwareState value should be 0x07 when using oscillator, 3.3V PLL */	

	 if(ui8CryOrOsc==CRYSTAL)
	 {
		 //Software checking, when power on failed, it will return 0 because I2C error or PLL error
		 ui8CheckResult = ATBMCheckDemodStatus();
		 if(ui8CheckResult == 1)
		 {
			 return 1;
		 }	
		 //Hardware checking, usually used after demodulator resume from suspend mode
		 for(ui8Loop=0; ui8Loop<2; ui8Loop++)
		 {
			 if(ui8CheckResult != 1)
			 {
				 // printf("I2C Error or it is not properly power up, need hardware reset\n");
				 DemodHardwareReset();    //hardware check
				 Delayms(50);	
				 ui8CheckResult = ATBMCheckDemodStatus(); 		
			 }
			 else
			 {
				 return 1;
			 }	 
		 }
		 return 0;
	 }	

	 if(ui8CryOrOsc==OSCILLATOR)
	 {
		 ATBMRead(0x06, 0x07, &ui8HardwareState);
		 for(ui8Loop=0; ui8Loop<10; ui8Loop++)
		 {
			 if(ui8HardwareState!=0x07)
			 {		            
				 // printf("I2C Error or it is not properly power up, need hardware reset\n");
				 DemodHardwareReset();
				 Delayms(100);					
				 ATBMRead(0x06, 0x07, &ui8HardwareState);
			 }
			 else
			 {
				 ATBMRead(0x02, 0x14, &ui8Tmp);
				 if((ui8Tmp&0x02) != 0x02)
				 {		
					 ATBMWrite(0x06, 0x06, 0x02); 
				 }
				 else
				 {
					 ATBMWrite(0x06, 0x06, 0x00); 
				 }
				 break;
			 }	 
		 }

		 if( ui8Loop == 10)
		 {
			 return 0;
		 }else
		 {
			 return 1;
		 }
	 }
	 return 0;
 }

/****************************************************************************
Function:    ATBM_I2CByPassOn
Parameters:  none
Return:      none 
Description: Enable demod to pass I2C commands between tuner and host.  
 ****************************************************************************/
void ATBMI2CByPassOn()
{
	ATBMWrite(0x01,0x03,1); /* the 3st parameter:1-> Enable demod to pass I2C commands between tuner and host. */
}

/****************************************************************************
Function:    ATBM_I2CByPassOff
Parameters:  none
Return:      none 
Description: Disable the I2C pass-through. Tuner is disconnected from the I2C BUS after this function is called. 
****************************************************************************/
void ATBMI2CByPassOff()
{
	ATBMWrite(0x01,0x03,0); /*the 3st parameter:0-> Disable demod to pass I2C commands between tuner and host*/
}

/****************************************************************************
Function:    ATBMLockedFlag
Parameters:  none
Return:      1: locked, 0:unlocked  
Description: This function checks ATBM886x Demod locking status   		 
****************************************************************************/
int ATBMLockedFlag()
{
	 HI_U8 ui8DTMBFEC           = 0;
	 HI_U8 ui8CurReceivingMode;
	 HI_U32   ui32DvbcStatus         = 0; 	 

	 ATBMRead(0x00, 0x13, &ui8CurReceivingMode); /*0:DTMB, 1:DVB-C*/
	 if(ui8CurReceivingMode == 0x00) /*DTMB*/
	 {
	     ATBMRead(0x0b, 0x08, &ui8DTMBFEC);/*read DTMB internal register*/
		 if((ui8DTMBFEC&0x01) == 0x01) /*FER locked:bit0*/
		 {
			 return 1;
		 }
	 }
	 if(ui8CurReceivingMode == 0x01)
	 {
		 ATBMDVBCRead(0x08, &ui32DvbcStatus);/*read DVB-C internal register*/        
		 if(ui32DvbcStatus&0x20) /*bit 5 is locked flag*/
		 {                           
			 return 1;   /*DVB-C FEC locked*/
		 } 	 
	}
	return 0;
}


/****************************************************************************
Function:    ATBM_sync_state
Parameters:  none
Return:      2:lock, 1: sync, 0:unlocked 
Description: This function checks ATBM886x signal state, this is an internal 
             function used by ATBMChannelLockCheck.
****************************************************************************/
int ATBM_sync_state()
{
	HI_U8 i, dtmb_fec, signal_sync, sync_times = 0;
	for(i=0; i<10; i++)  
	{
		ATBMRead(0x0b, 0x08, &dtmb_fec);
		if(dtmb_fec&0x01) //bit 0
		{                           
			return 2;           //DTMB FEC locked
		}      	
		ATBMRead(0x08, 0x2B, &signal_sync);
		if(signal_sync == 6)
		{
			sync_times++;
		}
		Delayms(30);
	}   
	if(sync_times >= 3)
	{
		return 1;
	}else
	{
		return 0;
	}
}

/****************************************************************************
Function:    ATBMChannelLockCheck
Parameters:  none
Return:      1: locked, 0:unlocked 
Description: This function keeps checking ATBM886x signal locking status until it's Locked or time out.
             This function can be used for Auto channel scanning.	 
****************************************************************************/
int ATBMChannelLockCheck()
{
	HI_U8 dtmb_fec = 0;
	int check_result;
	HI_U32 dvbc_status = 0; 	
	HI_U32 i32Iter;
	HI_U8 cur_receiving_mode;
	HI_U32 check_times = 6; //default DTMB: 100ms + 6*300ms timeout 
	HI_U32 dvbc_time_out = 600; //milliseconds
	ATBMRead(0x00, 0x13, &cur_receiving_mode); //0:DTMB, 1:DVB-C
	if(cur_receiving_mode == 0) //DTMB
	{		
		for(i32Iter =0; i32Iter<2; i32Iter++)
		{
			ATBMRead(0x0b, 0x08, &dtmb_fec);
			if(dtmb_fec&0x01) //bit 0
			{                           
				return 1;           //DTMB FEC locked
			}     
			Delayms(50);
		}		
		for(i32Iter=0; i32Iter<check_times; i32Iter++)  
		{
			check_result = ATBM_sync_state();
			if(check_result == 2)
			{
				return 1; //FEC locked
			}
			if(check_result ==0)
			{
				return 0; //signal unsync
			}
		}   
	}
	if(cur_receiving_mode == 1) //DVB-C
	{
		check_times = dvbc_time_out/20;
		for(i32Iter=0; i32Iter<check_times; i32Iter++)
		{
			ATBMDVBCRead(0x08, &dvbc_status);       //read DVB-C internal register        
			if(dvbc_status&0x20) //bit 5
			{                           
				return 1;           //DVB-C FEC locked
			}                 
			Delayms(20);
		}
	}
	return 0; //FEC not locked
}


/****************************************************************************
Function:    ATBMChannelLockCheckForManual
Parameters:  none
Return:      1: locked, 0:unlocked 
Description: This function keeps checking ATBM886x signal locking status until it's Locked or time out.
             This function can be used for Manual channel scanning.	 
****************************************************************************/
int ATBMChannelLockCheckForManual()
{   
	HI_U8  ui8DTMBFEC          = 0;
	HI_U32  ui32DVBCStatus       = 0; 	
	HI_U8  ui8CurReceivingMode;	 
	HI_U32 ui32DTMBTimeOut     = 5000;  /*5000 millisecond*/
	HI_U32 ui32DVBCTimeOut     = 600;  //milliseconds
	int    i32CheckTimes;
	int    i32Iter;

    ATBMRead(0x00, 0x13, &ui8CurReceivingMode); /*0:DTMB, 1:DVB-C*/
	if(ui8CurReceivingMode == 0) /*DTMB*/
	{
		i32CheckTimes = ui32DTMBTimeOut/20;
		for(i32Iter=0; i32Iter<i32CheckTimes; i32Iter++)
		{
			ATBMRead(0x0b, 0x08, &ui8DTMBFEC);			
			if(ui8DTMBFEC&0x01) /*bit 0*/
			{                           
				return 1;           /*DTMB FEC locked*/
			}                 
			Delayms(20);
		}
	}
	if(ui8CurReceivingMode == 1) /*DVB-C*/
	{
		i32CheckTimes = ui32DVBCTimeOut/20;
		for(i32Iter=0; i32Iter<i32CheckTimes; i32Iter++)
		{
			ATBMDVBCRead(0x08, &ui32DVBCStatus);       /*read DVB-C internal register*/        
			if(ui32DVBCStatus&0x20) /*bit 5*/
			{                           
				return 1;           /*DVB-C FEC locked*/
			}                 
			Delayms(20);
		}
	}
	return 0;
}

/****************************************************************************
Function:    ATBMHoldDSP
Parameters:  none
Return:      none  
Description: This function should be called before set tuner frequency.
 ****************************************************************************/
 void ATBMHoldDSP()
 {	 
	 ATBMWrite(0x00, 0x05, 0x01); 
 }

 /****************************************************************************
Function:    ATBMStartDSP
Parameters:  none
Return:      none 
Description: This function should be called after set tuner frequency.Attention: ATBMHoldDSP and ATBMStartDSP should be used together.
 ****************************************************************************/
 void ATBMStartDSP()
 {
	 ATBMWrite(0x00, 0x05, 0x00); 
 }
/****************************************************************************
Function:    ATBM_Standby
Parameters:  none
Return:      none
Description:
      This function put demodulator in standby mode, it can cut off part power,
	  the demodulator can wake up at once by calling ATBM_Standby_WakeUp.
	  two goals:
	  1, cut off part power when demodulator put in standby mode
	  2, put TS output pin in high-z state.
 ****************************************************************************/
void ATBMStandby()
{
	HI_U8 ui8ADCPD,ui8TSEdge,ui8RefStandby, ui8ADCRfv,ui8GPORge;

	ATBMRead(0x06, 0x00,&ui8RefStandby); 
	ATBMRead(0x06, 0x02,&ui8ADCPD);
	ATBMRead(0x03, 0x01,&ui8TSEdge);
	ATBMRead(0x06, 0x0d,&ui8ADCRfv);
	ATBMRead(0x10, 0xf7,&ui8GPORge);

	ui8RefStandby |= 0x02;     
	ui8ADCPD      |= 0x01;     
	ui8TSEdge     |= 0x02;     
	ui8ADCRfv     |=0x08;    
	ui8GPORge     |=0x01;      

	ATBMWrite(0x00, 0x05, 0x01); 
	ATBMWrite(0x06, 0x0d, ui8ADCRfv);
	ATBMWrite(0x06, 0x00, ui8RefStandby);
	ATBMWrite(0x06, 0x02, ui8ADCPD); 
	ATBMWrite(0x15, 0x00, 0x00); 
	ATBMWrite(0x03, 0x01, ui8TSEdge); /*all TS output PINs will be high-z*/
	ATBMWrite(0x10, 0xf7, ui8GPORge); 
	ATBMWrite(0x00, 0x19, 0x00); 
}

/****************************************************************************
Function:    ATBM_Standby_WakeUp
Parameters:  none
Return:      none
Description: Put demodulator in previous normal working mode from standby mode 		 
 ****************************************************************************/
void ATBMStandbyWakeUp()
{
	HI_U8 ui8ADCPD,ui8TSEdge,ui8RefStandby,ui8ADCRfv,ui8GPORge;

	ATBMRead(0x06, 0x00,&ui8RefStandby);
	ATBMRead(0x06, 0x02,&ui8ADCPD);
	ATBMRead(0x03, 0x01,&ui8TSEdge);
	ATBMRead(0x06, 0x0d,&ui8ADCRfv);
	ATBMRead(0x10, 0xf7,&ui8GPORge);
	ui8RefStandby  &= 0xfd;  //bit1 set 0
	ui8ADCPD       &= 0xfe;     //bit0 set 0
	ui8TSEdge      &= 0xfd;    //bit1 set 0     
	ui8ADCRfv      &= 0xf7;
	ui8GPORge      &= 0xfe; //bit 0 set 0

	ATBMWrite(0x06, 0x00, ui8RefStandby);
	ATBMWrite(0x06, 0x02, ui8ADCPD); 
	ATBMWrite(0x15, 0x00, 0x01); 
	ATBMWrite(0x03, 0x01, ui8TSEdge); //all TS output PINs will be in normal mode     
	ATBMWrite(0x10, 0xf7, ui8GPORge); 
	ATBMWrite(0x06, 0x0d, ui8ADCRfv);
	ATBMWrite(0x00, 0x19, 0x01); 
	ATBMWrite(0x00, 0x05, 0x00); 

}

/****************************************************************************
Function:    ATBM_Suspend
Parameters:  none
Return:      none
Description:
    Put ATBM886x demodulator in suspend mode, Hardware reset is needed to wake up
    the demodulator. if external oscillator is used Hardware pull down is used 
	to enter suspend mode.
****************************************************************************/
void ATBMSuspend()
{	
	HI_U8 ui8ChipID;
	ui8ChipID = ATBMChipID();	
	if(ui8ChipID == 0x40)
	{
       	ATBMWrite(0x06, 0x06, 0x2); 
	}
	ATBMStandby();	
	ATBMWrite(0x01, 0x0c, 0x00); //I2C clock switch 
	ATBMWrite(0x06, 0x04, 0x01); //shutdown PLL     
	ATBMWrite(0x06, 0x0e, 0x02); //shut down crystal clock
}

/****************************************************************************
Function:       ATBMSignalStrength
Parameters:     none
Return:         unsigned short PWM value, in the range of 0--1023
Description:
     This function checks ATBM886x signal strength: when signal is weak the number 
     will be big; The smaller the PWM is, the stronger the signal strength detected.	 
 ****************************************************************************/
int ATBMSignalStrength()	
{
    HI_U8 ui8PWM0, ui8PWM1;
	HI_U16 agc_value,signal_percent;

    ATBMWrite(0x08,0x4d, 1); /*latch read only registers*/	
    ATBMRead(0x10,0x28,&ui8PWM0);  
    ATBMRead(0x10,0x29,&ui8PWM1);	
    ATBMWrite(0x08,0x4d, 0);

	/*ALPS TDAC2-C02A tuner Reference evaluation*/
	agc_value=((ui8PWM1&0x03)<<8)|ui8PWM0;
	if(agc_value <=380)
	{
		signal_percent = 90;	
	}                    
	else if(agc_value <=870)
	{   	
		signal_percent = (HI_U16)((870 - agc_value)/7 + 20);	
	}
	else
	{
		signal_percent = 20;	
	}
	
	return signal_percent;
}

 /****************************************************************************
 Function:    ATBMSetDTMBMode
 Parameters:  none
 Return:      none   
 Description:
	This function initializes ATBM886x in DTMB mode, selects DTMB receiving mode,
	then calls ATBMInit function.	  
	ATBMSetConfigParas function should be called before this function.
 ******************************************************************************/
 void ATBMSetDTMBMode(void)
 {		 
	 stCustomConfig.ui8DtmbDvbcMode = ATBM_DTMB_MODE;

	 ATBMInit();
 }

/****************************************************************************
Function:        ATBMSignalNoiseRatio
Parameters:      none
Return:          double (dB) 
Description:
     This function checks ATBM886x signal-to-noise ratio.
     Can be used for signal quality evaluation.   		 
 ****************************************************************************/
double ATBMSignalNoiseRatio()
{
	 HI_U8  ui8PNValue,ui8PN, ui8TmpNoise,ui8IsSc,ui8SignalH, ui8SignalM, ui8SignalL,ui8NoiseH,ui8NoiseM, ui8NoiseL;   
	 int    i32SignalPower,i32NoisePower;
	 double dbSNR = 0;   
	 
	 ATBMWrite(0x08, 0x4d, 1);/*lock register value*/	

	 ATBMRead(0x08, 0x2d,&ui8PNValue);   
	 ui8PN       = (ui8PNValue&0x03);	 
	 ATBMRead(0x14, 0xAF,&ui8TmpNoise); 	
	 ATBMRead(0x0d, 0x0f,&ui8IsSc); 

	 if(ui8IsSc !=1 )
	 {
		 ATBMRead(0x0f, 0x1d,&ui8SignalL);
		 ATBMRead(0x0f, 0x1e,&ui8SignalM);
		 ATBMRead(0x0f, 0x1f,&ui8SignalH); 
		 ATBMRead(0x0f, 0x1a,&ui8NoiseL);
		 ATBMRead(0x0f, 0x1b,&ui8NoiseM);   
		 ATBMRead(0x0f, 0x1c,&ui8NoiseH);
		 i32SignalPower = ((ui8SignalH&0x0f)<<16)+(ui8SignalM<<8)+ui8SignalL;
		 i32NoisePower  = ((ui8NoiseH&0x07)<<16)+(ui8NoiseM<<8)+ui8NoiseL; 				 
		 if(i32SignalPower == 0)
		 {
			 i32SignalPower = 1; //signal_power should >0 in log calculation;
		 }		
		 dbSNR = (double) (10*(log10(i32SignalPower*1.0/(i32NoisePower+1))));	    
	 }	 
	 if(ui8IsSc ==1 )
	 {
		 if(ui8PN == 2) //PN595
		 {   	 
			 ATBMRead(0x14, 0xdc,&ui8SignalL);
			 ATBMRead(0x14, 0xdd,&ui8SignalM);
			 ATBMRead(0x14, 0xde,&ui8SignalH);  
			 i32SignalPower = ((ui8SignalH&0x0f)<<16)+(ui8SignalM<<8)+ui8SignalL;
			 i32SignalPower = i32SignalPower/16;
			 if(ui8TmpNoise != 3)
			 {
				 ATBMRead(0x14, 0xf8,&ui8NoiseL);		
				 ATBMRead(0x14, 0xf9,&ui8NoiseH);
				 i32NoisePower = ((ui8NoiseH&0x3f)<<8) + ui8NoiseL;
			 }
			 else
			 {                    
				 ATBMRead(0x13, 0x40,&ui8NoiseL);		
				 ATBMRead(0x13, 0x41,&ui8NoiseH);                       
				 i32NoisePower = ((ui8NoiseH&0x3f)<<8) + ui8NoiseL;
			 }	
			 if(i32SignalPower == 0)
			 {
				 i32SignalPower = 1; //signal_power should >0 in log calculation;
			 }		
			 dbSNR = (double) (10*(log10(i32SignalPower*1.0/(i32NoisePower+1))));	    
		 }
		 
		 if((ui8PN == 1)||(ui8PN == 3)) //PN420 & PN945
		 {   	   	
			 ATBMRead(0x09, 0xcc,&ui8SignalL);	
			 ATBMRead(0x09, 0xcd,&ui8SignalH);  
			 i32SignalPower = (ui8SignalH<<8) + ui8SignalL;
			 ATBMRead(0x09, 0xed,&ui8NoiseL);
			 ATBMRead(0x09, 0xee,&ui8NoiseH);
			 i32NoisePower  = ((ui8NoiseH&0x3f)<<8) + ui8NoiseL;						
			 
			 if(i32SignalPower == 0)
			 {
				 i32SignalPower = 1; //signal_power >0;
			 }			 
			 if(i32NoisePower == 0)
			 {
				 dbSNR = (double)(10*(log10(i32SignalPower*1.0/(i32NoisePower + 1))));
			 }
			 else
			 {	
				 dbSNR = (double)(10*(log10(i32SignalPower*1.0/i32NoisePower)));	    
			 }    
		 }
	 }

	 ATBMWrite(0x08,0x4d, 0);

	return dbSNR;
}

/****************************************************************************
Function:    ATBMSignalQuality
Parameters:  none
Return:      i32SNR100per, the signal quality evaluated value.
Description:
     This function checks ATBM886x signal-to-noise ratio.
     Can be used for signal quality evaluation:
	 max value:99, min:20
 ****************************************************************************/
int ATBMSignalQuality()
{   
	HI_U8  ui8TPSCode, ui8TSSync;  
	int    i32SNR = 0;	
	int    i32SNR100per;   
	double dbSNRdB = 0;    
	double dbFrameError;

	dbSNRdB    = ATBMSignalNoiseRatio(); //ATBM886x
	i32SNR     = (int)(pow(10, dbSNRdB/10) + 0.5);	

	ui8TPSCode = ATBMGetTPS();
	ATBMRead(0x03, 0x0d,&ui8TSSync);  
	if(ui8TSSync != 1)
	{
		ATBMRead(0x03, 0x0d,&ui8TSSync);  
	}
	dbFrameError = ATBMFrameErrorRatio();

	i32SNR100per = 20;  
	if((ui8TPSCode <11)&&(ui8TPSCode>2))/*4QAM FEC:0.6 0.8*/
	{
		if(i32SNR>100)
		{
			i32SNR100per = 90+ i32SNR/100;
		}
		else if(i32SNR>60)
		{
			i32SNR100per = 80 + (i32SNR-60)/4 ;
		}   	 
		else if(i32SNR>20)
		{
			i32SNR100per = 60 + (i32SNR-20)/2;
		}
		else if(i32SNR>5)
		{	  	
			i32SNR100per = 40 + i32SNR;
		}
		else
		{
			i32SNR100per = 40;
		}
	}

	if((ui8TPSCode <17)&&(ui8TPSCode>10)) /*For 16QAM FEC:0.6 0.8*/
	{
		if(i32SNR>100)
		{
			i32SNR100per = 90+ i32SNR/100;
		}
		else if(i32SNR>70)
		{
			i32SNR100per = 80 + (i32SNR-70)/3 ;
		}   	 
		else if(i32SNR>30)
		{
			i32SNR100per = 60 + (i32SNR-30)/2;
		}
		else if(i32SNR>10)
		{	  	
			i32SNR100per = 50 + (i32SNR-10)/2;
		}
		else
		{
			i32SNR100per = 40;
		}
	}   
	if((ui8TPSCode <25)&&(ui8TPSCode>16))/*64QAM FEC:0.6; 32QAM 0.8*/
	{

		if(i32SNR>100)
		{
			i32SNR100per = 90+ i32SNR/100;
		}
		else if(i32SNR>70)
		{
			i32SNR100per = 80 + (i32SNR-70)/3 ;
		}   	 
		else if(i32SNR>30)
		{
			i32SNR100per = 60 + (i32SNR-30)/2;
		}
		else if(i32SNR>20)
		{	  	
			i32SNR100per = 30+ i32SNR;
		}
		else
		{
			i32SNR100per = 40;
		}
	}

	//re-evaluation   
	if((ui8TSSync ==1)&&(dbFrameError < 0.0001)) //no frame error
	{
		//default:5, you can change this value form 0--10, for evaluation	
		i32SNR100per +=5; 	
	}
	else if((ui8TSSync ==1)&&(dbFrameError > 0.0001))
	{
		i32SNR100per = i32SNR100per - 10; 
	}
	else
	{
		i32SNR100per = 30;
	}
	if(i32SNR100per>99)
	{
		i32SNR100per = 99;
	}  

	return i32SNR100per;
}

/****************************************************************************
Function:    ATBMSignalStrength_reference
Parameters:  none
Return:      signal_percent 
Description:    
	This function is a sample of signal strength evaluation. 
	the value AGC_A, AGC_B, AGC_C should be calibrated according to can tuner used.	
****************************************************************************/
HI_U16 ATBMSignalStrength_reference()	
{
	HI_U8 agc0, agc1,LockValue;
	HI_U16 agc_value;
	HI_U16 signal_percent;

	//agc_value can be read from register 0x1028, 0x1029,  the AGC Value is related with AGC voltage: about (1024/3.3)*V_agc,
	//Refer to SDK document. the following value should be changed according to Tuner used.

	HI_U16 AGC_A = 186; //about AGC 0.6V, Signal strength:80
	HI_U16 AGC_B = 496; //about AGC 1.6V, Signal strength:60
	HI_U16 AGC_C = 930; //about AGC 3.0V, Signal strength:20

	LockValue =1; /*lock register value*/
	ATBMWrite(0x08,0x4d, LockValue);

	ATBMRead(0x10, 0x28, &agc0);  
	ATBMRead(0x10, 0x29, &agc1);

	LockValue = 0;
	ATBMWrite(0x08,0x4d, LockValue);
	agc_value = (HI_U16)(((agc1&0x03)<<8)|agc0);

	/*Tuner Reference evaluation*/
	if(agc_value <=AGC_A)
	{
		signal_percent = 80;	
	}                    
	else if(agc_value <=AGC_B)
	{   	
		signal_percent = (HI_U16)((AGC_B - agc_value)*20/(AGC_B - AGC_A) + 60);	
	}
	else if(agc_value <=AGC_C)
	{   	
		signal_percent = (HI_U16)((AGC_C - agc_value)*40/(AGC_C - AGC_B) + 20);	
	}
	else
	{
		signal_percent = 20;	
	}

	return signal_percent;
}

/****************************************************************************
Function:    ATBMFrameErrorRatio
Parameters:  none
Return:      dbFrameErrorRatio 
Description:    
	This function counts pre-BCH error frames in specified frame window size
	(Default 2048 frames), then print and return pre-BCH frames error ratio.   
****************************************************************************/
double ATBMFrameErrorRatio()
{

	HI_U8  ui8FrameError0, ui8FrameError1;
	HI_U8  ui8ArrFrameCountMax[2];
	double dbFrameErrorRatio;

	ATBMRead(0x0B, 0x09,&ui8ArrFrameCountMax[0]); 
	ATBMRead(0x0B, 0x0A,&ui8ArrFrameCountMax[1]);	
	ATBMWrite(0x08, 0x4d, 1);  /*latch read only registers*/
	ATBMRead(0x0B, 0x14,&ui8FrameError0);  
	ATBMRead(0x0b, 0x15,&ui8FrameError1);
	ATBMWrite(0x08, 0x4d, 0);
	if((ui8ArrFrameCountMax[1] == 0)&&(ui8ArrFrameCountMax[0] == 0))
	{
		//printf("I2C error\n");
		dbFrameErrorRatio = 0;
		return dbFrameErrorRatio;
	}
	dbFrameErrorRatio = ((ui8FrameError1&0x7f)*256 + ui8FrameError0)*1.0/((ui8ArrFrameCountMax[1]&0x7f)*256 + ui8ArrFrameCountMax[0]);	

	return  dbFrameErrorRatio; 
}

/****************************************************************************
Function:    ATBMPreBCHBlockErrorRatio
Parameters:  none
Return:      block_error_ratio

Description:
    This function counts blocks with error in specified block window size,
    Then calculate and return pre-BCH block error ratio.    		 
****************************************************************************/
double ATBMPreBCHBlockErrorRatio()
{	

	HI_U8 ui8BlockCountMax[2];
	HI_U8 ui8PreBCHBerCount[2];            
	HI_U8 ui8LockValue; 	
	double  dbBlockErrorRatio;

	ATBMRead(0x0B, 0x10,&ui8BlockCountMax[0]);	
	ATBMRead(0x0B, 0x11,&ui8BlockCountMax[1]);		

	ui8LockValue =1; 
	ATBMWrite(0x08, 0x4d, ui8LockValue);  /*latch read only registers*/	
	ATBMRead(0x0B, 0x12,&ui8PreBCHBerCount[0]); 
	ATBMRead(0x0B, 0x13,&ui8PreBCHBerCount[1]);  	
	ui8LockValue = 0;
	ATBMWrite(0x08, 0x4d, ui8LockValue);
	if((ui8BlockCountMax[0] == 0)&&(ui8BlockCountMax[1]==0))
	{
		//printf("I2C error\n");
		dbBlockErrorRatio = 0;
		return dbBlockErrorRatio;
	}
	dbBlockErrorRatio  = (ui8PreBCHBerCount[1]*256 + ui8PreBCHBerCount[0])*1.0/(ui8BlockCountMax[1]*256 + ui8BlockCountMax[0]);
	//printf(" DTMB Pre-BCH Block error ratio = %f\n", dbBlockErrorRatio);
	return dbBlockErrorRatio;
}

/****************************************************************************
Function:    ATBMBER_Calc
Parameters:  none
Return:      BER

Description:
    This function returns calculated BER.    		 
****************************************************************************/
double ATBMBER_Calc()
{	
	HI_U8 ber_count[3];            
	double ber_calc = 0;
	HI_U8 LockValue; 	

	LockValue =1; 
	ATBMWrite(0x08, 0x4d, LockValue);  /*latch read only registers*/	
	ATBMRead(0x0B, 0x21, &ber_count[0]);	
	ATBMRead(0x0B, 0x22, &ber_count[1]);
	ATBMRead(0x0B, 0x23, &ber_count[2]); 
	LockValue = 0;
	ATBMWrite(0x08, 0x4d, LockValue);	

	ber_calc  = ((ber_count[2]&0x07)*256*256 + ber_count[1]*256 + ber_count[0])*1.0/pow(2.0,27) ; //134217728
	return ber_calc;
}

/****************************DVBC Parameters************************************************/
HI_U8  ui8DVBCI2CBurstMode = 0;
double dbCMS0022MaxSymbolRateRatio         = (1.0/4);         
double dbCMS0022CIC2DELAY                  = (3.0);
double dbCMS0022CIC3DELAY                  = (0.0);
HI_U8  ui8ADvbcCciAci[]=
{		
	0x0a, 0x3b, 0x28,
	0x0a, 0x42, 0x10,
	0x0a, 0x43, 0x05,
	0x0a, 0x46, 0x00,
	0x0a, 0x47, 0x01,
	0x0a, 0x48, 0x14,
	0x0a, 0x49, 0x00,
	0x0a, 0x4a, 0x76,
	0x0a, 0x52, 0x10,
	0x0a, 0x53, 0x52,
	0x0a, 0x54, 0xb1,
	0x0a, 0x58, 0x00,
	0x0a, 0x59, 0x40,
	0x0a, 0x5b, 0x00,
	0x0a, 0x5c, 0x1f,
	0x0a, 0x5d, 0x05,
	0x0a, 0x5e, 0x20,
	0x0a, 0x5f, 0x00,
	0x0a, 0x60, 0x04,
	0x0a, 0x62, 0x1e,	
	0x0a, 0x91, 0x0f,
	0x0a, 0x95, 0x38,
	0x0a, 0xa0, 0xff,
	0x0a, 0xa1, 0xfa,
	0x0a, 0xa2, 0x00,
	0x0a, 0xac, 0xff,
	0x0a, 0xb2, 0x00,
	0x0a, 0xb3, 0xff,
	0x0a, 0xb4, 0x0f,
	0x0a, 0xb5, 0x68,
	0x0a, 0xb7, 0x5c,
	0x0a, 0xb9, 0xff,
	0x0a, 0xba, 0x0f,
	0x0a, 0xbe, 0x9d,
	0x0a, 0xc0, 0x14,
	0x0a, 0xd2, 0x30,
	0x0a, 0xd3, 0x00,
	0x0a, 0xd4, 0x05,
	0x0a, 0xd5, 0x40,
	0x0a, 0xd9, 0x0a,
	0x10, 0x81, 0xfa,
	0x10, 0x82, 0x00,
	0x10, 0x9d, 0x00,
	0x10, 0x9f, 0x00,
	0x10, 0xa1, 0x00,	
	0x10, 0xa2, 0x00,
	0x10, 0xa7, 0x0b,
	0x10, 0xac, 0x4d	
};

static HI_U8 ui8ADtmbInternal[]=
{	
	0x00, 0x16, 0x1d, //DTMB mode   
	0x09, 0x88, 0x08,
	0x09, 0x89, 0x0c,    
	0x09, 0x9a, 0x40,	
	0x09, 0x35,0x14,
	0x0a, 0xf9, 0x00,
	0x0e, 0x01, 0x08,
	0x08, 0x52, 0x28,
	0x0c, 0x24, 0x0a,
	0x0c, 0x26, 0x0a
};
static HI_U8 uiA8DvbcInternal[]=
{		
	0x00, 0x0a, 0x00,
	0x00, 0x09, 0x00,
	0x00, 0x13, 0x01,
	0x15, 0x18, 0x01,
	0x15, 0x15, 0x01,
	0x15, 0x20, 0x00,
	0x15, 0x21, 0x80,
	0x15, 0x2e, 0x01
};

HI_U8 ui8AATBM886xCommonReg[]=
{
	0x02, 0x45, 0x33,
	0x02, 0x4a, 0x96,
	0x02, 0xc6, 0x00,
	0x02, 0xc7, 0x01 
};
HI_U8 ui8AATBM886xDtmbSet[]=
{
	0x02, 0x28, 0x01, 
	0x02, 0xa6, 0x10, 
	0x02, 0xa9, 0x60, 
	0x02, 0xcb, 0x68, 
	0x02, 0xcc, 0x70, 
	0x02, 0x49, 0x30  
};
HI_U8 ui8AATBM886xDvbcSet[]=
{
	0x02, 0x28, 0x00, 
	0x02, 0x3c, 0x0c, 
	0x02, 0x49, 0x29, 
	0x02, 0xa6, 0x14, 
	0x02, 0xa9, 0x4a,
	0x02, 0xcb, 0x40, 
	0x02, 0xcc, 0x43, 
	0x15, 0x12, 0x00
};

HI_U8 ui8AATBM886xDtmb30Dot4MSet[]=
{	
	0x02, 0xC5, 0x05,
	0x02, 0x4E, 0x08,
	0x02, 0x3E, 0x1C,
	0x02, 0x3B, 0x1C,
	0x0A, 0xFB, 0x00,
	0x02, 0x3C, 0x16
};
HI_U8 ui8AATBM886xDtmb24MSet[]=
{
	0x02, 0xC5, 0x15,            
	0x02, 0x4E, 0x10,            
	0x02, 0x3E, 0x28,            
	0x02, 0x3B, 0x28,            
	0x0A, 0xFB, 0x01,            
	0x02, 0x3C, 0x1C
};
HI_U8 ui8ATBM886xDvbc30Dot4MSet[]=
{
	0x02, 0xC5, 0x05,
	0x02, 0x4E, 0x08,
	0x02, 0x3E, 0x1C,
	0x02, 0x3B, 0x1C
};
HI_U8 ui8AATBM886xDvbc24MSet[]=
{
	0x02, 0xC5, 0x15,            
	0x02, 0x4E, 0x10,            
	0x02, 0x3E, 0x28,            
	0x02, 0x3B, 0x28 
};

 /****************************************************************************
 Function:    ATBMSetDVBCMode
 Parameters:  none
 Return:      none   
 Description:
	  This function initializes ATBM886x in DVB-C mode, selects DVB-C receiving mode,
	  then calls ATBMInit function	
	  ATBMSetConfigParas function should be called before this function.
 ******************************************************************************/
void ATBMSetDVBCMode(void)
 {	  
	 stCustomConfig.ui8DtmbDvbcMode = ATBM_DVBC_MODE;
	
	 ATBMInit();  
 }

/****************************************************************************
Function:   ATBMDVBCSNR
Parameters:   None                    
Return:    DVBC signal-to-noise ratio in dB format

Description:
	Calculate  DVB-C SNR:			 
******************************************************************************/
double ATBMDVBCSNR()
{
	double dbDVBCSNR;
	HI_U32 i32Tmp,i32Tmp1; 				

	dbDVBCSNR = 0;
	ATBMDVBCRead(0x3C4,&i32Tmp);
	ATBMDVBCRead(0x3d0,&i32Tmp1);
	i32Tmp    = i32Tmp&0xffff;
	i32Tmp1   = (i32Tmp1&0x7)+1; //pow(2,i32Tmp1) means QAM		
    
	if(i32Tmp ==0)
	{    	
		i32Tmp = 1;
	}

	if((i32Tmp1==4) ||(i32Tmp1==6) ||(i32Tmp1==8))
	{
		if(i32Tmp!=0)
		{
			dbDVBCSNR = -20*log10(i32Tmp/65536.0)-0.8;//QAM=16,64,256
		}
	}
	else if( (i32Tmp1==5) ||(i32Tmp1==7))
	{
		if(i32Tmp !=0)
		{
			dbDVBCSNR = -20*log10(i32Tmp/65536.0)-4.0;//QAM=32,128
		}
	}
	else
	{
		dbDVBCSNR=0;

	}	
	//  printf("DVBC SNR = %f dB\n",dbDVBCSNR);	
	return dbDVBCSNR;

}

/****************************************************************************
Function:   ATBMDVBCBER
Parameters: BerExponent: scientific notation exponent                    
Return:     dbBerCoeffient: RS corrected Bit error rate real number.
Description:
	Calculate DVB-C RS bit error rate. 
******************************************************************************/
double   ATBMDVBCBER(int *pi32BerExponent)
{
	HI_U8  ui8Adata[2];
	HI_U8  ui8Iter;	
	int    i32Tmp;
	int    i32TotalBits = 13369344; //SampleSize: 8192*204*8
	int    i32EPara = 0;
	int    i32CorrectedBerBits = 0;	
	double dbBerCoeffient =0;

	ATBMRead(0x15, 0x23,&ui8Adata[1]);
	ATBMRead(0x15, 0x22,&ui8Adata[0]);
	i32CorrectedBerBits = ui8Adata[1]*256 + ui8Adata[0];
	ATBMRead(0x15, 0x21,&ui8Adata[1]);
	ATBMRead(0x15, 0x20,&ui8Adata[0]);
	i32TotalBits        = (ui8Adata[1]*256 + ui8Adata[0])*204*8;		
	if((i32CorrectedBerBits ==0)||(i32TotalBits == 0))
	{
		dbBerCoeffient = 0;
		*pi32BerExponent = 0;			
		//printf("PreRsBer = %f E- %d\n",dbBerCoeffient, *BerExponent);			
		return dbBerCoeffient;
	}	
	for (ui8Iter=0;ui8Iter<12;ui8Iter++)
	{
		i32Tmp = i32CorrectedBerBits/i32TotalBits;
		if (i32Tmp)
		{
			dbBerCoeffient = (i32CorrectedBerBits*1.0)/i32TotalBits;
			break;
		}
		else
		{
			i32EPara +=1;
			i32CorrectedBerBits *=10;
		}
	}	
	*pi32BerExponent  = i32EPara;
	//printf("PreRsBer = %f E- %d\n",dbBerCoeffient, *BerExponent);		
	return dbBerCoeffient;
}

/****************************************************************************
Function:   ATBMDVBCUncorrectablePER
Parameters: i32pPktsExponent: scientific notation exponent                   
Return:   dbPktsCoeffient: error packets rate real number.
Description:
	This function counts DVB-C ui8AData packet which contains uncorrected error and calculate packet error rates.
******************************************************************************/
double  ATBMDVBCUncorrectablePER(int *i32pPktsExponent )
{
	HI_U8  ui8AData[2],ui8Iter;
	int    i32Tmp;
	int    i32TotalPkts = 8192; //SampleSize: 8192 packets
	int    i32EPara = 0;
	int    i32UncorrectedPackets = 0;	
	double dbPktsCoeffient = 0;

	ATBMRead(0x15, 0x27,&ui8AData[1]);
	ATBMRead(0x15, 0x26,&ui8AData[0]);
	i32UncorrectedPackets = ui8AData[1]*256 + ui8AData[0];
	ATBMRead(0x15, 0x21,&ui8AData[1]);
	ATBMRead(0x15, 0x20,&ui8AData[0]);
	i32TotalPkts          = ui8AData[1]*256 + ui8AData[0];		

	if((i32UncorrectedPackets ==0)||(i32TotalPkts == 0))
	{
		dbPktsCoeffient   = 0;
		*i32pPktsExponent = 0;			
		//printf("Pks Error = %f E- %d\n",dbPktsCoeffient, *i32pPktsExponent);			
		return dbPktsCoeffient;
	}	
	for (ui8Iter=0;ui8Iter<12;ui8Iter++)
	{
		i32Tmp = i32UncorrectedPackets/i32TotalPkts;
		if (i32Tmp)
		{
			dbPktsCoeffient = (i32UncorrectedPackets*1.0)/i32TotalPkts;
			break;
		}
		else
		{
			i32EPara += 1;
			i32UncorrectedPackets *=10;
		}
	}	
	*i32pPktsExponent  = i32EPara;
	//printf("Pks Error = %f E- %d\n",dbPktsCoeffient, *i32pPktsExponent);	
	return dbPktsCoeffient;

}

/****************************************************************************
Function:     ATBMDVBCGetQAM
Parameters:   None                    
Return:       QAM_index
Description:
	Get DVB-C QAM index			
	4: 16QAM
	5: 32QAM
	6: 64QAM
	7: 128QAM
	8: 256QAM
******************************************************************************/
HI_U8   ATBMDVBCGetQAM()
{
	HI_U8 ui8QAMIndex; 				
	HI_U32 i32Tmp;

	ATBMDVBCRead(0x3d0,&i32Tmp);
	ui8QAMIndex = (HI_U8)((i32Tmp&0x7)+1); //pow(2,tmp) means QAM		
	//printf("DVB-C signal QAM index = %d \n", QAM_index);       
	return ui8QAMIndex;
}

/*****************************************************************************
Function:    ATBMDVBCSignalStrength
Parameters:  none
Return:      XgcGain
Description:
    This function checks DVB-C external IF gain value used by the XGC. The function 
    can be used for DVB-C signal strength evaluation.
    Different Tuner have different XgcGain Range. It is needed to test XgcGain value
    on different signal strength level to make a Tuner's private signal strength evaluation function.    
*****************************************************************************/
int ATBMDVBCSignalStrength()
{
	HI_U32 i32XgcGain;
	HI_U32 i32XgcGain1; 

	ATBMDVBCRead(0x114,&i32XgcGain1);	
	i32XgcGain = (int)(i32XgcGain1&0xfff);
//	printf(" XgcGain = 0x%x\n", XgcGain);
	return i32XgcGain;
}

/****************************************************************************
Function:		ATBMDVBCInit
Parameters:   stCustomConfig              
Return: 		None
Description:
	Call this function to configure all DVBC related registers.		 
******************************************************************************/
void ATBMDVBCInit( custom_config_t stCustomConfig)
{

	HI_U8  ui8RegH, ui8RegL , ui8Rst, ui8TmpData, ui8pll;
	HI_U8 ui8ATBMChipID;
	HI_U16 ui16ParamsDvbcRatio;
	//int    i32Length = 0;
	int    i32RifControl;
	//int    i32DebugData = 0;
	int	   i32ItbCoarseFrequencyOffset;
	double dbOSCFreq = 0;
	double dbIFFreq = 0;
	double dbSymbolRateM, dbSysClk;
	ui8ATBMChipID = ATBMChipID();
	ATBMDVBCWrite(0x04, 0x1);  //DVBC Module Reset
	if(stCustomConfig.stDvbcParams.ui8InputMode == DVBC_IF_INPUT)
	{
		i32RifControl = 0x01; //IF mode
		ATBMDVBCWrite(0x100, i32RifControl); 

		//886x special register
		if(ui8ATBMChipID == 0x40)
		{
			ATBMDVBCWrite(0x104,0x01);
			ATBMDVBCWrite(0x304,0x05);
		}	
		
	}else
	{
		i32RifControl = 0x00; //IQmode
		ATBMDVBCWrite(0x100, i32RifControl); 
		ATBMDVBCWrite(0x104,0x01);  //gain control   
		ATBMDVBCWrite(0x118, 0x00);
		ATBMWriteRegArray(ui8ADvbcCciAci,sizeof(ui8ADvbcCciAci)/sizeof(HI_U8)); 
	}

	dbSymbolRateM = (double)(stCustomConfig.stDvbcParams.i32SymbolRate*1.0)/1000; 

	//886x special register setting 
	if(ui8ATBMChipID == 0x40)
	{
		ui16ParamsDvbcRatio = (HI_U16)floor(dbSymbolRateM/2.0*1.008*pow(2.0, 11) +0.5);
		ui8RegH = ui16ParamsDvbcRatio/256;
		ui8RegL = ui16ParamsDvbcRatio&0xff;
		ATBMWrite(0x02, 0x53, ui8RegL); 
		ATBMWrite(0x02, 0x54, ui8RegH); 
		if(((stCustomConfig.dbSampleClkFrequency-30.4)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-30.4)>-0.00001)) //30.4MHz crystal
		{
			ATBMRead(0x06, 0x0e,&ui8TmpData);
			if(ui8TmpData&0x08)
			{
				dbSysClk = 69.5;
			}else
			{
				dbSysClk = 60.8;
			}
			ui8Rst =(HI_U8)floor(3.4375/dbSymbolRateM*dbSysClk/2 + 0.5); 
			ATBMWrite(0x15, 0x37, ui8Rst); 
		}
		if(((stCustomConfig.dbSampleClkFrequency-24)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-24)>-0.00001)) //24MHz crystal
		{
			ATBMRead(0x06, 0x0e,&ui8TmpData);
			if(ui8TmpData&0x08)
			{
				dbSysClk = 64;
			}else
			{
				dbSysClk = 72;
			}
			ui8Rst =(HI_U8)floor(3.4375/dbSymbolRateM*dbSysClk/2 + 0.5); 
			ATBMWrite(0x15, 0x37, ui8Rst); 
		}
	}
	//end for 886x

	dbDvbcSymbolRate = dbSymbolRateM;
	if(((stCustomConfig.dbSampleClkFrequency-24)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-24)>-0.00001)) //24MHz crystal
	{
		dbOSCFreq = stCustomConfig.dbSampleClkFrequency*2;	
		if(ui8ATBMChipID == 0x30)
		{
			ATBMDVBCWrite(0x104,0x14);
		}
	}
	else
	{
		dbOSCFreq = stCustomConfig.dbSampleClkFrequency;	
		if(ui8ATBMChipID == 0x30)
		{
			ATBMDVBCWrite(0x104,0x34);
		}
	}
	dbDvbcOscFreq= dbOSCFreq;	
	if(stCustomConfig.stDvbcParams.ui8InputMode == DVBC_IF_INPUT)
	{
		dbIFFreq = stCustomConfig.stTunerConfig.dbIFFrequency;	
		i32ItbCoarseFrequencyOffset = (int)(134217728*(dbOSCFreq - dbIFFreq)/dbOSCFreq);
		ATBMDVBCWrite(DVBC_COARSE_FREQUENCY_OFFSET_ADDR,i32ItbCoarseFrequencyOffset);
	}
	ATBMDVBCSetCarrier(dbOSCFreq,	dbSymbolRateM);	 	
	ATBMDVBCSetSymbolRate(dbOSCFreq, dbSymbolRateM);		 
	ATBMDVBCSetQAM();	 

	ATBMDVBCWrite(0x20c,0x10); 
	ATBMDVBCWrite(0x204,0x400); //DVB-C symbol rate offset related	
	ATBMDVBCWrite(0x04, 0x00);  //DVBC Module start processing
	if(ui8ATBMChipID == 0x40)
	{
		if(((stCustomConfig.dbSampleClkFrequency-24)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-24)>-0.00001)) //24MHz crystal
		{
			ATBMWrite(0x01, 0x0c, 0x00); 
			Delayms(1);/*1 ms*/ 
			ATBMRead(0x06, 0x0e, &ui8pll);
			ATBMWrite(0x06, 0x04, 0x01); 
			ui8pll |= 0x01;
			ATBMWrite(0x06, 0x0e,ui8pll);	
			ui8pll &= 0xfe;
			ATBMWrite(0x06, 0x0e,ui8pll);
			ATBMWrite(0x06, 0x04, 0x00);
			Delayms(1);/*1 ms*/ 
			ATBMWrite(0x01, 0x0c, 0x01); 
		}
	}
}

/****************************************************************************
Function:		ATBMDVBCSetSymbolRate
Parameters:     OSC_Freq: OSC sampling frequency, in MHz   
                dbSymbolRateM, symbol rate, in Msbps
Return: 		None
Description:
	Call this function to configure all DVB-C  symbol rate related registers.			 
******************************************************************************/
void ATBMDVBCSetSymbolRate(double dbOscFreq, double dbSymbolRateM)
{
	HI_U32  ui32Log2DecFactor;		
	HI_U32	ui32DecCicOutputShift; 
	HI_U32	ui32AfcSlowControlTC;	
	HI_U32	ui32TrlSampleRate;
	HI_U32	ui32TrlSampleRateRecip;
	double  dbSmpsPerSymb;		
	double  dbCicDcGain;			

	// Calculate factor by which decimation filter must reduce rate
	ui32Log2DecFactor = (HI_U32)(floor(log10(((dbCMS0022MaxSymbolRateRatio*dbOscFreq)/dbSymbolRateM))/log10(2.0)));
	// Samples per symbol
	if(ui32Log2DecFactor > 3)
	{
		ui32Log2DecFactor=3;
	}
	dbSmpsPerSymb = dbOscFreq/dbSymbolRateM;
	ui32TrlSampleRate = (HI_U32)((1<<16)*2/(dbSmpsPerSymb/(1<<ui32Log2DecFactor))+0.5);
	ATBMDVBCWrite(DVBC_SAMPLE_RATE_ADDR, ui32TrlSampleRate);
	ui32TrlSampleRateRecip = (int)(((1<<11)*(dbSmpsPerSymb/(1<<ui32Log2DecFactor))/2)+0.5);
	if(ui32TrlSampleRateRecip > 0x3fff)
	{
		ui32TrlSampleRateRecip = 0x3fff;
	}
	ATBMDVBCWrite(DVBC_SAMPLE_RATE_RECIP_ADDR, ui32TrlSampleRateRecip);
	// Calculate CIC gain
	dbCicDcGain  = pow(2.0*(1<<(ui32Log2DecFactor-1)),(int)dbCMS0022CIC2DELAY);
	//Calculate CIC gain *=
	dbCicDcGain *= pow(3.0*(1<<(ui32Log2DecFactor-1)),(int)dbCMS0022CIC3DELAY);
	ui32DecCicOutputShift = (ui32Log2DecFactor > 0) ? ((HI_U32)((log10(dbCicDcGain)/log10(2.0))+0.5)+1) : 1;
	ATBMDVBCWrite(DVBC_OUTPUT_SHIFT_ADDR, ui32DecCicOutputShift);
	ATBMDVBCWrite(DVBC_DECIMATION_FACTOR_ADDR,ui32Log2DecFactor);
	// Program AFC tracking time constant as function of symbol rate
	ui32AfcSlowControlTC = (1<<(ui32Log2DecFactor+2));
	ATBMDVBCWrite(DVBC_SLOW_CONTROL_TC_ADDR,ui32AfcSlowControlTC);
}

/****************************************************************************
Function:	ATBMDVBCSetCarrier
Parameters: OSC_Freq: OSC sampling frequency, 30.4, in MHz   
            dbSymbolRateM, symbol rate. 
Return: 	None
Description:
	Call this function to configure DVBC registers according to OSC frequency  
	and symbol rate. 
******************************************************************************/
void   ATBMDVBCSetCarrier(double dbOscFreq,double dbSymbolRateM)
{
	int	    i32IfSrchStartFreq;
	int	    i32IfSrchStepFreq;
	HI_U32	ui32TimSearchLockTimeout;
	HI_U32	ui32PlCarrierFrequencyRange;
	HI_U32	ui32CarrierLockAcquireTimeout;
	double  dbIfSearchStartFoOvrFsamp; 
	double  dbIfSearchStepFstpOvrFsamp;  
	double  dbWinEndFreqMHz = 0.15*dbSymbolRateM;

	dbIfSearchStartFoOvrFsamp = dbWinEndFreqMHz/dbOscFreq;
	if(dbWinEndFreqMHz != 0.0)
	{
		dbIfSearchStepFstpOvrFsamp = CMS0022_COARSE_CARRIER_ACQ_SWEEP_STEP*dbSymbolRateM/dbOscFreq;
	}
	else
	{
		dbIfSearchStepFstpOvrFsamp = 0;
	}
	i32IfSrchStartFreq = (int)(((dbIfSearchStartFoOvrFsamp)*(1<<27))+0.5); 
	ATBMDVBCWrite(DVBC_SEARCH_RANGE_ADDR,i32IfSrchStartFreq);
	i32IfSrchStepFreq = (int)(((dbIfSearchStepFstpOvrFsamp)*(1<<27))+0.5);
	ATBMDVBCWrite(DVBC_SEARCH_STEP_ADDR,i32IfSrchStepFreq);	
	// Program parameters for fine stepped carrier search
	// Step size
	ui32TimSearchLockTimeout = 20;	// 20 for 1% step;
	ATBMDVBCWrite(DVBC_PL_CARRIER_STEP_FREQUENCY_ADDR,ui32TimSearchLockTimeout);
	// Search range
	ui32PlCarrierFrequencyRange = 39;// 39 for +/-15% limits
	ATBMDVBCWrite(DVBC_PL_CARRIER_FREQUENCY_RANGE_ADDR,ui32PlCarrierFrequencyRange);
	// Standard times and thresholds for all other QAM levels
	ui32CarrierLockAcquireTimeout = 4;   	
	ATBMDVBCWrite(DVBC_CARRIER_LOCK_ACQUIRE_TIMEOUT_ADDR, ui32CarrierLockAcquireTimeout);

}

/****************************************************************************
Function:	ATBMDVBCSetQAM
Parameters: none                
Return: 	None
Description:Call this function to configure DVBC QAM related registers:			 
******************************************************************************/
void ATBMDVBCSetQAM()
{
	int	i32BitSyncDetectTimeout;   
	ATBMDVBCWrite(DVBC_AUTO_EQU_SEARCH_ADDR,0x3f8); //Auto QAM detect
	i32BitSyncDetectTimeout = 60000/(256*16); 
	ATBMDVBCWrite(DVBC_BITSYNC_DETECT_TIMEOUT_ADDR,i32BitSyncDetectTimeout);
}

void   ATBMSetConfigParas(custom_config_t stCustomConfigp)
{
	stCustomConfig = stCustomConfigp;
}

/****************************************************************************
Function:     ATBMInit
Parameters:   none
Return:       none  
Description:      
    Initializes ATBM886x demodulator and configures all necessary registers. 
    The demodulator will start processing incoming data after this function is called.                       
******************************************************************************/
void ATBMInit()
{    
	HI_U8 ui8ATBMChipID;
	Delayms(1);/*1 ms*/ 
	ATBMWrite(0x01, 0x0c, 0x01);//i2c clock using PLL, 1:PLL, 0:Crystal.
	//Delayms(10); /*10 ms*/   
	ATBMWrite(0x00, 0x04, 0x00);	
	ATBMWrite(0x10, 0xf7, 0xe8);	
	ATBMWrite(0x10, 0xfb, 0x07);	
	ui8ATBMChipID = ATBMChipID();
	if(ui8ATBMChipID == 0x40)
	{
		ATBMWriteRegArray(ui8AATBM886xCommonReg,sizeof(ui8AATBM886xCommonReg)/sizeof(unsigned char)); 	
	}
	ATBMConfig( stCustomConfig);
	if(stCustomConfig.ui8DtmbDvbcMode == ATBM_DVBC_MODE) 
	{
		ATBMWriteRegArray(uiA8DvbcInternal,sizeof(uiA8DvbcInternal)/sizeof(unsigned char)); 

		if(stCustomConfig.stDvbcParams.ui8InputMode == DVBC_IF_INPUT)
		{
			ATBMWrite(0x15, 0x11, 0x01); 
			if(ui8ATBMChipID == 0x40)
			{
				ATBMWrite(0x15, 0x12, 0x00); 
			}
			if(ui8ATBMChipID == 0x30)
			{
				ATBMWrite(0x15, 0x12, 0x01); 
			}
			ATBMWrite(0x00, 0x16, 0x00); 
		}
		if(stCustomConfig.stDvbcParams.ui8InputMode == DVBC_IQ_INPUT)
		{
			ATBMWrite(0x00, 0x16, 0x03); 
		}
		if(ui8ATBMChipID == 0x40)
		{
			ATBMWrite(0x02, 0x52, 0x01);
			ATBMWriteRegArray(ui8AATBM886xDvbcSet,sizeof(ui8AATBM886xDvbcSet)/sizeof(HI_U8)); 
			if(((stCustomConfig.dbSampleClkFrequency-24)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-24)>-0.00001)) 
			{
				ATBMWriteRegArray(ui8AATBM886xDvbc24MSet,sizeof(ui8AATBM886xDvbc24MSet)/sizeof(HI_U8));
			}
			if(((stCustomConfig.dbSampleClkFrequency-30.4)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-30.4)>-0.00001))
			{
				ATBMWriteRegArray(ui8ATBM886xDvbc30Dot4MSet,sizeof(ui8ATBM886xDvbc30Dot4MSet)/sizeof(HI_U8));
			}
		}
		ATBMDVBCInit(stCustomConfig);                     
	}
	else //DTMB mode
	{  
		ATBMWrite(0x00, 0x0a, 0x01); 
		ATBMWrite(0x00, 0x09, 0x01);  	     
		ATBMWrite(0x00, 0x13, 0x00); //DTMB mode
		ATBMWrite(0x15, 0x18, 0x00);			
		ATBMWrite(0x15, 0x15, 0x00); 	
		ATBMWrite(0x15, 0x11, 0x00); 
		ATBMWrite(0x15, 0x12, 0x00);
		if(ui8ATBMChipID == 0x40)
		{
			ATBMWrite(0x02, 0x52, 0x00);
			ATBMWriteRegArray(ui8AATBM886xDtmbSet,sizeof(ui8AATBM886xDtmbSet)/sizeof(unsigned char)); 	
			if(((stCustomConfig.dbSampleClkFrequency-24)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-24)>-0.00001)) 
			{
				ATBMWriteRegArray(ui8AATBM886xDtmb24MSet,sizeof(ui8AATBM886xDtmb24MSet)/sizeof(unsigned char));
			}
			if(((stCustomConfig.dbSampleClkFrequency-30.4)<0.00001)&&((stCustomConfig.dbSampleClkFrequency-30.4)>-0.00001))
			{
				ATBMWriteRegArray(ui8AATBM886xDtmb30Dot4MSet,sizeof(ui8AATBM886xDtmb30Dot4MSet)/sizeof(unsigned char));
			}
		}	 
		ATBMWriteRegArray(ui8ADtmbInternal,sizeof(ui8ADtmbInternal)/sizeof(unsigned char)); 			
	}

	ATBMWrite(0x08,0x00,0xa0);
	ATBMWrite(0x08,0x01,0x16);
	ATBMWrite(0x00, 0x04, 1);	// write 1 config done

#if ATMB_DBG_OUTPUT
	ATBMDebugRegister();
#endif  
}

/****************************************************************************
Function:    ATBMConfig
Parameters:  custom_config
Return:      none  

Description:  
     Set registers according to customer's configuration, 
     This function configures ATBM886x. 
 ******************************************************************************/
 void ATBMConfig( custom_config_t stCustomConfigp)
 {
	 /*Demodulator default clock 30.4MHz; Tuner IF 36.166Mhz*/
	 ATBMSetOSC(stCustomConfigp.stTunerConfig,stCustomConfigp.dbSampleClkFrequency);	
	 /*Configures  MPEG TS output  mode*/		 	
	 ATBMSetTSMode(stCustomConfigp.stTsMode);   	 
	 ATBMWrite(0x01, 0x02, 1);	 //I2C related
	 ATBMWrite(0x02, 0x0c, ui8ATBMSoftwareVersion);
 }

 /****************************************************************************
Function:    ATBMSetTSMode
Parameters:  stTSMode
Return:      none  
Description:
      This function configures Transport stream interface mode.   		 
******************************************************************************/
void ATBMSetTSMode( MPEG_TS_mode_t stTSMode)
{
	HI_U8 ui8ChipID;
	HI_U8 ui8SerialEnabled =0x0;/*parallel clock out */
	HI_U8 ui8TSOutputEdegeMode = stTSMode.ui8OutputEdge;  
	HI_U8 ui8TsClockMode = stTSMode.ui8SPIClockConstantOutput;  

	/*0x00: SPI data align with rising edge of TSBCLK; 0x01: SPI data align with falling edge of TSBCLK*/
	ATBMWrite(0x03,0x01, ui8TSOutputEdegeMode); 	
	ATBMWrite(0x03,0x06, ui8TsClockMode);  /*either free running or only active when data are valid*/
	ATBMWrite(0x03,0x11, 0);  
	ATBMRead(0x00,0x00, &ui8ChipID);
	if(ui8ChipID == 0x40)
	{
		ATBMWrite(0x03,0x12, 1);//886x special
		ATBMWrite(0x0b,0x24, 0x01);
	}
	if(stTSMode.ui8TSTransferType==TS_SERIAL_MODE)
	{    
		ui8SerialEnabled = 0x01;    /*enable serial TS mode */   

		//SSI_sync_hold_cycle = 0x00; //0x00:1 bit sync;0x01: 8 bit sync
		/*0: serial sync  holds 1 cycle; 1: serial sync holds 8 cycles*/
		ATBMWrite(0x03,0x0f, 0);  

		/*0:disable serial mode; 1: enable serial mode */  
		ATBMWrite(0x03,0x05, ui8SerialEnabled);      
		if(stTSMode.ui8TSSSIOutputSelection == TS_SSI_OUTPUT_ON_DATA_BIT0)
		{
			ATBMWrite(0x03, 0x1f, 0x00); 
		}else
		{
			ATBMWrite(0x03, 0x1f, 0x01); 
		}
	}
	else	
	{	  
		ui8SerialEnabled =0x0; /*parallel clock output */
		/*0: disable serial mode; 1:enable serial mode */	
		ATBMWrite(0x03,0x05, ui8SerialEnabled);  		
		if(stTSMode.ui8TSSPIMSBSelection == TS_SPI_MSB_ON_DATA_BIT7)
		{
			ATBMWrite(0x03, 0x1f, 0x00); 
		}else
		{
			ATBMWrite(0x03, 0x1f, 0x01); 
		}
	}
}

/****************************************************************************
Function:    ATBMSetOSC
Parameters:  stTunerConfig, dbSampleClkFrequency
Return:      1 for success,0 for error  
Description:
     This function configures ATBM886x according to external Clock source 
     and  Tuner IF frequency.
 ******************************************************************************/
int ATBMSetOSC( tuner_config_t stTunerConfig, double dbSampleClkFrequency) 
{
	/*Demodulator default registers are for 30.4 MHz crystal, no need to list here.*/	
	HI_U8  ui8ARXFLT[6]; 
	HI_U8  ui8BandWidth;
	HI_U8  ui8SwapIQ;	
	HI_U8  ui8ARge[3], ui8ChipID;	
	HI_U8 ui8Tmp;
	HI_U16 ui16ParamsFSADC, ui16ParamsIFFreq;
	HI_U32 ui32ParamsFSADCInverse;
	int    i32FreqLifTmp;
	int    i32RateRatioTmp;
	double dbTunerIFFrequency = stTunerConfig.dbIFFrequency;	   
	double dbCLKFreq = dbSampleClkFrequency;
	ATBMRead(0x00,0x00, &ui8ChipID);
	if(stTunerConfig.ui8IQmode == SWAP_IQ)
	{	 	
		ui8SwapIQ = 0x03;   
		ATBMWrite(0x0c,0x01, ui8SwapIQ); 
	}
	else
	{
		ATBMWrite(0x0c,0x01, 0x01); 
	}
	/* 0x0A03~0x0A05 are only related to sampling frequency; 
	* 0x0A00~0x0A02 are related to sampling frequency and tuner IF frequency.*/			
	if(dbTunerIFFrequency>dbCLKFreq) 
	{
		i32FreqLifTmp=(int)floor(2*PI*(dbTunerIFFrequency-dbCLKFreq)*(1<<22)/dbCLKFreq);
	}
	else //low IF
	{
		i32FreqLifTmp=(int)floor(2*PI*(dbTunerIFFrequency)*(1<<22)/dbCLKFreq);
	}
	ui8ARXFLT[0] = (HI_U8)(i32FreqLifTmp&0xff);
	ui8ARXFLT[1] = (HI_U8)((i32FreqLifTmp>>8)&0xff);
	ui8ARXFLT[2] = (HI_U8)((i32FreqLifTmp>>16)&0xff);

	ATBMWrite(0x0A,0x00, ui8ARXFLT[0]);
	ATBMWrite(0x0A,0x01, ui8ARXFLT[1]);
	ATBMWrite(0x0A,0x02, ui8ARXFLT[2]);   
	//printf("IF freq_lif: 0x%x 0x%x  0x%x\n", ui8ARXFLT[0], ui8ARXFLT[1], ui8ARXFLT[2]);

	ui8BandWidth = stTunerConfig.ui8DTMBBandwithMHz;
	i32RateRatioTmp = (int)floor(8.0/ui8BandWidth*2*dbCLKFreq*(1<<19)/30.4 + 0.5);

	//i32RateRatioTmp=(int)floor(2*dbCLKFreq*(1<<19)/30.4 + 0.5);
	ui8ARXFLT[3] = (HI_U8)(i32RateRatioTmp&0xff);
	ui8ARXFLT[4] = (HI_U8)((i32RateRatioTmp>>8)&0xff);
	ui8ARXFLT[5] = (HI_U8)((i32RateRatioTmp>>16)&0x1f);
	ATBMWrite(0x0A,0x03, ui8ARXFLT[3]);
	ATBMWrite(0x0A,0x04, ui8ARXFLT[4]);
	ATBMWrite(0x0A,0x05, ui8ARXFLT[5]);   	
	//printf("ADC rate ratio: 0x%x 0x%x  0x%x\n", ui8ARXFLT[3], ui8ARXFLT[4], ui8ARXFLT[5]);	
	if(((dbCLKFreq-24)<0.00001)&&((dbCLKFreq-24)>-0.00001)) //24MHz crystal, register should set here.
	{
		ATBMRead(0x06,0x0e,&ui8Tmp);
		ATBMWrite(0x06,0x0e, ui8Tmp|0x4);
		if(ui8ChipID == 0x30)
		{
			ATBMWrite(0x0a,0xfb,0x02);
		}
		if(ui8ChipID == 0x40)
		{
			ATBMWrite(0x0a,0xfb,0x01);
		}
	}
	ATBMRead(0x00,0x00, &ui8ChipID);
	if(ui8ChipID == 0x40)
	{
		ui16ParamsFSADC = (HI_U16)floor(dbCLKFreq * pow(2.0, 11) + 0.5);
		ui8ARge[1] = ui16ParamsFSADC/256;
		ui8ARge[0] = ui16ParamsFSADC&0xff;
		ATBMWrite(0x02, 0x33, ui8ARge[0]);
		ATBMWrite(0x02, 0x34, ui8ARge[1]);
		ui32ParamsFSADCInverse = (HI_U32)floor(1.0/dbCLKFreq*pow(2.0, 21) + 0.5);
		ui8ARge[0] = (HI_U8)(ui32ParamsFSADCInverse&0xff);
		ui8ARge[1] = (HI_U8)((ui32ParamsFSADCInverse>>8)&0xff);
		ui8ARge[2] = (HI_U8)((ui32ParamsFSADCInverse>>16)&0x01);
		ATBMWrite(0x02, 0x9d, ui8ARge[0]);
		ATBMWrite(0x02, 0x9e, ui8ARge[1]);
		//ATBMWrite(0x02, 0x9f, ui8ARge[2]);
		if(((dbCLKFreq-24)<0.00001)&&((dbCLKFreq-24)>-0.00001)) //24MHz crystal, register should set here.
		{
			ui8ARge[2] = ui8ARge[2]|0x02;
			ATBMWrite(0x02, 0x9f, ui8ARge[2]);
		}
		if(((dbCLKFreq-30.4)<0.00001)&&((dbCLKFreq-30.4)>-0.00001)) //30.4MHz crystal, register should set here.
		{					
			ATBMWrite(0x02, 0x9f, ui8ARge[2]);
		}
		if(dbTunerIFFrequency > dbCLKFreq) 
		{
			ui16ParamsIFFreq = (HI_U16)floor((dbTunerIFFrequency-dbCLKFreq)*pow(2.0, 11) + 0.5);
		}
		else //low IF
		{
			ui16ParamsIFFreq = (HI_U16)floor((dbTunerIFFrequency*pow(2.0, 11)) + 0.5);
		}
		ui8ARge[1] = ui16ParamsIFFreq/256; 
		ui8ARge[0] = ui16ParamsIFFreq&0xff;
		ATBMWrite(0x02, 0x31,  ui8ARge[0]);
		ATBMWrite(0x02, 0x32,  ui8ARge[1]);
		if(ui8BandWidth == 8)
		{	
			ATBMWrite(0x02, 0x51, 0x00);
		}
		else if(ui8BandWidth == 7)
		{
			ATBMWrite(0x02, 0x51, 0x01);
		}
		else if(ui8BandWidth == 6)
		{
			ATBMWrite(0x08, 0x44, 0x00); 
			ATBMWrite(0x14, 0x8a, 0x0f);
			ATBMWrite(0x02, 0x51, 0x02);
		}else
		{
			ATBMWrite(0x02, 0x51, 0x00);
		}
		if(((dbCLKFreq - 24)<0.00001)&&((dbCLKFreq - 24)>-0.00001)) 
		{
			ATBMWrite(0x02, 0x90, 0xd0); 
			ATBMWrite(0x02, 0x91, 0x93); 
			ATBMWrite(0x02, 0x92, 0x01); 
		}
		if(((dbCLKFreq - 30.4)<0.00001)&&((dbCLKFreq - 30.4)>-0.00001)) 
		{
			ATBMWrite(0x02, 0x90, 0x00); 
			ATBMWrite(0x02, 0x91, 0xe0); 
			ATBMWrite(0x02, 0x92, 0x01); 
		}
	}	
	return 1;			
}

/****************************************************************************
Function:    ATBMGetTPS
Parameters:  none
Return:      TPS index
Description:
      This function checks signal TPS. The return value is TPS index in GB20600-2006 		 
 ****************************************************************************/
HI_U8 ATBMGetTPS()
{  
	HI_U8 ui8TPS;
    if(ATBM_I2CREADWRITE_OK == ATBMRead(0x0d,0x04,&ui8TPS))
		return ui8TPS;
	else
		return 0xff;
}


/****************************************************************************
Function:    ATBMDebugRegister
Parameters:  none
Return:      none
Description:
      This function checks Demodulator registers,  used for debug purpose 
 ****************************************************************************/
void ATBMDebugRegister()
{
   /*common register*/
	HI_U8 ui8SoftwareVersion;		
	HI_U8  ui8dDmodState;	
	HI_U8  ui8ConfigDone;
 	HI_U8  ui8FreqOffset; 	
 	HI_U8  ui8SDPReset;	
	HI_U8  ui8I2CByPassState;
	HI_U8  ui8CANTuner;
	HI_U8  ui8ARXFLT[6];
	/*tuner related config*/
	HI_U8  ui8ADCConfig;	
	HI_U8  ui8LNAOnOff;
	HI_U8  ui8FatalRst, ui8WeakRst;
	/*TS interface configuration*/
	HI_U8 ui8SamplingEdegeMode;   
	HI_U8 ui8SerialEnabled;
	HI_U8 ui8ClockOutSelect;
	
	ATBMRead(0x02, 0x0c,&ui8SoftwareVersion);	
	ATBMRead(0x06, 0x07,&ui8dDmodState);	
	ATBMRead(0x00, 0x04,&ui8ConfigDone);	
	ATBMRead(0x00, 0x0A,&ui8SDPReset);	
	printf("Current software version: %d\n", ui8SoftwareVersion);	
	printf("ui8dDmodState:0x%x configure done:0x%x ui8SDPReset:0x%x\n",ui8dDmodState, ui8ConfigDone,ui8SDPReset);
	
	/*Demod passing I2C commands between host and tuner. 0:disable 1:enable.*/	
	ATBMRead(0x01, 0x03,&ui8I2CByPassState); 	
   	ATBMRead(0x00, 0x01,&ui8CANTuner);	
	printf("bypass state:0x%x; Tuner used(silicon tuner=0):0x%x\n", ui8I2CByPassState, ui8CANTuner);
	   	
	ATBMRead(0x06, 0x02,&ui8ADCConfig);	
	ATBMRead(0x10, 0x30,&ui8LNAOnOff);
	printf("tuner ui8ADCConfig: 0x%x; ui8LNAOnOff: 0x%x\n", ui8ADCConfig, ui8LNAOnOff);
	
   	ATBMRead(0x08, 0x00,&ui8FreqOffset);	
	printf("ui8FreqOffset 0x08_0x00: 0x%x\n", ui8FreqOffset);
	ATBMRead(0x08, 0x01,&ui8FreqOffset);	
	printf("ui8FreqOffset 0x08_0x01: 0x%x\n", ui8FreqOffset);
    
	ATBMRead(0x0A, 0x00,&ui8ARXFLT[0]);
	ATBMRead(0x0A, 0x01,&ui8ARXFLT[1]);
	ATBMRead(0x0A, 0x02,&ui8ARXFLT[2]);
	ATBMRead(0x0A, 0x03,&ui8ARXFLT[3]);
	ATBMRead(0x0A, 0x04,&ui8ARXFLT[4]);
	ATBMRead(0x0A, 0x05,&ui8ARXFLT[5]);
	printf("IF freq_lif: 0x%x 0x%x  0x%x\n", ui8ARXFLT[0], ui8ARXFLT[1], ui8ARXFLT[2]);
	printf("ADC rate ratio: 0x%x 0x%x  0x%x\n", ui8ARXFLT[3], ui8ARXFLT[4], ui8ARXFLT[5]);    	
	
	ATBMRead(0x00, 0x09,&ui8FatalRst);	
	ATBMRead(0x00, 0x0a,&ui8WeakRst);
	printf("DSP reset options ui8FatalRst = 0x%x, ui8WeakRst =  0x%x\n", ui8FatalRst, ui8WeakRst);

	ATBMRead(0x03, 0x01,&ui8SamplingEdegeMode);/*0x00: rising edge TS output; 0x01: falling edge TS output*/	  	                                                                                                                          
	ATBMRead(0x03, 0x05,&ui8SerialEnabled);    /*0: disable serial mode; 1: enable serial mode */
	ATBMRead(0x03, 0x0b,&ui8ClockOutSelect);   /*0: SPI clock output; 1: TS serial clock output */  	

	printf("TS interface ui8SerialEnabled= %d ui8SamplingEdegeMode =%d, clock select ui8ClockOutSelect%d\n",\
		ui8SerialEnabled,ui8SamplingEdegeMode, ui8ClockOutSelect);	  
}

/****************************************************************************
Function:    ATBMWriteRegArray
Parameters:  *ui8ARegTable, i32TableLen
Return:      none
Description:
      This function writes a group of registers to demodulator	 	 
 ****************************************************************************/
void ATBMWriteRegArray(HI_U8 *ui8ARegTable, int i32TableLen)
{
   int i32Iter;
   for(i32Iter=0;i32Iter<i32TableLen;i32Iter+=3)
   {
         ATBMWrite(ui8ARegTable[i32Iter],ui8ARegTable[i32Iter+1],ui8ARegTable[i32Iter+2]);
   }
}

/****************************************************************************
Function:    ATBMDebugRegArray
Parameters:  *ui8ARegTable, i32TableLen
Return:      none
Description:
     This function reads a group of registers from demodulator and prints them
     for debug purposes.	 
 ****************************************************************************/
void ATBMDebugRegArray(HI_U8 *ui8ARegTable, int i32TableLen)
{
   int i32Iter;
   HI_U8 ui8RegData;
   for(i32Iter=0;i32Iter<i32TableLen;i32Iter+=3)
   {
        ATBMRead(ui8ARegTable[i32Iter],ui8ARegTable[i32Iter+1],&ui8RegData);		
   }
}

/****************************************************************************
Function:    ATBMRead
Parameters:  ui8BaseAddr, ui8RegisterAddr, *ui8pValue
Return:      I2C read status:ATBM_I2CREADWRITE_OK is OK
Description: 
     ATBM886x demodulator register Reading API, it calls I2CRead. 	
****************************************************************************/
ATBM_I2CREADWRITE_STATUS  ATBMRead(HI_U8 ui8BaseAddr, HI_U8 ui8RegisterAddr,HI_U8 *ui8pValue)
{ 
	HI_U8 ui8AddrData[2];
    int enumRetval;
	ui8AddrData[0] = ui8RegisterAddr;
	ui8AddrData[1] = ui8BaseAddr;

    DRV_OS_SemWait_MS(&mutexI2C,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei

	enumRetval = I2CRead(ui8ATBMSlaveAddr, ui8AddrData, 2, 1);
	if(ATBM_I2CREADWRITE_OK != enumRetval)
	{
		DRV_OSSemSignal(&mutexI2C);
        printf("ATBMRead(...) ... I2C Read Data Error !!!\n");
		return ATBM_I2CREADWRITE_ERROR;
	} 	
	
	DRV_OSSemSignal(&mutexI2C);
	//One step I2C format, please port this function according to I2C format of this function..
	*ui8pValue = ui8AddrData[0];

	return ATBM_I2CREADWRITE_OK;//I2CRead(ui8ATBMSlaveAddr, 2, ui8AddrData, 1, ui8pValue);
}

/****************************************************************************
Function:    ATBMWrite
Parameters:  ui8BaseAddr, ui8RegisterAddr, ui8Data
Return:      I2C write status:ATBM_I2CREADWRITE_OK is OK  

Description: 
     ATBM886x demodulator register Writing API, It calls I2CWrite. 
    ****************************************************************************/
ATBM_I2CREADWRITE_STATUS  ATBMWrite(HI_U8 ui8BaseAddr, HI_U8 ui8RegisterAddr, HI_U8 ui8Data)
{ 

	int enumRetval ; 
	HI_U8 i2c_data[3];

	i2c_data[0] = ui8RegisterAddr;
	i2c_data[1] = ui8BaseAddr;
	i2c_data[2] = ui8Data;	
	
    DRV_OS_SemWait_MS(&mutexI2C,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei
	
	enumRetval = I2CWrite(ui8ATBMSlaveAddr, i2c_data, 2, 1);
	if(ATBM_I2CREADWRITE_OK != enumRetval)
	{
		DRV_OSSemSignal(&mutexI2C);
        printf("ATBMWrite(...) ... I2C Writer Data Error !!!\n");
		return ATBM_I2CREADWRITE_ERROR;
	} 
	
	DRV_OSSemSignal(&mutexI2C);

#if ATMB_DBG_OUTPUT
	printf("[DRV]ATBMWrite ui8BaseAddr = %x ui8RegisterAddr = %x ui8Data = %x\n", ui8BaseAddr, ui8RegisterAddr, ui8Data);	
#endif

	return ATBM_I2CREADWRITE_OK;
}

/****************************************************************************
Function:    ATBMTransRead
Parameters:  ui8BaseAddr, ui8RegisterAddr,ui8pValue
Return:      I2C read status:ATBM_I2CREADWRITE_OK is OK

Description:   
     ATBM886x demodulator register Reading API, This function is used for DVB-C transfer. 	   
****************************************************************************/
ATBM_I2CREADWRITE_STATUS  ATBMTransRead(HI_U8 ui8BaseAddr, HI_U8 ui8RegisterAddr,HI_U8 *ui8pValue)
{ 
	HI_U8 ui8AddrData[2];
    int enumRetval;
	ui8AddrData[0] = ui8BaseAddr;
	ui8AddrData[1] = ui8RegisterAddr;

    DRV_OS_SemWait_MS(&mutexI2C,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei
	//enumRetval = I2CWrite(ui8ATBMSlaveAddr, ui8AddrData, 2);
	//if(!enumRetval)
	//{
		//DRV_OSSemSignal(&mutexI2C);
        //printf("ATBMRead(...) ... I2C Writer Address Error !!!\n");
		//return ATBM_I2CREADWRITE_ERROR;
	//}
	//enumRetval = I2CRead(ui8ATBMSlaveAddr, ui8pValue, 1);
	enumRetval = I2CRead(ui8ATBMSlaveAddr, ui8AddrData, 2, 1);
	if(!enumRetval)
	{
		DRV_OSSemSignal(&mutexI2C);
        printf("ATBMRead(...) ... I2C Read Data Error !!!\n");
		return ATBM_I2CREADWRITE_ERROR;
	} 	
	DRV_OSSemSignal(&mutexI2C);
	*ui8pValue = ui8AddrData[0];
	return ATBM_I2CREADWRITE_OK;//I2CRead(ui8ATBMSlaveAddr, 2, ui8AddrData, 1, ui8pValue);      
    
}


/****************************************************************************
Function:        ATBMTransWrite
Parameters:  ui8BaseAddr, ui8RegisterAddr, ui8Data
Return:      I2C write status:ATBM_I2CREADWRITE_OK is OK 

Description: 
     ATBM886x demodulator register Writing API, I2CWrite is decoder platform independent.     
****************************************************************************/
ATBM_I2CREADWRITE_STATUS  ATBMTransWrite(HI_U8 ui8BaseAddr, HI_U8 ui8RegisterAddr, HI_U8 ui8Data)
{
	HI_U8 i2c_data[3];
    int enumRetval;
	i2c_data[0] = ui8BaseAddr;
	i2c_data[1] = ui8RegisterAddr;
	i2c_data[2] = ui8Data;
	
    DRV_OS_SemWait_MS(&mutexI2C,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei
	//enumRetval = I2CWrite(ui8ATBMSlaveAddr, i2c_data, 2);
	//if(!enumRetval)
	//{
		//DRV_OSSemSignal(&mutexI2C);
        //printf("ATBMWrite(...) ... I2C Writer Address Error !!!\n");
		//return ATBM_I2CREADWRITE_ERROR;
	//}
	enumRetval = I2CWrite(ui8ATBMSlaveAddr, i2c_data, 2, 1);
	if(!enumRetval)
	{
		DRV_OSSemSignal(&mutexI2C);
        printf("ATBMWrite(...) ... I2C Writer Data Error !!!\n");
		return ATBM_I2CREADWRITE_ERROR;
	} 	
	DRV_OSSemSignal(&mutexI2C);
	return ATBM_I2CREADWRITE_OK;// I2CWrite(ui8ATBMSlaveAddr, i2c_data, 3);
}

/****************************************************************************
Function:   ATBMDVBCWrite
Parameters: ui32AAddress: Register physical address ;ui32Data:data need to write to the register
ui32Data - 32bit data 
Return:    I2C write status:ATBM_I2CREADWRITE_OK is OK  

Description:
	For DVB-C register write, It is an encapsulated function by calling ATBMTransWrite function.			 
******************************************************************************/
ATBM_I2CREADWRITE_STATUS ATBMDVBCWrite(HI_U32 ui32AAddress, HI_U32 ui32Data)
{

	HI_U8 ui8AWData[4];
	HI_U8 ui8AddrL, ui8AddrH;


	ui8AddrL = ui32AAddress&0xff;
	ui8AddrH = (ui32AAddress>>8)&0xff;
        
	ui8AWData[0]=ui32Data&0xff,ui8AWData[1]=(ui32Data>>8)&0xff,ui8AWData[2]=(ui32Data>>16)&0xff,ui8AWData[3]=(ui32Data>>24)&0xff;

	//Add mutex lock here if there is multi-thread operation of demodulator's I2C
	//the following I2C operations will not be interrupted. 
	DVBCWriteValue(0x15, 0x03, 0x0);		
	DVBCWriteValue(0x15, 0x01, ui8AddrL);
	DVBCWriteValue(0x15, 0x02, ui8AddrH);

	DVBCWriteValue(0x15, 0x04, ui8AWData[0]);
	DVBCWriteValue(0x15, 0x05, ui8AWData[1]);
	DVBCWriteValue(0x15, 0x06, ui8AWData[2]);	
	DVBCWriteValue(0x15, 0x07, ui8AWData[3]);

	DVBCWriteValue(0x15, 0x03, 0x01);	

	DVBCWriteValue(0x15, 0x03, 0x0);
	//Release mutex lock.
	printf(" %x %x\n", ui32AAddress, ui32Data);
	return ATBM_I2CREADWRITE_OK;
}


/****************************************************************************
Function:      ATBMDVBCRead
Parameters:    ui32AAddress: Register physical address ;ui32pValue:32-bit data read from target register 
Return:        I2C write status:ATBM_I2CREADWRITE_OK is OK    

Description:
	For DVB-C register read, It is an encapsulated function by calling
	ATBMTransWrite and ATBMTransRead functions.
******************************************************************************/
ATBM_I2CREADWRITE_STATUS ATBMDVBCRead(HI_U32  ui32AAddress,HI_U32 *ui32pValue)
{

	HI_U8 ui8RoLatchOn=1,ui8RoLatchOff=0;
	HI_U8 ui8Data[4];
	HI_U8 ui8AddrL, ui8AddrH;
	HI_U32 ui32Ddata;
	ui8AddrL = ui32AAddress&0xff;
	ui8AddrH = (ui32AAddress>>8)&0xff;

	//Add mutex lock here if there is multi-thread operation of demodulator's I2C
	//the following I2C operations will not be interrupted. 

	DVBCWriteValue(0x08, 0x4d, ui8RoLatchOff);
	DVBCWriteValue(0x15, 0x03, 0x0);		
	DVBCWriteValue(0x15, 0x01, ui8AddrL);
	DVBCWriteValue(0x15, 0x02, ui8AddrH);

	DVBCWriteValue(0x08, 0x4d, ui8RoLatchOn);
	DVBCReadValue(0x15, 0x08,&ui8Data[0]);
	DVBCReadValue(0x15, 0x09,&ui8Data[1]);
	DVBCReadValue(0x15, 0x0a,&ui8Data[2]);
	DVBCReadValue(0x15, 0x0b,&ui8Data[3]);
	DVBCWriteValue(0x08, 0x4d, ui8RoLatchOff);

        //Release mutex lock.

	ui32Ddata = (ui8Data[3]<<24) + (ui8Data[2]<<16) + (ui8Data[1]<<8) +ui8Data[0];	
	*ui32pValue = ui32Ddata;
	return ATBM_I2CREADWRITE_OK;
}

/****************************************************************************
Function:      ATBMCarrierOffset
Parameters:    none
Return:        dbCarrierOffset1:double(KHz)

Description:
      This function checks DTMB signal carrier offset on field testing environment.
      This function is not meant to be used by customers. It's a debug utility to be used by FAEs.
 ****************************************************************************/
double ATBMCarrierOffset()
{
	HI_U8  ui8LockValue, ui8ChipID;
	HI_U8  ui8pABuff[4];
	double dbCarrierOffset;
	double dbCarrierOffset1;  
	ATBMRead(0x00, 0x00,&ui8ChipID);

	ui8LockValue =1; /*lock register value*/
	ATBMWrite(0x08,0x4d, ui8LockValue);

	ATBMRead(0x08, 0x27,&ui8pABuff[0]);//carrier_offset_est[7:0]
	ATBMRead(0x08, 0x28,&ui8pABuff[1]);//carrier_offset_est[15:8]
	ATBMRead(0x08, 0x29,&ui8pABuff[2]);//carrier_offset_est[21:16]

	ui8LockValue = 0; /*lock register value*/
	ATBMWrite(0x08,0x4d, ui8LockValue);
	/*=  carrier offset  =*/
	if(ui8ChipID == 0x20)
	{
		if(ui8pABuff[2]&0x20)
		{
			dbCarrierOffset = (0x3fffff+1- (((ui8pABuff[2]&0x3f)<<16)+(ui8pABuff[1]<<8)+ui8pABuff[0]))*7560000.0/2.0/PI/8388608.0;
			dbCarrierOffset = (dbCarrierOffset/1000.0);
			dbCarrierOffset1 = - dbCarrierOffset;
			//printf("DTMB signal  Carrier Offset = -%f KHz\n", dbCarrierOffset);
		}
		else
		{
			dbCarrierOffset= ((((ui8pABuff[2]&0x3f)<<16)+(ui8pABuff[1]<<8)+ui8pABuff[0]))*7560000.0/2.0/PI/8388608.0;
			dbCarrierOffset = dbCarrierOffset/1000.0;	
			dbCarrierOffset1 = dbCarrierOffset;
			//printf("DTMB signal Carrier Offset = %f KHz\n", dbCarrierOffset);                
		}
	}

	if(ui8ChipID != 0x20)
	{
		if(ui8pABuff[2]&0x80)
		{
			dbCarrierOffset = (0xffffff+1- (((ui8pABuff[2]&0xff)<<16)+(ui8pABuff[1]<<8)+ui8pABuff[0]))*7560000.0/2.0/PI/8388608.0;
			dbCarrierOffset = (dbCarrierOffset/1000.0);
			dbCarrierOffset1 = - dbCarrierOffset;
			//printf("DTMB signal  Carrier Offset = %f KHz\n", dbCarrierOffset);
		}
		else
		{
			dbCarrierOffset= ((((ui8pABuff[2]&0xff)<<16)+(ui8pABuff[1]<<8)+ui8pABuff[0]))*7560000.0/2.0/PI/8388608.0;
			dbCarrierOffset = dbCarrierOffset/1000.0;	
			dbCarrierOffset1 = dbCarrierOffset;
			//printf("DTMB signal Carrier Offset = %f KHz\n", dbCarrierOffset);                
		}
	}
	return dbCarrierOffset1;
}


/****************************************************************************
Function:      ATBM_DVBC_CarrierOffset
Parameters:    none
Return:        DVBC_CarrierOffset(KHz)

Description:
	This function checks DVB-C signal carrier offset on field testing environment.
	This function is not meant to be used by customers. It's a debug utility to be used by FAEs.
****************************************************************************/
double   ATBMDVBCCarrierOffset()
{  
	HI_U32 tmp;
	double dtmp;
	HI_U32 acq_status;
	double ItbCarFreqOffset_stat,ItbCoarseFrequencyOffset,ChannelCentreFrequency;
	double DVBC_CarrierOffset;

	//-------Carrier frequency offset, reg ItbCarFreqOffset_stat---------------
	ATBMDVBCRead(0x3C0,&tmp); //32bit signed
	ItbCarFreqOffset_stat = tmp/pow((double)2,(double)29)*dbDvbcSymbolRate*1000;//kHz

	//-----ChannelCentreFrequency , reg ItbFrequencyStatus------------------------
	 ATBMDVBCRead(0x11c,&tmp); //27bit signed      
	tmp = tmp&0x7ffffff;
	dtmp = tmp;
	if(tmp&(0x1<<26))
	{
		dtmp=-(0x7ffffff-dtmp+1);
	}
	ChannelCentreFrequency = dtmp*dbDvbcOscFreq/pow((double)2,(double)27)*1000; //kHz

	//------ItbCoarseFrequencyOffset--------------------------------------------
	ATBMDVBCRead(0x118,&tmp); //32bit signed      
	ItbCoarseFrequencyOffset = tmp*dbDvbcOscFreq/pow((double)2,(double)27)*1000; //kHz

	//-----------  calculate overall_CFO -------------
	if(ChannelCentreFrequency<0)
	{
		DVBC_CarrierOffset =  ChannelCentreFrequency  + fabs(ItbCoarseFrequencyOffset)+ ItbCarFreqOffset_stat;
	}
	else
	{
		DVBC_CarrierOffset =  ChannelCentreFrequency  - fabs(ItbCoarseFrequencyOffset)+ ItbCarFreqOffset_stat;
	}
	ATBMDVBCRead(0x3d0,&acq_status);	
	if(acq_status&(0x20))
	{
		DVBC_CarrierOffset = DVBC_CarrierOffset;
	}
	else
	{
		DVBC_CarrierOffset = -DVBC_CarrierOffset;
	}	
	printf("DVB-C signal Carrier Offset = %f KHz\n", DVBC_CarrierOffset);        
	return DVBC_CarrierOffset;		
}

/****************************************************************************
Function:	ATBM_DVBC_GetSymbolRate
Parameters:   none                
Return: 	symbol_rate(K symbol/s)

Description:
    Call this function to Check DVB-C Symbol Rate, valid for DVB-C mode.			 
******************************************************************************/
double ATBMDVBCGetSymbolRate()
{
	HI_U32 uTrlSampleRate;
	HI_U32 uTrlSampleRateError;
	HI_U32 uDecLog2DecFactor;
	HI_U8 mode;

	double		dProgrammedSymbRate;
	double		dOffset;
	double symbol_rate; // default is 6875.0;
	ATBMDVBCRead(0x218,&uTrlSampleRateError);
	ATBMDVBCRead(0x210,&uTrlSampleRate);
	ATBMDVBCRead(0x124,&uDecLog2DecFactor);
	ATBMRead(0x00, 0x13,&mode);
	if((mode != 0x01)||(uTrlSampleRate == 0)) //Not in DVB-C mode or I2C Error
	{
		return 0;
	}
	dProgrammedSymbRate = (dbDvbcOscFreq*1.0e6)/
		((double)(1<<(uDecLog2DecFactor+1))/
		((double)uTrlSampleRate/(1<<16)));

	dOffset = ((double)uTrlSampleRateError/(double)(137439))*
		((dbDvbcOscFreq*1.0e6)/(dProgrammedSymbRate * 
		(1<<uDecLog2DecFactor)));

	symbol_rate = dProgrammedSymbRate/1000;
	symbol_rate = symbol_rate + symbol_rate*(dOffset/1.0e6);
	printf("symbol rate = %f K\n", symbol_rate);
	return symbol_rate;
}

int ATBM_PPM_Test()
{
	HI_U8  ui8APhiAdj[2];  
	HI_U8  ui8ASCPPM[2];
	HI_U8  ui8ALCPPM[2];  
	HI_U8  ui8pABuffTmp[4];
	HI_U8  ui8LockValue;
	HI_U8  ui8HeaderMode;
	HI_U32 ui32Bias = 0;
	HI_U32 ui32AcqRateFrac, ui32AcqRateFracCurr;
	int    i32OffsetPPM;

	ui8LockValue =1; /*lock register value*/
	ATBMWrite(0x08, 0x4d, ui8LockValue);
	ATBMRead(0x08, 0x39,&ui8APhiAdj[0]);//ui8APhiAdj[7:0]        
	ATBMRead(0x08, 0x3a,&ui8APhiAdj[1]);//ui8APhiAdj[13:8]
	ATBMRead(0x08, 0x5B,&ui8ASCPPM[0]);     
	ATBMRead(0x08, 0x5C,&ui8ASCPPM[1]);
	ATBMRead(0x08, 0x85,&ui8ALCPPM[0]);      
	ATBMRead(0x08, 0x86,&ui8ALCPPM[1]);
	ATBMRead(0x08, 0x00,&ui8pABuffTmp[0]);//ui32AcqRateFrac[7:0]
	ATBMRead(0x08, 0x01,&ui8pABuffTmp[1]);//ui32AcqRateFrac[12:8]      

	ui8LockValue = 0; /*cancel the lock*/
	ATBMWrite(0x08,0x4d, ui8LockValue);
	ui32AcqRateFrac = (ui8pABuffTmp[1]*256)+ui8pABuffTmp[0];
	ui32AcqRateFracCurr = ui32AcqRateFrac;              
	ATBMRead(0x08,0x2d,&ui8HeaderMode); 
	if(ui8APhiAdj[1]&(0x20)) // sign bit, negative
	{    
		//ui32Bias= (0xfff+1- (((ui8APhiAdj[1]&0x3f)*256)+ ui8APhiAdj[0]))*1000000/8388608;//power(2,23)          
		ui32Bias= (0x3fff+1- (((ui8APhiAdj[1]&0x3f)*256)+ ui8APhiAdj[0]))*1192/10000;//power(2,23)   
		ui32AcqRateFrac = ui32AcqRateFrac -ui32Bias;
	}else     
	{  
		ui32Bias = ((((ui8APhiAdj[1]&0x3f)*256)+ ui8APhiAdj[0]))*1192/10000;           
		ui32AcqRateFrac = ui32AcqRateFrac + ui32Bias;
	}
	if((ui8HeaderMode&0x3)==2) //single-carrier mode
	{
		if(ui8ASCPPM[1]&(0x01))//sign bit, negative
		{
			//      ui32Bias =(0x1ff+1-(((ui8ASCPPM[1]&0x01)*256)+ ui8ASCPPM[0]))*1000000/262144;//power(2,18)              
			ui32Bias =(0x1ff+1-(((ui8ASCPPM[1]&0x01)*256)+ ui8ASCPPM[0]))*38/10;//power(2,18)
			ui32AcqRateFrac = ui32AcqRateFrac - ui32Bias;
			ui8pABuffTmp[3]= (ui32AcqRateFrac>>8)&0xff;
			ui8pABuffTmp[2]= ui32AcqRateFrac&0xff;
		}
		else{
			ui32Bias =((( ui8ASCPPM[1]&0x01)*256)+ ui8ASCPPM[0])*38/10;                      
			ui32AcqRateFrac = ui32AcqRateFrac + ui32Bias;
			ui8pABuffTmp[3]= (ui32AcqRateFrac>>8)&0xff;
			ui8pABuffTmp[2]= ui32AcqRateFrac&0xff;
		}
	}
	//multi-carrier mode: ui8APhiAdj + ui8ALCPPM
	else if( ((ui8HeaderMode&0x3)==1)||(( ui8HeaderMode&0x3)==3)  )//multi-carrier mode
	{
		if(ui8ALCPPM[1]&(0x80))//sign bit
		{
			ui32Bias=(0xffff+1- (((ui8ALCPPM[1])*256)+ ui8ALCPPM[0]))*1192/10000;//power(2,23)
			ui32AcqRateFrac = ui32AcqRateFrac - ui32Bias;
			ui8pABuffTmp[3]= (ui32AcqRateFrac>>8)&0xff;
			ui8pABuffTmp[2]= ui32AcqRateFrac&0xff;
		}
		else{
			ui32Bias = ((( ui8ALCPPM[1])*256)+ ui8ALCPPM[0])*1192/10000;
			ui32AcqRateFrac = ui32AcqRateFrac + ui32Bias;
			ui8pABuffTmp[3]= (ui32AcqRateFrac>>8)&0xff;
			ui8pABuffTmp[2]= ui32AcqRateFrac&0xff;
		}

	}        
	i32OffsetPPM = ui32AcqRateFrac - ui32AcqRateFracCurr;
	return i32OffsetPPM;
}

void ATBM_GPO_I2CINT_Output(HI_U8 ui8Level)
{
	HI_U8 ui8GPIOReg;   	
	ATBMRead(0x10, 0xf7,&ui8GPIOReg); 
	ui8GPIOReg |= 0x20; //Set bit5, Enable I2CINT output
	if(ui8Level == 0)
		ui8GPIOReg &= 0xef; //Clear bit4, I2CINT pin output 0
	else
		ui8GPIOReg |= 0x10; //Set bit4, I2CINT pin output 1
	ATBMWrite(0x10, 0xf7, ui8GPIOReg);	
}

void ATBM_GPO_PWM1_Output(HI_U8 ui8Level)
{
	HI_U8 ui8GPIOReg;    	
	ATBMRead(0x10, 0xf7,&ui8GPIOReg); 
	ui8GPIOReg |= 0x08; //Set bit3,Enable PWM1 output
	if(ui8Level == 0)
		ui8GPIOReg &= 0xfb; //clear bit2,PWM1 pin output 0
	else 
		ui8GPIOReg |= 0x04; //set bit2,PWM1 pin output 1
	ATBMWrite(0x10, 0xf7, ui8GPIOReg);	
}

void ATBM_GPO_TestIO23_Output(HI_U8 ui8Level)
{
	HI_U8 ui8GPOReg; 
	HI_U8 ui8Data;
	ATBMRead(0x10, 0xf7,&ui8GPOReg); 
	ATBMRead(0x10, 0xfb,&ui8Data); 
	ui8GPOReg |= 0x80; //Set bit7,Enable TestIO23 output
	ui8Data|= 0x04; //set bit2
	if(ui8Level == 0)
	{
		ui8Data&=0xef; //clear bit4, this bit is output level
	}
	else
	{
		ui8Data|= 0x10; //set bit4 as High level
	}
	ATBMWrite(0x10, 0xfb, ui8Data);
	ATBMWrite(0x10, 0xf7, ui8GPOReg);  
}

/****************************************************************************
Function:    ATBM_TestIO23_Indicate_TS_Lock
Parameters: none
Return:      none
Description:  
    Enable lock indicator output on testIO23 when TS lock detected.
****************************************************************************/
void ATBM_TestIO23_Indicate_TS_Lock()
{
     HI_U8 ui8GPOReg; 
     HI_U8 ui8Data;
     ATBMRead(0x10, 0xf7,&ui8GPOReg); 
     ATBMRead(0x10, 0xfb,&ui8Data); 
     ui8GPOReg|= 0x80; //set bit7
     ui8Data&=0xfb;  //clear bit2 
     ATBMWrite(0x10, 0xfb, ui8Data);
     ATBMWrite(0x10, 0xf7, ui8GPOReg);  
     ATBMWrite(0x00, 0x1a, 0x00); //TestIO23 using TS lock.
}

/****************************************************************************
Function:    ATBM_TestIO23_Indicate_FEC_No_Error
Parameters:  none
Return:      none
Description:  
    TestIO23 auto output high only when FEC no Error occurred.
    Strict on lock indicator, if FEC error occurred, lock indicator will be off.
    Lock indicator will be on only if not FEC error occurred
****************************************************************************/
void ATBM_TestIO23_Indicate_FEC_No_Error()
{
     HI_U8 ui8LockUseErrorInv;       
     ui8LockUseErrorInv = 0x01;
     ATBM_TestIO23_Indicate_TS_Lock();          //enable TestIO23 normal output
     ATBMWrite(0x00, 0x1a, ui8LockUseErrorInv); //TestIO23 using FEC no error
}

void ATBM_GPO_TestIO20_Output(HI_U8 ui8Level)
{
	HI_U8 ui8GPOReg; 
	HI_U8 ui8Data;
	ATBMRead(0x10, 0xf7,&ui8GPOReg); 
	ATBMRead(0x10, 0xfb,&ui8Data); 
	ui8GPOReg |= 0x40; //Set bit6,Enable TestIO20 output
	ui8Data|=0x02; //set bit1 enable
	if(ui8Level == 0)
	{
		ui8Data&=0xf7; //clear bit3
	}
	else
	{   
		ui8Data|=0x08; //set bit3 output 1    
	}
	ATBMWrite(0x10, 0xfb, ui8Data);
	ATBMWrite(0x10, 0xf7, ui8GPOReg);  
}

/****************************************************************************
Function:    ATBM_TestIO20_Indicate_FEC_Error
Parameters: none
Return:      none
Description: 
     Enable error indicator output on testIO20 when FEC error detected.
****************************************************************************/
void ATBM_TestIO20_Indicate_FEC_Error()
{
     HI_U8 ui8GPOReg; 
     HI_U8 ui8Data;
     ATBMRead(0x10, 0xf7,&ui8GPOReg); 
     ATBMRead(0x10, 0xfb,&ui8Data); 
     ui8GPOReg|= 0x40; //set bit6 
     ui8Data&=0xfd;  //clear bit1
     ATBMWrite(0x10, 0xfb, ui8Data);
     ATBMWrite(0x10, 0xf7, ui8GPOReg);  
     ATBMWrite(0x00, 0x1b, 0x00 );    //TestIO20 using FEC error
}

/****************************************************************************
Function:    ATBM_TestIO20_Indicate_TS_Unlock
Parameters:  none
Return:      none
Description:   
    Strict on Error indicator, if lock occurred (TS sync is HI_TRUE),
    error indicator will be off. 
    error indicator will be on when TS sync is not checked.
****************************************************************************/
void ATBM_TestIO20_Indicate_TS_Unlock()
{
     HI_U8 ui8ErrorUseLockInv;          
     ui8ErrorUseLockInv = 0x01;
     ATBM_TestIO20_Indicate_FEC_Error(); //enable TestIO20 normal output error
     ATBMWrite(0x00, 0x1b, ui8ErrorUseLockInv); //using TS unlock
}

/****************************************************************************
Function:    ATBMErrorDurationMillisecond
Parameters:  ms (0--275 milliseconds)
Return:      none
Description:   
  Set error indicator duration time.
  The default time is 8.6MS (0x800*4.2us), register value is 0x800.
  The maximum value is 275 milliseconds
****************************************************************************/
void ATBMErrorOnDurationMillisecond(int i32MS)
{
     int   i32SetValue;
     HI_U8 ui8RegHigh, ui8RegLow;
     i32SetValue = (int)(i32MS*10000/42);
     ui8RegLow = (HI_U8)(i32SetValue&0xff);
     ui8RegHigh = (HI_U8)(i32SetValue/256);
     ATBMWrite(0x15, 0x2b, ui8RegLow);
     ATBMWrite(0x15, 0x2c, ui8RegHigh);
}
/****************************************************************************
Function:    ATBMLockOffDurationMillisecond
Parameters:  ms (0--1100 milliseconds)
Return:      none
Description:   
  Set lock indicator off Duration time after unlocked. 
  The default time is 34.4ms:(0x800*16.8us), register value is 0x800.
  The maximum value is 1100 milliseconds
****************************************************************************/
void ATBMLockOffDurationMillisecond(int i32MS)
{
     int i32SetValue;
     HI_U8 ui8RegHigh, ui8RegLow;
     i32SetValue = (int)(i32MS*10000/168);
     ui8RegLow = (HI_U8)(i32SetValue&0xff);
     ui8RegHigh = (HI_U8)(i32SetValue/256);
     ATBMWrite(0x03, 0x19, ui8RegLow);
     ATBMWrite(0x03, 0x1a, ui8RegHigh);
}

/****************************************************************************
Function:    ATBM_GetGuradInterval
Parameters:  none
Return:      gi_interval

Description: 
   Function to get guard interval from TPS value: 420 595, 945
****************************************************************************/
DTMB_GUARD_INTERVAL  ATBM_GetGuradInterval()
{
	HI_U8 pn_value,PN;  
	DTMB_GUARD_INTERVAL gi_interval;
	ATBMRead(0x08, 0x2d,&pn_value);   
	PN = (HI_U8)(pn_value&0x03);
	// check Guard interval
	if(PN == 2)
	{
		gi_interval = GI_595;
	}
	else if(PN == 1)
	{

		gi_interval = GI_420;
	}
	else if(PN == 3)
	{

		gi_interval= GI_945;
	}
	else
	{
		gi_interval = GI_UNKNOWN;		
	}
	return gi_interval;
}

/****************************************************************************
Function:    ATBM_GetQamIndex
Parameters:  none
Return:      code_rate

Description: 
   Function to get QAM from TPS value
   4QAM-NR, 4QAM, 16QAM, 32QAM, 64QAM
****************************************************************************/
DTMB_QAM_INDEX  ATBM_GetQamIndex()
{
	HI_U8 tps_code;
	DTMB_QAM_INDEX qam_index;
	ATBMRead(0x0d, 0x04,&tps_code);
	if((tps_code>24)||(tps_code<3))
	{
		qam_index =DTMB_QAM_UNKNOWN;
		return qam_index;
	}
	if(tps_code<=4)
	{
		qam_index = DTMB_QAM_4QAM_NR;
	}
	else if(tps_code<=10)
	{
		qam_index = DTMB_QAM_4QAM;;
	}
	else if(tps_code<=16)
	{
		qam_index = DTMB_QAM_16QAM;
	}
	else if(tps_code<=18)
	{
		qam_index = DTMB_QAM_32QAM;
	}
	else if(tps_code<=24)
	{
		qam_index = DTMB_QAM_64QAM;
	}
	else
	{
		qam_index = DTMB_QAM_UNKNOWN;
	}
	return qam_index;
}

/****************************************************************************
Function:    ATBM_GetCodeRate
Parameters:  none
Return:      code_rate

Description: 
   Function to get FEC code rate from TPS value: 0.4, 0.6, 0.8
****************************************************************************/
DTMB_CODE_RATE  ATBM_GetCodeRate()
{
	HI_U8 tps_code;
	DTMB_CODE_RATE code_rate;
	ATBMRead(0x0d, 0x04,&tps_code);
	if((tps_code>24)||(tps_code<3))
	{
		code_rate = DTMB_CODE_RATE_UNKNOWN;
		return code_rate;
	}
	if((tps_code == 5)||(tps_code == 6))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_4;
	}
	else if ((tps_code == 11)||(tps_code == 12))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_4;
	}
	else if((tps_code == 19)||(tps_code == 20))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_4;
	}
	else if((tps_code == 7)||(tps_code == 8))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_6;
	}
	else if((tps_code == 13)||(tps_code == 14))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_6;
	}
	else if((tps_code == 21)||(tps_code == 22))
	{
		code_rate = DTMB_CODE_RATE_0_DOT_6;
	}
	else
	{
		code_rate = DTMB_CODE_RATE_0_DOT_8;
	}
	return code_rate;
}

/****************************************************************************
Function:    ATBM_GetInterleavingMode
Parameters:  none
Return:      time_interleave

Description: 
Function to get Time interleaving mode from TPS value: 240 or 720
****************************************************************************/
DTMB_TIME_INTERLEAVE  ATBM_GetInterleavingMode()
{
	HI_U8 tps_code;
	DTMB_TIME_INTERLEAVE time_interleave = DTMB_TIME_INTERLEAVER_UNKNOWN;
	ATBMRead(0x0d, 0x04,&tps_code);

	if((tps_code>24)||(tps_code<3))
	{
		time_interleave = DTMB_TIME_INTERLEAVER_UNKNOWN;
		return time_interleave;
	}
	if((tps_code%2) == 0)
	{
		time_interleave = DTMB_TIME_INTERLEAVER_720;
	}
	if((tps_code%2) != 0)
	{
		time_interleave = DTMB_TIME_INTERLEAVER_240;
	}
	return  time_interleave;
}

/****************************************************************************
Function:    ATBM_GetCarrierMode
Parameters:  none
Return:      carrier_mode

Description: 
Function to get carrier mode: single carrier or Multi-carrier
****************************************************************************/
DTMB_CARRIER_MODE   ATBM_GetCarrierMode()
{
	HI_U8 is_sc;
	DTMB_CARRIER_MODE carrier_mode;
	ATBMRead(0x0d, 0x0f,&is_sc); 
	//check carrier mode
	if(is_sc == 1)
	{
		carrier_mode = DTMB_SINGLE_CARRIER;
	}
	else if(is_sc == 0)
	{

		carrier_mode = DTMB_MULTI_CARRIER;
	}
	else
	{
		carrier_mode = DTMB_CARRIER_UNKNOWN;		
	}	
	return carrier_mode;
}

/****************************************************************************
Function:    ATBMGetSignalParameters
Parameters:  singal_params
Return:       none

Description: 
API to get all DTMB signal parameters:
Carrier mode, Constellation QAM index,  FEC code rate, Guard interval
and time interleaving mode.
****************************************************************************/
void ATBMGetSignalParameters(DTMB_SIGNAL_PARAMS *singal_params)
{
	singal_params->dtmb_carrier_mode = ATBM_GetCarrierMode();

	singal_params->dtmb_qam_index =  ATBM_GetQamIndex();

	singal_params->dtmb_code_rate = ATBM_GetCodeRate();

	singal_params->dtmb_guard_interval =   ATBM_GetGuradInterval();

	singal_params->dtmb_time_interleave = ATBM_GetInterleavingMode();
}

/****************************************************************************
Function:    ATBMGetDTMBBitRate
Parameters:  none
Return:       payload

Description: 
API to get DTMB MPEG2 TS payload in unit of Mbps
****************************************************************************/
double  ATBMGetDTMBBitRate()
{
	DTMB_CODE_RATE code_rate;
	DTMB_QAM_INDEX qam_index;
	DTMB_GUARD_INTERVAL gi_interval;
	
	double payload = 0;
	
	qam_index =  ATBM_GetQamIndex();
	code_rate = ATBM_GetCodeRate();
	gi_interval =   ATBM_GetGuradInterval();	

	if(qam_index == DTMB_QAM_4QAM) 
	{	
		if (code_rate == DTMB_CODE_RATE_0_DOT_4) 
		{
			if(gi_interval == GI_420)
			{
				payload = 5.414;
			}
			else if(gi_interval == GI_595)
			{
				payload = 5.198;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 4.813;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_6) 
		{
			if(gi_interval == GI_420)
			{
				payload = 8.122;
			}
			else if(gi_interval == GI_595)
			{
				payload = 7.797;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 7.219;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_8) 
		{
			if(gi_interval == GI_420)
			{
				payload = 10.829;
			}
			else if(gi_interval == GI_595)
			{
				payload = 10.396;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 9.626;
			}
			else 
			{
				payload = 0;
			}
		}
		else
		{
			payload = 0;
		}
	}
	else if (qam_index == DTMB_QAM_16QAM) 
	{
		if (code_rate == DTMB_CODE_RATE_0_DOT_4) 
		{
			if(gi_interval == GI_420)
			{
				payload = 10.829;
			}
			else if(gi_interval == GI_595)
			{
				payload = 10.396;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 9.626;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_6) 
		{
			if(gi_interval == GI_420)
			{
				payload = 16.243;
			}
			else if(gi_interval == GI_595)
			{
				payload = 15.593;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 14.438;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_8) 
		{
			if(gi_interval == GI_420)
			{
				payload = 21.658;
			}
			else if(gi_interval == GI_595)
			{
				payload = 20.791;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 19.251;
			}
			else 
			{
				payload = 0;
			}
		}
		else
		{
			payload = 0;
		}
	}
	else if (qam_index == DTMB_QAM_64QAM)
	{ 
		if (code_rate == DTMB_CODE_RATE_0_DOT_4) 
		{
			if(gi_interval == GI_420)
			{
				payload = 16.243;
			}
			else if(gi_interval == GI_595)
			{
				payload = 15.593;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 14.438;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_6) 
		{
			if(gi_interval == GI_420)
			{
				payload = 24.365;
			}
			else if(gi_interval == GI_595)
			{
				payload = 23.390;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 21.658;
			}
			else 
			{
				payload = 0;
			}
		}
		else if (code_rate == DTMB_CODE_RATE_0_DOT_8) 
		{
			if(gi_interval == GI_420)
			{
				payload = 32.486;
			}
			else if(gi_interval == GI_595)
			{
				payload = 31.187;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 28.877;
			}
			else 
			{
				payload = 0;
			}
		}
		else
		{
			payload = 0;
		}
	}
	else if((qam_index == DTMB_QAM_32QAM))
	{
		if (code_rate == DTMB_CODE_RATE_0_DOT_8) 
		{
			if(gi_interval == GI_420)
			{
				payload = 27.072;
			}
			else if(gi_interval == GI_595)
			{
				payload = 25.989;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 24.064;
			}
			else 
			{
				payload = 0;
			}
		}else
		{
			payload = 0;
		}
	}
	else if((qam_index == DTMB_QAM_4QAM_NR))
	{
		if (code_rate == DTMB_CODE_RATE_0_DOT_8) 
		{
			if(gi_interval == GI_420)
			{
				payload = 5.414;
			}
			else if(gi_interval == GI_595)
			{
				payload = 5.198;
			}
			else if (gi_interval == GI_945) 
			{
				payload = 4.813;
			}
			else 
			{
				payload = 0;
			}
		}else
		{
			payload = 0;
		}
	}
	else
	{
		payload = 0;
	}	
	return payload;
}

/*Internal function used by ATBMSetClock64M*/
void ATBMSetTS_SPIClock64M()
{
	unsigned char tmp_data;
	ATBMRead(0x06, 0x0e, &tmp_data);
	tmp_data&=0x8f; //clear bit[14:12]
	tmp_data|=0x60; //set bit[14:12]:110
	ATBMWrite(0x06, 0x0e, tmp_data);
}

/*Internal function used by ATBMSetClock64M*/
void ATBMSetSYS_Clock64M()
{
	unsigned char adc_rfv0, adc_rfv1;
	ATBMRead(0x06, 0x0d, &adc_rfv0);
	adc_rfv0&=0x7f; //clear bit7
	ATBMRead(0x06, 0x0e, &adc_rfv1);
	adc_rfv1&=0xf7; //clear bit[11] 

	adc_rfv0|=0x80; //set bit[7]:1
	ATBMWrite(0x06, 0x0d, adc_rfv0);
	adc_rfv1|=0x08; //set bit[11]:1
	ATBMWrite(0x06, 0x0e, adc_rfv1);	
}

/*Internal function used by ATBMSetClock72M*/
void ATBMSetTS_SPIClock72M()
{
	unsigned char tmp_data;
	ATBMRead(0x06, 0x0e, &tmp_data);
	tmp_data&=0x8f; //clear bit[14:12], default is 72M for 24MHz Crystal
	tmp_data|=0x00;//set bit[14:12]:000
	ATBMWrite(0x06, 0x0e, tmp_data);
}

/*Internal function used by ATBMSetClock72M*/
void ATBMSetSYS_Clock72M()
{
	unsigned char adc_rfv0,adc_rfv1;
	ATBMRead(0x06, 0x0e, &adc_rfv1);
	adc_rfv1&=0xf7; //clear bit[11] 
	ATBMRead(0x06, 0x0d, &adc_rfv0);
	adc_rfv0&=0x7f; //clear bit7

	adc_rfv0|=0x00;//set bit[7]:0
	ATBMWrite(0x06, 0x0d, adc_rfv0); 
	adc_rfv1|=0x00;//set bit[11]:0
	ATBMWrite(0x06, 0x0e, adc_rfv1); 			
}

/****************************************************************************
Function:    ATBMSetClock64M
Parameters:  none
Return:      none  

Description:
      Call this function to change TS SPI clock,and sys clock to 64MHz, 
      the default used clock is 72MHz with 24MHz crystal  or oscillator.

	  Valid only for 24MHz crystal or oscillator.
******************************************************************************/
void ATBMSetClock64M()
{
	unsigned char ui8Rst;
	ATBMWrite(0x01, 0x0c, 0); 
	Delayms(2);
	ATBMSetTS_SPIClock64M();
	ATBMSetSYS_Clock64M();    
	if((dbDvbcSymbolRate > 7.5)||(dbDvbcSymbolRate < 1))
	{
		dbDvbcSymbolRate = 6.875;
	}
	ui8Rst =(HI_U8)floor(3.4375/dbDvbcSymbolRate*64/2 + 0.5); 
	ATBMWrite(0x15, 0x37, ui8Rst);
	Delayms(2);
	ATBMWrite(0x01, 0x0c, 1); 
}

/****************************************************************************
Function:    ATBMSetClock72M
Parameters:  none
Return:      none  

Description:
      Call this function to change TS SPI clock,and sys clock to 72MHz, 
      the default used clock is 72MHz with 24MHz crystal or oscillator.

	  Valid only for 24MHz crystal or oscillator.
******************************************************************************/
void ATBMSetClock72M()
{
	unsigned char ui8Rst;
	ATBMWrite(0x01, 0x0c, 0); 
	Delayms(2);
	ATBMSetTS_SPIClock72M();
	ATBMSetSYS_Clock72M();
	if((dbDvbcSymbolRate > 7.5)||(dbDvbcSymbolRate < 1))
	{
		dbDvbcSymbolRate = 6.875;
	}
	ui8Rst =(HI_U8)floor(3.4375/dbDvbcSymbolRate*72/2 + 0.5); 
	ATBMWrite(0x15, 0x37, ui8Rst);
	Delayms(2);
	ATBMWrite(0x01, 0x0c, 1); 
}

/****************************************************************************

   The following are APIs users need to implement based on their platform.you should change to 
   the header file to modify the enum ATBM_I2CREADWRITE_STATUS which value is success for i2c write if the enum define
   is not the same as your i2c return value

   I2CSlaveAddr:设备I2C地址
   data:数据指针，指针data前面addrlen个数据是芯片偏移地址，后面指向的是要写入的数据值
   addrlen:指定设备偏移地址的字节数
   length:需要写入寄存器的个数
                
****************************************************************************/
int I2CWrite(HI_U8 I2CSlaveAddr, HI_U8 *data, int addrlen, int length)
{
   /* I2C write, please port this function*/
	HI_S32 ret = 1;
	HI_U32 RegAddr = 0;
	HI_U8 *pdata = NULL;
	int len;

	if(NULL == data)
	{
		printf("I2CWrite data is NULL!\n");
		return ATBM_I2CREADWRITE_ERROR;
	}

	if(addrlen > 4)
	{
		printf("[DRV]I2CWrite addrlen is too long!\n");
		return ATBM_I2CREADWRITE_ERROR;
	}
	
	//把芯片偏移地址设置到RegAddr中
	for(len = 0;len < addrlen;len ++)
	{
		RegAddr += (*(data + len) << (8*len));
	}
	//移动指针，使得pdata指向所需要写入寄存器的数据指针
	pdata = data + addrlen;
	
	//ret = DRV_I2CWrite(I2CSlaveAddr, data, length);
	ret = HI_UNF_I2C_Write(TUNER_I2C_NUM, I2CSlaveAddr, RegAddr, addrlen, pdata, length);
	if(HI_SUCCESS != ret)
	{ 
		printf("I2CWrite HI_UNF_I2C_Write is ret = %x!!!\n",ret);
		return ATBM_I2CREADWRITE_ERROR;
	}

	return ATBM_I2CREADWRITE_OK;
}


/****************************************************************************************  
I2CRead:  Read one register by one step I2C format, parameters: 
   I2CSlaveAddr:  I2C slave address;   
   addr_length:   address length, here is 2 bytes; 
   addr_dat:      pointer to 2 bytes register address;  
   data_length:   Data length (in byte) to read, here is 1 byte;
   reg_dat:       pointer to retuned 1 byte register value;
   
   Reading register 0x0004 the I2C sequences is:   
   S_0x80_A_0x00_A_0x04_A_S_0x81_A_0x01_N_P   
   
   S:I2C start;
   A:ACK;
   N:NACK;
   P:stop   

   I2CSlaveAddr:设备I2C地址
   data:数据指针，指针data前面addrlen个数据是芯片偏移地址
   addrlen:指定设备偏移地址的字节数
   length:需要读取寄存器的个数
   
****************************************************************************/
int I2CRead(HI_U8 I2CSlaveAddr, HI_U8 *data, int addrlen, int length)
{
 /*I2C read in one step format, please port this function if needed*/	
	unsigned int ret = 1;
 	HI_U32 RegAddr = 0;
	int len;

	if(NULL == data)
	{
		printf("I2CRead data is NULL!\n");
		return ATBM_I2CREADWRITE_ERROR;
	}

	if(addrlen > 4)
	{
		printf("[DRV]I2CRead addrlen is too long!\n");
		return ATBM_I2CREADWRITE_ERROR;
	}
	
	//把芯片偏移地址设置到RegAddr中
	for(len = 0;len < addrlen;len ++)
	{
		RegAddr += (*(data + len) << (8*len));
	}
	//ret = DRV_I2CRead(I2CSlaveAddr, data, length);
	ret = HI_UNF_I2C_Read(TUNER_I2C_NUM, I2CSlaveAddr, RegAddr, addrlen, data, length);
	if(HI_SUCCESS != ret)
	{ 
		printf("I2CWrite HI_UNF_I2C_Read is ret = %x!!!\n",ret);
		return ATBM_I2CREADWRITE_ERROR;
	}

	return ATBM_I2CREADWRITE_OK;
}

static BOOL mIsHardwareResetFirstCall;
/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void drv_SetConfigGpioPinMux(void)
{
	HI_S32 Ret;
	HI_U32 GpioRegValue;
	
	Ret = HI_SYS_ReadRegister((0x10203000 + 0x015c), &GpioRegValue); 
	if(HI_SUCCESS == Ret)
	{
		printf("[DRV]DVBTunerInit HI_SYS_ReadRegister pioRegValue = %x\n",GpioRegValue);
	}
    GpioRegValue &= 0xFFFFFFF8;
    GpioRegValue |= 0x00000003;
    Ret = HI_SYS_WriteRegister((0x10203000 + 0x015c), GpioRegValue); 
	if(HI_SUCCESS == Ret)
	{
		printf("[DRV]DVBTunerInit HI_SYS_WriteRegister pioRegValue = %x\n",GpioRegValue);
	}
}

extern void DemodHardwareReset(void)
{
	HI_S32 Ret;
	
	if(mIsHardwareResetFirstCall == FALSE)
	{
		mIsHardwareResetFirstCall = TRUE;
		
		//设置复用引脚为GPIO12_6
		drv_SetConfigGpioPinMux();
		
		Ret = HI_UNF_GPIO_Open();
	    if (HI_SUCCESS != Ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
	        return;
	    }

	    Ret = HI_UNF_GPIO_SetDirBit(12*8+6, HI_FALSE); /*set GPIO12_6 direction is output*/
	    if (HI_SUCCESS != Ret)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
	        HI_UNF_GPIO_Close();
	        return;
	    }
	}

	Ret = HI_UNF_GPIO_WriteBit(12*8+6, HI_FALSE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return;
    }
	usleep(25*1000); //25ms低电平

	Ret = HI_UNF_GPIO_WriteBit(12*8+6, HI_TRUE);
    if (HI_SUCCESS != Ret)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, Ret);
        return;
    }
	usleep(100*1000);  //100ms高电平

	return;
}

extern void Delayms (int ms) 
{ 
    if (ms > 0)
        usleep(ms*1000);
    return;
}

static void drv_TunerCallback(BOOL Locked)
{
    if(mTunerCallback_user != NULL)
        mTunerCallback_user(Locked);
    printf("Callback:Locked[%d]!\n",Locked);

	return;
}

U32 drv_TunerLockCallBack(BOOL IsLock)
{
   if(mTunerCallback != NULL)
       mTunerCallback(IsLock);			
   return 0;    
}

DRV_ErrCode DRV_TunerEnableCallback(DRV_TunerCallback Callback_f)
{
    mTunerCallback_user = Callback_f;
    return DRV_NO_ERROR;
}

DRV_ErrCode DRV_TunerDisableCallback(void)
{
    mTunerCallback_user = NULL;
    return DRV_NO_ERROR;
}

HI_BOOL AppRecivedLock = 0;  //默认应用开机收到UNLOCK消息
static HI_BOOL isFirst = 1;  //是否是开机或者设置新频点后第一次上报失锁消息
static void ATBM8830_task(void *pvParam)
{
	int result,error =0 ;
	int DemodStatus;
	HI_U32 beginclock = 0, endclock = 0;
	int lostcount = 0;
	
	
	beginclock = DRV_OS_TimeNow();
	while(1)
	{ 
		//{
		//	HI_U32  au32GpioRegValue;
		//	error = HI_SYS_ReadRegister((0x10203000 + 0x0150), &au32GpioRegValue); 
		//	if(HI_SUCCESS != error)
		//	{
		//		printf("[DRV]DVBTunerInit HI_SYS_ReadRegister ErrCode = %x\n",error);
		//	}
		//	printf("-->[DRV]DRV_PtiInit HI_SYS_ReadRegister au32GpioRegValue = %x\n",au32GpioRegValue);
		//	usleep(1000*1000);
		//}
	
		beginclock = DRV_OS_TimeNow();
		result = DRV_OS_SemWait_MS(&semNewTS,DRVOS_WAIT_FINITY,500); 
		if (Frequence > 50000 && Frequence < 900000)
		{
			if(result != -1)  
			{
				DRV_OS_SemWait_MS(&semSetNewParam,DRVOS_WAIT_INFINITY,0);
				error = SetTuner(Frequence);
				if(error == 0)
				{
					printf("SetTuner ... I2C Writer %d Error \n", Frequence);
				}
				DRV_OSSemSignal(&semSetNewParam);

				DemodStatus = 0;
				DRV_OS_SemWait_MS(&mutexI2Ctunersignal,DRVOS_WAIT_INFINITY,0);
				DemodStatus = ATBMChannelLockCheckForManual();
				DRV_OSSemSignal(&mutexI2Ctunersignal);
				
				if ( DemodStatus == 1)
				{ 
					endclock  = DRV_OS_TimeNow();
					if(beginclock < endclock)
					{
						TUNER_DEBUG("LOCK TIME : %d ms \n",endclock-beginclock);  
					}
					else
					{
						TUNER_DEBUG("LOCK TIME : %d ms \n",beginclock-endclock); 
					}
					
					g_TunerLock=HI_TRUE;
					DRV_Lock_Display(HI_TRUE);
					
					if(AppRecivedLock == 0)
					{
						drv_TunerCallback(TRUE);
						TUNER_DEBUG("LOCK Callback,line[%d]!!! \n",__LINE__); 
						AppRecivedLock = 1;
						lostcount =0;
					}
					
					TUNER_DEBUG("New Search ..... Signal Locked at Frequence: %d \n", Frequence);
					   // we have nodified the APP that we have locked the signal
					
				}
				else
				{
					g_TunerLock = HI_FALSE;
					DRV_Lock_Display(HI_FALSE);
					
					lostcount++;
					if(lostcount >= TUNER_UNLOCK_COUNT)
					{
						DRV_OSSemSignal(&semNewTS);
						if(isFirst == 1)
						{
							drv_TunerCallback(FALSE);
							TUNER_DEBUG("UNLOCK Callback,line[%d]!!! \n",__LINE__); 
							AppRecivedLock = 0;
							isFirst = 0;
						}
						else if(AppRecivedLock == 1)
						{
							drv_TunerCallback(FALSE);
							TUNER_DEBUG("UNLOCK Callback,line[%d]!!! \n",__LINE__); 
							AppRecivedLock = 0; 
						}
					}

					TUNER_DEBUG("New Search......  Signal Lost at Frequence %d \n", Frequence);                 
				}
			}
			else // tracking
			{ 
				DemodStatus =0; 
				DRV_OS_SemWait_MS(&mutexI2Ctunersignal,DRVOS_WAIT_INFINITY,0);
				DemodStatus = ATBMChannelLockCheck();
				DRV_OSSemSignal(&mutexI2Ctunersignal);
				
				if(DemodStatus == 1)  //still locked
				{
					g_TunerLock = HI_TRUE; 
					DRV_Lock_Display(HI_TRUE);
					
					if (AppRecivedLock == 0)
					{ 
						drv_TunerCallback(TRUE);
						TUNER_DEBUG("LOCK Callback,line[%d]!!! \n",__LINE__); 
						AppRecivedLock = 1;
						lostcount =0;
					}
					
					TUNER_DEBUG("Tracking.... Signal Locked at  Frequence: %d \n",  Frequence);
				}
				else
				{
					g_TunerLock = HI_FALSE;
					DRV_Lock_Display(HI_FALSE);
					
					lostcount++;
					if(lostcount >= TUNER_UNLOCK_COUNT)
					{
						DRV_OSSemSignal(&semNewTS);
						if(isFirst == 1)
						{
							drv_TunerCallback(FALSE);
							TUNER_DEBUG("UNLOCK Callback,line[%d]!!! \n",__LINE__); 
							AppRecivedLock = 0;
							isFirst = 0;
						}
						else if(AppRecivedLock == 1)
						{
							drv_TunerCallback(FALSE);
							TUNER_DEBUG("UNLOCK Callback,line[%d]!!! \n",__LINE__); 
							AppRecivedLock = 0;
						}
					}
					
					TUNER_DEBUG("Tracking .... Signal Lost at  Frequence: %d \n",  Frequence);
				}
			}
		}
		else
		{
			usleep(800*1000);
			g_TunerLock = HI_FALSE;
			drv_TunerCallback(FALSE);
			AppRecivedLock = 0;
			printf("Illegal  Frequence: %d \n",  Frequence);
		}
		
	}
	return;
}

	

DRV_ErrCode DRV_SetNewTS ( HI_U32 Fre, HI_U32 Sym, HI_U8 Qam )
{
	if (Frequence < 50000 && Frequence > 900000)
	{
		printf("[DRV]DRV_SetNewTS  Frequence is error!\n");
		return FALSE;
	}
	
	if((g_TunerLock==HI_TRUE) && (Frequence ==  Fre))
	{
		return FALSE; //this return value is used by APP
	}

	g_TunerLock=HI_FALSE;

	DRV_OS_SemWait_MS(&semSetNewParam,DRVOS_WAIT_INFINITY,0);// we still need this protection?? BingdanShi
	Frequence = Fre;
	cQamSize = Qam;      
	iSymbolRate = Sym ;     
	iTransponderFreq = Fre ; 
	DRV_OSSemSignal(&semSetNewParam);

	/*如果之前的锁频状态为锁住，需要把状态设置为邋Unlock*/
	AppRecivedLock = 0;
	isFirst = 1;
	
	DRV_OS_SemWait_MS(&semNewTS,DRVOS_WAIT_NO,0);
	DRV_OSSemSignal(&semNewTS);	
	//SetTuner(Frequence);
	return TRUE;
}

DRV_ErrCode DVBTunerInit( void)
{ 
	DRV_ErrCode ErrCode;
    HI_U8   chipid = 0x00;
	HI_U32  GpioRegValue;

    DRV_OSSemInit(&semNewTS,0);
    DRV_OSSemInit(&semSetNewParam,1);
    DRV_OSSemInit(&mutexI2C,1);
    DRV_OSSemInit(&mutexI2Ctunersignal,1);

	ErrCode = HI_UNF_I2C_Open();
	if(HI_SUCCESS != ErrCode)
	{
		printf("[DRV]DVBTunerInit HI_UNF_I2C_Open ErrCode = %d\n",ErrCode);
	}
	//设置IO口复用，设置成I2C0
	ErrCode = HI_SYS_ReadRegister((0x10203000 + 0x0144), &GpioRegValue); 
	if(HI_SUCCESS != ErrCode)
	{
		printf("[DRV]DVBTunerInit HI_SYS_ReadRegister ErrCode = %x\n",ErrCode);
	}
    GpioRegValue &= 0xFFFFFFFC;
    GpioRegValue |= 0x00000002;
    ErrCode = HI_SYS_WriteRegister((0x10203000 + 0x0144), GpioRegValue); 
	if(HI_SUCCESS != ErrCode)
	{
		printf("[DRV]DVBTunerInit HI_SYS_WriteRegister ErrCode = %x\n",ErrCode);
	}	    

	ATBMPowerOnInit();
	ATBMSetDTMBMode();
    chipid = ATBMChipID();
    TUNER_DEBUG("\nGet chipID is [%x]\n",chipid);
    TUNER_DEBUG("\nDVBTunerInit Succeed!!\n");
    pTask=DRV_OS_TaskCreate("ATBM8830_task", (void*)ATBM8830_task, 10, 1024, pvParam, 0);
	
	mTunerCallback = (DRV_TunerCallback)drv_TunerCallback;
    return HI_SUCCESS;
}

int SetTuner(HI_U32 Fre)
{
    HI_U8 TunerReg[5],i;
    HI_U8 TunerAddr;
    int  ErrCode;
    int  temp;
	HI_U8 CP, R, BS,AGST;

#if (TUNER_SHARP_SELECT==1)
    {
        if (Fre<50000 || Fre>858000)
        Fre = 0; // input parameter test
	
        TunerReg[0] = ( (36167+Fre)*6/1000) >> 8;
        TunerReg[1] = ( (36167+Fre)*6/1000) & 0x00ff;

        if (Fre >= 50000 && Fre <100000)  TunerReg[3] = 0x51;
        else if (Fre >=100000 && Fre <122000) TunerReg[3] = 0x71;
        else if (Fre >=122000 && Fre <129000) TunerReg[3] = 0x91;
        else if (Fre >=129000 && Fre <136000) TunerReg[3] = 0xb1;
        else if (Fre >=136000 && Fre <147000) TunerReg[3] = 0xd1;
        else if (Fre >=147000 && Fre <240000) TunerReg[3] = 0x32;
        else if (Fre >=240000 && Fre <310000) TunerReg[3] = 0x52;
        else if (Fre >=310000 && Fre <380000) TunerReg[3] = 0x72;
        else if (Fre >=380000 && Fre <430000) TunerReg[3] = 0x92;
        else if (Fre >=430000 && Fre <578000) TunerReg[3] = 0x94;
        else if (Fre >=578000 && Fre <650000) TunerReg[3] = 0xb4;
        else if (Fre >=650000 && Fre <746000) TunerReg[3] = 0xd4;
        else if (Fre >=746000 && Fre <=858000) TunerReg[3] = 0xf4;
        else printf("Illegal Frequence\n");
	
        TunerReg[2] = 0xca;
        TunerReg[4] = 0x81; // 0x88 for better performance in search mode
                        // 121dBuB TOP for better performance. BingdanShi 20080602
        TunerAddr = 0xc0;
    }
#else   
    switch(TunerType)
    {
        case Tuner_TDAC7:/**/
        {
#ifndef TUNER_QUALITY_TEST
            if( (47000 > Fre) || (870000 < Fre ) )
        	{
        		TUNER_DEBUG("Input Freq Err , FREQ is %d\n", Fre);
        		return HI_FAILURE;
}
#endif	
            temp = (Fre +36000)*6/1000;//modified by wrwei
            TunerReg[0]=(temp&0x7f00)>>8;
            TunerReg[1]=temp&0xff;
    		
        	if (Fre >= 47000 && Fre < 125000)
        	{
        		CP = 0x02;//// only CP1 and CP0
        		BS = 0x04;
        	}
        	else if (Fre >= 125000 && Fre < 230000)
        	{
        		CP = 0x02;//// only CP1 and CP0
        		BS = 0x06;
        	}
        	else if (Fre >= 230000 && Fre < 366000)
        	{
        		CP = 0x03;
        		BS = 0x06;
        	}
        	else if (Fre >= 366000 && Fre < 602000)
        	{
        		CP = 0x02;
        		BS = 0x0E;
        	}
        	else if (Fre >= 602000 && Fre < 846000)
        	{
        		CP = 0x03;
        		BS = 0x0E;
        	}
        	else if (Fre >= 846000 && Fre < 870000)
        	{
        		CP = 0x00;
        		BS = 0x0E;
        	}
        	else
        	{
        		CP = 0x02;
        		BS = 0x0E; //with filter bandwidth setting
        	}
        	AGST = 0x05;
        	R = 0x00; // 166.67kHz
        //==========Evaluated to control byte=============
        	TunerReg[2] = (HI_U8)((0x80)|(AGST << 3)|R);
        	TunerReg[3] = (HI_U8)((0x00)|(CP << 6)|BS);
        	TunerReg[4] = (HI_U8)(0xC0);

            TunerAddr = 0xc0;

        	TUNER_DEBUG("------tuner freq =  %d\n",Fre);
        	TUNER_DEBUG("------tuner i2c data  %x  %x  %x  %x  %x\n",TunerReg[0],TunerReg[1],TunerReg[2],TunerReg[3],TunerReg[4]);
            break;
        }
        default:
            break;
    }
#endif
    i = 0;
    do
    {
		usleep(50);
        DRV_OS_SemWait_MS(&mutexI2Ctunersignal,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei
        ATBMI2CByPassOn();
		usleep(10);
		ErrCode = I2CWrite(TunerAddr, TunerReg, 1, 4);
		usleep(50);
        ATBMI2CByPassOff();
        DRV_OSSemSignal(&mutexI2Ctunersignal);
        i++;
    } while ( ErrCode != ATBM_I2CREADWRITE_OK && i <= 3 ); // try 3 times max.

    if ( ErrCode != ATBM_I2CREADWRITE_OK )
	{ 
		printf("[DRV]SetTuner Freq is error!\n");
        return 0;
    }
    
    TUNER_DEBUG("TunerData = [ 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x ]\n", 
            TunerReg[0],TunerReg[1],TunerReg[2],TunerReg[3],TunerReg[4]);

    //DRV_OS_SemWait_MS(&mutexI2Ctunersignal,DRVOS_WAIT_INFINITY,0);// we still need this protection add by wrwei
    //ATBMI2CByPassOn();
	//usleep(10);

	//ErrCode = I2CRead((TunerAddr+1), TunerReg, 1);
	//usleep(50 );
    //ATBMI2CByPassOff();
    //DRV_OSSemSignal(&mutexI2Ctunersignal);
    //TUNER_DEBUG("Read from tuner: 0x%x\n",TunerReg[0]);

    return 1;

}


DRV_ErrCode GetSignalQuality( HI_U32 * pCN_Ratio, HI_U32 * pBitErrRate, HI_U32* pCN_dB100)
{ 
    HI_U16 power,BER;
    HI_U32 CNR;

    if (g_TunerLock == HI_TRUE)
    {
	    DRV_OS_SemWait_MS(&mutexI2Ctunersignal,DRVOS_WAIT_INFINITY,0);  
        BER = ATBMFrameErrorRatio(); 
        power = ATBMSignalStrength();///2 -3;
        CNR = ATBMSignalNoiseRatio();
        DRV_OSSemSignal(&mutexI2Ctunersignal);        
	
        *pCN_dB100 = (HI_U32)power;
        *pCN_Ratio = (HI_U32)CNR;
        *pBitErrRate = (float)BER;
        TUNER_DEBUG("Power: %d, C/N: %d, BER: %d \n", *pCN_dB100, *pCN_Ratio, *pBitErrRate);
	}
    else
    {
        *pBitErrRate=1;
        *pCN_Ratio = 0;
        *pCN_dB100=0;
    }

	return HI_SUCCESS;
}


//-----------------old functions

void SignalDiagnose(void)
{
	return;
}

void SignalDiagnoseStop(void)
{
	return;
}
	
void DRV_CurrentTuner(HI_U32  *pFre, HI_U32 *pSym,  HI_U8 *pQam)
{
	*pFre = Frequence;
	*pSym = 6875;
	*pQam = 3;

	return;
}

/*******************************************************************************
名称:Tuner_Reset
功能:Tuner_Reset
参数:
返回: 无
*******************************************************************************/
void Tuner_Reset(void)
{
	ATBMReset(custom_config.ui8CrystalOrOscillator);

	return;
}

BOOL_YS DRV_TunerGetLockStatus(void)
{
	return HI_TRUE;
}
