#ifndef __SDPTOOL__
#define __SDPTOOL__


/*
 * Support for Service (de)registration
 */
typedef struct {
	uint32_t handle;
	char *name;
	char *provider;
	char *desc;
	unsigned int class;
	unsigned int profile;
	uint16_t psm;
	uint8_t channel;
	uint8_t network;
} svc_info_t;


int adp_get_oop_channel(char *dest_addr);
int add_opush(sdp_session_t *session, svc_info_t *si);



#endif
