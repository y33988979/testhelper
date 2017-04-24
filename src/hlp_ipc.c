/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_config.h>
#include <hlp_core.h>
#include <hlp_ring.h>
#include <hlp_log.h>
#include <sys/un.h>

#define UNIX_DOMAIN "/tmp/UNIX.domain"
#define WEB_DATA_MAX    512

extern sem_t uart_data_sem;
hlp_buf_t web_data = {0};

void hlp_put_web_data(char *buffer, int len)
{
	web_data.len = len;
	memcpy(web_data.data, buffer, len);
}

hlp_buf_t *hlp_get_web_data()
{
	return &web_data;
}

int hlp_web_msg_handler(hlp_buf_t *pWebData)
{
	
	printf("Message from web (%d) :%s\n", pWebData->len, pWebData->data); 
	LOG_WRITE_ERR("Message from web (%d) :%s\n", pWebData->len, pWebData->data); 

	sem_post(&uart_data_sem);

	return 0;
}

void *hlp_ipc_task(void *args)
{
	struct sockaddr_un clt_addr;
	socklen_t clt_len;  
	int lsn_fd, apt_fd;
	hlp_u8_t *rBuffer;

	rBuffer = web_data.data;
	lsn_fd = *(int*)args;
	clt_len = sizeof(clt_addr);  
	
	while(1)
	{    
		//memset(rbuffer, 0, WEB_DATA_MAX);
		apt_fd = accept(lsn_fd, (struct sockaddr*)&clt_addr, &clt_len);  
		if(apt_fd < 0)    
		{     
			perror("can't listen client connect request"); 
			LOG_WRITE_ERR("can't listen client connect request: %s", strerror(errno)); 
			close(lsn_fd);     
			unlink(UNIX_DOMAIN);     
			return NULL;
		}      
		
		printf("received a connection\n");
		LOG_WRITE_ERR("received a connection\n");
		web_data.len= read(apt_fd, web_data.data, WEB_DATA_MAX);
		if(web_data.len < 0)
		{
			perror("web_data read error");
			LOG_WRITE_ERR("web_data read error: %s", strerror(errno));
			close(apt_fd);
			break;
		}
		hlp_web_msg_handler(&web_data);
		close(apt_fd);
	}
	
	close(apt_fd); 
	close(lsn_fd);  
	unlink(UNIX_DOMAIN);  
}

int hlp_ipc_init()
{

	int lsn_fd;
	int ret;   
	struct sockaddr_un srv_addr;  
	
	//create socket to bind local IP and PORT  
	lsn_fd = socket(PF_UNIX, SOCK_STREAM, 0);  
	if(lsn_fd < 0)	  
	{   
		perror("can't create communication socket!");   
		LOG_WRITE_ERR("can't create communication socket!: %s", strerror(errno));
		return HLP_ERROR; 
	}
	
	//create local IP and PORT
	srv_addr.sun_family = AF_UNIX;  
	strncpy(srv_addr.sun_path, UNIX_DOMAIN, sizeof(srv_addr.sun_path) - 1); 
	unlink(UNIX_DOMAIN);  
	
	//bind sockfd and sockaddr 
	ret = bind(lsn_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)); 
	if(ret == -1)  
	{   
		perror("can't bind local sockaddr!");   
		LOG_WRITE_ERR("can't bind local sockaddr: %s", strerror(errno));   
		close(lsn_fd);   
		unlink(UNIX_DOMAIN);    
		return HLP_ERROR; 
	}
	//listen lsn_fd, try listen 1 
	ret = listen(lsn_fd, 1);
	if(ret == -1) 
	{   
		perror("can't listen client connect request:");   
		LOG_WRITE_ERR("can't listen client connect request: %s", strerror(errno));  
		close(lsn_fd);    
		unlink(UNIX_DOMAIN);    
		return HLP_ERROR; 
	}
	
	return lsn_fd;
}

int hlp_ipc_task_start(void)
{
	int        ret, ipc_fd;
	pthread_t  tid;

	ipc_fd = hlp_ipc_init();
	if(ipc_fd < 0)
	{
		return HLP_ERROR;
	}
	ret = pthread_create(&tid, NULL, (void * (*)(void*)) hlp_ipc_task, (void*)&ipc_fd);
	if(ret < 0)
	{
		perror("pthread_create hlp_ipc_task error:");
		LOG_WRITE_ERR("pthread_create hlp_ipc_task error: %s", strerror(errno));
		return HLP_ERROR;
	}
	
	sleep(1);
	printf("Start new hlp_ipc_task success! [0x%x]\n", (int)&tid);
	LOG_WRITE_ERR("Start new hlp_ipc_task success! [0x%x]\n", (int)&tid);

//test
	
	while(1)
	{
		sleep(1);
		hlp_put_web_data("reboot", 7);
		hlp_web_msg_handler(&web_data);
	}
	
	return HLP_OK;
}

