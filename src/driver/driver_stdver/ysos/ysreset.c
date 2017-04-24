/*****************************************************************************
*
* Description: 
*    驱动层OS模块头文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2004-03-16    Yzb            创建
*
*****************************************************************************/
#include <stddefs.h>
#include <sttbx.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "hi_unf_ecs.h"
#include "drv_os.h"
#include "ysostime.h"

U8 gStandbyDisp = 0;   //待机后显示模式控制全局变量；0:不显示；1:显示时间 ; 2:显示节目频道(暂时没有实现)  wrwei 20130114

void DRV_Standby_DISP(U8 Flag)
{
	gStandbyDisp = Flag;

	return;
}

DRV_ErrCode RebootSTB(void)
{
	U32 Value;
    DRV_ErrCode errCode;
	printf("\n Reboot STB...\n");
	DRV_OS_TaskLock();
	DRV_OS_InterruptLock();
    /* Open WDG*/
    errCode = HI_UNF_WDG_Open();
    if (errCode != DRV_NO_ERROR)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    /* Set WDG TimeOut */
    Value = 1000;    //ms
    errCode = HI_UNF_WDG_SetTimeout(Value);
    if (errCode != DRV_NO_ERROR)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    /* Enable WDG */
    errCode = HI_UNF_WDG_Enable();
    if (errCode != DRV_NO_ERROR)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

	/* Waiting for data flush to flash, Especially for EMMC flash! */
	system("sync");
    DRV_OS_TaskDelay_MS(1000);

	while(1)
	{
		sleep(1);
	}
}



/*
when you want use your x5hdtest.bin, please re-define USR_BIN_SWITCH to 1. 
Also don't forget put x5hdtest.bin to this path: "SDK\sample\pmoc"    
*/
#define USR_BIN_SWITCH  0

DRV_ErrCode  DRV_RealStandBy()
{
#if 1
	HI_S32  i;
	HI_S32  ret;
	HI_U32  tmp = 0;
	time_t  time0;
	struct tm *time1;
	HI_UNF_PMOC_MODE_E  mode;
	HI_UNF_PMOC_WKUP_S  wakeup = {0}; 
	HI_UNF_PMOC_DEV_TYPE_S devType = {0};
	HI_UNF_PMOC_ACTUAL_WKUP_E  actual ;  
	HI_UNF_PMOC_STANDBY_MODE_S  standbyMode = {0};
#if USR_BIN_SWITCH	
	FILE *fb;
	HI_U32 len;
	HI_U8 *ptr;
#endif
	printf("------DRV_RealStandBy enter\n");
    //DRV_PanelTerm();
	//printf("there are three pmoc scene: 0 standard / 1 with eth forward/ 2 with phone \n");
	//printf("now only support standart scene\n");
	// 1
	//printf("please select dev type, by actual instance: ir & keyled type \n");
	memset(&devType, 0, sizeof(HI_UNF_PMOC_DEV_TYPE_S));
	// 1.0
	//printf("please select ir type (now only support 0 , 4): \n");
	//printf("0 -- nec simaple \n");
	//printf("1 -- tc9012 \n");
	//printf("2 -- nec full \n");
	//printf("3 -- sony_12bit \n");
	//printf("4 -- raw \n");
	//scanf("%u", &tmp);
	tmp = 4;
	if(tmp == 4)
	{
		devType.irtype = HI_UNF_IR_CODE_RAW;
	}
	else 
	{
		devType.irtype = HI_UNF_IR_CODE_NEC_SIMPLE;
	}
	// 1.1
	//printf("please select keyled type : \n");
	//printf("0 -- std \n");
	//printf("1 -- 6961 \n");
	//scanf("%u", &tmp);
	
#if (defined(CHIP_HI3716C) || defined(BIGUA))
	tmp = 0;
#else
    tmp = 1;
#endif
	if(tmp == 1)
	{
		devType.kltype = HI_UNF_KEYLED_TYPE_CT1642;
	}
	else 
	{
		devType.kltype = HI_UNF_KEYLED_TYPE_STD;
	}

	// 2
	memset(&wakeup, 0, sizeof(HI_UNF_PMOC_WKUP_S));
	// 2.0
	//printf("do you want to wakeup by time (0 yes/1 no): \n");
	//scanf("%u", &tmp);
	tmp = 1;
	if(tmp == 0 )
	{
		printf("timeval: \n");
		scanf("%u", &tmp);
		wakeup.u32WakeUpTime = tmp;
	}
	else
	{
		wakeup.u32WakeUpTime = -1;
	}
	// 2.1
	if(devType.irtype == HI_UNF_IR_CODE_RAW)
	{
		wakeup.u32IrPmocNum = 3;
		wakeup.u32IrPowerKey0[0] = 0xf50a5da0;//九州
		wakeup.u32IrPowerKey1[0] = 0x0;
        wakeup.u32IrPowerKey0[1] = 0x23dcfd01;//菏泽1
		wakeup.u32IrPowerKey1[1] = 0x0;
        wakeup.u32IrPowerKey0[2] = 0x23dc0820;//菏泽2
		wakeup.u32IrPowerKey1[2] = 0x0;
	}
	else 
	{
		wakeup.u32IrPmocNum = 1;
		wakeup.u32IrPowerKey0[0] = 0xf50a5da0;//九州
		wakeup.u32IrPowerKey1[0] = 0x0;		
	}
	// 2.2
	if(devType.kltype == HI_UNF_KEYLED_TYPE_PT6961)
	{
		wakeup.u32KeypadPowerKey = 0x8;
	}
    else if(devType.kltype == HI_UNF_KEYLED_TYPE_CT1642)
    {
        wakeup.u32KeypadPowerKey = 0xff; //CT1642前面板无待机唤醒键在此不设置
    }
	else
	{
		wakeup.u32KeypadPowerKey = 0x0;
	}

	// 3
	memset(&standbyMode, 0, sizeof(HI_UNF_PMOC_STANDBY_MODE_S));
	//printf("input standby mode (0 nodisplay /1 chan display /2 timer display): \n");
	//scanf("%u", &tmp);

	tmp = 0;
	/* 待机后前面板显示情况 */
	switch(gStandbyDisp)
	{
		case 0:
			tmp = 0;
			break;
		case 1:
			tmp = 2;
			break;
		case 2:
			tmp = 1;
			break;
		case 3:
			tmp = 3;
			break;	
		default:
			tmp = 0;
			printf("[DRV][%s]standby mode error!\n",__func__);	
	}
    
	standbyMode.u32Mode = tmp;
	if(tmp == 1)
    {
		printf("chan num: (0 <= channum <= 9999)\n");
		scanf("%u", &tmp);
		standbyMode.u32DispCode = tmp;
	}
    else if(tmp == 2)
    {
        StbTm_t         *pstTime;
        StbTime_t       siTimeNow;
         
        siTimeNow = GetCurrentTime();
        pstTime = STBLocalTime(&siTimeNow);
    
		standbyMode.stTimeInfo.u32Hour   = pstTime->Hour;
		standbyMode.stTimeInfo.u32Minute = pstTime->Minute;
	}

	// 4
	//printf("how many count do you want to run: \n");
	//scanf("%u", &tmp);
    tmp = 1;
	// 5  main
	ret = HI_UNF_PMOC_Init();
	if (HI_SUCCESS != ret)
	{
		printf("HI_UNF_PMOC_Init err, 0x%08x !\n", ret);
		return ret;
	}
#if USR_BIN_SWITCH
	fb = fopen("./x5hdtest.bin","r");
	if(fb <= 0)
	{
		printf("open file x5hdtest.bin fail \n");
		return -1;
	}

	fseek(fb,0,SEEK_END);
	len = ftell(fb);
	fseek(fb,0,SEEK_SET);
	printf("c51 code len = %d \n",len);
	ptr = malloc(len);
	if(ptr == NULL)
	{
		printf("malloc  fail \n");
		return -1;
	}
	len = fread(ptr,1,len,fb);
	printf("read len = %d \n",len);
	
	ret = HI_UNF_PMOC_LoadMcuCode(ptr, len);
	if (HI_SUCCESS != ret)
	{
		printf("HI_UNF_PMOC_LoadMcuCode err, 0x%08x !\n", ret);
		HI_UNF_PMOC_DeInit();
		return ret;
	}
	close(fb);	
#endif	
	ret = HI_UNF_PMOC_SetDevType(&devType);
	if (HI_SUCCESS != ret)
	{
		printf("HI_UNF_PMOC_SetDevType err, 0x%08x !\n", ret);
		HI_UNF_PMOC_DeInit();
		return ret;
	}

	ret = HI_UNF_PMOC_SetScene(HI_UNF_PMOC_SCENE_STANDARD);
	if (HI_SUCCESS != ret)
	{
		printf("HI_UNF_PMOC_SetScene err, 0x%08x !\n", ret);
		HI_UNF_PMOC_DeInit();
		return ret;
	}

        /* 在待机前关闭led */
	DRV_LED_DisplayNULL();

	for(i = 0; i < tmp; i++)
	{
		ret = HI_UNF_PMOC_SetWakeUpAttr(&wakeup);
		if (HI_SUCCESS != ret)
		{
			printf("HI_UNF_PMOC_SetWakeUpAttr err, 0x%08x !\n", ret);
			HI_UNF_PMOC_DeInit();
			return ret;
		}

		ret = HI_UNF_PMOC_SetStandbyDispMode(&standbyMode);
		if (HI_SUCCESS != ret)
		{
			printf("HI_UNF_PMOC_SetStandbyDispMode err, 0x%08x !\n", ret);
			HI_UNF_PMOC_DeInit();
			return ret;
		}

		system("rmmod ehci-hcd.ko;rmmod ohci-hcd.ko");   //uninstall usb driver
		mode = HI_UNF_PMOC_MODE_SLOW;
		ret = HI_UNF_PMOC_SwitchSystemMode(mode, &actual);
		if (HI_SUCCESS != ret)
		{
			printf("HI_UNF_PMOC_SwitchSystemMode err, 0x%08x !\n", ret);
			HI_UNF_PMOC_DeInit();
			return ret;
		}
		system("insmod /kmod/usb/ehci-hcd.ko;insmod /kmod/usb/ohci-hcd.ko");  //install usb driver
		
		if(actual == HI_UNF_PMOC_WKUP_IR){
			printf("wakeup by ir !!!\n");
		}else if(actual == HI_UNF_PMOC_WKUP_KEYLED){
			printf("wakeup by key !!!\n");
		}else if(actual == HI_UNF_PMOC_WKUP_TIMEOUT){
			printf("wakeup by timeout !!!\n");
		}else{
			printf("wakeup by no mode ??? \n");
		}

		printf("%%%%%%%% cnt = %d %%%%%%%% \n", i);
	}
	HI_UNF_PMOC_DeInit();
	printf("------DRV_RealStandBy out\n");
	DRV_OS_TaskDelay_MS(100);//延时时间不能太短，否则会导致DAC视频输出
	RebootSTB();
	DRV_PanelInit(0);
    return HI_SUCCESS;
#else
exit(0);
#endif
}






