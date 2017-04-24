/*****************************************************************************
*
* Description: 
*    Çý¶¯²ãÃæ°åÄ£¿éÍ·ÎÄ¼þ
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2004-03-17    yzb            ´´½¨
* 2. 2005-10-12    Qiujordan      Ìí¼Ó´íÎóÂë¶¨Òå
* 3. 2005-12-27    Qiujordan      Ìí¼Ó»Øµ÷º¯Êý£¬Çý¶¯²ãºÍÓ¦ÓÃ²ã·ÖÀë
*
*****************************************************************************/
#ifndef _YS_PANEL_H_
#define _YS_PANEL_H_

#include "drv_typedef.h"
#include "ys_typedef.h"
#ifdef __cplusplus
extern "C" {
#endif

#define YSPANEL_KEY_PRESS_EVT 0xff000000

#define REMOTE_KEY_REPEATED                     0x80  /* 重复键�?*/

#define RKEY_DIGIT0         	0x00   /* 0 */
#define RKEY_DIGIT1         	0x01   /* 1 */
#define RKEY_DIGIT2         	0x02   /* 2 */
#define RKEY_DIGIT3         	0x03   /* 3 */
#define RKEY_DIGIT4         	0x04   /* 4 */
#define RKEY_DIGIT5         	0x05   /* 5 */
#define RKEY_DIGIT6         	0x06   /* 6 */
#define RKEY_DIGIT7         	0x07   /* 7 */
#define RKEY_DIGIT8         	0x08   /* 8 */
#define RKEY_DIGIT9         	0x09   /* 9 */

#define RKEY_UP               0x0A    /* UP/CH+      */
#define RKEY_DOWN             0x0B    /* DOWN/CH-    */
#define RKEY_LEFT             0x0C    /* LEFT/VOL-   */
#define RKEY_RIGHT            0x0D    /* RIGHT/VOL+  */

#define RKEY_PAGE_UP          0x0E
#define RKEY_PAGE_DOWN        0x0F

#define RKEY_MUTE             0x10    /* MUTE  */
#define RKEY_TRACK            0x11
#define RKEY_VOL_INC          0x12
#define RKEY_VOL_DEC          0x13

#define RKEY_LAST_CHANNEL     0x14    /* LASTCHANNEL */
#define RKEY_CHNL_INC         0x15
#define RKEY_CHNL_DEC         0x16

#define RKEY_OK               0x17
#define RKEY_MENU             0x18
#define RKEY_EXIT             0x19
#define RKEY_POWER            0x1A    /* POWER */
#define RKEY_PAUSE            0x1B
#define RKEY_HELP             0x1C    /* HELP/ZOOM */
#define RKEY_DOUBLE_DIGIT     0x1D    /* DOUBLE DIGIT -/-- */
#define RKEY_CHNSET           0x1E    /*channel set*/
#define RKEY_BACK             0x1F    /*返回*/

#define RKEY_SWITCH            0x20    /*切换*/
#define RKEY_GAME             0x21    /*游戏*/
#define RKEY_STOCK            0x22    /*股票*/
#define RKEY_WEB              0x23    /*网站    阳光政务*/
#define RKEY_MOVIE           0x24     /*影院*/
#define RKEY_TV               0x25  
#define RKEY_RADIO            0x26
#define RKEY_VOD              0x27
#define RKEY_EPG    		  0x28
#define RKEY_DBC              0x29     
#define RKEY_EMAIL            0x2A
#define RKEY_INFO             0x2B  
#define RKEY_FAVOURITE        0x2C
#define RKEY_BOOK          0x2D   /*预览*/
#define RKEY_CAPTION          0x2E   /*字幕*/
#define RKEY_CHNLIST          0x2F   /*频道列表，浏览，指南*/

#define RKEY_RED              0x30
#define RKEY_GREEN            0x31
#define RKEY_YELLOW           0x32
#define RKEY_BLUE             0x33

#define RKEY_F3			0x34
#define RKEY_F4			0x35
#define RKEY_F5				  0x36
#define RKEY_F6				  0x37
#define RKEY_F7               0x38
#define RKEY_F8               0x39
#define RKEY_F9               0x3A
#define RKEY_F10              0x3B

#define RKEY_TELEPLAY		0x3C  
#define RKEY_MUSIC	          0x3D   
#define RKEY_LANGUAGE	          0x3E   
#define RKEY_USER_DEFINED_4	          0x3F   


#define RKEY_UP_REPEAT      	(RKEY_UP + REMOTE_KEY_REPEATED)
#define RKEY_DOWN_REPEAT    	(RKEY_DOWN + REMOTE_KEY_REPEATED)
#define RKEY_LEFT_REPEAT    	(RKEY_LEFT + REMOTE_KEY_REPEATED)
#define RKEY_RIGHT_REPEAT   	(RKEY_RIGHT + REMOTE_KEY_REPEATED)
#define RKEY_PAGE_UP_REPEAT    	(RKEY_PAGE_UP + REMOTE_KEY_REPEATED)
#define RKEY_PAGE_DOWN_REPEAT  	(RKEY_PAGE_DOWN + REMOTE_KEY_REPEATED)
 
#define RKEY_VOL_INC_REPEAT     (RKEY_VOL_INC + REMOTE_KEY_REPEATED)
#define RKEY_VOL_DEC_REPEAT     (RKEY_VOL_DEC + REMOTE_KEY_REPEATED)


/*面板按键定义 */
#define MENU_CUM_EXIT_KEY               				0x51    /* when SW1 & SW2 are pressed together */
#define VOL_DOWN_CUM_LEFT_ARROW_KEY   		0x55    /* volume- & left arrow key */
#define VOL_UP_CUM_RIGHT_ARROW_KEY      		0x53    /* volume+ & right arrow key */
#define PROG_PREV_CUM_UP_ARROW_KEY     		0x54    /* prog- & up arrow key */
#define PROG_NEXT_CUM_DOWN_ARROW_KEY    		0x52    /* prog+ & down arrow key */
#define POWER_CUM_SELECT_KEY            			0x56    /* power & select key */
#define SELECT_KEY                  					0x57        /* RSW22 or SW5 */ 
#define STANDBY_KEY                                                          0x59

#define NUM_NULL 0xFF
#define NUM_0 0x03
#define NUM_1 0x9F
#define NUM_2 0x25
#define NUM_3 0x0d
#define NUM_4 0x99
#define NUM_5 0x49
#define NUM_6 0x41
#define NUM_7 0x1f
#define NUM_8 0x01
#define NUM_9 0x09

#define CHAR_BIG_A 0x11
#define CHAR_BIG_B 0xFF
#define CHAR_BIG_C 0x63
#define CHAR_BIG_D 0xFF
#define CHAR_BIG_E 0x61
#define CHAR_BIG_F 0x71
#define CHAR_BIG_G 0x43
#define CHAR_BIG_H 0x91
#define CHAR_BIG_I 0xF3
#define CHAR_BIG_J 0x8F
#define CHAR_BIG_K 0xFF
#define CHAR_BIG_L 0xE3
#define CHAR_BIG_M 0xFF
#define CHAR_BIG_N 0x13
#define CHAR_BIG_O 0x03
#define CHAR_BIG_P 0x31
#define CHAR_BIG_Q 0xFF
#define CHAR_BIG_R 0xFF
#define CHAR_BIG_S 0x49
#define CHAR_BIG_T 0xFF
#define CHAR_BIG_U 0x83
#define CHAR_BIG_V 0xFF
#define CHAR_BIG_W 0xFF
#define CHAR_BIG_X 0xFF
#define CHAR_BIG_Y 0xFF
#define CHAR_BIG_Z 0xFF

#define CHAR_LIT_a 0x05
#define CHAR_LIT_b 0xC1
#define CHAR_LIT_c 0xE5
#define CHAR_LIT_d 0x85
#define CHAR_LIT_e 0x21
#define CHAR_LIT_f 0xFF
#define CHAR_LIT_g 0x09
#define CHAR_LIT_h 0xD1
#define CHAR_LIT_i 0xDF
#define CHAR_LIT_j 0xFF
#define CHAR_LIT_k 0xFF
#define CHAR_LIT_l 0x9F
#define CHAR_LIT_m 0xFF
#define CHAR_LIT_n 0xD5
#define CHAR_LIT_o 0xC5
#define CHAR_LIT_p 0x31
#define CHAR_LIT_q 0x19
#define CHAR_LIT_r 0xF5
#define CHAR_LIT_s 0x49
#define CHAR_LIT_t 0xE1
#define CHAR_LIT_u 0xC7
#define CHAR_LIT_v 0xC7
#define CHAR_LIT_w 0xFF
#define CHAR_LIT_x 0xFF
#define CHAR_LIT_y 0x89
#define CHAR_LIT_z 0xFF

#define STANDBY_STATUS_LINE 0xfd//0x9F
#define STANDBY_DOUBLE_LINE 0xfd//0x9F  

enum
{
	LED_DISPLAY_PXXX        = 0,    // Pxxx
	LED_DISPLAY_AXXX        = 1,   // Axxx
	LED_DISPLAY_STANDBY = 2,    // ----
	LED_DISPLAY_CHAR        = 3,   // !@#$%^&*
	LED_DISPLAY_CXXX        = 4,   // Cxxx
	LED_DISPLAY_DXXX        = 5    //dxxx
};

typedef enum DRV_PanelType_e
{
    DRV_PANEL_TYPE_RESERVED
} DRV_PanelType;

typedef void (*DRV_PanelCallback)(U8_YS Key);
DRV_ErrCode DRV_PanelGetLeaderValue(U32 *Mode,U32 *LeaderValue);
DRV_ErrCode DRV_PanelSetRemoteParam(void);
DRV_ErrCode DRV_PanelInit(DRV_PanelType PanelType);
DRV_ErrCode DRV_PanelTerm(void);
DRV_ErrCode DRV_PanelEnableCallback(DRV_PanelCallback Callback_f);
BOOL DRV_IsKeyboardLocked(void);
void DRV_KeyboardLock(void);
void DRV_KeyboardUnLock(void);
void DRV_LED_VideoChan(U16 ledNumber);
void DRV_LED_VideoChan2(U16 ledNumber);
void DRV_LED_VideoChan3_dxxx(U16 ledNumber);
void DRV_LED_AudioChan(U16 ledNumber);
void DRV_LED_DisplayClock(U8 Hour, U8 Min);
void DRV_LED_DisplayLoad();
void DRV_LED_DisplaydAtA();
void DRV_LED_Displayboot();
void DRV_LED_DisplayNvod(void);
void DRV_LED_Displayvod(void);
void DRV_LED_DisplayNULL(void);
void DRV_LED_DisplayColon();
void DRV_LED_HideColon();
void DRV_LED_DisplayIP(void);
void DRV_LED_DisplaySCAN(void);
void DRV_LED_SetStandBy(void);
void DRV_LED_ALLOPEN(void);
void DRV_LED_DisplayChar(U8 d1,U8 d2,U8 d3,U8 d4);
void DRV_LED_DisplayIndicator(U32_YS d,BOOL mask);
void DRV_AudioSpdifEnable(void);
void DRV_AudioHardwareMute(BOOL IsMute);
void DRV_TunerResetStatus(BOOL IsHigh);
DRV_ErrCode DRV_Lock_Display(BOOL Lock);
void DRV_LED_DisplayTime(HI_U32 hour,HI_U32 min);
void DRV_LED_UnDisplayTime();


#ifdef __cplusplus
}
#endif
#endif
