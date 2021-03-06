/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : bgbmp.h

    Description : 图形引擎模块，主要提供图形绘制功能，
    实现线、矩形、圆、域填充、文本、颜色、图形
    等的绘制；它调用设备驱动模块提供的服务接口；

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-12
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2004-03-01
    作    者：XXX
    说    明：1. 问题简述
              2. 修改内容简述
              3. 其他

*******************************************************************************/

#ifndef BGBMP_H__
#define BGBMP_H__
/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "driver.h"
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Enum & Macro
 *----------------------------------------------------------------------------*/
typedef OSD_BitMap_p (*BmpNameFun)(char *BmpName);


/*----------------------------------------------------------------------------
 *	Global Function Prototype
 *----------------------------------------------------------------------------*/

/*************************************************
* Function:       // 函数名称
* Description:    // 函数功能、性能等的描述
* Input:          // 输入参数说明，包括每个参数的作
                  // 用、取值说明及参数间关系。
* Output:         // 对输出参数的说明。
* Return:         // 函数返回值的说明
* Others:         // 其它说明
*************************************************/
void GUI_BmpNameSet(BmpNameFun BmpName);
void MidControlDrawBmp(void * pItem,int nStyle);//only draw the picture 
void MidTableRowDrawBmp(void * pTableRow);//only draw the picture 
void MidDrawBmp(char *GraphicName, Rect_t *rcCell);
void MidDirectDrawBmp(char *GraphicName, int x, int y);
#endif //BGBMP_H__


