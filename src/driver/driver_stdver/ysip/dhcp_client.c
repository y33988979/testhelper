/*************************************************************************
* Copyright(c) 2009, xxxcorporation
* All rights reserved

* dhcp_client.c : dhcp client c file

* version: 	1.0
* author: 	ywg
* time:	2009-02-07

* history:	none
**************************************************************************/

#include "dhcp_config.h"
#include "dhcp_type.h"
#include "dhcp_error.h"
#include "dhcp_client.h"
#include "dhcp_system.h"
#include "ip_cfg.h"

#include "drv_os.h"


struct dhcp_opt60
{
    int our_len;
    int you_len;
    char our_optbuf[32];
    char you_optbuf[32];
};

//#define DHCP_DEBUG   printf
//#define DHCP_VENDOR_CLASS_ID_OPTION_NEEDED  1
#define DHCP_VENDOR_SPECIFIC_INFO_OPTION_NEEDED 1
//#define DHCP_DO_ARP_CHECK   1

static int DHCP_VENDOR_CLASS_ID_OPTION_NEEDED=0;
static int DHCP_OUR_OPTION60_NEEDED=0;
static int DHCP_YOU_OPTION60_NEEDED=0;
static int DHCP_DO_ARP_CHECK=0;

static u32_t xid = 0xABCD0000;
static int state;
static int dhcp_timing;
static int dhcp_time_sw=0;
static int dhcp_time_bisc = 500; /* dhcp timer 500us */

static char current_if[16];

static struct dhcp_info dhcp_desp;
static struct dhcp_opt60 dhcp_opt60;

static char * dhcp_msg_queue;
static pthread_mutex_t dhcp_locker;
static DRVOS_Sem_t *dhcp_sem;

#if defined(__LINUX_SYS__)

static u32_t msg_project_id=2;
static DRVOS_MsgQueue_p msg_queue_id;

#endif

void GetDHCP(struct dhcp_msg *hdr,void *p, int optionlen)
{
    int i;
    u8_t * tmpayload;
    UIN16BIT temp;
    UINT32BIT temp32;
    
    //memset(hdr,0, DHCP_MSG_LEN+4);
    memset(hdr,0, sizeof(struct dhcp_msg));
    tmpayload = (u8_t*)p;
    
    hdr->op = *(tmpayload);
    hdr->htype = *(tmpayload + 1);
    hdr->hlen = *(tmpayload + 2);
    hdr->hops = *(tmpayload + 3);
    
    temp32.byte.ucByte0 = *(tmpayload+4);
    temp32.byte.ucByte1 = *(tmpayload+5);
    temp32.byte.ucByte2 = *(tmpayload+6);
    temp32.byte.ucByte3 = *(tmpayload+7);
    hdr->xid = temp32.uiWord32;
    
    temp.byte.ucByte0 = *(tmpayload+8);
    temp.byte.ucByte1 = *(tmpayload+9);
    hdr->secs = temp.uiWord16;
    
    temp.byte.ucByte0 = *(tmpayload+10);
    temp.byte.ucByte1 = *(tmpayload+11);
    hdr->flags = temp.uiWord16;
    
    temp32.byte.ucByte0 = *(tmpayload+12);
    temp32.byte.ucByte1 = *(tmpayload+13);
    temp32.byte.ucByte2 = *(tmpayload+14);
    temp32.byte.ucByte3 = *(tmpayload+15);
    hdr->ciaddr.addr = temp32.uiWord32;
    
    temp32.byte.ucByte0 = *(tmpayload+16);
    temp32.byte.ucByte1 = *(tmpayload+17);
    temp32.byte.ucByte2 = *(tmpayload+18);
    temp32.byte.ucByte3 = *(tmpayload+19);
    hdr->yiaddr.addr = temp32.uiWord32;
    
    temp32.byte.ucByte0 = *(tmpayload+20);
    temp32.byte.ucByte1 = *(tmpayload+21);
    temp32.byte.ucByte2 = *(tmpayload+22);
    temp32.byte.ucByte3 = *(tmpayload+23);
    hdr->siaddr.addr = temp32.uiWord32;
    
    temp32.byte.ucByte0 = *(tmpayload+24);
    temp32.byte.ucByte1 = *(tmpayload+25);
    temp32.byte.ucByte2 = *(tmpayload+26);
    temp32.byte.ucByte3 = *(tmpayload+27);
    hdr->giaddr.addr = temp32.uiWord32;
    
    memcpy(hdr->chaddr, tmpayload + 28,  DHCP_CHADDR_LEN);
    memcpy(hdr->sname, tmpayload + 28+DHCP_CHADDR_LEN,  DHCP_SNAME_LEN);
    memcpy(hdr->file, tmpayload + 28+DHCP_CHADDR_LEN+DHCP_SNAME_LEN,  DHCP_FILE_LEN);
    
    temp32.byte.ucByte0 = *(tmpayload+236);
    temp32.byte.ucByte1 = *(tmpayload+237);
    temp32.byte.ucByte2 = *(tmpayload+238);
    temp32.byte.ucByte3 = *(tmpayload+239);
    hdr->cookie = temp32.uiWord32;
    
    /* copy options */
    for(i=0; i<optionlen; i++){
        if(i > DHCP_OPTIONS_LEN){
            DHCP_DEBUG("GetDHCP : copy options error max_len=%d, fact_len=%d\n", DHCP_OPTIONS_LEN, optionlen);
        }
        hdr->options[i] = *(tmpayload + 240 + i);
    }
}

void SetDHCP(struct dhcp_msg *hdr,void *p,int optionlen)
{
    int i;
    u8_t * tmpayload;
    UIN16BIT temp;
    UINT32BIT temp32;
    
    tmpayload = (u8_t*)p;
    
    *(tmpayload) = hdr->op;
    *(tmpayload + 1) = hdr->htype;
    *(tmpayload + 2) = hdr->hlen;
    *(tmpayload + 3) = hdr->hops;
    
    temp32.uiWord32 =  hdr->xid;
    *(tmpayload+4) = temp32.byte.ucByte0;
    *(tmpayload+5) = temp32.byte.ucByte1;
    *(tmpayload+6) = temp32.byte.ucByte2;
    *(tmpayload+7) = temp32.byte.ucByte3;
    
    temp.uiWord16 = hdr->secs;
    *(tmpayload+8) = temp.byte.ucByte0;
    *(tmpayload+9) = temp.byte.ucByte1;
    
    temp.uiWord16 = hdr->flags;
    *(tmpayload+10) = temp.byte.ucByte0;
    *(tmpayload+11) = temp.byte.ucByte1;
    
    temp32.uiWord32 =  hdr->ciaddr.addr;
    *(tmpayload+12) = temp32.byte.ucByte0;
    *(tmpayload+13) = temp32.byte.ucByte1;
    *(tmpayload+14) = temp32.byte.ucByte2;
    *(tmpayload+15) = temp32.byte.ucByte3;
    
    temp32.uiWord32 =  hdr->yiaddr.addr;
    *(tmpayload+16) = temp32.byte.ucByte0;
    *(tmpayload+17) = temp32.byte.ucByte1;
    *(tmpayload+18) = temp32.byte.ucByte2;
    *(tmpayload+19) = temp32.byte.ucByte3;
    
    temp32.uiWord32 =  hdr->siaddr.addr;
    *(tmpayload+20) = temp32.byte.ucByte0;
    *(tmpayload+21) = temp32.byte.ucByte1;
    *(tmpayload+22) = temp32.byte.ucByte2;
    *(tmpayload+23) = temp32.byte.ucByte3;
    
    temp32.uiWord32 =  hdr->giaddr.addr;
    *(tmpayload+24) = temp32.byte.ucByte0;
    *(tmpayload+25) = temp32.byte.ucByte1;
    *(tmpayload+26) = temp32.byte.ucByte2;
    *(tmpayload+27) = temp32.byte.ucByte3;
    
    memcpy(tmpayload + 28, hdr->chaddr,  DHCP_CHADDR_LEN);
    memcpy(tmpayload + 28+DHCP_CHADDR_LEN, hdr->sname,  DHCP_SNAME_LEN);
    memcpy(tmpayload + 28+DHCP_CHADDR_LEN+DHCP_SNAME_LEN, hdr->file,  DHCP_FILE_LEN);
    
    temp32.uiWord32 =  hdr->cookie;
    *(tmpayload+236) = temp32.byte.ucByte0;
    *(tmpayload+237) = temp32.byte.ucByte1;
    *(tmpayload+238) = temp32.byte.ucByte2;
    *(tmpayload+239) = temp32.byte.ucByte3;
    
    /* copy options */
    for(i=0; i<optionlen; i++){
        if(i > DHCP_OPTIONS_LEN){
            DHCP_DEBUG("SetDHCP : copy options error max_len=%d, fact_len=%d\n", DHCP_OPTIONS_LEN, optionlen);
        }
        *(tmpayload + 240 + i) = hdr->options[i];
    }
}

void dhcp_option_cfg()
{
    /* process all avaliable options */
}

/*
 * Concatenate an option type and length field to the outgoing
 * DHCP message.
 *
 */
static void dhcp_option(struct dhcp_info *dhcp, u8_t option_type, u8_t option_len)
{
    dhcp->msg_out->options[dhcp->options_out_len++] = option_type;
    dhcp->msg_out->options[dhcp->options_out_len++] = option_len;
}

/*
 * Concatenate a single byte to the outgoing DHCP message.
 *
 */
static void dhcp_option_byte(struct dhcp_info *dhcp, u8_t value)
{
    dhcp->msg_out->options[dhcp->options_out_len++] = value;
}

static void dhcp_option_short(struct dhcp_info *dhcp, u16_t value)
{
    dhcp->msg_out->options[dhcp->options_out_len++] = (value & 0xff00U) >> 8;
    dhcp->msg_out->options[dhcp->options_out_len++] =  value & 0x00ffU;
}

static void dhcp_option_long(struct dhcp_info *dhcp, u32_t value)
{
    dhcp->msg_out->options[dhcp->options_out_len++] = (value & 0xff000000UL) >> 24;
    dhcp->msg_out->options[dhcp->options_out_len++] = (value & 0x00ff0000UL) >> 16;
    dhcp->msg_out->options[dhcp->options_out_len++] = (value & 0x0000ff00UL) >> 8;
    dhcp->msg_out->options[dhcp->options_out_len++] = (value & 0x000000ffUL);
}

/**
 * Add a DHCP message trailer
 *
 * Adds the END option to the DHCP message, and if
 * necessary, up to three padding bytes.
 */
static void dhcp_option_trailer(struct dhcp_info *dhcp)
{
    dhcp->msg_out->options[dhcp->options_out_len++] = DHCP_OPTION_END;
    /* packet is too small, or not 4 byte aligned? */
    while ((dhcp->options_out_len < DHCP_MIN_OPTIONS_LEN) || (dhcp->options_out_len & 3)) {
        /* LWIP_DEBUGF(DHCP_DEBUG,("dhcp_option_trailer:dhcp->options_out_len=%"U16_F", DHCP_OPTIONS_LEN=%"U16_F, dhcp->options_out_len, DHCP_OPTIONS_LEN)); */
        /* add a fill/padding byte */
        dhcp->msg_out->options[dhcp->options_out_len++] = 0;
    }
}

/**
 * Find the offset of a DHCP option inside the DHCP message.
 *
 * @param client DHCP client
 * @param option_type
 *
 * @return a byte offset into the UDP message where the option was found, or
 * zero if the given option was not found.
 */
static u8_t *dhcp_get_option_ptr(struct dhcp_info *dhcp, u8_t option_type)
{
    u8_t overload = DHCP_OVERLOAD_NONE;

    /* options available? */
    if ((dhcp->msg_in->options != NULL) && (dhcp->options_in_len > 0)) {
        /* start with options field */
        u8_t *options = (u8_t *)dhcp->msg_in->options;
        u16_t offset = 0;
        /* at least 1 byte to read and no end marker, then at least 3 bytes to read? */
        while ((offset < dhcp->options_in_len) && (options[offset] != DHCP_OPTION_END)) {
            /* LWIP_DEBUGF(DHCP_DEBUG, ("msg_offset=%"U16_F", q->len=%"U16_F, msg_offset, q->len)); */
            /* are the sname and/or file field overloaded with options? */
            if (options[offset] == DHCP_OPTION_OVERLOAD) {
                DHCP_DEBUG("overloaded message detected\n");
                /* skip option type and length */
                offset += 2;
                overload = options[offset++];
            }
            /* requested option found */
            else if (options[offset] == option_type) {
                //DHCP_DEBUG("option found at offset %lx in options\n", offset);
                return &options[offset];
            /* skip option */
            } else {
                //DHCP_DEBUG("skipping option %lx in options\n", options[offset]);
                /* skip option type */
                offset++;
                /* skip option length, and then length bytes */
                offset += 1 + options[offset];
            }
        }
        /* is this an overloaded message? */
        if (overload != DHCP_OVERLOAD_NONE) {
            u16_t field_len;
            if (overload == DHCP_OVERLOAD_FILE) {
                DHCP_DEBUG("overloaded file field\n");
                options = (u8_t *)dhcp->msg_in->file;
                field_len = DHCP_FILE_LEN;
            } else if (overload == DHCP_OVERLOAD_SNAME) {
                DHCP_DEBUG("overloaded sname field\n");
                options = (u8_t *)dhcp->msg_in->sname;
                field_len = DHCP_SNAME_LEN;
            /* TODO: check if else if () is necessary */
            } else {
                DHCP_DEBUG("overloaded sname and file field\n");
                options = (u8_t *)dhcp->msg_in->sname;
                field_len = DHCP_FILE_LEN + DHCP_SNAME_LEN;
                DHCP_DEBUG("overloaded sname %d AND %d field at %p\n",DHCP_FILE_LEN, DHCP_SNAME_LEN, options );
            }
            offset = 0;
            #if 0
            /* at least 1 byte to read and no end marker */
            while ((offset < field_len) && (options[offset] != DHCP_OPTION_END)) {
                if (options[offset] == option_type) {
                    DHCP_DEBUG("option found at offset=%lx \n", offset);
                    return &options[offset];
                /* skip option */
                } else {
                    DHCP_DEBUG("skipping option %lx \n", options[offset]);
                    /* skip option type */
                    offset++;
                    offset += 1 + options[offset];
                }
                DHCP_DEBUG("skipping option %lx \n", options[offset]);
            }
            #endif
        }
    }
    return 0;
}

/**
 * Return the byte of DHCP option data.
 *
 * @param client DHCP client.
 * @param ptr pointer obtained by dhcp_get_option_ptr().
 *
 * @return byte value at the given address.
 */
static u8_t dhcp_get_option_byte(u8_t *ptr)
{
    //DHCP_DEBUG("option byte value=%lx \n", (u16_t)(*ptr));
    return *ptr;
}

/**
 * Return the 16-bit value of DHCP option data.
 *
 * @param client DHCP client.
 * @param ptr pointer obtained by dhcp_get_option_ptr().
 *
 * @return byte value at the given address.
 */
static u16_t dhcp_get_option_short(u8_t *ptr)
{
    u16_t value;
    value = *ptr++ << 8;
    value |= *ptr;
    //DHCP_DEBUG("option short value=%lx \n", value);
    return value;
}

/**
 * Return the 32-bit value of DHCP option data.
 *
 * @param client DHCP client.
 * @param ptr pointer obtained by dhcp_get_option_ptr().
 *
 * @return byte value at the given address.
 */
static u32_t dhcp_get_option_long(u8_t *ptr)
{
    u32_t value;
    value = (u32_t)(*ptr++) << 24;
    value |= (u32_t)(*ptr++) << 16;
    value |= (u32_t)(*ptr++) << 8;
    value |= (u32_t)(*ptr++);
    //DHCP_DEBUG("option long value=%lx \n", value);
    return value;
}

dhcp_err_t dhcp_create_packet(struct dhcp_info *dhcp)
{
    /* create request */
    int i;
    
    dhcp->msg_out = (struct dhcp_msg *)dhcp_mem_malloc(sizeof(struct dhcp_msg));
    if(dhcp->msg_out == NULL){
        /* mem malloc fail */
        DHCP_DEBUG("mem malloc error when creating packet!\n");
        return 0;
    }
    //DHCP_DEBUG("*** mem malloc %lp \n", dhcp->msg_out);
    
    dhcp->xid = xid++;
    
    dhcp->msg_out->op = DHCP_BOOTREQUEST;
    dhcp->msg_out->htype = DHCP_HTYPE_ETH;
    /* TODO: make link layer independent */
    dhcp->msg_out->hlen = DHCP_HLEN_ETH;
    dhcp->msg_out->hops = 0;
    dhcp->msg_out->xid = ipcfg_htonl(dhcp->xid);
    dhcp->msg_out->secs = 0;
    dhcp->msg_out->flags = 0;
    //dhcp->msg_out->ciaddr.addr = IPCFG_GetIpAddr("eth0");
    dhcp->msg_out->ciaddr.addr = INADDR_ANY;//STBIp_Get();
    dhcp->msg_out->yiaddr.addr = 0;
    dhcp->msg_out->siaddr.addr = 0;
    dhcp->msg_out->giaddr.addr = 0;
    for (i = 0; i < DHCP_CHADDR_LEN; i++) {
        /* copy netif hardware address, pad with zeroes */
        dhcp->msg_out->chaddr[i] = 0/* pad byte*/;
    }
    IPCFG_GetMacAddr(current_if,dhcp->msg_out->chaddr);
    for (i = 0; i < DHCP_SNAME_LEN; i++) dhcp->msg_out->sname[i] = 0;
    for (i = 0; i < DHCP_FILE_LEN; i++) dhcp->msg_out->file[i] = 0;
    dhcp->msg_out->cookie = ipcfg_htonl(0x63825363UL);
    dhcp->options_out_len = 0;
    /* fill options field with an incrementing array (for debugging purposes) */
    for (i = 0; i < DHCP_OPTIONS_LEN; i++) dhcp->msg_out->options[i] = i;
    
    return DHCP_ERR_OK;
    
}

static void dhcp_delete_packet(struct dhcp_info *dhcp)
{
    dhcp_mem_free((void *)dhcp->msg_out);
    dhcp->msg_out = NULL;
}

void dhcp_parse_packet(struct dhcp_info *dhcp, char *buf, int len)
{
    if(len < DHCP_MSG_LEN)
    {
        DHCP_DEBUG("too small packet \n");
        return;
    }
    
    dhcp->options_in_len = len - DHCP_MSG_LEN - 4;
    GetDHCP(dhcp->msg_in, (void *)buf, dhcp->options_in_len/*len-DHCP_MSG_LEN*/);
    dhcp->msg_in->xid = ipcfg_htonl(dhcp->msg_in->xid);
    dhcp->msg_in->cookie = ipcfg_htonl(dhcp->msg_in->cookie);
    if(DHCP_VENDOR_CLASS_ID_OPTION_NEEDED){
        //yeah, we have got it!!!
        //dhcp->msg_in->sname
    }
}

int dhcp_send_packet(struct dhcp_info *dhcp)
{
    /* create socket buffer */
    int len;
    int ret;
    char sendbuf[512];
    struct sockaddr_in remote_addr;
    
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = ipcfg_htonl(0xffffffff);
    remote_addr.sin_port = ipcfg_htons(DHCP_SERVER_PORT);
    
    /* copy msg to buffer */
    len = DHCP_MSG_LEN + 4 + dhcp->options_out_len;
    //DHCP_DEBUG("socket is %d len is %d + %d \n",dhcp->sockfd, DHCP_MSG_LEN, dhcp->options_out_len);
    if(len > 1500){
        DHCP_DEBUG("dhcp msg length error \n");
    }
    SetDHCP(dhcp->msg_out, (void *)sendbuf, dhcp->options_out_len);
    
    /* send msg */
#if RAW_SOCKET_MODE
    ret = dhcp_send_raw_packet(sendbuf, len, INADDR_ANY, DHCP_CLIENT_PORT, 
                               INADDR_BROADCAST, DHCP_SERVER_PORT, MAC_BCAST_ADDR, dhcp->ifindex);
#else
    ret = dhcp_socket_send(dhcp->sockfd, sendbuf, len, 0, (struct sockaddr *)&remote_addr,sizeof(remote_addr));
#endif
    if(ret < 0)
    {
        DHCP_DEBUG("send packet error !\n");
        return 0;
    }
    return ret;
}

dhcp_err_t dhcp_discover(struct dhcp_info *dhcp)
{
    /* send discovery */
    dhcp_err_t result = DHCP_ERR_OK;
    
    DHCP_DEBUG("dhcp_discover()\n");
    
    //dhcp_ip_addr_set(&dhcp->offered_ip_addr, IP_ADDR_ANY);
    dhcp->offered_ip_addr.addr = 0x00000000UL;
    
    /* create and initialize the DHCP message header */
    result = dhcp_create_packet(dhcp);
    if (result == DHCP_ERR_OK)
    {
        //DHCP_DEBUG("dhcp_discover: making request\n");
        /* dhcp message type */
        dhcp_option(dhcp, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN);
        dhcp_option_byte(dhcp, DHCP_DISCOVER);
        /* dhcp option max msg size */
        dhcp_option(dhcp, DHCP_OPTION_MAX_MSG_SIZE, DHCP_OPTION_MAX_MSG_SIZE_LEN);
        dhcp_option_short(dhcp, 576);
        
        /* if need follows ? */
        //dhcp_option_byte(dhcp, DHCP_OPTION_VENDOR_CLASS_ID);
        
        /* dhcp option parameter request list */
        dhcp_option(dhcp, DHCP_OPTION_PARAMETER_REQUEST_LIST, 6/*num options*/);
        dhcp_option_byte(dhcp, DHCP_OPTION_SUBNET_MASK);
        dhcp_option_byte(dhcp, DHCP_OPTION_ROUTER);
        dhcp_option_byte(dhcp, DHCP_OPTION_BROADCAST);
        dhcp_option_byte(dhcp, DHCP_OPTION_DNS_SERVER);
        dhcp_option_byte(dhcp, DHCP_OPTION_SERVER_ID); 
        dhcp_option_byte(dhcp, DHCP_OPTION_VENDOR_CLASS_ID);

        /* if need other options as follows */
        if(DHCP_OUR_OPTION60_NEEDED){
            /* add vendor class id */
            int i;
            dhcp_option(dhcp, DHCP_OPTION_VENDOR_CLASS_ID,dhcp_opt60.our_len);
            for(i=0; i<dhcp_opt60.our_len; i++){
                dhcp_option_byte(dhcp, dhcp_opt60.our_optbuf[i]);
            }
        }
#if DHCP_VENDOR_SPECIFIC_INFO_OPTION_NEEDED
        /* add vendor specific information */
        dhcp_option(dhcp, DHCP_OPTION_VENDOR_SPECIFIC_INFO,2);
        dhcp_option_byte(dhcp, 'J');
        dhcp_option_byte(dhcp, 'Z');
#endif
        dhcp_option_trailer(dhcp);
        
        //DHCP_DEBUG("dhcp_discover: realloc()ing\n");
        //dhcp_mem_realloc(dhcp->msg_out, sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN + dhcp->options_out_len);
        
        /* send discover */
        dhcp_send_packet(dhcp);
        
        /* release sources */
        //DHCP_DEBUG("dhcp_discover: deleting()ing\n");
        dhcp_delete_packet(dhcp);
    } else {
        DHCP_DEBUG("dhcp_discover: could not allocate DHCP request\n");
    }
    
    return result;
}

dhcp_err_t dhcp_request(struct dhcp_info *dhcp)
{
    /* send request */
    dhcp_err_t result;
    
    //DHCP_DEBUG("dhcp_request\n");
    
    /* create and initialize the DHCP message header */
    result = dhcp_create_packet(dhcp);
    if (result == DHCP_ERR_OK)
    {
        dhcp_option(dhcp, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN);
        dhcp_option_byte(dhcp, DHCP_REQUEST);
        
        dhcp_option(dhcp, DHCP_OPTION_MAX_MSG_SIZE, DHCP_OPTION_MAX_MSG_SIZE_LEN);
        dhcp_option_short(dhcp, 576);
        
        /* MUST request the offered IP address */
        dhcp_option(dhcp, DHCP_OPTION_REQUESTED_IP, 4);
        dhcp_option_long(dhcp, ipcfg_ntohl(dhcp->offered_ip_addr.addr));
        
        dhcp_option(dhcp, DHCP_OPTION_SERVER_ID, 4);
        dhcp_option_long(dhcp, ipcfg_ntohl(dhcp->server_ip_addr.addr));
        
        /* if need ? */
        //dhcp_option_byte(dhcp, DHCP_OPTION_VENDOR_CLASS_ID);
        
        dhcp_option(dhcp, DHCP_OPTION_PARAMETER_REQUEST_LIST, 6/*num options*/);
        dhcp_option_byte(dhcp, DHCP_OPTION_SUBNET_MASK);
        dhcp_option_byte(dhcp, DHCP_OPTION_ROUTER);
        dhcp_option_byte(dhcp, DHCP_OPTION_BROADCAST);
        dhcp_option_byte(dhcp, DHCP_OPTION_DNS_SERVER);
        dhcp_option_byte(dhcp, DHCP_OPTION_SERVER_ID); 
        dhcp_option_byte(dhcp, DHCP_OPTION_VENDOR_CLASS_ID);

        if(DHCP_OUR_OPTION60_NEEDED){
            /* add vendor class id */
            int i;
            dhcp_option(dhcp, DHCP_OPTION_VENDOR_CLASS_ID,dhcp_opt60.our_len);
            for(i=0; i<dhcp_opt60.our_len; i++){
                dhcp_option_byte(dhcp, dhcp_opt60.our_optbuf[i]);
            }
        }
#if DHCP_VENDOR_SPECIFIC_INFO_OPTION_NEEDED
        /* add vendor specific information */
        dhcp_option(dhcp, DHCP_OPTION_VENDOR_SPECIFIC_INFO,2);
        dhcp_option_byte(dhcp, 'J');
        dhcp_option_byte(dhcp, 'Z');
#endif
        dhcp_option_trailer(dhcp);
        /* shrink the pbuf to the actual content length */
        //dhcp_mem_realloc(dhcp->msg_out, sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN + dhcp->options_out_len);
        
        /* send request */
        dhcp_send_packet(dhcp);

        /* release sources */
        //DHCP_DEBUG("dhcp_request: deleting()ing\n");
        dhcp_delete_packet(dhcp);
        //DHCP_DEBUG("dhcp_request: REQUESTING\n");
    } else {
        DHCP_DEBUG("dhcp_request: could not allocate DHCP request\n");
    }
    
    return result;
}

/**
 * Decline an offered lease.
 *
 * Tell the DHCP server we do not accept the offered address.
 * One reason to decline the lease is when we find out the address
 * is already in use by another host (through ARP).
 */
dhcp_err_t dhcp_decline(struct dhcp_info *dhcp)
{
    /* send dhcp decline msg */
    dhcp_err_t result = 0;
    
    DHCP_DEBUG("dhcp_decline()\n");
    
    //dhcp_set_state(dhcp, DHCP_BACKING_OFF);
    
    /* create and initialize the DHCP message header */
    result = dhcp_create_packet(dhcp);
    if (result == DHCP_ERR_OK)
    {
        dhcp_option(dhcp, DHCP_OPTION_MESSAGE_TYPE, DHCP_OPTION_MESSAGE_TYPE_LEN);
        dhcp_option_byte(dhcp, DHCP_DECLINE);
        
        dhcp_option(dhcp, DHCP_OPTION_MAX_MSG_SIZE, DHCP_OPTION_MAX_MSG_SIZE_LEN);
        dhcp_option_short(dhcp, 576);
        
        dhcp_option(dhcp, DHCP_OPTION_REQUESTED_IP, 4);
        dhcp_option_long(dhcp, ipcfg_ntohl(dhcp->offered_ip_addr.addr));
        
        dhcp_option_trailer(dhcp);
        
        /* resize pbuf to reflect true size of options */
        //dhcp_mem_realloc(dhcp->msg_out, sizeof(struct dhcp_msg) - DHCP_OPTIONS_LEN + dhcp->options_out_len);

        /* send decline packet */
        dhcp_send_packet(dhcp);

        /* release sources */
        dhcp_delete_packet(dhcp);
        //DHCP_DEBUG("dhcp_decline: BACKING OFF\n");
    } else {
        DHCP_DEBUG("dhcp_decline: could not allocate DHCP request\n");
    }
    //DHCP_DEBUG("dhcp_decline()\n");
    return result;
}

int dhcp_arp_check(struct dhcp_info *dhcp)
{
    /* send raw arp packet when receive ack */
    /* may be we can use ping instead */
    if(DHCP_DO_ARP_CHECK)
    {
        int ret;
        u32_t yiaddr;
        u32_t ip;
        u8_t mac[6];
        
        yiaddr = dhcp->offered_ip_addr.addr;
        ip = INADDR_ANY;
        IPCFG_GetMacAddr(current_if, mac);
        
        ret = arpping(yiaddr, ip, mac, current_if);
        if(ret > 0){
            return ret; /* free ip can be used in our system */
        }else{
            return 0;   /* error or used */
        }
    }
    else{
        return 1;       /* free ip can be used in our system */
    }
}

dhcp_app_callback dhcp_ok_callback;
dhcp_app_callback dhcp_fail_callback;

void register_dhcpok_callback(dhcp_app_callback callback)
{
    dhcp_ok_callback = callback;
}

void unregister_dhcpok_callback()
{
    dhcp_ok_callback = NULL;
}

void register_dhcpfail_callback(dhcp_app_callback callback)
{
    dhcp_fail_callback = callback;
}

void unregister_dhcpfail_callback()
{
    dhcp_fail_callback = NULL;
}

void dhcp_client_callback(CALLBACK_TYPE_T msg_type, struct dhcp_info *dhcp)
{
    switch(msg_type)
    {
        case DHCP_DISCOVER_FAIL:
            DHCP_DEBUG("DHCP_DISCOVER_FAIL\n");
            break;
        case DHCP_REQUEST_FAIL:
            DHCP_DEBUG("DHCP_REQUEST_FAIL\n");
            break;
        case DHCP_REBINDING_TIMEOUT:
            DHCP_DEBUG("DHCP_REBINDING_TIMEOUT\n");
            break;
        case DHCP_BOUND_OK:
            {
                OFFERED_INFO_T info;
                memset(&info, 0, sizeof(struct offered_info));
                info.offered_ip = dhcp->offered_ip_addr;
                info.offered_gw = dhcp->offered_gw_addr;
                info.offered_sn = dhcp->offered_sn_mask;
                info.offered_dns[0] = dhcp->offered_dns_addr[0];
		  info.offered_dns[1] = dhcp->offered_dns_addr[1];
                info.offered_timeout = dhcp->offered_t0_lease;
                /**/
                if(dhcp_ok_callback != NULL){
                    dhcp_ok_callback(info);
                }else{
                    IPCFG_SetIpAddr(current_if, info.offered_ip.addr);
                    IPCFG_SetSubMask(current_if, info.offered_sn.addr);
                    IPCFG_SetGateWay(current_if, info.offered_gw.addr);
                    //IPCFG_SetDns(info.offered_dns[0].addr);
		      IPCFG_SetDnsEx(info.offered_dns[0].addr, info.offered_dns[1].addr);
                    
                }
		  DHCP_DEBUG("DHCP get ip %lx \n", info.offered_ip.addr);
	         DHCP_DEBUG("DHCP get sm %lx \n", info.offered_sn.addr);
	         DHCP_DEBUG("DHCP get gw %lx \n", info.offered_gw.addr);
	         DHCP_DEBUG("DHCP get dns0 %lx \n", info.offered_dns[0].addr);
		  DHCP_DEBUG("DHCP get dns1 %lx \n", info.offered_dns[1].addr);
		  DHCP_DEBUG("DHCP get timeout  %ld \n", info.offered_timeout);
            }
            DHCP_DEBUG("DHCP_BOUND_OK\n");
	     
            break;
        case DHCP_BOUND_FAIL:
            DHCP_DEBUG("DHCP_BOUND_FAIL\n");
            break;
        case DHCP_RENEWING_OK:
            DHCP_DEBUG("DHCP_RENEWING_OK\n");
            {
                OFFERED_INFO_T info;
                memset(&info, 0, sizeof(struct offered_info));
                info.offered_ip = dhcp->offered_ip_addr;
                info.offered_gw = dhcp->offered_gw_addr;
                info.offered_sn = dhcp->offered_sn_mask;
                info.offered_dns[0] = dhcp->offered_dns_addr[0];
		  info.offered_dns[1] = dhcp->offered_dns_addr[1];
                info.offered_timeout = dhcp->offered_t0_lease;
                /**/
                if(dhcp_ok_callback != NULL){
                    dhcp_ok_callback(info);
                }else{
                    IPCFG_SetIpAddr(current_if, info.offered_ip.addr);
                    IPCFG_SetSubMask(current_if, info.offered_sn.addr);
                    IPCFG_SetGateWay(current_if, info.offered_gw.addr);
                    //IPCFG_SetDns(info.offered_dns[0].addr);
		      IPCFG_SetDnsEx(info.offered_dns[0].addr, info.offered_dns[1].addr);
                }
		  DHCP_DEBUG("DHCP get ip %lx \n", info.offered_ip.addr);
	         DHCP_DEBUG("DHCP get sm %lx \n", info.offered_sn.addr);
	         DHCP_DEBUG("DHCP get gw %lx \n", info.offered_gw.addr);
	         DHCP_DEBUG("DHCP get dns0 %lx \n", info.offered_dns[0].addr);
		  DHCP_DEBUG("DHCP get dns1 %lx \n", info.offered_dns[1].addr);
		  DHCP_DEBUG("DHCP get timeout  %ld \n", info.offered_timeout);
            }
            break;
        case DHCP_REBINDING_OK:
            DHCP_DEBUG("DHCP_REBINDING_OK\n");
            {
                OFFERED_INFO_T info;
                memset(&info, 0, sizeof(struct offered_info));
                info.offered_ip = dhcp->offered_ip_addr;
                info.offered_gw = dhcp->offered_gw_addr;
                info.offered_sn = dhcp->offered_sn_mask;
                info.offered_dns[0] = dhcp->offered_dns_addr[0];
		  info.offered_dns[1] = dhcp->offered_dns_addr[1];
                info.offered_timeout = dhcp->offered_t0_lease;
                /**/
                if(dhcp_ok_callback != NULL){
                    dhcp_ok_callback(info);
                }else{
                    IPCFG_SetIpAddr(current_if, info.offered_ip.addr);
                    IPCFG_SetSubMask(current_if, info.offered_sn.addr);
                    IPCFG_SetGateWay(current_if, info.offered_gw.addr);
                    //IPCFG_SetDns(info.offered_dns[0].addr);
		      IPCFG_SetDnsEx(info.offered_dns[0].addr, info.offered_dns[1].addr);
                }
		  DHCP_DEBUG("DHCP get ip %lx \n", info.offered_ip.addr);
	         DHCP_DEBUG("DHCP get sm %lx \n", info.offered_sn.addr);
	         DHCP_DEBUG("DHCP get gw %lx \n", info.offered_gw.addr);
	         DHCP_DEBUG("DHCP get dns0 %lx \n", info.offered_dns[0].addr);
		  DHCP_DEBUG("DHCP get dns1 %lx \n", info.offered_dns[1].addr);
		  DHCP_DEBUG("DHCP get timeout  %ld \n", info.offered_timeout);
            }
            break;
        case DHCP_REQUESTING_NAK:
            DHCP_DEBUG("DHCP_REQUESTING_NAK\n");
            break;
        case DHCP_RENEWING_NAK:
            DHCP_DEBUG("DHCP_RENEWING_NAK\n");
            break;
        case DHCP_REBINDING_NAK:
            DHCP_DEBUG("DHCP_REBINDING_NAK\n");
            break;
        default:
            break;
    }
}

void dhcp_timer(void *arg)
{
    /* send msg timeout */
    /* timeout base on socket and select */
    u8_t t_try;
    int retval;
    fd_set rfds;
    struct timeval tv;

    APP_MSG_T msg;
    
    while(dhcp_timing)
    {
        t_try = 0;
        retval = 0;
        sys_mutex_lock(&dhcp_locker);
        sys_sem_wait(dhcp_sem);
        if((dhcp_time_sw)&&(dhcp_desp.sockfd >= 0)){
            if(dhcp_desp.timeout <= 0){
                dhcp_time_sw = 0;
                sys_sem_signal(dhcp_sem);
                sys_mutex_unlock(&dhcp_locker);
                continue;
            }else if(dhcp_desp.timeout <= dhcp_time_bisc){
                tv.tv_sec = 0;
                tv.tv_usec = (dhcp_desp.timeout)*1000; /* fact */
                t_try = 1; /* last */
            }else{
                tv.tv_sec = 0;
    	        tv.tv_usec = dhcp_time_bisc*1000; /* 500ms */
            }
            
            FD_ZERO(&rfds);
            //FD_ZERO(&wfds);
            FD_SET(dhcp_desp.sockfd, &rfds);
            //FD_SET(dhcp_desp.sockfd, &wfds);
        	if (tv.tv_usec > 0) {
              if(state != DHCP_BOUND){
        	    //DHCP_DEBUG("Waiting on socket %d...select \n",dhcp_desp.sockfd);
        	    retval = dhcp_socket_select(dhcp_desp.sockfd + 1, &rfds, NULL, NULL, &tv);
                //DHCP_DEBUG("ret value is %d \n",retval);
                if((t_try == 1)&&(retval == 0)){
                    dhcp_desp.timeout = 0;
                    msg.type = MSG_TIMEOUT;
                    dhcp_time_sw = 0;
                    sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
                    //DHCP_DEBUG("timeout is zero \n");
                    //continue;
                    sys_sem_signal(dhcp_sem);
                    sys_mutex_unlock(&dhcp_locker);
                    continue;
                }/* if t_try && retval */
              /* state != DHCP_BOUND */
              }else{
                sys_delay((tv.tv_usec/1000));
              }
        	} else {
        	    retval = 0; /* If we already timed out, fall through */
                DHCP_DEBUG("tmo is zero \n");
        	}
        
            /* 超时，根据当前状态执行不同操作 */
        	if ((retval == 0) && (t_try == 0)) {
                /* time also has eslaped 500ms */
                dhcp_desp.timeout -= dhcp_time_bisc;
                //DHCP_DEBUG("timeout - 500ms \n"); 
                sys_sem_signal(dhcp_sem);
                sys_mutex_unlock(&dhcp_locker);
                continue;
            }/* 有数据包到达，可以进行receive相关处理 */
            else if (retval > 0 && FD_ISSET(dhcp_desp.sockfd, &rfds)) {
                //DHCP_DEBUG("ret value is %d will send msg_dhcp\n",retval);
                if(t_try){
                    msg.type = MSG_TIMEOUT;
                    dhcp_desp.timeout = 0;
                    //DHCP_DEBUG("*****timeout is zero \n");
                }else{
                    msg.type = MSG_DHCP;
                }
                dhcp_time_sw = 0;
                sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
                //DHCP_DEBUG("a dhcp msg send \n");
                sys_sem_signal(dhcp_sem);
                sys_mutex_unlock(&dhcp_locker);
                continue;
         }/* lease time over */
         else if((t_try == 1)&&(retval == 0)){
                dhcp_desp.timeout = 0;
                msg.type = MSG_TIMEOUT;
                dhcp_time_sw = 0;
                sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
                //DHCP_DEBUG("bound timeout is zero \n");
                //continue;
                sys_sem_signal(dhcp_sem);
                sys_mutex_unlock(&dhcp_locker);
                continue;
            }/* 出错处理 */
            else {
        	/* An error occured */
        	DHCP_DEBUG("Error on select retval <= -1 \n");
                //msg.type = MSG_STOP;
                //sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
                dhcp_time_sw = 0;
                sys_sem_signal(dhcp_sem);
                sys_mutex_unlock(&dhcp_locker);
                continue;
            }
        }/* if ... */
        //DHCP_DEBUG("dhcp timer --- \n");
        sys_delay(500); /* XXms */
        sys_sem_signal(dhcp_sem);
        sys_mutex_unlock(&dhcp_locker);
    }/* while ... */
    DHCP_DEBUG("dhcp timer stop and go out \n");
    return;
}

void dhcp_stop_timer()
{
    /* stop dhcp timer */
    dhcp_timing = 0;
}

int dhcp_get_offerinfo(struct dhcp_info *dhcp)
{
    /* we recv ack from server */

    u8_t *options_ptr;
    
    /* record lease set t1 t2 timeout */
    /* t1 renew */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_T1);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_T1 option not found\n");
        dhcp->offered_t1_renew = 0;
        //continue;
    }else{
        if ((dhcp->offered_t1_renew = dhcp_get_option_long(options_ptr + 2)) == 0) {
	    DHCP_DEBUG("couldnt get option from packet -- ignoring 3 \n");
	    //continue;
	    return 0;
        }
    }
    #if 0
    if ((dhcp->server_ip_addr.addr = dhcp_get_option_long(options_ptr + 2)) == NULL) {
	    //DHCP_DEBUG("couldnt get option from packet -- ignoring 1 \n");
	    //continue;
    }
    #endif
    /* t2 rebind */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_T2);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_T2 option not found\n");
        dhcp->offered_t2_rebind = 0;
        //continue;
    }else{
        if ((dhcp->offered_t2_rebind = dhcp_get_option_long(options_ptr + 2)) == 0) {
	    DHCP_DEBUG("couldnt get option from packet -- ignoring 3 \n");
	    //continue;
	    return 0;
        }
    }
    #if 0
    if ((dhcp->server_ip_addr.addr = dhcp_get_option_long(options_ptr + 2)) == NULL) {
	    //DHCP_DEBUG("couldnt get option from packet -- ignoring 2 \n");
	    //continue;
    }
    #endif
    /* t1 timeout */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_LEASE_TIME);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_LEASE_TIME option not found\n");
        dhcp->offered_t0_lease = 0;
        //continue;
        return 0;
    }
    if ((dhcp->offered_t0_lease = dhcp_get_option_long(options_ptr + 2)) == 0) {
	    DHCP_DEBUG("couldnt get option from packet -- ignoring 3 \n");
	    //continue;
	    return 0;
    }
    dhcp->t1_timeout = dhcp->offered_t1_renew *1000;
    dhcp->t2_timeout = dhcp->offered_t2_rebind *1000;
    if((dhcp->offered_t1_renew * 1000) > 0){
	  dhcp->timeout = dhcp->offered_t1_renew *1000;
    }else{
    	  dhcp->timeout = (dhcp->offered_t0_lease *1000) / 2;
    }
    
    
    //dhcp_time_sw = 1;
    
    /* offered ip */
    dhcp->offered_ip_addr.addr = ipcfg_htonl(dhcp->msg_in->yiaddr.addr);
    /* offered gw */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_ROUTER);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_ROUTER option not found\n");
        //continue;
        return 0;
    }
    if ((dhcp->offered_gw_addr.addr = dhcp_get_option_long(options_ptr + 2)) == 0) {
	    //DHCP_DEBUG("couldnt get option from packet -- ignoring 4 \n");
	    //continue;
    }
    /* offered mask */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_SUBNET_MASK);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_SUBNET_MASK option not found\n");
        //continue;
        return 0;
    }
    if ((dhcp->offered_sn_mask.addr = dhcp_get_option_long(options_ptr + 2)) == 0) {
	    DHCP_DEBUG("couldnt get option from packet -- ignoring 5 \n");
	    //continue;
	    return 0;
    }
    /* offered dns */
    options_ptr = dhcp_get_option_ptr(dhcp, DHCP_OPTION_DNS_SERVER);
    if(options_ptr == NULL){
        DHCP_DEBUG("DHCP_OPTION_DNS_SERVER option not found\n");
        //continue;
        //return 0;
    }else {
	  u8_t n;
	  dhcp->dns_count = dhcp_get_option_byte(&options_ptr[1]);
	  /* limit to at most DHCP_MAX_DNS DNS servers */
	  //DHCP_DEBUG("[*****dhcp debug] dhcp dns count is %d \n", dhcp->dns_count);
	  dhcp->dns_count = dhcp->dns_count >>2;
	  if (dhcp->dns_count > 2) dhcp->dns_count = 2;
	  for (n = 0; n < dhcp->dns_count; n++)
	  {
	    dhcp->offered_dns_addr[n].addr = dhcp_get_option_long(&options_ptr[2+(n<<2)]);
	  }
    }
    //if ((dhcp->offered_dns_addr[0].addr = dhcp_get_option_long(options_ptr + 2)) == 0) {
   //	    DHCP_DEBUG("couldnt get option from packet -- ignoring 6 \n");
	    //continue;
   //	    return 0;
   //}

    return 1;
}

static unsigned int dhcp_time_now = 0;
void dhcp_mainloop(void *arg /*dhcp_core_callback callback*/)
{ 
    int len;
    char recvbuf[1500];

    u8_t message_type;
    u8_t *options_ptr;
    struct dhcp_msg dhcpmsg;
    dhcp_core_callback callback;
    
    APP_MSG_T msg;

    callback = (dhcp_core_callback)arg;

    int ret;
    struct timeval time_wait;
    time_wait.tv_sec = 0;
    time_wait.tv_usec = 0;
    fd_set fds;
    char tmp[2];

    while(1)
    {
        //DHCP_DEBUG("dhcp mainloop running \n");
        sys_recv_msg(msg_queue_id, &msg, dhcp_msg_queue);
        if(msg.type == MSG_IDLE)
        {
            /* */
            DHCP_DEBUG("idle msg \n");
            continue;
        }
        switch(msg.type)
        {
            case MSG_IDLE:
                /* do nothing */
                DHCP_DEBUG("msg idle \n");
                break;
            case MSG_TIMEOUT:
	     {
		/* timeout dropped to zero */
		switch (state) {
		    case DHCP_INIT1:
		        /* do nothing */
		        DHCP_DEBUG("timeout of dhcp init1 state \n");
		        break;
		    case DHCP_INIT:
		        state = DHCP_SELECTING;
		        /*send discover */
			dhcp_discover(&dhcp_desp);
			/* set timeout and retry times */
			dhcp_desp.tried = 0;
			dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000; /* increase base on 2s */
			dhcp_desp.retries = 5; /* try 10 times */
			dhcp_time_sw = 1;
			break;
		case DHCP_SELECTING:
			/* timeout (discover) */
			if(dhcp_desp.tried < dhcp_desp.retries){
				/* send discover again */
				dhcp_discover(&dhcp_desp);
				/* set timeout and add retry times */
				dhcp_desp.tried++;
				dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000;
				dhcp_time_sw = 1;
				continue;
			}else{
				/* fail goto DHCP_INIT */
				{
		    			dhcp_discover(&dhcp_desp);
		    			dhcp_desp.timeout = 10*1000;
		   	 		dhcp_time_sw = 1;
		    			continue;
				}
				state = DHCP_INIT1;
				dhcp_desp.timeout = 0;
				(callback)(DHCP_DISCOVER_FAIL, &dhcp_desp);
				//continue; /* ?? */
				//return 0;
				DHCP_DEBUG("discover timeout \n");
			}
			break;
		//case RENEW_REQUESTED:
		case DHCP_REQUESTING:
			/* timeout (requesting) */
			if(dhcp_desp.tried < dhcp_desp.retries){
				/* send request again */
				dhcp_request(&dhcp_desp);
				/* set timeout and add retry times */
				dhcp_desp.tried++;
				dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000;
				dhcp_time_sw = 1;
				//continue;
			}else{
				/* fail goto DHCP_INIT */
				{
					state = DHCP_INIT;
		    			dhcp_desp.timeout = 5*1000;
		   	 		dhcp_time_sw = 1;
		    			continue;
				}
				state = DHCP_INIT1;
				dhcp_desp.timeout = 0;
				(callback)(DHCP_REQUEST_FAIL, &dhcp_desp);
				//continue; /* ?? */
				//return 0;
			}
			break;
		case DHCP_BOUND:
			DHCP_DEBUG("[*****dhcp debug] timeout at dhcp bound %ld \n", dhcp_desp.timeout);
			/*clean socket read buffer, avoid buffer full and can't receviced ACK packet.*/
			FD_ZERO(&fds);
			FD_SET(dhcp_desp.sockfd, &fds);
			memset(tmp, 0, sizeof(tmp));
			while(1) {
				ret = select(dhcp_desp.sockfd + 1, &fds, NULL, NULL, &time_wait);
				if (ret == 0)
					break;
			dhcp_socket_recv(dhcp_desp.sockfd, tmp, 1, 0);
			}

			/* t1 expires */
			/* Lease is starting to run out, time to enter renewing state */
			dhcp_desp.t2_test = 0;
			state = DHCP_RENEWING;
			/* fall right through */
			/* no breaking */
		case DHCP_RENEWING:
			DHCP_DEBUG("[*****dhcp debug] timeout at dhcp renewing %ld \n", dhcp_desp.timeout);
			/* Either set a new T1, or enter REBINDING state */
			if(dhcp_desp.t2_test/* t2 has expired */){
				/* t2 timeout enter rebinding state */
				dhcp_desp.t2_test = 0;
				state = DHCP_REBINDING;
				dhcp_desp.offered_ip_addr = dhcp_desp.msg_in->yiaddr;
				dhcp_request(&dhcp_desp);
				/* we can try some times */
				dhcp_desp.tried = 0;
				dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000;
				dhcp_desp.retries = 5;
				dhcp_time_sw = 1;
			}else{
				/* t1 expires we send request-msg to lease server */
				dhcp_desp.offered_ip_addr = dhcp_desp.msg_in->yiaddr;
				dhcp_request(&dhcp_desp);
				/* set new t12 timeout ? */
				/* now timeout is t2-t1 */
				if(dhcp_desp.t2_timeout > dhcp_desp.t1_timeout){
		    			dhcp_desp.timeout = dhcp_desp.t2_timeout - dhcp_desp.t1_timeout;
				}else{
		    			dhcp_desp.timeout = 10*1000;
				}

				/* test for t2_timeout */
				dhcp_desp.t2_test = 1;
				dhcp_time_sw = 1;
			}
			break;
		case DHCP_REBINDING:
			DHCP_DEBUG("[*****dhcp debug]timeout at dhcp rebinding %ld \n", dhcp_desp.timeout);
			/* Either set a new T2, or enter INIT state */
			if(dhcp_desp.tried < dhcp_desp.retries){
				dhcp_request(&dhcp_desp);
				/* set new t12 timeout ? */
				dhcp_desp.tried++;
				dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000;
				dhcp_time_sw = 1;
			}else{
				/* fail goto DHCP_INIT */
				{
					state = DHCP_INIT;
		    			dhcp_desp.timeout = 5*1000;
		   	 		dhcp_time_sw = 1;
		    			continue;
				}
				state = DHCP_INIT1;
				dhcp_desp.timeout = 0;
				(callback)(DHCP_REBINDING_TIMEOUT, &dhcp_desp);
			}
			break;
		case DHCP_RELEASED:
			/* yah, I know, *you* say it would never happen */
			{
				state = DHCP_INIT;
		    		dhcp_desp.timeout = 10*1000;
		   	 	dhcp_time_sw = 1;
		    		continue;
			}
			dhcp_desp.timeout = 0x7fffffff; /* forever */
			break;
		}
	     }
            break;
            case MSG_DHCP:
            {
    		   /* a packet is ready, read it */
                /* socket recv */
                //DHCP_DEBUG(" dhcp_mainloop: recv packet \n");
                dhcp_time_now = DRV_OS_TimeNow();
#if RAW_SOCKET_MODE
                len = dhcp_get_raw_packet(recvbuf, dhcp_desp.sockfd);
                if(len == -3){
                    //DHCP_DEBUG("***try again\n");
                    
                    len = dhcp_get_raw_packet(recvbuf, dhcp_desp.sockfd);
                    while((len == -3)){
                        //DHCP_DEBUG("***try again again \n");
                        len = dhcp_get_raw_packet(recvbuf, dhcp_desp.sockfd);
                        if((DRV_OS_TimeNow() - dhcp_time_now) > 500){
                            //DHCP_DEBUG("@@@@@ = %ld > %ld\n", dhcp_desp.timeout, ((DRV_OS_TimeNow() - dhcp_time_now)));
                            if((DRV_OS_TimeNow() - dhcp_time_now) >= dhcp_desp.timeout){
                                dhcp_desp.timeout = 500;
                            }else{
                                dhcp_desp.timeout -= (DRV_OS_TimeNow() - dhcp_time_now);
                            }
                            dhcp_time_sw = 1;
                            break;
                        }
                    }
                }
#else
                len = dhcp_socket_recv(dhcp_desp.sockfd, recvbuf, 1500, 0);
#endif
                //DHCP_DEBUG(" dhcp_mainloop: out recv packet [%d]\n",len);
                /* len > 0 ? */
    		  if (len <= 0) {
                      //DHCP_DEBUG(" dhcp_mainloop: out recv packet [%d]\n",len);
    			 dhcp_time_sw = 1;
    			 continue;
    		  }
                /* set dhcp info to dhcp_msg */
                dhcp_parse_packet(&dhcp_desp, recvbuf, len);
		 
                /* get transaction id from packet */
                //DHCP_DEBUG("Ignoring XID %lx (our xid is %lx)\n",(unsigned long)dhcp_desp.xid,dhcp_desp.msg_in->xid);
                /* transaction id is right ? */
    		  if (dhcp_desp.xid != dhcp_desp.msg_in->xid) {
    			 DHCP_DEBUG("Ignoring XID %lx (our xid is %lx)\n",(unsigned long)dhcp_desp.xid,dhcp_desp.msg_in->xid);
                      dhcp_time_sw = 1;
    		        continue;
    		  }
                
                /* get message type from option */
                options_ptr = dhcp_get_option_ptr(&dhcp_desp, DHCP_OPTION_MESSAGE_TYPE);
                if(options_ptr == NULL){
                     DHCP_DEBUG("DHCP_OPTION_MESSAGE_TYPE option not found\n");
                     dhcp_time_sw = 1;
                     continue;
                }
                
		  if ((message_type = dhcp_get_option_byte(options_ptr + 2)) == 0) {
			DHCP_DEBUG("couldnt get option from packet -- ignoring \n");
                     dhcp_time_sw = 1;
			continue;
		  }
                
                /* now process recv msg */
		 DHCP_DEBUG("[*****dhcp debug] dhcp msg--recv a dhcp packet %ld \n", dhcp_desp.timeout);
    		 switch (state) {
			case DHCP_INIT1:
				/* do nothing */
				DHCP_DEBUG("dhcp of dhcp init1 state \n");
			break;
			case DHCP_INIT:
				/* error */
				//state = DHCP_SELECTING;
			break;
			case DHCP_SELECTING:
				/* Must be a DHCPOFFER to one of our xid's */
                    
				/* select and record all offered packet */
                    
				/* collect replies and get one for our-request. first found ? */
				if (message_type == DHCP_OFFER) {
					if(DHCP_YOU_OPTION60_NEEDED){
						/* process vendor class id */
						int you_len;
						options_ptr = dhcp_get_option_ptr(&dhcp_desp, DHCP_OPTION_VENDOR_CLASS_ID);
						if(options_ptr == NULL){
						    DHCP_DEBUG("DHCP_OPTION_VENDOR_CLASS_ID option not found\n");
						     dhcp_time_sw = 1;
						    continue;
						}
						you_len = dhcp_get_option_byte(&options_ptr[1]);
						if((you_len != dhcp_opt60.you_len) ||
						    (strncmp(dhcp_opt60.you_optbuf,(char *)(options_ptr+2), dhcp_opt60.you_len) != 0)){
						    DHCP_DEBUG("No server vendor ID found \n");
						     dhcp_time_sw = 1;
						    continue;
						}else{
						    ;
						}
					}                  
					//dhcp_desp.msg_out = (struct dhcp_msg *)dhcp_mem_malloc(sizeof(struct dhcp_msg));
					//if(dhcp_desp.msg_out == NULL){
					/* mem malloc fail */
					//    DHCP_DEBUG("mem malloc error when creating packet!\n");
					//    return 0;
					//}
					// DHCP_DEBUG("*** ##mem malloc %lp \n", dhcp_desp.msg_out);
					// dhcp_mem_free((u8_t *)dhcp_desp.msg_out);
					//state = DHCP_REQUESTING;
					/* get necessary msg info */
					options_ptr = dhcp_get_option_ptr(&dhcp_desp, DHCP_OPTION_SERVER_ID);
					if(options_ptr == NULL){
						DHCP_DEBUG("DHCP_OPTION_SERVER_ID option not found\n");
						dhcp_time_sw = 1;
						continue;
					}
					if ((dhcp_desp.server_ip_addr.addr = ipcfg_htonl(dhcp_get_option_long(options_ptr + 2))) == 0) {
						DHCP_DEBUG("couldnt get option from packet -- ignoring \n");
						dhcp_time_sw = 1;
						continue;
					}
					dhcp_desp.offered_ip_addr = dhcp_desp.msg_in->yiaddr;
	                        
					/* send request */
					state = DHCP_REQUESTING;
					dhcp_request(&dhcp_desp);
					/* set timeout and retry times */
					dhcp_desp.tried = 0;
					dhcp_desp.timeout = (dhcp_desp.tried+1)*2*1000;
					dhcp_desp.retries = 10;
					dhcp_time_sw = 1;
    				}else{
					dhcp_time_sw = 1;
    				}
    				break;
			//case RENEW_REQUESTED:
			case DHCP_REQUESTING:
			case DHCP_RENEWING:
			case DHCP_REBINDING:
				DHCP_DEBUG("[*****dhcp debug] dhcp msg--state%d, timeout %ld \n ", state , dhcp_desp.timeout);
				/* ignore all other packets */
					if(DHCP_YOU_OPTION60_NEEDED){
						/* process vendor class id */
						int you_len;
						options_ptr = dhcp_get_option_ptr(&dhcp_desp, DHCP_OPTION_VENDOR_CLASS_ID);
						if(options_ptr == NULL){
						    DHCP_DEBUG("DHCP_OPTION_VENDOR_CLASS_ID option not found\n");
						     dhcp_time_sw = 1;
						    continue;
						}
						you_len = dhcp_get_option_byte(&options_ptr[1]);
						if((you_len != dhcp_opt60.you_len) ||
						    (strncmp(dhcp_opt60.you_optbuf,(char *)(options_ptr+2), dhcp_opt60.you_len) != 0)){
						    DHCP_DEBUG("No server vendor ID found \n");
						     dhcp_time_sw = 1;
						    continue;
						}else{
						    ;
						}
					} 
				if (message_type == DHCP_ACK) {					
					if(state == DHCP_REQUESTING){ 		                     
						if(dhcp_arp_check(&dhcp_desp)){ /* ip can be used */
							/* record offered info */
							if(!dhcp_get_offerinfo(&dhcp_desp)){
								state = DHCP_INIT;
								dhcp_desp.timeout = 10*1000;
								dhcp_time_sw = 1;
								continue;
							}
							/* state in bound */
							state = DHCP_BOUND;
							(callback)(DHCP_BOUND_OK, &dhcp_desp);
							dhcp_time_sw = 1;
						}else{
							/* not accept (ip has been using) send dhcpdecline */
							dhcp_decline(&dhcp_desp);
							/* goto init and try again */
							state = DHCP_INIT;
							dhcp_desp.timeout = 10*1000;
							dhcp_time_sw = 1;
							continue;
							//(callback)(DHCP_BOUND_FAIL, &dhcp_desp); /* arp check fail */
						}
					}else if(state == DHCP_RENEWING){
						/* dont need arp check */
						if(!dhcp_get_offerinfo(&dhcp_desp)){
							state = DHCP_INIT;
							dhcp_desp.timeout = 10*1000;
							dhcp_time_sw = 1;
							continue;
						}
						state = DHCP_BOUND;
						/* record lease set t1 t2 timeout */
						//dhcp_desp.t1_timeout = dhcp_desp.offered_t1_renew;
						//dhcp_desp.t2_timeout = dhcp_desp.offered_t2_rebind;
						//dhcp_desp.timeout = dhcp_desp.t1_timeout;
						(callback)(DHCP_RENEWING_OK, &dhcp_desp);
						dhcp_time_sw = 1;
					}else{/* DHCP_REBINDING */
						/* dont need arp check */
						DHCP_DEBUG("DHCP REBINDING state=%d, timeout=%ld \n", state, dhcp_desp.timeout);
						if(!dhcp_get_offerinfo(&dhcp_desp)){
							state = DHCP_INIT;
							dhcp_desp.timeout = 10*1000;
							dhcp_time_sw = 1;
							continue;
						}
						state = DHCP_BOUND;
						/* record lease set t1 t2 timeout */
						//dhcp_desp.t1_timeout = dhcp_desp.offered_t1_renew;
						//dhcp_desp.t2_timeout = dhcp_desp.offered_t2_rebind;
						//dhcp_desp.timeout = dhcp_desp.t1_timeout;
						(callback)(DHCP_REBINDING_OK, &dhcp_desp);
						dhcp_time_sw = 1;
					}
				} else if (message_type == DHCP_NAK) {
					options_ptr = dhcp_get_option_ptr(&dhcp_desp, DHCP_OPTION_SERVER_ID);
					if(options_ptr == NULL){
						DHCP_DEBUG("DHCP_OPTION_SERVER_ID option not found\n");
						
					}else if (dhcp_desp.server_ip_addr.addr != ipcfg_htonl(dhcp_get_option_long(&options_ptr[ 2]))){
						DHCP_DEBUG("server identifier of nak package is different from request package \n");
						dhcp_time_sw = 1;
						continue;
					}
					
					/* return to init state */
					if(state == DHCP_REQUESTING){
						state = DHCP_INIT;
						dhcp_desp.timeout = 10*1000;
						(callback)(DHCP_REQUESTING_NAK, &dhcp_desp);
						dhcp_time_sw = 1;
						continue;
					}else if(state == DHCP_RENEWING){
						state = DHCP_INIT;
						dhcp_desp.timeout = 10*1000;
						(callback)(DHCP_RENEWING_NAK, &dhcp_desp);
						dhcp_time_sw = 1;
						continue;
					}else{
						state = DHCP_INIT;
						dhcp_desp.timeout = 10*1000;
						(callback)(DHCP_REBINDING_NAK, &dhcp_desp);
						dhcp_time_sw = 1;
						continue;
					}
				}else{
					dhcp_time_sw = 1;
				}
				break;
				/* case BOUND, RELEASED: - ignore all packets (offered ack nak...)*/
    			}
		}
		break;
		case MSG_STOP:
		    /* close socket */
		    DHCP_DEBUG("dhcp mainloop close socket... \n");
		    //sys_mutex_lock(&dhcp_locker);
		    sys_sem_wait(dhcp_sem);
		    if(dhcp_desp.sockfd >= 0){
		        DHCP_DEBUG("dhcp mainloop close socket222... \n");
		        close(dhcp_desp.sockfd);
		        dhcp_desp.sockfd = -1;
		    }
		    dhcp_mem_free((void *)dhcp_desp.msg_in);
		    dhcp_desp.msg_in = NULL;
		    sys_sem_signal(dhcp_sem);
		    //sys_mutex_unlock(&dhcp_locker);
		    break;
		case MSG_QUIT:
		    /* quit main loop */
		    DHCP_DEBUG("dhcp mainloop goout... \n");
		    return;
		default:
		    break;
	}
    }
}

void dhcp_goout_mainloop()
{
    /* send msg quit */
    APP_MSG_T msg;

    msg.type = MSG_QUIT;

    sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
}

int dhcp_start(char *if_name, char *our_opt60, int our_len, char *you_opt60, int you_len)
{ 
    /* process option request */
    /* howto:
       get usr option more effectively
    */
    APP_MSG_T msg;

    int ifindex;
    u32_t addr;
    u8_t arp[6];
    int ret;

    if(if_name == NULL || (strlen(if_name)>16)){
        DHCP_DEBUG("dhcp start : no interface , return ... \n");
        return -1;
    }else{
        memset(current_if, '\0', sizeof(current_if));
        strcpy(current_if, if_name);
    }

    memset(&dhcp_opt60, 0, sizeof(struct dhcp_opt60));
    if(our_opt60 != NULL && our_len != 0){
        DHCP_OUR_OPTION60_NEEDED = 1;
        dhcp_opt60.our_len = our_len;
        strncpy(dhcp_opt60.our_optbuf, our_opt60, our_len);
    }else{
        DHCP_OUR_OPTION60_NEEDED = 0;
    }
    if(you_opt60 != NULL && you_len != 0){
        DHCP_YOU_OPTION60_NEEDED = 1;
        dhcp_opt60.you_len = you_len;
        strncpy(dhcp_opt60.you_optbuf, you_opt60, you_len);
    }else{
        DHCP_YOU_OPTION60_NEEDED = 0;
    }
    
    /* use socket */
#if RAW_SOCKET_MODE
    //changed by lzz 20091103
    //ret = read_interface(current_if, &ifindex, &addr, &arp);
    ret = read_interface(current_if, &ifindex, NULL, &arp);
    if(ret < 0){
        DHCP_DEBUG("dhcp_start read interface error return \n");
        return -1;
    }
    dhcp_desp.ifindex = ifindex;
    dhcp_desp.sockfd = dhcp_open_raw_socket(ifindex);
#else
    dhcp_desp.sockfd = dhcp_socket_register();
#endif

    /* get option and create request-packet */
    //dhcp_create_packet();

    /* init and send dhcp msg */
    /* start dhcp timer */
    state = DHCP_INIT;
    dhcp_desp.state = DHCP_INIT;
    dhcp_desp.timeout = 0;
    //dhcp_desp.msg_in = (struct dhcp_msg *)dhcp_mem_malloc(DHCP_MSG_LEN + 4);
    dhcp_desp.msg_in = (struct dhcp_msg *)dhcp_mem_malloc(sizeof(struct dhcp_msg));
    //DHCP_DEBUG("***mem malloced for in %lp \n",dhcp_desp.msg_in);
    msg.type = MSG_TIMEOUT;

    sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);

    return 1;
    
}

void dhcp_stop()
{
    /* send msg stop */
    APP_MSG_T msg;

    msg.type = MSG_STOP;
    //dhcp_mem_free(dhcp_desp.msg_in);
    sys_send_msg(msg_queue_id, &msg, dhcp_msg_queue);
    //wait stop 
    //dhcp_mem_free(dhcp_desp.msg_in);
    //dhcp_desp.msg_in = NULL;
}

void dhcp_init(void)
{
    u32_t timer_thread_id;
    u32_t mloop_thread_id;
    
    /* init dhcp info */
    dhcp_desp.sockfd = -1; /* not created */
    /* ... */
    dhcp_desp.arp[5] = 0x00;
    
    dhcp_timing = 1; /* timer has been ready */

    dhcp_sem = sys_sem_init(1);
    if(dhcp_sem == NULL){
        DHCP_DEBUG("DHCP INIT : sys sem init error \n");
        return ;
    }
    /* msg queue init */
    msg_queue_id = sys_msgqueue_init(msg_project_id);
    /* mutex init */
    sys_mutex_init(&dhcp_locker);
    
    /* create task a to act as timer */
    timer_thread_id = sys_new_thread(dhcp_timer, NULL, 3,"dhcp_timer");
    /* create task b to act as mainloop */
    mloop_thread_id = sys_new_thread(dhcp_mainloop, (void *)dhcp_client_callback, 8, "dhcp_mainloop");
    /* if valid ? */
    dhcp_desp.timer_thread_id = timer_thread_id;
    dhcp_desp.mloop_thread_id = mloop_thread_id;
}

void dhcp_term(void)
{
    /* stop dhcp timer */
    dhcp_stop_timer();
    /* release sources which was allocated for task a */
    sys_delete_thread(dhcp_desp.timer_thread_id);
    
    /* goout main loop */
    dhcp_goout_mainloop();
    /* realse sources which was allocated for task b */
    sys_delete_thread(dhcp_desp.mloop_thread_id);

    /* realse msg queue */
    sys_msgqueue_release(msg_queue_id);

    sys_sem_release(dhcp_sem);
    dhcp_msg_queue = NULL;
}

void result_callback (OFFERED_INFO_T info)
{
    DHCP_DEBUG("offered ip addr is %lx \n", info.offered_ip.addr);
    DHCP_DEBUG("offered gw addr is %lx \n", info.offered_gw.addr);
    DHCP_DEBUG("offered sn mask is %lx \n", info.offered_sn.addr);
    DHCP_DEBUG("offered dns0 addr is %lx \n", info.offered_dns[0].addr);
    DHCP_DEBUG("offered dns1 addr is %lx \n", info.offered_dns[1].addr);
    DHCP_DEBUG("offered lease is %lx seconds \n", info.offered_timeout);
}

void dhcp_test()
{
    int ret;
    /*
    dhcp option 60 ,,,,
    */
    dhcp_init(); /* allocate resources */
    
    /* register callback */
    DHCP_VENDOR_CLASS_ID_OPTION_NEEDED = 0;
    DHCP_DO_ARP_CHECK = 0;
    register_dhcpok_callback(result_callback);
    ret = dhcp_start("eth0", NULL, 0, NULL, 0);
    if(ret < 0){
        /* unregister callback */
        unregister_dhcpok_callback();
    }else{
        /* today evening */
        sys_delay(10*1000); /* 20s */
        dhcp_stop();
        /* unregister callback */
        unregister_dhcpok_callback();
    }
    
    sys_delay(2*1000); /* 60s */
    //sys_delay(30*1000); /* 60s */
    DHCP_DEBUG("stop dhcp \n");
    
    /* register callback */
    DHCP_VENDOR_CLASS_ID_OPTION_NEEDED = 0;
    DHCP_DO_ARP_CHECK = 0;
    register_dhcpok_callback(result_callback);
    ret = dhcp_start("wlan0", NULL, 0, NULL, 0);
    if(ret < 0){
        /* unregister callback */
        unregister_dhcpok_callback();
    }else{
        /* today evening */
        sys_delay(10*1000); /* 20s */
        dhcp_stop();
        /* unregister callback */
        unregister_dhcpok_callback();
    }
    
    sys_delay(2*1000); /* 60s */
    //sys_delay(30*1000); /* 60s */
    DHCP_DEBUG("stop dhcp \n");
    
    /* etc ... */
    while(1){
        sys_delay(2*1000);
    }
    /* machine shut down */
    dhcp_term(); /* release sources */
    sys_delay(2*1000); /* 60s */
    return;
}

#if 0//(defined(__WIN32__) || defined(__LINUX_SYS__))
int main(int argc, char **argv)
{
    dhcp_test();
    return 0;
}
#endif

