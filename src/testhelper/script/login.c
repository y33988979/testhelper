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
	
	// ���ǻ�ȡ��input�ַ������������µ���ʽ
	// Username="admin"&Password="aaaaa"
	// ����"Username="��"&Password="���ǹ̶���
	// ��"admin"��"aaaaa"���Ǳ仯�ģ�Ҳ������Ҫ��ȡ��

	// ǰ��9���ַ���UserName=
	// ��"UserName="��"&"֮���������Ҫȡ�������û���
	for ( i = 9; i < (int)strlen(input); i++ ) 
	{
		if ( input[i] == '&' ) 
		{
			name[j] = '\0';
			break;
		}                   
		name[j++] = input[i];
	}

    // ǰ��9���ַ� + "&Password="10���ַ� + Username���ַ���
    // �����ǲ�Ҫ�ģ���ʡ�Ե���������
	for ( i = 19 + strlen(name), j = 0; i < (int)strlen(input); i++ ) 
	{
		pass[j++] = input[i];
	}
	pass[j] = '\0';

	printf("Your Username is %s<br>Your Password is %s<br> \n", name, pass);
	//do_scandir("/root/HI3716/web/testhelper/logs");
	//printf("<a href=http://192.168.30.188/logs><font color=blue>logs download</font></a><br>");
	printf("<a href=http://192.168.30.188/stb_test.html><font color=blue>�����в���</font></a><br>");
	printf("<a href=http://192.168.30.188/script/download.cgi><font color=blue>��־����</font></a><br>");
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

