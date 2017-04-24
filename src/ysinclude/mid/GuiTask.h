#ifndef _GUITASK_H_
#define _GUITASK_H_

//#include "ys_typedef.h"
#include "stddefs.h"
#include "driver.h"

#define ROLL_TEXT

#define DYNAMIC_STATE_DESTORY           -1  //销毁本对象
#define DYNAMIC_STATE_INIT                   0  //初始化
#define DYNAMIC_STATE_FORWARD           1//向
#define DYNAMIC_STATE_BACKWARD          2 //暂不支持
#define DYNAMIC_STATE_PAUSE             0x8000//暂停

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
    int status;//状态
    S32 delayTime;    //下一帧间隔时间.允许可变
    GUI_OprateHadle Oprate;
    void *payLoad; 
};

#define GUI_TEXT_LINE_ROLL_MAX  128
struct GUI_TextLineRoll
{
    OSD_Region_t * region;
    DRVOS_Partition_t*              CPUPartition_p; 
    PrepareFrameFunc    funcPrepareFrame;//准备帧函数
    ShowFramFunc         funcShowFrame;//显示帧函数
    InitFunc                   funcInit;
    DestoryFunc             funcDestory;//

//帧控制
    int repeat;//重复标志 -1 循环  n 指定次数
    int returnOne;//循环结束，是否返回第一帧
    int roundEndInterval;//最后一帧与第一帧间隔时间
    int roundStartInterval;//第一二帧间隔时间
    int frameInterval;//前后帧间隔

    int frameStatus; // 1 第一帧 0 其它 -1 最后一帧
 //   int frameNumber;

//字符属性
    char text[GUI_TEXT_LINE_ROLL_MAX];
    int textColor;
    int textWidth;
    int textHeigth;//useless for now
    int textSpace;
    int textResizeFacterH;
    int textResizeFacterV;
    int textStepLength;//步进长度
  //背景属性
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
     
// private member. 内部维护. 由DRV_OSDRollInit赋值......
     OSD_Bitmap_t bitmap;//帧缓存
    int textOffsetX;
    int textOffsetY;
    int textCharOffset;//字符偏移
    int textCurrentChar;//当前字符从0计数
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



//画字的一部分
void DrawRollCharacter(OSD_Bitmap_t* Bitmap_p,
                                      char Character[20][5],
                                      int Font,
                                      int PositionX,
                                      int PositionY,
                                      int wbs,
                                      int hbs,
                                      int color,
                                      int startPix,
                                      int endPix);/*color为0时不能显示*/



//把字写进指定内存位置
void DrawBitmapCharacter(OSD_Bitmap_t *Bitmap_p,
                                      char Character[20][5],
                                      int Font,
                                      int PositionX,
                                      int PositionY,
                                      int wbs,
                                      int hbs,
                                      int Color);/*color为0时不能显示*/



//限定宽度的写字函数，支持前后都只显示字符的一部分的情况
//目前计算方式仅为从左至右
// 返回完全画完
int DRV_OSDRollTextDraw(OSD_Bitmap_t* Bitmap_p,
                                                char *Text,
                                                 int Offset,/*当前字符的已画像素数*/
                                                int x,/*文字所在行的横向起始位置*/
                                                int y,
                                                int wbs,
                                                int hbs, 
                                                int fontSpase,
                                                int TextColor,
                                                int TotalWidth);


//准备下一帧到缓存中
U32 DRV_OSDRollPrepareFrame(GUI_TextLineRoll_t* roll);


U32 DRV_OSDRollShowFrame(GUI_TextLineRoll_t* roll);



U32 DRV_OSDRollDestory(GUI_TextLineRoll_t* roll);


/*  0 scuess; -1 failed */
U32 DRV_OSDRollInit(GUI_TextLineRoll_t* roll);


void DRV_OSDRollOperation(GUI_SouceNode_t* dy);

#endif

#endif

