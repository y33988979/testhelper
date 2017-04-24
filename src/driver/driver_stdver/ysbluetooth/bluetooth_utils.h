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
	int obex_serv_rereg;		// 是否需要重新注册obex server
	int mmem_count;
	void *mmem[MMEM_COUNT];			// the memory pointers need to be freed
	char agent_path[128];
	unsigned int dev_count;	//本地适配器数量
	struct hci_dev_info *dev_info_list;	//本地适配器信息
}hci_dev_context_t;

extern char bt_filepath[128];				// 蓝牙接受文件保存路径

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
获取远程设备列表
Name:
	DRV_Get_Remote_Device_List
Params:
	context: hci_dev_context;
	dev_id: 使用本地设备扫描
	dev_list: 存储远程设备信息
return:
	非负数:远程设备数
	负数:错误
*/
int DRV_Get_Remote_Device_List(hci_dev_context_t *context, unsigned short dev_id, dev_info_sample_t **dev_list);

/*
获取本地设备列表
Name:
	DRV_Get_Local_Device_List
Params:
	context: hci_dev_context;
	dev_list: 存储本地设备信息
return:
	返回本地设备数
*/
int DRV_Get_Local_Device_List(hci_dev_context_t *context, dev_info_sample_t **dev_list);

//========================================================


/*
配对设备
Name:
	DRV_Pair_Device
Params:
	context: hci_dev_context;
	adapter_id:本地适配器id: hci0
	remote_addr: 远程设备地址
	passkey:配对密码
return:
	返回1表示成功，否则出错
*/
int DRV_Pair_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr, char *passkey);

/*
等待被配对，一直等待直到cancel_wait_for_paired()被调用
Name:
	DRV_Wait_For_Paired
Params:
	context: hci_dev_context;
	adapter_id:本地适配器id: hci0
	passkey:配对密码
return:
	出错返回负数，否则成功
*/
int DRV_Wait_For_Paired (hci_dev_context_t *context, char *adapter_id, char *passkey);

/*
等待被配对，一直等待直到cancel_wait_for_paired()被调用
Name:
	DRV_Cancel_Wait_For_Paired
Params:
return:
	null
*/
void DRV_Cancel_Wait_For_Paired();

/*
移除已配对的设备
Name:
	DRV_Remove_Paired_Device
Params:
	context: hci_dev_context;
	adapter_id:本地适配器id: hci0
	remote_addr:要移除的远程设备地址
return:
	成功返回0，否则出错
*/
int DRV_Remove_Paired_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr);


//========================================================



/*
ALSA蓝牙耳机初始化，每次playback或caputre需要执行一次
Name:
	DRV_ALSA_BTHeadset_Init
Params:
	btaddr: 要执行动作的蓝牙耳机地址
return:
	成功返回0，否则出错
*/
int DRV_ALSA_BTHeadset_Init(char *btaddr);

/*
通过蓝牙耳机播放wav文件
Name:
	DRV_ALSA_Playback
Params:
	filename: wav文件名
return:
	成功返回0，否则出错
*/
int DRV_ALSA_Playback(char *filename);

/*
停止播放
Name:
	DRV_ALSA_Playback_Stop
Params:
return:
*/
void DRV_ALSA_Playback_Stop();

/*
通过蓝牙耳机录制wav文件
Name:
	DRV_ALSA_Capture
Params:
	filename: wav文件名
	param:录制参数，当前设置为NULL即可
return:
	成功返回0，否则出错
*/
int DRV_ALSA_Capture(char *filename, alsa_param_t *param);

//========================================================


/*
停止录制
Name:
	DRV_ALSA_Caputre_Stop
Params:
return:
*/
void DRV_ALSA_Capture_Stop();

/*
获取指定蓝牙设备的OOP服务channel
Name:
	DRV_SDP_Get_OOP_Channel
Params:
	dest_addr:蓝牙设备地址
return:
	成功返回0，否则出错
*/
int DRV_SDP_Get_OOP_Channel(char *dest_addr);

/*
获取指定蓝牙设备指定服务的channel
Name:
	DRV_SDP_Get_OOP_Channel
Params:
	dest_addr:蓝牙设备地址
	service_name:服务名称
return:
	成功返回0，否则出错
*/
int DRV_SDP_Get_Service_Channel(char *dest_addr, char *service_name);

/*
本地作为client，连接远程设备，准备push文件
Name:
	DRV_OBEX_Connect
Params:
	context:hci context
	dest_addr:远程设备地址
	oop_channel: 远程设备oop channel
return:
	成功返回0，否则出错
*/
int DRV_OBEX_Connect(hci_dev_context_t *context, char *dest_addr, int oop_channel);

/*
向已建立连接的远端设备推送文件
Name:
	DRV_OBEX_Push_Object
Params:
	context:hci context
	filename: 文件名
return:
	成功返回0，否则出错
*/
int DRV_OBEX_Push_Object(hci_dev_context_t *context, char *filename);

/*
断开已建立的连接
Name:
	DRV_OBEX_Disconnect
Params:
	context:hci context
return:
	成功返回0，否则出错
*/
int DRV_OBEX_Disconnect(hci_dev_context_t *context);

/*
将本机注册为文件接受端
Name:
	DRV_OBEX_Server_Register
Params:
	context:hci context
return:
	成功返回0，否则出错
*/
int DRV_OBEX_Server_Register(hci_dev_context_t *context);

/*
开始等待接受远程文件推送
Name:
	DRV_OBEX_Accept
Params:
	context:hci context
	timeout_s:等待超时时间，单位为秒，超时后自动返回
return:
	成功返回0，负数表示超时
*/
int DRV_OBEX_Accept(hci_dev_context_t *context, int timeout_s);

/*
accept返回后开始receive文件
Name:
	DRV_OBEX_Receive_Object
Params:
	context:hci context
	recv_path: 接收文件保存到路径
	timeout_s:等待超时时间，单位为秒，超时后自动返回
return:
	成功返回0，负数表示超时
*/
int DRV_OBEX_Receive_Object(hci_dev_context_t *context,  char *recv_path, int timeout_s);


//========================================================


/*
蓝牙工具初始化
Name:
	DRV_BT_Init
Params:
	context:hci context
	bt_dir: 文件接收存放目录
return:
	成功返回0，否则出错
*/
int DRV_BT_Init(hci_dev_context_t *context);

/*
蓝牙工具销毁
Name:
	DRV_BT_Uninit
Params:
	context:hci context
return:
	成功返回0，否则出错
*/
int DRV_BT_Uninit(hci_dev_context_t *context);


#endif

