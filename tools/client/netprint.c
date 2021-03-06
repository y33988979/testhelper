
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  /* netdb is necessary for struct hostent */

#define PORT 7078  /* server port */

#define MAXDATASIZE 128
static char gIpAddr[16] = {"192.168.30.188"};

int main(int argc, char *argv[])
{
    int i, count;
    int sockfd, num;    /* files descriptors */
    char buf[MAXDATASIZE];    /* buf will store received text */
    struct hostent *he;    /* structure that will get information about remote host */
    struct sockaddr_in server;
    
    if (argc != 2)
    {
        printf("Usage: %s <IP Address>\n",argv[0]);
        exit(1);
    }
    
    if((he=gethostbyname(argv[1]))==NULL)
    {
        printf("gethostbyname() error\n");
        exit(1);
    }
    
	strcpy(gIpAddr, argv[1]);
    if((sockfd=socket(AF_INET,SOCK_STREAM, 0))==-1)
    {
        printf("socket() error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    //server.sin_addr = *((struct in_addr *)he->h_addr);
    server.sin_addr.s_addr = inet_addr(gIpAddr);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))==-1)
    {
        perror("connect() error");
        exit(1);
    }

	while(1)
	{
		count=0;
		if((num=recv(sockfd,buf,MAXDATASIZE,0))<=0)
		{
			close(sockfd);
			printf("recv() error\n");
			exit(1);
		}
		buf[num]='\0';
		for(i=0; i<num; i++)
		{
			if(buf[i] == 0x07)
				count++;
		}
		printf("server message: len=%d, count=%d %s\n",num, count, buf);
	}
	close(sockfd);
    
    return 0;
}
