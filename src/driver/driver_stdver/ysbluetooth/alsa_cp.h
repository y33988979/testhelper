#ifndef __ALSA_CP_H__
#define __ALSA_CP_H__

#define ALSA_PCMDEV_BT_PLAYBACK		"bt_play"
#define ALSA_PCMDEV_BT_CAPTURE		"bt_record"


enum alsa_sample_format
{
	ALSA_SAMPLE_FORMAT_CD,
};

enum alsa_filetype
{
	ALSA_FILETYPE_DEFAULT	= -1,
	ALSA_FILETYPE_RAW		= 0,
	ALSA_FILETYPE_VOC		= 1,
	ALSA_FILETYPE_WAVE		= 2,
	ALSA_FILETYPE_AU		= 3,

};


typedef struct alsa_param
{
// for capture only:
	int channel;
	int sample_format;
	int sample_rate;
	int file_type;
}alsa_param_t;


int alsa_playback_start(char *filename);
void alsa_playback_stop();
int alsa_capture_start(char *filename, alsa_param_t *param);
void alsa_capture_stop();

#endif


