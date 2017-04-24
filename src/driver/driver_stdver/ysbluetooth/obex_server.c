/*********************************************************************
 *                
 * Filename:      obex_test_server.c
 * Version:       0.8
 * Description:   Server OBEX Commands
 * Status:        Experimental.
 * Author:        Pontus Fuchs <pontus.fuchs@tactel.se>
 * Created at:    Sun Aug 13 03:00:28 PM CEST 2000
 * Modified at:   Sun Aug 13 03:00:33 PM CEST 2000
 * Modified by:   Pontus Fuchs <pontus.fuchs@tactel.se>
 * 
 *     Copyright (c) 2000, Pontus Fuchs, All Rights Reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>

#include <openobex/obex.h>

#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include "obex_io.h"
#include "bluetooth_utils.h"
#include "obex_server.h"

//#include "obex_transport.h"

//#include "extend.h"

#define TRUE  1
#define FALSE 0


void put_server(obex_t *handle, obex_object_t *object)
{
	obex_headerdata_t hv;
	uint8_t hi;
	uint32_t hlen;

	const uint8_t *body = NULL;
	int body_len = 0;
	char *name = NULL;
	char *namebuf = NULL;

	printf("%s()\n", __FUNCTION__);

	while (OBEX_ObjectGetNextHeader(handle, object, &hi, &hv, &hlen))	{
		switch(hi)	{
		case OBEX_HDR_BODY:
			printf("%s() Found body\n", __FUNCTION__);
			body = hv.bs;
			body_len = hlen;
			break;
		case OBEX_HDR_NAME:
			printf("%s() Found name\n", __FUNCTION__);
			if( (namebuf = malloc(hlen / 2)))	{
				OBEX_UnicodeToChar((uint8_t *) namebuf, hv.bs, hlen);
				name = namebuf;
			}
			break;
		
		default:
			printf("%s() Skipped header %02x\n", __FUNCTION__, hi);
		}
	}
	if(!body)	{
		printf("Got a PUT without a body\n");
		return;
	}
	if(!name)	{
		name = "OBEX_PUT_Unknown_object";
		printf("Got a PUT without a name. Setting name to %s\n", name);

	}
	safe_save_file(name, body, body_len);
	free(namebuf);
}

//
//
//
void get_server(obex_t *handle, obex_object_t *object)
{
	uint8_t *buf;

	obex_headerdata_t hv;
	uint8_t hi;
	uint32_t hlen;
	int file_size;

	char *name = NULL;
	char *namebuf = NULL;

	printf("%s()\n", __FUNCTION__);

	while (OBEX_ObjectGetNextHeader(handle, object, &hi, &hv, &hlen))	{
		switch(hi)	{
		case OBEX_HDR_NAME:
			printf("%s() Found name\n", __FUNCTION__);
			if( (namebuf = malloc(hlen / 2)))	{
				OBEX_UnicodeToChar((uint8_t *) namebuf, hv.bs, hlen);
				name = namebuf;
			}
			break;
		
		default:
			printf("%s() Skipped header %02x\n", __FUNCTION__, hi);
		}
	}

	if(!name)	{
		printf("%s() Got a GET without a name-header!\n", __FUNCTION__);
		OBEX_ObjectSetRsp(object, OBEX_RSP_NOT_FOUND, OBEX_RSP_NOT_FOUND);
		return;
	}
	printf("%s() Got a request for %s\n", __FUNCTION__, name);

	buf = easy_readfile(name, &file_size);
	if(buf == NULL) {
		printf("Can't find file %s\n", name);
		OBEX_ObjectSetRsp(object, OBEX_RSP_NOT_FOUND, OBEX_RSP_NOT_FOUND);
		return;
	}

	OBEX_ObjectSetRsp(object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
	hv.bs = buf;
	OBEX_ObjectAddHeader(handle, object, OBEX_HDR_BODY, hv, file_size, 0);
	hv.bq4 = file_size;
	OBEX_ObjectAddHeader(handle, object, OBEX_HDR_LENGTH, hv, sizeof(uint32_t), 0);
	free(buf);
	return;
}

//
//
//
void connect_server(obex_t *handle, obex_object_t *object)
{
	obex_headerdata_t hv;
	uint8_t hi;
	uint32_t hlen;

	const uint8_t *who = NULL;
	int who_len = 0;
	printf("%s()\n", __FUNCTION__);

	while (OBEX_ObjectGetNextHeader(handle, object, &hi, &hv, &hlen))	{
		if(hi == OBEX_HDR_WHO)	{
			who = hv.bs;
			who_len = hlen;
		}
		else	{
			printf("%s() Skipped header %02x\n", __FUNCTION__, hi);
		}
	}
	if (who_len == 6)	{
		if(memcmp("Linux", who, 6) == 0)	{
			printf("Weeeha. I'm talking to the coolest OS ever!\n");
		}
	}
	OBEX_ObjectSetRsp(object, OBEX_RSP_SUCCESS, OBEX_RSP_SUCCESS);
}

//
//
//
void server_request(obex_t *handle, obex_object_t *object, int event, int cmd)
{
	switch(cmd)	{
	case OBEX_CMD_CONNECT:
		connect_server(handle, object);
		break;
	case OBEX_CMD_DISCONNECT:
		printf("We got a disconnect-request\n");
		OBEX_ObjectSetRsp(object, OBEX_RSP_SUCCESS, OBEX_RSP_SUCCESS);
		break;
	case OBEX_CMD_GET:
		/* A Get always fits one package */
		get_server(handle, object);
		break;
	case OBEX_CMD_PUT:
		OBEX_ObjectSetRsp(object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
		put_server(handle, object);
		break;
	case OBEX_CMD_SETPATH:
		printf("Got a SETPATH request\n");
		OBEX_ObjectSetRsp(object, OBEX_RSP_CONTINUE, OBEX_RSP_SUCCESS);
		break;
	default:
		printf("%s() Denied %02x request\n", __FUNCTION__, cmd);
		OBEX_ObjectSetRsp(object, OBEX_RSP_NOT_IMPLEMENTED, OBEX_RSP_NOT_IMPLEMENTED);
		break;
	}
	return;
}

//
//
//
void server_done(obex_t *handle, obex_object_t *object, int obex_cmd, int obex_rsp)
{
	obex_context_t *gt;
	gt = OBEX_GetUserData(handle);

	printf("Server request finished!\n");

	switch (obex_cmd) {
	case OBEX_CMD_DISCONNECT:
		printf("Disconnect done!\n");
		OBEX_TransportDisconnect(handle);
		gt->serverdone = TRUE;
		break;

	default:
		printf("%s() Command (%02x) has now finished\n", __FUNCTION__, obex_cmd);
		break;
	}
}

#if 0
int obex_handle_input(obex_t *self, int timeout);
{
	obex_return_val_if_fail(self != NULL, -1);

	int ret;
	
	if(self->trans.type == OBEX_TRANS_CUSTOM) {
		if(self->ctrans.handleinput)
			ret = self->ctrans.handleinput(self, self->ctrans.customdata, timeout);
		else {
			DEBUG(4, "No handleinput-callback exist!\n");
			ret = -1;
		}
	}
	else if (self->trans.type == OBEX_TRANS_USB) {
		ret = obex_data_indication(self, NULL, 0);
	}
	else {
		struct timeval time;
		fd_set fdset;
		int highestfd = 0;
	
		DEBUG(4, "\n");
		obex_return_val_if_fail(self != NULL, -1);

		/* Check of we have any fd's to do select on. */
		if(self->fd < 0 && self->serverfd < 0) {
			DEBUG(0, "No valid socket is open\n");
			return -1;
		}

		time.tv_sec = timeout;
		time.tv_usec = 0;

		/* Add the fd's to the set. */
		FD_ZERO(&fdset);
		if(self->fd >= 0) {
			FD_SET(self->fd, &fdset);
				highestfd = self->fd;
		}
		if(self->serverfd >= 0) {
			FD_SET(self->serverfd, &fdset);
			if(self->serverfd > highestfd)
				highestfd = self->serverfd;
		}

		/* Wait for input */
		ret = select(highestfd+1, &fdset, NULL, NULL, &time);
	
		/* Check if this is a timeout (0) or error (-1) */
		if (ret < 1)
			return ret;
	
		if( (self->fd >= 0) && FD_ISSET(self->fd, &fdset)) {
			DEBUG(4, "Data available on client socket\n");
			ret = obex_data_indication(self, NULL, 0);
		}

		else if( (self->serverfd >= 0) && FD_ISSET(self->serverfd, &fdset)) {
			DEBUG(4, "Data available on server socket\n");
			/* Accept : create the connected socket */
			ret = obex_transport_accept(self);

			/* Tell the app to perform the OBEX_Accept() */
			if(self->keepserver)
				obex_deliver_event(self, OBEX_EV_ACCEPTHINT,
						   0, 0, FALSE);
			/* Otherwise, just disconnect the server */
			if((ret >= 0) && (! self->keepserver)) {
				obex_transport_disconnect_server(self);
			}
		}
		else
			ret = -1;
	}
	return ret;
}
#endif


//
//
//
int server_do(hci_dev_context_t *context, int timeout)
{
        int ret;
	obex_context_t *gt;
	gt = OBEX_GetUserData(context->obex_handle);

	gt->serverdone = FALSE;

	struct timeval time[2];
	
	while(!gt->serverdone) {
		gettimeofday(&time[0], 0);
	        ret = OBEX_HandleInput(context->obex_handle, timeout);
		gettimeofday(&time[1], 0);
//		ret = obex_handle_input(handle, timeout);

		if(ret < 0)
		{
			printf("Error while doing OBEX_HandleInput()\n");
			return -1;
		}
		else if (ret == 0) 
		{
			//区分是接收到连接还是真正超时
			if (time[1].tv_sec - time[0].tv_sec < timeout - 1)
			{
				printf("!!! Data available, receive it\n");
				return 0;
			}
//			OBEX_TransportDisconnect(handle);
//			OBEX_Cleanup(handle);


			printf("Timeout while doing OBEX_HandleInput()\n");
			return -1;
		}
		else
		{
			//接收结束，下次接收需要重新注册server
			if (context->obex_handle)
				context->obex_serv_rereg = 1;
			printf("OBEX_HandleInput() returned %d\n",ret);
		}
	}


}


#if 0
void server_do(obex_t *handle)
{
        int ret;
	struct context *gt;
	gt = OBEX_GetUserData(handle);

	gt->serverdone = FALSE;
	while(!gt->serverdone) {
	        ret = OBEX_HandleInput(handle, 10);
		if(ret < 0) {
			printf("Error while doing OBEX_HandleInput()\n");
			break;
		} else if (ret == 0) {
			printf("Timeout while doing OBEX_HandleInput()\n");
			break;
		} else {
			printf("OBEX_HandleInput() returned %d\n",ret);
		}
	}
}

#endif

