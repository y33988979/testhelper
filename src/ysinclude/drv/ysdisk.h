#ifndef _YSDISK_H_
#define _YSDISK_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif
#include <drv_typedef.h>

typedef enum
{
    DRV_DISK_TYPE_HARDDISK,
    DRV_DISK_TYPE_CDOM,
    DRV_DISK_TYPE_DVDROM,
}DRV_DiskType;

typedef enum
{
    DRV_DISK_TYPE_IDE,
    DRV_DISK_TYPE_SATA,
    DRV_DISK_TYPE_USB,
}DRV_DiskBusType;



typedef struct DRV_DiskInfo_s
{
    DRV_DiskType Type;
    DRV_DiskBusType BusType;
    U32_YS SectorSize;
    U32_YS Size; // sector
    U32_YS FreeSize;  //sector
    U32_YS PartitionNb;
}DRV_DiskInfo;

typedef struct DRV_PartitionInfo_s
{
    U32_YS SectorSize;
    U32_YS Start; // 
    U32_YS Size;  //
    BOOL_YS Formatted;
//以下信息只有有文件系统以及被mount上之后才�
    U32_YS Logicnb;
    U8_YS  Label[24];
    U8_YS  Uuid[16];
    U32_YS FreeSize; //KB
    U8_YS  MountPath[80];
}DRV_PartitionInfo;


U32_YS DRV_DiskGetDiskNumber(U32_YS *Number);
U32_YS DRV_DiskGetPartitionNumber(U32_YS DiskNb,U32_YS* Number);
U32_YS DRV_DiskGetPartitionInfo(U32_YS DiskNb, U32_YS PartitionID,DRV_PartitionInfo* Info_p);
U32_YS DRV_DiskGetDiskInfo(U32_YS DiskNb,DRV_DiskInfo* Info_p);
U32_YS DRV_DiskAddPartiton(U32_YS Disk,DRV_PartitionInfo* Info);
U32_YS DRV_DiskDeletePartiton(U32_YS DiskNb,U32_YS PartitionID);

#ifdef __cplusplus
}
#endif
#endif
