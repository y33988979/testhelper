/*****************************************************************************
*
* Description: 
*    驱动层面板模块实现文件
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2004-03-17    yzb            创建
* 2. 2005-10-12    Qiujordan      添加错误码定义
* 3. 2005-12-27    Qiujordan      添加回调函数，驱动层和应用层分离
*
*****************************************************************************/
#define CURRENT_MODULE DRV_MODULE_PANEL

/* for file operation fuction */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <hi_unf_ecs.h>
#include "drv_os.h"
#include "drv_basic.h"
#include "yspanel.h"
#include "drv_debug.h"
#include <sys/time.h>
#include <unistd.h>

/**************** IR RECEIVE CONTROL***************/
//#define TEST_IR_VALUE  //测试未知遥控器键值，打开

//#define IR_RECEIVE_FLATG  //需要收到遥控器键值后灯闪，打开

/****************************************/
struct timeval  tv;

HI_UNF_KEYLED_TIME_S stLedTime;

BOOL mKeyboardLocked = FALSE;
static HI_BOOL mIrTaskRunning = HI_FALSE;
static HI_BOOL mLedDisplayTaskRunning = HI_FALSE;
static HI_BOOL mLedUpdate = HI_FALSE;
static HI_BOOL mLedDisplayTime = HI_FALSE;
static HI_U8   IrReceive;

const HI_CHAR IR_name[4][16] =
{
    "nec sample",
    "tc9012",
    "nec full",
    "sony"
};

const HI_CHAR IR_Stuts_Str[3][10] =
{
    "DOWN",
    "HOLD",
    "UP",
};
#define LED_DELAY_TIME 100000
/*
*    数字0-9
*    0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09
*    大写26个字母
*    0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,
*    0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
*    小写26个字母
*    0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,
*    0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
*/
HI_U8 mLedDisplayCode[64] = {0x03, 0x9F, 0x25, 0x0d, 0x99, 0x49, 0x41, 0x1f, 0x01, 0x09,
    0xFF, 0xFF, 0x63, 0xFF, 0x61, 0x71, 0xFF, 0x91, 0xFF, 0xFF, 0xFF, 0xE3, 0xFF,
    0xFF, 0x03, 0x31, 0xFF, 0xFF, 0x49, 0xFF, 0x83, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xC1, 0xE5, 0x85, 0x21, 0xFF, 0x09, 0xD1, 0xDF, 0xFF, 0xFF, 0x9F, 0xFF,
    0xD5, 0xC5, 0x31, 0x19, 0xFF, 0x49, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
  
static HI_U8 mLedData[4];
static HI_U8 mLedDataTemp[4];
static void PanelDefaultCallback(U8 Key)
{
    switch(Key)
    {
         case STANDBY_KEY:
            printf("STANDBY_KEY");
            break;
        case MENU_CUM_EXIT_KEY:
            printf("MENU_CUM_EXIT_KEY_APP");
            break;
        case VOL_DOWN_CUM_LEFT_ARROW_KEY:
            printf("VOL_DOWN_CUM_LEFT_ARROW_KEY");
            break;
        case VOL_UP_CUM_RIGHT_ARROW_KEY:
            printf("VOL_UP_CUM_RIGHT_ARROW_KEY");
            break;
        case PROG_PREV_CUM_UP_ARROW_KEY:
            printf("PROG_PREV_CUM_UP_ARROW_KEY");
            break;
        case PROG_NEXT_CUM_DOWN_ARROW_KEY:
            printf("PROG_NEXT_CUM_DOWN_ARROW_KEY");
            break;
        case POWER_CUM_SELECT_KEY:
            printf("POWER_CUM_SELECT_KEY");
            break;
        case SELECT_KEY:
            printf("SELECT_KEY");
            break;
        default:
            printf("Unrecongnised Key[0x%x:%d]",Key,Key);
            break;
    }
    printf("\n");
    return;
}
static HI_U64 RemoteCustomerCodeMODE0JIUZHOU = 0x5da0;
static U8 mRemoteKeyValueBeforeMODE0JIUZHOU[64] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x11,0x12,0x13,0x14,0x42,0x43,\
	                                    0x0b,0x1c,0x1e,0x41,0x10,0xff,0xff,0x15,0x1f,0x40,0x0a,0xff,0xff,0x18,0xff,0xff,\
	                                    0xff,0x19,0x1b,0x44,0xff,0x0e,0xee,0x17,0x16,0xff,0x1d,0x48,0x0f,0xff,0xff,0x0d,\
	                                    0x4f,0x47,0x4b,0x0c,0x1a,0x46,0x4e,0x4a,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff\
                                       };
static HI_U64 RemoteCustomerCodeMODE0WASU = 0x7748;
static U8 mRemoteKeyValueBeforeMODE0WASU[64] = { 0xef10,0xee11,0xed12,0xec13,0xeb14,0xea15,0xe916,0xe817,0xe718,0xe619,0xff00,0xfe01,0xfc03,0xfd02,0xfb04,0xf708,\
	                                    0xf30c,0xf00f,0xf906,0xf40b,0xe31c,0xffff,0xffff,0xe01f,0xf807,0xe21d,0xf50a,0xffff,0xffff,0xe51a,0xffff,0xffff,\
	                                    0xc23d,0xd42b,0xd52a,0xffff,0xffff,0xffff,0xffff,0xfa05,0xf609,0xffff,0xd02f,0xffff,0xe41b,0xffff,0xffff,0xffff,\
	                                    0xf10e,0xe11e,0xb34c,0xbb44,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff\
                                       };


static HI_U64 RemoteCustomerCodeMODE0HEZE = 0x820;
static HI_U64 RemoteCustomerCodeMODE0HEZE2 = 0xfd01;
static U8 mRemoteKeyValueBeforeMODE0HEZE[64] = {0x87,0x92,0x93,0xcc,0x8e,0x8f,0xc8,0x8a,0x8b,0xc4,0xca,0xd2,0x99,0xc1,0xdd,0x8c,\
	                                    0x9c,0xff,0xff,0xff,0x95,0xff,0xff,0xce,0xd6,0xc5,0xdc,0xc9,0xff,0xff,0xcd,0x82,\
	                                    0xff,0xff,0xc0,0xff,0xff,0x85,0xff,0x98,0x4f,0xd9,0x0c,0xc7,0xff,0xff,0xff,0xff,\
	                                    0xda,0xd8,0x83,0xc6,0xff,0xff,0xff,0xff,0xff,0xff,0x88,0xd0,0xff,0xff,0xff,0xff\
                                       };
static U8 mRemoteKeyValueAfter[64] = { 0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,\
	                                    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,\
	                                    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,\
	                                    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F\
                                       };
static U8 *mRemoteKeyValueBefore0 = 0;
static U8 *mRemoteKeyValueBeforeTemp0 = 0;	

static DRV_PanelCallback fPanelCallback = PanelDefaultCallback;

void * drv_IrReceiveTask(void *args)
{
    HI_S32 ErrCode;
    HI_U64 u64KeyId;
    HI_U64 u64KeyCompId;
    HI_U8  u8KeyValue[8];
    HI_U8  index;
	static HI_U8  lastKey2bStored,key2bStored;
	static HI_U32  repeatKey;
    HI_UNF_KEY_STATUS_E PressStatus;
	HI_UNF_IR_PROTOCOL_E Protocol;

    while (mIrTaskRunning)
    {
        /*get ir press codevalue & press status*/
        ErrCode = HI_UNF_IR_GetValue(&PressStatus, &u64KeyId, 200);
        if (HI_SUCCESS == ErrCode)
        {       	
#ifdef TEST_IR_VALUE 
            ErrCode = HI_UNF_IR_GetProtocol(&Protocol);    	
	        if (HI_SUCCESS == ErrCode)
	        {
	            printf("IR   Protocol : %d \t", Protocol);
	        }
            printf("IR   KeyId : 0x%llx   PressStatus :%d[%s]\n", u64KeyId, PressStatus
                ,IR_Stuts_Str[PressStatus]);
#endif

#ifdef IR_RECEIVE_FLATG
            //IrReceive = 3;
            HI_UNF_LED_Display(mLedData[3] << 24 | (mLedData[2]&0xfe) << 16
                                        | mLedData[1] << 8 | mLedData[0]);
            usleep(24000);
            HI_UNF_LED_Display(mLedData[3] << 24 | (mLedData[2]|0x01) << 16
                                        | mLedData[1] << 8 | mLedData[0]);  
#endif
	        if(PressStatus == HI_UNF_KEY_STATUS_DOWN)
	        {
		        if((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0JIUZHOU)
		        {
					memcpy(u8KeyValue,(HI_U8*)(&u64KeyId),8);
					mRemoteKeyValueBefore0 = mRemoteKeyValueBeforeMODE0JIUZHOU;
					/*转换为标准键值*/
					mRemoteKeyValueBeforeTemp0 = mRemoteKeyValueBefore0;
	                for (index = 0; index < 64; index++)
	                {
	                    if(*mRemoteKeyValueBeforeTemp0 == u8KeyValue[2])
	                    {
	                        key2bStored = mRemoteKeyValueAfter[index];
							lastKey2bStored = key2bStored; 
	               		    if( mKeyboardLocked == FALSE)
	               		    {
								if(fPanelCallback != NULL)
									fPanelCallback(key2bStored);
			    		    }   
		                       
					       	break;                  			  
	                    }
						mRemoteKeyValueBeforeTemp0++;
	                }
				}	
				else if((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0WASU)
		        {
					memcpy(u8KeyValue,(HI_U8*)(&u64KeyId),8);
					mRemoteKeyValueBefore0 = mRemoteKeyValueBeforeMODE0WASU;
					/*转换为标准键值*/
					mRemoteKeyValueBeforeTemp0 = mRemoteKeyValueBefore0;
	                for (index = 0; index < 64; index++)
	                {
	                    if(*mRemoteKeyValueBeforeTemp0 == u8KeyValue[2])
	                    {
	                        key2bStored = mRemoteKeyValueAfter[index];
							lastKey2bStored = key2bStored; 
	               		    if( mKeyboardLocked == FALSE)
	               		    {
								if(fPanelCallback != NULL)
									fPanelCallback(key2bStored);
			    		    }   
		                       
					       	break;                  			  
	                    }
						mRemoteKeyValueBeforeTemp0++;
	                }
				}	
                else if(((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0HEZE)||((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0HEZE2))
		        {
					memcpy(u8KeyValue,(HI_U8*)(&u64KeyId),8);
                    //Err_Print("遥控器 键值 %x \n",u8KeyValue[2]);
					mRemoteKeyValueBefore0 = mRemoteKeyValueBeforeMODE0HEZE;
					/*转换为标准键值*/
					mRemoteKeyValueBeforeTemp0 = mRemoteKeyValueBefore0;
	                for (index = 0; index < 64; index++)
	                {
	                    if(*mRemoteKeyValueBeforeTemp0 == u8KeyValue[2])
	                    {
	                        key2bStored = mRemoteKeyValueAfter[index];
							lastKey2bStored = key2bStored; 
	               		    if( mKeyboardLocked == FALSE)
	               		    {
								if(fPanelCallback != NULL)
									fPanelCallback(key2bStored);
			    		    }   
		                       
					       	break;                  			  
	                    }
						mRemoteKeyValueBeforeTemp0++;
	                }
				}
                repeatKey = 0;
			}
	        else if((PressStatus == HI_UNF_KEY_STATUS_HOLD)&&(
				((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0JIUZHOU)||
				((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0HEZE)||
				((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0HEZE2)||
				((u64KeyId&0xFFFF) == RemoteCustomerCodeMODE0WASU)))
	        {
		    	if((lastKey2bStored == RKEY_LEFT) 
			    || (lastKey2bStored == RKEY_RIGHT))
		        {
		            repeatKey++;
		            if((repeatKey%2 ==0)&&(repeatKey > 9))   
		            {
               		    if( mKeyboardLocked == FALSE)
               		    {
							if(fPanelCallback != NULL)
								fPanelCallback(key2bStored | REMOTE_KEY_REPEATED);
		    		    }   
		            }
		        }
		        else if((lastKey2bStored == RKEY_VOL_INC) 
					 || (lastKey2bStored == RKEY_VOL_DEC))
		        {
		            repeatKey++;
		            if((repeatKey%2 ==0)&&(repeatKey > 9))   
		            {
               		    if( mKeyboardLocked == FALSE)
               		    {
							if(fPanelCallback != NULL)
								fPanelCallback(key2bStored);
		    		    }   
		            }
		        }               
		        else if((lastKey2bStored == RKEY_UP)
		             || (lastKey2bStored == RKEY_DOWN) 
		             || (lastKey2bStored == RKEY_PAGE_UP)
		             || (lastKey2bStored == RKEY_PAGE_DOWN))
		        {
		            repeatKey++;
		            if((repeatKey%2 ==0)&&(repeatKey > 9))   
		            {
               		    if( mKeyboardLocked == FALSE)
               		    {
							if(fPanelCallback != NULL)
								fPanelCallback(key2bStored | REMOTE_KEY_REPEATED);
		    		    }   
		            }
		        }
		        else if((lastKey2bStored == RKEY_CHNL_INC)
		             || (lastKey2bStored == RKEY_CHNL_DEC))
		        {
		            repeatKey++;
		            if((repeatKey%2 ==0)&&(repeatKey > 9))
		            {
               		    if( mKeyboardLocked == FALSE)
               		    {
							if(fPanelCallback != NULL)
								fPanelCallback(key2bStored);
		    		    }   
		            }
		        } 
					
			}		
			//printf("IR   KeyId : 0x%llx   PressStatus :%d[%s]\n", u64KeyId, PressStatus,IR_Stuts_Str[PressStatus]);
        }
    }
    return 0;
}


void * drv_LedDisplayTask(void *args)
{
    HI_U32 u32Loop = 0;
    HI_S32 errCode;

    while (mLedDisplayTaskRunning == TRUE)
    {
        if(mLedDisplayTime)
        {
            HI_UNF_LED_DisplayTime(stLedTime);
            usleep(LED_DELAY_TIME);
            continue;
        }
        
    	if(mLedUpdate)
        {
			//V300平台led显示数字顺序反了，调整显示顺序；wrwei 20130110  
			//modi start
            errCode = HI_UNF_LED_Display(mLedData[u32Loop+3] << 24 | mLedData[u32Loop+2] << 16
                                    | mLedData[u32Loop+1] << 8 | mLedData[u32Loop+0]);
            if (HI_SUCCESS != errCode)
            {
                printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
                break;
            }
            mLedUpdate = FALSE;
			//modi end
        }
        else
        {
            usleep(LED_DELAY_TIME);
        }
    }
    mLedDisplayTaskRunning = FALSE;
    return 0;
}

#define MENU_CUM_EXIT_KEY               				0x51    /* when SW1 & SW2 are pressed together */
#define VOL_DOWN_CUM_LEFT_ARROW_KEY   		0x55    /* volume- & left arrow key */
#define VOL_UP_CUM_RIGHT_ARROW_KEY      		0x53    /* volume+ & right arrow key */
#define PROG_PREV_CUM_UP_ARROW_KEY     		0x54    /* prog- & up arrow key */
#define PROG_NEXT_CUM_DOWN_ARROW_KEY    		0x52    /* prog+ & down arrow key */
#define POWER_CUM_SELECT_KEY            			0x56    /* power & select key */
#define SELECT_KEY                  					0x57        /* RSW22 or SW5 */ 
#define STANDBY_KEY                                                          0x59

#ifndef BIGUA
void * drv_KeyReceiveTask(void *args)
{
    HI_S32 errCode;
    HI_U32 u32PressStatus, u32KeyId;
	static HI_U8 KeyData;
	static HI_BOOL KeyValid;
    while (mLedDisplayTaskRunning == TRUE)
    {
        /*get KEY press value & press status*/
        errCode = HI_UNF_KEY_GetValue(&u32PressStatus, &u32KeyId);
        if (HI_SUCCESS == errCode)
        {
         	KeyValid = TRUE;
       		switch(u32KeyId)
        	{
        		case 5:
       			KeyData = MENU_CUM_EXIT_KEY;
        		break;
        		case 4:
         		KeyData = VOL_DOWN_CUM_LEFT_ARROW_KEY;
        		break;
       			case 2:
        		KeyData = VOL_UP_CUM_RIGHT_ARROW_KEY;	
        		break;
       			case 6:
        		KeyData = PROG_PREV_CUM_UP_ARROW_KEY;	
        		break;
        		case 3:
        		KeyData = PROG_NEXT_CUM_DOWN_ARROW_KEY;	
        		break;
        		case 7:
        		KeyData = SELECT_KEY;	
        		break;        		
        		case 0:
        		KeyData = POWER_CUM_SELECT_KEY;	
        		break;
        		default:
        		KeyValid = FALSE;
        		break;
        	}
        	if(((KeyValid == TRUE) && (mKeyboardLocked == FALSE)) &&
				(KeyData==VOL_DOWN_CUM_LEFT_ARROW_KEY)||
				(KeyData==VOL_UP_CUM_RIGHT_ARROW_KEY)||
				(KeyData==PROG_PREV_CUM_UP_ARROW_KEY)||
				(KeyData==PROG_NEXT_CUM_DOWN_ARROW_KEY))
        	{
	 			if(fPanelCallback != NULL)
					fPanelCallback(KeyData);
		}	
		else if((KeyValid == TRUE) && (mKeyboardLocked == FALSE) && (0 == u32PressStatus))
        	{
	 			if(fPanelCallback != NULL)
					fPanelCallback(KeyData);
			}	
           	//printf("KEY  KeyId : 0x%x    PressStatus :%d[%s]\n", u32KeyId, u32PressStatus
            //    , (0 == u32PressStatus) ? "DOWN" 
            //    : (1 == u32PressStatus) ? "HOLD" : "UP");            
        }
        else
        {
            usleep(50000);
        }
    }

    return 0;
}
#else
/* 电平拉低设置引脚 */
#define led_gpio_sata0_hcken 0x078
static U32 g_led_gpio_spi_armen;
#define GPIO_CH_UP (5*8+2)

#define led_gpio_spi_pcken 0x07c
static U32 g_led_gpio_spi_pcken;
#define GPIO_CH_DOWN (5*8+3)

/* 按键值引脚 */
#define key_gpio_pcie0 0x08c
static U32 g_key_gpio_pcie0;
#define GPIO_KEY1 (5*8+7)

#define LED_GPIO_IR 0x080
static U32 g_key_gpio;
#define GPIO_KEY2 (5*8+4)

void * drv_KeyReceiveTask(void *args)
{
    HI_S32 errCode = HI_SUCCESS;
    HI_U32 u32PressStatus, u32KeyId;
    static HI_U8 KeyData;
    static HI_BOOL KeyValid;
    HI_U32 p32GpioNo;
    HI_BOOL bHighVolt;
    HI_U32 timestart = 0;
    HI_U32 timeover = 0;
	static HI_S32 flag_key1;	//按键1长按上报一次的循环次数
	static HI_S32 flag_key2;	//按键2长按上报一次的循环次数
	static HI_S32 flag_key;	//按键短按上报一次的循环次数

    /* 保存复用寄存器值，并设置gpio模式 */
    errCode = HI_SYS_ReadRegister((0x10203000 + led_gpio_sata0_hcken), &g_led_gpio_spi_pcken);
    errCode |= HI_SYS_WriteRegister((0x10203000 + led_gpio_sata0_hcken), 0x00);
    errCode |= HI_SYS_ReadRegister((0x10203000 + led_gpio_spi_pcken), &g_led_gpio_spi_armen);
    errCode |= HI_SYS_WriteRegister((0x10203000 + led_gpio_spi_pcken), 0x00);
    errCode |= HI_SYS_ReadRegister((0x10203000 + LED_GPIO_IR), &g_key_gpio_pcie0);
    errCode |= HI_SYS_WriteRegister((0x10203000 + LED_GPIO_IR), 0x00);
    errCode |= HI_SYS_ReadRegister((0x10203000 + key_gpio_pcie0), &g_key_gpio);
    errCode |= HI_SYS_WriteRegister((0x10203000 + key_gpio_pcie0), 0x00);
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_SYS_WriteRegister ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

#if 0
    errCode = HI_UNF_GPIO_Open();
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_UNF_GPIO_Open ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
#endif

    /* 输出 电平拉低*/
    errCode = HI_UNF_GPIO_SetDirBit(GPIO_CH_UP, 0);
    errCode |= HI_UNF_GPIO_SetDirBit(GPIO_CH_DOWN, 0);
    errCode |= HI_UNF_GPIO_WriteBit(GPIO_CH_UP, 0);
    errCode |= HI_UNF_GPIO_WriteBit(GPIO_CH_DOWN, 0);
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_UNF_GPIO_WriteBit ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    
   /* 输入 */
    errCode = HI_UNF_GPIO_SetDirBit(GPIO_KEY1, 1);
    errCode |= HI_UNF_GPIO_SetDirBit(GPIO_KEY2, 1);
    errCode |= HI_UNF_GPIO_SetIntType(GPIO_KEY1, HI_UNF_GPIO_INTTYPE_DOWN);
    errCode |= HI_UNF_GPIO_SetIntType(GPIO_KEY2, HI_UNF_GPIO_INTTYPE_DOWN);
    errCode |= HI_UNF_GPIO_SetIntEnable(GPIO_KEY1, 1);
    errCode |= HI_UNF_GPIO_SetIntEnable(GPIO_KEY2, 1);
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_UNF_GPIO_WriteBit ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    
    while (mLedDisplayTaskRunning == TRUE)
    {
        u32PressStatus = 0;        
        u32KeyId = 0xffffffff;
		flag_key1=0;    
		flag_key2=0;    
		flag_key=0;
        errCode = HI_UNF_GPIO_QueryInt(&p32GpioNo, 0);
        if(HI_SUCCESS != errCode)
        {
            usleep(1*1000);
            continue;
        }
    	while(p32GpioNo!=0xffffffff)
        {
    		u32KeyId = 0xffffffff;
			u32PressStatus = 0;  
        if(GPIO_KEY1 == p32GpioNo)
        {
            HI_UNF_GPIO_ReadBit(GPIO_KEY1, &bHighVolt);
            if(0 == bHighVolt)
            {
					flag_key1++;
					flag_key2=0;
					flag_key++;
					if(flag_key>=20)
					{
						flag_key=20;
					}
					if(flag_key1==700)//长按一次按键
					{
						u32KeyId =3;	
						flag_key1=0; 
						flag_key=0;
                printf("debug : GPIO_KEY1 press \n");
					}
        		}
				else if(1 == bHighVolt&&20==flag_key)//短按一次按键
				{
                u32KeyId = 3;
					flag_key=0;
					printf("debug : GPIO_KEY1 press \n");
				}
				else
					break;
            }
			else if(GPIO_KEY2 == p32GpioNo)
        {
            HI_UNF_GPIO_ReadBit(GPIO_KEY2, &bHighVolt);
            if(0 == bHighVolt)
            {
					flag_key1=0;		
					flag_key2++;
					flag_key++;
					if(flag_key>=20)
					{
						flag_key=20;
					}
					if(flag_key2==700)
					{
						u32KeyId =6;	
						flag_key2=0;   
						flag_key=0;
                printf("debug : GPIO_KEY2 press \n");
					}
        		}
 				else if(1 == bHighVolt&&20==flag_key)
				{
                u32KeyId = 6;
					flag_key=0;
					printf("debug : GPIO_KEY2 press \n");
            }
				else
					break;
        }
        usleep(1000);
        	if (HI_SUCCESS == errCode&&u32KeyId!= 0xffffffff)
        {
         	KeyValid = TRUE;
       		switch(u32KeyId)
        	{
        		case 5:
       			KeyData = MENU_CUM_EXIT_KEY;
        		break;
        		case 4:
         		KeyData = VOL_DOWN_CUM_LEFT_ARROW_KEY;
        		break;
       			case 2:
        		KeyData = VOL_UP_CUM_RIGHT_ARROW_KEY;	
        		break;
       			case 6:
        		KeyData = PROG_PREV_CUM_UP_ARROW_KEY;	
        		break;
        		case 3:
        		KeyData = PROG_NEXT_CUM_DOWN_ARROW_KEY;	
        		break;
        		case 7:
        		KeyData = SELECT_KEY;	
        		break;        		
        		case 0:
        		KeyData = POWER_CUM_SELECT_KEY;	
        		break;
        		default:
        		KeyValid = FALSE;
        		break;
        	}
        	if((KeyValid == TRUE) && (mKeyboardLocked == FALSE) && (0 == u32PressStatus))
        	{
	 			if(fPanelCallback != NULL)
					fPanelCallback(KeyData);
			}
	           	printf("KEY  KeyId : 0x%x    PressStatus :%d[%s]\n", u32KeyId, u32PressStatus \
	                , (0 == u32PressStatus) ? "DOWN"              \
	                : (1 == u32PressStatus) ? "HOLD" : "UP");            
	        }
        }
    }

    HI_UNF_GPIO_Close();

    /* 回复复用寄存器 */
    errCode = HI_SYS_WriteRegister((0x10203000 + led_gpio_sata0_hcken), g_led_gpio_spi_pcken);
    errCode |= HI_SYS_WriteRegister((0x10203000 + led_gpio_spi_pcken), g_led_gpio_spi_armen);
    errCode |= HI_SYS_WriteRegister((0x10203000 + LED_GPIO_IR), g_key_gpio_pcie0);
    errCode |= HI_SYS_WriteRegister((0x10203000 + key_gpio_pcie0), g_key_gpio);
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_UNF_GPIO_WriteBit ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    return 0;
}

#endif

DRV_ErrCode DRV_PanelGetLeaderValue(U32 *Mode,U32 *LeaderValue)
{ 
	YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

DRV_ErrCode DRV_PanelSetRemoteParam(void)
{
	return DRV_NO_ERROR;
}
DRVOS_Task_t *pIrThread;
DRVOS_Task_t *pKeyTaskid;
DRVOS_Task_t *pLedTaskid;

DRV_ErrCode DRV_PanelInit(DRV_PanelType PanelType)
{
    DRV_ErrCode errCode;
    HI_UNF_IR_CODE_E ircode = HI_UNF_IR_CODE_NEC_SIMPLE;
    HI_UNF_KEYLED_TYPE_E keyled_type;

#if (defined(CHIP_HI3716MV300) || defined(CHIP_HI3716MV300TFADV) || defined(CHIP_HI3716MV300SMADV))
    keyled_type = HI_UNF_KEYLED_TYPE_CT1642;
#elif defined(CHIP_HI3716M) || defined(CHIP_HI3716C)
    keyled_type = HI_UNF_KEYLED_TYPE_STD;
#else
#error "not select chip type || select chip error !"
#endif

    printf("We use IR %s\n",IR_name[ircode]);
    
    /*open ir device*/
    errCode = HI_UNF_IR_Open();
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    errCode = HI_UNF_IR_SetCodeType(ircode);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        HI_UNF_IR_Close();
        return errCode;
    }
    
    errCode = HI_UNF_IR_EnableKeyUp(HI_TRUE);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        HI_UNF_IR_Close();
        return errCode;
    }

    errCode = HI_UNF_IR_SetRepKeyTimeoutAttr(108);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        HI_UNF_IR_Close();
        return errCode;
    }
    
    errCode = HI_UNF_IR_EnableRepKey(HI_TRUE);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        HI_UNF_IR_Close();
        return errCode;
    }

    errCode = HI_UNF_IR_Enable(HI_TRUE);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        HI_UNF_IR_Close();
        return errCode;
    }
    mIrTaskRunning = TRUE;
    /*create a thread for ir receive*/
	pIrThread = DRV_OS_TaskCreate("drv_IrReceiveTask",drv_IrReceiveTask,3,8096,NULL,0);
    if (pIrThread == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

#ifndef BIGUA
	/*KEYLED*/
    HI_UNF_KEYLED_Init();

    errCode = HI_UNF_KEYLED_SelectType(keyled_type);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    /*open LED device*/
    errCode = HI_UNF_LED_Open();
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    /*enable flash*/
    errCode = HI_UNF_LED_SetFlashFreq(HI_UNF_KEYLED_LEVEL_5);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    /*config LED flash or not*/
    errCode = HI_UNF_LED_SetFlashPin(HI_UNF_KEYLED_LIGHT_ALL);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
printf("setsetsetset HI_UNF_KEYLED_LIGHT_ALL^^^^^^^^!!\n");

    /*open KEY device*/
    errCode = HI_UNF_KEY_Open();
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    /*config keyup is valid*/
    errCode = HI_UNF_KEY_IsKeyUp(FALSE);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

    /*config keyhold is valid*/
    errCode = HI_UNF_KEY_IsRepKey(TRUE);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
#else
    /*
     * 3716MV300 因为ct1642编译到内核中，在壁挂的软件中需要
     * 把ct1642去初始化掉
     */    
	/*KEYLED*/
    HI_UNF_KEYLED_Init();
    errCode = HI_UNF_KEYLED_SelectType(keyled_type);
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    /*open KEY device*/
    errCode = HI_UNF_KEY_Open();
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    /*open LED device*/
    errCode = HI_UNF_LED_Open();
    if (HI_SUCCESS != errCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
    HI_UNF_LED_Close();
    HI_UNF_KEY_Close();
    HI_UNF_KEYLED_DeInit();

    errCode = HI_UNF_GPIO_Open();
    if(HI_SUCCESS != errCode)
    {
        printf("%s: %d  HI_UNF_GPIO_Open ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
#endif

    mLedDisplayTaskRunning = TRUE;

    /*create a thread for receive*/
	pKeyTaskid = DRV_OS_TaskCreate("drv_KeyReceiveTask",drv_KeyReceiveTask,3,8096,NULL,0);
    if (pKeyTaskid == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }

#ifndef BIGUA
    /*create a thread for led display*/
	pLedTaskid = DRV_OS_TaskCreate("drv_LedDisplayTask",drv_LedDisplayTask,3,8096,NULL,0);
    if (pLedTaskid == NULL)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, errCode);
        return errCode;
    }
#endif
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

DRV_ErrCode DRV_PanelTerm(void)
{
	DRV_ErrCode ErrCode;
	S32 result;
	mLedDisplayTaskRunning = FALSE;
	mIrTaskRunning = FALSE;
#ifndef BIGUA
	DRV_OS_TaskDelay_MS(1000);
    ErrCode = HI_UNF_KEY_Close();
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_LED_Close();
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
    ErrCode = HI_UNF_KEYLED_DeInit();
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
#endif
    ErrCode = HI_UNF_IR_Close();
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }

	while(1)
	{
		result = DRV_OS_TaskWait(pIrThread, DRVOS_WAIT_FINITY, 500); 
		if(result == 0)
			break;
		DRV_OS_TaskKill(pIrThread, 0, 0);
		DRV_OS_TaskDelay(DRV_OS_TimeGetTickPerSecond()/100); 
	}
	DRV_OSTaskDelete(pIrThread);
	while(1)
	{
		result = DRV_OS_TaskWait(pKeyTaskid, DRVOS_WAIT_FINITY, 500); 
		if(result == 0)
			break;
		DRV_OS_TaskKill(pKeyTaskid, 0, 0);
		DRV_OS_TaskDelay(DRV_OS_TimeGetTickPerSecond()/100); 
	}
	DRV_OSTaskDelete(pKeyTaskid);
#ifndef BIGUA
	while(1)
	{
		result = DRV_OS_TaskWait(pLedTaskid, DRVOS_WAIT_FINITY, 500); 
		if(result == 0)
			break;
		DRV_OS_TaskKill(pLedTaskid, 0, 0);
		DRV_OS_TaskDelay(DRV_OS_TimeGetTickPerSecond()/100); 
	}
	DRV_OSTaskDelete(pLedTaskid);
#endif
    return ErrCode;
}

DRV_ErrCode DRV_IR_SetRepKeyTimeout(HI_U32 TimeoutMs)
{
    DRV_ErrCode ErrCode;
    ErrCode = HI_UNF_IR_SetRepKeyTimeoutAttr(TimeoutMs);
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%08x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_IR_Close();
        return ErrCode;
    }

    return ErrCode;
}
DRV_ErrCode DRV_PanelEnableCallback(DRV_PanelCallback Callback_f)
{
    if(Callback_f == NULL)
        Callback_f = PanelDefaultCallback;
    fPanelCallback = Callback_f;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);
}

BOOL DRV_IsKeyboardLocked(void)
{
	return mKeyboardLocked;
}
void DRV_KeyboardLock(void)
{
	mKeyboardLocked = TRUE;
}

void DRV_KeyboardUnLock(void)
{
	mKeyboardLocked = FALSE;
}

static U8 drv_LedNumCovert(U8 uiNumber)
{
    U8 ledNumber;
    
    ledNumber = NUM_NULL;
    switch(uiNumber)
    {
        case 0:
        	ledNumber = NUM_0;
        	break;
        case 1:
        	ledNumber = NUM_1;
        	break;
        case 2:
        	ledNumber = NUM_2;
        	break;
        case 3:
        	ledNumber = NUM_3;
        	break;
        case 4:
        	ledNumber = NUM_4;
        	break;
        case 5:
        	ledNumber = NUM_5;
        	break;
        case 6:
        	ledNumber = NUM_6;
        	break;
        case 7:
        	ledNumber = NUM_7;
        	break;
        case 8:
        	ledNumber = NUM_8;
        	break;
        case 9:
        	ledNumber = NUM_9;
        	break;
        default:
        	break;
        
    }
    return ledNumber;

}

static void drv_LedDisplay(U8 ledType,U16 ledNumber)
{
    U8 ucbyte1,ucbyte2,ucbyte3,ucbyte4;
    U8 ucTemp1,ucTemp2,ucTemp3;
    ucbyte1 = CHAR_BIG_A;

    switch(ledType)
    {
        case 0:  
        	ucbyte1 = CHAR_BIG_P;
        case 1:  
        	ucTemp1 = (U8)(ledNumber / 100);
        	ucTemp2 = (U8)((ledNumber - ucTemp1*100) / 10);
        	ucTemp3 = (U8)(ledNumber % 10);
        	ucbyte2 = drv_LedNumCovert(ucTemp1);
        	ucbyte3 = drv_LedNumCovert(ucTemp2);
        	ucbyte4 = drv_LedNumCovert(ucTemp3);
        	break;
        case 2:  /* standby status */
        	ucbyte1 = STANDBY_STATUS_LINE;
        	ucbyte2 = STANDBY_STATUS_LINE;
        	ucbyte3 = STANDBY_STATUS_LINE;
        	ucbyte4 = STANDBY_STATUS_LINE;
        	break;
         case 3:
        	ucbyte1 = mLedDataTemp[0];
        	ucbyte2 = mLedDataTemp[1];
        	ucbyte3 = mLedDataTemp[2];
        	ucbyte4 = mLedDataTemp[3];
            break;
        case 4:  
        	ucbyte1 = CHAR_BIG_C;
        	ucTemp1 = (unsigned char)(ledNumber / 100);
        	ucTemp2 = (unsigned char)((ledNumber - ucTemp1*100) / 10);
        	ucTemp3 = (unsigned char)(ledNumber % 10);
        	ucbyte2 = drv_LedNumCovert(ucTemp1);
        	ucbyte3 = drv_LedNumCovert(ucTemp2);
        	ucbyte4 = drv_LedNumCovert(ucTemp3);
        	break;
        case 5:   //Disp : dxxx
        	ucbyte1 = CHAR_LIT_d;
        	ucTemp1 = (unsigned char)(ledNumber / 100);
        	ucTemp2 = (unsigned char)((ledNumber - ucTemp1*100) / 10);
        	ucTemp3 = (unsigned char)(ledNumber % 10);
        	ucbyte2 = drv_LedNumCovert(ucTemp1);
        	ucbyte3 = drv_LedNumCovert(ucTemp2);
        	ucbyte4 = drv_LedNumCovert(ucTemp3);
        	break;

    }

    mLedUpdate = FALSE;
    
#if (defined(CHIP_HI3716MV300) || defined(CHIP_HI3716MV300TFADV) || defined(CHIP_HI3716MV300SMADV))
    mLedData[0] = ~ucbyte1;
    mLedData[1] = ~ucbyte2;
    mLedData[2] = ~ucbyte3;
    mLedData[3] = ~ucbyte4;
 #else
     mLedData[0] = ucbyte1;
     mLedData[1] = ucbyte2;
     mLedData[2] = ucbyte3;
     mLedData[3] = ucbyte4;
#endif
    mLedUpdate = TRUE;

}

static U8 drv_LedCharCovert(U8 charAscc)
{
    U8 LedChar;
    
    LedChar = NUM_NULL;
    switch(charAscc)
    {
        case '0':
        	LedChar = NUM_0;
        	break;
        case '1':
        	LedChar = NUM_1;
        	break;
        case '2':
        	LedChar = NUM_2;
        	break;
        case '3':
        	LedChar = NUM_3;
        	break;
        case '4':
        	LedChar = NUM_4;
        	break;
        case '5':
        	LedChar = NUM_5;
        	break;
        case '6':
        	LedChar = NUM_6;
        	break;
        case '7':
        	LedChar = NUM_7;
        	break;
        case '8':
        	LedChar = NUM_8;
        	break;
        case '9':
        	LedChar = NUM_9;
        	break;
        case 'A':
        	LedChar = CHAR_BIG_A;
        	break;            
        case 'B':
        	LedChar = CHAR_BIG_B;
        	break;
        case 'C':
        	LedChar = CHAR_BIG_C;
        	break;
        case 'D':
        	LedChar = CHAR_BIG_D;
        	break;
        case 'E':
        	LedChar = CHAR_BIG_E;
        	break;
        case 'F':
        	LedChar = CHAR_BIG_F;
        	break;
        case 'G':
        	LedChar = CHAR_BIG_G;
        	break;
        case 'H':
        	LedChar = CHAR_BIG_H;
        	break;
        case 'I':
        	LedChar = CHAR_BIG_I;
        	break;
        case 'J':
        	LedChar = CHAR_BIG_J;
        	break;
        case 'K':
        	LedChar = CHAR_BIG_K;
        	break;
        case 'L':
        	LedChar = CHAR_BIG_L;
        	break;
        case 'M':
        	LedChar = CHAR_BIG_M;
        	break;
        case 'N':
        	LedChar = CHAR_BIG_N;
        	break;
        case 'O':
        	LedChar = CHAR_BIG_O;
        	break;
        case 'P':
        	LedChar = CHAR_BIG_P;
        	break;
        case 'Q':
        	LedChar = CHAR_BIG_Q;
        	break;
        case 'R':
        	LedChar = CHAR_BIG_R;
        	break;
        case 'S':
        	LedChar = CHAR_BIG_S;
        	break;
        case 'T':
        	LedChar = CHAR_BIG_T;
        	break;
        case 'U':
        	LedChar = CHAR_BIG_U;
        	break;
        case 'V':
        	LedChar = CHAR_BIG_V;
        	break;
        case 'W':
        	LedChar = CHAR_BIG_W;
        	break;
        case 'X':
        	LedChar = CHAR_BIG_X;
        	break;
        case 'Y':
        	LedChar = CHAR_BIG_Y;
        	break;
        case 'Z':
        	LedChar = CHAR_BIG_Z;
        	break;
        case 'a':
        	LedChar = CHAR_LIT_a;
        	break;
        case 'b':
        	LedChar = CHAR_LIT_b;
        	break;
        case 'c':
        	LedChar = CHAR_LIT_c ;
        	break;
        case 'd':
        	LedChar = CHAR_LIT_d;
        	break;
        case 'e':
        	LedChar = CHAR_LIT_e;
        	break;
        case 'g':
        	LedChar = CHAR_LIT_g;
        	break;
        case 'h':
        	LedChar = CHAR_LIT_h;
        	break;
        case 'l':
        	LedChar = CHAR_BIG_L;
        	break;            
        case 'm':
        	LedChar = CHAR_LIT_m;
        	break;
        case 'n':
        	LedChar = CHAR_LIT_n;
        	break;
        case 'o':
        	LedChar = CHAR_LIT_o;
        	break;
        case 'p':
        	LedChar = CHAR_LIT_p;
        	break;
        case 'q':
        	LedChar = CHAR_LIT_q;
        	break;
        case 'r':
        	LedChar = CHAR_LIT_r;
        	break;
        case 't':
        	LedChar = CHAR_LIT_t ;
        	break;
        case 'u':
        	LedChar = CHAR_LIT_u;
        	break;
        case 'y':
        	LedChar = CHAR_LIT_y;
        	break;
        case '-':
        	LedChar = STANDBY_STATUS_LINE;
        	break;            
        default:
        	break;        
    }
    return LedChar;

}

void DRV_LED_VideoChan(U16 ledNumber)
{
	drv_LedDisplay(0, ledNumber );
}
void DRV_LED_VideoChan2(U16 ledNumber)
{
	drv_LedDisplay(4, ledNumber );	
}
void DRV_LED_VideoChan3_dxxx(U16 ledNumber)
{
	drv_LedDisplay(LED_DISPLAY_DXXX,ledNumber );	
}


void DRV_LED_AudioChan(U16 ledNumber)
{
	drv_LedDisplay(1, ledNumber );
}

void DRV_LED_DisplayClock(U8 Hour, U8 Min)
{
    U8 ucbyte1;

    ucbyte1 = Hour/10; 
    mLedDataTemp[0] = drv_LedNumCovert(ucbyte1);
    ucbyte1 = Hour%10; 
    mLedDataTemp[1] = drv_LedNumCovert(ucbyte1);
    ucbyte1 = Min/10; 
    mLedDataTemp[2] = drv_LedNumCovert(ucbyte1)|(mLedDataTemp[2]&0x01);
    ucbyte1 = Min%10; 
    mLedDataTemp[3] = drv_LedNumCovert(ucbyte1);

    drv_LedDisplay(3,0 );
}

void DRV_LED_DisplayLoad()
{
    mLedDataTemp[0] = CHAR_BIG_L;
    mLedDataTemp[1] = CHAR_LIT_o;
    mLedDataTemp[2] = CHAR_BIG_A;
    mLedDataTemp[3] = CHAR_LIT_d;

    drv_LedDisplay(3,0 );
}

void DRV_LED_DisplaydAtA()
{
    mLedDataTemp[0] = CHAR_LIT_d;
    mLedDataTemp[1] = CHAR_BIG_A;
    mLedDataTemp[2] = CHAR_LIT_t;
    mLedDataTemp[3] = CHAR_BIG_A;

    drv_LedDisplay(3,0 );
}

void DRV_LED_Displayboot()
{
    mLedDataTemp[0] = CHAR_LIT_b;
    mLedDataTemp[1] = CHAR_LIT_o;
    mLedDataTemp[2] = CHAR_LIT_o;
    mLedDataTemp[3] = CHAR_LIT_t;

    drv_LedDisplay(3,0 );
}

void DRV_LED_DisplayNvod(void)
{
    mLedDataTemp[0] = CHAR_LIT_n;
    mLedDataTemp[1] = CHAR_LIT_u;
    mLedDataTemp[2] = CHAR_LIT_o;
    mLedDataTemp[3] = CHAR_LIT_d;

    drv_LedDisplay(3,0 );
}

void DRV_LED_Displayvod(void)
{
    mLedDataTemp[0] = NUM_NULL;
    mLedDataTemp[1] = CHAR_LIT_u;
    mLedDataTemp[2] = CHAR_LIT_o;
    mLedDataTemp[3] = CHAR_LIT_d;

    drv_LedDisplay(3,0 );
}
void DRV_LED_DisplayNULL(void)
{
    mLedDataTemp[0] = NUM_NULL;
    mLedDataTemp[1] = NUM_NULL;
    mLedDataTemp[2] = NUM_NULL;
    mLedDataTemp[3] = NUM_NULL;

    drv_LedDisplay(3,0 );
}

void DRV_LED_DisplayColon()
{
    mLedDataTemp[2] &= 0xfe;
	drv_LedDisplay(3,0 );
}

void DRV_LED_HideColon()
{
    mLedDataTemp[2] |= 0x01;
	drv_LedDisplay(3,0 );
}

void DRV_LED_DisplayTime(HI_U32 hour,HI_U32 min)
{
    mLedDisplayTime = HI_TRUE;
    
    stLedTime.u32Hour = hour;
    stLedTime.u32Minute = min;
    
    return;
}

void DRV_LED_UnDisplayTime()
{
    mLedDisplayTime = HI_FALSE;

    return;
}

void DRV_LED_DisplayIP(void)
{
    mLedDataTemp[0] = NUM_NULL;
    mLedDataTemp[1] = CHAR_BIG_I;
    mLedDataTemp[2] = CHAR_BIG_P;
    mLedDataTemp[3] = NUM_NULL;

    drv_LedDisplay(LED_DISPLAY_CHAR,0 );
}
void DRV_LED_DisplaySCAN(void)
{
    mLedDataTemp[0] = CHAR_BIG_S;
    mLedDataTemp[1] = CHAR_BIG_C;
    mLedDataTemp[2] = CHAR_BIG_A;
    mLedDataTemp[3] = CHAR_BIG_N;

    drv_LedDisplay(LED_DISPLAY_CHAR,0 );
}

void DRV_LED_SetStandBy(void)
{
    drv_LedDisplay(2,0);
}

 void DRV_LED_ALLOPEN(void)
{
    mLedDataTemp[0] = NUM_8;
    mLedDataTemp[1] = NUM_8;
    mLedDataTemp[2] = NUM_8;
    mLedDataTemp[3] = NUM_8;
 
    drv_LedDisplay(LED_DISPLAY_CHAR,0 );
}

void DRV_LED_DisplayChar(U8 d1,U8 d2,U8 d3,U8 d4)
{
    mLedDataTemp[0] = drv_LedCharCovert(d1);
    mLedDataTemp[1] = drv_LedCharCovert(d2);
    mLedDataTemp[2] = drv_LedCharCovert(d3)|(mLedDataTemp[2]&0x01);
    mLedDataTemp[3] = drv_LedCharCovert(d4);

    drv_LedDisplay(3,0 );
}

void DRV_LED_DisplayIndicator(U32_YS d,BOOL mask)
{
    U8 i,j,k;

    if ((d<0)||(d>31))
        return;

    if (mask)
    {
        mLedDataTemp[0] = ((d & 0xff000000) >> 24);
        mLedDataTemp[1] = ((d & 0xff0000) >> 16);
        mLedDataTemp[2] = ((d & 0xff00) >> 8);
        mLedDataTemp[3] = (d & 0xff);
    }
    else
    {
        mLedDataTemp[0] = ~((d & 0xff000000) >> 24);
        mLedDataTemp[1] = ~((d & 0xff0000) >> 16);
        mLedDataTemp[2] = ~((d & 0xff00) >> 8);
        mLedDataTemp[3] = ~(d & 0xff);
    }
        
    drv_LedDisplay(3,0);
}

void DRV_AudioSpdifEnable(void)
{
	/* SPDIF *//* Output */
}
void DRV_AudioHardwareMute(BOOL IsMute)
{
    switch(IsMute)
    {
        case FALSE:
            break;
        case TRUE:
        default:
            break;
    }
}

void DRV_TunerResetStatus(BOOL IsHigh)
{
    switch(IsHigh)
    {
        case TRUE:
            break;
        case FALSE:
        default:
            break;
    }
}

static U32 g_au32SampleGpioRegValue = 0;
static BOOL mIsFirstCall;
#if (defined(CHIP_HI3716MV300) || defined(CHIP_HI3716MV300TFADV) || defined(CHIP_HI3716MV300SMADV))
#ifndef BIGUA
/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void sampleConfigGpioPinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + 0x00B0), &g_au32SampleGpioRegValue); /*store old pin function define */
    g_au32SampleGpioRegValue &= 0xFFFFFFF8;
    HI_SYS_WriteRegister((0x10203000 + 0x00B0), g_au32SampleGpioRegValue);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

/* you don't resume the pin function if the pin  all use to gpio,otherwise you deed resume the pin functin with old function*/
static void sampleRestoreGpioPinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + 0x00B0), g_au32SampleGpioRegValue); /* resume the pin functin with old function */
}

DRV_ErrCode DRV_Lock_Display(BOOL Lock)
{
    DRV_ErrCode ErrCode;
    ErrCode = HI_UNF_LED_DispLockLed(Lock);
    if ( HI_SUCCESS != ErrCode )
    {
        printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
        return ErrCode;
    }
    return (DRV_NO_ERROR);
}
#else
#define uart1_gpio_usb1_i2c1 0x28
#define GPIO1_4 (1*8+4)

/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void sampleConfigGpioPinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + uart1_gpio_usb1_i2c1), &g_au32SampleGpioRegValue); /*store old pin function define */
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb1_i2c1), 0x00);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

/* you don't resume the pin function if the pin  all use to gpio,otherwise you deed resume the pin functin with old function*/
static void sampleRestoreGpioPinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + uart1_gpio_usb1_i2c1), g_au32SampleGpioRegValue); /* resume the pin functin with old function */
}

DRV_ErrCode DRV_Lock_Display(BOOL Lock)
{
    DRV_ErrCode ErrCode;
    if(mIsFirstCall == FALSE)
    {
        mIsFirstCall = TRUE;
        sampleConfigGpioPinMux();
#if 0
        ErrCode = HI_UNF_GPIO_Open();
        if (HI_SUCCESS != ErrCode)
        {
            printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
            return ErrCode;
        }
#endif
        ErrCode = HI_UNF_GPIO_SetDirBit(GPIO1_4, HI_FALSE); /*set GPIO8_6 direction is output*/
        if (HI_SUCCESS != ErrCode)
        {
            printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
            HI_UNF_GPIO_Close();
            return ErrCode;
        }
    }
    
    if(Lock == TRUE)
    {
        ErrCode = HI_UNF_GPIO_WriteBit(GPIO1_4, HI_FALSE);
    }
    else
    {
        ErrCode = HI_UNF_GPIO_WriteBit(GPIO1_4, HI_TRUE);
    }
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_GPIO_Close();
        return ErrCode;
    }
    
    return (DRV_NO_ERROR);
}
#endif
#elif defined(CHIP_HI3716M)
/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void sampleConfigGpioPinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + 0x00B0), &g_au32SampleGpioRegValue); /*store old pin function define */
    g_au32SampleGpioRegValue &= 0xFFFFFFF8;
    HI_SYS_WriteRegister((0x10203000 + 0x00B0), g_au32SampleGpioRegValue);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

/* you don't resume the pin function if the pin  all use to gpio,otherwise you deed resume the pin functin with old function*/
static void sampleRestoreGpioPinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + 0x00B0), g_au32SampleGpioRegValue); /* resume the pin functin with old function */
}

DRV_ErrCode DRV_Lock_Display(BOOL Lock)
{
	DRV_ErrCode ErrCode;
	if(mIsFirstCall == FALSE)
	{
		mIsFirstCall = TRUE;
	    sampleConfigGpioPinMux();
	    
	    ErrCode = HI_UNF_GPIO_Open();
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
	        return ErrCode;
	    }

	    ErrCode = HI_UNF_GPIO_SetDirBit(86, HI_FALSE); /*set GPIO8_6 direction is output*/
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
	        HI_UNF_GPIO_Close();
	        return ErrCode;
	    }
	}
	if(Lock == TRUE)
	{
    	ErrCode = HI_UNF_GPIO_WriteBit(86, HI_FALSE);
    }
    else
	{
    	ErrCode = HI_UNF_GPIO_WriteBit(86, HI_TRUE);
    }
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_GPIO_Close();
        return ErrCode;
    }
    return (DRV_NO_ERROR);
}

#elif defined(CHIP_HI3716C)
/* you need make sure the multi-purpose pin config gpio mode before using gpio function.configing register please reference hardware user manual*/
static void sampleConfigGpioPinMux(void)
{
    HI_SYS_ReadRegister((0x10203000 + 0x0128), &g_au32SampleGpioRegValue); /*store old pin function define */
    g_au32SampleGpioRegValue &= 0xFFFFFFFC;
    g_au32SampleGpioRegValue |= 0x00000001;
    HI_SYS_WriteRegister((0x10203000 + 0x0128), g_au32SampleGpioRegValue);   /*config0x10203188 relation  pin function with 0x01 ,you need reference manual to understand this meaning */
}

/* you don't resume the pin function if the pin  all use to gpio,otherwise you deed resume the pin functin with old function*/
static void sampleRestoreGpioPinMux(void)
{
    HI_SYS_WriteRegister((0x10203000 + 0x00B0), g_au32SampleGpioRegValue); /* resume the pin functin with old function */
}

DRV_ErrCode DRV_Lock_Display(BOOL Lock)
{
	DRV_ErrCode ErrCode;
	if(mIsFirstCall == FALSE)
	{
		mIsFirstCall = TRUE;
	    sampleConfigGpioPinMux();
	    
	    ErrCode = HI_UNF_GPIO_Open();
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
	        return ErrCode;
	    }

	    ErrCode = HI_UNF_GPIO_SetDirBit(7*8+5, HI_FALSE); /*set GPIO7_5 direction is output*/
	    if (HI_SUCCESS != ErrCode)
	    {
	        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
	        HI_UNF_GPIO_Close();
	        return ErrCode;
	    }
	}
	if(Lock == TRUE)
	{
    	ErrCode = HI_UNF_GPIO_WriteBit(7*8+5, HI_FALSE);
    }
    else
	{
    	ErrCode = HI_UNF_GPIO_WriteBit(7*8+5, HI_TRUE);
    }
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        HI_UNF_GPIO_Close();
        return ErrCode;
    }
    return (DRV_NO_ERROR);
}

#else
#error "not select chip type || select chip error !"
#endif

#if 0
static HI_BOOL mHornIsFirstCall;
static HI_U32 g_au32SampleGpioRegValue;
DRV_ErrCode DRV_Horn_Control( int Link ) /*0:断开 ， 1:连接 */
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;

	if ( mHornIsFirstCall == FALSE )
	{
		mHornIsFirstCall = TRUE;
		
        HI_SYS_ReadRegister(MULIT_CONFIG_BASE + 0x148, &g_au32SampleGpioRegValue); /*store old pin function define */
        g_au32SampleGpioRegValue &= 0xFFFFFFFc;
        HI_SYS_WriteRegister(MULIT_CONFIG_BASE + 0x148, g_au32SampleGpioRegValue); 
        
		ErrCode = HI_UNF_GPIO_Open();
		if ( HI_SUCCESS != ErrCode )
		{
			printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
			return ErrCode;
		}

		ErrCode = HI_UNF_GPIO_SetDirBit( 11*8+3, HI_FALSE ); /*set GPIO11_3 direction is output*/
		if ( HI_SUCCESS != ErrCode )
		{
			printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
			HI_UNF_GPIO_Close();
			return ErrCode;
		}
	}

	if ( Link == 1 )
	{
		ErrCode = HI_UNF_GPIO_WriteBit( 11*8+3, HI_FALSE );
	}
	else
	{
		ErrCode = HI_UNF_GPIO_WriteBit( 11*8+3, HI_TRUE );
	}
	
	if ( HI_SUCCESS != ErrCode )
	{
		printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
		HI_UNF_GPIO_Close();
		return ErrCode;
	}

	return ErrCode;
}
#endif


#define MULIT_CONFIG_BASE (0x10203000)
static HI_BOOL mHornIsFirstCall;
static HI_U32 g_au32SampleGpioRegValue;
DRV_ErrCode DRV_Horn_Control( int Link ) /*0:断开 ， 1:连接 */
{
	DRV_ErrCode ErrCode = DRV_NO_ERROR;

	if ( mHornIsFirstCall == FALSE )
	{
		mHornIsFirstCall = TRUE;
		
        HI_SYS_ReadRegister(MULIT_CONFIG_BASE + 0x158, &g_au32SampleGpioRegValue); /*store old pin function define */
        g_au32SampleGpioRegValue &= 0xFFFFFFF8;
        g_au32SampleGpioRegValue |= 0x00000003;
        HI_SYS_WriteRegister(MULIT_CONFIG_BASE + 0x158, g_au32SampleGpioRegValue); 
        
		ErrCode = HI_UNF_GPIO_Open();
		if ( HI_SUCCESS != ErrCode )
		{
			printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
			return ErrCode;
		}

		ErrCode = HI_UNF_GPIO_SetDirBit( 12*8+4, HI_FALSE ); /*set GPIO12_4 direction is output*/
		if ( HI_SUCCESS != ErrCode )
		{
			printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
			HI_UNF_GPIO_Close();
			return ErrCode;
		}
	}

	if ( Link == 1 )
	{
		ErrCode = HI_UNF_GPIO_WriteBit( 12*8+4, HI_FALSE );
	}
	else
	{
		ErrCode = HI_UNF_GPIO_WriteBit( 12*8+4, HI_TRUE );
	}
	
	if ( HI_SUCCESS != ErrCode )
	{
		printf( "%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode );
		HI_UNF_GPIO_Close();
		return ErrCode;
	}

	return ErrCode;
}




