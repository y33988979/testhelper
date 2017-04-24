#ifndef __BLUETOOTH_UTILS__
#define __BLUETOOTH_UTILS__

#include <dbus/dbus.h>
#include <openobex/obex.h>

#include "alsa_cp.h"

#define OOP_DEF_CHANNEL		9			// obex object push service channel (default)

// for free the memory alloced by malloc()
enum mmem_index
{
	REMOTE_DEV_LIST		= 0,
	LOCAL_DEV_LIST			= 1,
	OBEX_CONTEXT			= 2,
	MMEM_COUNT			= 3,
};


typedef struct hci_dev_context
{
//	int fd;
	DBusConnection *dbus_conn;
	obex_t *obex_handle;		// obex handle
	int obex_serv_rereg;		// �Ƿ���Ҫ����ע��obex server
	int mmem_count;
	void *mmem[MMEM_COUNT];			// the memory pointers need to be freed
	char agent_path[128];
	unsigned int dev_count;	//��������������
	struct hci_dev_info *dev_info_list;	//������������Ϣ
}hci_dev_context_t;

extern char bt_filepath[128];				// ���������ļ�����·��

typedef struct dev_info_sample
{
	//only effect local device, the value is the field 'n' of 'hcin'
	unsigned short dev_id;		
	//Device name, for local devices, is 'hcin', for remote devices, is device name
	char name[64];			
	//the address of Local/Remote devices
	char addr[18];
}dev_info_sample_t;

typedef struct obex_context
{
	int serverdone;
	int clientdone;
	char *get_name;	/* Name of last get-request */
}obex_context_t;


/*
��ȡԶ���豸�б�
Name:
	DRV_Get_Remote_Device_List
Params:
	context: hci_dev_context;
	dev_id: ʹ�ñ����豸ɨ��
	dev_list: �洢Զ���豸��Ϣ
return:
	�Ǹ���:Զ���豸��
	����:����
*/
int DRV_Get_Remote_Device_List(hci_dev_context_t *context, unsigned short dev_id, dev_info_sample_t **dev_list);

/*
��ȡ�����豸�б�
Name:
	DRV_Get_Local_Device_List
Params:
	context: hci_dev_context;
	dev_list: �洢�����豸��Ϣ
return:
	���ر����豸��
*/
int DRV_Get_Local_Device_List(hci_dev_context_t *context, dev_info_sample_t **dev_list);

//========================================================


/*
����豸
Name:
	DRV_Pair_Device
Params:
	context: hci_dev_context;
	adapter_id:����������id: hci0
	remote_addr: Զ���豸��ַ
	passkey:�������
return:
	����1��ʾ�ɹ����������
*/
int DRV_Pair_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr, char *passkey);

/*
�ȴ�����ԣ�һֱ�ȴ�ֱ��cancel_wait_for_paired()������
Name:
	DRV_Wait_For_Paired
Params:
	context: hci_dev_context;
	adapter_id:����������id: hci0
	passkey:�������
return:
	�����ظ���������ɹ�
*/
int DRV_Wait_For_Paired (hci_dev_context_t *context, char *adapter_id, char *passkey);

/*
�ȴ�����ԣ�һֱ�ȴ�ֱ��cancel_wait_for_paired()������
Name:
	DRV_Cancel_Wait_For_Paired
Params:
return:
	null
*/
void DRV_Cancel_Wait_For_Paired();

/*
�Ƴ�����Ե��豸
Name:
	DRV_Remove_Paired_Device
Params:
	context: hci_dev_context;
	adapter_id:����������id: hci0
	remote_addr:Ҫ�Ƴ���Զ���豸��ַ
return:
	�ɹ�����0���������
*/
int DRV_Remove_Paired_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr);


//========================================================



/*
ALSA����������ʼ����ÿ��playback��caputre��Ҫִ��һ��
Name:
	DRV_ALSA_BTHeadset_Init
Params:
	btaddr: Ҫִ�ж���������������ַ
return:
	�ɹ�����0���������
*/
int DRV_ALSA_BTHeadset_Init(char *btaddr);

/*
ͨ��������������wav�ļ�
Name:
	DRV_ALSA_Playback
Params:
	filename: wav�ļ���
return:
	�ɹ�����0���������
*/
int DRV_ALSA_Playback(char *filename);

/*
ֹͣ����
Name:
	DRV_ALSA_Playback_Stop
Params:
return:
*/
void DRV_ALSA_Playback_Stop();

/*
ͨ����������¼��wav�ļ�
Name:
	DRV_ALSA_Capture
Params:
	filename: wav�ļ���
	param:¼�Ʋ�������ǰ����ΪNULL����
return:
	�ɹ�����0���������
*/
int DRV_ALSA_Capture(char *filename, alsa_param_t *param);

//========================================================


/*
ֹͣ¼��
Name:
	DRV_ALSA_Caputre_Stop
Params:
return:
*/
void DRV_ALSA_Capture_Stop();

/*
��ȡָ�������豸��OOP����channel
Name:
	DRV_SDP_Get_OOP_Channel
Params:
	dest_addr:�����豸��ַ
return:
	�ɹ�����0���������
*/
int DRV_SDP_Get_OOP_Channel(char *dest_addr);

/*
��ȡָ�������豸ָ�������channel
Name:
	DRV_SDP_Get_OOP_Channel
Params:
	dest_addr:�����豸��ַ
	service_name:��������
return:
	�ɹ�����0���������
*/
int DRV_SDP_Get_Service_Channel(char *dest_addr, char *service_name);

/*
������Ϊclient������Զ���豸��׼��push�ļ�
Name:
	DRV_OBEX_Connect
Params:
	context:hci context
	dest_addr:Զ���豸��ַ
	oop_channel: Զ���豸oop channel
return:
	�ɹ�����0���������
*/
int DRV_OBEX_Connect(hci_dev_context_t *context, char *dest_addr, int oop_channel);

/*
���ѽ������ӵ�Զ���豸�����ļ�
Name:
	DRV_OBEX_Push_Object
Params:
	context:hci context
	filename: �ļ���
return:
	�ɹ�����0���������
*/
int DRV_OBEX_Push_Object(hci_dev_context_t *context, char *filename);

/*
�Ͽ��ѽ���������
Name:
	DRV_OBEX_Disconnect
Params:
	context:hci context
return:
	�ɹ�����0���������
*/
int DRV_OBEX_Disconnect(hci_dev_context_t *context);

/*
������ע��Ϊ�ļ����ܶ�
Name:
	DRV_OBEX_Server_Register
Params:
	context:hci context
return:
	�ɹ�����0���������
*/
int DRV_OBEX_Server_Register(hci_dev_context_t *context);

/*
��ʼ�ȴ�����Զ���ļ�����
Name:
	DRV_OBEX_Accept
Params:
	context:hci context
	timeout_s:�ȴ���ʱʱ�䣬��λΪ�룬��ʱ���Զ�����
return:
	�ɹ�����0��������ʾ��ʱ
*/
int DRV_OBEX_Accept(hci_dev_context_t *context, int timeout_s);

/*
accept���غ�ʼreceive�ļ�
Name:
	DRV_OBEX_Receive_Object
Params:
	context:hci context
	recv_path: �����ļ����浽·��
	timeout_s:�ȴ���ʱʱ�䣬��λΪ�룬��ʱ���Զ�����
return:
	�ɹ�����0��������ʾ��ʱ
*/
int DRV_OBEX_Receive_Object(hci_dev_context_t *context,  char *recv_path, int timeout_s);


//========================================================


/*
�������߳�ʼ��
Name:
	DRV_BT_Init
Params:
	context:hci context
	bt_dir: �ļ����մ��Ŀ¼
return:
	�ɹ�����0���������
*/
int DRV_BT_Init(hci_dev_context_t *context);

/*
������������
Name:
	DRV_BT_Uninit
Params:
	context:hci context
return:
	�ɹ�����0���������
*/
int DRV_BT_Uninit(hci_dev_context_t *context);


#endif

