/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : ysosd.c

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
    最近更新:
        1. 2005-12-20 9:15  配合osddrv.c中DRV_OSDCharacterDraw对ATB字符的支持，修改TextLineDraw
        2. 2008-01-10 cran  1.修改TextLineDraw对未显示部分的判断和绘画错误
        3. 2008-12-22 cran  1.修改TextLineDraw可能致指针越界的问题
        4. 2000-09-11 cran  1.修改当一个英文单词过长在于一行宽度时时画不出来的bug




*******************************************************************************/

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "ysosd.h"

#include <string.h>
#include <math.h>

#include "osddrv.h"
/*----------------------------------------------------------------------------
 *	Debug
 *----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
 *	External Function Prototype
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	External variable
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Macro
 *----------------------------------------------------------------------------*/
#define REGION_MAX_NUMBER  5

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Private Constant & Enum 
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global variable
 *----------------------------------------------------------------------------*/
OSD_Region_t *g_OSDCurrentRegionHandle = NULL;

/*----------------------------------------------------------------------------
 *	Private Module Variable
 *----------------------------------------------------------------------------*/
static S32 m_BackgroundColor = 0;
static S32 m_ForegroundColor = 0;

static U8 m_CurrentRegionsNumber = 0;
static OSD_Region_t* m_RegionList[REGION_MAX_NUMBER];
/*----------------------------------------------------------------------------
 *	Private Function Prototype
 *----------------------------------------------------------------------------*/

 
/*----------------------------------------------------------------------------
 *	Function
 *----------------------------------------------------------------------------*/
OSD_Region_t *MidCreateRegion(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 bits_per_pixel)

{
  OSD_Region_t *region = NULL;

  if (m_CurrentRegionsNumber < REGION_MAX_NUMBER)
    region = DRV_OSDRegionCreate(name, xstart, ystart, xend, yend, bits_per_pixel);

  if (NULL == region)
  {
    Err_Print("!!!MidCreateRegion error, name is %s!\n", name);
  }
  else
  {
       m_RegionList[m_CurrentRegionsNumber++] = region;
  }
  
  return region;
}

OSD_Region_t *MidCreateRegion16(char *name,
                                                  S32 xstart,
                                                  S32 ystart,
                                                  S32 xend,
                                                  S32 yend,
                                                  S32 bits_per_pixel)

{
    return MidCreateRegion(name,xstart,ystart,xend,yend,bits_per_pixel);
}

void MidDrawRegionSet(OSD_Region_t *region )
{
  if ((NULL != region) && (region != g_OSDCurrentRegionHandle))
  {
    DRV_OSDRegionHide(g_OSDCurrentRegionHandle);  
 
    g_OSDCurrentRegionHandle = region;
    DRV_OSDRegionShow(g_OSDCurrentRegionHandle);
  }
}

void MidDrawRegionSet16(OSD_Region_t *region )
{
    MidDrawRegionSet(region);
}

OSD_Region_t * MidDrawRegionGet(void)
{
  return g_OSDCurrentRegionHandle;
}

void MidDrawRegionNameSet(char *name)
{
  S32 i;
  OSD_Region_t *region = NULL;
  
  i = m_CurrentRegionsNumber;
  while((i--) && (NULL == region))
  {
    if (0 == strncmp(name, DRV_OSDRegionNameGet(m_RegionList[i]), OSD_REGION_NAME_LENGTH))
      region = m_RegionList[i];
  }

  if ((NULL == region))
  {
    Err_Print("!!!MidDrawRegionNameSet error, name is %s!\n", name);
  }
  else
  {        
    MidDrawRegionSet(region);
  }
}

void gprim_setbcol(S32 color)
{
  m_BackgroundColor = (color);
}

void gprim_setfcol(S32 color)
{
  m_ForegroundColor = (color);
}

void MidDrawDot(S32 x, S32 y, S32 color)
{
  OSD_Region_t * region = g_OSDCurrentRegionHandle;
  
  DRV_OSDPixelDraw(region, x, y, color);
}


void MidDrawLine(S32 x0,S32 y0,S32 x1,S32 y1,S32 style,S32 width,S32 color)
{
	S32 i;
	OSD_Region_t *region = g_OSDCurrentRegionHandle;

	if(width<=0)
		return;
	
	gprim_setfcol(color);
	
	if(style==SOLID)
	{
	
		if(x0==x1)//vertical line
		{
			for(i= 0;i<width;i++)
			{
				DRV_OSDLineDraw(region, x0++,y0,x1++,y1, color);
			}
			return ;
		}
		else if(y0==y1)//horizontal line
		{
			for(i=0;i<width;i++)
			{
				DRV_OSDLineDraw(region,x0,y0++,x1,y1++, color);
			}
			return ;
		}

	}
	 
	return ;
}

//draw a rectangle with certain color and fill it or not
void MidDrawRect(Rect_t rect, S32 color,S32 IsFill,S32 width,S32 style)
{
	S32 i;
	OSD_Region_t *region = g_OSDCurrentRegionHandle;

	if(width<0)
		return;
	
	gprim_setfcol(color);
	
	if (IsFill)
	{
		DRV_OSDRectFillDraw(region, rect.x0 ,rect.y0 ,rect.x1 ,rect.y1, color);
	}
	else if (style == SOLID)
	{
		for(i = 0; i < width; i++)
		{
			DRV_OSDRectDraw(region, 
				                        rect.x0 + i ,
				                        rect.y0 + i,
				                        rect.x1 - i,
				                        rect.y1 - i,
				                        color);
		}
		
	}
	
	return;
}

void MidDrawCircle(Rect_t rect,S32 color,S32 IsFill,S32 width,S32 style)
{
	OSD_Region_t *region = g_OSDCurrentRegionHandle;
	S32 i=0;

	gprim_setfcol(color);

	if(IsFill)
	{
		DRV_OSDCircleFillDraw(region, 
                                       rect.x0 +(rect.x1 -rect.x0 )/2,
                                       rect.y0 +(rect.y1 -rect.y0 )/2,
                                       abs(rect.x1 -rect.x0 )/2,
                                       abs(rect.y1 -rect.y0 )/2,
                                       color);
		return;
	}
	if(style==SOLID)
	{
		for(i=1;i<=width;i++)
		{
          DRV_OSDCircleFillDraw(region, 
                                             rect.x0 +(rect.x1 -rect.x0 )/2,
                                             rect.y0 +(rect.y1 -rect.y0 )/2,
                                             abs(rect.x1 -rect.x0 )/2-i,
                                             abs(rect.y1 -rect.y0 )/2-i,
                                             color);
		}
		return;
	}
}

//draw the shadow of a rectangle with certain color
void MidDrawShadow(Rect_t rect, S32 FillColor, S32 ShadowColor, S32 style,S32 width)
{
	S32 i=0;
	OSD_Region_t *region = g_OSDCurrentRegionHandle;

	if(width < 0)
		return;

	gprim_setfcol(FillColor);

	DRV_OSDRectFillDraw(region, rect.x0 ,rect.y0 ,rect.x1 ,rect.y1, FillColor);
	
       gprim_setfcol(ShadowColor);

	if(style == OSD_UP_SHADOW)
	{		
		for(i=0;i<width;i++)
		{
			DRV_OSDLineDraw(region, rect.x0,rect.y0 -i,rect.x1,rect.y0 - i, ShadowColor);
			DRV_OSDLineDraw(region, rect.x0-i,rect.y0,rect.x0-i,rect.y1, ShadowColor);
		}
	}
	else if(style == OSD_DOWN_SHADOW)
	{
		for(i=0;i<width;i++)
		{
			DRV_OSDLineDraw(region, rect.x0,rect.y0 + i,rect.x1,rect.y0 + i, ShadowColor);
			DRV_OSDLineDraw(region, rect.x0+i,rect.y0,rect.x0+i,rect.y1, ShadowColor);
		}
	}
	
	return;	
}

//draw a triangle with certain color and fill it or not. 
//The style of the triangle is one of its angles  pointing to the top or to the right.
void MidDrawTriangle(Rect_t rect,S32 color,S32 IsFill,S32 style,S32 width)
{
	OSD_Region_t *region = g_OSDCurrentRegionHandle;
	S32 iA1=0,iB1=0,iA2=0,iB2=0,iA3=0,iB3=0;
	S32 x1=0,y1=0,iX2=0,iY2=0;
	S32 i=0;
	S32 iSpace=0;
	S32 iA=0,iB=0;
	S32 iSign=1;
	S32 iHorizontal=0;
	double dX1=0,dX2=0, dY1=0,dY2=0;
	double dTop=(double)rect.y0 ;
	double dLeft=(double)rect.x0 ;
	double dRight=(double)rect.x1 ;
	double dBottom=(double)rect.y1 ;
	double dTemp1=0,dTemp2=0;

	if(width<0)
		return;

	gprim_setfcol(color);

	iA=rect.y0;
	iB=rect.y1;
	x1=rect.x0;
	iX2=rect.x1;
	y1=rect.y0;
	iY2=rect.y1;
	dX1=dLeft;
	dX2=dRight;
	dY1=dTop;
	dY2=dBottom;
	iSign=1;
	if(IsFill)//fill the triangle
	{
		switch(style)
		{
		case OSD_UP_TRIANGLE:
			y1=0;
			iY2=0;
			break;
		case OSD_DOWN_TRIANGLE:
			y1=0;
			iY2=0;
			iA=rect.y1;
			iB=rect.y0;
			iSign=-1;
			break;
		case OSD_LEFT_TRIANGLE:
			iA=rect.x0;
			iB=rect.x1;
			x1=0;
			iX2=0;
			dX1=dTop;
			dX2=dBottom;
			dY1=dLeft;		
			dY2=dRight;	
			break;
		case OSD_RIGHT_TRIANGLE:
			iA=rect.x1;
			iB=rect.x0;
			x1=0;
			iX2=0;
			dX1=dTop;
			dX2=dBottom;
			dY1=dLeft;
			dY2=dRight;
			iSign=-1;
			break;
		}
		for(i=1;abs(iA-iB)>0;i++)
		{
			iSpace=(S32)((double)i*(dX2-dX1)/2.0/(dY2-dY1));
			if(x1!=0)//one angle of triangle pointing to  top or bottom
			{
				iA1=x1+(iX2-x1)/2-iSpace;
				iA2=x1+(iX2-x1)/2+iSpace;
			}
			else//one angle of triangle pointing to  left or right
			{
				iA1=iA;
				iA2=iA;
			}
			if(y1!=0)//one angle of triangle pointing to  left or right
			{
				iB1=y1+(iY2-y1)/2-iSpace;
				iB2=y1+(iY2-y1)/2+iSpace;
			}
			else//one angle of triangle pointing to  top or botttom
			{	
				iB1=iA;
				iB2=iA;
			}
			DRV_OSDLineDraw(region, iA1,iB1,iA2,iB2, color);
			iA=iA+iSign;
		}
		return;
	}
	else//don't fill triangle
	{
		switch(style)
		{
		case OSD_UP_TRIANGLE:
      
			break;
		case OSD_LEFT_TRIANGLE:
			x1=rect.y0;
			iX2=rect.y1;
			y1=rect.x0;
			iY2=rect.x1;
			dX1=dTop;
			dX2=dBottom;
			dY1=dLeft;
			dY2=dRight;
			iSign=1;
			iHorizontal=1;
			break;
		case OSD_RIGHT_TRIANGLE:
			x1=rect.y1;
			iX2=rect.y0;
			y1=rect.x1;
			iY2=rect.x0;
			dX1=dBottom;
			dX2=dTop;
			dY1=dRight;
			dY2=dLeft;
			iSign=-1;
			iHorizontal=1;
			break;
		case OSD_DOWN_TRIANGLE:
			x1=rect.x1;
			iX2=rect.x0;
			y1=rect.y1;
			iY2=rect.y0;
			dX1=dRight;
			dX2=dLeft;
			dY1=dBottom;
			dY2=dTop;
			iSign=-1;
			break;
		}
		for(i=1;i<=width;i++)
		{
			dTemp1=sqrt((dX1-dX2)*(dX1-dX2)/4.0+(dY2-dY1)*(dY2-dY1));
			iA2=(iX2-x1)/2+x1;
			iB2=y1+(S32)((double)i*2.0/(dX2-dX1)*dTemp1);
			dTemp2=(double)i/(dY2-dY1)*dTemp1+dX1;
			iA1=(S32)(dTemp2+(double)i/(dY2-(double)iB2)*((double)iSign*(dX2-dX1)/2.0-(dTemp2-dX1)));
			iB1=iY2-iSign*i;
			iA3=iX2+x1-iA1;
			iB3=iB1;
			if(iHorizontal)//one angle of triangle pointing to the right or left
			{
				iHorizontal=iA1;iA1=iB1;iB1=iHorizontal;	
				iHorizontal=iA2;iA2=iB2;iB2=iHorizontal;
				iHorizontal=iA3;iA3=iB3;iB3=iHorizontal;
				iHorizontal=1;
			}
			DRV_OSDLineDraw(region, iA1,iB1,iA2,iB2, color);
			DRV_OSDLineDraw(region, iA2,iB2,iA3,iB3, color);
			DRV_OSDLineDraw(region, iA1,iB1,iA3,iB3, color);
			
		}
	}
}

//draw the text in a rectangle with certain alignment 

void MidDrawHz(char *pHz,S32 x,S32 y,S32 nFontSize,S32 nFontStyle)
{
  OSD_Region_t *region = g_OSDCurrentRegionHandle;
  S32 color = m_ForegroundColor;
  DRV_OSDTextLineDraw(region,pHz,strlen(pHz),x,y,1,1,0,color);
}


S32 TextLineDraw(char *TextInput,
                                  S32 nFontSize,
                                  S32 nFontStyle, 
                                  S32 wbs, 
                                  S32 hbs, 
                                  Rect_t *rect,
                                  TEXT_TextStyle_e TextStyle,
                                  S32 FontSpace,
                                  S32 crFore,
                                  S32 crBack)
{
    OSD_Region_t *region = g_OSDCurrentRegionHandle;
    Rect_t Rect;
    S32 CharUesdNum = 0;
    S32 SpaceWidth, SpaceHigh;
    S32 TextLineWidth = 0;
    S32 TextLineHigh = 0;
    S32 FontWidth, FontHigh;
    S32 VaSpace;
    S32 CharacterNum = 0;

    S32 Xstart;
    S32 Ystart;

    S32 PointFlag = FALSE;
    char Point[3] = {0};

    if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
        return CharUesdNum;

    Rect = *rect;

    SpaceWidth = Rect.x1 - Rect.x0 + 1 + /*补间隔*/FontSpace;
    SpaceHigh = Rect.y1 - Rect.y0 + 1;

    FontWidth = nFontSize * wbs;
    FontHigh = nFontSize * hbs;

    TextLineHigh = FontHigh;

    /*下面代码计算可输出字符个数及输出宽度*/
    for(CharUesdNum = 0; 
        ((TextLineWidth <= SpaceWidth) 
        //&& (TextLineHigh <= SpaceHigh)
        && ('\0' != TextInput[CharUesdNum])
        && ('\n' != TextInput[CharUesdNum])); 
        CharUesdNum++)
    {
        if(TextInput[CharUesdNum] & 0x80)/*汉字*/
        {
            CharUesdNum++;/*补齐汉字*/
            TextLineWidth += FontWidth + FontSpace;
            CharacterNum++;
        }
        else/*ASCII码*/
        {
            if(0X9 == TextInput[CharUesdNum])/*tab键处理,调整宽度范围为1~40*/
            {
                TextLineWidth += 40 - TextLineWidth%40;
            }
            else if ((TEXT_TEXTER_FLAG & TextStyle) && ((('a' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'z')) || (('A' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'Z'))))
            {

                S32 tmpWidth;
                S32 tmpUsedNum;
                S32 tmpCharNum;
		S32 WordWidth;
		S32 WordIndex;

                tmpWidth = TextLineWidth;
                tmpUsedNum = CharUesdNum - 1;/*去掉本单词首字母*/
                tmpCharNum = CharacterNum - 1;

                TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
                CharacterNum++;
                CharUesdNum++;

                while((TextLineWidth <= SpaceWidth) && ((('a' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'z')) || (('A' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'Z'))))
                {
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
                    CharacterNum++;
                    CharUesdNum++;
                }
		
		if (TextLineWidth > SpaceWidth)
		{
			WordWidth = TextLineWidth - tmpWidth;
			WordIndex = CharUesdNum;
			while((('a' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= 'z')) 
				|| (('A' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= 'Z'))
				|| (('0' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= '9')))
			{
				WordWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
				WordIndex++;
			}
			/*如果一个word所占用的宽度大于行宽，则分为两行来画*/
			if ((0 != tmpWidth)&&(WordWidth < SpaceWidth))
			{
				/*防止单词过长，单行显示不下*/
				TextLineWidth = tmpWidth;
				CharUesdNum = tmpUsedNum;
				CharacterNum = tmpCharNum;
			}
		break;
                }

                CharacterNum--;//对应上面的++
                CharUesdNum--;//对应上面的++

            }
            else
            {
                TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
            }
            CharacterNum++;
        }  
    }

    if('\n' == TextInput[CharUesdNum])
        CharacterNum++;/*回车计数个数*/
    
    if(CharacterNum > 0) 
    {
        if (TextLineWidth > SpaceWidth)/*最后字符超出范围*/
        {  
            CharUesdNum--;/*恢复为最后字符下标*/
            CharacterNum--;
            /* 最后两个字符均大于0x80即为汉字(GB & BIG5) */
            /*TextInput[CharUesdNum]为最后一个asc字符*/
            if((TextInput[CharUesdNum -1] & 0x80)&&(TextInput[CharUesdNum] & 0x80)&&(CharUesdNum > CharacterNum))
            {
                TextLineWidth -= FontWidth + FontSpace;
                CharUesdNum --;/*回退一个汉字*/
            }
            else
            {
                TextLineWidth -= gFontWidth[TextInput[CharUesdNum]] + FontSpace;
            }

            if(CharacterNum < 1)return 0;/*没有字符可画*/

            /*超出部分用省略号,针对GB2312*/
            if (!(TEXT_TEXTER_FLAG & TextStyle))
            {
                if(CharUesdNum < 2)return 0;/*没有空间可画*/
                if((TextInput[CharUesdNum - 2] & 0x80)&&(TextInput[CharUesdNum -1] & 0x80))/*最后一个是汉字*/
                {
                    TextLineWidth -= FontWidth + FontSpace;

                    Point[1] = TextInput[CharUesdNum - 1];
                    CharUesdNum --;
                    Point[0] = TextInput[CharUesdNum - 1];

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharacterNum--;

                }
                else if ((TextInput[CharUesdNum - 3] & 0x80)&&(TextInput[CharUesdNum-2] & 0x80)&&(TextInput[CharUesdNum - 1] > 0))/*最后是汉字+ASC*/
                {
                    if(CharUesdNum < 3)return 0;/*没有字符可画*/
                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    CharUesdNum --;
                    CharacterNum--;

                    TextLineWidth -= FontWidth + FontSpace;

                    Point[1] = TextInput[CharUesdNum - 1];
                    CharUesdNum --;
                    Point[0] = TextInput[CharUesdNum - 1];

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharacterNum--;
                }
                else/*最后是ASC*/
                {
                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    Point[1] = TextInput[CharUesdNum - 1];

                    CharUesdNum--;
                    CharacterNum--;

                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    Point[0] = TextInput[CharUesdNum - 1];

                    CharacterNum--;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                }
                PointFlag = TRUE;
            }
        }
        else if ('\0' != TextInput[CharUesdNum])/*正常范围内*/
        {
            CharUesdNum++;/*跳过所有非结束符的非打印字符，如'\n'*/
        }

        TextStyle &= ~TEXT_TEXTER_FLAG;
    }

    VaSpace = SpaceWidth - TextLineWidth;

    /*下面代码输出字符*/
    if(CharUesdNum > 0)
    {/*有字符输出*/
        gprim_setfcol(crFore);
        gprim_setbcol(crBack);

        switch(TextStyle)
        {
            case TEXT_UP_LEFT:
                Xstart = Rect.x0;
                Ystart = Rect.y0;
                break;
            case TEXT_UP_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = Rect.y0;
                break;
            case TEXT_UP_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = Rect.y0;
                break;
            case TEXT_MID_LEFT:
                Xstart = Rect.x0;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_MID_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_MID_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_DOWN_LEFT:
                Xstart = Rect.x0;
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_DOWN_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_DOWN_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_LINE_FULL:
                Xstart = Rect.x0;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            default:
                CharUesdNum = 0;
                break;
        }

        if (CharUesdNum > 0)
        {
            if ((TEXT_LINE_FULL != TextStyle) 
                    || ('\0' == TextInput[CharUesdNum])
                    || ('\n' == TextInput[CharUesdNum - 1]))
            {
                DRV_OSDTextLineDraw(region, 
                                                TextInput,
                                                CharUesdNum,
                                                Xstart,
                                                Ystart,
                                                wbs,
                                                hbs,
                                                FontSpace,
                                                crFore);
            }
            else
            {
                S32 i=0;
                S32 iDistance = 0;
                char asc[3] = {0, 0, 0};
                S32 ACom, Quot;
                S32 PrevChar;

                ACom = VaSpace % CharacterNum;
                Quot = VaSpace / CharacterNum;
                PrevChar = (CharacterNum - ACom) >> 1;

                for (i = 0; i < CharUesdNum; i++)
                {
                    if(TextInput[i] & 0x80)
                    {
                        memcpy(asc, &(TextInput[i]), 2);
                        i++;
                    }
                    else
                    {
                        asc[0] = TextInput[i];
                        asc[1] = '\0';
                    }

                    iDistance += Quot + FontSpace + DRV_OSDCharacterDraw(region, 
                                                                                                            asc, 
                                                                                                            Xstart,
                                                                                                            iDistance, 
                                                                                                            Ystart, 
                                                                                                            wbs, 
                                                                                                            hbs, 
                                                                                                            crFore,
                                                                                                            NULL,
                                                                                                            NULL);

                    if (0 < PrevChar)
                    {
                        PrevChar--;
                    }
                    else if (0 != ACom)
                    {
                        ACom--;
                        iDistance++;
                    }
                }
            }
        }

        /*省略号恢复为原数据*/
        if ((PointFlag) && (0 != CharUesdNum))
        {
            TextInput[CharUesdNum - 1] = Point[1];
            CharUesdNum --;
            TextInput[CharUesdNum - 1] = Point[0];
            CharUesdNum --;//去掉省略号占用的字数
        }
    }

    return CharUesdNum;
}

S32 TexterDraw(char *TextInput,
                            S32 nFontSize,
                            S32 nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle,
                            S32 crFore,
                            S32 crBack)
{
  Rect_t Rect;
  S32 CharUesdNum = 0;
  S32 SpaceWidth, SpaceHigh;
  S32 TextLineHigh = 0;
  S32 FontWidth, FontHigh;
  S32 wbs = 1;
  S32 hbs = 1;

  S32 LineSpace = 0;
  S32 FontSpace = 0;
  S32 LeftSpace = 0;
  S32 RightSpace = 0;
  S32 TopSpace = 0;
  S32 BottomSpace = 0;
  
  if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
    return CharUesdNum;

  Rect = *rect;

  if (NULL != TexterStyle)
  {
    LineSpace = TexterStyle->LineSpace;
    FontSpace = TexterStyle->FontSpace;
    LeftSpace = TexterStyle->LeftSpace;
    RightSpace = TexterStyle->RightSpace;
    TopSpace = TexterStyle->TopSpace;
    BottomSpace = TexterStyle->BottomSpace;
  }
    
  SpaceWidth = Rect.x1 - Rect.x0 + 1 - LeftSpace - RightSpace;
  SpaceHigh = Rect.y1 - Rect.y0 + 1 - TopSpace - BottomSpace;

  FontWidth = nFontSize * wbs;
  FontHigh = nFontSize * hbs;

  TextLineHigh = FontHigh;

  Rect.x0 += LeftSpace;
  Rect.x1 -= RightSpace;
  Rect.y0 += TopSpace;
  Rect.y1 = Rect.y0;

  TextStyle |= TEXT_TEXTER_FLAG;//区分后续省略符

  for(CharUesdNum = 0; 
            ((TextLineHigh <= SpaceHigh)
            && ('\0' != TextInput[CharUesdNum]));/*结束符被返回*/ 
        TextLineHigh += FontHigh + LineSpace)
  {
    Rect.y0 = Rect.y1;
    Rect.y1 = Rect.y0 + FontHigh;
    
    CharUesdNum += TextLineDraw(&(TextInput[CharUesdNum]),
                                                    nFontSize,
                                                    nFontStyle, 
                                                    wbs, 
                                                    hbs, 
                                                    &Rect,
                                                    TextStyle,
                                                    FontSpace,
                                                    crFore,
                                                    crBack);
    Rect.y1 += LineSpace;
  }

  return CharUesdNum;
}

#define X_DISTANCE 1
void MidDrawText(char *TextInput,S32 nFontSize,S32 nFontStyle,Rect_t rect,S32 style,S32 crFore,S32 crBack)
{
	S32 iCoordinateX=0;
	S32 iCoordinateY=0;
	S32 iTextHigth=0;
	S32 iCharNum=0;
	S32 iAsciiWidth =0;
	S32 iHalfHzWidth= 0;
	S32 iTextWidth =0;
	S32 i = 0;
	
	char *pText=NULL;
	S32 bAll=FALSE; 
	char *pTextIn = NULL;
	static S32 bWork = FALSE;
   S32 Zoom = 1;

#if 1
/*只能是8Bit的region*/
  TEXT_TextStyle_e NewStyle;
  switch (style)
  {
    case ALIGNMENT_MID:
      NewStyle = TEXT_MID_MID;
      break;
    case ALIGNMENT_LEFT:
      NewStyle = TEXT_MID_LEFT;
      break;
    case ALIGNMENT_RIGHT:
      NewStyle = TEXT_MID_RIGHT;
      break;
    default:
      NewStyle = TEXT_LINE_FULL;
      break;
  }

  if (nFontStyle > 1)
    Zoom = nFontStyle;

  TextLineDraw(TextInput, 
                        nFontSize,
                        nFontStyle,
                        Zoom,
                        Zoom,
                        &rect,
                        NewStyle,
                        0,
                        crFore,
                        crBack);
  return;
#endif

    i = 0;
    while(bWork)
    {
        i++;
        if(i>200)
            return;
    }
    bWork = TRUE;
    if(strlen(TextInput) == 0)
    {
        bWork = FALSE;
        return;
    }
    if(nFontSize == 16)
        iHalfHzWidth = 16/2;
    else 
        iHalfHzWidth = 20/2;
    
    pTextIn = TextInput;
	iCharNum=(rect.x1 -rect.x0-X_DISTANCE*2) / iHalfHzWidth;
	if(strlen(pTextIn)<iCharNum)
	{
		iCharNum=strlen(pTextIn);
		bAll=TRUE;
	}
	pText=(char*)malloc(sizeof(char)*(iCharNum+1));
	memset(pText,0,(iCharNum+1));
	{
		S32 iAscii=0;
		S32 i=0;
		while(i <iCharNum)
		{
			if((pTextIn[i] & 0x80) )/*半个汉字宽度加iHalfHzWidth*/
			{			
    			if(iTextWidth + iHalfHzWidth  > rect.x1 -rect.x0-X_DISTANCE*2)
    			    break;
				iTextWidth = iTextWidth +iHalfHzWidth;
			}
			else/*ASCII码，按每个码的宽度增加*/
			{
                if(nFontSize == 16)
				    iAsciiWidth = gFontWidth[pTextIn[i]]/2+1;                    
                else
				    iAsciiWidth = gFontWidth[pTextIn[i]];
    			if(iTextWidth +iAsciiWidth  > rect.x1 -rect.x0-X_DISTANCE*2)
    			    break;
				iAscii++;
				iTextWidth = iTextWidth + iAsciiWidth;
			}
			i++;
		}
		i--;/*去掉最后一个超出的字符*/
		iCharNum = i+1;

		/*判断最后一个字符是否为半个汉字*/
    	while(i>=0 && (pTextIn[i] & 0x80) )
    	{
    	    if(!(pTextIn[i+1] & 0x80))
    	        break;
    	    i=i-2;    	    
    	}
    	if(i+1 != iCharNum)
    	{
    	    if(!(pTextIn[i+1] & 0x80))
    	    {
    	        iCharNum--;    	  
    	        iTextWidth = iTextWidth - iHalfHzWidth;
    	    }
    	}
    	
	}
	strncpy(pText,pTextIn,iCharNum);

	if(iCharNum<=0) //the width of text
    {
    	free(pText);
        bWork = FALSE;
        return;
    }
	if(nFontSize<=0) //the font size of text 
    {
    	free(pText);
        bWork = FALSE;
        return;
    }
		gprim_setfcol(crFore);
		gprim_setbcol(crBack);
    
    if(nFontSize == 16)
    	iTextHigth=16;        
    else
    	iTextHigth=20; 
    
	switch (style)
	{
		case ALIGNMENT_MID:
		     iCoordinateX=X_DISTANCE+rect.x0+((rect.x1-rect.x0)-iTextWidth-X_DISTANCE*2)/2;
		     break;
		case ALIGNMENT_LEFT:
		     iCoordinateX=X_DISTANCE+rect.x0;
		     break;
		case ALIGNMENT_RIGHT:
		     iCoordinateX=rect.x1- iTextWidth - X_DISTANCE;
		     break;
		default:
		     break;
	}
	iCoordinateY=rect.y0+((rect.y1-rect.y0)-iTextHigth)/2;
	MidDrawHz(pText,iCoordinateX,iCoordinateY,nFontSize,nFontStyle);
	free(pText);
	bWork  = FALSE;
}

//draw a textstyle
void MidDrawTextStyle(char* pText,Rect_t rect,MidTextStyle_t *pTextStyle)
{
	MidDrawText(pText,
				pTextStyle->nFontSize,
				pTextStyle->nFontStyle,
				rect,
				pTextStyle->nStyle,
				pTextStyle->crFore,
				pTextStyle->crBack);
}


void MidDrawCls (S32 color)
{
  OSD_Region_t *region = g_OSDCurrentRegionHandle;

  DRV_OSDRegionFill(region, color);
}

S32 MidDrawSaveArea(S32 x0, S32 y0, S32 x1, S32 y1, OSD_Bitmap_t *area)
{
   	OSD_Region_t *region = g_OSDCurrentRegionHandle;

   return DRV_OSDAreaGet(region, x0, y0,  x1-x0, y1-y0, area);
   
}

S32 MidDrawRestoreArea(S32 x0, S32 y0,OSD_Bitmap_t *area)
{
   	OSD_Region_t *region = g_OSDCurrentRegionHandle;

   return DRV_OSDAreaRestore(region, x0, y0, area);   
}

S32 MidDrawShowArea(S32 x0, S32 y0,OSD_Bitmap_t *area)
{
   	OSD_Region_t *region = g_OSDCurrentRegionHandle;

   return DRV_OSDAreaShow(region, x0, y0, area);   
}

S32 MidDrawCloseArea(OSD_Bitmap_t *area)
{
    return DRV_OSDAreaClose(area);
}

void bmpdraw(void *graphic, Rect_t *rect)
{
  OSD_Region_t *region = g_OSDCurrentRegionHandle;
  STOSD_Bitmap_t *BitMap;

  BitMap = (STOSD_Bitmap_t*)graphic;
  
  rect->x1 = rect->x0 + BitMap->Width;
  rect->y1 = rect->y0 + BitMap->Height;
  DRV_OSDBitmapDraw(region, rect->x0, rect->y0, graphic);	
}

void drawbmp(S32 x,S32 y, void *graphic)
{
  OSD_Region_t *region = g_OSDCurrentRegionHandle;

  DRV_OSDBitmapDraw(region, x, y, graphic);	
}




S32 DRV_TextLineDraw(OSD_Region_t *region,char *TextInput,
                                  S32 nFontSize,
                                  S32 nFontStyle, 
                                  S32 wbs, 
                                  S32 hbs, 
                                  Rect_t *rect,
                                  TEXT_TextStyle_e TextStyle,
                                  S32 FontSpace,
                                  S32 crFore,
                                  S32 crBack)
{
  Rect_t Rect;
  S32 CharUesdNum = 0;
  S32 SpaceWidth, SpaceHigh;
  S32 TextLineWidth = 0;
  S32 TextLineHigh = 0;
  S32 FontWidth, FontHigh;
  S32 VaSpace;
  S32 CharacterNum = 0;
  
  S32 Xstart;
  S32 Ystart;

  S32 PointFlag = FALSE;
  char Point[3] = {0};
  
  if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
    return CharUesdNum;

  Rect = *rect;

  SpaceWidth = Rect.x1 - Rect.x0 + 1 + /*补间隔*/FontSpace;
  SpaceHigh = Rect.y1 - Rect.y0 + 1;

  FontWidth = nFontSize * wbs;
  FontHigh = nFontSize * hbs;

  TextLineHigh = FontHigh;

  /*下面代码计算可输出字符个数及输出宽度*/
  for(CharUesdNum = 0; 
        ((TextLineWidth <= SpaceWidth) 
            //&& (TextLineHigh <= SpaceHigh)
            && ('\0' != TextInput[CharUesdNum])
            && ('\n' != TextInput[CharUesdNum])); 
        CharUesdNum++)
  {
    if(TextInput[CharUesdNum] & 0x80)/*汉字*/
    {
      CharUesdNum++;/*补齐汉字*/
      TextLineWidth += FontWidth + FontSpace;
      CharacterNum++;
    }
    else/*ASCII码*/
    {
        if(0X9 == TextInput[CharUesdNum])/*tab键处理,调整宽度范围为1~40*/
        {
            TextLineWidth += 40 - TextLineWidth%40;
        }
        else
        {
      TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
        }
      CharacterNum++;
    }  
  }

  #if 0
  if((CharUesdNum > 0) 
      && (('\0' != TextInput[CharUesdNum])/*字符超出输出范围*/
        || (TextLineWidth > SpaceWidth)))/*最后字符超出范围*/
  #endif
  if(CharacterNum > 0) 
  {
     if (TextLineWidth > SpaceWidth)/*最后字符超出范围*/
    {  
      CharUesdNum--;/*恢复为最后字符下标*/
      CharacterNum--;
	/* 第一个字符大于0x80即为汉字(GB & BIG5) */
	/*TextInput[CharUesdNum]为最后一个asc字符*/
      if((TextInput[CharUesdNum -1] & 0x80)&&(CharUesdNum > CharacterNum))
      {
        TextLineWidth -= FontWidth + FontSpace;
        CharUesdNum --;/*回退一个汉字*/
      }
      else
      {
        TextLineWidth -= gFontWidth[TextInput[CharUesdNum]] + FontSpace;
      }

	  if(CharacterNum < 1)return 0;/*没有字符可画*/

      /*超出部分用省略号,针对GB2312*/
      if (!(TEXT_TEXTER_FLAG & TextStyle))
      {
        if(CharUesdNum < 2)return 0;/*没有空间可画*/
        if((TextInput[CharUesdNum - 2] & 0x80)&&(TextInput[CharUesdNum -1] & 0x80))/*最后一个是汉字*/
        {
          TextLineWidth -= FontWidth + FontSpace;
          
          Point[1] = TextInput[CharUesdNum - 1];
          CharUesdNum --;
          Point[0] = TextInput[CharUesdNum - 1];
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          
          CharUesdNum++;
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

          CharacterNum--;

        }
        else if ((TextInput[CharUesdNum - 3] & 0x80)&&(TextInput[CharUesdNum-2] & 0x80)&&(TextInput[CharUesdNum - 1] > 0))/*最后是汉字+ASC*/
        {
          if(CharUesdNum < 3)return 0;/*没有字符可画*/
          TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          CharUesdNum --;
          CharacterNum--;

          TextLineWidth -= FontWidth + FontSpace;
          
          Point[1] = TextInput[CharUesdNum - 1];
          CharUesdNum --;
          Point[0] = TextInput[CharUesdNum - 1];
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          
          CharUesdNum++;
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

          CharacterNum--;
        }
        else/*最后是ASC*/
        {
          TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          Point[1] = TextInput[CharUesdNum - 1];

          CharUesdNum--;
          CharacterNum--;
          
          TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          Point[0] = TextInput[CharUesdNum - 1];

          CharacterNum--;
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
          
          CharUesdNum++;
          
          TextInput[CharUesdNum - 1] = '.';
          TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
        }
        PointFlag = TRUE;
      }
    }
    else if ('\0' != TextInput[CharUesdNum])/*正常范围内*/
    {
      CharUesdNum++;/*跳过所有非结束符的非打印字符，如'\n'*/
    }
    
    TextStyle &= ~TEXT_TEXTER_FLAG;
  }

  VaSpace = SpaceWidth - TextLineWidth;

  /*下面代码输出字符*/
  if(CharUesdNum > 0)
  {/*有字符输出*/
    gprim_setfcol(crFore);
    gprim_setbcol(crBack);

    switch(TextStyle)
    {
      case TEXT_UP_LEFT:
        Xstart = Rect.x0;
        Ystart = Rect.y0;
        break;
      case TEXT_UP_MID:
        Xstart = Rect.x0 + (VaSpace >> 1);
        Ystart = Rect.y0;
        break;
      case TEXT_UP_RIGHT:
        Xstart = Rect.x1 - TextLineWidth;
        Ystart = Rect.y0;
        break;
      case TEXT_MID_LEFT:
        Xstart = Rect.x0;
        Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
        break;
      case TEXT_MID_MID:
        Xstart = Rect.x0 + (VaSpace >> 1);
        Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
        break;
       case TEXT_MID_RIGHT:
        Xstart = Rect.x1 - TextLineWidth;
        Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
        break;
      case TEXT_DOWN_LEFT:
        Xstart = Rect.x0;
        Ystart = Rect.y1 - TextLineHigh;
        break;
      case TEXT_DOWN_MID:
        Xstart = Rect.x0 + (VaSpace >> 1);
        Ystart = Rect.y1 - TextLineHigh;
        break;
      case TEXT_DOWN_RIGHT:
        Xstart = Rect.x1 - TextLineWidth;
        Ystart = Rect.y1 - TextLineHigh;
        break;
      case TEXT_LINE_FULL:
        Xstart = Rect.x0;
        Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
        break;
      default:
        CharUesdNum = 0;
        break;
    }

    if (CharUesdNum > 0)
    {
      if ((TEXT_LINE_FULL != TextStyle) 
          || ('\0' == TextInput[CharUesdNum])
          || ('\n' == TextInput[CharUesdNum - 1]))
      {
        DRV_OSDTextLineDraw(region, 
                                            TextInput,
                                            CharUesdNum,
                                            Xstart,
                                            Ystart,
                                            wbs,
                                            hbs,
                                            FontSpace,
                                            crFore);
      }
      else
      {
        S32 i=0;
        S32 iDistance = 0;
        char asc[3] = {0, 0, 0};
        S32 ACom, Quot;
        S32 PrevChar;
        
        ACom = VaSpace % CharacterNum;
        Quot = VaSpace / CharacterNum;
        PrevChar = (CharacterNum - ACom) >> 1;
        
        for (i = 0; i < CharUesdNum; i++)
        {
          if(TextInput[i] & 0x80)
          {
            memcpy(asc, &(TextInput[i]), 2);
            i++;
          }
          else
          {
            asc[0] = TextInput[i];
            asc[1] = '\0';
          }
          
          iDistance += Quot + FontSpace + DRV_OSDCharacterDraw(region, 
                                                                                        asc, 
                                                                                        Xstart,
                                                                                        iDistance, 
                                                                                        Ystart, 
                                                                                        wbs, 
                                                                                        hbs, 
                                                                                        crFore,
                                                                                        NULL,
                                                                                        NULL);
            
          if (0 < PrevChar)
          {
            PrevChar--;
          }
          else if (0 != ACom)
          {
            ACom--;
            iDistance++;
          }
        }
      }
    }

    /*省略号恢复为原数据*/
    if ((PointFlag) && (0 != CharUesdNum))
    {
      TextInput[CharUesdNum - 1] = Point[1];
      CharUesdNum --;
      TextInput[CharUesdNum - 1] = Point[0];
      CharUesdNum --;//去掉省略号占用的字数
    }
  }

  return CharUesdNum;
}



S32 DRV_OSDTexterDraw(OSD_Region_t *region,char *TextInput,
                            S32 nFontSize,
                            S32 nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle,
                            S32 crFore,
                            S32 crBack)
{
  Rect_t Rect;
  S32 CharUesdNum = 0;
  S32 SpaceWidth, SpaceHigh;
  S32 TextLineHigh = 0;
  S32 FontWidth, FontHigh;
  S32 wbs = 1;
  S32 hbs = 1;

  S32 LineSpace = 0;
  S32 FontSpace = 0;
  S32 LeftSpace = 0;
  S32 RightSpace = 0;
  S32 TopSpace = 0;
  S32 BottomSpace = 0;
  
  if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
    return CharUesdNum;

  Rect = *rect;

  if (NULL != TexterStyle)
  {
    LineSpace = TexterStyle->LineSpace;
    FontSpace = TexterStyle->FontSpace;
    LeftSpace = TexterStyle->LeftSpace;
    RightSpace = TexterStyle->RightSpace;
    TopSpace = TexterStyle->TopSpace;
    BottomSpace = TexterStyle->BottomSpace;
  }
    
  SpaceWidth = Rect.x1 - Rect.x0 + 1 - LeftSpace - RightSpace;
  SpaceHigh = Rect.y1 - Rect.y0 + 1 - TopSpace - BottomSpace;

  FontWidth = nFontSize * wbs;
  FontHigh = nFontSize * hbs;

  TextLineHigh = FontHigh;

  Rect.x0 += LeftSpace;
  Rect.x1 -= RightSpace;
  Rect.y0 += TopSpace;
  Rect.y1 = Rect.y0;

  TextStyle |= TEXT_TEXTER_FLAG;//区分后续省略符

  for(CharUesdNum = 0; 
            ((TextLineHigh <= SpaceHigh)
            && ('\0' != TextInput[CharUesdNum]));/*结束符被返回*/ 
        TextLineHigh += FontHigh + LineSpace)
  {
    Rect.y0 = Rect.y1;
    Rect.y1 = Rect.y0 + FontHigh;
    
    CharUesdNum += DRV_TextLineDraw(region,&(TextInput[CharUesdNum]),
                                                    nFontSize,
                                                    nFontStyle, 
                                                    wbs, 
                                                    hbs, 
                                                    &Rect,
                                                    TextStyle,
                                                    FontSpace,
                                                    crFore,
                                                    crBack);
    Rect.y1 += LineSpace;
  }

  return CharUesdNum;
}
/*******************************************************************************
Name        : OsdYcrcb2Argb
Description : Convert the palette entries' color format 
Parameters  :
Assumptions : 
Limitations :   OsdYcrcb2Argb
Returns     : 
*******************************************************************************/
void DRV_OSDYcrcb2Argb(const CAST_OSD_Color_t *Ycrcb_p,
                                S32 *Argb_p)
{
    S32 A_YCBCR,Y,Cb,Cr;
    S32 A_RGB,R,G,B;

    A_YCBCR = Ycrcb_p->MixWeight;
    Y       = Ycrcb_p->Value.Y;
    Cb      = Ycrcb_p->Value.Cb;
    Cr      = Ycrcb_p->Value.Cr;

    if(Y == 0 && Cr == 0 && Cb == 0)
    {
        A_RGB = A_YCBCR <<2;
        R     = 0;
        G     = 0;
        B     = 0;
    }
    else
    {
        if(Y < 16)
             Y = 16;
        else if(Y > 235)
             Y = 235;
        if(Cb < 16)
             Cb = 16;
        else if(Cb > 240)
             Cb = 240;
        if(Cr < 16)
             Cr = 16;
        else if(Cr > 240)
             Cr = 240;

        A_RGB = A_YCBCR << 2;//aplpha分量只有64级,扩展成256级
        R     = (298*(Y - 16) + 409*(Cr - 128))/256;
        G     = (298*(Y - 16) - 208*(Cr - 128) - 100*(Cb - 128))/256;
        B     = (298*(Y - 16) + 517*(Cb - 128))/256;

        if(R < 0)
             R = 0;
         else if(R > 255)
             R = 255;
        if(G < 0)
             G = 0;
        else if(G > 255)
             G = 255;
        if(B < 0)
             B = 0;
        else if(B > 255)
             B = 255;
    }

    *Argb_p = (((U8)A_RGB)<<24 | ((U8)R)<< 16|((U8)G)<<8|(U8)B);
}



#if 1/*新文字输出，可形成独立的文本控件*/

int MidGetTextLineWidth(char *TextInput,
                                  int nFontSize,
                                  int nFontStyle, 
                                  int wbs, 
                                  int hbs, 
                                  Rect_t *rect,
                                  TEXT_TextStyle_e TextStyle,
                                  int FontSpace)
{
    Rect_t Rect;
    int CharUesdNum = 0;
    int SpaceWidth, SpaceHigh;
    int TextLineWidth = 0;
    int TextLineHigh = 0;
    int FontWidth, FontHigh;
    int VaSpace;
    int CharacterNum = 0;

    int Xstart;
    int Ystart;

    int PointFlag = FALSE;
    char Point[3] = {0};

    if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
        return CharUesdNum;

    Rect = *rect;

    SpaceWidth = Rect.x1 - Rect.x0 + 1 + /*补间隔*/FontSpace;
    SpaceHigh = Rect.y1 - Rect.y0 + 1;

    FontWidth = nFontSize * wbs;
    FontHigh = nFontSize * hbs;

    TextLineHigh = FontHigh;

    /*下面代码计算可输出字符个数及输出宽度*/
    for(CharUesdNum = 0; 
        ((TextLineWidth <= SpaceWidth) 
        //&& (TextLineHigh <= SpaceHigh)
        && ('\0' != TextInput[CharUesdNum])
        && ('\n' != TextInput[CharUesdNum])); 
        CharUesdNum++)
    {
        if(TextInput[CharUesdNum] & 0x80)/*汉字*/
        {
            CharUesdNum++;/*补齐汉字*/
            TextLineWidth += FontWidth + FontSpace;
            CharacterNum++;
        }
        else/*ASCII码*/
        {
            if(0X9 == TextInput[CharUesdNum])/*tab键处理,调整宽度范围为1~40*/
            {
                TextLineWidth += 40 - TextLineWidth%40;
            }
            else if ((TEXT_TEXTER_FLAG & TextStyle) && ((('a' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'z')) || (('A' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'Z'))))
            {

                int tmpWidth;
                int tmpUsedNum;
                int tmpCharNum;
		int WordWidth;
		int WordIndex;

                tmpWidth = TextLineWidth;
                tmpUsedNum = CharUesdNum - 1;/*去掉本单词首字母*/
                tmpCharNum = CharacterNum - 1;

                TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
                CharacterNum++;
                CharUesdNum++;

                while((TextLineWidth <= SpaceWidth) && ((('a' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'z')) || (('A' <= TextInput[CharUesdNum]) && (TextInput[CharUesdNum] <= 'Z'))))
                {
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
                    CharacterNum++;
                    CharUesdNum++;
                }
		
		if (TextLineWidth > SpaceWidth)
		{
			WordWidth = TextLineWidth - tmpWidth;
			WordIndex = CharUesdNum;
			while((('a' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= 'z')) 
				|| (('A' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= 'Z'))
				|| (('0' <= TextInput[WordIndex]) && (TextInput[WordIndex] <= '9')))
			{
				WordWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
				WordIndex++;
			}
			/*如果一个word所占用的宽度大于行宽，则分为两行来画*/
			if ((0 != tmpWidth)&&(WordWidth < SpaceWidth))
			{
				/*防止单词过长，单行显示不下*/
				TextLineWidth = tmpWidth;
				CharUesdNum = tmpUsedNum;
				CharacterNum = tmpCharNum;
			}
		break;
                }

                CharacterNum--;//对应上面的++
                CharUesdNum--;//对应上面的++

            }
            else
            {
                TextLineWidth += gFontWidth[TextInput[CharUesdNum]] * wbs + FontSpace;
            }
            CharacterNum++;
        }  
    }

    if('\n' == TextInput[CharUesdNum])
        CharacterNum++;/*回车计数个数*/
    
    if(CharacterNum > 0) 
    {
        if (TextLineWidth > SpaceWidth)/*最后字符超出范围*/
        {  
            CharUesdNum--;/*恢复为最后字符下标*/
            CharacterNum--;
            /* 最后两个字符均大于0x80即为汉字(GB & BIG5) */
            /*TextInput[CharUesdNum]为最后一个asc字符*/
            if((TextInput[CharUesdNum -1] & 0x80)&&(TextInput[CharUesdNum] & 0x80)&&(CharUesdNum > CharacterNum))
            {
                TextLineWidth -= FontWidth + FontSpace;
                CharUesdNum --;/*回退一个汉字*/
            }
            else
            {
                TextLineWidth -= gFontWidth[TextInput[CharUesdNum]] + FontSpace;
            }

            if(CharacterNum < 1)return 0;/*没有字符可画*/

            /*超出部分用省略号,针对GB2312*/
            if (!(TEXT_TEXTER_FLAG & TextStyle))
            {
                if(CharUesdNum < 2)return 0;/*没有空间可画*/
                if((TextInput[CharUesdNum - 2] & 0x80)&&(TextInput[CharUesdNum -1] & 0x80))/*最后一个是汉字*/
                {
                    TextLineWidth -= FontWidth + FontSpace;

                    Point[1] = TextInput[CharUesdNum - 1];
                    CharUesdNum --;
                    Point[0] = TextInput[CharUesdNum - 1];

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharacterNum--;

                }
                else if ((TextInput[CharUesdNum - 3] & 0x80)&&(TextInput[CharUesdNum-2] & 0x80)&&(TextInput[CharUesdNum - 1] > 0))/*最后是汉字+ASC*/
                {
                    if(CharUesdNum < 3)return 0;/*没有字符可画*/
                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    CharUesdNum --;
                    CharacterNum--;

                    TextLineWidth -= FontWidth + FontSpace;

                    Point[1] = TextInput[CharUesdNum - 1];
                    CharUesdNum --;
                    Point[0] = TextInput[CharUesdNum - 1];

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharacterNum--;
                }
                else/*最后是ASC*/
                {
                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    Point[1] = TextInput[CharUesdNum - 1];

                    CharUesdNum--;
                    CharacterNum--;

                    TextLineWidth -= gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                    Point[0] = TextInput[CharUesdNum - 1];

                    CharacterNum--;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;

                    CharUesdNum++;

                    TextInput[CharUesdNum - 1] = '.';
                    TextLineWidth += gFontWidth[TextInput[CharUesdNum - 1]] + FontSpace;
                }
                PointFlag = TRUE;
            }
        }
        else if ('\0' != TextInput[CharUesdNum])/*正常范围内*/
        {
            CharUesdNum++;/*跳过所有非结束符的非打印字符，如'\n'*/
        }

        TextStyle &= ~TEXT_TEXTER_FLAG;
    }

    VaSpace = SpaceWidth - TextLineWidth;

    /*下面代码输出字符*/
    if(CharUesdNum > 0)
    {/*有字符输出*/
        switch(TextStyle)
        {
            case TEXT_UP_LEFT:
                Xstart = Rect.x0;
                Ystart = Rect.y0;
                break;
            case TEXT_UP_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = Rect.y0;
                break;
            case TEXT_UP_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = Rect.y0;
                break;
            case TEXT_MID_LEFT:
                Xstart = Rect.x0;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_MID_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_MID_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            case TEXT_DOWN_LEFT:
                Xstart = Rect.x0;
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_DOWN_MID:
                Xstart = Rect.x0 + (VaSpace >> 1);
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_DOWN_RIGHT:
                Xstart = Rect.x1 - TextLineWidth;
                Ystart = Rect.y1 - TextLineHigh;
                break;
            case TEXT_LINE_FULL:
                Xstart = Rect.x0;
                Ystart = (((Rect.y1 + Rect.y0) + 1 - TextLineHigh) >> 1);
                break;
            default:
                CharUesdNum = 0;
                break;
        }

        if (CharUesdNum > 0)
        {
            if ((TEXT_LINE_FULL != TextStyle) || ('\0' == TextInput[CharUesdNum]) || ('\n' == TextInput[CharUesdNum - 1]))
            {
            }
            else
            {
                int i=0;
                int iDistance = 0;
                char asc[3] = {0, 0, 0};
                int ACom, Quot;
                int PrevChar;

                ACom = VaSpace % CharacterNum;
                Quot = VaSpace / CharacterNum;
                PrevChar = (CharacterNum - ACom) >> 1;

                for (i = 0; i < CharUesdNum; i++)
                {
                    if(TextInput[i] & 0x80)
                    {
                        memcpy(asc, &(TextInput[i]), 2);
                        i++;
                    }
                    else
                    {
                        asc[0] = TextInput[i];
                        asc[1] = '\0';
                    }


                    if (0 < PrevChar)
                    {
                        PrevChar--;
                    }
                    else if (0 != ACom)
                    {
                        ACom--;
                    }
                }
            }
        }

        /*省略号恢复为原数据*/
        if ((PointFlag) && (0 != CharUesdNum))
        {
            TextInput[CharUesdNum - 1] = Point[1];
            CharUesdNum --;
            TextInput[CharUesdNum - 1] = Point[0];
            CharUesdNum --;//去掉省略号占用的字数
        }
    }

    return CharUesdNum;
}

int MidGetTextWidth(char *TextInput,
                            int nFontSize,
                            int nFontStyle,
                            Rect_t *rect,
                            TEXT_TextStyle_e TextStyle,
                            TEXT_TexterStyle_t *TexterStyle)
{
  Rect_t Rect;
  int CharUesdNum = 0;
  int SpaceWidth, SpaceHigh;
  int TextLineHigh = 0;
  int FontWidth, FontHigh;
  int wbs = 1;
  int hbs = 1;

  int LineSpace = 0;
  int FontSpace = 0;
  int LeftSpace = 0;
  int RightSpace = 0;
  int TopSpace = 0;
  int BottomSpace = 0;
  
  if ((NULL == TextInput) || (nFontSize <= 0) || (NULL == rect))
    return CharUesdNum;

  Rect = *rect;

  if (NULL != TexterStyle)
  {
    LineSpace = TexterStyle->LineSpace;
    FontSpace = TexterStyle->FontSpace;
    LeftSpace = TexterStyle->LeftSpace;
    RightSpace = TexterStyle->RightSpace;
    TopSpace = TexterStyle->TopSpace;
    BottomSpace = TexterStyle->BottomSpace;
  }
    
  SpaceWidth = Rect.x1 - Rect.x0 + 1 - LeftSpace - RightSpace;
  SpaceHigh = Rect.y1 - Rect.y0 + 1 - TopSpace - BottomSpace;

  FontWidth = nFontSize * wbs;
  FontHigh = nFontSize * hbs;

  TextLineHigh = FontHigh;

  Rect.x0 += LeftSpace;
  Rect.x1 -= RightSpace;
  Rect.y0 += TopSpace;
  Rect.y1 = Rect.y0;

  TextStyle |= TEXT_TEXTER_FLAG;//区分后续省略符

  for(CharUesdNum = 0; 
            ((TextLineHigh <= SpaceHigh)
            && ('\0' != TextInput[CharUesdNum]));/*结束符被返回*/ 
        TextLineHigh += FontHigh + LineSpace)
  {
    Rect.y0 = Rect.y1;
    Rect.y1 = Rect.y0 + FontHigh;
    
    CharUesdNum += MidGetTextLineWidth(&(TextInput[CharUesdNum]),
                                                    nFontSize,
                                                    nFontStyle, 
                                                    wbs, 
                                                    hbs, 
                                                    &Rect,
                                                    TextStyle,
                                                    FontSpace);
    Rect.y1 += LineSpace;
  }

  return CharUesdNum;
}
#endif
