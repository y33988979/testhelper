/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 


#include <hlp_common.h>
#include <hlp_string.h>
#include <hlp_log.h>

void hlp_get_current_data_string(hlp_u8_t *dateString)
{
	char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	time_t timep;
	struct tm *p;

	time(&timep);
	p=gmtime(&timep);
	printf("%04d%02d%02d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
	printf("%s%02d%02d%02d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	sprintf((char*)dateString, "%04d%02d%02d", 1900+p->tm_year, p->tm_mon, p->tm_mday);
}

void hlp_get_current_time_string(hlp_u8_t *timeString)
{
	char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	time_t timep;
	struct tm *p;

	time(&timep);
	p=gmtime(&timep);
	printf("%04d%02d%02d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
	printf("%s%02d%02d%02d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	sprintf((char*)timeString, "%d04%02d%02d", p->tm_hour, p->tm_min, p->tm_sec);
}

void hlp_get_current_datetime_string(hlp_u8_t *datetimeString)
{
	char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
	time_t timep;
	struct tm *p;

	time(&timep);
	p=gmtime(&timep);
	printf("%04d%02d%02d",(1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
	printf("%s%02d%02d%02d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
	sprintf((char*)datetimeString, "%04d%02d%02d_%02d%02d%02d", \
		1900+p->tm_year, p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
}

void hlp_taskdelay_ms(hlp_u32_t Delay)
{
    struct timespec x;

    /* if < 2ms */
    if (Delay <= 2)
    {
        x.tv_sec  = 0;
        x.tv_nsec = 2000001;
    }
    else
    {
        x.tv_sec  = Delay / 1000;
        x.tv_nsec = (Delay % 1000) * 1000000; 
    }
    nanosleep (&x, 0);
}

hlp_s32_t hlp_get_dir_file_num(hlp_u8_t *path)
{
	struct dirent **namelist;
	int n;

	n = scandir(path, &namelist, 0, alphasort);
	if(n < 0)
	{
		perror("scandir");
		return -1;
	}
	else 
		return n;
}

hlp_bool_t hlp_file_isexist(hlp_u8_t *path, hlp_u8_t *filename)
{
	struct dirent **namelist;
	int n;

	n = scandir(path, &namelist, 0, alphasort);
	if(n < 0)
	{
		perror("scandir");
	}
	else 
	{
		while (n--) 
		{
			printf("%s\n", namelist[n]->d_name);
			if(hlp_strcmp(namelist[n]->d_name, filename) == 0)
			{
				return HLP_TRUE;
			}
			free(namelist[n]);
		}
		free(namelist);
	}
	return HLP_FALSE;
}


