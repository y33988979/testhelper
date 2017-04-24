/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#ifndef __HLP_RING_H__ 
#define __HLP_RING_H__

#include <hlp_config.h>
#include <hlp_core.h>

#define HLP_BUFFER_MAX_LEN      128

typedef struct hlp_buf_s  hlp_buf_t;

struct hlp_buf_s
{
	hlp_u32_t len;
	hlp_u8_t  data[HLP_BUFFER_MAX_LEN];
};

typedef struct hlp_ring_s  hlp_ring_t;

struct hlp_ring_s
{
	hlp_u32_t       size;       //大小
	hlp_u32_t       write;      //写位置
	hlp_u32_t       read;       //读位置
	hlp_u32_t       count;      //数据个数
	hlp_buf_t      *buffer;     //缓冲区
	pthread_mutex_t lock;     //互斥量
};

hlp_ring_t* hlp_ring_create(size_t size);

void hlp_ring_destroy(hlp_ring_t *ring);

hlp_u32_t hlp_ring_get_count(hlp_ring_t *ring);

hlp_u32_t ring_pkt_get(hlp_ring_t *ring, hlp_buf_t *buffer);

hlp_u32_t ring_pkt_put(hlp_ring_t *ring, hlp_buf_t *buffer);


#endif /* __HLP_RING_H__ */
 
 