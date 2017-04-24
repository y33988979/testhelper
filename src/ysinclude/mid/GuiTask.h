#ifndef _GUITASK_H_
#define _GUITASK_H_

//#include "ys_typedef.h"
#include "stddefs.h"
#include "driver.h"

#define ROLL_TEXT

#define DYNAMIC_STATE_DESTORY           -1  //���ٱ�����
#define DYNAMIC_STATE_INIT                   0  //��ʼ��
#define DYNAMIC_STATE_FORWARD           1//��
#define DYNAMIC_STATE_BACKWARD          2 //�ݲ�֧��
#define DYNAMIC_STATE_PAUSE             0x8000//��ͣ

#define BACKGROUND_COLOR 0
#define BACKGROUND_BITMAP -1

typedef struct GUI_SouceNode GUI_SouceNode_t;
typedef struct GUI_TextLineRoll GUI_TextLineRoll_t;

typedef void (*GUI_OprateHadle)(GUI_SouceNode_t *this);


typedef U32 (*InitFunc)(GUI_TextLineRoll_t* dynamic);
typedef U32 (*PrepareFrameFunc)(GUI_TextLineRoll_t* dynamic);
typedef U32 (*ShowFramFunc)(GUI_TextLineRoll_t* dynamic);
typedef U32 (*DestoryFunc)(GUI_TextLineRoll_t* dynamic);


struct GUI_SouceNode
{
    int status;//״̬
    S32 delayTime;    //��һ֡���ʱ��.����ɱ�
    GUI_OprateHadle Oprate;
    void *payLoad; 
};

#define GUI_TEXT_LINE_ROLL_MAX  128
struct GUI_TextLineRoll
{
    OSD_Region_t * region;
    DRVOS_Partition_t*              CPUPartition_p; 
    PrepareFrameFunc    funcPrepareFrame;//׼��֡����
    ShowFramFunc         funcShowFrame;//��ʾ֡����
    InitFunc                   funcInit;
    DestoryFunc             funcDestory;//

//֡����
    int repeat;//�ظ���־ -1 ѭ��  n ָ������
    int returnOne;//ѭ���������Ƿ񷵻ص�һ֡
    int roundEndInterval;//���һ֡���һ֡���ʱ��
    int roundStartInterval;//��һ��֡���ʱ��
    int frameInterval;//ǰ��֡���

    int frameStatus; // 1 ��һ֡ 0 ���� -1 ���һ֡
 //   int frameNumber;

//�ַ�����
    char text[GUI_TEXT_LINE_ROLL_MAX];
    int textColor;
    int textWidth;
    int textHeigth;//useless for now
    int textSpace;
    int textResizeFacterH;
    int textResizeFacterV;
    int textStepLength;//��������
  //��������
     int bgType;
     union
     {
         int bgColor;
         OSD_Bitmap_t *bgBitmap;
     }bg;
     int bgPositionX;
     int bgPositionY;    
     int bgWidth;
     int bgHeigth;      
     int bgStepLength;
     
// private member. �ڲ�ά��. ��DRV_OSDRollInit��ֵ......
     OSD_Bitmap_t bitmap;//֡����
    int textOffsetX;
    int textOffsetY;
    int textCharOffset;//�ַ�ƫ��
    int textCurrentChar;//��ǰ�ַ���0����
    int magicWord;
};


S32 GUI_DynamicNode(GUI_OprateHadle oprate, void * data);

BOOL GUI_ResumeNode(S32 NodeNum);

BOOL GUI_PauseNode(S32 NodeNum);

void* GUI_GetPayLoad(S32 NodeNum);

BOOL GUI_RestNode(S32 NodeNum);

void GUI_RestAll(void);

void GUI_FieldInputDelay(int DelayMs);

void GUI_FieldInputComplete(void);

BOOL GUI_StartNode(S32 NodeNum,S32 DelayTime);

void GUI_TaskInit(void);

#ifdef ROLL_TEXT
int DRV_OSDTextLineWidth(char *hz,int wbs,int FontSpace);



//���ֵ�һ����
void DrawRollCharacter(OSD_Bitmap_t* Bitmap_p,
                                      char Character[20][5],
                                      int Font,
                                      int PositionX,
                                      int PositionY,
                                      int wbs,
                                      int hbs,
                                      int color,
                                      int startPix,
                                      int endPix);/*colorΪ0ʱ������ʾ*/



//����д��ָ���ڴ�λ��
void DrawBitmapCharacter(OSD_Bitmap_t *Bitmap_p,
                                      char Character[20][5],
                                      int Font,
                                      int PositionX,
                                      int PositionY,
                                      int wbs,
                                      int hbs,
                                      int Color);/*colorΪ0ʱ������ʾ*/



//�޶���ȵ�д�ֺ�����֧��ǰ��ֻ��ʾ�ַ���һ���ֵ����
//Ŀǰ���㷽ʽ��Ϊ��������
// ������ȫ����
int DRV_OSDRollTextDraw(OSD_Bitmap_t* Bitmap_p,
                                                char *Text,
                                                 int Offset,/*��ǰ�ַ����ѻ�������*/
                                                int x,/*���������еĺ�����ʼλ��*/
                                                int y,
                                                int wbs,
                                                int hbs, 
                                                int fontSpase,
                                                int TextColor,
                                                int TotalWidth);


//׼����һ֡��������
U32 DRV_OSDRollPrepareFrame(GUI_TextLineRoll_t* roll);


U32 DRV_OSDRollShowFrame(GUI_TextLineRoll_t* roll);



U32 DRV_OSDRollDestory(GUI_TextLineRoll_t* roll);


/*  0 scuess; -1 failed */
U32 DRV_OSDRollInit(GUI_TextLineRoll_t* roll);


void DRV_OSDRollOperation(GUI_SouceNode_t* dy);

#endif

#endif

