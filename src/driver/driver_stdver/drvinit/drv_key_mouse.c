
#include "drv_os.h"
#include "drv_key_mouse.h"
//0~127为键盘上的按键代码,0x110~0x116 为鼠标上按键代码

#define EVENT_DEV_CREATE		1
#define EVENT_DEV_DELETE		2
#define HANLD_DIGITAL         1
#define HANLD_ANALOG         2

int drv_ReadNotify(const char *dirname, int nfd);
int drv_InputInit( DRV_InputHandle *handle);
int drv_InitPlugListener( DRV_InputHandle *handle);

DRVOS_Task_t *pMouseThread;
DRV_InputHandle *handle;
MouseKeyboardCallBack  CallBack=NULL;
int DigitalAnalog_mode=HANLD_DIGITAL;
usbvalue_t USB_Value;
hatswitch_value_t hatswitch_value;

int drv_GetEvent( DRV_InputHandle *handle,  struct input_event *event)
{
	int rn, i,j=0;
	static struct input_event event_save[3];
	static int last_flag=0;
	static int size = sizeof(struct input_event);	
	while (1)
	{
		rn = poll(handle->fds, handle->count, -1);
		if (rn < 0)
		{
			perror("poll");
			return -1;
		}
// 如果有插拔事件，重新扫描/dev
		if(handle->fds->revents & POLLIN) 
		{
			rn = drv_ReadNotify("/dev/", handle->fds->fd);
			if (rn)
			{
				drv_InputInit(handle);
				continue;
			}
		}
//检测event设备的输入信息
		for (i = 1; i < handle->count; i++)
		{
			if (((handle->fds)+i)->revents & POLLIN)
			{
				j=read(((handle->fds)+i)->fd, event, size);
#if 0		
				printf("ev.type=%d\n",event->type);
				printf("ev.code=%d\n",event->code);
				printf("ev.value=%d\n",event->value);
#endif		
//屏蔽游戏手柄的数字模拟模式的按键值上报
				if(event->type == EV_ABS&&event->code == REL_X&&
					event->value==127&&last_flag==0&&DigitalAnalog_mode==HANLD_ANALOG)
				{
					memcpy(&event_save[0],event,sizeof(struct input_event));
					last_flag=1;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_Y&&event->value==127&&last_flag==1)
				{
					memcpy(&event_save[1],event,sizeof(struct input_event));
					last_flag=2;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_RX&&event->value==127&&last_flag==2)
				{
					memcpy(&event_save[2],event,sizeof(struct input_event));
					last_flag=3;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_RZ&&event->value==127&&last_flag==3)
				{
					last_flag=0;
					DigitalAnalog_mode=HANLD_DIGITAL;
					hatswitch_value.value_x==127;
					hatswitch_value.value_y==127;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_X&&
					event->value==128&&last_flag==0&&DigitalAnalog_mode==HANLD_DIGITAL)
				{
					memcpy(&event_save[0],event,sizeof(struct input_event));
					last_flag=5;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_Y&&event->value==128&&last_flag==5)
				{
					memcpy(&event_save[1],event,sizeof(struct input_event));
					last_flag=6;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_RX&&event->value==128&&last_flag==6)
				{
					memcpy(&event_save[2],event,sizeof(struct input_event));
					last_flag=7;
					return 0;
				}
				else if(event->type == EV_ABS&&event->code == REL_RZ&&event->value==128&&last_flag==7)
				{
					last_flag=0;
					DigitalAnalog_mode=HANLD_ANALOG;
					hatswitch_value.value_x=0;
					hatswitch_value.value_y=0;
					USB_Value.stickkey.axis_x=128;
					USB_Value.stickkey.axis_y=128;
					USB_Value.stickkey.axis_rx=128;
					USB_Value.stickkey.axis_ry=128;
					return 0;
				}
				else if ((event->type == EV_KEY &&event->code != 0 &&(event->value == 1 || event->value == 2))||
					(event->type == EV_REL)||
					((event->type == EV_ABS&&event->code!=2) ))
				{
					if(last_flag==1||last_flag==5)
					{
						drv_GetKey(&event_save[0]);
					}
					if(last_flag==2||last_flag==6)
					{
						drv_GetKey(&event_save[0]);
						drv_GetKey(&event_save[1]);
					}
					if(last_flag==3||last_flag==7)
					{
						drv_GetKey(&event_save[0]);
						drv_GetKey(&event_save[1]);
						drv_GetKey(&event_save[2]);
					}
					last_flag=0;
				}
//将键值转化为标准格式			
				if ((event->type == EV_KEY &&event->code != 0 )||
					(event->type == EV_REL)||
					((event->type == EV_ABS&&event->code!=2) ))
					{
						drv_GetKey(event);
						return 0;
					}
			}
		}
	}
	return 0;
}

//将获得的键值信息上报给回调函数
int drv_GetKey(struct input_event *setevent)
{
	//键盘
	if (setevent->type == EV_KEY &&setevent->code >0 &&setevent->code<=127 )
	{
		USB_Value.usbtype=DEVICE_TYPE_KEYBOARD;
		USB_Value.keyboardkey.keyboard_stat=setevent->value;
		USB_Value.keyboardkey.keyboard_value=setevent->code;
		if(CallBack!=NULL)
				CallBack(USB_Value);
	}//鼠标
	else if ((setevent->type == EV_KEY &&(setevent->code  ==272||setevent->code  ==273||setevent->code  ==274))||
		(setevent->type == EV_REL&&(setevent->code==REL_X||setevent->code==REL_Y||setevent->code==REL_WHEEL)))
	{
		USB_Value.usbtype=DEVICE_TYPE_MOUSE;
		if(setevent->type == EV_KEY)
		{
			USB_Value.mousevalue.type=MOUSE_KEY;
			USB_Value.mousevalue.mousekey=setevent->code;
			USB_Value.mousevalue.mousekey_stat=setevent->value;
		}
		else if(setevent->type == EV_REL)
		{
			if(setevent->code==REL_X)
			{
				USB_Value.mousevalue.type=MOUSE_COORD;
				USB_Value.mousevalue.mouse_x+=setevent->value;
				if(USB_Value.mousevalue.mouse_x>=1280)USB_Value.mousevalue.mouse_x=1280;
				if(USB_Value.mousevalue.mouse_x<=0)USB_Value.mousevalue.mouse_x=0;
			}
			else if(setevent->code==REL_Y)
			{
				USB_Value.mousevalue.type=MOUSE_COORD;
				USB_Value.mousevalue.mouse_y+=setevent->value;
				if(USB_Value.mousevalue.mouse_y>=720)USB_Value.mousevalue.mouse_y=720;
				if(USB_Value.mousevalue.mouse_y<=0)USB_Value.mousevalue.mouse_y=0;
			}
			else if(setevent->code==REL_WHEEL)
			{
				USB_Value.mousevalue.type=MOUSE_WHILE;
				USB_Value.mousevalue.wheelvalue+=setevent->value;
			}
		}
		if(CallBack!=NULL)
				CallBack(USB_Value);
		
	}
	//手柄
	else if ((setevent->type == EV_KEY&&setevent->code >=280&&setevent->code<=300)||
		(setevent->type == EV_ABS&&setevent->code!=2))
	{
		USB_Value.usbtype=DEVICE_TYPE_JOYSTICK;
		if(setevent->type == EV_KEY)
		{
			USB_Value.stickkey.type=JOYSTICK_KEY;
			USB_Value.stickkey.button=setevent->code;
			USB_Value.stickkey.button_stat=setevent->value;
		}
		else if(setevent->type == EV_ABS)
		{
			if(DigitalAnalog_mode==HANLD_DIGITAL)
			{
				if(setevent->code==ABS_X)
				{
					hatswitch_value.value_x=setevent->value;
				}
				else if(setevent->code==ABS_Y)
				{
					hatswitch_value.value_y=setevent->value;
				}

				if(hatswitch_value.value_x==127&&hatswitch_value.value_y==0)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP;
				}
				else if(hatswitch_value.value_x==0&&hatswitch_value.value_y==0)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP_LEFT;
				}
				else if(hatswitch_value.value_x==0&&hatswitch_value.value_y==127)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_LEFT;
				}
				else if(hatswitch_value.value_x==0&&hatswitch_value.value_y==255)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN_LEFT;
				}
				else if(hatswitch_value.value_x==127&&hatswitch_value.value_y==255)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN;
				}
				else if(hatswitch_value.value_x==255&&hatswitch_value.value_y==255)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN_RIGHT;
				}
				else if(hatswitch_value.value_x==255&&hatswitch_value.value_y==127)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_RIGHT;
				}
				else if(hatswitch_value.value_x==255&&hatswitch_value.value_y==0)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP_RIGHT;
				}
				else if(hatswitch_value.value_x==127&&hatswitch_value.value_y==127)
				{
					USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_NULL;
				}
				USB_Value.stickkey.type=JOYSTICK_DIREC;
			}
			if(DigitalAnalog_mode==HANLD_ANALOG)
			{
				if(setevent->code==ABS_HAT0X||setevent->code==ABS_HAT0Y)
				{
					if(setevent->code==ABS_HAT0X)
					{
						hatswitch_value.value_x=setevent->value;
					}
					else if(setevent->code==ABS_HAT0Y)
					{
						hatswitch_value.value_y=setevent->value;
					}

					if(hatswitch_value.value_x==0&&hatswitch_value.value_y==-1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP;
					}
					else if(hatswitch_value.value_x==-1&&hatswitch_value.value_y==-1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP_LEFT;
					}
					else if(hatswitch_value.value_x==-1&&hatswitch_value.value_y==0)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_LEFT;
					}
					else if(hatswitch_value.value_x==-1&&hatswitch_value.value_y==1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN_LEFT;
					}
					else if(hatswitch_value.value_x==0&&hatswitch_value.value_y==1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN;
					}
					else if(hatswitch_value.value_x==1&&hatswitch_value.value_y==1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_DOWN_RIGHT;
					}
					else if(hatswitch_value.value_x==1&&hatswitch_value.value_y==0)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_RIGHT;
					}
					else if(hatswitch_value.value_x==1&&hatswitch_value.value_y==-1)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_UP_RIGHT;
					}
					else if(hatswitch_value.value_x==0&&hatswitch_value.value_y==0)
					{
						USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_NULL;
					}
					USB_Value.stickkey.type=JOYSTICK_DIREC;
				
				}
				else if(setevent->code==ABS_X||setevent->code==ABS_Y)
				{
					if(setevent->code==ABS_X)
					{
						USB_Value.stickkey.axis_x=setevent->value;
					}
					else if(setevent->code==ABS_Y)
					{
						USB_Value.stickkey.axis_y=setevent->value;
					}
					USB_Value.stickkey.type=JOYSTICK_STICK;
				}
				else if(setevent->code==ABS_RX||setevent->code==ABS_RZ)
				{
					if(setevent->code==ABS_RX)
					{
						USB_Value.stickkey.axis_rx=setevent->value;
					}
					else if(setevent->code==ABS_RZ)
					{
						USB_Value.stickkey.axis_ry=setevent->value;
					}
					USB_Value.stickkey.type=JOYSTICK_STICK;
				}
			}
		}
		if(CallBack!=NULL)
			CallBack(USB_Value);
	}
	return ;
}
int drv_ReadNotify(const char *dirname, int nfd)
{
	int res;
	char devname[PATH_MAX];
	char *filename;
	char event_buf[512];
	int event_size;
	int event_pos = 0;
	struct inotify_event *event;

	res = read(nfd, event_buf, sizeof(event_buf));
	if(res < (int)sizeof(struct inotify_event))
	{
		if(errno == EINTR)
		    return 0;
		fprintf(stderr, "could not get event, %s\n", strerror(errno));
		return 1;
    	}

	strcpy(devname, dirname);
	filename = devname + strlen(devname);
	*filename++ = '/';

	while(res >= (int)sizeof(*event)) 
	{
        	event = (struct inotify_event *)(event_buf + event_pos);
        	if(event->len) 
		{
			if (strncmp(event->name, "event", 5))
			{
				event_size = sizeof(*event) + event->len;
				res -= event_size;
				event_pos += event_size;
				continue;
			}
            		strcpy(filename, event->name);
            		if(event->mask & IN_CREATE) 
			{
				//open_device(devname);
				printf ("New device is found: %s\n", filename);
				return EVENT_DEV_CREATE;
            		}
            		else 
			{
				//close_device(devname);
				printf ("Device is removed: %s\n", filename);
				return EVENT_DEV_DELETE;
            		}
        	}
		event_size = sizeof(*event) + event->len;
		res -= event_size;
		event_pos += event_size;
    }
    return 0;
}

int drv_InputInit( DRV_InputHandle *handle)
{	
	int count = 1;
	char devname[64];
	char *dirname = "/dev";
	char *filename;
	DIR *dir;
	int i=0;
	struct dirent *de;
	memset (handle, 0, sizeof( DRV_InputHandle));

	DigitalAnalog_mode=HANLD_ANALOG;	
	drv_InitPlugListener(handle);
	printf ("Notify, fd = %d\n",  handle->fds->fd);
	(handle->count)++;
    
	dir = opendir(dirname);
	if(dir == NULL)
	{
	    return -1;
	}

	strcpy(devname, dirname);
	filename = devname + strlen(devname);
	printf(" =%d  \n",strlen(devname));
	*filename++ = '/';
		
    	while((de = readdir(dir))) 
    	{
		if(de->d_name[0] == '.' &&
		       (de->d_name[1] == '\0' ||(de->d_name[1] == '.' && de->d_name[2] == '\0')) 
		        ||strncmp(de->d_name, "event", 5))
		{
		        continue;
		}
		    
		strcpy(filename, de->d_name);
		if (((handle->fds+handle->count)->fd = open(devname, O_RDONLY)) < 0)
		{
			perror ("open");
			continue;
		}
		
		printf ("Add %s, fd = %d\n", devname, (handle->fds+handle->count)->fd);
		
		(handle->fds+handle->count)->events = POLLIN;
		(handle->count)++;
		//printf(" handle->count=%d  \n",handle->count);
    	}
    
    	closedir(dir);
	
    	return 0;
}

int DRV_InputUninit( DRV_InputHandle *handle)
{
	int i;
	
	if (handle->count > 0)
		for (i = 1; i < handle->count; i++)
			close((handle->fds+i)->fd);
		
	return  0;
}

int drv_InitPlugListener( DRV_InputHandle *handle)
{
    int res;
    const char *device_path = "/dev/";
	
    handle->fds->fd = inotify_init();
    handle->fds->events = POLLIN;

	res = inotify_add_watch(handle->fds->fd, device_path, IN_DELETE | IN_CREATE);
	if(res < 0) 
	{
	    return -1;
	}

    return 0;
}

void drv_MouseReceiveTask()
{
	int error;
	struct input_event * eventtemp;
	while(1)
	{
		error=drv_GetEvent(handle, eventtemp);
		if(error!=0)
		{
			printf("ERROR\n");
		}
	}
}
void MouseCallBack(usbvalue_t Usb_Value)
{
	if(Usb_Value.usbtype==DEVICE_TYPE_KEYBOARD)
	{
		printf("type: KEYBOARD\n");
		printf("value =%d \n",Usb_Value.keyboardkey.keyboard_value);
	}
	else if(Usb_Value.usbtype==DEVICE_TYPE_MOUSE)
	{
		printf("type: MOUSE\n");
		if(Usb_Value.mousevalue.type==MOUSE_WHILE)
		{
			printf("wheelvalue =%d \n",Usb_Value.mousevalue.wheelvalue);
		}
		if(Usb_Value.mousevalue.type==MOUSE_KEY)
		{
			printf("key =%d \n",Usb_Value.mousevalue.mousekey);
		}
		if(Usb_Value.mousevalue.type==MOUSE_COORD)
		{
			printf("mouse_x =%d \n",Usb_Value.mousevalue.mouse_x);
			printf("mouse_y=%d \n",Usb_Value.mousevalue.mouse_y);
		}	
	}
	else if(Usb_Value.usbtype==DEVICE_TYPE_JOYSTICK)
	{
		printf("type: JOYSTICK\n");
		if(Usb_Value.stickkey.type==JOYSTICK_KEY)
		{
			printf("button =%d \n",Usb_Value.stickkey.button);
		}	
		if(Usb_Value.stickkey.type==JOYSTICK_DIREC)
		{
			printf("hatswitch =%d \n",Usb_Value.stickkey.hatswitch);
		}	
		if(Usb_Value.stickkey.type==JOYSTICK_STICK)
		{
			printf("axis_x =%d \n",Usb_Value.stickkey.axis_x);
			printf("axis_y =%d \n",Usb_Value.stickkey.axis_y);
			printf("axis_rx =%d \n",Usb_Value.stickkey.axis_rx);
			printf("axis_ry =%d \n",Usb_Value.stickkey.axis_ry);
		}	
	}
}

void Register_MouseKey_callback(MouseKeyboardCallBack cb)
{
    CallBack = cb;
}

int DRV_MouseInit()
{
	int error;
	handle=malloc(sizeof(DRV_InputHandle));
	memset(&USB_Value,0,sizeof(USB_Value));
	USB_Value.stickkey.hatswitch=DRV_HAT_SWITCH_NULL;
	hatswitch_value.value_x=127;
	hatswitch_value.value_y=127;
	USB_Value.stickkey.axis_x=128;
	USB_Value.stickkey.axis_y=128;
	USB_Value.stickkey.axis_rx=128;
	USB_Value.stickkey.axis_ry=128;
	USB_Value.mousevalue.mouse_x=640;
	USB_Value.mousevalue.mouse_y=360;
	error=drv_InputInit( handle);
	if(error!=HI_SUCCESS)
	{
		printf("%s: %d \n", __FILE__, __LINE__);
		return -1;
	}
	CallBack=MouseCallBack;
	
	pMouseThread = DRV_OS_TaskCreate("drv_MouseReceiveTask",drv_MouseReceiveTask,3,8096,NULL,0);
	if (pMouseThread == NULL)
	{
	    printf("%s: %d \n", __FILE__, __LINE__);
	    return -1;
	}
	return 0;
}