#include <ysusb.h>
#include <ysvfs.h>
#include <errno.h>
#include <stddefs.h>
#include <sys/wait.h>   
//#include <sys/types.h>  
#include <fcntl.h>
usb_notify usbdevice;
callback app_cb;
callback in_cb;
callback mountok_cb;
callback mountfailed_cb;
char DEVINDEX=0;
char DEVNUM=0;
static char readtime=0;
char flag=0;
enum udev_monitor_netlink_group {
	UDEV_MONITOR_KERNEL	= 1,//鍖呮嫭add remove change绛夋秷鎭?	UDEV_MONITOR_UDEV	= 2,//鍙湁add remove娑堟伅
};
static int init_hotplug_sock(void)
{

    struct sockaddr_nl snl;
    const int buffersize = 16 * 1024 * 1024;
    int retval;
	int flags;
    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family = AF_NETLINK;
    snl.nl_pid = getpid();
    snl.nl_groups = UDEV_MONITOR_KERNEL;

    int hotplug_sock = socket(PF_NETLINK, SOCK_DGRAM, 15);
    if (hotplug_sock == -1)
    {
        printf("error getting socket: %s", strerror(errno));
        return -1;
    }

    /* set receive buffersize */

    setsockopt(hotplug_sock, SOL_SOCKET, 32, &buffersize, sizeof(buffersize));
    retval = bind(hotplug_sock, (struct sockaddr *) &snl, sizeof(struct sockaddr_nl));
    if (retval < 0) 
    {
        printf("bind failed: %s", strerror(errno));
        close(hotplug_sock);
        hotplug_sock = -1;
        return -1;
    }

	flags=fcntl(hotplug_sock,F_GETFL,0);
	fcntl(hotplug_sock,F_SETFL,flags|O_NONBLOCK);
	
    return hotplug_sock;

}

#define UEVENT_BUFFER_SIZE      2048
int aweigh_checkpanfu(char Panfu)
{
    int i,j;
    for(i =0;i < MAX_DEVICE;i ++)
    {
        if(usbdevice.devinfo[i].state != 0)
        {
            for(j = 0;j < usbdevice.devinfo[i].mountnum;j ++)
            {
                if(Panfu == usbdevice.devinfo[i].logicname[j][VOLUME_INDEX])
                {
                    return -1;
        
                }

            }

        }

    }
    return 0;
}
char aweigh_getpanfu()
{
    char panfu = 'c';
    {
        for(panfu = 'c';panfu <= 'z';panfu ++)  
        {
            if(aweigh_checkpanfu(panfu) == 0)
            {
                return panfu;
            }
        }
        return -1;
    }


}
/***********************************************
drv_SystemIsOK;
    判断system()函数中的命令或脚本是否执行成功
    return 1:执行成功
    return 0:执行失败

***********************************************/
U8 drv_SystemIsOK(pid_t status)
{  
    //pid_t status;   
    //status = system("./test.sh");  
  
    if (-1 == status)  
    {  
        printf("[drv_SystemIsOK]system error!\n");  
		return 0;
    }  
    else  
    {  
        printf("[drv_SystemIsOK]exit status value = [0x%x]\n", status);  
  
        if (WIFEXITED(status))  
        {  
            if (0 == WEXITSTATUS(status))  
            {  
                printf("[drv_SystemIsOK]run shell script successfully.\n");  
            }  
            else  
            {  
                printf("[drv_SystemIsOK]run shell script fail, script exit code: %d\n", WEXITSTATUS(status)); 
				return 0;
            }  
        }  
        else  
        {  
            printf("[drv_SystemIsOK]exit status = [%d]\n", WEXITSTATUS(status));  
			return 0;
        }  
    }  
  
    return 1;  
} 



void vfsmount()
{
    int i,externflag,j,num,mountfailed,externnum;
    char devname[MAX_VOLUME][32]={{0}};    //设备分区的逻辑名称存储
    char mountdir[32]={0};                 //设备分区挂载到的文件系统路径名称
    char tempname[MAX_VOLUME][32]={{0}};   //用来处理扩展分区的中间存储数组
    char aweigh = 0;
    DIR *fp;
    externflag=0;
    j=num=0;
    mountfailed=0;
	pid_t status; 
	char OrderName[128];
    
    if(usbdevice.state==1)
    {  
        //printf("strlen %d\n",strlen(usbdevice.devname[0]));
        if((usbdevice.devinfo[DEVINDEX].logicdev_num==1)) /*对只有一个分区的设备的处理*/
        {
            strcpy((char *)&devname[0],(char *)usbdevice.devinfo[DEVINDEX].logicname[0]);
			sprintf(mountdir,"/media/volume%d%d",DEVINDEX,usbdevice.devinfo[DEVINDEX].logicdev_num);
            printf("logicname %s\n",devname[0]);
            printf("mountdir %s\n",mountdir);
			
            if((fp=opendir(mountdir))==NULL)
            { 
                if(mkdir(mountdir,S_IRWXU|S_IRWXG|S_IRWXO )==0)   //路径不存在，就创建
                {
					printf("create dir %s success\n",mountdir);
				}
                else
                {
					printf("create dir failed\n");
				}   
            }
            else 
            {
                if(closedir(fp)==0)
                {
					printf("closedir\n");
				}
                else 
                {
                    printf("closedir failed\n");
				}
            }
			
            if(mount(devname[0],mountdir,"vfat",0xC0ED0000,"codepage=936,iocharset=gb2312,usefree")==0)
            {
                printf("mount vfat success\n");
				strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].logicdev_num-1],(char *)&mountdir);
                    
                aweigh = aweigh_getpanfu();
                usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num-1][VOLUME_INDEX] = aweigh;
				strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[0],(char *)&devname[0]);

                
                usbdevice.devinfo[DEVINDEX].mountnum++;
				usbdevice.mountnum++;
				if(mountok_cb)mountok_cb();
            }
            else 
            {
                if(mount(devname[0],mountdir,"ext2",0xC0ED0000,NULL)==0) 
                {  
                    printf("mount ext2 fs sucess\n");
			    	strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].logicdev_num-1],(char *)&mountdir);
                    aweigh = aweigh_getpanfu();
                    usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num-1][VOLUME_INDEX] = aweigh;
					strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[0],(char *)&devname[0]);
                    usbdevice.devinfo[DEVINDEX].mountnum++;
					usbdevice.mountnum++;
					if(mountok_cb)mountok_cb();
                }
                else  
                {  
					//if(mount(devname[0],mountdir,"ntfs",0xC0ED0000,"umask=0000,nls=gb2312")==0)   //挂载ntfs，只读方式
					sprintf(OrderName, "ntfs-3g -o locale=zh_CN.UTF-8 %s %s", devname[0],mountdir); 
				    status = system(OrderName);      //使用ntfs-3g挂载ntfs，可读写
					if(1 == drv_SystemIsOK(status))
                    {  
                        printf("mount ntfs fs sucess\n");
    			    	strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].logicdev_num-1],(char *)&mountdir);
                        aweigh = aweigh_getpanfu();
                        usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num-1][VOLUME_INDEX] = aweigh;
    					strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[0],(char *)&devname[0]);
                        usbdevice.devinfo[DEVINDEX].mountnum++;
    					usbdevice.mountnum++;
    					if(mountok_cb)mountok_cb();
                    }
                    else
                    {
    					if(mount(devname[0],mountdir,"msdos",0xC0ED0000,"usefree")==0)
    		            {
                            printf("mount fat16 fs sucess\n");
    			            strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].logicdev_num-1],(char *)&mountdir);
                            aweigh = aweigh_getpanfu();
                            usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num-1][VOLUME_INDEX] = aweigh;

    						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[0],(char *)&devname[0]);
                            usbdevice.devinfo[DEVINDEX].mountnum++;
    						usbdevice.mountnum++;
    				 		if(mountok_cb)mountok_cb();
    	                }
                        else  
                        {   
                            if(mount(devname[0],mountdir,"ext3",0xC0ED0000,"usefree")==0)
                            {
                                printf("mount ext3 fs sucess\n");
        			            strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].logicdev_num-1],(char *)&mountdir);
                                aweigh = aweigh_getpanfu();
                                usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num-1][VOLUME_INDEX] = aweigh;

        						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[0],(char *)&devname[0]);
                                usbdevice.devinfo[DEVINDEX].mountnum++;
        						usbdevice.mountnum++;
        				 		if(mountok_cb)mountok_cb();
    	                    }                          
                            else
                            {
	        					if(mountfailed_cb) mountfailed_cb();    
	        					printf("mount u failed\n");
	        				}
	                    }
				    }
	            }
	        }
        }
        else/*对含有多个分区的设备的处理*/
        {
            for(i=0;i<usbdevice.devinfo[DEVINDEX].logicdev_num;i++)
            {
                if(usbdevice.devinfo[DEVINDEX].logicname[i][8]=='5')
                {
                    externnum = i-1;	
                    externflag = 1;
                }
            }
            j=0;
            if(externflag == 1)//有扩展分区
            { 
                for(i=0;i<usbdevice.devinfo[DEVINDEX].logicdev_num;i++)
    			{
    				if(externnum!=i)
    				{  
    				    strcpy((char *)&tempname[j],(char *)usbdevice.devinfo[DEVINDEX].logicname[i]);
    				    j++;
    				}
    			}
                usbdevice.devinfo[DEVINDEX].logicdev_num=usbdevice.devinfo[DEVINDEX].logicdev_num-1;
                
                for(i=0;i<usbdevice.devinfo[DEVINDEX].logicdev_num;i++)
                {
                   memset(&(usbdevice.devinfo[DEVINDEX].logicname[i]),0,32);
                   strcpy((char *)usbdevice.devinfo[DEVINDEX].logicname[i],(char *)&tempname[i]);
                }
            }
			
            for(i=0;i<usbdevice.devinfo[DEVINDEX].logicdev_num;i++)
            {
                printf("dev name is %s\n",usbdevice.devinfo[DEVINDEX].logicname[i]);
                
    			strcpy((char *)&devname[i],(char *)usbdevice.devinfo[DEVINDEX].logicname[i]);
    			sprintf(mountdir,"/media/volume%d%d",DEVINDEX,i);
    			printf("mountdir is %s\n",mountdir);
                printf("logicname %s\n",devname[i]);
    			if((fp=opendir(mountdir))==NULL)
                { 
                    if(mkdir(mountdir,S_IRWXU|S_IRWXG|S_IRWXO )==0)
    	                printf("create dir %s success\n",mountdir);
                    else
                        printf("create dir failed\n");
                }
                else 
                {
                    if(closedir(fp)==0)
                        printf("closedir\n");
                    else 
                        printf("closedir failed\n");
                }


    			if(mount(devname[i],mountdir,"vfat",0xC0ED0000,"codepage=936,iocharset=gb2312,usefree")==0)
    	        {
                    printf("mount fat32 success\n");
    					strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&mountdir);
                        aweigh = aweigh_getpanfu();
                        usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].mountnum][VOLUME_INDEX] = aweigh;
    					strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&devname[i]);
                    usbdevice.devinfo[DEVINDEX].mountnum++;
                    usbdevice.mountnum++;
                    memset(mountdir,0,32);
    				if(mountok_cb)mountok_cb();

                }
                else 
                {
    				printf("result[%s]\n",strerror(errno));
                    if(mount(devname[i],mountdir,"ext2",0xC0ED0000,NULL)==0)
                    {
                        printf("mount ext2 fs sucess\n");
						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&mountdir);
                        aweigh = aweigh_getpanfu();
                        usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].mountnum][VOLUME_INDEX] = aweigh;

						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&devname[i]);
                        usbdevice.devinfo[DEVINDEX].mountnum++;
                        usbdevice.mountnum++;
                        memset(mountdir,0,32);
    					if(mountok_cb)mountok_cb();
                        
                    }
                    else  
                    {
        				printf("result[%s]\n",strerror(errno));
						//if(mount(devname[i],mountdir,"ntfs",0xC0ED0000,"umask=0000,nls=gb2312")==0) //挂载ntfs，只读方式
						sprintf(OrderName, "ntfs-3g -o locale=zh_CN.UTF-8 %s %s", devname[i],mountdir); 
					    status = system(OrderName);      //使用ntfs-3g挂载ntfs，可读写
						if(1 == drv_SystemIsOK(status))
						{
                            printf("mount ntfs fs sucess\n");
    						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&mountdir);
                            aweigh = aweigh_getpanfu();
                            usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].mountnum][VOLUME_INDEX] = aweigh;
    						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&devname[i]);
                            usbdevice.devinfo[DEVINDEX].mountnum++;
                            usbdevice.mountnum++;
                            memset(mountdir,0,32);
        					if(mountok_cb)mountok_cb();
                            
                        }
                        else
                        {
                            if(mount(devname[i],mountdir,"ext3",0xC0ED0000,NULL)==0)
                            {
                                printf("mount ext3 fs sucess\n");
        						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountdir[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&mountdir);
                                aweigh = aweigh_getpanfu();
                                usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].mountnum][VOLUME_INDEX] = aweigh;
        						strcpy((char *)usbdevice.devinfo[DEVINDEX].mountedlogname[usbdevice.devinfo[DEVINDEX].mountnum],(char *)&devname[i]);
                                usbdevice.devinfo[DEVINDEX].mountnum++;
                                usbdevice.mountnum++;
                                memset(mountdir,0,32);
            					if(mountok_cb)mountok_cb();
                            }
                            else
                            {
                            printf("mount disk failed\n");
            				if(mountfailed_cb) mountfailed_cb();
                            mountfailed++;
                        }
                        }

                    }
                }

            }
        }
        //printf("dev num is%d\n",usbdevice.devnum);
        usbdevice.state=0;
        printf("devnum =%d\n",usbdevice.devnum);
        printf("mountnum =%d\n",usbdevice.mountnum);
        //printf("state=%d\n",usbdevice.state); 
       printf("$$$$$$$$$$$$$$$$$$$@@@@@@@@@@@@@@DEVINDEX =%d\n",DEVINDEX);	
	printf("$$$$$$$$$$$$$$$$$$$@@@@@@@@@@@@@@DEVNUM =%d\n",DEVNUM);	
	printf("$$$$$$$$$$$$$$$$$$$@@@@@@@@@@@@@@usbdevice.devnum =%d\n",usbdevice.devnum);
	printf("$$$$$$$$$$$$$$$$$$$@@@@@@@@@@@@@@usbdevice.mountnum =%d\n",usbdevice.mountnum);
			
    }
	flag=0;
}	
/*
不采用字符串长度的判断，而采用字符串比较后缀的判断,暂不考虑超过26个字母
/block/sda
/block/sda/sda1
/block/sdb
/block/sdb/sdb1
/block/mmcblk1
/block/mmcblk1/mmcblk1p1
*/
BOOL drv_UsbCheckString(char *key,int keylen)
{
	char CmpBuf1[16] = {0};
	char CmpBuf2[16] = {0};
	char CmpBuf3[16] = {0};
	char CmpBuf4[25] = {0};
	memcpy(CmpBuf1,key+keylen-10,10);
	memcpy(CmpBuf2,key+keylen-15,15);
	memcpy(CmpBuf3,key+keylen-14,14);
	memcpy(CmpBuf4,key+keylen-24,24);
	if(memcmp(&CmpBuf1,"/block/sda",9)==0)
	{
		return TRUE;
	}
	else if(memcmp(&CmpBuf2,"/block/sda/sda1",9)==0)
	{
		return TRUE;
	}
	else if(memcmp(&CmpBuf1,"/block/sdb",9)==0)
	{
		return TRUE;
	}
	else if(memcmp(&CmpBuf2,"/block/sdb/sdb1",9)==0)
	{
		return TRUE;
	}
	else if(memcmp(&CmpBuf3,"/block/mmcblk1",13)==0)
	{
		return TRUE;
	}
	else if(memcmp(&CmpBuf4,"/block/mmcblk1/mmcblk1p1",13)==0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void checkdevice(void)
{
	if(readtime==1)
	{
		readtime=0;
		usbdevice.state=1;
		sprintf(usbdevice.devinfo[DEVINDEX].logicname[0],"%s",usbdevice.devinfo[DEVINDEX].devname);
		usbdevice.devinfo[DEVINDEX].logicdev_num++;	
		vfsmount();

	}

}
void *usbhotplug(void)
{
	int hotplug_sock       = init_hotplug_sock();
    int len,i,index,err;
    BOOL Ret;
    char rem[]="remove";
    char add[]="add";
    char mountdir[]="/media/volume";
    char sourcedir[]="/dev/";
	char *sda_add[]={"add@ /block/sda",
					"add@ /block/sda/sda1",	
					"add@ /block/sda/sda2",	
					"add@ /block/sda/sda3",	
					"add@ /block/sda/sda4",	
					};
	char *sda_remove[]={"remove@ /block/sda",
						"remove@ /block/sda/sda1",
						"remove@ /block/sda/sda2",
						"remove@ /block/sda/sda3",
						"remove@ /block/sda/sda4",
					};
    char temp[256]={0};
	char cmd[128]={0};
    char *key;
    size_t keylen;
    int finddev,findlog,num=0,a;
    DIR *fp;
	char *p=NULL;
	
	int fd;
	int fd1;
	char flag_state=0;
	char flag_mount=0;
	char flag_dev[20];
	char command[30];
	char flag_num=0;
	char flag_count=0;
	char flag_add=0;
	char flag_remove=0;

	readtime=0;
    while(1)
    {
        char buf[UEVENT_BUFFER_SIZE*2] = {0};
        size_t bufpos=0;
        size_t devnamepos=0;
        if(usbdevice.state==1&&flag_mount==1)
        {
			readtime=0;
            signal(SIGALRM,vfsmount);		
            	alarm(1);
		flag_mount=0;	
        }
	if(readtime==1&&usbdevice.state==0&&flag_mount==1)
		{

			signal(SIGALRM,checkdevice);		
		alarm(1);
		flag_mount=0;
		}
        len = recv(hotplug_sock, &buf, sizeof(buf), 0);
        key = &buf[bufpos];
        keylen = strlen(key);
	if(buf[keylen-1]=='a'||buf[keylen-2]=='a')
	{
		memset(&buf,0,sizeof(buf));  
	}	
		
	fd = open("/dev/sda", O_RDONLY);
	if(fd > 0)close(fd);
	if ((fd < 0)&&(flag_state==1)) 
	{
		flag_num=flag_count;
	
		flag_remove=1;
		flag_state=0;
		for(i=1;i<=flag_count;i++)
		{
			sprintf(command,"rm /dev/sda%d",i);	
			system(command);	
			printf(" %s\n",command);
		}
		
	}
	else if((fd > 0)&&(flag_state==0))
	{
		
		flag_state=1;
		flag_add=1;
		flag_count=0;
		for(i=1;i<16;i++)
		{	
			sprintf(flag_dev,"/dev/sda%d",i);
			
			fd1 = open(flag_dev, O_RDONLY);
			if( fd1 <0)
			{
				sprintf(command,"mknod /dev/sda%d b 8 %d",i,i);	
				system(command);	
				printf(" %s\n",command);
			}
			else
			{
				close(fd1);
			}

			fd1 = open(flag_dev, O_RDONLY);
			if( fd1 >0)
			{
				flag_count++;
				close(fd1);
			}
			else
			{	
				sprintf(command,"rm /dev/sda%d",i);	
				system(command);	
				printf(" %s\n",command);
				flag_num=flag_count;
				break;
			}	
		}
	
	}
	

	
	if ((fd>0)&&(flag_add==1))
	{
		strcpy((char *)buf,(char *)sda_add[flag_count-flag_num]);
		if(flag_num==0)flag_add=0;
		flag_num--;
	}
	if ((fd<0)&&(flag_remove==1))
	{
		strcpy((char *)buf,(char *)sda_remove[flag_num]);
		if(flag_num==0)flag_remove=0;
		flag_num--;
	}	

        key = &buf[bufpos];
        keylen = strlen(key);
        //printf("---------usbdevice.state is[%d] rem[%s] add[%s] buf[%s] %d\n",usbdevice.state,rem,add,buf,keylen);
        Ret = drv_UsbCheckString(key,keylen);
        if(Ret == FALSE) 
	{
		continue; 
	}
	
	//printf("#############buf=%s\n",buf);
	//printf("#############keylen=%d\n",keylen);	
	
        if(memcmp(&rem,&buf,6)==0)  //remove
    	{
    		printf("device is remove\n");
			//printf("buf is %d\n",strlen(buf));
	   		devnamepos=keylen-4;
    		key=&buf[devnamepos];
    		if(buf[devnamepos]=='/')  // is /sda
			{	
				for(i=0;i<MAX_DEVICE;i++)
				{
                   printf("usbdevice.devinfo[%d].devname=%s,buf[devnamepos+1]=%s\n",i,usbdevice.devinfo[i].devname,&buf[devnamepos+1]);
				   p=strstr(usbdevice.devinfo[i].devname,&buf[devnamepos+1]);
				   if(p)
				   {
						if(strlen((char *)usbdevice.devinfo[i].logicname)==8)
						{
							if(umount(usbdevice.devinfo[i].mountdir[0])==0)
								printf("umount %s success\n",usbdevice.devinfo[i].mountdir[0]);
						}
                      DEVINDEX=i;
					  usbdevice.devinfo[i].state=0;
					  DEVNUM--;
					  printf("find dev\n");
					  memset(&(usbdevice.devinfo[i]),0,sizeof(dev_info));
					  if(DEVNUM<=0)
					  {
                          DEVINDEX=0;
						  DEVNUM=0;
						  memset(&usbdevice,0,sizeof(usb_notify));
					  }
					usbdevice.devnum=DEVNUM;//设备数没有减少导致错误
					  if(app_cb) app_cb(); 
					  break;
				   }
				}
				
    		}
			else if((buf[devnamepos]=='s') && (buf[devnamepos - 1]=='/'))  // is /sda1
			{
                finddev=0;
				findlog=0;
				for(i=0;i<MAX_DEVICE;i++)
				{
                   if(strncmp(&(usbdevice.devinfo[i].devname[5]),&buf[devnamepos],3)==0)
				   {
                      num=i;
					  finddev=1;
					  break;
					  
				   }
				}
				if(finddev==1)
				{
					index=buf[keylen-1]-48;
					for(i=0;i<MAX_VOLUME;i++)
					{
						if(strncmp(&(usbdevice.devinfo[num].logicname[i][5]),&buf[devnamepos],4)==0)
					   {
						  findlog=1;
						  break;
						  
					   }

					}
                    if(findlog==0) continue;
					sync();
					if(usbdevice.devinfo[num].logicdev_num==0)
						  continue;
					if(index>4) index=index-1;
					if(strlen(usbdevice.devinfo[num].mountdir[index-1])==0)
						continue;
    	         	if(umount(usbdevice.devinfo[num].mountdir[index-1])!=0)
	    	    	{
	                    printf("umount %s failed\n",usbdevice.devinfo[num].mountdir[index-1]);
						sprintf(cmd," umount -l %s",usbdevice.devinfo[num].mountdir[index-1]);	
						printf("%s\n",cmd);
						a = system_vfork(cmd);
						if(a != 0)
						{
							printf("excute [%s]failed \n",cmd);
							//return -1;
						}
						else
						{
							printf("excute [%s]success \n",cmd);
						}
						
					}
					else
					{		
					 printf("umount %s success\n",usbdevice.devinfo[num].mountdir[index-1]);
				    usbdevice.mountnum--;
					}	
					usbdevice.devinfo[num].logicdev_num--;
				}
    		}
#ifdef EMMC_FLASH
	        num=0;
	    }
#else
			else   // is SD 卡
			{
				devnamepos=keylen-8;
				key=&buf[devnamepos];
				if(buf[devnamepos]=='/')  // is /mmcblk1
				{	
					for(i=0;i<MAX_DEVICE;i++)
					{
			           printf("usbdevice.devinfo[%d].devname=%s,buf[devnamepos+1]=%s\n",i,usbdevice.devinfo[i].devname,&buf[devnamepos+1]);
					   p=strstr(usbdevice.devinfo[i].devname,&buf[devnamepos+1]);
					   if(p)
					   {
							if(strlen((char *)usbdevice.devinfo[i].logicname)==12)
							{
								if(umount(usbdevice.devinfo[i].mountdir[0])==0)
									printf("umount %s success\n",usbdevice.devinfo[i].mountdir[0]);
							}
							DEVINDEX=i;
							usbdevice.devinfo[i].state=0;
							DEVNUM--;
							printf("find dev\n");
							memset(&(usbdevice.devinfo[i]),0,sizeof(dev_info));
							if(DEVNUM<=0)
							{
								DEVINDEX=0;
								DEVNUM=0;
								memset(&usbdevice,0,sizeof(usb_notify));
							}
							if(app_cb)
							{
								app_cb(); 
							} 
							break;
					   }
					}
					
	    		}
				else if((buf[devnamepos]=='m') && (buf[devnamepos - 1]=='m'))  // is /mmcblk1p1
				{
					devnamepos = devnamepos - 1;
	                finddev=0;
					findlog=0;
					for(i=0;i<MAX_DEVICE;i++)
					{
						if(strncmp(&(usbdevice.devinfo[i].devname[5]),&buf[devnamepos],6)==0) //6 :对比mmcblk
						{
							num=i;
							finddev=1;
							break; 
						}
					}
					if(finddev==1)
					{
						index=buf[keylen-1]-48;   
						for(i=0;i<MAX_VOLUME;i++)
						{
							if(strncmp(&(usbdevice.devinfo[num].logicname[i][5]),&buf[devnamepos],9)==0)  // /dev/***与mmcblklp1 
							{
								findlog=1;
								break; 
							}
						}
	                    if(findlog==0)
	                    {
							continue;
						}
						
						sync();
						
						if(usbdevice.devinfo[num].logicdev_num==0)
							  continue;
						if(index>4) index=index-1;
						if(strlen(usbdevice.devinfo[num].mountdir[index-1])==0)
							continue;
	    	         	if(umount(usbdevice.devinfo[num].mountdir[index-1])!=0)
		    	    	{
		                    printf("umount %s failed\n",usbdevice.devinfo[num].mountdir[index-1]);
							sprintf(cmd," umount -l %s",usbdevice.devinfo[num].mountdir[index-1]);	
							printf("%s\n",cmd);
							a = system_vfork(cmd);
							if(a != 0)
							{
								printf("excute [%s]failed \n",cmd);
								//return -1;
							}
							else
							{
								printf("excute [%s]success \n",cmd);
							}
							
						}
						else
						{		
							printf("umount %s success\n",usbdevice.devinfo[num].mountdir[index-1]);
							usbdevice.mountnum--;
						}	
						usbdevice.devinfo[num].logicdev_num--;
					}
	    		}
			}
			num=0;
    	}
#endif  //eMMC or NAND flash
        else if (memcmp(&add,&buf,3)==0)  // is add
    	{
    		flag_mount=1;
    		devnamepos=keylen-4;
			readtime++;	
    		if(buf[devnamepos]=='/') 
    		{	
			if(flag==1)
			{
				sleep(3);
			}
			flag=1;
				strcpy((char *)&temp,(char *)&sourcedir);
				strcat((char *)&temp,(char *)&buf[devnamepos+1]);
				for(i=0;i<MAX_DEVICE;i++)
				{
					if(usbdevice.devinfo[i].state==0)
					{   
						usbdevice.devinfo[i].state=1;
						break;
					}
				}
				DEVINDEX=i;
				sprintf(usbdevice.devinfo[DEVINDEX].devname,"%s",temp);
				printf("add  %s \n",usbdevice.devinfo[DEVINDEX].devname);
				DEVNUM++;
				usbdevice.devnum=DEVNUM;
				if(in_cb) in_cb();
			}
			else if((buf[devnamepos]=='s') && (buf[devnamepos - 1]=='/'))
			{	
               key=&buf[devnamepos];
			   strcpy((char *)&temp,(char *)&sourcedir);
    		   strcat((char *)&temp,key);
			   sprintf(usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num],"%s",temp);
			   usbdevice.devinfo[DEVINDEX].logicdev_num++;
			   usbdevice.state=1;
			printf("@@@@@@@@@@@@@@DEVINDEX =%d\n",DEVINDEX);	
			printf("@@@@@@@@@@@@@@DEVNUM =%d\n",DEVNUM);	
			printf("@@@@@@@@@@@@@@usbdevice.devnum =%d\n",usbdevice.devnum);
			printf("@@@@@@@@@@@@@@usbdevice.mountnum =%d\n",usbdevice.mountnum);
			   
			}

#ifdef EMMC_FLASH
			memset(&temp,0,sizeof(temp));  		
		}
#else
			else
			{
	    		devnamepos=keylen-8;
				readtime++;	
	    		if(buf[devnamepos]=='/') 
	    		{	
					strcpy((char *)&temp,(char *)&sourcedir);
					strcat((char *)&temp,(char *)&buf[devnamepos+1]);
					for(i=0;i<MAX_DEVICE;i++)
					{
						if(usbdevice.devinfo[i].state==0)
						{   
							usbdevice.devinfo[i].state=1;
							break;
						}
					}
					DEVINDEX=i;
					sprintf(usbdevice.devinfo[DEVINDEX].devname,"%s",temp);
					printf("add  %s \n",usbdevice.devinfo[DEVINDEX].devname);
					DEVNUM++;
					usbdevice.devnum=DEVNUM;
					if(in_cb) in_cb();
				}
				else if((buf[devnamepos]=='m') && (buf[devnamepos - 1]=='m'))
				{
					devnamepos = devnamepos - 1;
					key=&buf[devnamepos];
					strcpy((char *)&temp,(char *)&sourcedir);
					strcat((char *)&temp,key);
					sprintf(usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num],"%s",temp);
					usbdevice.devinfo[DEVINDEX].logicdev_num++;
					usbdevice.state=1; 
				}
			}
			printf("@@@@@@@@@@@@@@DEVINDEX =%d\n",DEVINDEX);	
			printf("@@@@@@@@@@@@@@DEVNUM =%d\n",DEVNUM);	
			printf("@@@@@@@@@@@@@@usbdevice.devnum =%d\n",usbdevice.devnum);
			printf("@@@@@@@@@@@@@@usbdevice.mountnum =%d\n",usbdevice.mountnum);
			
			//printf("usbdevice.devname[%d-1] is %s\n",usbdevice.devname[num-1]);
			memset(&temp,0,sizeof(temp));    		
		}
#endif	//eMMC or NAND flas	   
    }

    return 0;
}

int DRV_UsbInit(void)
{
	pthread_t ntid;
	int err,num,i,j,k,ready;
    FILE *fp;
    char line[50], ptname[50];
    char name[]="/dev/";
	char volumename[16][16];
	int ma, mi, sz;
	
	ready=j=num=0;
	
	for(i=0;i<16;i++)
	{
		memset(&volumename[i],0,16);
	}
		
    fp=fopen("/proc/partitions", "r");
    while (fgets(line, sizeof(line), fp)) 
    {
		if (sscanf(line, " %d %d %d %[^\n ]",&ma, &mi, &sz, ptname) != 4) //179  0  30400  mmcblk0
		{
			continue;
		}
			
        printf("ptname[%s]\n",ptname);

#ifdef EMMC_FLASH
		if((ptname[0]=='s' && ptname[1]=='d'&& ptname[2]!='a'))
#else
		if((ptname[0]=='s' && ptname[1]=='d'&& ptname[2]!='a') || (ptname[0]=='m' && ptname[1]=='m' && ptname[2]=='c' & ptname[3]=='b' && ptname[4]=='l' && ptname[5]=='k'))//????SD????支??
#endif
		{ 
			strcpy((char *)&volumename[j],(char *)&ptname);
			//printf("-->>>>>>>>>>>>>>>>ptname[%s]\n",ptname);
			j++;
		}
    }
	
	for(k=0;k<j;k++)
	{
		if((strlen((char *)&volumename[k])==3) ||   // for /dev/sda
		    (strlen((char *)&volumename[k])==7))     //for /dev/mmcblk0
		{
			if(ready==1) //判断是否要设置逻辑名，并挂载分区
			{	
				if(usbdevice.devinfo[DEVINDEX].logicdev_num==0)
				{
					usbdevice.state=1;
					sprintf(usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num],"%s",usbdevice.devinfo[DEVINDEX].devname);
					usbdevice.devinfo[DEVINDEX].logicdev_num++;
				}
				usbdevice.devinfo[DEVINDEX].state = 1;
				vfsmount();
				ready=0;
			}
			//printf("######################\n");
			for(i=0;i<MAX_DEVICE;i++)
			{
                if(strlen(usbdevice.devinfo[i].devname) == 0)
				{   
					break;
				}
			}
			DEVINDEX=i;
			sprintf(usbdevice.devinfo[DEVINDEX].devname,"%s%s",name,volumename[k]);
			printf("usbdevice.devinfo[%d].devname=%s\n",DEVINDEX,usbdevice.devinfo[DEVINDEX].devname);
			DEVNUM++;
			usbdevice.devnum=DEVNUM;
			ready=1;   //确定是否要设置逻辑名，并挂载分区

		}	
		else if((strlen((char *)&volumename[k])==4) ||   //for /dev/sda1
			     (strlen((char *)&volumename[k])==9))     //for /dev/mmcblk0p1
		{	

			sprintf(usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num],"%s%s",name,volumename[k]);
			printf("usbdevice.devinfo[DEVINDEX].logicdev_num=%d\n",usbdevice.devinfo[DEVINDEX].logicdev_num);
			printf("usbdevice.devinfo[DEVINDEX].logicname[logicdev_num]=%s\n",usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num]);
			usbdevice.devinfo[DEVINDEX].logicdev_num++;
			usbdevice.state=1;

		}

		//printf("usbdevice.devname[%d-1] is %s\n",usbdevice.devname[num-1]);
	}
	
	if(ready==1)
	{	
		if(usbdevice.devinfo[DEVINDEX].logicdev_num==0)
		{
			usbdevice.state=1;
			sprintf(usbdevice.devinfo[DEVINDEX].logicname[usbdevice.devinfo[DEVINDEX].logicdev_num],"%s",usbdevice.devinfo[DEVINDEX].devname);
			usbdevice.devinfo[i].logicdev_num++;
		}
		usbdevice.devinfo[DEVINDEX].state = 1;
		vfsmount();
		ready=0;
	}
	
	fclose(fp);
	//printf("devnun is %d\n",usbdevice.devnum);
	// vfsmount();
	err=pthread_create(&ntid,NULL,usbhotplug,NULL);
	if(err != 0)
    {
        printf("can't create thread: %s\n",strerror(err));
        return 1;
    }
     return 0;
}
/*int main(int argc, char* argv[])
{
    int hotplug_sock       = init_hotplug_sock();
    int len,n;
    char rem[]="remove";
    char add[]="add";
    char mountdir[]="/mnt/volume1";
    char sourcedir[]="/dev/sdb1";
    char temp[256]={0};
    char *key;
    size_t keylen;
    while(1)
    {
        char buf[UEVENT_BUFFER_SIZE*2] = {0};
        size_t bufpos=0;
        size_t devnamepos=0;
        n=0;
        len = recv(hotplug_sock, &buf, sizeof(buf), 0);
        //printf("len is %d\n",len);
        key = &buf[bufpos];
        keylen = strlen(key);
        if(keylen>22) continue;
        printf("keylen=%d\n",keylen);
        if(memcmp(&rem,&buf,6)==0)
        	{
        		printf("device is remove\n");
        		printf("%s\n", key);
        		}
        else if (memcmp(&add,&buf,3)==0)
        	{
        		printf("device is add\n");	
        		devnamepos=keylen-4;
        		//printf("%s\n", key);
        		key=&buf[devnamepos];
        		printf("%s\n", key);
        		if(buf[devnamepos]=='/') continue;
        		strcpy(&temp,&sourcedir);
        		strcat(&temp,key);
        		//mountdir[11]=buf[keylen-1];
        		printf("sourcedir is %s\n",temp);
        		printf("mountdir is %s\n",mountdir);
        		if(mount(sourcedir,mountdir,"vfat",0xC0ED0000,NULL)==0)
   	        {
   		          printf("mount success\n");
   		      }
   		      else
   			    printf("mount failed\n");
        		memset(&temp,0,sizeof(temp));
        		
       }
       if(len > 0)
        {
            printf("-----------------------------------------------------------------------------------------------\n");
            while(bufpos < sizeof(buf))
            {
                char *key;
                size_t keylen;

                key = &buf[bufpos];
                keylen = strlen(key);
                 printf("keylen=%d\n", keylen);
                if (keylen == 0)
                    break;
                bufpos += keylen + 1;
                printf("%s\n", key);
            }
        }
    }

    return 0;
}
*/
void unregister_app_plugout_callback()
{
    app_cb = NULL;
}

void register_app_plugout_callback(callback cb)
{
    app_cb = cb;
}
void unregister_app_plugin_callback()
{
    in_cb = NULL;
}

void register_app_plugin_callback(callback cb)
{
    in_cb = cb;
}
void unregister_app_mountok_callback()
{
    mountok_cb = NULL;
}

void register_app_mountok_callback(callback cb)
{
    mountok_cb = cb;
}
void unregister_app_mountfailed_callback()
{
    mountfailed_cb = NULL;
}

void register_app_mountfailed_callback(callback cb)
{
    mountfailed_cb = cb;
}
