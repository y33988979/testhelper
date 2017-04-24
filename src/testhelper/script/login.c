#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "common.h"   //getcgidata()


int login(char *input)
{
	int i = 0, j = 0;
	char name[64];
	char pass[64];
	
	// 我们获取的input字符串可能像如下的形式
	// Username="admin"&Password="aaaaa"
	// 其中"Username="和"&Password="都是固定的
	// 而"admin"和"aaaaa"都是变化的，也是我们要获取的

	// 前面9个字符是UserName=
	// 在"UserName="和"&"之间的是我们要取出来的用户名
	for ( i = 9; i < (int)strlen(input); i++ ) 
	{
		if ( input[i] == '&' ) 
		{
			name[j] = '\0';
			break;
		}                   
		name[j++] = input[i];
	}

    // 前面9个字符 + "&Password="10个字符 + Username的字符数
    // 是我们不要的，故省略掉，不拷贝
	for ( i = 19 + strlen(name), j = 0; i < (int)strlen(input); i++ ) 
	{
		pass[j++] = input[i];
	}
	pass[j] = '\0';

	printf("Your Username is %s<br>Your Password is %s<br> \n", name, pass);
	//do_scandir("/root/HI3716/web/testhelper/logs");
	//printf("<a href=http://192.168.30.188/logs><font color=blue>logs download</font></a><br>");
	printf("<a href=http://192.168.30.188/stb_test.html><font color=blue>机顶盒测试</font></a><br>");
	printf("<a href=http://192.168.30.188/script/download.cgi><font color=blue>日志下载</font></a><br>");
}

int main()
{
    char *input;
    char *req_method;
       
//  printf("Content-type: text/plain; charset=iso-8859-1\n\n");
    printf("Content-type: text/html\n\n");
    printf("The following is query reuslt:<br><br>");

    req_method = getenv("REQUEST_METHOD");
    input = getcgidata(stdin, req_method);

	//printf("<br>---------<br>||%s||<br>--------<br>", input);
	login(input);
	
    return 0;
}

