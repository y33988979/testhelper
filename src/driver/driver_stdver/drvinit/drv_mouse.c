#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/input.h>

#include "drv_os.h"
#include "stddefs.h"
#include "drv_mouse.h"

#define MOUSE_EVENT_COUNT 10

int button = 0;
int realx = 0;
int realy = 0;
int offx = 0;
int offy = 0;
int typecode = 0;
static int event0_fd = -1;
struct input_event ev0[64];

//for handling event0, mouse/key/ts

static int handle_event0()
{
    int DevExist,i, rd, count;   
    realx = 0;
    realy = 0;
    offx = 0;
    offy = 0;
    typecode = 0;
    
    //DevExist = access(event0_fd,F_OK);
    //if(-1 == DevExist)
    //{
    //    return -1;
    //}
    
    rd = read(event0_fd, ev0, sizeof(struct input_event) * 64);
    //printf ("mUI_MouseRead read event0_fd  end!!!\n");
    if ( rd < sizeof(struct input_event) ) return -1;
    
    count = rd / sizeof(struct input_event);
    if(MOUSE_EVENT_COUNT < count)
    {
        if (ev0[0].type == 2 && ev0[0].code == 0)
            offx = ev0[count/2].value*MOUSE_EVENT_COUNT*2;   //滑动的X坐标相对位移
        else if (ev0[count/2].type == 2 && ev0[count/2].code == 1)
            offy = ev0[count/2].value*MOUSE_EVENT_COUNT*2;   //滑动的Y坐标相对位移 
        else if ((ev0[count/2].code == 8) && (ev0[count/2].type == 2))
        {
            if(ev0[4].value == 1)
            {
                typecode = 4;  //滑轮外移
            }
            else if(ev0[4].value == -1)
            {
                typecode = 3;   //滑轮内移
            }
        }
    }
    else
    {
    for (i = 0; i < count; i++) 
	{
        if (ev0[i].type == 3 && ev0[i].code == 0)
            realx = ev0[i].value;
        else if (ev0[i].type == 3 && ev0[i].code == 1)
            realy = ev0[i].value;
        else if (ev0[i].type == 2 && ev0[i].code == 0)
            offx += ev0[i].value;   //滑动的X坐标相对位移
        else if (ev0[i].type == 2 && ev0[i].code == 1)
            offy += ev0[i].value;   //滑动的Y坐标相对位移
        else if (ev0[i].type == 1)
		{
            if((ev0[i].code == 0x110) && (ev0[i].value == 0))
            {
                typecode = 1;   //鼠标左键
            }
            else if((ev0[i].code == 0x111) && (ev0[i].value == 0))
            {
                typecode = 2;   //鼠标右键
            }
            else if (ev0[i].code == 158) 
			{
                //if key esc then exit
                return -1;
            }
        }
        else if ((ev0[i].code == 8) && (ev0[i].type == 2))
        {
            if(ev0[i].value == 1)
            {
                typecode = 4;  //滑轮外移
            }
            else if(ev0[i].value == -1)
            {
                typecode = 3;   //滑轮内移
            }
        }
		else if (ev0[i].type == 0 && ev0[i].code == 0 && ev0[i].value == 0) 
		{
            realx = 0, realy = 0;
        }
        //printf("event(%d): type: %d; code: %3d; value: %3d; realx: %3d; realy: %3d; offx: %3d; offy: %3d\n", i,ev0[i].type, ev0[i].code, ev0[i].value, realx, realy, offx, offy);
    }
    }
    return 1;

}

int drv_MouseReceiveTask(void *args)
{
#if 0
	if (argc != 2)
	{
		printf ("Usage: ./event_data <eventN>\n	\
e.g.    ./touch_data /dev/input/event2\n");
		return -1;
	}
#endif	

	int done = 1;
    printf("sizeof(struct input_event) = %d\n", sizeof(struct input_event));
	
	char filename[128];
/*	int num = atoi(argv[1]);
	strcpy (filename, "/dev/input/event");
*/	
	//sprintf (filename, "%s", argv[1]);
	
    //event0_fd = open(filename, O_RDWR);
    //if ( event0_fd < 0 )
    //    return -1;
    while ( done )
	{
		//printf("begin handel_%s..\n", filename);
		done = handle_event0();
		//printf("end handel_%s...\n", filename);
    }
    if ( event0_fd > 0 )
	{
        close(event0_fd);
        event0_fd = -1;
    }

    return 0;
}

DRVOS_Task_t *pMouseThread;
int DRV_MouseOpen(void)
{
    char filename[128] = "/dev/mice";
/*	int num = atoi(argv[1]);
	strcpy (filename, "/dev/input/event");
*/	
	//sprintf (filename, "%s", argv[1]);
	
    event0_fd = open(filename, O_RDWR);
    if ( event0_fd < 0 )
    {
        printf ("open device  %s  error!!!\n", filename);	
        return -1;
    }

    return event0_fd;
}

void DRV_MouseClose(int handle)
{
    int ErrCode;
    
    if ( event0_fd > 0 )
	{
        close(event0_fd);
        event0_fd = -1;
    }
    
    return;
}
static int EventFlag = 0;
static int Event0Flag = 0;
static int Event1Flag = 0;

int DRV_MouseRead(U32 handle, S32 *type, S32 *xOff, S32 *yOff )
{
    int ErrCode;
    char filename0[16] = "/dev/event0";
    char filename1[16] = "/dev/event1";

    if(EventFlag == 0)
    {
        event0_fd = open(filename0, O_RDWR);
        if(event0_fd > 0)
        {
            usleep(100000);
            event0_fd = open(filename1, O_RDWR);
            if ( event0_fd < 0 )
            {
                //printf ("mUI_MouseRead open device  %s  success!!!\n", filename0);
                event0_fd = open(filename0, O_RDWR);
                Event0Flag = 1;
                Event1Flag = 0;
            }
            else
            {
                //printf ("mUI_MouseRead open device  %s  success!!!\n", filename1);
                Event0Flag = 0;
                Event1Flag = 1;
            }
            EventFlag = 1;
        }
        else
        {
            //printf ("mUI_MouseRead open device  event  error!!!\n");
            EventFlag = 0;
            return -1;
        }
    }
    else
    {
        if(Event0Flag == 1)
        {
            event0_fd = open(filename0, O_RDWR);
            if ( event0_fd < 0 )
            {
                //printf ("mUI_MouseRead open device  %s  error!!!\n", filename0);
                Event0Flag = 0;
                Event1Flag = 0;

                return -1;
            }
        }
        else if(Event1Flag == 1)
        {
            event0_fd = open(filename1, O_RDWR);
            if ( event0_fd < 0 )
            {
                //printf ("mUI_MouseRead open device  %s  error!!!\n", filename1);
                Event0Flag = 0;
                Event1Flag = 0;

                return -1;
            }
        } 
        else
        {
            EventFlag = 0;

            return -1;
        }
    }
    
    //printf ("mUI_MouseRead open device  event0  success!!!\n");	
    ErrCode = handle_event0();
    //printf ("mUI_MouseRead handle_event0  end!!!\n");
    if(ErrCode == 1)
    {
        *xOff = offx;
        *yOff = -offy;
        *type = typecode;
    }
    else 
    {
        printf("DRV_MouseRead error!!!"); 
        close(event0_fd);
        event0_fd = -1;
        return -1;
    }
    //printf("DRV_MouseRead Mouse info type = %d xoff = %d yoff = %d\n",*type,*xOff,*yOff);  

    if ( event0_fd > 0 )
	{
        close(event0_fd);
        event0_fd = -1;
    }
    //printf ("mUI_MouseRead out!!!\n");
    return 1;
}





