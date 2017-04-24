/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <hlp_core.h>
#include <hlp_config.h>
#include <dirent.h>
#include <sys/un.h>
#include <hlp_uart_command.h>

#define UNIX_DOMAIN "/tmp/UNIX.domain"


void do_scandir(char *path)
{
	struct dirent **namelist;
	int n;

	n = scandir(path, &namelist, 0, alphasort);
	if (n < 0)
		perror("scandir error: ");
	else 
	{
		while (n--) 
		{
			//printf("%s<br>", namelist[n]->d_name);
			printf("<a href=http://192.168.30.188/logs/%s><font color=blue>%s</font></a><br>",\
			namelist[n]->d_name, namelist[n]->d_name);
			free(namelist[n]);
		}
		free(namelist);
	}
}

int uart_command_init(UartCommand *pCmd)
{
	hlp_u8_t **argv = NULL;

	argv = get_input_argv();
	#if 0
	pCmd->dataLen = 0;
	pCmd->Cmd_type = Cmd_type;
	pCmd->Cmd_args = Cmd_args;
	pCmd->checksum = checksum;
	pCmd->m_CRLF = m_CRLF;
	#endif
}

int submit_command(UartCommand *pCmd)
{
	int connect_fd;
	struct sockaddr_un srv_addr;
	char snd_buf[1024];
	char rcv_buf[1024];
	int ret;
	int i;

	//create client socket
	connect_fd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(connect_fd < 0)
	{
		perror("client create socket failed");
		return -1;
	}

	//set server sockaddr_un
	srv_addr.sun_family = AF_UNIX;
	strcpy(srv_addr.sun_path, UNIX_DOMAIN);

	//connect to server
	ret = connect(connect_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	if(ret == -1)
	{
		perror("connect to server failed!");
		close(connect_fd);
		unlink(UNIX_DOMAIN);
		return -1;
	}

	strcpy(snd_buf, "1234567890..!");
	//send message to server
	write(connect_fd, snd_buf, strlen(snd_buf));
	close(connect_fd);

	return 0;
}

int main()
{
    hlp_u8_t *input;
    hlp_u8_t *req_method;
	UartCommand Command = {0};      

//  printf("Content-type: text/plain; charset=iso-8859-1\n\n");
    printf("Content-type: text/html\n\r\n");
    printf("The following is query reuslt:<br><br>");

    //req_method = getenv("REQUEST_METHOD");
    //input = getcgidata(stdin, req_method);
	
	cgidata_process();

	uart_command_init(&Command);
	submit_command(&Command);
	
    return 0;
}




 
