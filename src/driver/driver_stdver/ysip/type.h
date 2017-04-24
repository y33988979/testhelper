/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_type.h : dhcp typedef head file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _IFCFG_TYPE_H_
#define _IFCFG_TYPE_H_

#define __LINUX__

#if 1/*(!defined(__STSYSTEM__))*/
#ifndef u8_t
typedef unsigned   char    u8_t;
#endif
#ifndef s8_t
typedef signed     char    s8_t;
#endif
#ifndef u16_t
typedef unsigned   short   u16_t;
#endif
#ifndef s16_t
typedef signed     short   s16_t;
#endif
#ifndef u32_t
typedef unsigned   long    u32_t;
#endif
#ifndef s32_t
typedef signed     long    s32_t;
#endif
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE  (1 == 1)
#endif

#ifndef FALSE
#define FALSE  (!TRUE)
#endif

#endif /* _IFCFG_TYPE_H_ */

