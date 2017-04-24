
#include "volume_id_internal.h"

int get_label_uuid(const char *device, char **label, char **uuid, int iso_only)
{
	int rv = 1;
	uint64_t size;
	struct volume_id *vid;

	vid = volume_id_open_node(device);
	if (!vid)
		return rv;

	if (ioctl(vid->fd, BLKGETSIZE64, &size) != 0)
		size = 0;

#if ENABLE_FEATURE_VOLUMEID_ISO9660
	if ((iso_only ?
	     volume_id_probe_iso9660(vid, 0) :
	     volume_id_probe_all(vid, 0, size)
	    ) != 0
	) {
		goto ret;
	}
#else
	if (volume_id_probe_all(vid, 0, size) != 0) {
		goto ret;
	}
#endif
//    vid->needcheck = 1;
    if(vid->needcheck == 1)
    {
        char cmd[100];
        sprintf(cmd,"fsck -p %s\n",device);
        system_vfork(cmd);
    }

	if (vid->label[0] != '\0' || vid->uuid[0] != '\0') {
		*label = strndup(vid->label, sizeof(vid->label));
		*uuid  = strndup(vid->uuid, sizeof(vid->uuid));
		dbg("found label '%s', uuid '%s' on %s", *label, *uuid, device);
		rv = 0;
	}
 ret:
	free_volume_id(vid);
	return rv;
}
