/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : image.h

    Description :

    Others:

    History:   1. wwqing creat 2012-10-17


*******************************************************************************/
#ifndef	_IMAGE_H_
#define	_IMAGE_H_

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif
		
#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3
		
#define IMG_SBITMAP_WIDE 120
#define IMG_SBITMAP_HEIGHT 100
		
#define IMG_BITMAP_WIDE 1920
#define IMG_BITMAP_HEIGHT 1080
		
/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct Bmp_Bitmap_s
{
	OSD_ColorType_t  ColorType;
	U8				   BitsPerPel;
	int ColorNum;
	U32 			   Width;/*Bitmap的高度*/
	U32 			   Height;/*Bitmap的宽度*/
	U32 			   Pitch;/*上下两个像素间隔的像素数*/
	void*			   Data_p;
	void*			   Palette;
	int 			   Compression;
} Bmp_Bitmap_t;
	
typedef struct tagBITMAPFILEHEADER 
{
	unsigned int		bfSize; 
	unsigned int		bfReserved; 
	unsigned int		bfOffBits; 
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	int 			biWidth; 
	int 			biHeight; 
	unsigned short	biPlanes; 
	unsigned short	biBitCount ;
	unsigned int	biCompression; 
	unsigned int	biSizeImage; 
	int 			biXPelsPerMeter; 
	int 			biYPelsPerMeter; 
	unsigned int	biClrUsed; 
	unsigned int	biClrImportant; 
} BITMAPINFOHEADER; 

typedef struct tagRGBQUAD 
{	
	unsigned char	 Blue; /*该颜色的蓝色分量*/
	unsigned char	 Green;  /*该颜色的绿色分量*/
	unsigned char	 Red;  /*该颜色的红色分量*/
	unsigned char	 Alph;	/*该颜色的透明度分量*/	
} RGBQUAD;

typedef struct tagRGBAINDEX
{ 
	unsigned int	Rindex;
	unsigned int	Gindex; 
	unsigned int	Bindex; 
	unsigned int	Aindex; 
} RGBAINDEX;

typedef RGBQUAD* LPRGBQUAD;

typedef struct tagLPBITMAPINFOHEADER
{
	BITMAPFILEHEADER	FileHeader;
	unsigned int		biSize; 
	BITMAPINFOHEADER	FileInfo;
	RGBAINDEX			RGBAIndex;
}LPBITMAPINFOHEADER;


typedef struct tagHFILE 
{
	unsigned char* Data;
	unsigned int Length;
	unsigned char* Cur;
	unsigned int Count;
}HFILE ;

typedef void* LpPalette;


/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/
/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_InitMem
  Description	  : 申请处理图片数据的内存
  Input 		  : Partition
  Outut 		  : 成功返回0，失败返回-1
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_InitMem(DRVOS_Partition_t *Partition);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_FreeMem
  Description	  : 释放处理图片数据的内存
  Input 		  : 
  Outut 		  : 
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
void IMG_FreeMem(void);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_FreeImage
  Description	  : 停止并释放gif
  Input 		  : 
  Outut 		  : 
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_FreeImage(void* FileData,int FileSize,int FileType);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_StopImage
  Description	  : 暂停gif
  Input 		  : 
  Outut 		  : 成功返回0，失败返回-1
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_StopImage(void* FileData,int FileSize,int FileType);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_PreviewImage
  Description	  : 显示图片缩略图
  Input 		  : 
  Outut 		  : 成功返回0，失败返回-1
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_PreviewImage(OSD_Region_t* Region_p,OSD_Rect_t* Rect_p,void* FileData,int FileSize,int FileType,OSD_ColorType_t ColorType,int Angle);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_PreviewImage
  Description	  : 显示图片
  Input 		  : 
  Outut 		  : 成功返回0，失败返回-1
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_DrawImage(OSD_Region_t* Region_p,OSD_Rect_t* Rect_p,void* FileData,int FileSize,int FileType,OSD_ColorType_t ColorType,int Angle);

/*****************************************************************************
  Date&Time 	  : 2012-10-17
  Author		  :    wwqing
  Function Name   : IMG_ShowImage
  Description	  : 显示图片
  Input 		  : 
  Outut 		  : 成功返回0，失败返回-1
  Calls 		  :
  Called by 	  :
  Global Variables:
  Others		  :
*****************************************************************************/
int IMG_ShowImage(OSD_Region_t* Region_p,OSD_Rect_t* Rect_p,void* FileData,int FileSize,int FileType,BOOL bPreview,OSD_ColorType_t ColorType,int Angle);

#endif
