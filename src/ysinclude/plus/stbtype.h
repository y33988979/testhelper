/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : stbtype.h

    Description : 变量类型定义

    Others:      

    History:  1. yzb creat 2004-3-29
			  2. xyan Modify 2011-11-14

*******************************************************************************/
#ifndef _STBTYPE_H
#define _STBTYPE_H

/*----------------------------------------------------------------------------
 *	Include
 *----------------------------------------------------------------------------*/
#include "stddefs.h"

/*----------------------------------------------------------------------------
 *	Define
 *----------------------------------------------------------------------------*/
#define CHAR 	signed char
#define BYTE 	unsigned char
#define SHORT 	signed short
#define USHORT 	unsigned short
#define LONG 	long
#define ULONG	unsigned long
#define INT		signed int
#define UINT	unsigned int
#define DWORD	unsigned int
#define UI32	unsigned int
#define UI16	unsigned short

#define S8		signed char
#define S16		signed short
#define S32		signed int  
#define U8		unsigned char
#define U16		unsigned short
#define U32		unsigned int 

#define Msg_Queue DRVOS_MsgQueue_t  
#define MSG_ID	unsigned int
#define KEY_CODE unsigned short
#define HRESULT	unsigned int
#define NET_ID	unsigned short
#define TS_ID	unsigned short
#define PRG_ID	unsigned short
#define SERVICE_ID unsigned short
#define COMP_ID	unsigned short
#define EVENT_ID unsigned short
#define DataNO 	signed short
#define DVBPID unsigned short

#ifndef bool
#define bool 	char
#endif

#ifndef BOOLEAN
#define BOOLEAN	int
#endif

#ifndef BOOL
#define BOOL	int
#endif

#ifndef NULL
#define NULL  (void*)0
#endif

#ifndef INVALID_CODE
#define INVALID_CODE  0xFFFFFFFF
#endif

#ifndef TRUE
#define TRUE  (1 == 1)
#endif

#ifndef FALSE
#define FALSE  (!TRUE)
#endif

#ifndef SUCCESS 
#define SUCCESS  0
#endif

#ifndef FAILURE
#define FAILURE  -1
#endif

/*----------------------------------------------------------------------------
 *	Struct
 *----------------------------------------------------------------------------*/
typedef struct MessageInfo 
{
	unsigned int	nMsgCode;	
	unsigned int	nHWord;		
	unsigned int	nMWord;		
	unsigned int	nLWord;		
} MessageInfo_t;

typedef struct SizeRect 
{
	int x;
	int y;
	unsigned int nWidth;
	unsigned int nHeight;
} SizeRect_t;

typedef	union
{
	unsigned int uiWord32;
	struct
	{
		unsigned char ucByte0;	/* LSB */
		unsigned char ucByte1;
		unsigned char ucByte2;
		unsigned char ucByte3;	/* MSB */
	} byte;
} WORD2BYTE;

typedef	union 
{
    unsigned int uiWord32;
    struct 
    {
        unsigned short sLo16;
        unsigned short sHi16;
    } unShort;
} WORD2SHORT;

typedef	union
{
	unsigned short sWord16;
	struct
	{
		unsigned char ucByte0;	/* LSB */
		unsigned char ucByte1; /* MSB */
	} byte;
} SHORT2BYTE;


#endif   

