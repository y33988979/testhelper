/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* if_cfg.c : interface cfg file

* version: 	1.0
* author: 	ywg
* time:	2009-08-07

* history:	none
**************************************************************************/

#include "stdio.h"
#include "if_cfg.h"

#define IFCFG_DEBUG printf

static if_table_t IF_TABLES[10];

/* interfaces */

/* 
    display all interfaces and status 

    like this:

    eth0:
    eth1:
    ...
    lp:

    wlan0:
    wlan1:
    ...
*/

/*
    interface up (index)
*/

/*
    interface down (index)
*/

/*
    switch (index1, index2)
    down index1 and up index2
*/

/*------------------------------------------------------------------*/
/*
 * Enumerate devices and call specified routine
 * The new way just use /proc/net/wireless, so get all wireless interfaces,
 * whether configured or not. This is the default if available.
 * The old way use SIOCGIFCONF, so get only configured interfaces (wireless
 * or not).
 */
int IFCFG_ShowDevices(int flag)
{
    char buff[1024];
    FILE *fh;
    struct ifconf ifc;
    struct ifreq *ifr;
    int		i;
    int skfd;

    if((skfd = iw_sockets_open()) < 0)
    {
        IFCFG_DEBUG("IFCFG_ShowDevices : error \n");
        return -1;
    }
    
    if(flag){
#ifndef IW_RESTRIC_ENUM
        /* Check if /proc/net/dev is available */
        fh = fopen(PROC_NET_DEV, "r");
#else
        /* Check if /proc/net/wireless is available */
        fh = fopen(PROC_NET_WIRELESS, "r");
#endif

        if(fh != NULL)
        {
            /* Success : use data from /proc/net/wireless */
    	IFCFG_DEBUG("Success : use data from /proc/net/wireless\n");

            /* Eat 2 lines of header */
            fgets(buff, sizeof(buff), fh);
            fgets(buff, sizeof(buff), fh);

            /* Read each device line */
            while(fgets(buff, sizeof(buff), fh))
            {
                char	name[IFNAMSIZ + 1];
                char *s;

                /* Skip empty or almost empty lines. It seems that in some
                * cases fgets return a line with only a newline. */
                if((buff[0] == '\0') || (buff[1] == '\0'))
                    continue;

                /* Extract interface name */
                s = iw_get_ifname(name, sizeof(name), buff);
                IFCFG_DEBUG("extract interface name %s \n", name);

                if(!s)
                {
                    /* Failed to parse, complain and continue */
#ifndef IW_RESTRIC_ENUM
                    fprintf(stderr, "Cannot parse " PROC_NET_DEV "\n");
#else
                    fprintf(stderr, "Cannot parse " PROC_NET_WIRELESS "\n");
#endif
                }
                else
                    /* Got it, print info about this interface */
                    //(*fn)(skfd, name, args, count);
                    IWCFG_PrintInfo(skfd, name);
            }

            fclose(fh);
        }
    }
    else
    {
        /* Get list of configured devices using "traditional" way */
        ifc.ifc_len = sizeof(buff);
        ifc.ifc_buf = buff;
        if(ioctl(skfd, SIOCGIFCONF, &ifc) < 0)
        {
            fprintf(stderr, "SIOCGIFCONF: %s\n", strerror(errno));
            return;
        }
        ifr = ifc.ifc_req;

        /* Print them */
        for(i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; ifr++)
            //(*fn)(skfd, ifr->ifr_name, args, count);
            IWCFG_PrintInfo(skfd, ifr->ifr_name);
    }
    iw_sockets_close(skfd);
}

void IFCFG_DisplayIf()
{
    IWCFG_DisplayALLIfs();
    return;
}

void IFCFG_Init()
{
    IPCFG_Init();
    return;
}

/*
* first get all interfaces include wired(eth0) wireless(wlan0) loopback(l0) ppp...
*
* second select a interface to config
*	eth0 get ip change ip ...
*	wlan0 scanning set essid ... get ip ...
*
* third switch to a interface
*	if eth0 down wlan0 up eth0
*	if wlan0 down eth0 up wlan0...
*/
#define TEST_WIF    "wlan0"
int ifcfg_main(int argc, char **argv)
{
    int ret;
    int i;
    int skfd;
    wireless_scan_head context;
    wireless_scan *pwscan;
    unsigned int addr;
    struct sockaddr_in saddr;
    struct iw_range range;
    struct wireless_info info;

    char buff[128];
    char buffer[128];

    /* test dhcp tools */
    dhcp_test();

    while(1){
        sys_delay(1000);
    }
    
    /* test this wireless tool */
    IFCFG_DEBUG("\n");
    IFCFG_DEBUG("/********wireless tools...\n");
    IFCFG_DEBUG("\n");
    IFCFG_DEBUG("/********now test iw(wireless extention) tools...\n");
    IFCFG_DEBUG("\n");
    
    IFCFG_DEBUG("/********first show all devices... \n");
    //ifconfig show all interface
    //do like ifconfig
    //IFCFG_DEBUG("\n");
    //first display all devices and interfaces info...
    //IFCFG_ShowDevices(0);
    //IFCFG_ShowDevices(1);
    //IWCFG_DisplayALLIfs();
    //IFCFG_DEBUG("/********out show all devices... \n\n");
    
    //second select one and config it
    IFCFG_DEBUG("/********second config one interface \n");
    IFCFG_DEBUG("/********config eth0\n");
    //ETH0
#if 0
    addr = IPCFG_GetIpAddr("eth0");
    IFCFG_DEBUG("eth0 addr is %lx \n", addr);
    addr = IPCFG_GetSubMask("eth0");
    IFCFG_DEBUG("eth0 submask is %lx \n", addr);
    ret = IPCFG_GetGateWay(buff);
    if(ret >= 0){
        IFCFG_DEBUG("eth0 gw is %s \n", buff);
    }else{
        IFCFG_DEBUG("eth0 get gw error\n");
    }
#endif
    IFCFG_DEBUG("\n");
    
    IFCFG_DEBUG("/********config TEST_WIF \n");
    IFCFG_DEBUG("/********TEST_WIF scanning all wireless network \n");
#if 1
    //up the wireless nic
    ret = IPCFG_UpInterface(TEST_WIF);
    if(ret < 0){
        IFCFG_DEBUG("IPCFG_UpInterface : up the TEST_WIF interface error \n");
    }else{
        IFCFG_DEBUG("IPCFG_UpInterface : up the TEST_WIF interface ok \n");
        usleep(1000*10);
    }
#endif
    
    skfd = iw_sockets_open();
    if(skfd < 0){
        IFCFG_DEBUG("@@@@@@@@@@@@@@@@@@@@ error to socket open in scanning...\n");
    }

#if 0
    //get range infomation from driver 
    if(iw_get_range_info(skfd,TEST_WIF, &range) < 0){
        IFCFG_DEBUG("no frequency infomation \n");
        return 0;
    }
#endif
#if 0
    IWCFG_ScanningIf(skfd, TEST_WIF, 29, &context);
#endif
#if 0
    if(context.result != NULL){
        IFCFG_DEBUG("scanning retry %d \n", context.retry);
        pwscan = context.result;
        for(i=0; pwscan!=NULL; i++, pwscan = pwscan->next){
            IFCFG_DEBUG("CELL%d\n",i);
            if(pwscan->has_ap_addr){
               IFCFG_DEBUG("\t Access Point  %s \n", iw_sawap_ntop(&(pwscan->ap_addr),buff));
                //234567
                //IFCFG_DEBUG("\t ap addr [%lx] \n", saddr.sin_addr.s_addr);
            }
            //IFCFG_DEBUG("\t name [%s]\n", pwscan->b.name);
            if(pwscan->b.has_essid){
                IFCFG_DEBUG("\t essid [%s]\n",pwscan->b.essid);
            }
            if(pwscan->b.has_mode){
                IFCFG_DEBUG("\t mode [%s]\n",iw_operation_mode[pwscan->b.mode]);
            }
            if(pwscan->b.has_freq){
                int channel = -1;		/* Converted to channel */
                //IFCFG_DEBUG("\t channel [%d]\n", iw_freq_to_channel(pwscan->b.freq,&range));
#if 1
                /* Display frequency / channel */

                /* Some drivers insist of returning channel instead of frequency.
                * This fixes them up. Note that, driver should still return
                * frequency, because other tools depend on it. */
                if((pwscan->b.freq < KILO))
                    channel = iw_channel_to_freq((int) pwscan->b.freq, &pwscan->b.freq, &range);
                    IFCFG_DEBUG("\t channel [%d]\n", channel);
                /* Display */
                iw_print_freq(buffer, sizeof(buffer), pwscan->b.freq, -1, pwscan->b.freq_flags);
                printf("\t %s  \n", buffer); 
#endif
            }
            //encryption on or off
            if(pwscan->b.has_key){
                if(pwscan->b.key_flags & IW_ENCODE_DISABLED){
                    IFCFG_DEBUG("\t encryption key : off \n");
                }else{
                    IFCFG_DEBUG("\t encryption key: on \n");
                }
                //IFCFG_DEBUG("\t encryption on: size %d flag %lx keyis %s\n",pwscan->b.key_size, pwscan->b.key_flags, pwscan->b.key);
            }else{
                IFCFG_DEBUG("\t encryption off \n");
            }
            //sens quality...
            if(pwscan->has_stats){
                iw_print_stats(buffer, sizeof(buffer),
		                            &pwscan->stats.qual, &range, 1);
                printf("\t Link %s\n", buffer);
            }
        }
        
        /////////////////////////////////////////////////////
        // we must be free scan result by manual
        // walk through wireless scan list 
#if 1
        {
            wireless_scan *temp;
            for(pwscan = context.result; pwscan != NULL;  ){
                temp = pwscan;
                pwscan = pwscan->next;
                free(temp);
            }
            context.result = NULL;
        }
#endif
        /////////////////////////////////////////////////////
    }
#endif
    /* select a ap and set our wireless nic */
#if 0
    //if connected check essid
    if(IWCFG_GetInfo(skfd, TEST_WIF, &info) < 0){
        IFCFG_DEBUG("Interface %s may be down \n", TEST_WIF);
    }else{
        IFCFG_DEBUG("\n");
        if((info.b.has_essid && info.b.essid[0] != '\0')
            && (info.b.has_key && info.b.key[0] != '\0' )&& (info.has_ap_addr ) ){
            //we assume that wireless device has connected to a ap
            //add check access point
            IFCFG_DEBUG("/****************************/ \n");
            IFCFG_DEBUG("Interface %s has connected to a ap \n", TEST_WIF);
            IFCFG_DEBUG("/*****************************/ \n");
        }else{
            IFCFG_DEBUG("/****************************/ \n");
            IFCFG_DEBUG("Interface %s is alone -:) \n", TEST_WIF);
            IFCFG_DEBUG("/*****************************/ \n");
        }
        
        IWCFG_DisplayInfo(skfd, TEST_WIF, &info);
        IFCFG_DEBUG("\n");
    }
#endif

#if 0
    ret = IWCFG_SetEssid(skfd,TEST_WIF, "V54-HOME001");
    if(ret < 0){
        IFCFG_DEBUG("set essid error\n");
    }
    IFCFG_DEBUG("@set essid passed \n");
#endif
    
#if 1
    //set essid
    ret = IWCFG_SetEssid(skfd,TEST_WIF, "Fahai_0_0");
    if(ret < 0){
        IFCFG_DEBUG("set essid error\n");
    }
    IFCFG_DEBUG("@set essid passed \n");
    
    //set mode
    //ret = IWCFG_SetMode(skfd, TEST_WIF, "1");
    //if(ret < 0){
    //    IFCFG_DEBUG("set mode error \n");
    //}
    //IFCFG_DEBUG("@set mode passed \n");
    
    //set key
    ret = IWCFG_SetEnc(skfd, TEST_WIF, "s", "s:wirelessfahai");
    if(ret < 0){
        IFCFG_DEBUG("set enc error\n");
    }
    IFCFG_DEBUG("@set enc passed\n");
#if 0 //dont need
    //set ap
    ret = IWCFG_SetApaddr(skfd, TEST_WIF, "00:21:27:AF:C5:5A");
    if(ret < 0){
        IFCFG_DEBUG("set ap addr error\n");
    }
    IFCFG_DEBUG("@set ap addr passed\n");
#endif

#if 0//freq  dont need 
    ret = IWCFG_SetFreq(skfd, TEST_WIF, "6");
    if(ret < 0){
        IFCFG_DEBUG("set freq error\n");
    }else{
        IFCFG_DEBUG("@set freq passed\n");
    }
#endif

    //IWCFG_SetCommit(skfd, TEST_WIF, NULL);
#endif

    //set ip 
    //todo: how to dhcp??
    //udchpc -i eth0/wlan0
#if 1
    IFCFG_DEBUG("\n");
    ret = IPCFG_SetIpAddr(TEST_WIF, 0xc0a8de0b);
    if(ret < 0){
        IFCFG_DEBUG("set TEST_WIF ip addr error \n");
    }else{
        IFCFG_DEBUG("set TEST_WIF ip addr ok \n");
    }
    addr = IPCFG_GetIpAddr(TEST_WIF);
    IFCFG_DEBUG("TEST_WIF addr is %lx \n", addr);
#endif
    iw_sockets_close(skfd);
#if 0
    //up the test_wireless_interface
    //up the wireless nic
    //ret = IPCFG_UpInterface(TEST_WIF);
    //if(ret < 0){
    //    IFCFG_DEBUG("IPCFG_UpInterface : up the TEST_WIF interface error \n");
    //}else{
    //    IFCFG_DEBUG("IPCFG_UpInterface : up the TEST_WIF interface ok \n");
    //}
#endif
    IFCFG_DEBUG("\n");
    usleep(1000*10000);
    //IFCFG_ShowDevices(0);
    IFCFG_ShowDevices(1);
    //switch to selected interface
    //wlan0 or eth0 ...
    
    IFCFG_DEBUG("/********third select one and up it\n");
    IFCFG_DEBUG("out main...\n");
    
    return 0;
}

//now we can get eth0 info and wlan0 info
//for wlan0  todo...
//* 1 get sensc quality
//* 2 get encryption on or off
//* 3 scanning operation/command
//* 4 check if connect to ap  ***add check ap addr
//* 7 free scan result
//5 dhcp on wlan0
//6 encap interfaces ...

//others if thinked

//opt on root 
//static library creation+math lib(lm)

