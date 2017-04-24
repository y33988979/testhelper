/*

copyright(c)2008
tftp client implement

*/

/*
#include "string.h"
#include "stdio.h"
#include "tcpip/sockets.h"
#include "lwip/sys.h"
#include "lwip/lwipopts.h"
#include "arch/lib.h"
#include "arch/sys_arch.h"
#include "lwip/inet.h"
#include "lwip/api.h"
#include "lwip/mem.h"
#include "tftp/tftp.h"
*/

#include <sys/socket.h>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include "tftp.h"
/*
用linux下的相关头文件代替上述头文件
*/

#define TFTP_DEBUG printf
#define TFTP_SOCKET     1
#define DEBUG_TFTP      0

static struct tftp_socket g_tftp_sock;
static char *tftpbuf;
static short int g_block_no=0;

#if TFTP_SOCKET
static int tftp_construct_rq(char *filename, int filelen, char *mode, int flag, char *buf)
{
    char *pbuffer;
    int i=0;
    
    if(buf == NULL){
        TFTP_DEBUG("[tftp]param buf is null! \n");
        return TFTP_RQ_ERR;
    }
    pbuffer = buf;
    
    if(flag == 1){
        /* read request */
        *pbuffer = 0x00;
        *(pbuffer+1) = 0x01;
        pbuffer += 2;
    }else if(flag == 2){
        /* write request */
        *pbuffer = 0x00;
        *(pbuffer+1) = 0x02;
        pbuffer += 2;
    }else{
        /* go here unknown flag */
        TFTP_DEBUG("[tftp]unknown flag ! \n");
        return TFTP_RQ_ERR;
    }
    
    /* file name */
    for(i=0; i<filelen; i++){
        *pbuffer = filename[i];
        pbuffer++;
    }
    /* 0 */
    *pbuffer = 0;
    pbuffer++;
    /* mode */
    if(*mode == 'o'){
        for(i=0; i<5; i++){
            *pbuffer = mode[i];
            pbuffer++;
        }
    }else if(*mode == 'n'){
        for(i=0; i<8; i++){
            *pbuffer = mode[i];
            pbuffer++;
        }
    }else{
        TFTP_DEBUG("[tftp]mode error! \n");
        return TFTP_RQ_ERR;
    }
    /* 0 */
    *pbuffer = 0;
    
    return 0;
    
}

static int tftp_construct_ack(char *buf, short int blockno)
{
    char *pbuffer;
    
    if(buf == NULL){
        TFTP_DEBUG("[tftp]param buf is null! \n");
        return TFTP_ACK_ERR;
    }
    pbuffer = buf;
    
    /* ack */
    *pbuffer = 0x00;
    *(pbuffer+1) = 0x04;
    pbuffer += 2;

    /* ack */
    *pbuffer = (blockno>>8) & 0xff;
    *(pbuffer+1) = blockno & 0xff;
    pbuffer += 2;
    
    return 0;
    
}

static int tftp_consrtuct_data(void)
{
    return 0;
}

static int tftp_construct_err(int err_no)
{
    return 0;
}

/*
@return >0 send bytes
        =0 error
*/
static int tftp_send_rrq(char *filename, int filelen, struct tftp_socket tftp_sock)
{
    int i;
    int rqlen;
    char *tftp_buffer;
    int ret;
    int nbytes;
    int len;
    int socketfd;
    fd_set wset;
    struct timeval tmo;
    struct sockaddr_in remote_addr;
    char mode[5]={'o','c','t','e','t'};
       
    rqlen = 2 + filelen + 1 + 5 + 1;
    tftp_buffer = (char *)malloc(rqlen);
    if(tftp_buffer==NULL){
        TFTP_DEBUG("[tftp]mem malloc error! \n");
        return TFTP_MEM;
    }

    ret = tftp_construct_rq(filename, filelen, mode, 1, tftp_buffer);
    if(ret < 0){
        free((void *)tftp_buffer);
        return ret;
    }
#if DEBUG_TFTP
    for(i=0; i<rqlen; i++){
        TFTP_DEBUG("%c", tftp_buffer[i]);
    }
    TFTP_DEBUG("\n");
#endif
    socketfd = tftp_sock.sockfd;
    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(tftp_sock.server_addr.s_addr);
    remote_addr.sin_port = htons(tftp_sock.server_port);
    len = sizeof(remote_addr);
    
/* send request */
    tmo.tv_sec = 1;
    tmo.tv_usec = 0; /* 100 m-seconds */
    i = 0;
try_again:    
#if 1
    FD_ZERO(&wset);
    FD_SET(socketfd, &wset);
    ret = 0;
    ret = select(socketfd+1, NULL, &wset, NULL, &tmo);
    if(ret > 0){
    }else if(ret == 0){
        TFTP_DEBUG("[tftp]tftp rrq socket select timeout! \n");
        i++;
        if(i ==4 ){
            TFTP_DEBUG("[tftp]tftp rrq socket select %d times quit! \n", i);
	     free((void *)tftp_buffer);
            return 0;
        }
        goto try_again;
    }else{
        /* ret < 0 error*/
        TFTP_DEBUG("[tftp]tftp socket select error! \n");
	 free((void *)tftp_buffer);
        return 0;
    }
#endif
    if(FD_ISSET(socketfd, &wset)){ 
        /* send data */
        nbytes = sendto(socketfd, tftp_buffer, rqlen, 0,
                        (struct sockaddr *)&remote_addr, len);
        
        if(nbytes > 0){
            /* send ok */
            free((void *)tftp_buffer);
            return nbytes;
        }else{
            /* send fail */
            TFTP_DEBUG("[tftp]tftp socket send error! \n");
            free((void *)tftp_buffer);
            return TFTP_SOCK_ERR;
        }
    }else{
        TFTP_DEBUG("[tftp]tftp socket select error! \n");
        goto try_again;
        //return 0;
    }
}

/*
@return >0 send bytes
        =0 error
*/
static int tftp_send_ack(struct tftp_socket tftp_sock, short int block_no)
{
    int i;
    char tftp_buffer[4];
    int ret;
    int nbytes;
    int len;
    int socketfd;
    //fd_set wset;
    struct timeval tmo;
    struct sockaddr_in remote_addr;

    socketfd = tftp_sock.sockfd;
    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(tftp_sock.server_addr.s_addr);
    remote_addr.sin_port = htons(tftp_sock.server_port);
    len = sizeof(remote_addr);

    ret = tftp_construct_ack(tftp_buffer, block_no);
    if(ret < 0){
        return ret;
    }

    /* send request */
    tmo.tv_sec = 0;
    tmo.tv_usec = 500*1000; /* 100 m-seconds */
    i = 0;
try_again:  
#if 0
    FD_ZERO(&wset);
    FD_SET(socketfd, &wset);
    ret = 0;
    ret = select(socketfd+1, NULL, &wset, NULL, &tmo);
    if(ret > 0){
    }else if(ret == 0){
        TFTP_DEBUG("[tftp]tftp socket select timeout! \n");
        i++;
        if(i ==4 ){
            TFTP_DEBUG("[tftp]tftp socket select %d times quit! \n", i);
            return 0;
        }
        goto try_again;
    }else{
        /* ret < 0 error*/
        TFTP_DEBUG("[tftp]tftp socket select error! \n");
        return 0;
    }
#endif
    //STTBX_Print(("*****tftp send  ack no is %d\n",block_no));
    if(1/*FD_ISSET(socketfd, &wset)*/){ 
        /* send data */
        nbytes = sendto(socketfd, tftp_buffer, sizeof(tftp_buffer), 0,
                        (struct sockaddr *)&remote_addr, len);
        
        if(nbytes > 0){
            /* send ok */
            return nbytes;
        }else{
            /* send fail */
            TFTP_DEBUG("[tftp]tftp socket send error! \n");
            return TFTP_SOCK_ERR;
        }
    }else{
        TFTP_DEBUG("[tftp]tftp socket select error! \n");
        goto try_again;
        //return 0;
    }
}

static void tftp_send_data(void)
{
}

static void tftp_send_err(void)
{
}

static void tftp_send_oack(void)
{
}
#else
static void tftp_timeout(void)
{
}
int tftp_callback(void *arg);
#endif

static void tftp_send_process(void)
{
}

static int tftp_recv_process(char *from, int len, char *to)
{
    int ret;
    short int tftp_type;
    short int block_no;
    short int error_no;
    char *pfrom;

    ret = 0;
    pfrom = from;

#if DEBUG_TFTP
    /* need test */
    for(ret=0; ret<len; ret++){
        TFTP_DEBUG("%c",from[ret]);
        if(ret%16==0){
            TFTP_DEBUG("\n");
        }
    }
    TFTP_DEBUG("\n");
    ret=0;
#endif
    
    tftp_type = (*(short int *)pfrom);
    tftp_type = htons(tftp_type);
    
    switch(tftp_type)
    {
        case TFTP_RRQ:
            /* server? */
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv rrq ...\n");
#endif
            ret = TFTP_NOSUPPORT_OPT;
            break;
        case TFTP_WRQ:
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv wrq ...\n");
#endif
            ret = TFTP_NOSUPPORT_OPT;
            break;
        case TFTP_DATA:
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv data ...\n");
#endif
            pfrom += 2;
            block_no = (*(short int *)pfrom);
            block_no = htons(block_no);
            if(block_no == g_block_no){
                tftp_send_ack(g_tftp_sock, block_no);
                //STTBX_Print(("tftp send dup ack no is %d\n",block_no));
                return 0;
            }
            pfrom += 2;
            /* copy to buf */
            memcpy(to, from+4, len);

#if DEBUG_TFTP
            TFTP_DEBUG("from is %p \n", (from+4));
            TFTP_DEBUG("to is %p \n", to);
            TFTP_DEBUG("tftpbuf is %p \n", tftpbuf);
            for(ret=0; ret<len; ret++){
                TFTP_DEBUG("%c",tftpbuf[ret]);
                if(ret%16==0){
                    TFTP_DEBUG("\n");
                }
            }
            TFTP_DEBUG("\n");
            ret=0;
#endif
            g_block_no = block_no;
            //STTBX_Print(("current block no is %d\n",g_block_no));
            /* send ack */
            ret = tftp_send_ack(g_tftp_sock, block_no);
            //STTBX_Print(("tftp send ok ack no is %d\n",block_no));
            break;
        case TFTP_ACK:
            /* no action */
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv ack ...\n");
#endif
            ret = TFTP_NOSUPPORT_OPT;
            break;
        case TFTP_ERROR:
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv error error...\n");
#endif
            pfrom += 2;
            error_no = (*(short int *)pfrom);
            error_no = htons(error_no);
            pfrom += 2;
            TFTP_DEBUG("[tftp]recv error ...%d\n",error_no);
            while(*pfrom != 0x00){
                TFTP_DEBUG("%c", *pfrom);
                pfrom++;
            }
            TFTP_DEBUG("\n[tftp]error message end! \n");
            if(error_no == 1){
                ret = TFTP_FILE_NOT_FOUND;
            }else{
                ret = TFTP_RCV_ERR;
            }
            break;
        case TFTP_OACK:
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv oack ...\n");
#endif
            ret = TFTP_NOSUPPORT_OPT;
            break;
        default:
#if DEBUG_TFTP
            TFTP_DEBUG("[tftp]recv default ...\n");
#endif
            ret = TFTP_NOSUPPORT_OPT;
            break;
    }
    return ret;
}

/* 
0 error 
else recv data size
*/
static int tftp_recv(struct tftp_socket tftp_sock, char *buf, int bufsize)
{
    int i;
    char *recvbuf;
    int socketfd;
    fd_set rset;
    struct timeval tmo;
    int ret;
    int nbytes;
    unsigned int len;
    int recv_data_size;
    struct sockaddr_in remote_addr;

    recvbuf = (char *)malloc(600);/* max 516*/
    if(recvbuf == NULL){
        TFTP_DEBUG("[tftp]recv buf mem malloc error! \n");
        return TFTP_MEM;
    }

    socketfd = tftp_sock.sockfd;
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(tftp_sock.server_addr.s_addr);
    remote_addr.sin_port = htons(tftp_sock.server_port);
    
    len = sizeof(remote_addr);
    
    /* tftp recv */
    tmo.tv_sec = 1;
   // tmo.tv_usec = 500*1000; /* 100 m-seconds */
   tmo.tv_usec = 0;
    i = 0;
    recv_data_size = 0;

    for(;;){
try_again:    
        FD_ZERO(&rset);
        FD_SET(socketfd, &rset);
        ret = 0;
        ret = select(socketfd+1, &rset, NULL, NULL, &tmo);
        if(ret > 0){
            i=0;
        }else if(ret == 0){
            TFTP_DEBUG("[tftp]tftp recv socket select timeout! %d\n", i);
            i++;
            if(i ==20 ){
                TFTP_DEBUG("[tftp]tftp recv socket select %d times quit! \n", i);
                free((void *)recvbuf);
                return TFTP_LINK;
            }
            goto try_again;
        }else{
            /* ret < 0 error*/
            TFTP_DEBUG("[tftp]tftp socket select error! \n");
            free((void *)recvbuf);
            return TFTP_SOCK_ERR;
        }

        if(FD_ISSET(socketfd, &rset)){ 
            /* recv data */
            nbytes = recvfrom(socketfd, recvbuf, 516, 0, 
                    (struct sockaddr *)&remote_addr, &len);
            if(nbytes > 0){
                /* recv ok */
                g_tftp_sock.server_port = htons(remote_addr.sin_port);
#if DEBUG_TFTP
                //tftp_sock.server_addr.s_addr = htonl(remote_addr.sin_addr.s_addr);
                TFTP_DEBUG("*****tftp server addr is %lx \n", g_tftp_sock.server_addr.s_addr);
                TFTP_DEBUG("*****tftp server port is %lx \n", g_tftp_sock.server_port);
                TFTP_DEBUG("*****tftp recv num is %d \n", nbytes);
#endif
                if((recv_data_size+nbytes-4)>bufsize){
                    TFTP_DEBUG("[tftp]recv data size[%d] + nbytes[%d] > bufsize[%d]!\n",recv_data_size, nbytes-4, bufsize);
                    free((void *)recvbuf);
                    return TFTP_BUF;
                }
                ret = tftp_recv_process(recvbuf, nbytes, buf+recv_data_size);
                if(ret < 0){
                    TFTP_DEBUG("[tftp]tftp recv process error! \n");
                    free((void *)recvbuf);
                    return ret;
                }
                
                if(nbytes < 516){
                    /* recv end */
                    free((void *)recvbuf);
                    return recv_data_size+nbytes-4;
                }
                
                if(ret == 0){/* duplicate data */
                    //recv_data_size +=4;
                    //recv_data_size -=nbytes;
                }else{
                    recv_data_size += nbytes;
                    recv_data_size -= 4;
                }
            }else if(nbytes == 0){
                /* recv end? */
                /* data end at 512 bytes align */
                free((void *)recvbuf);
                return recv_data_size;
            }else{
                /* recv fail */
                TFTP_DEBUG("[tftp]tftp socket recv error! \n");
                free((void *)recvbuf);
                return TFTP_SOCK_ERR;
            }
        }else{
            TFTP_DEBUG("[tftp]tftp socket select error! \n");
            goto try_again;
            //return 0;
        }
    }
}

/*
 @hostname: local host name or ip
 @hostport: local host port for sending-request
 @servername: tftp server name or ip
 @filename: to get or put file on server (may be include path)
 @blocksize: data size for per-trans (default 512)

 @desc: get the file fixed by param filename from tftp server

 @todo:
 own our ip addr
 own our eth addr
 own server ip addr(through resolver )
 own server eth addr(arp)
 send rd request to server on tftp well-known port
 get data response from server on anyport and remember it
 send ack response to server on anyport(we have remembered it)
 ...
 while data size < blocksize 512 end

 @return
 >0 data size really read from server
 <=0 error
*/
int tftp_get_file(char *hostname, int hostport, char *servername, 
                   char *filename, int filelen, char *buf, int bufsize)
{
    struct in_addr server_addr;
    struct in_addr host_addr;
    int server_port;

    /* param check */
    if(servername != NULL){
        if((server_addr.s_addr = htonl(inet_addr(servername)))!= INADDR_NONE){
            ;
        }else{
            server_addr.s_addr = (IPCFG_GetHostByName(servername));
            if(server_addr.s_addr == 0){
                TFTP_DEBUG("[tftp]server name error or dns error! \n");
                return TFTP_SERVER_NOT_FOUND;
            }else{
                TFTP_DEBUG("[tftp]server addr is %lx\n",server_addr.s_addr);
            }
        }
    }

    server_port = TFTP_PORT;
    
    if(hostname == NULL){
        host_addr.s_addr = htonl(INADDR_ANY);
    }

    if(hostport == 0 || (hostport < 1024)){
        hostport = 5000;
    }

    if(filename == NULL){
        TFTP_DEBUG("[tftp]filename is null! \n");
        return TFTP_PARAM;
    }else if(filename != "00_5105_314.bin")
    {
    }else{
    }

    if(buf == NULL){
        TFTP_DEBUG("[tftp]buf param is null! \n");
        return TFTP_PARAM;
    }
    
    g_block_no = 0;
    
#if TFTP_SOCKET
/* use socket */
{
    int ret;
    int len;
    int socketfd;
    struct sockaddr_in local_addr;
    struct sockaddr_in remote_addr;

    //memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET; //address family
    local_addr.sin_addr.s_addr = htonl(host_addr.s_addr); // ip addr
    local_addr.sin_port = htons(hostport); //port number

    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(server_addr.s_addr);
    remote_addr.sin_port = htons(server_port);
    
    len = sizeof(remote_addr);

    g_tftp_sock.host_addr = host_addr;
    g_tftp_sock.host_port = hostport;
    g_tftp_sock.server_addr = server_addr;
    g_tftp_sock.server_port = server_port;
    
    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0){
        TFTP_DEBUG("[tftp]tftp socket error! \n");
        return TFTP_SOCK_ERR;
    }
    g_tftp_sock.sockfd = socketfd;
	//TFTP_DEBUG("g_tftp_sock.sockfd =%d\n",g_tftp_sock.sockfd);
    
    ret = bind(socketfd,(struct sockaddr*)&local_addr,sizeof(local_addr));
    if(ret < 0){
        close(socketfd);
        TFTP_DEBUG("[tftp]tftp socket bind error! \n");
        return TFTP_SOCK_ERR;
    }
    
#if DEBUG_TFTP
    TFTP_DEBUG("*****tftp host addr is %lx \n", g_tftp_sock.host_addr.s_addr);
    TFTP_DEBUG("*****tftp server addr is %lx \n", g_tftp_sock.server_addr.s_addr);
    TFTP_DEBUG("*****tftp host port is %lx \n", g_tftp_sock.host_port);
    TFTP_DEBUG("*****tftp server port is %lx \n", g_tftp_sock.server_port);
#endif

    /* send read request */
    ret = tftp_send_rrq(filename, filelen, g_tftp_sock);
    if(ret <= 0){
        close(g_tftp_sock.sockfd);
        TFTP_DEBUG("[tftp]tftp socket send rrq error! \n");
        return ret;
    }

    /* recv data from tftp server */
    ret = tftp_recv(g_tftp_sock, buf, bufsize);
    if(ret <= 0){
        close(g_tftp_sock.sockfd);
        TFTP_DEBUG("[tftp]tftp recv error! \n");
        return ret;
    }

    /* sucessly recv data */
    close(g_tftp_sock.sockfd);
    return ret;
}
#else
/* use raw api 
{
   send request
   set TIMEOUT
   set recv callback arg is buf
   process in callback
   check end 
   if timeout return
   if end return
}
*/   
#endif
    /* ... */
}

int tftp_put_file(char *hostname, int hostport, char *servername, 
                   char *filename, int filelen, int blocksize, char *buf)
{
    return 0;
}

#if 0
void tftp_test_task(void)
{
    int i;
    int ret;
    
    tftpbuf = (char *)malloc(1024*15);
    if(tftpbuf == NULL){
        TFTP_DEBUG("[tftp]tftp buf error! \n");
        while(1){
            DRV_OS_TaskDelay(6250000);
        }
    }
    
    DRV_OS_TaskDelay(62500000*2);
    ret = tftp_get_file(NULL, 6000, "192.168.4.177", "tftp_test_file", 14, tftpbuf, 1024*10);
    if(ret == 0){
        TFTP_DEBUG("[tftp]tftp task get file failed ! \n");
        free((void *)tftpbuf);
        tftpbuf = NULL;
        while(1){
            DRV_OS_TaskDelay(6250000);
        }
    }

    TFTP_DEBUG("**********tftp get file start...(length is %d) \n", ret);
    i=0;
    while(i != ret){
        if((tftpbuf[i])==13){
            TFTP_DEBUG("\n");
        }
        TFTP_DEBUG("%c",tftpbuf[i]);
        i++;
        
    }
    TFTP_DEBUG("\n**********tftp get file end\n");
    
    free((void *)tftpbuf);
    tftpbuf = NULL;
    
    while(1){
        DRV_OS_TaskDelay(6250000);
    }
}

void tftp_test_init(void)
{
    task_t *app_tftp_test_task;
    
    app_tftp_test_task = task_create(
                                    (void (*)(void *))tftp_test_task, //function to call
                    			    NULL,   //No Arguement
                    			    1024*5,   //Minimum Define stack size (16KB)
                                    5,      //Priority 254, main runs at 255 ( MAX_USER_PRIORITY)
                    			    "tftp_test_task",
                    			    0       //No flag
                    			    );

}

#endif
