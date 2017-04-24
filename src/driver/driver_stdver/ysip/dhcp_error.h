/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_error.h : dhcp error definition file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#ifndef _DHCP_ERROR_H_
#define _DHCP_ERROR_H_

#ifndef dhcp_err_t
typedef signed char  dhcp_err_t;
#endif

/* dhcp error code */
/* Definitions for error constants. */
#define DHCP_ERR_OK    0      /* No error, everything OK. */
#define DHCP_ERR_MEM  -1      /* Out of memory error.     */
#define DHCP_ERR_BUF  -2      /* Buffer error.            */


#define DHCP_ERR_ABRT -3      /* Connection aborted.      */
#define DHCP_ERR_RST  -4      /* Connection reset.        */
#define DHCP_ERR_CLSD -5      /* Connection closed.       */
#define DHCP_ERR_CONN -6      /* Not connected.           */

#define DHCP_ERR_VAL  -7      /* Illegal value.           */

#define DHCP_ERR_ARG  -8      /* Illegal argument.        */

#define DHCP_ERR_RTE  -9      /* Routing problem.         */

#define DHCP_ERR_USE  -10     /* Address in use.          */

#define DHCP_ERR_IF   -11     /* Low-level netif error    */
#define DHCP_ERR_ISCONN -12   /* Already connected.       */



#endif /* _DHCP_ERROR_H_ */

