/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
#include <hlp_file.h>
#include <hlp_gpio.h>
#include <hlp_string.h>
#include <hlp_drv.h>
#include <hlp_log.h>
#include <hlp_file_manager.h>

hlp_bool_t usb_mount_ok = HLP_FALSE;

hlp_s32_t hlp_file_remove(hlp_file_t *file)
{
	return unlink((char*)file->path);
}

hlp_s32_t hlp_file_copy(hlp_file_t *file)
{
	return 0;
}

hlp_s32_t hlp_file_remove_oldest(hlp_file_t *file)
{
	return system("ls -rt | head -40 | rm -rf");
}

hlp_u32_t hlp_get_file_size(char *path)  
{  
	hlp_u32_t filesize = -1;
	
	struct stat statbuff;  
	if(stat(path, &statbuff) < 0)
	{  
		return filesize;  
	}
	else
	{  
		filesize = statbuff.st_size;  
	}  
	return filesize;  
} 

hlp_bool_t hlp_mount_ok()
{
	FILE       *fp;
	hlp_s32_t   i = 0;
	hlp_u8_t    line[256];
	hlp_u8_t   *mount_dir_ptr = NULL;
	hlp_u8_t    cmd[64] = {0};
	hlp_s32_t   filesize = -1;
	hlp_bool_t  usb_mount_ok = HLP_FALSE;

	if((fp = fopen("/proc/mounts", "r")) != NULL)
	{
		while(fgets((char*)line, sizeof(line), fp))
		{
			if((mount_dir_ptr = (hlp_u8_t*)hlp_strstr(line, "/media/volume")) != NULL)
			{
				while(*(mount_dir_ptr+i) != ' ' && i<256)
					i++;
				*(mount_dir_ptr+i) = '\0';
				sprintf((char*)cmd, "ls -la %s > file_list", (char*)mount_dir_ptr);
				if(system((char*)cmd) < 0)
				{
					printf("[testhelper]: [error]: system exec failed!!!!!!\n");
					LOG_WRITE_ERR("[testhelper]: [error]: system exec failed!!!!!!\n");
				}
				
				filesize = hlp_get_file_size((char*)"file_list");
				if(filesize > 0)
					usb_mount_ok = HLP_TRUE;
			}
		}
		fclose(fp);
	}
	return usb_mount_ok;
}

void hlp_mount_handle()
{
	usb_mount_ok = HLP_TRUE;
	hlp_redled_off();
	hlp_greenled_on();
	hlp_buzzer_ring_ms(200);
	printf("[testhelper]: usb mount ok!!!\n");
	LOG_WRITE_ERR("[testhelper]: usb mount ok!!!\n");
}

void hlp_unmount_handle()
{
	usb_mount_ok = HLP_FALSE;
	if(system("umount /media/volume01") < 0)
	{
		printf("umount failed -----!!!!\n");
		LOG_WRITE_ERR("umount failed -----!!!!\n");
	}
	hlp_greenled_off();
	hlp_redled_on();
	hlp_buzzer_ring_ms(200);
	hlp_redled_off();
	printf("[testhelper]: usb unmount ok!!!\n");
	LOG_WRITE_ERR("[testhelper]: usb unmount ok!!!\n");
}

void usb_callback_register()
{
	register_app_mountok_callback((int)hlp_mount_handle);
	register_app_plugout_callback((int)hlp_unmount_handle);
}


