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
��һ��fdsΪ���/devĿ¼�Ƿ���event�豸����
�����fdsΪevent�豸����Ϣ����
*/
typedef struct 
{
	struct pollfd fds[MAX_EVDEV_COUNT];
	int count;
}DRV_InputHandle;

typedef	enum
{
	TYPE_KEY=EV_KEY,//����ֵ
	TYPE_REL=EV_REL,//���λ��
	TYPE_ABS=EV_ABS,//����λ��
} type_value;

typedef	enum
{
	CODE_KEY_UP=0,
	CODE_KEY_DOWN,  
	CODE_KEY_HOLD,
	
	CODE_REL_X,       //REL_X   ���λ��X��
	CODE_REL_Y,        //REL_Y   ���λ��Y��
	CODE_REL_WHEEL,//REL_WHEEL    ����
	
	CODE_ABS_X,       //ABS_X    ����λ��X��
	CODE_ABS_Y,       //ABS_Y    ����λ��Y��
	CODE_ABS_RX,      // ABS_RX   ����λ��RX��
	CODE_ABS_RZ,      // ABS_RZ   ����λ��RZ��
	CODE_ABS_HAT0X,//ABS_HAT0X  ����λ��HAT0X��
	CODE_ABS_HAT0Y,//ABS_HAT0Y  ����λ��HAT0Y��
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
	S32_YS wheelvalue;/*������껬��ֵ�ͻ��ְ�����Ϸ�ӳ���ֵ�״̬*/
	S32_YS mousekey;/*����������Ұ����Լ����ְ��µļ�ֵ�����궨��*/
	S32_YS mousekey_stat;
	S32_YS mouse_x;/*�����������ֵ*/
	S32_YS mouse_y;
} mouse_value_t;
typedef struct joystick_value_s
{
	joystick_type_t type;
	S32_YS axis_x;/*�����ϱ�D-Aģʽ��HT1��x����ֵ����ʼ��ֵΪ128���������ʱ��ֵ
	�𽥼�С��0�����Ҳ���ʱ����ֵ������255����D-Dģʽ�������ϱ����ܡ�*/
	S32_YS axis_y;/*�����ϱ�D-Aģʽ��HT1��y����ֵ����ʼ��ֵΪ128�����ϲ���ʱ��ֵ
	�𽥼�С��0�����²���ʱ����ֵ������255����D-Dģʽ�������ϱ����ܡ�*/
	S32_YS axis_z;/*�����书��*/ 
	S32_YS axis_rx;/*�����ϱ�D-Aģʽ��HT2��x����ֵ����ʼ��ֵΪ128���������ʱ��ֵ
	�𽥼�С��0�����Ҳ���ʱ����ֵ������255����D-Dģʽ�������ϱ����ܡ�*/
	S32_YS axis_ry;/*�����ϱ�D-Aģʽ��HT2��y����ֵ����ʼ��ֵΪ128�����ϲ���ʱ��ֵ
	�𽥼�С��0�����²���ʱ����ֵ������255����D-Dģʽ�������ϱ����ܡ�*/
	S32_YS axis_rz;/*�����书��*/ 
	S32_YS slider;/*�����书��*/ 
	S32_YS hatswitch;/*�����ϱ�D-A��D-Dģʽ��8�������Ӧ�ļ�ֵ��������궨��*/ 
	S32_YS button;/* �����ϱ�D-A��D-Dģʽ��A��B��X��Y��LB��RB��LT��RT�ļ�ֵ���궨��*/
	S32_YS button_stat;
} joystick_value_t;

typedef struct keyboard_value_t
{
	S32_YS keyboard_stat;	
	S32_YS keyboard_value;
} keyboard_value_t;

typedef struct usbvalue_s
{
	usbtype_t usbtype;/*�����豸����*/
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

