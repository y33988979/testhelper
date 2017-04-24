#ifndef __GIF_H__
#define __GIF_H__

#include "driver.h"
/* ID_GIF 的值
 * -1     无效GIF
 * 其它    有效GIF. */

/* 此版本实现不再区分单帧和多帧，单帧也按多帧处理 */

/* 使用 场景*/
/* 括号为可选 */
/* DRV_NewGIF  (DRV_PauseGIF) (DRV_ChangeGIF) (DRV_ResumeGIF)  DRV_FreeGIF*/
/* DRV_LoadGIF  DRV_DrawGIF (DRV_PauseGIF) (DRV_ResumeGIF)  DRV_FreeGIF*/

#define  DRVOS_Partition_p DRVOS_Partition_t*
typedef  int ID_GIF;
typedef struct GIF_STRUCT_s GIF_t; 
/*******************************************************************************
Name        : DRV_NewGIF
Description : 创建gif对象
Parameters  : BufferBase     GIF文件内存基地址
              BufferSize     GIF文件内存大小
              PositionX      GIF要显示的区域横坐标
              PoistionY      GIF要显示的区域纵坐标
              Region         GIF所要依附的region handle
              pPartition     内存来源,为NULL时,来源为system partition
Assumptions : 在该GIF对象的生命周期内，要保证GIF文件buffer的数据不会被修改. 此函数返回时，会显示gif第一帧。
Limitations : none
Returns     :   0                      错误
                    other                正确
*******************************************************************************/

ID_GIF DRV_NewGIF(void* BufferBase,int BufferSize,int PositionX,int PoistionY,OSD_Region_t* Region,DRVOS_Partition_p pPartition);

/*******************************************************************************
Name        : DRV_NewGIF
Description : 释放gif对象及资源
Parameters  : GIF ID 
Assumptions :
Limitations :  
Returns     : none
*******************************************************************************/
void DRV_FreeGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_NewGIF
Description : 暂停gif显示
Parameters  : GIF ID
Assumptions :   本函数基本上能即时地暂停gif显示，无需额外等待
Limitations :    
Returns     : none
*******************************************************************************/
void DRV_PauseGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_ResumeGIF
Description : 恢复gif显示
Parameters  : GIF ID
Assumptions :   本函数基本上能即时地显动gif显示，但gif重新解码需要时间，
                        故实际上gif会在调用此函数之后一段时间内显恢复显示
Limitations :    
Returns     : none
*******************************************************************************/
int DRV_ResumeGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_ChangeGIF
Description : 改变gif对象中实际gif图片
Parameters  : GIF ID
Assumptions :   不涉及底层资源的操作，只是重设解码器，故适用于gif的快速切换
Limitations :   要求新gif的宽和高与原来的相同. 
Returns     : none
*******************************************************************************/
int DRV_ChangeGIF(ID_GIF gifID, void* BufferBase,int BufferSize);

/*******************************************************************************
Name        : DRV_LoadGIF
Description : 新建gif对象,并解出第一帧.
Parameters  : 
Assumptions :   
Limitations :    
Returns     : NULL    失败
		      other   成功
*******************************************************************************/
GIF_t* DRV_LoadGIF(void* BufferBase,int BufferSize,OSD_ColorType_t ColorType,DRVOS_Partition_p pPartition);

/*******************************************************************************
Name        : DRV_DrawGIF
Description : 画GIF
Parameters  : 
Assumptions :   
Limitations :    
Returns     :0                      错误
                 other                正确
*******************************************************************************/
ID_GIF DRV_DrawGIF(GIF_t* gif, int positionX, int positionY, OSD_Region_t* region);

/*******************************************************************************
Name        : DRV_CreateGIF
*******************************************************************************/

ID_GIF DRV_CreateGIF(void* BufferBase,int BufferSize,OSD_ColorType_t ColorType,DRVOS_Partition_p pPartition);
/*******************************************************************************
Name        : DRV_StartGIF
*******************************************************************************/

void  DRV_StartGIF(GIF_t* gif, int positionX, int positionY, OSD_Region_t* Region);

/*******************************************************************************
Name        : DRV_NewGIFRect
*******************************************************************************/
ID_GIF DRV_NewGIFRect(void* BufferBase,int BufferSize,DRV_RectWh *Rect_p,OSD_Region_t* Region,DRVOS_Partition_p pPartition);

/*******************************************************************************
Name        : DRV_FreeGIF
*******************************************************************************/
void DRV_FreeGIF(ID_GIF gifID);
/*******************************************************************************
Name        : DRV_SetGifRect
*******************************************************************************/
U32 DRV_SetGifRect(GIF_t* gif,DRV_RectWh* Rect_p);
#endif

/* END */
