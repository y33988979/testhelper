#include "osddrv.h"
#include "drv_os.h"
/*
 * ���ڴ��: ����jpeg.h�ǹ���ͷ�ļ�, ���Էŵ������ط�������ģ��ʹ��֮��, ����ͷ�ļ�������ģ��˽��ͷ�ļ�,��ú�c�ļ����ڵ�һ��,���Զ���Ŀ¼����,���ڰ汾ά��.
 */

/*************************************************
* Function:       DRV_OSDLoadJpeg
* Description:   
* Input:          
* Output:        
* Return:         
* Others:       �˺�����Ϊ��ǰ���ݶ�����, �Ƽ�ʹ��DRV_BitmapLoadJpeg
*************************************************/
int DRV_OSDLoadJpeg (void *BufferBase,int BufferSize, STOSD_Bitmap_t* bmp,DRVOS_Partition_t *Partition);

/*************************************************
* Function:       DRV_BitmapLoadJpeg
* Description:   ��һ��bmp�ļ����ݽ��һ��bitmap
* Input:          BufferBase            jpeg �ļ�����buffer ����ַ
*                 BufferSize             jpeg �ļ�����buffer ��С
*                 ColorType             ��������õ���bitmap����ɫ����
*                 Ŀǰֻ֧��OSD_COLOR_TYPE_ARGB4444 �� OSD_COLOR_TYPE_UNSIGNED_YCBCR888_422
* Output:        Bmp                      ��������õ���bitmap �ṹָ��
* Return:         
* Others:        �ڴ���system_partition �з���
*        ʹ������Ҫ�ֶ��ͷ��ڴ�. �ͷŷ���
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

