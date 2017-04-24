#include "osddrv.h"
#include "drv_os.h"
/*
 * 关于存放: 除了jpeg.h是公共头文件, 可以放到其它地方供更它模块使用之外, 其它头文件都属于模块私有头文件,最好和c文件放在到一起,并以独立目录放置,便于版本维护.
 */

/*************************************************
* Function:       DRV_OSDLoadJpeg
* Description:   
* Input:          
* Output:        
* Return:         
* Others:       此函数仅为向前兼容而保留, 推荐使用DRV_BitmapLoadJpeg
*************************************************/
int DRV_OSDLoadJpeg (void *BufferBase,int BufferSize, STOSD_Bitmap_t* bmp,DRVOS_Partition_t *Partition);

/*************************************************
* Function:       DRV_BitmapLoadJpeg
* Description:   把一个bmp文件数据解成一张bitmap
* Input:          BufferBase            jpeg 文件数据buffer 基地址
*                 BufferSize             jpeg 文件数据buffer 大小
*                 ColorType             解码后所得到的bitmap的颜色类型
*                 目前只支持OSD_COLOR_TYPE_ARGB4444 和 OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422
* Output:        Bmp                      解码后所得到的bitmap 结构指针
* Return:         
* Others:        内存在system_partition 中分配
*        使用完需要手动释放内存. 释放方法
*      if(bitmap->Data_p != NULL)
*        {
*            DRV_OSFree(system_partition,bitmap->Data_p);
*            bitmap->Data_p = NULL;
*        }
*        DRV_OSFree(system_partition,bitmap);
*
*************************************************/
//int DRV_BitmapLoadJpeg (void *BufferBase,int BufferSize,OSD_ColorType_t ColorType, OSD_Bitmap_t** Bmp,DRVOS_Partition_t *Partition);
int DRV_BitmapLoadJpeg (void *BufferBase,int BufferSize,OSD_ColorType_t ColorType, OSD_Bitmap_t** Bmp);

int DRV_DrawJpegToBmpRect (void *BufferBase,int BufferSize,OSD_Bitmap_t *bitmap, OSD_Rect_t*Rect_p);

