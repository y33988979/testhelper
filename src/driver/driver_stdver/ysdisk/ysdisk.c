/*****************************************************************************
*
* Description: 
* 
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2004-03-17    yzb            
* 2. 2005-10-12    Qiujordan      
* 3. 2005-12-27    Qiujordan      
*
*****************************************************************************/
#define CURRENT_MODULE DRV_MODULE_DISK

/*** LOCAL VARIABLES *********************************************************/
#include <sys/types.h>
#include <mntent.h>

#include <stddef.h>
#include <asm/ioctls.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/hdreg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

#include "mng.h"
#include "ysdisk.h"
#include "drv_os.h"
#include <sys/wait.h>

#include "drv_basic.h"

typedef unsigned long long ullong;

int checkdiskexistence()
{
	FILE *procpt;
	char line[100], ptname[100];
	int ma, mi, sz;
    int disk = 0;

	procpt = fopen("/proc/partitions", "r");
    if(procpt == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_OPEN_FILE);
    }
	while (fgets(line, sizeof(line), procpt)) {
		if (sscanf(line, " %d %d %d %[^\n ]",
				&ma, &mi, &sz, ptname) != 4)
			continue;
        if(!(ptname[0]=='s' && ptname[1]=='d'))
            continue;
        disk++;
	}
	fclose(procpt);
    return disk;
}

U32_YS DRV_DiskCheckDiskReady(void)
{
    unsigned int starttime;
    unsigned int timenow;

    starttime = DRV_OS_TimeNow();
    while(!checkdiskexistence())
    {
        printf("check failed,retry\n");
        DRV_OS_TaskDelay(50);
        timenow = DRV_OS_TimeNow();
        if ((timenow-starttime)>10000)
            return -1;
    }
    DRV_OS_TaskDelay(500);
    return 0;
}


U32_YS DRV_DiskInit(void)
{
	FILE *procpt;
	char line[100], ptname[100];
	int ma, mi, sz;

	procpt = fopen("/proc/partitions", "r");
    if(procpt == NULL)
    {
        YSDRIVER_ERRORCODE_RETURN(DRV_ERROR_OPEN_FILE);
    }

	while (fgets(line, sizeof(line), procpt)) 
	{
		if (sscanf(line, " %d %d %d %[^\n ]",
				&ma, &mi, &sz, ptname) != 4)
		{	
			continue;
		}

        printf("ptname[%s]\n",ptname);

        if(!(ptname[0]=='s' && ptname[1]=='d'))
            continue;
		
		if (isdigit(ptname[3]))
        {
            addpart(ptname);
        }
        else
        {
            adddisk(ptname);
        }
	}
	
	fclose(procpt);
    return 0;
}


U32_YS DRV_DiskTerm()
{
    DRV_PartitionInfo info;
    U32_YS disknumber,partnumber;
    int i,j;
    char cmd[100];

    DRV_DiskGetDiskNumber(&disknumber);
    
    for(i= 0; i< disknumber; i++)
    {
        DRV_DiskGetPartitionNumber(i+1,&partnumber);
        for(j=0; j<partnumber; j++)
        {
            DRV_DiskGetPartitionInfo(i+1,j+1,&info);
            if(info.Formatted == TRUE)
            {
                int a;
                sprintf(cmd,"/bin/umount /media/%c",info.Logicnb);
                a = system_vfork(cmd);
                if(a == -1)
                {
                    printf("excute [%s] result[%s]\n",cmd,strerror(errno));
                }
                else
                {
                    printf("excute [%s] result[%d]\n",cmd,WEXITSTATUS(a));
                }
                logifree(info.Logicnb -'A');
            }
            putpart(i+1,j+1);
        }
        putdisk(i+1);
        
    }
    return 0;
}

U32_YS DRV_DiskGetDiskNumber(U32_YS *Number)
{
    *Number =  disknumber();
	return DRV_NO_ERROR;
}

U32_YS DRV_DiskGetPartitionNumber(U32_YS DiskNb,U32_YS* Number)
{
    DRV_DiskInfo* info;
    info = getdiskinfo(DiskNb);
    *Number = info->PartitionNb;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}

U32_YS DRV_DiskGetDiskInfo(U32_YS DiskNb,DRV_DiskInfo* Info_p)
{
    DRV_DiskInfo* info;
    info = getdiskinfo(DiskNb);
    *Info_p = *info;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}

U32_YS DRV_DiskGetPartitionInfo(U32_YS DiskNb, U32_YS PartitionID,DRV_PartitionInfo* Info_p)
{
    DRV_PartitionInfo *info;
    info = getpartinfo(DiskNb,PartitionID);
    info->FreeSize = freesize(info->MountPath);
    *Info_p = *info;
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}

U32_YS DRV_DiskDeletePartiton(U32_YS DiskNb,U32_YS PartitionID)
{
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}



U32_YS DRV_DiskAddPartiton(U32_YS Disk,DRV_PartitionInfo* Info)
{
    char cmd[100];
    int a;
    sprintf(cmd,"fdisk -a %d %d /dev/%s",Info->Start,Info->Size,getdiskkernelname(Disk));
    a = system_vfork(cmd);
    if(a == -1)
    {
        printf("excute [%s] result[%s]\n",cmd,strerror(errno));
    }
    else
    {
        printf("excute [%s] result[%d]\n",cmd,WEXITSTATUS(a));
    }
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}

//let uuid auto generate
U32_YS DRV_DiskFormatPartiton(U32_YS DiskNb,U32_YS PartitionID,char* Label)
{
    char cmd[100];
    int a;
    sprintf(cmd,"mke2fs -t ext3 /dev/%s",getpartkernelname(DiskNb,PartitionID));
    printf("%s\n",cmd);
    a = system_vfork(cmd);
    if(a == -1)
    {
        printf("excute [%s] result[%s]\n",cmd,strerror(errno));
    }
    else
    {
        printf("excute [%s] result[%d]\n",cmd,WEXITSTATUS(a));
    }
    
    YSDRIVER_ERRORCODE_RETURN(DRV_NO_ERROR);        
}

