#include <stdio.h>
#include <string.h>

#include "bluetooth_utils.h"
#include "log.h"


struct paired_thread_param
{
	hci_dev_context_t *hci_dev_context;
	char *adapter_id;
	char *passkey;
};

struct alsa_thread_param
{
	char *filename;
};

void local_dev_info(hci_dev_context_t *hci_dev_context, dev_info_sample_t **local_dev_list)
{
	int count = DRV_Get_Local_Device_List(hci_dev_context, local_dev_list);
	LOG0("DRV_Get_Local_Device_List() returned: %d\n", count);
	LOG0 ("Get local device count %d\n", count);

	int i;
	for (i = 0; i < count; i++)
	{
		LOG0 ("<%d> %s |  %s\n", i, (*local_dev_list+i)->name, (*local_dev_list+i)->addr);
	}
}

void blue_scan(hci_dev_context_t *hci_dev_context, unsigned short dev_id)
{
	dev_info_sample_t *remote_dev_list = NULL;
	LOG0 ("\n Scanning....\n");
	int rn = DRV_Get_Remote_Device_List(hci_dev_context, dev_id, &remote_dev_list);
	LOG0("DRV_Get_Remote_Device_List() returned: %d\n", rn);
	LOG0 ("Remote devices count: %d\n", rn);

	int i;
	for (i = 0; i < rn; i++)
	{
		LOG0 ("[R] <%d> %s | %s\n", i, (remote_dev_list+i)->name, (remote_dev_list+i)->addr);
	}
}
#if 1
void push_file(hci_dev_context_t *hci_dev_context, char *dest_addr, int channel, char *filename)
{
	if (channel <= 0)
		channel = OOP_DEF_CHANNEL;
	
	int rn = DRV_OBEX_Connect(hci_dev_context, dest_addr, channel);
	if (rn < 0)
		return;

	LOG0 ("!!! Sending ...\n");
	DRV_OBEX_Push_Object(hci_dev_context, filename);
	LOG0 ("!!! OK\n");
	
	DRV_OBEX_Disconnect(hci_dev_context);
	
}
#endif

void *wait_for_paired_thread(struct paired_thread_param *param)
{
	LOG0 ("wait_for_paired_thread() start ...\n");
	pthread_detach (pthread_self());
	
	int rn = DRV_Wait_For_Paired(param->hci_dev_context, param->adapter_id, param->passkey);

	if (rn < 0)
	{
		LOG0 ("!!!! Failed to waitting for paired\n");
	}
	LOG0 ("wait_for_paired_thread() exit ...\n");

	pthread_exit();
}

void *alsa_playback_thread(struct alsa_thread_param *param)
{
	LOG0 ("alsa_playback_thread() start ...\n");
	pthread_detach (pthread_self());

	int rn;
	rn = DRV_ALSA_Playback_Start(param->filename);
	if (rn == 0)
		LOG0 ("!!! Playback Done !!!\n");
	else
		LOG0("!!! Playback failed!!!\n");

	LOG0 ("alsa_playback_thread() exit ...\n");
	pthread_exit();
}

void *alsa_capture_thread(struct alsa_thread_param *param)
{
	LOG0 ("alsa_capture_thread() start ...\n");
	pthread_detach (pthread_self());

	int rn;
	rn = DRV_ALSA_Capture_Start(param->filename, NULL);
	if (rn == 0)
		LOG0 ("!!! Capture Done !!!\n");
	else
		LOG0("!!! Capture failed!!!\n");
				
	LOG0 ("alsa_capture_thread() exit ...\n");
	pthread_exit();
}

void print_usage()
{
	printf ("\n***********************************************\n");
	printf ("<1> Get local adapter infomation\n");
	printf ("<2> Scan remote devices (Depend on <1>)\n");
	printf ("<3> Paire remote device (Depend on <1>)\n");
	printf ("<4> Remove paired device (Depend on <1>)\n");
	printf ("<5> Get OOP channel\n");
	printf ("<6> Push file\n");
	printf ("<7> Reveive file\n");
	printf ("<8> Wait for pair\n");
	printf ("<9> Cancel wait for paired\n\n");
	printf ("<a> Play a audio file\n");
	printf ("<b> Capture audio\n\n");
	printf ("<c> Cancel playback\n");
	printf ("<d> Cancel capture\n");
	printf ("\n<q> Exit\n");
	printf ("***********************************************\n");
}

int bluetest()
{
	printf ("\n<Build3 %s %s>\n\n",  __DATE__, __TIME__);

	hci_dev_context_t hci_dev_context;
	dev_info_sample_t *local_dev_list = NULL;

	int rn = 0;
    local_dev_list = malloc(sizeof(dev_info_sample_t) * 10);
    if(local_dev_list == NULL)
    {

        printf("[aweigh]  local_dev_list == NULL   \n");
        exit(1);
    }
	rn = DRV_BT_Init(&hci_dev_context);
	LOG0("!!!!DRV_BT_Init returned: %d\n", rn);
	if (rn < 0)
	{
		LOG0 ("Failed to initialize hci_dev_context\n");
		return -1;
	}
//    system("hciconfig -a");
	int n = 0;
	char addr[18];
	char passkey[32];
	char adapter_id[32];
	char filename[64];
	int channel = 0;
	
	while(1)
	{
		print_usage();
		n = getchar();
		getchar();
		if (n == 'q')
			break;

		switch(n)
		{
			case '1':
				local_dev_info(&hci_dev_context, &local_dev_list);
				break;
			case '2':
				blue_scan(&hci_dev_context, local_dev_list->dev_id);
				break;
			case '3':
				memset(adapter_id, 0, sizeof(adapter_id));
				memset(addr, 0, sizeof(addr));
				memset(passkey, 0, sizeof(passkey));

				LOG0 ("type in the local adapter id(hci0):\n");
				scanf("%s", adapter_id);
				LOG0 ("type in the address of remote device:\n");
				scanf("%s", addr);
				LOG0 ("type in passkey:\n");
				scanf("%s", passkey);
				getchar();
				rn = DRV_Pair_Device(&hci_dev_context, adapter_id, addr, passkey);
				if (rn == 1)
					LOG0 ("!!!! paired succeccfully !!!!\n");
				else
					LOG0("!!!!! paired fail !!!!\n");
				break;
			case '4':
				memset(adapter_id, 0, sizeof(adapter_id));
				memset(addr, 0, sizeof(addr));
				LOG0 ("type in the local adapter id(hci0):\n");
				scanf("%s", adapter_id);
				LOG0 ("type in the address of remote device:\n");
				scanf("%s", addr);
				getchar();
				rn = DRV_Remove_Paired_Device (&hci_dev_context, adapter_id, addr);
				if (rn == 0)
					LOG0 ("!!!! Removed done !!!!\n");
				else
					LOG0("!!!!! Removed fail !!!!\n");
				break;
				#if 1
			case '5':
				memset(addr, 0, sizeof(addr));
				LOG0 ("type in the address of remote device:\n");
				scanf("%s", addr);
				getchar();
				rn = DRV_SDP_Get_OOP_Channel(addr);
				LOG0 ("!!!!! [%s] OOP channel: %d !!!!!\n", addr, rn);
				break;

			case '6':
				memset(addr, 0, sizeof(addr));
				memset(filename, 0, sizeof(filename));
				LOG0("type in the address of remote device:\n");
				scanf("%s", addr);
				LOG0("type in the file name:\n");
				scanf("%s", filename);
				LOG0("type in the OOP service channel of remote device:\n");
				scanf("%d", &channel);
				getchar();
				push_file(&hci_dev_context, addr, channel, filename);
				LOG0("!!!! Push OK\n");
				break;
				
			case '7':
				if (DRV_OBEX_Server_Register(&hci_dev_context) < 0)
				{
					LOG0 ("!!!Failed to register\n ");
					break;
				}
				if (DRV_OBEX_Accept(&hci_dev_context, 80) < 0)
				{
					LOG0 ("!!!Failed to accept\n ");
					break;
				}
				//接收文件存放到/root/HI3716M/bluetooth/files/xxx
				DRV_OBEX_Receive_Object(&hci_dev_context, "/usr/share/bluetooth/files", 15);
				LOG0 ("!!! Finish  !!!\n");
				break;
				#endif
			case '8':
				memset(adapter_id, 0, sizeof(adapter_id));
				memset(passkey, 0, sizeof(passkey));
				LOG0 ("type in the local adapter id(hci0):\n");
				scanf("%s", adapter_id);
				LOG0 ("type in passkey:\n");
				scanf("%s", passkey);
				getchar();
				
				struct paired_thread_param param;
				param.hci_dev_context = &hci_dev_context;
				param.adapter_id = adapter_id;
				param.passkey = passkey;
				int p_tid;
				pthread_create(&p_tid, NULL, (void*)wait_for_paired_thread, &param);
				break;
			case '9':
				DRV_Cancel_Wait_For_Paired();
				break;
			case 'a':
				memset (addr, 0, sizeof(addr));
				memset (filename, 0, sizeof(filename));
				LOG0("type in the address of remote device:\n");
				scanf("%s", addr);
				LOG0("type in the file name:\n");
				scanf("%s", filename);
				rn = DRV_ALSA_BTHeadset_Init(addr);
				if (rn < 0)
				{
					LOG0("Can not set headset address for ALSA\n");
					continue;
				}

				struct alsa_thread_param ap_param;
				ap_param.filename = filename;
				int a_tid;
				pthread_create(&a_tid, NULL, (void*)alsa_playback_thread, &ap_param);
				break;
			case 'b':
				memset (addr, 0, sizeof(addr));
				memset (filename, 0, sizeof(filename));
				LOG0("type in the address of remote device:\n");
				scanf("%s", addr);
				LOG0("type in the file name:\n");
				scanf("%s", filename);
				rn = DRV_ALSA_BTHeadset_Init(addr);
				if (rn < 0)
				{
					LOG0("Can not set headset address for ALSA\n");
					continue;
				}
				struct alsa_thread_param ac_param;
				ac_param.filename = filename;
				int c_tid;
				pthread_create(&c_tid, NULL, (void*)alsa_capture_thread, &ac_param);
				break;
			case 'c':
				DRV_ALSA_Playback_Stop();
				break;
			case 'd':
				DRV_ALSA_Capture_Stop();
				break;

		}

	}

	
	DRV_BT_Uninit(&hci_dev_context);

	return 0;
}

