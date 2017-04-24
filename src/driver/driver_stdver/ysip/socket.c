#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <sys/time.h>

#include "hi_unf_common.h"
#include "drv_os.h"
#include "gb_u16.h"

#define PACKET_LEN 2048
#define MAX_SOCKET 1024

typedef enum _tagNetSocketType
{
    NET_SOCKET_TCP,
    NET_SOCKET_UDP
} NETSOCKETTYPE;

//Default timeout = 10s
HI_U32 TimeOutSec = 10;
HI_S16 ServerPort = 6789;
HI_S16 ClientPort = 6789;
NETSOCKETTYPE SocketType = NET_SOCKET_TCP;
HI_CHAR serverip[64] = "10.73.10.75";
HI_CHAR clientip[64] = "10.73.10.70";
HI_U32 speedtest_packet_len = 1024 * 1024;
HI_U32 speedtest_loop_num = 10;
HI_U32 speedtest_units = 1024 * 1024;
HI_CHAR speedtest_units_string = 'm';
HI_BOOL server_flag = HI_FALSE;
HI_BOOL client_flag = HI_FALSE;    // socket server||client 创建标志

extern DRVOS_Partition_t *SystemPartition;

HI_S32 Net_Socket_Create(NETSOCKETTYPE NetSocketType)
{
    HI_S32 ret = HI_FAILURE;

    if (NET_SOCKET_TCP == NetSocketType)
    {
        ret = socket(AF_INET, SOCK_STREAM, 0);
        if (ret == -1)
        {
            perror("socket\n");
        }
    }
    else if (NET_SOCKET_UDP == NetSocketType)
    {
        ret = socket(AF_INET, SOCK_DGRAM, 0);
        if (ret == -1)
        {
            perror("socket\n");
        }
    }

    return ret;
}

HI_VOID Net_Socket_Destroy(HI_S32 fd)
{
    close(fd);
}

HI_S32 Net_Socket_Bind(HI_S32 sockfd, HI_U16 ipPort)
{
    HI_S32 ret = HI_SUCCESS;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ipPort);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(serv_addr.sin_zero, 8);

    ret = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (ret != HI_SUCCESS)
    {
        perror("socket bind error.\n");
    }

    return ret;
}

HI_S32 Net_Socket_Listen(HI_S32 sockfd)
{
    if (listen(sockfd, 10) == -1)
    {
        perror("socket listen error.\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 Net_Socket_Connect(HI_S32 sockfd, HI_CHAR * ipString, HI_U16 ipport, HI_U32 timeout_s)
{
    HI_S32 blockflag;
    HI_S32 status;
    HI_U32 nsec = 1, maxsec = 0;
    struct sockaddr_in serv_addr;

    if (ipString == HI_NULL)
    {
        return HI_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(ipport);
    serv_addr.sin_addr.s_addr = inet_addr(ipString);

    blockflag  = fcntl(sockfd, F_GETFL, 0);
    blockflag |= O_NONBLOCK;
    (void)fcntl(sockfd, F_SETFL, blockflag);
    do
    {
        status = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
        if (status == 0)
        {
            return HI_SUCCESS;
        }

        if (maxsec >= timeout_s)
        {
            break;
        }

        (void)sleep(nsec);
        maxsec += nsec;
        nsec <<= 1;
    } while (1);

    return HI_FAILURE;
}

HI_S32 Net_Socket_Accept(HI_S32 fd, HI_S32 *clientfd, HI_U32 timeout_ms)
{
    HI_U32 len;
    HI_S32 client;
    HI_S32 ret = HI_SUCCESS;
    fd_set acceptfd;
    struct timeval timeout_val;
    struct sockaddr_in client_addr;

    if (clientfd == HI_NULL)
    {
        return HI_FAILURE;
    }

    timeout_val.tv_sec  = timeout_ms / 1000;
    timeout_val.tv_usec = ((timeout_ms % 1000) * 1000);

    FD_ZERO(&acceptfd);      //将acceptfd初始化为空集NULL
    FD_SET(fd, &acceptfd);   //向集合acceptfd添加描述字fd

    ret = select(fd + 1, &acceptfd, NULL, NULL, &timeout_val);//确定一个套接口的状态是否可用
    if (ret == -1)
    {
        perror("select error\n");
        return HI_FAILURE;
    }
    else if (ret == 0)
    {
        return HI_FAILURE;
    }

    if (FD_ISSET(fd, &acceptfd))//若fd为集合中一员，非零；否则为零
    {
        len = sizeof(struct sockaddr_in);
        client = accept(fd, (struct sockaddr *)&client_addr, &len); //从fd的等待连接队列中抽取第一个连接
        if (client == -1)
        {
            perror("Accept Error.\n");
            return HI_FAILURE;
        }

        *clientfd = client;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 Net_Socket_Recv(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_U32 *recvlen, HI_U32 timeout_ms)
{
    HI_S32 recvbytes;
    fd_set readfd;
    struct timeval timeout_val;

    if ((buf == HI_NULL) || (recvlen == HI_NULL))
    {
        return HI_FAILURE;
    }

    timeout_val.tv_sec  = timeout_ms / 1000;
    timeout_val.tv_usec = ((timeout_ms % 1000) * 1000);
    FD_ZERO(&readfd);
    FD_SET(sockfd, &readfd);
    if (select(sockfd + 1, &readfd, NULL, NULL, &timeout_val) <= 0)
    {
        return HI_FAILURE;
    }

    if (FD_ISSET(sockfd, &readfd))
    {
        if ((recvbytes = recv(sockfd, buf, len, 0)) == -1)
        {
            perror("recv error!");
            return HI_FAILURE;
        }

        *recvlen = recvbytes;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 Net_Socket_RecvFrom(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_U32 *recvlen, HI_CHAR *ipstring, HI_S16 *port,
                           HI_U32 timeout_ms)
{
    HI_S32 recvbytes;
    fd_set readfd;
    struct timeval timeout_val;
    struct sockaddr_in send_addr;
    socklen_t alen;

    if ((buf == HI_NULL) || (recvlen == HI_NULL))
    {
        return HI_FAILURE;
    }

    timeout_val.tv_sec  = timeout_ms / 1000;
    timeout_val.tv_usec = ((timeout_ms % 1000) * 1000);
    FD_ZERO(&readfd);
    FD_SET(sockfd, &readfd);
    if (select(sockfd + 1, &readfd, NULL, NULL, &timeout_val) <= 0)
    {
        return HI_FAILURE;
    }

    if (FD_ISSET(sockfd, &readfd))
    {
        alen = sizeof(struct sockaddr);
        if ((recvbytes = recvfrom(sockfd, buf, len, 0, (struct sockaddr *)&send_addr, &alen)) == -1)
        {
            perror("recv error");
            return HI_FAILURE;
        }

        if ((ipstring != HI_NULL) && (port != HI_NULL))
        {
            printf("recv form ip = %s port = %d success recvlen = %d \n", inet_ntoa(send_addr.sin_addr),
                   ntohs(send_addr.sin_port), recvbytes);
            strcpy(ipstring, inet_ntoa(send_addr.sin_addr));
            *port = ntohs(send_addr.sin_port);
        }

        *recvlen = recvbytes;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 Net_Socket_Send(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_U32 *sendlen, HI_U32 timeout_ms)
{
    HI_S32 sendbytes;
    fd_set writefd;
    struct timeval timeout_val;

    if ((buf == HI_NULL) || (sendlen == HI_NULL))
    {
        return HI_FAILURE;
    }

    timeout_val.tv_sec  = timeout_ms / 1000;
    timeout_val.tv_usec = (timeout_ms % 1000) * 1000;
    FD_ZERO(&writefd);
    FD_SET(sockfd, &writefd);
    if (select(sockfd + 1, NULL, &writefd, NULL, &timeout_val) <= 0)
    {
        perror("write error\n");
        return HI_FAILURE;
    }

    if (FD_ISSET(sockfd, &writefd))
    {
        sendbytes = send(sockfd, buf, len, 0);
        if (sendbytes == -1)
        {
            perror("send error");
            return HI_FAILURE;
        }

        *sendlen = sendbytes;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_S32 Net_Socket_SendTo(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_U32 *sendlen, HI_CHAR *ipString, HI_S16 ipport,
                         HI_U32 timeout_ms)
{
    HI_S32 sendbytes;
    fd_set writefd;
    struct timeval timeout_val;
    struct sockaddr_in recv_addr;

    if ((buf == HI_NULL) || (sendlen == HI_NULL))
    {
        return HI_FAILURE;
    }

    timeout_val.tv_sec  = timeout_ms / 1000;
    timeout_val.tv_usec = (timeout_ms % 1000) * 1000;
    FD_ZERO(&writefd);
    FD_SET(sockfd, &writefd);
    if (select(sockfd + 1, NULL, &writefd, NULL, &timeout_val) <= 0)
    {
        perror("write error\n");
        return HI_FAILURE;
    }

    if (FD_ISSET(sockfd, &writefd))
    {
        recv_addr.sin_family = AF_INET;
        recv_addr.sin_port = htons(ipport);
        recv_addr.sin_addr.s_addr = inet_addr(ipString);

        sendbytes = sendto(sockfd, buf, len, 0, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr));
        if (sendbytes == -1)
        {
            perror("send error!");
            return HI_FAILURE;
        }

        *sendlen = sendbytes;
        return HI_SUCCESS;
    }

    return HI_FAILURE;
}

HI_CHAR * Net_Socket_GetSocketName(HI_S32 sockfd)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CHAR *ipstr;
    struct sockaddr_in client_addr;
    socklen_t alen;

    alen = sizeof(struct sockaddr);
    ret = getsockname(sockfd, (struct sockaddr *)&client_addr, &alen);
    if (ret == -1)
    {
        perror("getsockname error\n");
        return HI_NULL;
    }

    ipstr = inet_ntoa(client_addr.sin_addr);

    return ipstr;
}

HI_CHAR * Net_Socket_GetPeerName(HI_S32 sockfd)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CHAR *ipstr;
    struct sockaddr_in client_addr;
    socklen_t alen;
    
    alen = sizeof(struct sockaddr);
    ret = getpeername(sockfd, (struct sockaddr *)&client_addr, &alen);
    if (ret == -1)
    {
        perror("getpeername error\n");
        return HI_NULL;
    }

    ipstr = inet_ntoa(client_addr.sin_addr);
    
    return ipstr;
}


HI_S32 Net_Send(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_CHAR *ipString, HI_S16 ipport)
{
    HI_S32 ret;
    HI_U32 errornum;
    HI_U32 sendbufflen;
    HI_U32 sendbufflen_act;
    HI_CHAR *psend = buf;

    if (buf == HI_NULL)
    {
        return HI_FAILURE;
    }

    if ((ipString == HI_NULL) && (SocketType == NET_SOCKET_UDP))
    {
        return HI_FAILURE;
    }

    errornum = 10;
    sendbufflen = len;
    while (sendbufflen)
    {
        if (SocketType == NET_SOCKET_TCP)
        {
            ret = Net_Socket_Send(sockfd, psend, sendbufflen, &sendbufflen_act, 2000);
        }
        else
        {
            ret = Net_Socket_SendTo(sockfd, psend, sendbufflen, &sendbufflen_act, ipString, ipport, 2000);
        }

        if (ret == HI_SUCCESS)
        {
            psend = psend + sendbufflen_act;
            sendbufflen = sendbufflen - sendbufflen_act;
        }
        else
        {
            errornum--;
            if (!errornum)
            {
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 Net_Recv(HI_S32 sockfd, HI_CHAR *buf, HI_U32 len, HI_CHAR *ipString, HI_S16 *ipport)
{
    HI_S32 ret;
    HI_U32 errornum;
    HI_U32 recvbufflen;
    HI_U32 recvlen_act;
    HI_CHAR *psend = buf;

    if (buf == HI_NULL)
    {
        return HI_FAILURE;
    }

    errornum = 10;
    recvbufflen = len;
    while (recvbufflen)
    {
        if (SocketType == NET_SOCKET_TCP)
        {
            ret = Net_Socket_Recv(sockfd, psend, recvbufflen, &recvlen_act, 2000);
        }
        else
        {
            ret = Net_Socket_RecvFrom(sockfd, psend, recvbufflen, &recvlen_act, ipString, ipport, 2000);
        }

        if (ret == HI_SUCCESS)
        {
            psend = psend + recvlen_act;
            recvbufflen = recvbufflen - recvlen_act;
        }
        else
        {
            errornum--;
            if (!errornum)
            {
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 Net_Upload(HI_S32 sockfd, HI_CHAR *filepath)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CHAR cmdflag[16];
    HI_CHAR downloadbuff[PACKET_LEN];
    HI_BOOL bfile = HI_FALSE;
    HI_U32 msecbegin, msecend, msecall, msecall_total;
    HI_U32 len   = 0, send_len = 0;
    HI_U32 index = 0;
    HI_U32 maxindex = speedtest_loop_num;
    HI_U32 speed_integar, speed_decimal;
    HI_U64 speed;
    HI_S32 fd;
    off_t fd_len, fd_send_len;

    struct timeval tm;

    if (SocketType == NET_SOCKET_TCP)
    {
        strcpy(cmdflag, "TCP");
    }
    else
    {
        strcpy(cmdflag, "UDP");
    }

    //if (strncmp(filepath, "none", 4) != 0)
    {
        bfile = HI_TRUE;
    }

    index = 0;

    if (bfile)
    {
        fd = open(filepath, O_RDONLY);
        if (fd == -1)
        {
            printf("open file %s error\n", filepath);
            return HI_FAILURE;
        }

        fd_len = lseek(fd, 0, SEEK_END);
        printf("Server %s Upload File %s ,File len %lu\n", cmdflag, filepath, fd_len);

        //if (SocketType == NET_SOCKET_TCP)
        //{
        //    ret = Net_Send(sockfd, (HI_CHAR*)&fd_len, sizeof(off_t), HI_NULL, 0);
        //}
        //else
        //{
        //    ret = Net_Send(sockfd, (HI_CHAR*)&fd_len, sizeof(off_t), clientip, ClientPort);
        //}

        lseek(fd, 0, SEEK_SET);
        len = 0;
        fd_send_len   = 0;
        msecall_total = 0;
        gettimeofday(&tm, HI_NULL);
        msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;
        while (fd_send_len < fd_len)
        {
            send_len = read(fd, downloadbuff, PACKET_LEN);
            if (send_len <= 0)
            {
                break;
            }

            if (SocketType == NET_SOCKET_TCP)
            {
                ret = Net_Send(sockfd, downloadbuff, send_len, HI_NULL, 0);
            }
            else
            {
                ret = Net_Send(sockfd, downloadbuff, send_len, clientip, ClientPort);
            }

            if (ret != HI_SUCCESS)
            {
                printf("UpLoad Send Data error \n");
                break;
            }

            fd_send_len = fd_send_len + send_len;

            len = len + send_len;

            //calculate speed when have send 1M data
            if (len >= speedtest_packet_len)
            {
                gettimeofday(&tm, HI_NULL);
                msecend = tm.tv_sec * 1000 + tm.tv_usec / 1000;
                msecall = msecend - msecbegin;
                msecall_total = msecall_total + msecall;

                speed = len;
                speed = speed * 1000 / msecall; //b/s
                speed_decimal = speed % speedtest_units;
                speed_decimal = speed_decimal * 100 / speedtest_units;
                speed_integar = speed / speedtest_units;
                printf("[FILE] send %lu%c  time = %dms , speed %d.%d%c/s \n",
                       fd_send_len / speedtest_units, speedtest_units_string, msecall,
                       speed_integar, speed_decimal, speedtest_units_string);
                len = 0;
                index++;

                gettimeofday(&tm, HI_NULL);
                msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;
            }
        }

        close(fd);
        
#if 0        
        gettimeofday(&tm, HI_NULL);
        msecend = tm.tv_sec * 1000 + tm.tv_usec / 1000;
        msecall = msecend - msecbegin;
        msecall_total = msecall_total + msecall;

        printf("===========>>>>>>>>[DRV_SOCKET]msecall_total = %d\n",msecall_total);
        speed = fd_send_len;
        speed = speed * 1000 / msecall_total; //b/s

        speed_decimal = speed % speedtest_units;
        speed_decimal = speed_decimal * 100 / speedtest_units;
        speed_integar = speed / speedtest_units;

        printf("Server %s Upload send %lu%c time = %dms ,average speed %d.%d%c/s\n", cmdflag,
               fd_send_len / speedtest_units, speedtest_units_string, msecall_total,
               speed_integar, speed_decimal, speedtest_units_string);
#endif
    }
    else
    {
        printf("Server %s Upload PacketLen %dk,LoopNum %d\n", cmdflag,
               speedtest_packet_len / 1024,
               speedtest_loop_num);

        memset(downloadbuff, 0xaa, PACKET_LEN);

        len = 0;
        msecall_total = 0;
        gettimeofday(&tm, HI_NULL);
        msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;

        while (maxindex > index)
        {
            if (SocketType == NET_SOCKET_TCP)
            {
                ret = Net_Send(sockfd, downloadbuff, PACKET_LEN, HI_NULL, 0);
            }
            else
            {
                ret = Net_Send(sockfd, downloadbuff, PACKET_LEN, clientip, ClientPort);
            }

            if (ret != HI_SUCCESS)
            {
                printf("UpLoad Send Data error \n");
                return HI_FAILURE;
            }

            len = len + PACKET_LEN;
            if (len >= speedtest_packet_len)
            {
                gettimeofday(&tm, HI_NULL);
                msecend = tm.tv_sec * 1000 + tm.tv_usec / 1000;
                msecall = msecend - msecbegin;
                msecall_total = msecall_total + msecall;

                speed = len;
                speed = speed * 1000 / msecall; //b/s
                speed_decimal = speed % speedtest_units;
                speed_decimal = speed_decimal * 100 / speedtest_units;
                speed_integar = speed / speedtest_units;
                printf("[%d] send %d%c  time = %dms , speed %d.%d%c/s \n", index,
                       len / speedtest_units, speedtest_units_string, msecall,
                       speed_integar, speed_decimal, speedtest_units_string);

                len = 0;
                index++;
                if (SocketType == NET_SOCKET_UDP)
                {
                    sleep(1);
                }

                gettimeofday(&tm, HI_NULL);
                msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;
            }
        }

        speed = speedtest_packet_len * speedtest_loop_num;
        speed = speed * 1000 / msecall_total; //b/s

        speed_decimal = speed % speedtest_units;
        speed_decimal = speed_decimal * 100 / speedtest_units;
        speed_integar = speed / speedtest_units;

        printf("Server %s Upload send %d%c time = %dms ,average speed %d.%d%c/s\n", cmdflag,
               speedtest_packet_len * speedtest_loop_num / speedtest_units, speedtest_units_string, msecall_total,
               speed_integar, speed_decimal, speedtest_units_string);
    }

    return HI_SUCCESS;
}

HI_S32 Net_Download(HI_S32 sockfd, HI_CHAR *filepath)
{
    HI_S32 ret = HI_SUCCESS;
    HI_CHAR cmdflag[16];
    HI_CHAR downloadbuff[PACKET_LEN];
    HI_BOOL bfile = HI_FALSE;
    HI_U32 msecbegin, msecend, msecall, msecall_total;
    HI_U32 len   = 0, recv_len;
    HI_U32 index = 0;
    HI_U32 maxindex = speedtest_loop_num;
    HI_U32 speed_integar, speed_decimal;
    HI_U64 speed;
    HI_S32 fd;
    off_t fd_len, fd_recv_len;

    struct timeval tm;

    if (SocketType == NET_SOCKET_TCP)
    {
        strcpy(cmdflag, "TCP");
    }
    else
    {
        strcpy(cmdflag, "UDP");
    }

    printf("---------------------------%s---------------------------------\n", filepath);

    if (strncmp(filepath, "none", 4) != 0)
    {
        bfile = HI_TRUE;
    }

    index = 0;
    if (bfile)
    {
        fd = open(filepath, O_WRONLY | O_CREAT);
        if (fd == -1)
        {
            printf("open file %s error\n", filepath);
            return HI_FAILURE;
        }

        ret = Net_Recv(sockfd, (HI_CHAR *)&fd_len, sizeof(off_t), HI_NULL, HI_NULL);

        printf("Server %s Download File %s ,File len %lu\n", cmdflag, filepath, fd_len);
        len = 0;
        msecall_total = 0;
        fd_recv_len = 0;
        gettimeofday(&tm, HI_NULL);
        msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;
        while (fd_recv_len < fd_len)
        {
            if (fd_len - fd_recv_len >= PACKET_LEN)
            {
                recv_len = PACKET_LEN;
            }
            else
            {
                recv_len = fd_len - fd_recv_len;
            }

            ret = Net_Recv(sockfd, downloadbuff, recv_len, HI_NULL, HI_NULL);
            if (ret != HI_SUCCESS)
            {
                printf("Recv Data error \n");
                break;
            }

            if (recv_len != write(fd, downloadbuff, recv_len))
            {
                printf("write to file len error \n");
            }

            fd_recv_len = fd_recv_len + recv_len;
            len = len + recv_len;
            if (len >= speedtest_packet_len)
            {
                gettimeofday(&tm, HI_NULL);
                msecend   = tm.tv_sec * 1000 + tm.tv_usec / 1000;
                msecall   = msecend - msecbegin;
                msecbegin = msecend;
                msecall_total = msecall_total + msecall;

                speed = len;
                speed = speed * 1000 / msecall; //b/s
                speed_decimal = speed % speedtest_units;
                speed_decimal = speed_decimal * 100 / speedtest_units;
                speed_integar = speed / speedtest_units;
                printf("[FILE] recv %lu%c  time = %dms , speed %d.%d%c/s \n",
                       fd_recv_len / speedtest_units, speedtest_units_string, msecall,
                       speed_integar, speed_decimal, speedtest_units_string);

                len = 0;
                index++;
            }
        }

        gettimeofday(&tm, HI_NULL);
        msecend = tm.tv_sec * 1000 + tm.tv_usec / 1000;
        msecall = msecend - msecbegin;
        msecall_total = msecall_total + msecall;

        close(fd);

        speed = speedtest_packet_len * speedtest_loop_num;
        speed = speed * 1000 / msecall_total; //b/s

        speed_decimal = speed % speedtest_units;
        speed_decimal = speed_decimal * 100 / speedtest_units;
        speed_integar = speed / speedtest_units;

        printf("Server %s DownLoad recv %lu%c time = %dms ,average speed %d.%d%c/s\n", cmdflag,
               fd_recv_len / speedtest_units, speedtest_units_string, msecall_total,
               speed_integar, speed_decimal, speedtest_units_string);
        printf("------------------------------------------------------------\n");
    }
    else
    {
        printf("Server %s DownLoad PacketLen %dk,LoopNum %d\n", cmdflag,
               speedtest_packet_len / 1024,
               speedtest_loop_num);

        memset(downloadbuff, 0xff, PACKET_LEN);

        len = 0;
        msecall_total = 0;
        gettimeofday(&tm, HI_NULL);
        msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;

        while (maxindex > index)
        {
            ret = Net_Recv(sockfd, downloadbuff, PACKET_LEN, HI_NULL, HI_NULL);
            if (ret != HI_SUCCESS)
            {
                printf("Recv Data error \n");
                return HI_FAILURE;
            }

            len = len + PACKET_LEN;
            if (len >= speedtest_packet_len)
            {
                gettimeofday(&tm, HI_NULL);
                msecend = tm.tv_sec * 1000 + tm.tv_usec / 1000;
                msecall = msecend - msecbegin;
                msecall_total = msecall_total + msecall;

                speed = len;
                speed = speed * 1000 / msecall; //b/s
                speed_decimal = speed % speedtest_units;
                speed_decimal = speed_decimal * 100 / speedtest_units;
                speed_integar = speed / speedtest_units;
                printf("[%d] recv %d%c  time = %dms , speed %d.%d%c/s \n", index,
                       len / speedtest_units, speedtest_units_string, msecall,
                       speed_integar, speed_decimal, speedtest_units_string);

                len = 0;
                index++;
                gettimeofday(&tm, HI_NULL);
                msecbegin = tm.tv_sec * 1000 + tm.tv_usec / 1000;
            }
        }

        speed = speedtest_packet_len * speedtest_loop_num;
        speed = speed * 1000 / msecall_total; //b/s

        speed_decimal = speed % speedtest_units;
        speed_decimal = speed_decimal * 100 / speedtest_units;
        speed_integar = speed / speedtest_units;

        printf("Server %s Upload recv %d%c time = %dms ,average speed %d.%d%c/s\n", cmdflag,
               speedtest_packet_len * speedtest_loop_num / speedtest_units, speedtest_units_string, msecall_total,
               speed_integar, speed_decimal, speedtest_units_string);
        printf("------------------------------------------------------------\n");
    }

    return HI_SUCCESS;
}

HI_VOID TCP_Server(HI_S32 argc)
{
    HI_S32 ret;
    HI_CHAR recvbuff[PACKET_LEN];
    HI_U32 recvlen;
    HI_S32 clientsocket;
    HI_CHAR filename[] = "/FlashData/Channel_list.xml";
    HI_S32 temp,i;

    clientsocket = (HI_S32)argc;
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@@@@@@@@@@@@@@TCP Socket %d Recv begin @@@@@@@@@@@@@\n", clientsocket);
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");

    while (1)
    {
        //Recognize the command head
        ret = Net_Recv(clientsocket, recvbuff, 1, HI_NULL, HI_NULL);
        if (ret != HI_SUCCESS)
        {
            continue;
        }

        if (recvbuff[0] != '$')
        {
            continue;
        }

        ret = Net_Recv(clientsocket, recvbuff, 3, HI_NULL, HI_NULL);
        if (ret != HI_SUCCESS)
        {
            continue;
        }

        if (recvbuff[2] != '$')
        {
            continue;
        }

        //Get command length
        recvlen = recvbuff[1] << 8;
        recvlen = recvlen + recvbuff[0];
        if (recvlen > PACKET_LEN)
        {
            continue;
        }

        memset(recvbuff, 0, PACKET_LEN);
        ret = Net_Recv(clientsocket, recvbuff, recvlen, HI_NULL, HI_NULL);
        if (ret != HI_SUCCESS)
        {
            printf("recv sockfd %d error \n", clientsocket);
            return;
        }

        printf("recv cmd:%s\n", recvbuff);
        if (strncmp(recvbuff, "close", 5) == 0)
        {
            break;
        }
        else if (strstr(recvbuff, "get_channel_list") != 0)//获取频道列表命令
        {
            DRV_Socket_CreateXML();
            Net_Upload(clientsocket, filename);
        }
        else if (strstr(recvbuff, "change_channel") != 0)//获取切台命令
        {
            temp = 0;
            for(i = 15;i < recvlen;i ++)
            {
                 temp = temp * 10 + (recvbuff[i] - '0');
            }
            DTV_ChangeChannelMsg(1,temp);
        }
        else 
        {
            printf("[DRV_SOCKET]Net_Recv Cmd Error!!!\n");
        }
    }

    Net_Socket_Destroy(clientsocket);

    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
    printf("@@@@@@@@@@@@@@@@TCP Socket %d close @@@@@@@@@@@@@@@\n", clientsocket);
    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
}

HI_VOID UDP_Server(HI_S32 sockfd)
{
    HI_S32 ret;
    HI_CHAR recvbuff[PACKET_LEN];
    HI_U32 recvlen;

    printf("UDP socket %d recv begin \n", sockfd);

    while (1)
    {
        //Recognize the command head
        ret = Net_Recv(sockfd, recvbuff, 4, clientip, &ClientPort);
        if (ret != HI_SUCCESS)
        {
            continue;
        }

        if ((recvbuff[0] != '$') || (recvbuff[3] != '$'))
        {
            continue;
        }

        //Get command length
        recvlen = recvbuff[2] << 8;
        recvlen = recvlen + recvbuff[1];
        if (recvlen > PACKET_LEN)
        {
            continue;
        }

        memset(recvbuff, 0, PACKET_LEN);
        ret = Net_Recv(sockfd, recvbuff, recvlen, clientip, &ClientPort);
        if (ret != HI_SUCCESS)
        {
            continue;
        }

        printf("%s\n", recvbuff);
        if (strncmp(recvbuff, "close", 5) == 0)
        {
            break;
        }
        else if (strncmp(recvbuff, "speed", 5) == 0)
        {
            Net_Upload(sockfd, recvbuff + 6);
        }
        else if (strncmp(recvbuff, "down", 4) == 0)
        {
            printf("udp no support down test \n");
        }
    }
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_GBcode2UTF8
  Description     : GB2312字符转换成UTF8字符
  Input           : GBcode:源字符指针；
  Outut           : pOutUTF8:目标字符指针 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
static unsigned short drv_GBcode2UTF8(unsigned char *GBcode,unsigned char *pOutUTF8)  
{  
	unsigned short AscOdd,AscEven,ret;
	unsigned short rec,qm,wm;
	unsigned short unicode16;

	if((NULL == GBcode)||(NULL == pOutUTF8))
	{
		return 0;
	}
	
	AscOdd = GBcode[0];
	if(AscOdd & 0x80)
	{ 
		AscEven = GBcode[1];
		if(AscEven & 0x80)
		{
			rec=0;
			if(AscEven >0x00a0 && AscOdd > 0x00a0)
			{
				qm=(AscOdd - 0x00a1) & 0x7f;
				wm=(AscEven - 0x00a1) & 0x7f;
				rec = (qm*94 + wm) ;
				if(rec >=8178)//GB2312只有8178个字符
					rec=0;
			}
		}
		else
		{
			rec=0;
		}
		unicode16 = gb2312_U16_Table[rec];
	}
	else
	{
		unicode16 = AscOdd;
	}

	if(unicode16 < 0x80)
	{
		ret = 1;
		pOutUTF8[0] = unicode16;
		pOutUTF8[1] = 0;
		pOutUTF8[2] = 0;
	}
	else if(unicode16 < 0x800)
	{
		ret = 2;
		AscOdd = unicode16>>6;
		AscEven = unicode16&0x3f;
		pOutUTF8[0] = 0xc0 | (AscOdd&0x1f);
		pOutUTF8[1] = 0x80 | AscEven;
		pOutUTF8[2] = 0;
	}
	else
	{
		ret = 3;
		AscOdd = (unicode16>>12)&0xf;
		AscEven = (unicode16>>6)&0x3f;
		rec = unicode16&0x3f;
		
		pOutUTF8[0] = 0xe0 | AscOdd;
		pOutUTF8[1] = 0x80 | AscEven;
		pOutUTF8[2] = 0x80 | rec;
	}
	
	return ret;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_StrGB2UTF8
  Description     : GB2312字符串转换成UTF8字符串
  Input           : strSourcer:源字符串指针；n:字符串长度
  Outut           : strDest:目标字符串指针 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
static int drv_StrGB2UTF8(const char *strSourcer,const char *strDest,int n)  
{  
	char cTmp;  
	unsigned short hz,tmphz; 
	unsigned short Changeret = 0;

	unsigned char *pSrc;  
	unsigned char *pDest;  

	int i = 0;

	pSrc = strSourcer;
	pDest = strDest;

	while(n > 0)
	{
		Changeret = drv_GBcode2UTF8(pSrc,pDest);

		if(0 == Changeret)
		{
			return 0;
		}
		else if(1 == Changeret)
		{
			pSrc ++;
			n --;
		}
		else
		{
			pSrc += 2;
			n -= 2;
		}
		
		pDest += Changeret;
		i += Changeret;
	}
    
	*pDest='\0';
	
	return i;  
	
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_Socket_CreateXML
  Description     : 把频道列表转换成XML格式的文件
  Input           : strsrc:频道列表字符串指针；StrLength:字符串长度
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
HI_S32 drv_Socket_CreateXML(HI_CHAR *strsrc,HI_S32 StrLength)
{
    char ChannelID[3];          /*ChannelID */
    char ChannelName[60];       /*ChannelName */ /*数据结构channelID[3] ChannelName[20]\n*/
    char ChannelNameSrc[60];    /*ChannelNameSrc */
    int pos0=0;                 /*ChannelID string pos */ 
    int posx=0;                 /*ChannelName string pos*/
    FILE* fp_xml = NULL;

    if((NULL == strsrc)||(StrLength <= 0))
    {
        printf("Channel Param Error!\n");
        return HI_FAILURE;
    }

    fp_xml = fopen("/FlashData/Channel_list.xml", "w+");
    if(NULL == fp_xml)
    {
        printf("open Channel_list.xml Error!\n");
        return HI_FAILURE;
    }
    
    printf("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    printf(" <channellist>\n");
    fprintf(fp_xml,"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    fprintf(fp_xml," <channellist>\n");
 
    do 
    {      
        if(strchr(strsrc,'\n') == NULL)                 /*can not find '\n':end of index string*/
        {
            break;
        } 
        
        pos0 = strchr(strsrc,'\n') - strsrc;            /*ChannelID string 分隔符指针偏移值 */       
        posx = 4;                                       /*ChannelName string 指针偏移值*/   
        
        strncpy(ChannelID,strsrc,3);                    /*ChannelID string cpy*/ 
        strncpy(ChannelNameSrc,&strsrc[posx],pos0 - 4); /*ChannelName string cpy*/ 

        drv_StrGB2UTF8(ChannelNameSrc,ChannelName,pos0 - 4); /*GB2312字符转换成UTF8*/

        /*--------------format to output-----------*/    
        printf("    <channel>\n");
        printf("        <id>%s</id>\n",ChannelID);
        printf("        <name>%s</name>\n",ChannelName);
        printf("    </channel>\n");
        fprintf(fp_xml,"    <channel>\n");
        fprintf(fp_xml,"        <id>%s</id>\n",ChannelID);
        fprintf(fp_xml,"        <name>%s</name>\n",ChannelName);
        fprintf(fp_xml,"    </channel>\n");
        
        strsrc = strchr(strsrc,'\n') + 1;               /*指向下一个频道ID*/     
        StrLength = StrLength - pos0 - 1;
        
        if(strsrc == NULL)
        {
            printf("Channel cpy over!\n");
            break;
        }
        
        if(0 >= StrLength)
        {
            break;
        }
        
        memset(ChannelID,0,3);
        memset(ChannelName,0,pos0);
        memset(ChannelNameSrc,0,pos0);

    }while(1) ; 
    
    printf(" </channellist>\n");
    fprintf(fp_xml," </channellist>\n");
    fclose(fp_xml);

    return HI_SUCCESS;
} 

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_SocketServerTask
  Description     : socket server任务
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
HI_S32 drv_SocketServerTask()
{
    HI_S32 ret;
    HI_S32 serversocket;
    HI_S32 clientsocket;

    serversocket = Net_Socket_Create(SocketType);
    if (serversocket == HI_FAILURE)
    {
        printf("Create Socket Failure\n");
        return HI_FAILURE;
    }

    ret = Net_Socket_Bind(serversocket, ServerPort);
    if (ret != HI_SUCCESS)
    {
        printf("Bind Socket Failure\n");
        return HI_FAILURE;
    }

    if (SocketType == NET_SOCKET_TCP)
    {
        ret = Net_Socket_Listen(serversocket);
        if (ret != HI_SUCCESS)
        {
            printf("Listen Socket Failure\n");
            return HI_FAILURE;
        }

        printf("------------------------------------------------------------\n");
        printf("Server listening on TCP port %d\n", ServerPort);
        printf("------------------------------------------------------------\n");

        while (1)
        {
            ret = Net_Socket_Accept(serversocket, &clientsocket, 2000);
            if (ret != HI_SUCCESS)
            {
                continue;
            }

            printf("socket %d server ip = %s\n", clientsocket, Net_Socket_GetSocketName(clientsocket));
            printf("socket %d client ip = %s\n", clientsocket, Net_Socket_GetPeerName(clientsocket));

            TCP_Server(clientsocket);
            printf("------------------------------------------------------------\n");
            printf("Server listening on TCP port %d\n", ServerPort);
            printf("------------------------------------------------------------\n");
        }
    }
    else
    {
        printf("------------------------------------------------------------\n");
        printf("Server listening on UDP port %d\n", ServerPort);
        printf("------------------------------------------------------------\n");
        while (1)
        {
            UDP_Server(serversocket);
        }
    }

    Net_Socket_Destroy(serversocket);
    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_SocketClient
  Description     : socket Client 通信函数，带参数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
HI_S32 drv_SocketClient_Cmd(HI_S32 sockfd, HI_CHAR *cmdline)
{
    HI_U32 cmdlen = 0;
    HI_CHAR cmdhead[4];

    cmdlen = strlen(cmdline);
    cmdlen = cmdlen;
    cmdline[cmdlen - 1] = 0;
    cmdhead[0] = '$';
    cmdhead[1] = cmdlen & 0xff;
    cmdhead[1] = cmdhead[1] - 1;
    cmdhead[2] = (cmdlen >> 8) & 0xff;
    cmdhead[3] = '$';
    Net_Send(sockfd, cmdhead, 4, serverip, ServerPort);
    Net_Send(sockfd, cmdline, cmdlen, serverip, ServerPort);
    if (strncmp(cmdline, "speed", 5) == 0)
    {
        Net_Download(sockfd, cmdline + 6);
    }

    if (strncmp(cmdline, "down", 4) == 0)
    {
        if (SocketType == NET_SOCKET_TCP)
        {
            Net_Download(sockfd, cmdline + 5);
        }
        else
        {
            printf("udp no support down test \n");
        }
    }
    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : drv_SocketClient
  Description     : socket Client 通信函数
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 
*****************************************************************************/
HI_S32 drv_SocketClient(HI_CHAR *cmdline)
{
    HI_S32 i;
    HI_S32 ret;
    HI_CHAR sendbuff[PACKET_LEN];
    HI_CHAR cmdflag[16];
    HI_S32 clientsocket;

    clientsocket = Net_Socket_Create(SocketType);
    if (clientsocket == HI_FAILURE)
    {
        printf("Create Socket Failure\n");
        return HI_FAILURE;
    }

    if (SocketType == NET_SOCKET_TCP)
    {
        for (i = 0; i < 3; i++)
        {
            ret = Net_Socket_Connect(clientsocket, serverip, ServerPort, 32);
            if (ret == HI_SUCCESS)
            {
                break;
            }
        }

        if (i == 3)
        {
            printf("------------------------------------------------------------\n");
            printf("Client Connecting on TCP ip %s port %d Failure\n", serverip, ServerPort);
            printf("------------------------------------------------------------\n");

            return HI_FAILURE;
        }
        else
        {
            printf("------------------------------------------------------------\n");
            printf("Client Connecting on TCP ip %s port %d Success\n", serverip, ServerPort);
            printf("------------------------------------------------------------\n");
        }

        Net_Socket_GetPeerName(clientsocket);
        strcpy(cmdflag, "TCP#");
    }
    else
    {
        printf("------------------------------------------------------------\n");
        printf("Client Connecting on UDP port %d Success\n", ServerPort);
        printf("------------------------------------------------------------\n");
        strcpy(cmdflag, "UDP#");
    }

    if (cmdline == HI_NULL)
    {
        for (;;)
        {
            printf("%s ", cmdflag);
            memset(sendbuff, 0, PACKET_LEN);
            fgets(sendbuff, PACKET_LEN, stdin);
            drv_SocketClient_Cmd(clientsocket, sendbuff);
            if (strncmp(sendbuff, "close", 5) == 0)
            {
                break;
            }
        }
    }
    else
    {
        drv_SocketClient_Cmd(clientsocket, cmdline);
    }

    Net_Socket_Destroy(clientsocket);

    printf("---------------------------------Client END----------------------------\n");
    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : DRV_Socket_CreateXML
  Description     : 获取目前所有的频道列表并转换成XML格式的文件
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 创建的XML文件保存在/FlashData/Channel_list.xml
*****************************************************************************/
HI_S32 DRV_Socket_CreateXML(void)
{
    HI_U8  *pChannelList = NULL;
    HI_S32 siDataLength = 18000;
    HI_S32 GetStringLength;
    HI_S32 Ret;

    pChannelList = (HI_U8 *)DRV_OSMalloc(SystemPartition,siDataLength);
    memset(pChannelList,0,siDataLength);
    
    STBDATA_GetTVChannelListInString(pChannelList,siDataLength,&GetStringLength);
    drv_Socket_CreateXML(pChannelList,siDataLength);

    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : DRV_Socket_CreateServer
  Description     : 创建Socket Server任务
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 支持TCP和UDP
*****************************************************************************/
HI_S32 DRV_Socket_CreateServer(HI_S16 Type,HI_S16 Port)
{
    DRVOS_Task_t *pTunerTaskid;

    //设置socket通信方式，TCP/UDP
    if(0 == Type)
    {
        SocketType = NET_SOCKET_TCP;
    }
    else if(1 == Type)
    {
        SocketType = NET_SOCKET_UDP;
    }
    else
    {
        return HI_FAILURE;
    }

    ServerPort = Port; //设置server端口号

    if (server_flag == HI_TRUE)
    {
        printf("socket server exist!!!");
        return HI_FAILURE;
    }

    //创建Socket server 任务
	pTunerTaskid = DRV_OS_TaskCreate("drv_SocketServerTask",drv_SocketServerTask,200,1024*16,NULL,0);
    if (pTunerTaskid == NULL)
    {
        printf("%s: %d Error\n", __FILE__, __LINE__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/*****************************************************************************
  Date&Time       : 2012-09-13
  Author          : wrwei create
  Function Name   : DRV_Socket_CreateClient
  Description     : 创建Socket Client
  Input           : 
  Outut           : 
  Calls           : 
  Called by       :
  Global Variables: 
  Others          : 支持TCP和UDP
*****************************************************************************/
HI_S32 DRV_Socket_CreateClient(HI_S16 Type,HI_CHAR *IP,HI_U8 Speed_flag)
{
    //设置socket通信方式，TCP/UDP
    if(0 == Type)
    {
        SocketType = NET_SOCKET_TCP;
    }
    else if(1 == Type)
    {
        SocketType = NET_SOCKET_UDP;
    }
    else
    {
        return HI_FAILURE;
    }

    //serverip = IP; //设置server IP
    //strncpy(serverip, IP, 12);
    printf("==++>>>>strncpy IP: %s\n",strncpy(serverip, IP, strlen(IP)));

    if (client_flag == HI_TRUE)
    {
        printf("socket server exist!!!");
        return HI_FAILURE;
    }

    if (Speed_flag == HI_TRUE)
    {
        drv_SocketClient("speed none");
    }
    else
    {
        drv_SocketClient(HI_NULL);
    }

    return HI_SUCCESS;
}

