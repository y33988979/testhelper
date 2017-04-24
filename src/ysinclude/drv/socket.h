/*****************************************************************************
*
* Description: 
*    socket
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2012-09-12    wrwei          ´´½¨
* 
*
*****************************************************************************/
#ifndef _SOCKET_H_
#define _SOCKET_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

#include "stddefs.h"
//#include "type.h"
//#include "dhcp_client.h"
HI_S32 DRV_Socket_CreateXML(void);
HI_S32 DRV_Socket_CreateServer(HI_S16 Type,HI_S16 Port);
HI_S32 DRV_Socket_CreateClient(HI_S16 Type,HI_CHAR *IP,HI_U8 Speed_flag);

#ifdef __cplusplus
}
#endif
#endif

