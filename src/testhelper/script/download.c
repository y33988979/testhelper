/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define UNIX_DOMAIN      "/tmp/UNIX.domain"
#define LOG_FILE_PATH    "/root/HI3716/www/logs"


void print_log_files(char *path)
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
			if(strcmp(namelist[n]->d_name, ".") == 0 ||
				strcmp(namelist[n]->d_name, "..") == 0)
				continue;
			//printf("%s<br>", namelist[n]->d_name);
			printf("<a href=http://192.168.30.188/logs/%s><font color=blue>%s</font></a><br>",\
			namelist[n]->d_name, namelist[n]->d_name);
			free(namelist[n]);
		}
		free(namelist);
	}
}

int main()
{
    char *input;
    char *req_method;
       
//  printf("Content-type: text/plain; charset=iso-8859-1\n\n");
    printf("Content-type: text/html\n\r\n");
    printf("The logs :<br><br>");

    //req_method = getenv("REQUEST_METHOD");
    //input = getcgidata(stdin, req_method);

	print_log_files(LOG_FILE_PATH);
	
    return 0;
}




 
