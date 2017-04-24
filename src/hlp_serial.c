/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>

//#include <hlp_serial.h>

static int onoff_flag = 0;
static int onoff_interval = 5;

static void hlp_switch_onoff()
{
    if(onoff_flag == 0)
        onoff_flag = 1;
}

void *serial_thread(void *args)
{
    int   i,len = 0;
    char  buffer[2048];
    char *ptr = NULL;

    hlp_power_switch_on();
    memset(buffer, 0, sizeof(buffer));
    printf("[Command #] ");
    fflush(stdout);

    while(fgets(buffer, sizeof(buffer), stdin))
    {
        /* do nothing, print bash */
        if(buffer[0] == '\n')
        {
            printf("[Command #] ");
            fflush(stdout);
            continue;
        }

        len = strlen(buffer);
        buffer[len-1] = '\0';
        len--;
        printf("recv command: %s\n", buffer);
        if(!strncmp(buffer, "onoff", 5))
        {
            /* switch_thread is busy */
            if(onoff_flag == 1)
            {
                printf("please wait...\n");
                continue;
            }

            /* no parameter */
            if(len == 5 || len == 6)
            {
                hlp_switch_onoff();
                continue;
            }

            /* parse power on/off interval */
            for(i=5; i<len; i++)
            {
                if(buffer[i] != ' ' && buffer[i] != '\t')
                    break;
            }
            if(i != len)
            {
                 onoff_interval = atoi(&buffer[i]);
                 if(onoff_interval == 0)
                     printf("error command!\n");
                 else
                    hlp_switch_onoff();
                 continue;
            }
        }
        /* other command */
        printf("error command!\n");
        printf("[Command #] ");
        fflush(stdout);
    }
}


void *switch_thread(void *args)
{
    static int count = 0;
    while(1)
    {
        /* free sleep.. */
        if(onoff_flag == 0)
        {
            usleep(300*1000);
            continue;
        }
        /* exec poweroff-->sleep(val)-->poweroff */
        printf("poweroff-->poweron, interval=%d, cnt[%d]\n", onoff_interval, count++);
        hlp_power_switch_off();
        sleep(onoff_interval);
        hlp_power_switch_on();
        onoff_flag = 0;
    }
}

int main(int argc, char **argv)
{
    int ret = -1;
    pthread_t switch_tid, serial_tid;

    hlp_gpio_init();
    printf("Enter switch control system v1.0!\n");
    if(pthread_create(&switch_tid, NULL, switch_thread, NULL) < 0)
        perror("pthread_create [switch_thread]error");
    if(pthread_create(&serial_tid, NULL, serial_thread, NULL) < 0)
        perror("pthread_create [serial_thread] error");

    pthread_join(serial_tid, NULL);
    pthread_join(switch_tid, NULL);

    return 0;
}




