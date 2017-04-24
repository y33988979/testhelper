/**
 \file
 \brief common head file or key&mouse
 \author wrwei
 \date 2012.08.03
 */

#ifndef __DRV_KEY_MOUSE_H__
#define __DRV_KEY_MOUSE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <poll.h>
#include <linux/input.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <errno.h>
#include "stbtype.h"

#define MAX_EVDEV_COUNT		10

#define DRV_HAT_SWITCH_UP                   0
#define DRV_HAT_SWITCH_UP_RIGHT         1
#define DRV_HAT_SWITCH_RIGHT              2
#define DRV_HAT_SWITCH_DOWN_RIGHT    3
#define DRV_HAT_SWITCH_DOWN               4
#define DRV_HAT_SWITCH_DOWN_LEFT      5
#define DRV_HAT_SWITCH_LEFT               6
#define DRV_HAT_SWITCH_UP_LEFT          7
#define DRV_HAT_SWITCH_NULL                   8


#define DRV_STICK_BUTTON_1         288
#define DRV_STICK_BUTTON_2          289
#define DRV_STICK_BUTTON_3          290
#define DRV_STICK_BUTTON_4         291
#define DRV_STICK_BUTTON_5      292
#define DRV_STICK_BUTTON_6     293
#define DRV_STICK_BUTTON_7       294
#define DRV_STICK_BUTTON_8     295
#define DRV_STICK_BUTTON_SELECT     296
#define DRV_STICK_BUTTON_START     297

/*
第一个fds为检测/dev目录是否有event设备插入
后面的fds为event设备有信息传入
*/
typedef struct 
{
	struct pollfd fds[MAX_EVDEV_COUNT];
	int count;
}DRV_InputHandle;

typedef	enum
{
	TYPE_KEY=EV_KEY,//按键值
	TYPE_REL=EV_REL,//相对位置
	TYPE_ABS=EV_ABS,//绝对位置
} type_value;

typedef	enum
{
	CODE_KEY_UP=0,
	CODE_KEY_DOWN,  
	CODE_KEY_HOLD,
	
	CODE_REL_X,       //REL_X   相对位置X轴
	CODE_REL_Y,        //REL_Y   相对位置Y轴
	CODE_REL_WHEEL,//REL_WHEEL    滑轮
	
	CODE_ABS_X,       //ABS_X    绝对位置X轴
	CODE_ABS_Y,       //ABS_Y    绝对位置Y轴
	CODE_ABS_RX,      // ABS_RX   绝对位置RX轴
	CODE_ABS_RZ,      // ABS_RZ   绝对位置RZ轴
	CODE_ABS_HAT0X,//ABS_HAT0X  绝对位置HAT0X轴
	CODE_ABS_HAT0Y,//ABS_HAT0Y  绝对位置HAT0Y轴
} code_value;



typedef struct
{
	S32  value_x;
	S32  value_y;
} hatswitch_value_t;

typedef enum
{
	DEVICE_TYPE_MOUSE,
	DEVICE_TYPE_KEYBOARD,
	DEVICE_TYPE_JOYSTICK,
	DEVICE_TYPE_UNKWN
} usbtype_t;

typedef enum
{
	MOUSE_WHILE,
	MOUSE_KEY,
	MOUSE_COORD,
	MOUSE_UNKWN
} mouse_type_t;

typedef enum
{
	JOYSTICK_DIREC,
	JOYSTICK_KEY,
	JOYSTICK_STICK,
	JOYSTICK_UNKWN
} joystick_type_t;

typedef struct mouse_value_s
{
	mouse_type_t type;
	S32_YS wheelvalue;/*返回鼠标滑轮值和滑轮按键配合反映滑轮的状态*/
	S32_YS mousekey;/*返回鼠标左右按键以及滑轮按下的键值，见宏定义*/
	S32_YS mousekey_stat;
	S32_YS mouse_x;/*返回鼠标坐标值*/
	S32_YS mouse_y;
} mouse_value_t;
typedef struct joystick_value_s
{
	joystick_type_t type;
	S32_YS axis_x;/*用来上报D-A模式下HT1的x坐标值，初始化值为128，向左操作时该值
	逐渐减小到0，向右操作时该数值逐渐增大到255；在D-D模式保留其上报功能。*/
	S32_YS axis_y;/*用来上报D-A模式下HT1的y坐标值，初始化值为128，向上操作时该值
	逐渐减小到0，向下操作时该数值逐渐增大到255；在D-D模式保留其上报功能。*/
	S32_YS axis_z;/*保留其功能*/ 
	S32_YS axis_rx;/*用来上报D-A模式下HT2的x坐标值，初始化值为128，向左操作时该值
	逐渐减小到0，向右操作时该数值逐渐增大到255；在D-D模式保留其上报功能。*/
	S32_YS axis_ry;/*用来上报D-A模式下HT2的y坐标值，初始化值为128，向上操作时该值
	逐渐减小到0，向下操作时该数值逐渐增大到255；在D-D模式保留其上报功能。*/
	S32_YS axis_rz;/*保留其功能*/ 
	S32_YS slider;/*保留其功能*/ 
	S32_YS hatswitch;/*用来上报D-A和D-D模式下8个方向对应的键值，具体见宏定义*/ 
	S32_YS button;/* 用来上报D-A和D-D模式下A，B，X，Y，LB，RB，LT，RT的键值见宏定义*/
	S32_YS button_stat;
} joystick_value_t;

typedef struct keyboard_value_t
{
	S32_YS keyboard_stat;	
	S32_YS keyboard_value;
} keyboard_value_t;

typedef struct usbvalue_s
{
	usbtype_t usbtype;/*返回设备类型*/
	mouse_value_t mousevalue;
	keyboard_value_t keyboardkey;
	joystick_value_t stickkey;
} usbvalue_t;

typedef void 	(*MouseKeyboardCallBack)(usbvalue_t Usb_Value);

void Register_MouseKey_callback(MouseKeyboardCallBack cb);
int DRV_MouseInit();
int DRV_InputUninit( DRV_InputHandle *handle);

#ifdef __cplusplus
}
#endif
#endif /* __SAMPLE_AUDIO_PUB_H__ */

