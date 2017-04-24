#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>


#include <bluetooth.h>
#include <rfcomm.h>

#include <hci_lib.h>
#include <hci.h>

#include <openobex/obex.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>


#include "log.h"
#include "bluetooth_utils.h"
#include "obex_client.h"
#include "obex_server.h"
#include "sdptool.h"
#include "alsa_cp.h"

#include "hi_bluetooth.h"
#define DAEMON_PATH		"/kmod/"
#define DAEMON_START_SH	"/kmod/bluez_init"

#define MAX_REMOTE_DEV_COUNT		10


//hci_dev_context_t hci_dev_context;

char bt_filepath[128];

static char *passkey_value;
// __p_io_canceled/__p_io_terminated是配对设备时所用的标志
static volatile sig_atomic_t __p_io_canceled = 0;
static volatile sig_atomic_t __p_io_terminated = 0;
// __w_io_canceled/__w_io_terminated是接受配对时所用的标志
static volatile sig_atomic_t __w_io_canceled = 0;
static volatile sig_atomic_t __w_io_terminated = 0;

static volatile sig_atomic_t exit_on_release = 1;


static DBusHandlerResult request_pincode_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;

	if (!passkey_value)
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
							DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for RequestPinCode method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	printf("Pincode request for device %s\n", path);

	dbus_message_append_args(reply, DBUS_TYPE_STRING, &passkey_value,
							DBUS_TYPE_INVALID);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult request_passkey_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;
	unsigned int passkey;

	if (!passkey_value)
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
							DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for RequestPasskey method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	printf("Passkey request for device %s\n", path);

	passkey = strtoul(passkey_value, NULL, 10);

	dbus_message_append_args(reply, DBUS_TYPE_UINT32, &passkey,
							DBUS_TYPE_INVALID);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult request_confirmation_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path;
	unsigned int passkey;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
						DBUS_TYPE_UINT32, &passkey,
							DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for RequestPasskey method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	printf("Confirmation request of %u for device %s\n", passkey, path);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult authorize_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;
	const char *path, *uuid;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_OBJECT_PATH, &path,
						DBUS_TYPE_STRING, &uuid,
							DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for Authorize method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	printf("Authorizing request for %s\n", path);

send:
	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult cancel_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for passkey Confirm method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	printf("Request canceled\n");

	reply = dbus_message_new_method_return(msg);
	if (!reply) {
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}

static DBusHandlerResult release_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	DBusMessage *reply;

	if (!dbus_message_get_args(msg, NULL, DBUS_TYPE_INVALID)) 
	{
		fprintf(stderr, "Invalid arguments for Release method");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (!__w_io_canceled)
		fprintf(stderr, "Agent has been released\n");

	if (exit_on_release)
	{
    		 printf("%s, %d:unlikely\n", __FILE__, __LINE__);
     
		__p_io_terminated = 1;
	 }
	else
	{
	 	printf("%s, %d:likely\n", __FILE__, __LINE__);   
	 }

	reply = dbus_message_new_method_return(msg);
	if (!reply) 
	{
		fprintf(stderr, "Can't create reply message\n");
		return DBUS_HANDLER_RESULT_NEED_MEMORY;
	}

	dbus_connection_send(conn, reply, NULL);

	dbus_connection_flush(conn);

	dbus_message_unref(reply);

	return DBUS_HANDLER_RESULT_HANDLED;
}


static DBusHandlerResult agent_message(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPinCode"))
		return request_pincode_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestPasskey"))
		return request_passkey_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent",
							"RequestConfirmation"))
		return request_confirmation_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Authorize"))
		return authorize_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Cancel"))
		return cancel_message(conn, msg, data);

	if (dbus_message_is_method_call(msg, "org.bluez.Agent", "Release"))
		return release_message(conn, msg, data);

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}


static DBusHandlerResult agent_filter(DBusConnection *conn,
						DBusMessage *msg, void *data)
{
	 printf (">>>> %s  %d | start ... \n", __FUNCTION__,__LINE__);
	const char *name, *old, *new;

	if (!dbus_message_is_signal(msg, DBUS_INTERFACE_DBUS,
						"NameOwnerChanged"))
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

	if (!dbus_message_get_args(msg, NULL,
					DBUS_TYPE_STRING, &name,
					DBUS_TYPE_STRING, &old,
					DBUS_TYPE_STRING, &new,
					DBUS_TYPE_INVALID)) {
		fprintf(stderr, "Invalid arguments for NameOwnerChanged signal");
		return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
	}

	if (!strcmp(name, "org.bluez") && *new == '\0') {
		fprintf(stderr, "Agent has been terminated\n");
		__p_io_terminated = 1;
	}

	return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

static void create_paired_device_reply(DBusPendingCall *pending,
							void *user_data)
{
    LOG1("%s, %d:create_paired_device_reply\n", __FILE__, __LINE__);
	
#if 1
    
	DBusMessage *message;
	DBusError err;

	int rn = 1;
	

	/* steal_reply will always return non-NULL since the callback
	 * is only called after a reply has been received */
	message = dbus_pending_call_steal_reply(pending);

	dbus_error_init(&err);
	if (dbus_set_error_from_message(&err, message))
	{
		printf("%s, %d:--------Agent replied with an error: %s, %s\n", __FILE__, __LINE__, err.name, err.message);
		if (strcmp(err.name, "org.bluez.Error.AlreadyExists"))
			rn = -1;
		
		dbus_error_free(&err);
		
	}
	else
	{
 	       printf("%s, %d:--------paired succeed\n", __FILE__, __LINE__);	    
	}


	if (message)
		dbus_message_unref(message);

	dbus_pending_call_cancel(pending);
	
	
#endif

	 __p_io_terminated = rn;

	return;
}


static int create_paired_device(DBusConnection *conn, const char *adapter_path,
						const char *agent_path,
						const char *capabilities,
						const char *device)
{
	dbus_bool_t success;
	DBusMessage *msg;
	DBusPendingCall *pending;

	msg = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter",
						"CreatePairedDevice");
	if (!msg)
	{
		LOG0("Can't allocate new method call\n");
		return -1;
	}

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &device,
					DBUS_TYPE_OBJECT_PATH, &agent_path,
					DBUS_TYPE_STRING, &capabilities,
					DBUS_TYPE_INVALID);
	LOG1("%s, %d creating paired device\n", __FILE__, __LINE__);
	exit_on_release = 0;
	success = dbus_connection_send_with_reply(conn, msg, &pending, -1);
	if (pending)
		dbus_pending_call_set_notify(pending,
						create_paired_device_reply,
						NULL, NULL);

	dbus_message_unref(msg);

	if (!success) 
	{
		LOG0("Not enough memory for message send\n");
		return -1;
	}

	dbus_connection_flush(conn);

	return 0;
}

static char *get_adapter_path(DBusConnection *conn, const char *adapter)
{
	DBusMessage *msg, *reply;
	DBusError err;
	const char *reply_path;
	char *path;

	if (!adapter)
		return NULL;

	msg = dbus_message_new_method_call("org.bluez", "/",
					"org.bluez.Manager", "FindAdapter");

	if (!msg) {
		printf("Can't allocate new method call\n");
		return NULL;
	}

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &adapter,
					DBUS_TYPE_INVALID);

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, &err);

	dbus_message_unref(msg);

	if (!reply)
	{
		printf("Can't find adapter %s\n", adapter);
		if (dbus_error_is_set(&err)) {
			printf("%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	if (!dbus_message_get_args(reply, &err,
					DBUS_TYPE_OBJECT_PATH, &reply_path,
					DBUS_TYPE_INVALID)) 
	{
		printf("Can't get reply arguments\n");
		if (dbus_error_is_set(&err)) 
		{
			printf("%s\n", err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}

	path = strdup(reply_path);

	dbus_message_unref(reply);

	dbus_connection_flush(conn);

	return path;
}

//获取已经配对设备
#if 0
int DRV_Get_Paired_Device (hci_dev_context_t *context, char *adapter_id,struct bt_device *pdev,int max)
{

    int result;
	char *adapter_path = get_adapter_path(context->dbus_conn, adapter_id);
	if (!adapter_path)
	{
		LOG0("Failed to get adapter [%s] path\n", adapter_id);
		return -1;
	}

	result = __get_paird_devices(context->dbus_conn,adapter_path,pdev,max);
	if(result < 0){
		printf("get paird devices failed\n");
	}
	
	free(adapter_path);

	
	return result;
}
#endif


//配对设备
int DRV_Pair_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr, char *passkey)
{
	if (passkey)
		passkey_value = passkey;
	else
		passkey_value = "0000";
	
	if (!remote_addr)
	{
		LOG0("Invalid remote address\n");
		return -1;
	}
	
	char *adapter_path = get_adapter_path(context->dbus_conn, adapter_id);
	if (!adapter_path)
	{
		LOG0("Failed to get adapter [%s] path\n", adapter_id);
		return -1;
	}
//    printf("[aweigh] 1111111\n");
	const char *capabilities = "DisplayYesNo";

	if (create_paired_device(context->dbus_conn, adapter_path, context->agent_path,
					capabilities, remote_addr) < 0)
	{
		LOG0("Failed to create_paired_device\n");
		return -1;
	}
//    printf("[aweigh] 222222222\n");

	if (!dbus_connection_add_filter(context->dbus_conn, agent_filter, NULL, NULL))
		LOG0("Can't add signal filter\n");

//    printf("[aweigh] 3333333333\n");

	char match_string[128];
	snprintf(match_string, sizeof(match_string),
			"interface=%s,member=NameOwnerChanged,arg0=%s",
			DBUS_INTERFACE_DBUS, "org.bluez");

	dbus_bus_add_match(context->dbus_conn, match_string, NULL);
//    printf("[aweigh] 444444444\n");

	while (!__p_io_canceled && !__p_io_terminated) 
	{
		if (dbus_connection_read_write_dispatch(context->dbus_conn, 500) != TRUE)
			break;
	}

	int rn = 1;
	if (__p_io_terminated <= 0)
		rn = -1;

	__p_io_canceled = __p_io_terminated = 0;
	passkey_value = NULL;
	
	free(adapter_path);
	
	return rn;
}


// 接受配对，一直等待直到cancel_wait_for_paired()被调用
int DRV_Wait_For_Paired (hci_dev_context_t *context, char *adapter_id, char *passkey)
{
	if (passkey)
		passkey_value = passkey;
	else
		passkey_value = "0000";

	
	char *adapter_path = get_adapter_path(context->dbus_conn, adapter_id);
	if (!adapter_path)
	{
		LOG0("Failed to get adapter [%s] path\n", adapter_id);
		return -1;
	}

	const char *capabilities = "DisplayYesNo";

	DBusMessage *msg = 0, *reply = 0;
	DBusError err;

	msg = dbus_message_new_method_call("org.bluez", adapter_path,
					"org.bluez.Adapter", "RegisterAgent");
	if (!msg) {
		fprintf(stderr, "Can't allocate new method call\n");
		return -1;
	}

	char * agent_path = context->agent_path;
	dbus_message_append_args(msg, DBUS_TYPE_OBJECT_PATH, &agent_path,
					DBUS_TYPE_STRING, &capabilities,
					DBUS_TYPE_INVALID);

	dbus_error_init(&err);

	reply = dbus_connection_send_with_reply_and_block(context->dbus_conn, msg, -1, &err);

	dbus_message_unref(msg);

	if (!reply) {
		fprintf(stderr, "Can't register agent\n");
		if (dbus_error_is_set(&err)) {
			fprintf(stderr, "%s\n", err.message);
			dbus_error_free(&err);
		}
		return -1;
	}

	dbus_message_unref(reply);

	dbus_connection_flush(context->dbus_conn);


	if (!dbus_connection_add_filter(context->dbus_conn, agent_filter, NULL, NULL))
		LOG0("Can't add signal filter\n");


	char match_string[128];
	snprintf(match_string, sizeof(match_string),
			"interface=%s,member=NameOwnerChanged,arg0=%s",
			DBUS_INTERFACE_DBUS, "org.bluez");

	dbus_bus_add_match(context->dbus_conn, match_string, NULL);

	while (!__w_io_canceled && !__w_io_terminated) 
	{
		printf ("wait for agent ------------[c=%d, w=%d]--------\n", __w_io_canceled, __w_io_terminated);
		if (dbus_connection_read_write_dispatch(context->dbus_conn, 500) != TRUE)
			break;
	}

	int rn = 1;
	if (__w_io_terminated <= 0)
		rn = -1;

	__w_io_canceled = __w_io_terminated = 0;
	passkey_value = NULL;

	//unregister_agent
	msg = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter", "UnregisterAgent");
	if (!msg) 
	{
		fprintf(stderr, "Can't allocate new method call\n");
		return -1;
	}
	
	dbus_message_append_args(msg, DBUS_TYPE_OBJECT_PATH, &agent_path,
								DBUS_TYPE_INVALID);
	
	reply = dbus_connection_send_with_reply_and_block(context->dbus_conn, msg, -1, &err);
	
	dbus_message_unref(msg);
	
	if (!reply) 
	{
		fprintf(stderr, "Can't unregister agent\n");
		if (dbus_error_is_set(&err)) 
		{
			fprintf(stderr, "%s\n", err.message);
			dbus_error_free(&err);
		}
		return -1;
	}
	
	dbus_message_unref(reply);

	dbus_connection_flush(context->dbus_conn);
	
//	dbus_connection_unregister_object_path(context->dbus_conn, agent_path);

	
	free(adapter_path);
	
	return rn;
}

void DRV_Cancel_Wait_For_Paired()
{
	__w_io_terminated = 1;

}

int DRV_Remove_Paired_Device (hci_dev_context_t *context, char *adapter_id, char *remote_addr)
{
	if (!remote_addr)
	{
		LOG0("Invalid remote address\n");
		return -1;
	}

	char *adapter_path = get_adapter_path(context->dbus_conn, adapter_id);
	if (!adapter_path)
	{
		LOG0("Failed to get adapter [%s] path\n", adapter_id);
		return -1;
	}

	LOG1("%s | > Get Adatper Path: %s\n", __FUNCTION__, adapter_path);
	char *dev_objpath = (char *)malloc(64);
	memset (dev_objpath, 0, 64);
	strcpy (dev_objpath, adapter_path);
	dev_objpath[strlen(adapter_path)] = '/';
	strcat(dev_objpath, "dev_");
	strcat(dev_objpath, remote_addr);
	int i = 0;
	while(dev_objpath[i])
	{
		if (dev_objpath[i] == ':')
			dev_objpath[i] = '_';
		i++;
	}
	LOG1("%s | > Get Removing Device Path: %s\n", __FUNCTION__, dev_objpath);

	DBusMessage *msg, *reply;	
	DBusError err;	
	char *path;

	msg = dbus_message_new_method_call("org.bluez", adapter_path,
						"org.bluez.Adapter",
						"RemoveDevice");
	if (!msg) 
	{
		free(dev_objpath);
		fprintf(stderr, "Can't allocate new method call\n");
		return -1;
	}

	dbus_message_append_args(msg, DBUS_TYPE_OBJECT_PATH, &dev_objpath ,
					DBUS_TYPE_INVALID);
	
	dbus_error_init(&err);	
	reply = dbus_connection_send_with_reply_and_block(context->dbus_conn, msg, -1, &err);	
	dbus_message_unref(msg);

	if (!reply)
	{		
		fprintf(stderr,	"Can't remove device %s\n", dev_objpath);		
		if (dbus_error_is_set(&err))
		{			
			fprintf(stderr, "%s\n", err.message);
			dbus_error_free(&err);
		}
		free(dev_objpath);
		return -1;	
	}

	dbus_message_unref(reply);	
	dbus_connection_flush(context->dbus_conn);
	free(dev_objpath);

	return 0;
}



static const DBusObjectPathVTable agent_table = {
	.message_function = agent_message,
};



static int agent_init(hci_dev_context_t *context )
{
	context->dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!context->dbus_conn)
	{
		LOG0("Can't get on system bus\n");
		return -1;
	}

	snprintf (context->agent_path, sizeof(context->agent_path), "/org/bluez/agent_%d", getpid());
	if (!dbus_connection_register_object_path(context->dbus_conn, context->agent_path,
							&agent_table, NULL)) 
	{
		LOG0("Can't register object path for agent\n");
		return -1;
	}

	return 0;
}

static int agent_uninit(hci_dev_context_t *context)
{
	context->dbus_conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
	if (!context->dbus_conn)
	{
		LOG0("agent_uninit   Can't get on system bus\n");
		return -1;
	}

	snprintf (context->agent_path, sizeof(context->agent_path), "/org/bluez/agent_%d", getpid());
	if (!dbus_connection_unregister_object_path(context->dbus_conn, context->agent_path)) 
	{
		LOG0("  agent_uninit   Can't register object path for agent\n");
		return -1;
	}
	dbus_connection_unref(context->dbus_conn);
	context->dbus_conn = NULL;


	return 0;
}

int DRV_Get_Remote_Device_List(hci_dev_context_t *context, unsigned short dev_id, dev_info_sample_t **dev_list)
{
	inquiry_info *ii;
	int len = 8;
	
	ii = malloc(MAX_REMOTE_DEV_COUNT*sizeof(*ii));
	LOG1 ("hci_inquiry start (dev_id=%u, ii=%p)...\n", dev_id, ii);
	int remote_dev_num = hci_inquiry(dev_id, len, MAX_REMOTE_DEV_COUNT, NULL, &ii, IREQ_CACHE_FLUSH);
	
	LOG1 ("Get remote devices count: %d\n", remote_dev_num);
    if(remote_dev_num <= 0)
    {
	    free(ii);
        return remote_dev_num;
    }
    
	int mem_size = sizeof(dev_info_sample_t) * remote_dev_num;
	*dev_list = (dev_info_sample_t *)malloc(mem_size);
	memset(*dev_list, 0, mem_size);
#if 0

	if (context->mmem[REMOTE_DEV_LIST])
    {
		free(context->mmem[REMOTE_DEV_LIST]);
        context->mmem[REMOTE_DEV_LIST] = NULL;
    }
    context->mmem[REMOTE_DEV_LIST] = *dev_list;
    context->mmem[REMOTE_DEV_LIST] = malloc(MAX_REMOTE_DEV_COUNT * sizeof(dev_info_sample_t));
    if(context->mmem[REMOTE_DEV_LIST] == NULL)
    {
        printf("[aweigh]  %s  %d   malloc   context->mmem[REMOTE_DEV_LIST] failed \n",__FUNCTION__,__LINE__);
       	free(ii);
        return -1;
    }
    
#endif
	int fd = hci_open_dev(dev_id);

	dev_info_sample_t *p;
	int i;
	for (i = 0; i < remote_dev_num; i++)
	{
		p = *dev_list+i;
		ba2str(&((ii+i)->bdaddr), p->addr);
		hci_read_remote_name(fd, &(ii+i)->bdaddr, sizeof(p->name), p->name, 0);

		LOG2 ("<%d> %s, %s\n", i, p->name, p->addr);		
	}
	close (fd);
	free(ii);
	
	return remote_dev_num;

}


int DRV_Get_Local_Device_List(hci_dev_context_t *context, dev_info_sample_t **dev_list)
{
	LOGTMP("TTTTTTTT start \n");

	int mem_size = sizeof(dev_info_sample_t) * context->dev_count;
	
	*dev_list = (dev_info_sample_t *)malloc(mem_size);
	memset(*dev_list, 0, mem_size);
#if 0

	if (context->mmem[LOCAL_DEV_LIST])
	{
		free(context->mmem[LOCAL_DEV_LIST]);
        context->mmem[LOCAL_DEV_LIST] = NULL;
    }
	context->mmem[LOCAL_DEV_LIST] = *dev_list;
    context->mmem[LOCAL_DEV_LIST] = 
#endif
	LOGTMP("TTTTTTT part1 OK \n");

	dev_info_sample_t *p;
	int i;
	for (i = 0; i < context->dev_count; i++)
	{
		p = *dev_list+i;
		p->dev_id = (context->dev_info_list+i)->dev_id;
		ba2str(&((context->dev_info_list+i)->bdaddr), p->addr);
		strcpy(p->name, (context->dev_info_list+i)->name);

//		printf ("<%d> %s, %s [%x,%x,%x]\n", i, str_addr, name, (ii+i)->dev_class[0], (ii+i)->dev_class[1], (ii+i)->dev_class[2]);
		LOG2 ("<%d> %s, %s\n", i, p->name, p->addr);		
	}

	return context->dev_count;
}


static int get_dev_addr(hci_dev_context_t *context, char *device)
{
//	ba2str(&(local_dev_info.dev_info.bdaddr), addr);

	return 0;
}

static int get_dev_name(hci_dev_context_t *context, char *device)
{
//	strcpy(name, local_dev_info.dev_info.name);

	return 0;
}

int DRV_ALSA_BTHeadset_Init(char *btaddr)
{
	int rn;
	int fd;

	if((fd = open("/.asoundrc", O_CREAT | O_TRUNC | O_WRONLY)) < 0)
	{
		LOG0("Can not open /.asoundrc\n");
	        return -1;
	}
	
	char str[512];
	memset(str, 0, sizeof(str));
#if 0
	sprintf(str, "pcm.bluetooth_record{\n \
		\rtype bluetooth\n \
		\rdevice %s\n \
		\rprofile voice\n \
		\r}\n\n \
		\rpcm.bluetooth_play{\n \
		\rtype bluetooth\n \
		\rdevice %s\n \
		\r}\n\n", btaddr, btaddr);
#else
	sprintf (str, "pcm.bt_record{\n \
	type plug\n \
	slave {\n \
	pcm \"bt_record_hw\"\n \
	}\n \
	}\n\n \
	pcm.bt_record_hw{\n \
	type bluetooth\n \
	device %s\n \
	profile voice\n\n \
	}\n\n \
	pcm.bt_play{\n \
	type plug\n \
	slave {\n \
	pcm \"bt_play_hw\"\n \
	}\n \
	}\n\n \
	pcm.bt_play_hw{\n \
	type bluetooth \n \
	device %s\n \
	}\n\n", btaddr, btaddr);

#endif

	rn = write(fd, str, strlen(str));
	close(fd);
	
	return rn;
}

int DRV_ALSA_Playback_Start(char *filename)
{
	
	return alsa_playback_start(filename);
}

void DRV_ALSA_Playback_Stop()
{
	LOG0("Stop playback...\n");
	alsa_playback_stop();
}

int DRV_ALSA_Capture_Start(char *filename, alsa_param_t *param)
{
	//set up default params for capture

	if (!param)
	{
		LOG0("Param is NULL, use default param\n");
		alsa_param_t ap;
		memset (&ap, 0, sizeof(alsa_param_t));
		ap.channel = 1;
		ap.sample_format = ALSA_SAMPLE_FORMAT_CD;
		ap.file_type = ALSA_FILETYPE_WAVE;
		ap.sample_rate = 8000;
		return alsa_capture_start(filename, &ap);
	}
	else
		return alsa_capture_start(filename, param);
}

void DRV_ALSA_Capture_Stop()
{
	LOG0("Stop capture...\n");

	alsa_capture_stop();
}



/*允许蓝牙设备被扫描(SCAN_INQUIRY)和被连接(SCAN_PAGE) */
static int enable_hci_piscan(int hci_fd, int dev_id)
{
	struct hci_dev_req dr;
	dr.dev_id = dev_id;
	dr.dev_opt = SCAN_INQUIRY | SCAN_PAGE;

	if (ioctl(hci_fd, HCISETSCAN, &dr) < 0)
	{
		fprintf(stderr, "fd=%d | Can't set scan mode: %s\n", hci_fd, strerror(errno));
		return -1;
	}

	LOG1("Enabled hci piscan, hci:%d \n", dev_id);
	return 0;
}

//  HCIDEVUP adapter  get local hci devices list
static int hci_dev_context_init(hci_dev_context_t *context)
{
	LOGTMP("TTT 1111start ...\n");
	/* Open HCI socket  */
    int dd; 
	int fd = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
	if (fd  < 0) 
	{
		perror("Can't open HCI socket.");
		return -1;
	}

	LOGTMP("socket OK\n");

	context->dev_info_list = (struct hci_dev_info*)malloc(sizeof(struct hci_dev_info) * HCI_MAX_DEV);
	memset (context->dev_info_list, 0, sizeof(sizeof(struct hci_dev_info) * HCI_MAX_DEV));
	
	struct hci_dev_list_req *dl;
	struct hci_dev_req *dr;
	int dev_id = -1;
	int i, err = 0;
	
	dl = malloc(HCI_MAX_DEV * sizeof(*dr) + sizeof(*dl));
	if (!dl) {
		err = errno;
		return -1;
	}
	
	memset(dl, 0, HCI_MAX_DEV * sizeof(*dr) + sizeof(*dl));
	
	dl->dev_num = HCI_MAX_DEV;
	dr = dl->dev_req;

	LOGTMP("TTTTTT 1\n");
	
	if (ioctl(fd, HCIGETDEVLIST, (void *) dl) < 0)
	{
		err = errno;
		goto free;
	}
	context->dev_count = dl->dev_num;

	LOGTMP("TTTTTT 2\n");

	
	//get the local hci devices list
	for (i = 0; i < dl->dev_num; i++) 
	{
		LOGTMP("TTTT (i=%d, dev_num=%d, fd=%d\n)", i, dl->dev_num, fd);
		(context->dev_info_list+i)->dev_id = (dr+i)->dev_id;

		/* startup hci device */
		if(ioctl(fd, HCIDEVUP, (dr+i)->dev_id) < 0)
		{
			LOGTMP("ioctl errorrrrrrrrrrr\n");
			/* 如果非'已UP'错误，则继续下一个设备 */
			if (errno != EALREADY)
				continue;
			else
				LOG0("Hci%d has upped already!!!\n", (dr+i)->dev_id);
				//fprintf(stderr, "Hci%d has upped already!!!\n", (dr+i)->dev_id);
		}
		else
			printf ("Hci%d UP!!!\n", (dr+i)->dev_id);

		LOGTMP("icol 11111111111111 OK\n");

		/* 获取本地Adapter信息*/
		if(ioctl(fd, HCIGETDEVINFO, (void *)(context->dev_info_list+i)) < 0)
		{
			perror("Can not get hci device infomation");
			continue;
		}

		LOGTMP("[aweigh1]icol  OK\n");
#if 0       //add by ganwei
    	if (hci_test_bit(HCI_RAW, &((context->dev_info_list+i)->flags)) &&
    			!bacmp_utils(&((context->dev_info_list+i)->bdaddr), BDADDR_ANY)) {
            dd = hci_open_dev((context->dev_info_list+i)->dev_id);
            if(dd < 0)
            {
                printf("[aweigh] hci_open_dev failed,dd = %d,(context->dev_info_list+i)->dev_id = %d\n ",dd,(context->dev_info_list+i)->dev_id);
            }
            else
            {
        		hci_read_bd_addr(dd, &((context->dev_info_list+i)->bdaddr), 1000);
        		hci_close_dev(dd);

            }

    	}
        else
        {
            printf("[aweigh]   check mac addr failed \n");

        }
#endif
        sleep(1);
		enable_hci_piscan(fd, (dr+i)->dev_id);
	}

	LOGTMP("TTTTTT OVER\n");

free:
	close(fd);
	free(dl);
	LOG0("init_hci_dev_context() | > %s\n", strerror(err));

	return err;
}


static int hci_dev_context_uninit(hci_dev_context_t *context)
{

//	shutdown the hci devices

	int fd = socket(AF_BLUETOOTH, SOCK_RAW, BTPROTO_HCI);
	if (fd  < 0) 
	{
		perror("Can't open HCI socket.");
		return -1;
	}

	int dev_id;
	int i;

//    printf("[aweigh]   context->dev_count = %d\n",context->dev_count);
	for (i = 0; i < context->dev_count; i++) 
	{
		/* startup hci device */
		if(ioctl(fd, HCIDEVDOWN, (context->dev_info_list+i)->dev_id) < 0)
		{
			if (errno != EALREADY)
				continue;
			else
				fprintf(stderr, "Hci%d has down already!!!\n", (context->dev_info_list+i)->dev_id);
		}
		else
			printf ("Hci%d DOWN!!!\n", (context->dev_info_list+i)->dev_id);
	}
	close(fd);

//    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);

	free (context->dev_info_list);
#if 0
    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);
	if (context->mmem[REMOTE_DEV_LIST])
	{
		free(context->mmem[REMOTE_DEV_LIST]);
        context->mmem[REMOTE_DEV_LIST] = NULL;
    }
    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);
	if (context->mmem[LOCAL_DEV_LIST])
	{
		free(context->mmem[LOCAL_DEV_LIST]);
        context->mmem[LOCAL_DEV_LIST] = NULL;
    }
#endif        

    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);
	return 0;
}


#if 1
//
// Called by the obex-layer when some event occurs.
//
static void obex_event(obex_t *handle, obex_object_t *object, int mode, int event, int obex_cmd, int obex_rsp)
{
	switch (event)	{
	case OBEX_EV_PROGRESS:
		LOG1("Made some progress...\n");
		break;

	case OBEX_EV_ABORT:
		printf("Request aborted!\n");
		break;

	case OBEX_EV_REQDONE:
		if(mode == OBEX_CLIENT) {
			client_done(handle, object, obex_cmd, obex_rsp);
		}
		else	{
			server_done(handle, object, obex_cmd, obex_rsp);
		}
		break;
	case OBEX_EV_REQHINT:
		/* Accept any command. Not rellay good, but this is a test-program :) */
		OBEX_ObjectSetRsp(object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
		break;

	case OBEX_EV_REQ:
		server_request(handle, object, event, obex_cmd);
		break;

	case OBEX_EV_LINKERR:
		OBEX_TransportDisconnect(handle);
		printf("Link broken!\n");
		break;

	case OBEX_EV_STREAMEMPTY:
		LOG1 ("%s | > case OBEX_EV_STREAMEMPTY!!!\n", __FUNCTION__);
		fillstream(handle, object);
		break;

	default:
//		printf("Unknown event %02x!\n", event);
		break;
	}
}


static int obex_init(hci_dev_context_t *context)
{
	char cmd[10];
	int end = 0;

	obex_t *handle;
	obex_interface_t *obex_intf;

//	obex_context_t global_context = {0,};
	int size = sizeof(obex_context_t);
	obex_context_t *global_context = (obex_context_t *)malloc(size);
	memset (global_context, 0, size);
#if 0
    if (context->mmem[OBEX_CONTEXT])
		free(context->mmem[OBEX_CONTEXT]);
	context->mmem[OBEX_CONTEXT] = global_context;
#endif
	//如果没有OOP服务，添加，channel=9
	if (DRV_SDP_Get_OOP_Channel("ff:ff:ff:00:00:00") < 0)
	{
        
		sdp_add_oop_service(OOP_DEF_CHANNEL);
//        printf("[aweigh]   add  channel 10  \n");
    }

	char *port;
	obex_ctrans_t custfunc;

	LOG1("Using Bluetooth RFCOMM transport\n");
	if(!(handle = OBEX_Init(OBEX_TRANS_BLUETOOTH, obex_event, 0))) 
	{
		perror( "OBEX_Init failed");
		return -1;
	}

//	OBEX_SetUserData(handle, &global_context);
	OBEX_SetUserData(handle, global_context);

	context->obex_handle = handle;
	//第一次需要注册接收server
	context->obex_serv_rereg = 1;		
	
	LOG0( "OBEX Initialized Success !! \n");

	return 0;
}

static int obex_uninit(hci_dev_context_t *context)
{
	OBEX_Cleanup(context->obex_handle);
#if 0
	if (context->mmem[OBEX_CONTEXT])
	{
		free(context->mmem[OBEX_CONTEXT]);
        context->mmem[OBEX_CONTEXT] = NULL;
    }
#endif
	return 0;
}

int DRV_SDP_Get_Service_Channel(char *dest_addr, char *service_name)
{
	return get_oop_channel(dest_addr, service_name);
}

int DRV_SDP_Get_OOP_Channel(char *dest_addr)
{
	return get_oop_channel(dest_addr, "OBEX Object Push");
}

int sdp_add_oop_service(int channel)
{
	return sdp_add_service("opush", channel);
}


int sdp_add_service(char *service_name, int channel)
{
	if (service_name == NULL || channel < 0)
		return -1;
	
	svc_info_t si;
	memset(&si, 0, sizeof(si));
	si.handle = 0xffffffff;
	si.name = service_name;
	si.channel = channel;
	
	sdp_session_t *sess = sdp_connect(BDADDR_LOCAL, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
	if (!sess)
		return -1;

	int rn = add_opush(sess, &si);

	sdp_close(sess);
	
	
	return rn;
}

int DRV_OBEX_Connect(hci_dev_context_t *context, char *dest_addr, int oop_channel)
{
	LOG1("obex_connect: dest_addr=%s, oop_channel=%d\n", dest_addr, oop_channel);
	
	bdaddr_t bdaddr;
	str2ba(dest_addr, &bdaddr);

	if (bacmp(&bdaddr, BDADDR_ANY) == 0) 
	{
		LOG0("Device address error! (Bluetooth)\n");
		return -1;
	}
	
	if(BtOBEX_TransportConnect(context->obex_handle, BDADDR_ANY, &bdaddr, oop_channel) <0) 
	{
		LOG0("Transport connect error! (Bluetooth)\n");
		return -1;
	}
	
	// Now send OBEX-connect.	
	connect_client(context->obex_handle);

	return 0;
}

int DRV_OBEX_Disconnect(hci_dev_context_t *context)
{
	disconnect_client(context->obex_handle);

	return 0;
}


int DRV_OBEX_Push_Object(hci_dev_context_t *context, char *filename)
{
	push_client(context->obex_handle, filename);

	return 0;
}

int DRV_OBEX_Server_Register(hci_dev_context_t *context)
{
	// 如果已经创建socket，，则跳过reg，直接返回
	if (context->obex_serv_rereg == 0)
		return 0;
	int local_oop_channel = DRV_SDP_Get_OOP_Channel("ff:ff:ff:00:00:00");
//	int local_oop_channel = DRV_SDP_Get_OOP_Channel("00:37:6D:BF:4A:88");
	if (local_oop_channel < 0)
	{
		LOG0("Can not obtain local OOP channel\n");
		return -1;
	}
	
	LOG1("Get OOP channel: %d\n", local_oop_channel);

	//BtOBEX_ServerRegister()建立rfcomm socket并listen
	int rn = BtOBEX_ServerRegister(context->obex_handle, BDADDR_ANY, local_oop_channel);
	if(rn < 0) 
	{
//		LOG0("Obex server register faild: %s\n", strerror(rn));
		LOG0("Server register error! (Bluetooth)\n");
		return -1;
	}

//除非文件接收成功，否则均不在注册server，因为当文件接受成功后，obex接口里会把server unregister
	context->obex_serv_rereg = 0;

	return 0;
}

int DRV_OBEX_Accept(hci_dev_context_t *context, int timeout)
{
	//accept
	return server_do(context, timeout);

}

int DRV_OBEX_Receive_Object(hci_dev_context_t *context, char *recv_path, int timeout)
{
	//建立接收目录
	if (!recv_path)
	{
		LOG0("Invalid receving path!!\n");
		return -1;
	}

	if (recv_path[strlen(recv_path)] == '/')
		recv_path[strlen(recv_path)]  = 0;
	strcpy(bt_filepath, recv_path);
	char cmd[128];
	sprintf(cmd, "mkdir -p %s", bt_filepath);
	LOG1("Create directory for receving files: %s\n", cmd);
	system(cmd);

	//receive
	return server_do(context, timeout);

}

#endif 

static int daemon_start()
{
#if 0
	char *path_copy = get_current_dir_name();

	chdir(DAEMON_PATH);
	int rn = system(DAEMON_START_SH);
	if (rn < 0)
		return -1;

	chdir(path_copy);
	free (path_copy);
#endif

	int rn[3] = {0};
	system("mkdir -p /var/run/dbus");
	rn[0] = system("rm -rf /usr/ipphone/var/run/messagebus.pid");
	rn[1] = system("/usr/ipphone/bin/dbus-daemon --system");
	rn[2] = system("/usr/ipphone/sbin/bluetoothd -d");

	sleep(3);
	
	if (rn[0] < 0 ||rn[1] <  0 || rn[2] < 0)
	{
		printf("daemon_start() failed\n");
		return -1;
	}
	else
	{
		printf("daemon_start() successed\n");
		return 0;
	}

	
}

static int daemon_stop()
{
	return 0;
}


int DRV_BT_Init(hci_dev_context_t *context)
{
#if 0
	if (daemon_start() < 0)
	{
		LOG0("Failed to initizlize bluetooth daemon\n");
		return -1;
	}
#endif

	memset(context, 0, sizeof(hci_dev_context_t));
	LOGTMP("memset OK !!! \n");

	fprintf(stderr, "xxxxxxxxxxxx\n");
	if (hci_dev_context_init(context) < 0)
	{
		LOG0("Failed to initizlize hci_dev_context\n");
		return -1;
	}

	LOGTMP("TTTTTTTTTTTMP 1 \n");
	
	if (agent_init(context) < 0)
	{
		LOG0("Failed to initizlize agent\n");
		return -1;
	}

	LOGTMP("TTTTTTTTTTTMP 2 \n");
#if 1
	if (obex_init(context) < 0)
	{
		LOG0("Failed to initizlize OBEX\n");
		return -1;
	}
#endif
	LOG0("Initialized !!! \n");

	LOG2("Bluetooth has been initialized\n");
	
	return 0;
}


int DRV_BT_Uninit(hci_dev_context_t *context)
{
//	daemon_stop();

	hci_dev_context_uninit(context);
//    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);
	agent_uninit(context);
//    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);
    
	obex_uninit(context);
//    printf("[aweigh]   %s  %d\n",__FUNCTION__,__LINE__);

	return 0;
}



