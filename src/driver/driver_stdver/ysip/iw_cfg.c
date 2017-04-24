/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* iw_cfg.c : interface wireless config file

* version: 	1.0
* author: 	ywg
* time:	2009-08-17

* history:	none
**************************************************************************/

#include "iw_cfg.h"

#define IWCFG_DEBUG printf

/*********************** SETTING SUB-ROUTINES ***********************/
/*
 * The following functions are use to set some wireless parameters and
 * are called by the set dispatcher set_info().
 * They take as arguments the remaining of the command line, with
 * arguments processed already removed.
 * An error is indicated by a negative return value.
 * 0 and positive return values indicate the number of args consumed.
 */

/*
    set essid 
    off/any/on/specify
*/
int IWCFG_SetEssid(int skfd, char *ifname, char *p_essid)
{ 
    /* ---------- Set ESSID ---------- */
    struct iwreq wrq;
    char essid[IW_ESSID_MAX_SIZE + 1];
    int we_kernel_version;
    
    if((!strcasecmp(p_essid, "off")) ||
     (!strcasecmp(p_essid, "any"))){
        //wrq.u.data.flags = 0;
        wrq.u.essid.flags = 0;
        essid[0] = '\0';
    }
    else{
        if(!strcasecmp(p_essid, "on")){
            /* Get old essid */
            //wrq.u.data.pointer = (caddr_t) essid;
            //wrq.u.data.length = 0;
            //wrq.u.data.flags = 0;
            memset(essid, '\0', sizeof(essid));
        	wrq.u.essid.pointer = (caddr_t) essid;
        	wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
        	wrq.u.essid.flags = 0;
            if(iw_get_ext(skfd, ifname, SIOCGIWESSID, &wrq) < 0){
            //if(ioctl(skfd, SIOCGIWESSID, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetEssid: SIOCGIWESSID error \n");
                return(-1);
            }
            //strcpy(wrq.ifr_name, ifname);
            //wrq.u.data.flags = 1;
            wrq.u.essid.flags = 1;
        }
        else{
            if(strlen(p_essid) > IW_ESSID_MAX_SIZE){
                IWCFG_DEBUG("ESSID too long (max %d): ``%s''\n",IW_ESSID_MAX_SIZE, p_essid);
                return -1;
            }
            else{
                //wrq.u.data.flags = 1;
                wrq.u.essid.flags = 1;
                strcpy(essid, p_essid);
                printf("essid===%s==%s\n",p_essid, essid);
            }
        }
    }

    /* Get version from kernel, device may not have range... */
    we_kernel_version = iw_get_kernel_we_version();
    //wrq.u.data.pointer = (caddr_t) essid;
    //wrq.u.data.length = strlen(essid) + 1;
    wrq.u.essid.pointer = (caddr_t) essid;
    wrq.u.essid.length = strlen(essid);
    if(we_kernel_version < 21){
        wrq.u.essid.length++;
    }
    IWCFG_DEBUG("IWCFG_SetEssid: flag %x \n", wrq.u.essid.flags);
    if(iw_set_ext(skfd, ifname, SIOCSIWESSID, &wrq) < 0){
    //if(ioctl(skfd, SIOCSIWESSID, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetEssid: SIOCSIWESSID :error \n");
        return(-1);
    }

    IWCFG_SetCommit(skfd, ifname, &wrq);
    return 0;
}

/*
    set ap addr
*/
int IWCFG_SetApaddr(int skfd, char *ifname, char *apddr)
{
    struct iwreq wrq;

    if((!strcasecmp(apddr, "auto")) ||
         (!strcasecmp(apddr, "any"))){
          /* Send a broadcast address */
          iw_broad_ether(&(wrq.u.ap_addr));
    }else{
        if(!strcasecmp(apddr, "off"))
        {
            /* Send a NULL address */
            iw_null_ether(&(wrq.u.ap_addr));
        }
        else
        {
            /* Get the address and check if the interface supports it */
            if(iw_in_addr(skfd, ifname, apddr, &(wrq.u.ap_addr)) < 0)
            {
                IWCFG_DEBUG("IWCFG_SetApaddr %-8.8s  Interface doesn't support MAC & IP addresses\n", ifname);
                return(-1);
            }
        }
    }

    if(iw_set_ext(skfd, ifname, SIOCSIWAP, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetApaddr: SIOCSIWAP: error \n");
        return(-1);
    }

#if !WE_EXT_VERSION
    /* Check if we have valid address types */
    if(check_addr_type(skfd, ifname) < 0){
        IWCFG_DEBUG("IWCFG_SetApaddr %-8.8s  Interface doesn't support MAC & IP addresses\n", ifname);
        return(-1);
    }
    
    /* Get the address */
    if(in_addr(skfd, ifname, apddr, &(wrq.u.ap_addr)) < 0){
        IWCFG_DEBUG("IWCFG_SetApaddr: error \n");
        return -1;
    }

    if(ioctl(skfd, SIOCSIWAP, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetApaddr: SIOCSIWAP: error \n");
        return(-1);
    }
#endif
    IWCFG_SetCommit(skfd, ifname, &wrq);
    return 0;
}

/*
    set nwid
    off/any/on/specify
*/
int IWCFG_SetNwid(int skfd, char *ifname, char *nwid)
{
    /* ---------- Set network ID ---------- */
    struct iwreq wrq;
    unsigned long temp;
    int	i;

    if((!strcasecmp(nwid, "off")) ||
         (!strcasecmp(nwid, "any")))
        wrq.u.nwid.disabled = 1;
    else
        if(!strcasecmp(nwid, "on"))
        {
            /* Get old nwid */
            if(iw_get_ext(skfd, ifname, SIOCGIWNWID, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetNwid: SIOCGIWNWID: error \n");
                return(-1);
            }
            wrq.u.nwid.disabled = 0;
        }
        else
            if(sscanf(nwid, "%lX", &(temp)) != 1)
            {
                IWCFG_DEBUG("IWCFG_SetNwid: arg: error \n");
                return(-1);
            }
            else
            {
                wrq.u.nwid.value = temp;
                wrq.u.nwid.disabled = 0;
            }

    wrq.u.nwid.fixed = 1;

    /* Set new nwid */
    if(iw_set_ext(skfd, ifname, SIOCSIWNWID, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetNwid: SIOCSIWNWID: error \n");
        return(-1);
    }
    
#if !WE_EXT_VERSION
    i++;
    if((!strcasecmp(nwid, "off")) || (!strcasecmp(nwid, "any")))
        wrq.u.nwid.on = 0;
    else
        if(!strcasecmp(nwid, "on")){
            /* Get old nwid */
            if(ioctl(skfd, SIOCGIWNWID, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetNwid: SIOCGIWNWID: error \n");
                return(-1);
            }
            strcpy(wrq.ifr_name, ifname);
            wrq.u.nwid.on = 1;
        }
        else{
            (wrq.u.nwid.nwid) = &nwid;
            strcpy(wrq.ifr_name, ifname);
            wrq.u.nwid.on = 1;
        }

    if(ioctl(skfd, SIOCSIWNWID, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetNwid: SIOCSIWNWID: error \n");
        return(-1);
    }
#endif
    return 0;
}

/*
    set mode
*/
int IWCFG_SetMode(int skfd, char *ifname, char *mode)
{
    struct iwreq wrq;
    unsigned int k;		/* Must be unsigned */
    
    /* Check if it is a uint, otherwise get is as a string */
    if(sscanf(mode, "%i", &k) != 1){
        k = 0;
        while((k < IW_NUM_OPER_MODE) &&
                strncasecmp(mode, iw_operation_mode[k], 3))
        k++;
    }
    if(k >= IW_NUM_OPER_MODE){
        IWCFG_DEBUG("IWCFG_SetMode unkown cmd \n");
        return -1;
    }

    wrq.u.mode = k;

    if(iw_set_ext(skfd, ifname, SIOCSIWMODE, &wrq) < 0){
    //if(ioctl(skfd, SIOCSIWMODE, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetMode: SIOCSIWMODE: error \n");
        return(-1);
    }
    
    return 0;
}

/*
    set sensitivity
*/
int IWCFG_SetSens(int skfd, char *ifname, char *sens)
{
    /* ---------- Set sensitivity ---------- */
    struct iwreq wrq;
    int temp;
    
    if(sscanf(sens, "%i", &(temp)) != 1){
        IWCFG_DEBUG("IWCFG_SetSens : error \n");
        return -1;
    }
    wrq.u.sens.value = temp;

    if(iw_set_ext(skfd, ifname, SIOCSIWSENS, &wrq) < 0){
    //if(ioctl(skfd, SIOCSIWSENS, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetSens: SIOCSIWSENS: error \n");
        return(-1);
    }
    
    return 0;
}

/*
    set freq
*/
int IWCFG_SetFreq(int skfd, char *ifname, char *p_freq)
{
    double		freq;
    struct iwreq wrq;
    int i=1;

    if(!strcasecmp(p_freq, "auto"))
    {
        wrq.u.freq.m = -1;
        wrq.u.freq.e = 0;
        wrq.u.freq.flags = 0;
    }
    else
    {
        if(!strcasecmp(p_freq, "fixed"))
        {
            /* Get old frequency */
            if(iw_get_ext(skfd, ifname, SIOCGIWFREQ, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetFreq: SIOCGIWFREQ error \n");
                return -1;
            }
            wrq.u.freq.flags = IW_FREQ_FIXED;
        }
        else			/* Should be a numeric value */
        {
            double		freq;
            char *		unit;

            freq = strtod(p_freq, &unit);
            if(unit == p_freq)
            {
                IWCFG_DEBUG("IWCFG_SetFreq: arg error \n");
                return -1;
            }
            if(unit != NULL)
            {
              if(unit[0] == 'G') freq *= GIGA;
              if(unit[0] == 'M') freq *= MEGA;
              if(unit[0] == 'k') freq *= KILO;
            }

            iw_float2freq(freq, &(wrq.u.freq));

            wrq.u.freq.flags = IW_FREQ_FIXED;
        }
    }

    if(iw_set_ext(skfd, ifname, SIOCSIWFREQ, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetFreq: SIOCSIWFREQ: error \n");
        return(-1);
    }
  
#if !WE_EXT_VERSION
    if(sscanf(p_freq, "%lg", &(freq)) != 1){
        IWCFG_DEBUG("IWCFG_SetFreq: error \n");
        return -1;
    }
    
    if(index(p_freq, 'G')) freq *= GIGA;
    if(index(p_freq, 'M')) freq *= MEGA;
    if(index(p_freq, 'k')) freq *= KILO;

    float2freq(freq, &(wrq.u.freq));

    if(ioctl(skfd, SIOCSIWFREQ, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetFreq: SIOCSIWFREQ: error \n");
        return(-1);
    }
#endif
    
    IWCFG_SetCommit(skfd, ifname, &wrq);
    
    return 0;
}

/*
    set channel
*/
int IWCFG_SetChannel(int skfd, char *ifname, char *channel)
{
#if WE_EXT_VERSION
    return IWCFG_SetFrag(skfd, ifname, channel);
#else //!WE_EXT_VERSION
    double		freq;
    struct iwreq wrq;

    if(sscanf(channel, "%lg", &(freq)) != 1){
        IWCFG_DEBUG("IWCFG_SetChannel: error \n");
        return -1;
    }
    
    if(index(channel, 'G')) freq *= GIGA;
    if(index(channel, 'M')) freq *= MEGA;
    if(index(channel, 'k')) freq *= KILO;

    float2freq(freq, &(wrq.u.freq));

    if(ioctl(skfd, SIOCSIWFREQ, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetChannel: SIOCSIWFREQ: error \n");
        return(-1);
    }
    return 0;
#endif
}

/*
    specify ap mac or id
*/

/*
    set nick
*/
int IWCFG_SetNick(int skfd, char *ifname, char *nick)
{
    /* ---------- Set NickName ---------- */
    struct iwreq wrq;
    int we_kernel_version;

    if(strlen(nick) > IW_ESSID_MAX_SIZE)
    {
        IWCFG_DEBUG("IWCFG_SetNick: Name too long (max %d) : ``%s''\n", IW_ESSID_MAX_SIZE, nick);
        return -1;
    }
    
    wrq.u.essid.pointer = (caddr_t) nick;
    wrq.u.essid.length = strlen(nick);

    we_kernel_version = iw_get_kernel_we_version();
    if(we_kernel_version < 21){
        wrq.u.essid.length++;
    }
    
    if(iw_set_ext(skfd, ifname, SIOCSIWNICKN, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetNick: SIOCSIWNICKN: error \n");
        return(-1);
    }
    
#if !WE_EXT_VERSION
    if(strlen(nick) > IW_ESSID_MAX_SIZE){
        IWCFG_DEBUG("IWCFG_SetNick: Name too long (max %d) : ``%s''\n", IW_ESSID_MAX_SIZE, nick);
        return -1;
    }
    
    wrq.u.data.pointer = (caddr_t) nick;
    wrq.u.data.length = strlen(nick) + 1;
    if(ioctl(skfd, SIOCSIWNICKN, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetNick: SIOCSIWNICKN: error \n");
        return(-1);
    }
#endif
    return 0;
}

/*
    set rate
*/
int IWCFG_SetRate(int skfd, char *ifname, char *rate)
{
    /* ---------- Set Bit-Rate ---------- */
    struct iwreq wrq;

    wrq.u.bitrate.flags = 0;
    if(!strcasecmp(rate, "auto"))
    {
        wrq.u.bitrate.value = -1;
        wrq.u.bitrate.fixed = 0;
    }
    else
    {
        if(!strcasecmp(rate, "fixed"))
        {
            /* Get old bitrate */
            if(iw_get_ext(skfd, ifname, SIOCGIWRATE, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetRate: SIOCGIWRATE: error \n");
                return(-1);
            }
            wrq.u.bitrate.fixed = 1;
        }
        else			/* Should be a numeric value */
        {
            double		brate;
            char *		unit;

            brate = strtod(rate, &unit);
            if(unit == rate)
            {
                IWCFG_DEBUG("IWCFG_SetRate: arg: error \n");
                return(-1);
            }
            if(unit != NULL)
            {
                if(unit[0] == 'G') brate *= GIGA;
                if(unit[0] == 'M') brate *= MEGA;
                if(unit[0] == 'k') brate *= KILO;
            }
            wrq.u.bitrate.value = (long) brate;
            wrq.u.bitrate.fixed = 1;
        }
    }
    
    if(iw_set_ext(skfd, ifname, SIOCSIWRATE, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetRate : SIOCSIWRATE : error \n");
        return(-1);
    }
    
#if !WE_EXT_VERSION
    if(!strcasecmp(rate, "auto")){
        wrq.u.bitrate.value = -1;
        wrq.u.bitrate.fixed = 0;
    }
    else{
        if(!strcasecmp(rate, "fixed")){
            /* Get old bitrate */
            if(ioctl(skfd, SIOCGIWRATE, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetRate: SIOCGIWRATE: error \n");
                return(-1);
            }
            strcpy(wrq.ifr_name, ifname);
        }
        else			/* Should be a numeric value */{
            double		brate;
            if(sscanf(rate, "%lg", &(brate)) != 1){
                IWCFG_DEBUG("SIOCGIWRATE: error \n");
                return -1;
            }
            if(index(rate, 'G')) brate *= GIGA;
            if(index(rate, 'M')) brate *= MEGA;
            if(index(rate, 'k')) brate *= KILO;
            wrq.u.bitrate.value = (long) brate;
        }
        wrq.u.bitrate.fixed = 1;
    }
    
    if(ioctl(skfd, SIOCSIWRATE, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetRate : SIOCSIWRATE : error \n");
        return(-1);
    }
#endif
    return 0;
}

/*
    set rts
*/
int IWCFG_SetRts(int skfd, char *ifname, char *rts)
{
    /* ---------- Set RTS threshold ---------- */
    struct iwreq wrq;

    wrq.u.rts.value = -1;
    wrq.u.rts.fixed = 1;
    wrq.u.rts.disabled = 0;

    if(!strcasecmp(rts, "off"))
        wrq.u.rts.disabled = 1;	/* i.e. max size */
    else
        if(!strcasecmp(rts, "auto"))
            wrq.u.rts.fixed = 0;
        else
        {
            if(!strcasecmp(rts, "fixed"))
            {
                /* Get old RTS threshold */
                if(iw_get_ext(skfd, ifname, SIOCGIWRTS, &wrq) < 0){
                    IWCFG_DEBUG("IWCFG_SetRts: SIOCGIWRTS: error \n");
                    return(-1);
                }
                wrq.u.rts.fixed = 1;
            }
            else
            {	/* Should be a numeric value */
                long temp;
                if(sscanf(rts, "%li", (unsigned long *) &(temp)) != 1)
                {
                    IWCFG_DEBUG("IWCFG_SetRts: error \n");
                    return -1;
                }
                wrq.u.rts.value = temp;
            }
        }

    if(iw_set_ext(skfd, ifname, SIOCSIWRTS, &wrq) < 0){
        IWCFG_DEBUG("SIOCGIWRTS: SIOCSIWRTS: error \n");
        return(-1);
    }
#if !WE_EXT_VERSION
    if(!strcasecmp(rts, "auto")){
        wrq.u.rts.value = -1;
        wrq.u.rts.fixed = 0;
    }
    else{
        if(!strcasecmp(rts, "fixed")){
            /* Get old RTS threshold */
            if(ioctl(skfd, SIOCGIWRTS, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetRts: SIOCGIWRTS: error \n");
                return(-1);
            }
            strcpy(wrq.ifr_name, ifname);
        }
        else{
            if(!strcasecmp(rts, "off"))
                wrq.u.rts.value = -1;	/* i.e. max size */
            else{			/* Should be a numeric value */
                if(sscanf(rts, "%ld", (unsigned long *) &(wrq.u.rts.value))
                 != 1){
                    IWCFG_DEBUG("IWCFG_SetRts: error \n");
                    return -1;
                }
            }
        }
        wrq.u.rts.fixed = 1;
    }

    if(ioctl(skfd, SIOCSIWRTS, &wrq) < 0){
        IWCFG_DEBUG("SIOCGIWRTS: SIOCSIWRTS: error \n");
        return(-1);
    }
#endif
    return 0;
}

/*
    set frag
*/
int IWCFG_SetFrag(int skfd, char *ifname, char *frag)
{
    /* ---------- Set fragmentation threshold ---------- */
    struct iwreq wrq;

    wrq.u.frag.value = -1;
    wrq.u.frag.fixed = 1;
    wrq.u.frag.disabled = 0;

    if(!strcasecmp(frag, "off"))
        wrq.u.frag.disabled = 1;	/* i.e. max size */
    else
        if(!strcasecmp(frag, "auto"))
            wrq.u.frag.fixed = 0;
        else
        {
            if(!strcasecmp(frag, "fixed"))
            {
                /* Get old fragmentation threshold */
                if(iw_get_ext(skfd, ifname, SIOCGIWFRAG, &wrq) < 0){
                    IWCFG_DEBUG("IWCFG_SetFrag: SIOCGIWFRAG: error \n");
                    return(-1);
                }
                wrq.u.frag.fixed = 1;
            }
            else
            {	/* Should be a numeric value */
                long temp;
                if(sscanf(frag, "%li", &(temp))!= 1)
                {
                    IWCFG_DEBUG("IWCFG_SetFrag: error \n");
                    return -1;
                }
                wrq.u.frag.value = temp;
            }
        }

    if(iw_set_ext(skfd, ifname, SIOCSIWFRAG, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetFrag: SIOCSIWFRAG: error \n");
        return(-1);
    }
    
#if !WE_EXT_VERSION
    if(!strcasecmp(frag, "auto")){
        wrq.u.frag.value = -1;
        wrq.u.frag.fixed = 0;
    }
    else{
        if(!strcasecmp(frag, "fixed")){
            /* Get old fragmentation threshold */
            if(ioctl(skfd, SIOCGIWFRAG, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetFrag: SIOCGIWFRAG: error \n");
                return(-1);
            }
            strcpy(wrq.ifr_name, ifname);
        }
        else{
            if(!strcasecmp(frag, "off"))
                wrq.u.frag.value = -1;	/* i.e. max size */
            else			/* Should be a numeric value */
                if(sscanf(frag, "%ld", (unsigned long *) &(wrq.u.frag.value))
                 != 1){
                    IWCFG_DEBUG("IWCFG_SetFrag: error \n");
                    return -1;
                }
        }
        wrq.u.frag.fixed = 1;
    }

    if(ioctl(skfd, SIOCSIWFRAG, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetFrag: SIOCSIWFRAG: error \n");
        return(-1);
    }
#endif
    return 0;
}

/*
    specify enc (key)
*/
int IWCFG_SetEnc(int skfd, char *ifname, char *keyflag, char *keyvalue)
{
    /* ---------- Set encryption stuff ---------- */
    struct iwreq wrq;
    int i=1;

    unsigned char key[IW_ENCODING_TOKEN_MAX];

    if(!strcasecmp(keyflag, "on"))
    {
        /* Get old encryption information */
        wrq.u.data.pointer = (caddr_t) key;
        wrq.u.data.length = IW_ENCODING_TOKEN_MAX;
        wrq.u.data.flags = 0;
        if(iw_get_ext(skfd, ifname, SIOCGIWENCODE, &wrq) < 0){
            IWCFG_DEBUG("IWCFG_SetEnc SIOCGIWENCODE: error \n");
            return(-1);
        }
        wrq.u.data.flags &= ~IW_ENCODE_DISABLED;	/* Enable */
    }
    else
    {
        int	gotone = 0;
        int	oldone;
        int	keylen;
        int	temp;
	int skip=0;

        wrq.u.data.pointer = (caddr_t) NULL;
        wrq.u.data.flags = 0;
        wrq.u.data.length = 0;
        i = 0;

        /* Allow arguments in any order (it's safe) */
        do
        {
            oldone = gotone;

            /* -- Check for the key -- */
            
            
            keylen = iw_in_key_full(skfd, ifname,keyvalue, key, &wrq.u.data.flags);
            if(keylen > 0)
            {
                wrq.u.data.length = keylen;
                wrq.u.data.pointer = (caddr_t) key;
                ++i;
                gotone++;
            }
            skip++;
	    if(skip == 5){
	        wrq.u.data.length = 0;
                wrq.u.data.pointer = NULL;
		gotone++;
	    }
            printf("oldone=%d, gotone=%d \n",oldone, gotone);
        }while(gotone == oldone);

        /* Pointer is absent in new API */
        if(wrq.u.data.pointer == NULL)
            wrq.u.data.flags |= IW_ENCODE_NOKEY;

        /* Check if we have any invalid argument */
        if(!gotone)
        {
            IWCFG_DEBUG("IWCFG_SetEnc : arg: error \n");
            return(-1);
        }
    }
    IWCFG_DEBUG("IWCFG_SetEnc : flag %x \n", wrq.u.data.flags);

    if(iw_set_ext(skfd, ifname, SIOCSIWENCODE, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetEnc : SIOCSIWENCODE(%d): error \n");
        return(-1);
    }
    
#if !WE_EXT_VERSION
    unsigned long long	key = 0;
    
    if(!strcasecmp(key, "off"))
        wrq.u.encoding.method = 0;
    else{
        if(!strcasecmp(key, "on")){
            /* Get old encryption information */
            if(ioctl(skfd, SIOCGIWENCODE, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetEnc SIOCGIWENCODE: error \n");
                return(-1);
            }
            strcpy(wrq.ifr_name, ifname);
        }
        else{
            
            char *	buff;
            char *	p;
            u_long	temp;

            p = buff = malloc(strlen(key + 1));
            strcpy(buff, key);

            p = strtok(buff, "-:;.,*#");
            while(p != (char *) NULL)
            {
                key = key << 16;
                if(sscanf(p, "%lX", &temp) != 1){
                    IWCFG_DEBUG("IWCFG_SetEnc error \n");
                }
                key += temp;
                p = strtok((char *) NULL, "-:;.,*#");
            }

            free(buff);
            wrq.u.encoding.code = key;
        }
        /* TODO : check for "(method)" in args list */
        wrq.u.encoding.method = 1;
    }

    if(ioctl(skfd, SIOCSIWENCODE, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetEnc : SIOCSIWENCODE(%d): error \n");
        return(-1);
    }
#endif

    IWCFG_SetCommit(skfd, ifname, &wrq);
    return 0;
}

#if IWCFG_EXTENTIONAL
/*
    set power
*/
int IWCFG_SetPower(int skfd, char *ifname, char *power, char *mode)
{
    struct iwreq wrq;
    int	i = 1;

    if(!strcasecmp(power, "off"))
        wrq.u.power.disabled = 1;	/* i.e. max size */
    else
        if(!strcasecmp(power, "on"))
        {
            /* Get old Power info */
            wrq.u.power.flags = 0;
            if(iw_get_ext(skfd, ifname, SIOCGIWPOWER, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetPower : SIOCGIWPOWER error \n");
                return -1;
            }
            wrq.u.power.disabled = 0;
        }
        else
        {
            double value;
            char *unit;
            int gotone = 0;

            /* Parse modifiers */
            i = parse_modifiers(args, count, &wrq.u.power.flags,
            	    iwmod_power, IWMOD_POWER_NUM);
            if(i < 0)
                //return(i);
                return -1

            wrq.u.power.disabled = 0;

            /* Is there any value to grab ? */
            value = strtod(power, &unit);
            if(unit != power)
            {
                struct iw_range	range;
                int			flags;
                /* Extract range info to handle properly 'relative' */
                if(iw_get_range_info(skfd, ifname, &range) < 0)
                    memset(&range, 0, sizeof(range));

                /* Get the flags to be able to do the proper conversion */
                switch(wrq.u.power.flags & IW_POWER_TYPE)
                {
                    case IW_POWER_SAVING:
                        flags = range.pms_flags;
                        break;
                    case IW_POWER_TIMEOUT:
                        flags = range.pmt_flags;
                        break;
                    default:
                        flags = range.pmp_flags;
                        break;
                }
                /* Check if time or relative */
                if(flags & IW_POWER_RELATIVE)
                {
                    if(range.we_version_compiled < 21)
                        value *= MEGA;
                    else
                        wrq.u.power.flags |= IW_POWER_RELATIVE;
                }
                else
                {
                    value *= MEGA;	/* default = s */
                    if(unit[0] == 'u') value /= MEGA;
                    if(unit[0] == 'm') value /= KILO;
                }
                wrq.u.power.value = (long) value;
                /* Set some default type if none */
                if((wrq.u.power.flags & IW_POWER_TYPE) == 0)
                    wrq.u.power.flags |= IW_POWER_PERIOD;
                ++i;
                gotone = 1;
            }

            /* Now, check the mode */
            if(!strcasecmp(mode, "all"))
                wrq.u.power.flags |= IW_POWER_ALL_R;
            if(!strncasecmp(mode, "unicast", 4))
                wrq.u.power.flags |= IW_POWER_UNICAST_R;
            if(!strncasecmp(mode, "multicast", 5))
                wrq.u.power.flags |= IW_POWER_MULTICAST_R;
            if(!strncasecmp(mode], "force", 5))
                wrq.u.power.flags |= IW_POWER_FORCE_S;
            if(!strcasecmp(mode, "repeat"))
                wrq.u.power.flags |= IW_POWER_REPEATER;
            if(wrq.u.power.flags & IW_POWER_MODE)
            {
                ++i;
                gotone = 1;
            }
            if(!gotone)
            {
                IWCFG_DEBUG("IWCFG_SetPower arg error \n");
                return -1;
            }
        }
    
    if(iw_set_ext(skfd, ifname, SIOCSIWPOWER, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetPower : SIOCSIWPOWER error \n");
        return -1;
    }

    return 0;
}

/*
    set retry
*/
int IWCFG_SetRetry(int skfd, char *ifname, char *retry)
{
    struct iwreq wrq;
    int	i = 0;
    double value;
    char *unit;

    /* Parse modifiers */
    i = parse_modifiers(args, count, &wrq.u.retry.flags,
          iwmod_retry, IWMOD_RETRY_NUM);
    
    if(i < 0)
        return(i);

    /* Add default type if none */
    if((wrq.u.retry.flags & IW_RETRY_TYPE) == 0)
        wrq.u.retry.flags |= IW_RETRY_LIMIT;

    wrq.u.retry.disabled = 0;

    /* Is there any value to grab ? */
    value = strtod(retry, &unit);
    if(unit == retry)
    {
        IWCFG_DEBUG("IWCFG_SetRetry : arg error \n");
        return -1;
    }

    /* Limit is absolute, on the other hand lifetime is seconds */
    if(wrq.u.retry.flags & IW_RETRY_LIFETIME)
    {
        struct iw_range	range;
        /* Extract range info to handle properly 'relative' */
        if(iw_get_range_info(skfd, ifname, &range) < 0)
            memset(&range, 0, sizeof(range));

        if(range.r_time_flags & IW_RETRY_RELATIVE)
        {
            if(range.we_version_compiled < 21)
                value *= MEGA;
            else
                wrq.u.retry.flags |= IW_RETRY_RELATIVE;
        }
        else
        {
            /* Normalise lifetime */
            value *= MEGA;	/* default = s */
            if(unit[0] == 'u') value /= MEGA;
            if(unit[0] == 'm') value /= KILO;
        }
    }
    wrq.u.retry.value = (long) value;
    ++i;

    if(iw_set_ext(skfd, ifname, SIOCSIWRETRY, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetRetry: SIOCSIWRETRY: error \n");
        return -1;
    }

    return 0;
}

/*
    set txpower
*/
int IWCFG_SetTXPower(int skfd, char *ifname, char *txpower)
{
    struct iwreq wrq;
    int i = 1;

    /* Prepare the request */
    wrq.u.txpower.value = -1;
    wrq.u.txpower.fixed = 1;
    wrq.u.txpower.disabled = 0;
    wrq.u.txpower.flags = IW_TXPOW_DBM;

    if(!strcasecmp(txpower, "off"))
        wrq.u.txpower.disabled = 1;	/* i.e. turn radio off */
    else
        if(!strcasecmp(txpower, "auto"))
            wrq.u.txpower.fixed = 0;	/* i.e. use power control */
        else
        {
            if(!strcasecmp(txpower, "on"))
            {
                /* Get old tx-power */
                if(iw_get_ext(skfd, ifname, SIOCGIWTXPOW, &wrq) < 0){
                    IWCFG_DEBUG("IWCFG_SetTXPower: SIOCGIWTXPOW error \n");
                    return -1;
                }
                wrq.u.txpower.disabled = 0;
            }
            else
            {
                if(!strcasecmp(txpower, "fixed"))
                {
                    /* Get old tx-power */
                    if(iw_get_ext(skfd, ifname, SIOCGIWTXPOW, &wrq) < 0){
                        IWCFG_DEBUG("IWCFG_SetTXPower: SIOCGIWTXPOW error \n");
                        return -1;
                    }
                    wrq.u.txpower.fixed = 1;
                    wrq.u.txpower.disabled = 0;
                }
                else			/* Should be a numeric value */
                {
                    int	power;
                    int	ismwatt = 0;
                    struct iw_range	range;

                    /* Extract range info to do proper conversion */
                    if(iw_get_range_info(skfd, ifname, &range) < 0)
                        memset(&range, 0, sizeof(range));

                    /* Get the value */
                    if(sscanf(txpower, "%i", &(power)) != 1)
                    {
                        IWCFG_DEBUG("IWCFG_SetTXPower : arg error \n");
                        return -1;
                    }

                    /* Check if milliWatt
                    * We authorise a single 'm' as a shorthand for 'mW',
                    * on the other hand a 'd' probably means 'dBm'... */
                    ismwatt = ((strchr(txpower, 'm') != NULL)
                    && (strchr(txpower, 'd') == NULL));

                    /* We could check 'W' alone... Another time... */

                    /* Convert */
                    if(range.txpower_capa & IW_TXPOW_RELATIVE)
                    {
                        /* Can't convert */
                        if(ismwatt)
                        {
                            IWCFG_DEBUG("IWCFG_SetTXPower : arg error \n");
                            return -1;
                        }
                        wrq.u.txpower.flags = IW_TXPOW_RELATIVE;
                    }
                    else
                        if(range.txpower_capa & IW_TXPOW_MWATT)
                        {
                            if(!ismwatt)
                                power = iw_dbm2mwatt(power);
                            wrq.u.txpower.flags = IW_TXPOW_MWATT;
                        }
                        else
                        {
                            if(ismwatt)
                                power = iw_mwatt2dbm(power);
                            wrq.u.txpower.flags = IW_TXPOW_DBM;
                        }
                    wrq.u.txpower.value = power;
                }
            }
        }

    if(iw_set_ext(skfd, ifname, SIOCSIWTXPOW, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetTXPower: SIOCSIWTXPOW error \n");
        return -1;
    }
    
    return 0;
}

int IWCFG_SetModulation(int skfd, char *ifname, char *modulation)
{
    struct iwreq wrq;
    int	i = 1;
    
    if(!strcasecmp(modulation, "auto"))
        wrq.u.param.fixed = 0;	/* i.e. use any modulation */
    else
    {
        if(!strcasecmp(modulation, "fixed"))
        {
            /* Get old modulation */
            if(iw_get_ext(skfd, ifname, SIOCGIWMODUL, &wrq) < 0){
                IWCFG_DEBUG("IWCFG_SetModulation: SIOCGIWMODUL: error \n");
                return -1;
            }
            wrq.u.param.fixed = 1;
        }
        else
        {
            int		k;

            /* Allow multiple modulations, combine them together */
            wrq.u.param.value = 0x0;
            i = 0;
            do
            {
                for(k = 0; k < IW_SIZE_MODUL_LIST; k++)
                {
                    if(!strcasecmp(args[i], iw_modul_list[k].cmd))
                    {
                        wrq.u.param.value |= iw_modul_list[k].mask;
                        ++i;
                        break;
                    }
                }
            }
            /* For as long as current arg matched and not out of args */
            while((i < count) && (k < IW_SIZE_MODUL_LIST));

            /* Check we got something */
            if(i == 0)
            {
                IWCFG_DEBUG("IWCFG_SetModulation: arg error \n");
                return -1;
            }
        }
    }

    if(iw_set_ext(skfd, ifname, SIOCSIWMODUL, &wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetModulation: SIOCSIWMODUL: error \n");
        return -1;
    }

    return 0;
}
#endif

int IWCFG_SetCommit(int skfd, char *ifname, struct iwreq *wrq)
{
    //struct iwreq wrq;
#if 0
    if(iw_set_ext(skfd, ifname, SIOCSIWCOMMIT, wrq) < 0){
        IWCFG_DEBUG("IWCFG_SetCommit : SIOCSIWCOMMIT error \n");
        return -1;
    }
#endif
    return 0;
}

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
                        char *enc)
{
    int ret;
    int skfd = -1;		/* generic raw socket desc.	*/

    /* Create a channel to the NET kernel. */
    if((skfd = iw_sockets_open()) < 0){
        IWCFG_DEBUG("IWCFG_SetInfo: socket open error! \n");
        return -1;
    }

    if(pessid != NULL){
        ret = IWCFG_SetEssid(skfd, ifname, pessid);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(apaddr != NULL){
        ret = IWCFG_SetApaddr(skfd, ifname, apaddr);
        if(ret <0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(nwid != NULL){
        ret = IWCFG_SetNwid(skfd, ifname, nwid);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(mode != NULL){
        ret = IWCFG_SetMode(skfd, ifname, mode);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(sens != NULL){
        ret = IWCFG_SetSens(skfd, ifname, sens);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(freq != NULL){
        ret = IWCFG_SetFrag(skfd, ifname, frag);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(nick != NULL){
        ret = IWCFG_SetNick(skfd, ifname, nick);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(rate !=  NULL){
        ret = IWCFG_SetRate(skfd, ifname, rate);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(rts != NULL){
        ret = IWCFG_SetRts(skfd, ifname, rts);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(frag != NULL){
        ret = IWCFG_SetFrag(skfd, ifname, frag);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

    if(enc != NULL){
        ret = IWCFG_SetEnc(skfd, ifname, "key", enc);
        if(ret < 0){
            iw_sockets_close(skfd);
            return -1;
        }
    }

#if 0
    /* commit the change */
    ret  = IWCFG_SetCommit(skfd, ifname, NULL);
    if(ret < 0){
        return -1;
    }
#endif
    iw_sockets_close(skfd);

    return 0;
}

int IWCFG_CheckInfo(iwcfg_info_t *info)
{
    return 1;
}
/*
    connect to ap
    essid
    key
    managed or ad-hoc
    ...
    return <0 failed
           else success
*/
int IWCFG_ConnectAp(char *ifname, iwcfg_info_t *info)
{
    int ret;
    int skfd;
    struct wireless_info read_info;
    
    ret = IWCFG_SetInfo(ifname, 
                        info->essid, 
                        info->apaddr, 
                        info->nwid, 
                        info->mode, 
                        info->sens, 
                        info->freq, 
                        info->nick, 
                        info->rate, 
                        info->rts, 
                        info->frag, 
                        info->enc);
    if(ret < 0){
        IWCFG_DEBUG("IWCFG_ConnectAp : error \n");
        return -1;
    }

    skfd = iw_sockets_open();
    if(skfd < 0){
        IWCFG_DEBUG("IWCFG_ConnectAp : open socket error \n");
        return -1;
    }
    
    ret = IWCFG_GetInfo(skfd, ifname, &read_info);
    if(ret < 0){
        iw_sockets_close(skfd);
        IWCFG_DEBUG("IWCFG_ConnectAp : IWCFG_GetInfo get info error \n");
        return -1;
    }
    
    /* check for read_info */
    if(IWCFG_CheckInfo(&read_info)){
        iw_sockets_close(skfd);
        IWCFG_DEBUG("IWCFG_ConnectAp : success \n");
        return 0;
    }else{
        iw_sockets_close(skfd);
        IWCFG_DEBUG("IWCFG_ConnectAp : check info error \n");
        return -1;
    }
    
}


/*
    get series
*/


/*
 * Get wireless informations & config from the device driver
 * We will call all the classical wireless ioctl on the driver through
 * the socket to know what is supported and to get the settings...
 */
int IWCFG_GetInfo(int skfd, char *ifname, struct wireless_info *info)
{
    struct iwreq wrq;

    memset((char *) info, 0, sizeof(struct wireless_info));

    /* Get wireless name */
    strcpy(wrq.ifr_name, ifname);
    //if(ioctl(skfd, SIOCGIWNAME, &wrq) < 0)
    if(iw_get_ext(skfd, ifname, SIOCGIWNAME, &wrq) < 0)
        /* If no wireless name : no wireless extensions */
        return(-1);
    else{
        //strcpy(info->name, wrq.u.name);
        strncpy(info->b.name, wrq.u.name, IFNAMSIZ);
        info->b.name[IFNAMSIZ] = '\0';
    }

    /* Get network ID */
    if(iw_get_ext(skfd, ifname, SIOCGIWNWID, &wrq) >= 0)
    {
        info->b.has_nwid = 1;
        memcpy(&(info->b.nwid), &(wrq.u.nwid), sizeof(iwparam));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWNWID, &wrq) >= 0){
        info->has_nwid = 1;
        info->nwid_on = wrq.u.nwid.on;
        info->nwid = wrq.u.nwid.nwid;
    }
    #endif
    
    /* Get frequency / channel */
    if(iw_get_ext(skfd, ifname, SIOCGIWFREQ, &wrq) >= 0)
    {
        info->b.has_freq = 1;
        info->b.freq = iw_freq2float(&(wrq.u.freq));
        info->b.freq_flags = wrq.u.freq.flags;
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWFREQ, &wrq) >= 0){
        info->has_freq = 1;
        info->freq = freq2float(&(wrq.u.freq));
    }
    #endif
    
    /* Get sensitivity */
    if(iw_get_ext(skfd, ifname, SIOCGIWSENS, &wrq) >= 0)
    {
        info->has_sens = 1;
        memcpy(&(info->sens), &(wrq.u.sens), sizeof(iwparam));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWSENS, &wrq) >= 0){
        info->has_sens = 1;
        info->sens = wrq.u.sensitivity;
    }
    #endif

    /* Get encryption information */
    wrq.u.data.pointer = (caddr_t) info->b.key;
    wrq.u.data.length = IW_ENCODING_TOKEN_MAX;
    wrq.u.data.flags = 0;
    if(iw_get_ext(skfd, ifname, SIOCGIWENCODE, &wrq) >= 0)
    {
        info->b.has_key = 1;
        info->b.key_size = wrq.u.data.length;
        info->b.key_flags = wrq.u.data.flags;
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWENCODE, &wrq) >= 0){
        info->has_enc = 1;
        info->enc_method = wrq.u.encoding.method;
        info->enc_key = wrq.u.encoding.code;
    }
    #endif
    
    /* Get ESSID */
    wrq.u.essid.pointer = (caddr_t) info->b.essid;
    wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
    wrq.u.essid.flags = 0;
    if(iw_get_ext(skfd, ifname, SIOCGIWESSID, &wrq) >= 0)
    {
        info->b.has_essid = 1;
        info->b.essid_on = wrq.u.data.flags;
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    wrq.u.data.pointer = (caddr_t) info->essid;
    wrq.u.data.length = 0;
    wrq.u.data.flags = 0;
    if(ioctl(skfd, SIOCGIWESSID, &wrq) >= 0){
        info->has_essid = 1;
        info->essid_on = wrq.u.data.flags;
    }
    #endif
    
    /* Get operation mode */
    if(iw_get_ext(skfd, ifname, SIOCGIWMODE, &wrq) >= 0)
    {
        info->b.has_mode = 1;
        /* Note : event->u.mode is unsigned, no need to check <= 0 */
        if(wrq.u.mode < IW_NUM_OPER_MODE)
            info->b.mode = wrq.u.mode;
        else
            info->b.mode = IW_NUM_OPER_MODE;	/* Unknown/bug */
    }
    
    /* Get AP address */
    if(iw_get_ext(skfd, ifname, SIOCGIWAP, &wrq) >= 0)
    {
        info->has_ap_addr = 1;
        memcpy(&(info->ap_addr), &(wrq.u.ap_addr), sizeof (sockaddr));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWAP, &wrq) >= 0){
        info->has_ap_addr = 1;
        memcpy(&(info->ap_addr), &(wrq.u.ap_addr), sizeof (sockaddr));
    }
    #endif
    
    /* Get NickName */
    wrq.u.essid.pointer = (caddr_t) info->nickname;
    wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
    wrq.u.essid.flags = 0;
    if(iw_get_ext(skfd, ifname, SIOCGIWNICKN, &wrq) >= 0)
        if(wrq.u.data.length > 1)
          info->has_nickname = 1;
    #if 0
    strcpy(wrq.ifr_name, ifname);
    wrq.u.data.pointer = (caddr_t) info->nickname;
    wrq.u.data.length = 0;
    wrq.u.data.flags = 0;
    if(ioctl(skfd, SIOCGIWNICKN, &wrq) >= 0)
    if(wrq.u.data.length > 1)
        info->has_nickname = 1;
    #endif

    /* Get bit rate */
    if(iw_get_ext(skfd, ifname, SIOCGIWRATE, &wrq) >= 0)
    {
        info->has_bitrate = 1;
        memcpy(&(info->bitrate), &(wrq.u.bitrate), sizeof(iwparam));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWRATE, &wrq) >= 0){
        info->has_bitrate = 1;
        info->bitrate_fixed = wrq.u.bitrate.fixed;
        info->bitrate = wrq.u.bitrate.value;
    }
    #endif
    
    /* Get RTS threshold */
    if(iw_get_ext(skfd, ifname, SIOCGIWRTS, &wrq) >= 0)
    {
        info->has_rts = 1;
        memcpy(&(info->rts), &(wrq.u.rts), sizeof(iwparam));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWRTS, &wrq) >= 0){
        info->has_rts = 1;
        info->rts_fixed = wrq.u.rts.fixed;
        info->rts = wrq.u.rts.value;
    }
    #endif

    /* Get fragmentation thershold */
    if(iw_get_ext(skfd, ifname, SIOCGIWFRAG, &wrq) >= 0)
    {
        info->has_frag = 1;
        memcpy(&(info->frag), &(wrq.u.frag), sizeof(iwparam));
    }
    #if 0
    strcpy(wrq.ifr_name, ifname);
    if(ioctl(skfd, SIOCGIWFRAG, &wrq) >= 0){
        info->has_frag = 1;
        info->frag_fixed = wrq.u.frag.fixed;
        info->frag = wrq.u.frag.value;
    }
    #endif

    /* Get stats */
    if(iw_get_stats(skfd, ifname, &(info->stats),
          &info->range, info->has_range) >= 0)
    {
        info->has_stats = 1;
    }
    #if 0
    if(iw_getstats(ifname, &(info->stats)) == 0){
        info->has_stats = 1;
    }
    #endif
    
    /* Get ranges */
    //if(get_range_info(skfd, ifname, &(info->range)) >= 0)
    if(iw_get_range_info(skfd, ifname, &(info->range)) >= 0)
        info->has_range = 1;

    /* Get Power Management settings */
    wrq.u.power.flags = 0;
    if(iw_get_ext(skfd, ifname, SIOCGIWPOWER, &wrq) >= 0)
    {
        info->has_power = 1;
        memcpy(&(info->power), &(wrq.u.power), sizeof(iwparam));
    }

#ifdef DISPLAY_WPA
  /* Note : currently disabled to not bloat iwconfig output. Also,
   * if does not make total sense to display parameters that we
   * don't allow (yet) to configure.
   * For now, use iwlist instead... Jean II */

  /* Get WPA/802.1x/802.11i security parameters */
  if((info->has_range) && (info->range.we_version_compiled > 17))
    {
      wrq.u.param.flags = IW_AUTH_KEY_MGMT;
      if(iw_get_ext(skfd, ifname, SIOCGIWAUTH, &wrq) >= 0)
	{
	  info->has_auth_key_mgmt = 1;
	  info->auth_key_mgmt = wrq.u.param.value;
	}

      wrq.u.param.flags = IW_AUTH_CIPHER_PAIRWISE;
      if(iw_get_ext(skfd, ifname, SIOCGIWAUTH, &wrq) >= 0)
	{
	  info->has_auth_cipher_pairwise = 1;
	  info->auth_cipher_pairwise = wrq.u.param.value;
	}

      wrq.u.param.flags = IW_AUTH_CIPHER_GROUP;
      if(iw_get_ext(skfd, ifname, SIOCGIWAUTH, &wrq) >= 0)
	{
	  info->has_auth_cipher_group = 1;
	  info->auth_cipher_group = wrq.u.param.value;
	}
    }
#endif

#if IWCFG_EXTENTIONAL
    if((info->has_range) && (info->range.we_version_compiled > 10))
    {
        /* Get retry limit/lifetime */
        if(iw_get_ext(skfd, ifname, SIOCGIWRETRY, &wrq) >= 0)
        {
            info->has_retry = 1;
            memcpy(&(info->retry), &(wrq.u.retry), sizeof(iwparam));
        }
    }

    if((info->has_range) && (info->range.we_version_compiled > 9))
    {
        /* Get Transmit Power */
        if(iw_get_ext(skfd, ifname, SIOCGIWTXPOW, &wrq) >= 0)
        {
            info->has_txpower = 1;
            memcpy(&(info->txpower), &(wrq.u.txpower), sizeof(iwparam));
        }
    }
#endif
    return(0);
}

/*
    print all infs ' info
*/
void IWCFG_PrintInfo(int skfd, char *ifname)
{
    struct wireless_info	info;
    char buffer[128];	/* Temporary buffer */

    /* One token is more of less 5 characters, 14 tokens per line */
    int	tokens = 3;	/* For name */
    
    if(IWCFG_GetInfo(skfd, ifname, &info) < 0){
        IWCFG_DEBUG("%-8.8s  no wireless extensions.\n\n",ifname);
        return;
    }
    
    /* Display it ! */
    
    /* Display device name and wireless name (name of the protocol used) */
    //printf("%-8.8s  %s  ", ifname, info.name);
    IWCFG_DEBUG("%-8.16s  %s  ", ifname, info.b.name);
    
    /* Display ESSID (extended network), if any */
    if(info.b.has_essid)
    {
        if(info.b.essid_on)
        {
            /* Does it have an ESSID index ? */
            if((info.b.essid_on & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG("ESSID:\"%s\" [%d]  ", info.b.essid,
                       (info.b.essid_on & IW_ENCODE_INDEX));
            else
                IWCFG_DEBUG("ESSID:\"%s\"  ", info.b.essid);
        }
        else
            IWCFG_DEBUG("ESSID:off/any  ");
    }
    
    /* Display NickName (station name), if any */
    if(info.has_nickname)
        IWCFG_DEBUG("Nickname:\"%s\"", info.nickname);

    /* Formatting */
    if(info.b.has_essid || info.has_nickname)
    {
        IWCFG_DEBUG("\n          ");
        tokens = 0;
    }

    /* Display Network ID */
    if(info.b.has_nwid)
    {
        /* Note : should display proper number of digits according to info
        * in range structure */
        if(info.b.nwid.disabled)
            IWCFG_DEBUG("NWID:off/any  ");
        else
            IWCFG_DEBUG("NWID:%X  ", info.b.nwid.value);
        tokens +=2;
    }

    /* Display the current mode of operation */
    if(info.b.has_mode)
    {
        IWCFG_DEBUG("Mode:%s  ", iw_operation_mode[info.b.mode]);
        tokens +=3;
    }

    /* Display frequency / channel */
    if(info.b.has_freq)
    {
        double		freq = info.b.freq;	/* Frequency/channel */
        int		channel = -1;		/* Converted to channel */
        /* Some drivers insist of returning channel instead of frequency.
        * This fixes them up. Note that, driver should still return
        * frequency, because other tools depend on it. */
        if(info.has_range && (freq < KILO))
            channel = iw_channel_to_freq((int) freq, &freq, &info.range);
        /* Display */
        iw_print_freq(buffer, sizeof(buffer), freq, -1, info.b.freq_flags);
        IWCFG_DEBUG("%s  ", buffer);
        tokens +=4;
    }

    /* Display sensitivity */
    if(info.has_sens)
    {
        /* A bit of clever formatting */
        if(tokens > 10)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=4;

        /* Fixed ? */
        IWCFG_DEBUG("Sensitivity%c", info.sens.fixed ? '=' : ':');

        if(info.has_range)
            /* Display in dBm ? */
            if(info.sens.value < 0)
                IWCFG_DEBUG("%d dBm  ", info.sens.value);
            else
                IWCFG_DEBUG("%d/%d  ", info.sens.value, info.range.sensitivity);
        else
            IWCFG_DEBUG("%d  ", info.sens.value);
    }

    /* Display the address of the current Access Point */
    if(info.has_ap_addr)
    {
        /* A bit of clever formatting */
        if(tokens > 8)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=6;

        /* Oups ! No Access Point in Ad-Hoc mode */
        if((info.b.has_mode) && (info.b.mode == IW_MODE_ADHOC))
            IWCFG_DEBUG("Cell:");
        else
            IWCFG_DEBUG("Access Point:");
        
        IWCFG_DEBUG(" %s   ", iw_sawap_ntop(&info.ap_addr, buffer));
    }

    /* Display the currently used/set bit-rate */
    if(info.has_bitrate)
    {
        /* A bit of clever formatting */
        if(tokens > 11)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=3;

        /* Display it */
        iw_print_bitrate(buffer, sizeof(buffer), info.bitrate.value);
        IWCFG_DEBUG("Bit Rate%c%s   ", (info.bitrate.fixed ? '=' : ':'), buffer);
    }
    
    printf("\n          ");
    tokens = 0;
    
    /* Display the RTS threshold */
    if(info.has_rts)
    {
        /* Disabled ? */
        if(info.rts.disabled)
            IWCFG_DEBUG("RTS thr:off   ");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("RTS thr%c%d B   ",
            info.rts.fixed ? '=' : ':',
            info.rts.value);
        }
        tokens += 3;
    }

    /* Display the fragmentation threshold */
    if(info.has_frag)
    {
        /* A bit of clever formatting */
        if(tokens > 10)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=4;

        /* Disabled ? */
        if(info.frag.disabled)
            IWCFG_DEBUG("Fragment thr:off");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("Fragment thr%c%d B   ",
             info.frag.fixed ? '=' : ':',
             info.frag.value);
        }
    }

    /* Formating */
    if(tokens > 0)
    IWCFG_DEBUG("\n          ");

    /* Display encryption information */
    /* Note : we display only the "current" key, use iwlist to list all keys */
    if(info.b.has_key)
    {
        IWCFG_DEBUG("Encryption key:");
        if((info.b.key_flags & IW_ENCODE_DISABLED) || (info.b.key_size == 0))
            IWCFG_DEBUG("off");
        else
        {
            /* Display the key */
            iw_print_key(buffer, sizeof(buffer),
                   info.b.key, info.b.key_size, info.b.key_flags);
            IWCFG_DEBUG("%s", buffer);

            /* Other info... */
            if((info.b.key_flags & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG(" [%d]", info.b.key_flags & IW_ENCODE_INDEX);
            if(info.b.key_flags & IW_ENCODE_RESTRICTED)
                IWCFG_DEBUG("   Security mode:restricted");
            if(info.b.key_flags & IW_ENCODE_OPEN)
                IWCFG_DEBUG("   Security mode:open");
        }
        IWCFG_DEBUG("\n          ");
    }

    /* Display statistics */
    if(info.has_stats)
    {
        iw_print_stats(buffer, sizeof(buffer),
             &info.stats.qual, &info.range, info.has_range);
            IWCFG_DEBUG("Link %s\n", buffer);

        if(info.range.we_version_compiled > 11)
            IWCFG_DEBUG("          Rx invalid nwid:%d  Rx invalid crypt:%d  Rx invalid frag:%d\n          Tx excessive retries:%d  Invalid misc:%d   Missed beacon:%d\n",
                   info.stats.discard.nwid,
                   info.stats.discard.code,
                   info.stats.discard.fragment,
                   info.stats.discard.retries,
                   info.stats.discard.misc,
                   info.stats.miss.beacon);
        else
            IWCFG_DEBUG("          Rx invalid nwid:%d  invalid crypt:%d  invalid misc:%d\n",
                   info.stats.discard.nwid,
                   info.stats.discard.code,
                   info.stats.discard.misc);
    }

#if IWCFG_EXTENTIONAL

    /* Display the Transmit Power */
    if(info.has_txpower)
    {
        /* A bit of clever formatting */
        if(tokens > 11)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=3;

        /* Display it */
        iw_print_txpower(buffer, sizeof(buffer), &info.txpower);
        IWCFG_DEBUG("Tx-Power%c%s   ", (info.txpower.fixed ? '=' : ':'), buffer);
    }
  
    /* Display retry limit/lifetime information */
    if(info.has_retry)
    { 
        IWCFG_DEBUG("Retry");
        /* Disabled ? */
        if(info.retry.disabled)
            IWCFG_DEBUG(":off");
        else
        {
            /* Let's check the value and its type */
            if(info.retry.flags & IW_RETRY_TYPE)
            {
                iw_print_retry_value(buffer, sizeof(buffer),
                   info.retry.value, info.retry.flags,
                   info.range.we_version_compiled);
                IWCFG_DEBUG("%s", buffer);
            }

            /* Let's check if nothing (simply on) */
            if(info.retry.flags == IW_RETRY_ON)
                IWCFG_DEBUG(":on");
        }
        IWCFG_DEBUG("   ");
        tokens += 5;	/* Between 3 and 5, depend on flags */
    }

    /* Display Power Management information */
    /* Note : we display only one parameter, period or timeout. If a device
    * (such as HiperLan) has both, the user need to use iwlist... */
    if(info.has_power)	/* I hope the device has power ;-) */
    { 
        IWCFG_DEBUG("Power Management");
        /* Disabled ? */
        if(info.power.disabled)
            IWCFG_DEBUG(":off");
        else
        {
            /* Let's check the value and its type */
            if(info.power.flags & IW_POWER_TYPE)
            {
                iw_print_pm_value(buffer, sizeof(buffer),
                	info.power.value, info.power.flags,
                	info.range.we_version_compiled);
                IWCFG_DEBUG("%s  ", buffer);
            }

            /* Let's check the mode */
            iw_print_pm_mode(buffer, sizeof(buffer), info.power.flags);
            IWCFG_DEBUG("%s", buffer);

            /* Let's check if nothing (simply on) */
            if(info.power.flags == IW_POWER_ON)
                IWCFG_DEBUG(":on");
        }
        IWCFG_DEBUG("\n          ");
    }
#endif

    IWCFG_DEBUG("\n");
    
    return;
}

void IWCFG_DisplayEssid(wireless_info *info)
{
    /* Display ESSID (extended network), if any */
    if(info->b.has_essid)
    {
        if(info->b.essid_on)
        {
            /* Does it have an ESSID index ? */
            if((info->b.essid_on & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG("ESSID:\"%s\" [%d]  ", info->b.essid,
                       (info->b.essid_on & IW_ENCODE_INDEX));
            else
                IWCFG_DEBUG("ESSID:\"%s\"  ", info->b.essid);
        }
        else
            IWCFG_DEBUG("ESSID:off/any  ");
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayApaddr(wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */
    /* Display the address of the current Access Point */
    if(info->has_ap_addr)
    {
        /* Oups ! No Access Point in Ad-Hoc mode */
        if((info->b.has_mode) && (info->b.mode == IW_MODE_ADHOC))
            IWCFG_DEBUG("Cell:");
        else
            IWCFG_DEBUG("Access Point:");
        
        IWCFG_DEBUG(" %s   ", iw_sawap_ntop(&info->ap_addr, buffer));
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayNwid(wireless_info *info)
{
    /* Display Network ID */
    if(info->b.has_nwid)
    {
        /* Note : should display proper number of digits according to info
        * in range structure */
        if(info->b.nwid.disabled)
            IWCFG_DEBUG("NWID:off/any  ");
        else
            IWCFG_DEBUG("NWID:%X  ", info->b.nwid.value);
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayMode(wireless_info *info)
{
    /* Display the current mode of operation */
    if(info->b.has_mode)
    {
        IWCFG_DEBUG("Mode:%s  ", iw_operation_mode[info->b.mode]);
        IWCFG_DEBUG("\n");
    }
}

void IWCFG_DisplaySens(wireless_info *info)
{
     /* Display sensitivity */
    if(info->has_sens)
    {
        /* Fixed ? */
        IWCFG_DEBUG("Sensitivity%c", info->sens.fixed ? '=' : ':');

        if(info->has_range)
            /* Display in dBm ? */
            if(info->sens.value < 0)
                IWCFG_DEBUG("%d dBm  ", info->sens.value);
            else
                IWCFG_DEBUG("%d/%d  ", info->sens.value, info->range.sensitivity);
        else
            IWCFG_DEBUG("%d  ", info->sens.value);
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayFreq(wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */
    /* Display frequency / channel */
    if(info->b.has_freq)
    {
        double		freq = info->b.freq;	/* Frequency/channel */
        int		channel = -1;		/* Converted to channel */
        /* Some drivers insist of returning channel instead of frequency.
        * This fixes them up. Note that, driver should still return
        * frequency, because other tools depend on it. */
        if(info->has_range && (freq < KILO))
            channel = iw_channel_to_freq((int) freq, &freq, &info->range);
        /* Display */
        iw_print_freq(buffer, sizeof(buffer), freq, -1, info->b.freq_flags);
        IWCFG_DEBUG("%s  ", buffer);
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayChannel(wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */
    /* Display frequency / channel */
    if(info->b.has_freq)
    {
        double		freq = info->b.freq;	/* Frequency/channel */
        int		channel = -1;		/* Converted to channel */
        /* Some drivers insist of returning channel instead of frequency.
        * This fixes them up. Note that, driver should still return
        * frequency, because other tools depend on it. */
        if(info->has_range && (freq < KILO))
            channel = iw_channel_to_freq((int) freq, &freq, &info->range);
        /* Display */
        iw_print_freq(buffer, sizeof(buffer), freq, -1, info->b.freq_flags);
        IWCFG_DEBUG("%s  ", buffer);
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayRate(wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */
    /* Display the currently used/set bit-rate */
    if(info->has_bitrate)
    {
        /* Display it */
        iw_print_bitrate(buffer, sizeof(buffer), info->bitrate.value);
        IWCFG_DEBUG("Bit Rate%c%s   ", (info->bitrate.fixed ? '=' : ':'), buffer);
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayRts(wireless_info *info)
{
    /* Display the RTS threshold */
    if(info->has_rts)
    {
        /* Disabled ? */
        if(info->rts.disabled)
            IWCFG_DEBUG("RTS thr:off   ");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("RTS thr%c%d B   ",
            info->rts.fixed ? '=' : ':',
            info->rts.value);
        }
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayFrag(wireless_info *info)
{
    /* Display the fragmentation threshold */
    if(info->has_frag)
    {
        /* Disabled ? */
        if(info->frag.disabled)
            IWCFG_DEBUG("Fragment thr:off");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("Fragment thr%c%d B   ",
             info->frag.fixed ? '=' : ':',
             info->frag.value);
        }
    }
    IWCFG_DEBUG("\n");
}

void IWCFG_DisplayEnc(wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */
    /* Display encryption information */
    /* Note : we display only the "current" key, use iwlist to list all keys */
    if(info->b.has_key)
    {
        IWCFG_DEBUG("Encryption key:");
        if((info->b.key_flags & IW_ENCODE_DISABLED) || (info->b.key_size == 0))
            IWCFG_DEBUG("off");
        else
        {
            /* Display the key */
            iw_print_key(buffer, sizeof(buffer),
                   info->b.key, info->b.key_size, info->b.key_flags);
            IWCFG_DEBUG("%s", buffer);

            /* Other info... */
            if((info->b.key_flags & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG(" [%d]", info->b.key_flags & IW_ENCODE_INDEX);
            if(info->b.key_flags & IW_ENCODE_RESTRICTED)
                IWCFG_DEBUG("   Security mode:restricted");
            if(info->b.key_flags & IW_ENCODE_OPEN)
                IWCFG_DEBUG("   Security mode:open");
        }
        IWCFG_DEBUG("\n          ");
    }
}

void IWCFG_DisplayNick(wireless_info *info)
{
     /* Display NickName (station name), if any */
    if(info->has_nickname)
        IWCFG_DEBUG("Nickname:\"%s\"", info->nickname);
    IWCFG_DEBUG("\n");
}

/*
    print all infs ' info
*/
void IWCFG_DisplayInfo(int skfd, char *ifname, wireless_info *info)
{
    char buffer[128];	/* Temporary buffer */

    /* One token is more of less 5 characters, 14 tokens per line */
    int	tokens = 3;	/* For name */
    
    /* Display it ! */
    
    /* Display device name and wireless name (name of the protocol used) */
    //printf("%-8.8s  %s  ", ifname, info.name);
    IWCFG_DEBUG("%-8.16s  %s  ", ifname, info->b.name);
    
    /* Display ESSID (extended network), if any */
    if(info->b.has_essid)
    {
        if(info->b.essid_on)
        {
            /* Does it have an ESSID index ? */
            if((info->b.essid_on & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG("ESSID:\"%s\" [%d]  ", info->b.essid,
                       (info->b.essid_on & IW_ENCODE_INDEX));
            else
                IWCFG_DEBUG("ESSID:\"%s\"  ", info->b.essid);
        }
        else
            IWCFG_DEBUG("ESSID:off/any  ");
    }
    
    /* Display NickName (station name), if any */
    if(info->has_nickname)
        IWCFG_DEBUG("Nickname:\"%s\"", info->nickname);

    /* Formatting */
    if(info->b.has_essid || info->has_nickname)
    {
        IWCFG_DEBUG("\n          ");
        tokens = 0;
    }

    /* Display Network ID */
    if(info->b.has_nwid)
    {
        /* Note : should display proper number of digits according to info
        * in range structure */
        if(info->b.nwid.disabled)
            IWCFG_DEBUG("NWID:off/any  ");
        else
            IWCFG_DEBUG("NWID:%X  ", info->b.nwid.value);
        tokens +=2;
    }

    /* Display the current mode of operation */
    if(info->b.has_mode)
    {
        IWCFG_DEBUG("Mode:%s  ", iw_operation_mode[info->b.mode]);
        tokens +=3;
    }

    /* Display frequency / channel */
    if(info->b.has_freq)
    {
        double		freq = info->b.freq;	/* Frequency/channel */
        int		channel = -1;		/* Converted to channel */
        /* Some drivers insist of returning channel instead of frequency.
        * This fixes them up. Note that, driver should still return
        * frequency, because other tools depend on it. */
        if(info->has_range && (freq < KILO))
            channel = iw_channel_to_freq((int) freq, &freq, &info->range);
        /* Display */
        iw_print_freq(buffer, sizeof(buffer), freq, -1, info->b.freq_flags);
        IWCFG_DEBUG("%s  ", buffer);
        tokens +=4;
    }

    /* Display sensitivity */
    if(info->has_sens)
    {
        /* A bit of clever formatting */
        if(tokens > 10)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=4;

        /* Fixed ? */
        IWCFG_DEBUG("Sensitivity%c", info->sens.fixed ? '=' : ':');

        if(info->has_range)
            /* Display in dBm ? */
            if(info->sens.value < 0)
                IWCFG_DEBUG("%d dBm  ", info->sens.value);
            else
                IWCFG_DEBUG("%d/%d  ", info->sens.value, info->range.sensitivity);
        else
            IWCFG_DEBUG("%d  ", info->sens.value);
    }

    /* Display the address of the current Access Point */
    if(info->has_ap_addr)
    {
        /* A bit of clever formatting */
        if(tokens > 8)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=6;

        /* Oups ! No Access Point in Ad-Hoc mode */
        if((info->b.has_mode) && (info->b.mode == IW_MODE_ADHOC))
            IWCFG_DEBUG("Cell:");
        else
            IWCFG_DEBUG("Access Point:");
        
        IWCFG_DEBUG(" %s   ", iw_sawap_ntop(&info->ap_addr, buffer));
    }

    /* Display the currently used/set bit-rate */
    if(info->has_bitrate)
    {
        /* A bit of clever formatting */
        if(tokens > 11)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=3;

        /* Display it */
        iw_print_bitrate(buffer, sizeof(buffer), info->bitrate.value);
        IWCFG_DEBUG("Bit Rate%c%s   ", (info->bitrate.fixed ? '=' : ':'), buffer);
    }

    /* Display the RTS threshold */
    if(info->has_rts)
    {
        /* Disabled ? */
        if(info->rts.disabled)
            IWCFG_DEBUG("RTS thr:off   ");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("RTS thr%c%d B   ",
            info->rts.fixed ? '=' : ':',
            info->rts.value);
        }
        tokens += 3;
    }

    /* Display the fragmentation threshold */
    if(info->has_frag)
    {
        /* A bit of clever formatting */
        if(tokens > 10)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=4;

        /* Disabled ? */
        if(info->frag.disabled)
            IWCFG_DEBUG("Fragment thr:off");
        else
        {
            /* Fixed ? */
            IWCFG_DEBUG("Fragment thr%c%d B   ",
             info->frag.fixed ? '=' : ':',
             info->frag.value);
        }
    }

    /* Formating */
    if(tokens > 0)
    IWCFG_DEBUG("\n          ");

    /* Display encryption information */
    /* Note : we display only the "current" key, use iwlist to list all keys */
    if(info->b.has_key)
    {
        IWCFG_DEBUG("Encryption key:");
        if((info->b.key_flags & IW_ENCODE_DISABLED) || (info->b.key_size == 0))
            IWCFG_DEBUG("off");
        else
        {
            /* Display the key */
            iw_print_key(buffer, sizeof(buffer),
                   info->b.key, info->b.key_size, info->b.key_flags);
            IWCFG_DEBUG("%s", buffer);

            /* Other info... */
            if((info->b.key_flags & IW_ENCODE_INDEX) > 1)
                IWCFG_DEBUG(" [%d]", info->b.key_flags & IW_ENCODE_INDEX);
            if(info->b.key_flags & IW_ENCODE_RESTRICTED)
                IWCFG_DEBUG("   Security mode:restricted");
            if(info->b.key_flags & IW_ENCODE_OPEN)
                IWCFG_DEBUG("   Security mode:open");
        }
        IWCFG_DEBUG("\n          ");
    }

    /* Display statistics */
    if(info->has_stats)
    {
        iw_print_stats(buffer, sizeof(buffer),
             &info->stats.qual, &info->range, info->has_range);
            IWCFG_DEBUG("Link %s\n", buffer);

        if(info->range.we_version_compiled > 11)
            IWCFG_DEBUG("          Rx invalid nwid:%d  Rx invalid crypt:%d  Rx invalid frag:%d\n          Tx excessive retries:%d  Invalid misc:%d   Missed beacon:%d\n",
                   info->stats.discard.nwid,
                   info->stats.discard.code,
                   info->stats.discard.fragment,
                   info->stats.discard.retries,
                   info->stats.discard.misc,
                   info->stats.miss.beacon);
        else
            IWCFG_DEBUG("          Rx invalid nwid:%d  invalid crypt:%d  invalid misc:%d\n",
                   info->stats.discard.nwid,
                   info->stats.discard.code,
                   info->stats.discard.misc);
    }

#if IWCFG_EXTENTIONAL

    /* Display the Transmit Power */
    if(info->has_txpower)
    {
        /* A bit of clever formatting */
        if(tokens > 11)
        {
            IWCFG_DEBUG("\n          ");
            tokens = 0;
        }
        tokens +=3;

        /* Display it */
        iw_print_txpower(buffer, sizeof(buffer), &info->txpower);
        IWCFG_DEBUG("Tx-Power%c%s   ", (info->txpower.fixed ? '=' : ':'), buffer);
    }
  
    /* Display retry limit/lifetime information */
    if(info->has_retry)
    { 
        IWCFG_DEBUG("Retry");
        /* Disabled ? */
        if(info->retry.disabled)
            IWCFG_DEBUG(":off");
        else
        {
            /* Let's check the value and its type */
            if(info->retry.flags & IW_RETRY_TYPE)
            {
                iw_print_retry_value(buffer, sizeof(buffer),
                   info->retry.value, info->retry.flags,
                   info->range.we_version_compiled);
                IWCFG_DEBUG("%s", buffer);
            }

            /* Let's check if nothing (simply on) */
            if(info->retry.flags == IW_RETRY_ON)
                IWCFG_DEBUG(":on");
        }
        IWCFG_DEBUG("   ");
        tokens += 5;	/* Between 3 and 5, depend on flags */
    }

    /* Display Power Management information */
    /* Note : we display only one parameter, period or timeout. If a device
    * (such as HiperLan) has both, the user need to use iwlist... */
    if(info->has_power)	/* I hope the device has power ;-) */
    { 
        IWCFG_DEBUG("Power Management");
        /* Disabled ? */
        if(info->power.disabled)
            IWCFG_DEBUG(":off");
        else
        {
            /* Let's check the value and its type */
            if(info->power.flags & IW_POWER_TYPE)
            {
                iw_print_pm_value(buffer, sizeof(buffer),
                	info->power.value, info->power.flags,
                	info->range.we_version_compiled);
                IWCFG_DEBUG("%s  ", buffer);
            }

            /* Let's check the mode */
            iw_print_pm_mode(buffer, sizeof(buffer), info->power.flags);
            IWCFG_DEBUG("%s", buffer);

            /* Let's check if nothing (simply on) */
            if(info->power.flags == IW_POWER_ON)
                IWCFG_DEBUG(":on");
        }
        IWCFG_DEBUG("\n          ");
    }
#endif

    IWCFG_DEBUG("\n");
    
    return;
}

/*
    display all wireless interfaces 
*/
int IWCFG_DisplayALLIfs()
{
    int i;
    char buff[1024];
    struct ifconf ifc;
    struct ifreq *ifr;
    int skfd = -1;		/* generic raw socket desc.	*/

    /* Create a channel to the NET kernel. */
    if((skfd = sockets_open()) < 0){
        IWCFG_DEBUG("IWCFG_DisplayALLIfs: socket open error! \n");
        return -1;
    }

    /* Get list of active devices */
    ifc.ifc_len = sizeof(buff);
    ifc.ifc_buf = buff;
    if(ioctl(skfd, SIOCGIFCONF, &ifc) < 0){
        IWCFG_DEBUG("IWCFG_DisplayALLIfs: ioctl SIOCGIFCONF error \n");
        return -1;
    }
    ifr = ifc.ifc_req;

    /* Print them */
    for(i = ifc.ifc_len / sizeof(struct ifreq); --i >= 0; ifr++){
        IWCFG_PrintInfo(skfd, ifr->ifr_name);
    }

    iw_sockets_close(skfd);
    return 0;
}

/*********************** SCANNING SUBROUTINES ***********************/
/*
 * The Wireless Extension API 14 and greater define Wireless Scanning.
 * The normal API is complex, this is an easy API that return
 * a subset of the scanning results. This should be enough for most
 * applications that want to use Scanning.
 * If you want to have use the full/normal API, check iwlist.c...
 *
 * Precaution when using scanning :
 * The scanning operation disable normal network traffic, and therefore
 * you should not abuse of scan.
 * The scan need to check the presence of network on other frequencies.
 * While you are checking those other frequencies, you can *NOT* be on
 * your normal frequency to listen to normal traffic in the cell.
 * You need typically in the order of one second to actively probe all
 * 802.11b channels (do the maths). Some cards may do that in background,
 * to reply to scan commands faster, but they still have to do it.
 * Leaving the cell for such an extended period of time is pretty bad.
 * Any kind of streaming/low latency traffic will be impacted, and the
 * user will perceive it (easily checked with telnet). People trying to
 * send traffic to you will retry packets and waste bandwidth. Some
 * applications may be sensitive to those packet losses in weird ways,
 * and tracing those weird behavior back to scanning may take time.
 * If you are in ad-hoc mode, if two nodes scan approx at the same
 * time, they won't see each other, which may create associations issues.
 * For those reasons, the scanning activity should be limited to
 * what's really needed, and continuous scanning is a bad idea.
 * Jean II
 */

/*------------------------------------------------------------------*/
/*
 * Process/store one element from the scanning results in wireless_scan
 */
static inline struct wireless_scan *
iw_process_scanning_token(struct iw_event *event,
			  struct wireless_scan *wscan)
{
    struct wireless_scan *oldwscan;

    /* Now, let's decode the event */
    switch(event->cmd)
    {
        case SIOCGIWAP:
          /* New cell description. Allocate new cell descriptor, zero it. */
          oldwscan = wscan;
          wscan = (struct wireless_scan *) malloc(sizeof(struct wireless_scan));
          if(wscan == NULL)
            return(wscan);
          /* Link at the end of the list */
          if(oldwscan != NULL)
            oldwscan->next = wscan;

          /* Reset it */
          bzero(wscan, sizeof(struct wireless_scan));

          /* Save cell identifier */
          wscan->has_ap_addr = 1;
          memcpy(&(wscan->ap_addr), &(event->u.ap_addr), sizeof(sockaddr));
          break;
        case SIOCGIWNWID:
          wscan->b.has_nwid = 1;
          memcpy(&(wscan->b.nwid), &(event->u.nwid), sizeof(iwparam));
          break;
        case SIOCGIWFREQ:
          wscan->b.has_freq = 1;
          wscan->b.freq = iw_freq2float(&(event->u.freq));
          wscan->b.freq_flags = event->u.freq.flags;
          break;
        case SIOCGIWMODE:
          wscan->b.mode = event->u.mode;
          if((wscan->b.mode < IW_NUM_OPER_MODE) && (wscan->b.mode >= 0))
            wscan->b.has_mode = 1;
          break;
        case SIOCGIWESSID:
          wscan->b.has_essid = 1;
          wscan->b.essid_on = event->u.data.flags;
          memset(wscan->b.essid, '\0', IW_ESSID_MAX_SIZE+1);
          if((event->u.essid.pointer) && (event->u.essid.length))
            memcpy(wscan->b.essid, event->u.essid.pointer, event->u.essid.length);
          break;
        case SIOCGIWENCODE:
          wscan->b.has_key = 1;
          wscan->b.key_size = event->u.data.length;
          wscan->b.key_flags = event->u.data.flags;
          if(event->u.data.pointer)
            memcpy(wscan->b.key, event->u.essid.pointer, event->u.data.length);
          else
            wscan->b.key_flags |= IW_ENCODE_NOKEY;
          break;
        case IWEVQUAL:
          /* We don't get complete stats, only qual */
          wscan->has_stats = 1;
          memcpy(&wscan->stats.qual, &event->u.qual, sizeof(struct iw_quality));
          break;
        case SIOCGIWRATE:
          /* Scan may return a list of bitrates. As we have space for only
           * a single bitrate, we only keep the largest one. */
#if (WIRELESS_EXT > 20)
          if((!wscan->has_maxbitrate) ||
             (event->u.bitrate.value > wscan->maxbitrate.value))
          {
            wscan->has_maxbitrate = 1;
            memcpy(&(wscan->maxbitrate), &(event->u.bitrate), sizeof(iwparam));
          }
#endif
        case IWEVCUSTOM:
          /* How can we deal with those sanely ? Jean II */
        default:
          break;
    }	/* switch(event->cmd) */

    return(wscan);
}

/*------------------------------------------------------------------*/
/*
 * Initiate the scan procedure, and process results.
 * This is a non-blocking procedure and it will return each time
 * it would block, returning the amount of time the caller should wait
 * before calling again.
 * Return -1 for error, delay to wait for (in ms), or 0 for success.
 * Error code is in errno
 */
int iw_process_scan(int skfd,
                    		char *ifname,
                    		int	we_version,
                    		wireless_scan_head *context)
{
    struct iwreq		wrq;
    unsigned char *buffer = NULL;		/* Results */
    int buflen = IW_SCAN_MAX_DATA; /* Min for compat WE<17 */
    unsigned char *newbuf;
    
    /* Don't waste too much time on interfaces (150 * 100 = 15s) */
    context->retry++;
    if(context->retry > 150){
        IWCFG_DEBUG("iw_process_scan: out of retry range \n");
        return(-1);
    }

    /* If we have not yet initiated scanning on the interface */
    if(context->retry == 1){
        /* Initiate Scan */
        wrq.u.data.pointer = NULL;		/* Later */
        wrq.u.data.flags = 0;
        wrq.u.data.length = 0;
        /* Remember that as non-root, we will get an EPERM here */
        if((iw_set_ext(skfd, ifname, SIOCSIWSCAN, &wrq) < 0)){
            IWCFG_DEBUG("iw_process_scan: SIOCSIWSCAN error \n");
            return(-1);
        }
        /* Success : now, just wait for event or results */
        return(250);	/* Wait 250 ms */
    }

 realloc:
    /* (Re)allocate the buffer - realloc(NULL, len) == malloc(len) */
    newbuf = realloc(buffer, buflen);
    if(newbuf == NULL){
        /* man says : If realloc() fails the original block is left untouched */
        if(buffer){
            free(buffer);
            //errno = ENOMEM;
            IWCFG_DEBUG("iw_process_scan: realloc error \n");
        }
        return(-1);
    }
    buffer = newbuf;

    /* Try to read the results */
    wrq.u.data.pointer = buffer;
    wrq.u.data.flags = 0;
    wrq.u.data.length = buflen;
    if(iw_get_ext(skfd, ifname, SIOCGIWSCAN, &wrq) < 0){
        /* Check if buffer was too small (WE-17 only) */
        if((errno == E2BIG) && (we_version > 16)){
            /* Some driver may return very large scan results, either
            * because there are many cells, or because they have many
            * large elements in cells (like IWEVCUSTOM). Most will
            * only need the regular sized buffer. We now use a dynamic
            * allocation of the buffer to satisfy everybody. Of course,
            * as we don't know in advance the size of the array, we try
            * various increasing sizes. Jean II */

            /* Check if the driver gave us any hints. */
            if(wrq.u.data.length > buflen)
                buflen = wrq.u.data.length;
            else
                buflen *= 2;

            /* Try again */
            goto realloc;
        }
        
        /* Check if results not available yet */
        if(errno == EAGAIN){
            free(buffer);
            /* Wait for only 100ms from now on */
            return(100);	/* Wait 100 ms */
        }
        
        free(buffer);
        /* Bad error, please don't come back... */
        return(-1);
    }

    /* We have the results, process them */
    if(wrq.u.data.length)
    {
        struct iw_event iwe;
        struct stream_descr	stream;
        struct wireless_scan *wscan = NULL;
        int	ret;
#ifdef DEBUG
        /* Debugging code. In theory useless, because it's debugged ;-) */
        int	i;
        IWCFG_DEBUG("Scan result [%02X", buffer[0]);
        for(i = 1; i < wrq.u.data.length; i++)
            IWCFG_DEBUG(":%02X", buffer[i]);
        IWCFG_DEBUG("]\n");
#endif

        /* Init */
        iw_init_event_stream(&stream, (char *) buffer, wrq.u.data.length);
        /* This is dangerous, we may leak user data... */
        context->result = NULL;

        /* Look every token */
        do
        {
            /* Extract an event and print it */
            ret = iw_extract_event_stream(&stream, &iwe, we_version);
            if(ret > 0)
            {
                /* Convert to wireless_scan struct */
                wscan = iw_process_scanning_token(&iwe, wscan);
                /* Check problems */
                if(wscan == NULL)
                {
                    free(buffer);
                    errno = ENOMEM;
                    return(-1);
                }
                /* Save head of list */
                if(context->result == NULL)
                    context->result = wscan;
            }
        }while(ret > 0);
    }else{
        IWCFG_DEBUG("IW scanning has no result \n");
        free(buffer);
        return -1; //as fail
    }

    /* Done with this interface - return success */
    free(buffer);
    return(0);
}

/*------------------------------------------------------------------*/
/*
 * Perform a wireless scan on the specified interface.
 * This is a blocking procedure and it will when the scan is completed
 * or when an error occur.
 *
 * The scan results are given in a linked list of wireless_scan objects.
 * The caller *must* free the result himself (by walking the list).
 * If there is an error, -1 is returned and the error code is available
 * in errno.
 *
 * The parameter we_version can be extracted from the range structure
 * (range.we_version_compiled - see iw_get_range_info()), or using
 * iw_get_kernel_we_version(). For performance reason, you should
 * cache this parameter when possible rather than querying it every time.
 *
 * Return -1 for error and 0 for success.
 */

/*
    scanning wireless network
    implement in iwlist
*/
int IWCFG_ScanningIf(int skfd, char *ifname, int we_version, wireless_scan_head *context)
{
    int	delay;		/* in ms */

    /* Clean up context. Potential memory leak if(context.result != NULL) */
    context->result = NULL;
    context->retry = 0;
    
    /* Wait until we get results or error */
    while(1)
    {
        /* Try to get scan results */
        delay = iw_process_scan(skfd, ifname, we_version, context);

        /* Check termination */
        if(delay <= 0)
            break;

        /* Wait a bit */
        usleep(delay * 1000);
    }
    
    /* End - return -1 or 0 */
    return(delay);
}

/*
    rename if name
*/


/*
    dhcp ip
*/

