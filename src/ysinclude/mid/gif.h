#ifndef __GIF_H__
#define __GIF_H__

#include "driver.h"
/* ID_GIF ��ֵ
 * -1     ��ЧGIF
 * ����    ��ЧGIF. */

/* �˰汾ʵ�ֲ������ֵ�֡�Ͷ�֡����֡Ҳ����֡���� */

/* ʹ�� ����*/
/* ����Ϊ��ѡ */
/* DRV_NewGIF  (DRV_PauseGIF) (DRV_ChangeGIF) (DRV_ResumeGIF)  DRV_FreeGIF*/
/* DRV_LoadGIF  DRV_DrawGIF (DRV_PauseGIF) (DRV_ResumeGIF)  DRV_FreeGIF*/

#define  DRVOS_Partition_p DRVOS_Partition_t*
typedef  int ID_GIF;
typedef struct GIF_STRUCT_s GIF_t; 
/*******************************************************************************
Name        : DRV_NewGIF
Description : ����gif����
Parameters  : BufferBase     GIF�ļ��ڴ����ַ
              BufferSize     GIF�ļ��ڴ��С
              PositionX      GIFҪ��ʾ�����������
              PoistionY      GIFҪ��ʾ������������
              Region         GIF��Ҫ������region handle
              pPartition     �ڴ���Դ,ΪNULLʱ,��ԴΪsystem partition
Assumptions : �ڸ�GIF��������������ڣ�Ҫ��֤GIF�ļ�buffer�����ݲ��ᱻ�޸�. �˺�������ʱ������ʾgif��һ֡��
Limitations : none
Returns     :   0                      ����
                    other                ��ȷ
*******************************************************************************/

ID_GIF DRV_NewGIF(void* BufferBase,int BufferSize,int PositionX,int PoistionY,OSD_Region_t* Region,DRVOS_Partition_p pPartition);

/*******************************************************************************
Name        : DRV_NewGIF
Description : �ͷ�gif������Դ
Parameters  : GIF ID 
Assumptions :
Limitations :  
Returns     : none
*******************************************************************************/
void DRV_FreeGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_NewGIF
Description : ��ͣgif��ʾ
Parameters  : GIF ID
Assumptions :   �������������ܼ�ʱ����ͣgif��ʾ���������ȴ�
Limitations :    
Returns     : none
*******************************************************************************/
void DRV_PauseGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_ResumeGIF
Description : �ָ�gif��ʾ
Parameters  : GIF ID
Assumptions :   �������������ܼ�ʱ���Զ�gif��ʾ����gif���½�����Ҫʱ�䣬
                        ��ʵ����gif���ڵ��ô˺���֮��һ��ʱ�����Իָ���ʾ
Limitations :    
Returns     : none
*******************************************************************************/
int DRV_ResumeGIF(ID_GIF gifID);

/*******************************************************************************
Name        : DRV_ChangeGIF
Description : �ı�gif������ʵ��gifͼƬ
Parameters  : GIF ID
Assumptions :   ���漰�ײ���Դ�Ĳ�����ֻ���������������������gif�Ŀ����л�
Limitations :   Ҫ����gif�Ŀ�͸���ԭ������ͬ. 
Returns     : none
*******************************************************************************/
int DRV_ChangeGIF(ID_GIF gifID, void* BufferBase,int BufferSize);

/*******************************************************************************
Name        : DRV_LoadGIF
Description : �½�gif����,�������һ֡.
Parameters  : 
Assumptions :   
Limitations :    
Returns     : NULL    ʧ��
		      other   �ɹ�
*******************************************************************************/
GIF_t* DRV_LoadGIF(void* BufferBase,int BufferSize,OSD_ColorType_t ColorType,DRVOS_Partition_p pPartition);

/*******************************************************************************
Name        : DRV_DrawGIF
Description : ��GIF
Parameters  : 
Assumptions :   
Limitations :    
Returns     :0                      ����
                 other                ��ȷ
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
