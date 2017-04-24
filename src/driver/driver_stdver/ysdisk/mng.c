#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <linux/hdreg.h>
#include <stddefs.h>

#include "ysdisk.h"
//#include <uuid/uuid.h>

#define DISK_MAX_SUPPORT 5
#define PART_MAX_SUPPORT 16

typedef unsigned long long ullong;

typedef struct diskid_s
{
    char id;
    char kernelname[8];
    DRV_DiskInfo diskinfo;
}diskid_t;

typedef struct partid_s
{
    char kernelname[8];
    char id;
    char kernelindex;
    char diskid;
    DRV_PartitionInfo partinfo;
}partid_t;

diskid_t disk[DISK_MAX_SUPPORT] = {0};
partid_t part[DISK_MAX_SUPPORT][PART_MAX_SUPPORT] = {0};

int get_sectorsize(int fd)
{
    int arg;
    if (ioctl(fd, BLKSSZGET, &arg) == 0)
        return arg;
}

static ullong bb_BLKGETSIZE(int fd)
{
	uint64_t v64;
	unsigned long longsectors;

	if (ioctl(fd, BLKGETSIZE64, &v64) == 0) {
		/* Got bytes, convert to 512 byte sectors */
		return (v64 >> 9);
	}
	/* Needs temp of type long */
	if (ioctl(fd, BLKGETSIZE, &longsectors))
		longsectors = 0;
	return longsectors;
}

static int get_start_offset(int fd)
{
	struct hd_geometry geometry;
	
	if (!ioctl(fd, HDIO_GETGEO, &geometry)) 
	{
        return geometry.start;
		/* never use geometry.cylinders - it is truncated */
	}

        return 0;
    }
static int disk_id = 1;

int adddisk(char* kernelname)
{
    char host_name[]="host";
    char usb_name[]="usb";    
    int i;
    char diskpath[24];
    int size;
    int fd;
    DRV_DiskInfo info;
    char linkname[200]={0};

    sprintf(diskpath,"/dev/%s",kernelname);
    fd = open(diskpath, O_RDONLY);
    if (fd < 0)
        return -1;
        
    size = bb_BLKGETSIZE(fd);
    info.Size = size;

    info.SectorSize =   get_sectorsize(fd);

    close(fd);

    info.Type = DRV_DISK_TYPE_HARDDISK;
#if 0
    sprintf(diskpath,"/sys/block/%s",kernelname);
	fd = readlink(diskpath,linkname,sizeof(linkname));
	if (fd < 1)
        return -1;

    i=5;
    char *s= linkname;
    while(i--)
    {
        s=strchr(s,'/');
        if(s == NULL)
            break;
        else
            s++;
        
    }
    s[-1]='\0';
    fd = strstr(linkname,host_name);
    if(fd != 0)
    {
        info.BusType = DRV_DISK_TYPE_SATA;
    }
    fd = strstr(linkname,usb_name);
    if(fd != 0)
    {
        info.BusType = DRV_DISK_TYPE_USB;
    }
#else
    info.BusType = DRV_DISK_TYPE_USB;
#endif    
    size = 0;
    int j=0;
    for(i=1;i<15;i++)
    {
        sprintf(diskpath,"/dev/%s%d",kernelname,i);
        fd = open(diskpath, O_RDONLY);
        if (fd < 0)
            continue;
        size = bb_BLKGETSIZE(fd);
        if(size != 2)
        {
            info.FreeSize += size;
            j++;
        }
        close(fd);
    }
    info.PartitionNb = j;
    info.FreeSize = info.Size - info.FreeSize;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == 0)
        {
            disk[i].id = disk_id;
            strcpy(disk[i].kernelname,kernelname);
            disk[i].diskinfo = info;
            break;
        }
    }
    disk_id++;
    return disk[i].id;
}

int getdiskindex(int diskid)
{
    int i;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == diskid)
        {
            return i;
        }
    }
    return -1;
}

void putdisk(int diskid)
{
    int i;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == diskid)
        {
            disk[i].id = 0;
            disk_id--;
            break;
        }
    }
    
}

int disknumber()
{
    int i;
    int j=0;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id != 0)
        {
            j++;
        }
    }
    return j;
}

char* getdiskkernelname(int diskid)
{
    int i;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == diskid)
        {
            return disk[i].kernelname;
        }
    }
    return NULL;
}

void setdiskinfo(int diskid,DRV_DiskInfo* info)
{
    int i;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == diskid)
        {
             disk[i].diskinfo = *info;
        }
    }
}

DRV_DiskInfo* getdiskinfo(int diskid)
{
    int i;
    for(i=0;i<DISK_MAX_SUPPORT; i++)
    {
        if(disk[i].id == diskid)
        {
            return &disk[i].diskinfo;
        }
    }
	return NULL;
}
static int part_number[DISK_MAX_SUPPORT] = {1,1,1,1,1};

int freesize(char* mount_dir)
{
	struct statfs fs_info;
	long long llsize, llblock, llbfree;

    statfs(mount_dir, &fs_info);
    llsize = fs_info.f_bsize;
    llblock = fs_info.f_blocks;
    llbfree = fs_info.f_bfree;
    return llsize * llbfree /1024;
}
int addpart(char* kernelname)
{
    char part_path[20];
    int i,j;
    int diskid;
    int fd;
    DRV_PartitionInfo info;
    char cmd[100];

    sprintf(part_path,"/dev/%s",kernelname);

	fd = open(part_path, O_RDONLY);
	if (fd < 0)
        return -1;
    info.Start = get_start_offset(fd);
    info.Size = bb_BLKGETSIZE(fd);
    info.SectorSize =   get_sectorsize(fd);
    close(fd);
    if(info.Size == 2)
        return -1;
    {
        char *label,*uuid;
        int rv;
        char *mount_path = info.MountPath;
        rv = get_label_uuid(part_path,&label,&uuid,0);
        if(rv == 0)
        {
            info.Formatted = TRUE;
            strncpy(info.Label,label,24);
            memcpy(info.Uuid,uuid,16);
            free(label);
            free(uuid);
/*            sprintf(cmd,"mkdir /media/%s",label);
            system_vfork(cmd);
            sprintf(cmd,"mount %s /media/%s",part_path,label);
            system_vfork(cmd);*/
            info.Logicnb = 'A'+ logicallocate();
            sprintf(mount_path,"/media/%c",info.Logicnb);
            sprintf(cmd,"mkdir %s",mount_path);
            system_vfork(cmd);
            sprintf(cmd,"mount %s %s",part_path,mount_path);
            system_vfork(cmd);
            info.FreeSize = freesize(mount_path);
        }
        else
        {
            printf("found not farmatted partiton[%s]\n",kernelname);
            info.Formatted = FALSE;
        }
    }

    /* find disk id*/
    for(i=0;i<DISK_MAX_SUPPORT;i++)
    {
        if(strstr(kernelname,disk[i].kernelname))
            break;
    }
    diskid = i+1;
    for(j=0;j<PART_MAX_SUPPORT; j++)
    {
        if(part[i][j].id == 0)
        {
            part[i][j].id = part_number[i];
            part[i][j].diskid = diskid;
            part[i][j].partinfo = info;
            strcpy(part[i][j].kernelname,kernelname);
            break;
        }
    }
    part_number[i]++;
    return part[diskid][i].id; 
}

void putpart(int diskid,int partid)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id == partid)
        {
            part[diskid][i].id = 0;
            part_number[diskid]--;
            break;
        }
    }
}

int partnumber(int diskid)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id != 0)
        {
            i++;
        }
    }
    return i;
}

int getpartdiskid(int diskid,int partid)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id == partid)
        {
            return part[diskid][i].diskid; 
        }
    }
    return 0;
}

void setpartinfo(int diskid,int partid,DRV_PartitionInfo* info)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id == diskid)
        {
             part[diskid][i].partinfo = *info;
        }
    }
}

DRV_PartitionInfo* getpartinfo(int diskid,int partid)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id == partid)
        {
            return &part[diskid][i].partinfo;
        }
    }
	return NULL;
}

char* getpartkernelname(int diskid,int partid)
{
    int i;
    diskid = getdiskindex(diskid);
    for(i=0;i<PART_MAX_SUPPORT; i++)
    {
        if(part[diskid][i].id == partid)
        {
            return part[diskid][i].kernelname;
        }
    }
    return NULL;
}

char abc[256]={0};

int logicallocate()
{
    int i;
    for(i = 0; i<256; i++)
    {
        if(abc[i]==0)
        {
            abc[i]=1;
            return i;
        }
    }
}

void logifree(int i)
{
    for(i = 0; i<256; i++)
    {
        if(abc[i]==1)
        {
            abc[i]=0;
        }
    }
}


#if 0
struct logicname
{
    uuid_t UUID;
    char allocate
}

int logicallocate(char* uuid)
{
    int i;
    for(i = 0; i<256; i++)
    {
        if(abc[i]==0)
        {
            abc[i]=1;
            return i;
        }
    }
}

void logifree(int i)
{
    for(i = 0; i<256; i++)
    {
        if(abc[i]==1)
        {
            abc[i]=0;
        }
    }
}
#endif
