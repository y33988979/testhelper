#ifndef OBEX_TEST_SERVER_H
#define OBEX_TEST_SERVER_H

#include "bluetooth_utils.h"

int server_do(hci_dev_context_t *context, int timeout);
void server_done(obex_t *handle, obex_object_t *object, int obex_cmd, int obex_rsp);
void server_request(obex_t *handle, obex_object_t *object, int event, int cmd);

#endif
