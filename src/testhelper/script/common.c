/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */

#include <common.h>

#define  CGI_INPUT_BUFFER_MAX_NUM   1024
#define  CGI_INPUT_ARG_MAX_NUM      10

hlp_u8_t   cgi_input[CGI_INPUT_BUFFER_MAX_NUM];
hlp_u8_t  *cgi_input_argv[CGI_INPUT_ARG_MAX_NUM] = {NULL};
hlp_u8_t   cgi_input_argc = 0;
hlp_bool_t cgi_data_parse_done = HLP_FALSE;

hlp_u8_t* getcgidata(FILE* fp, hlp_u8_t* requestmethod)
{
    hlp_u8_t* input;
    int len;
    int size = CGI_INPUT_BUFFER_MAX_NUM;
    int i = 0;

	   //从这里可以看出来，GET在cgi中传递的Username="admin"&Password="aaaaa"被放置在环境变量QUERY_STRING中了。
    if (!strcmp(requestmethod, "GET")) { 
        input = getenv("QUERY_STRING");
        return input;
      } else if (!strcmp(requestmethod, "POST")) {
        len = atoi(getenv("CONTENT_LENGTH"));
        input = cgi_input;
        
           if (len == 0) {
            input[0] = '\0';
            return input;
           }
        //从这里可以看出来，POST在cgi中传递的Username="admin"&Password="aaaaa"被写入stdin标准输入流中了。
        while(1) { 
            input[i] = (char)fgetc(fp);
               if (i == size) {
                input[i+1] = '\0';
                return input;
             }
                     
               --len;
               if (feof(fp) || (!(len))) {
                i++;
                input[i] = '\0';
                return input;
            }
               i++;
           
         }
    }
    return NULL;
}

hlp_u8_t input_parse(hlp_u8_t *input)
{
	hlp_u8_t        *pData;
	hlp_u16_t        i, j;

	i = 0;
	j = 0;
	pData = input;
	cgi_input_argc = 0;
	cgi_input_argv[j++] = pData;
	for(i=0; i<CGI_INPUT_BUFFER_MAX_NUM; i++)
	{
		if(pData[i] == '&')
		{
			pData[i] = '\0';
			cgi_input_argv[j++] = pData+i+1;
			cgi_input_argc++;
			continue;
		}
		if(pData[i] == '\0')
			break;
	}
	cgi_data_parse_done = HLP_TRUE;
	return cgi_input_argc;
}

hlp_u8_t get_input_argc()
{
	if(cgi_data_parse_done)
		return cgi_input_argc;
	else
		return 0;
}

hlp_u8_t** get_input_argv()
{
	if(cgi_data_parse_done)
		return cgi_input_argv;
	else
		return NULL;
}

hlp_u8_t cgidata_process()
{
	hlp_u8_t *input;
	hlp_u8_t *req_method;
	
	req_method = getenv("REQUEST_METHOD");
	input = getcgidata(stdin, req_method);
	input_parse(input);
	cgi_data_parse_done = HLP_TRUE;
}

