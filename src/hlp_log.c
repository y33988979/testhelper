/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_log.h>
#include <hlp_file.h>
#include <hlp_common.h>
#include <hlp_string.h>


#define hlp_stderr          STDERR_FILENO
#define HLP_MAX_ERROR_STR   2048

static hlp_log_t    hlp_log;
static hlp_file_t   hlp_log_file;

static char *err_levels[] = 
{    
	NULL,    
	("emerg"),    
	("alert"),    
	("crit"),    
	("error"),    
	("warn"),    
	("notice"),    
	("info"),    
	("debug")
};

static inline int hlp_write_console(char *buf)
{
	return write(hlp_stderr, buf, strlen(buf));
}

void hlp_log_stderr(const char *fmt, ...)
{    
	char     *p, *last;    
	va_list   args;    
	char      errstr[HLP_MAX_ERROR_STR];    
	last = errstr + HLP_MAX_ERROR_STR;    
	hlp_memcpy(errstr, "helper: ", 9);

	p = errstr;
	va_start(args, fmt);    
	vsprintf(p, fmt, args);
	va_end(args);
	
	//ngx_linefeed(p);
	(void) hlp_write_console(errstr);
}

hlp_s32_t hlp_log_cleanup()
{
	return 0;
}

hlp_s32_t hlp_log_file_judge(hlp_u8_t *filename)
{
	hlp_u32_t count = 200;
	hlp_u8_t  tmpfilename[64] = {0};
	hlp_u8_t  dataString[64] = {0};
	hlp_u8_t  file_num = 1;
	hlp_u8_t  file_num_string[16] = {0};
	
	hlp_get_current_data_string(dataString);
	while(count--)
	{
		
		hlp_strcpy(tmpfilename, "syslog_");
		hlp_strcat(tmpfilename, dataString);
		sprintf(file_num_string, "_%03d", file_num);
		hlp_strcat(tmpfilename, file_num_string);
		hlp_strcat(tmpfilename, ".txt");
		printf("the file_num=%d, file_num_string=%s.\n", file_num, file_num_string);
		/* �������ļ��Ѿ�����������һ���ļ����ٴ��� 
		(������û��rtc���������ļ����ܻ��ظ�)*/
		if(hlp_file_isexist(HLP_ERROR_LOG_PATH, tmpfilename) == HLP_TRUE)
		{
			file_num++;
		}
		else
		{
			break;
		}
	}

	if(count == 0)
	{
		printf("the runlog files is reach max,count=200, clean up!!!\n");
		return -1;
	}

	hlp_strcpy(filename, tmpfilename);
	return 0;
}

hlp_log_t* hlp_log_init()
{
	hlp_u8_t      *path, *p = NULL;
	hlp_u8_t       dataTimeString[64] = {0};
	hlp_u16_t      nlen;
	
	hlp_log.file = &hlp_log_file;
	hlp_log.log_level = HLP_LOG_NOTICE;

	path = (hlp_u8_t *) HLP_ERROR_LOG_PATH;
	nlen = hlp_strlen(path);
	p = path;

	if(hlp_log_file_judge(hlp_log_file.name) < 0)
	{
		system("rm /root/HI3716/runlog/*");
		hlp_log_file_judge(hlp_log_file.name);
	}

	hlp_strcpy(hlp_log_file.path, HLP_ERROR_LOG_PATH);
	hlp_strcat(hlp_log_file.path, hlp_log_file.name);
	
	hlp_log_file.fd = hlp_file_open(hlp_log_file.path, O_CREAT|O_WRONLY, 664);
	if(hlp_log_file.fd < 0)
	{
		hlp_log_file.fd = hlp_stderr;
		//hlp_log_stderr("[alert] could not open error log file: open() \"%s\" failed", path);      
	}
	
	return &hlp_log;

}

hlp_s32_t hlp_log_close()
{
	hlp_s32_t ret =-1;
	ret = hlp_file_close(hlp_log_file.fd);
	
	if(ret < 0)
	{
		printf("hlp_log_close file=%s, error:%s", hlp_log_file.name, strerror(errno));
		LOG_WRITE_ERR("hlp_log_close file=%s, error:%s", hlp_log_file.name, strerror(errno));
	}
}

void hlp_log_set_levels(hlp_u8_t level)
{

}

hlp_s32_t hlp_log_error_core(hlp_u8_t level, const char *fmt, ...)
{
	va_list      args;
	char        *p, *last;
	char         errstr[HLP_MAX_ERROR_STR];
	hlp_u16_t     n;

	last = errstr + HLP_MAX_ERROR_STR;
	//p = hlp_slprintf(p, last, " [%V] ", &err_levels[level]);

	p = errstr;
	va_start(args, fmt);    
	vsprintf(p, fmt, args);    
	va_end(args);

	//printf("[%s]\n", err_levels[level]);
	if (hlp_log.log_level < level) 
	{
		return HLP_ERROR;
	}
	n = hlp_write_fd(hlp_log.file->fd, errstr, strlen(p));
	return n;
}


int hlp_log_test()
{
	return 0;
}

