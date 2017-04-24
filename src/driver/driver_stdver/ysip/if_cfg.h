/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* if_cfg.c : head files

* version: 	1.0
* author: 	ywg
* time:	2009-08-07

* history:	none
**************************************************************************/
#ifndef __IF_CFG_H__
#define __IF_CFG_H__

#include "ip_cfg.h"
#include "wireless.h"
#include "iwcommon.h"
#include "iw_cfg.h"

typedef struct if_table{
    unsigned int ip_addr;
    unsigned int gw_addr;
    unsigned int mk_addr;
}if_table_t;

#endif /* __IF_CFG_H__ */

