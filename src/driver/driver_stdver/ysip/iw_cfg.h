/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* iw_cfg.h : wireless interface cfg head file

* version: 	1.0
* author: 	ywg
* time:	2009-08-07

* history:	none
**************************************************************************/

#ifndef __IW_CFG_H__
#define __IW_CFG_H__

#include "wireless.h"
#include "compat.h"
#include "iwcommon.h"

#define IWCFG_EXTENTIONAL   0

typedef struct iwcfg_info{
    char *essid;
    char *apaddr;
    char *nwid;
    char *mode;
    char *sens;
    char *freq;
    char *channel;
    char *nick;
    char *rate;
    char *rts;
    char *frag;
    char *enc;
    char *power;
    char *retry;
    char *txpower;
    char *modulation;
}iwcfg_info_t;

int IWCFG_SetEssid(int skfd, char *ifname, char *p_essid);

/*
    set ap addr
*/
int IWCFG_SetApaddr(int skfd, char *ifname, char *apddr);

/*
    set nwid
    off/any/on/specify
*/
int IWCFG_SetNwid(int skfd, char *ifname, char *nwid);

/*
    set mode
*/
int IWCFG_SetMode(int skfd, char *ifname, char *mode);

/*
    set sensitivity
*/
int IWCFG_SetSens(int skfd, char *ifname, char *sens);

/*
    set freq
*/
int IWCFG_SetFreq(int skfd, char *ifname, char *p_freq);

/*
    set channel
*/
int IWCFG_SetChannel(int skfd, char *ifname, char *channel);

/*
    specify ap mac or id
*/

/*
    set nick
*/
int IWCFG_SetNick(int skfd, char *ifname, char *nick);

/*
    set rate
*/
int IWCFG_SetRate(int skfd, char *ifname, char *rate);

/*
    set rts
*/
int IWCFG_SetRts(int skfd, char *ifname, char *rts);

/*
    set frag
*/
int IWCFG_SetFrag(int skfd, char *ifname, char *frag);

/*
    specify enc (key)
*/
int IWCFG_SetEnc(int skfd, char *ifname, char *keyflag, char *keyvalue);

#if IWCFG_EXTENTIONAL
/*
    set power
*/
int IWCFG_SetPower(int skfd, char *ifname, char *power, char *mode);

/*
    set retry
*/
int IWCFG_SetRetry(int skfd, char *ifname, char *retry);

/*
    set txpower
*/
int IWCFG_SetTXPower(int skfd, char *ifname, char *txpower);

int IWCFG_SetModulation(int skfd, char *ifname, char *modulation);
#endif

int IWCFG_SetCommit(int skfd, char *ifname, struct iwreq *wrq);

/*
 * Set the wireless options requested on command line
 * This function is too long and probably should be split,
 * because it look like the perfect definition of spaghetti code,
 * but I'm way to lazy
 */
int IWCFG_SetInfo(char *ifname, 
                        char *pessid,
                        char *apaddr,
                        char *nwid,
                        char *mode,
                        char *sens,
                        char *freq,
                        char *nick,
                        char *rate,
                        char *rts,
                        char *frag,
                        char *enc);

/*
    connect to ap
    essid
    key
    managed or ad-hoc
    ...
*/
int IWCFG_ConnectAp(char *ifname, iwcfg_info_t *info);


/*
    get series
*/


/*
 * Get wireless informations & config from the device driver
 * We will call all the classical wireless ioctl on the driver through
 * the socket to know what is supported and to get the settings...
 */
int IWCFG_GetInfo(int skfd, char *ifname, struct wireless_info *info);

/*
    print all infs ' info
*/
void IWCFG_PrintInfo(int skfd, char *ifname);

/*
    display all wireless interfaces 
*/
int IWCFG_DisplayALLIfs();

/*
    scanning wireless network
    implement in iwlist
*/
int IWCFG_ScanningIf(int skfd, char *ifname, int we_version, wireless_scan_head *context);

#endif /* __IW_CFG_H__ */

