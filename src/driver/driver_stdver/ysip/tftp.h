/*
copyright(c)2008
    
*/


#ifndef _TFTP_CLIENT_H_
#define _TFTP_CLIENT_H_
/*
#include "tcpip/lwipsock.h"
*/
/* 
用linux下的相关头文件代替上述头文件 
支持 struct in_addr。。。
*/
#include <netinet/in.h>

#define TFTP_PORT	69		/* Well known TFTP port # */

#define TIMEOUT     5

/*
 *	TFTP operations.
 */
#define TFTP_RRQ	1
#define TFTP_WRQ	2
#define TFTP_DATA	3
#define TFTP_ACK	4
#define TFTP_ERROR	5
#define TFTP_OACK	6

/*
Error Codes

   Value     Meaning

   0         Not defined, see error message (if any).
   1         File not found.
   2         Access violation.
   3         Disk full or allocation exceeded.
   4         Illegal TFTP operation.
   5         Unknown transfer ID.
   6         File already exists.
   7         No such user.
*/
#define TFTP_MEM        -1
#define TFTP_BUF        -2
#define TFTP_LINK       -3
#define TFTP_SOCK_ERR    -4
#define TFTP_PARAM      -5
#define TFTP_SERVER_NOT_FOUND   -6
#define TFTP_FILE_NOT_FOUND -7
#define TFTP_RQ_ERR   -8
#define TFTP_ACK_ERR   -9
#define TFTP_RCV_ERR  -10
#define TFTP_NOSUPPORT_OPT -11

typedef struct tftp_socket
{
    int sockfd;
    struct in_addr host_addr;
    struct in_addr server_addr;
    int host_port;
    int server_port;
}tftp_socket_t;

int tftp_get_file(char *hostname, int hostport, char *servername, 
                   char *filename, int filelen, char *buf, int bufsize);
int tftp_put_file(char *hostname, int hostport, char *servername, 
                   char *filename, int filelen, int blocksize, char *buf);

#endif /* _TFTP_CLIENT_H_ */

