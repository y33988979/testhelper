/*
 * Copyright (C) ychen
 * Copyright (C) Jiuzhou, Inc.
 */
 
#include <hlp_config.h>
#include <hlp_core.h>
#include <hlp_ring.h>
#include <hlp_log.h>


//����������
hlp_ring_t* hlp_ring_create(size_t size)
{
	//assert(buffer);
	hlp_ring_t *ring = NULL;
	if (!is_power_of_2(size))
	{
		fprintf(stderr,"size=%d, error: size must be power of 2.\n", size);
		LOG_WRITE_ERR("hlp_ring_create size=%d, error: size must be power of 2.\n", size);
		return NULL;
	}
	ring = (hlp_ring_t *)malloc(sizeof(hlp_ring_t));
	if (!ring)
	{
		fprintf(stderr,"Failed to malloc 'ring->buffer' memory,errno:%u,reason:%s",
		errno, strerror(errno));
		LOG_WRITE_ERR("hlp_ring_create malloc failed. size=%d\n", size);
		return NULL;
	}
	memset(ring, 0, sizeof(hlp_ring_t));
	
	ring->buffer = (hlp_buf_t *)malloc(size * sizeof(hlp_buf_t));
	if(!ring->buffer)
	{
		free(ring);
		printf("Failed to malloc 'ring->buffer' memory \n");
		LOG_WRITE_ERR("hlp_ring_create malloc failed\n");
		return NULL;
	}
	
	memset(ring->buffer, 0, sizeof(hlp_buf_t) * size);
	ring->size = size;
	ring->count = 0;
	ring->write = 0;
	ring->read = 0;
	pthread_mutex_init(&ring->lock, NULL);
	
	return ring;
}

//�ͷŻ�����
void hlp_ring_destroy(hlp_ring_t *ring)
{
	pthread_mutex_destroy(&ring->lock);
	if(ring->buffer)
	{
		free(ring->buffer);
		ring->buffer = NULL;
	}
	if(ring)
	{
		free(ring);
	}
}

//�����������ݸ���
hlp_u32_t hlp_ring_get_count(hlp_ring_t *ring)
{
	return ring->count;
}

//�ӻ�������ȡ����
hlp_u32_t ring_pkt_get(hlp_ring_t *ring, hlp_buf_t *buffer)
{
	//assert(pkt);
	pthread_mutex_lock(&ring->lock);
	if(ring->count == 0)
	{
		printf("ring buffer is empty!!\n");
		LOG_WRITE_ERR("ring buffer is empty!!\n");
		pthread_mutex_unlock(&ring->lock);
		return HLP_ERROR;
	}
	buffer->len = ring->buffer[ring->read].len;
	memcpy(buffer->data, ring->buffer[ring->read].data, ring->buffer[ring->read].len);
	ring->read = (ring->read + 1) % ring->size;
	ring->count--;
	pthread_mutex_unlock(&ring->lock);
	
	return HLP_OK;
	
}
//�򻺳����д������
hlp_u32_t ring_pkt_put(hlp_ring_t *ring, hlp_buf_t *buffer)
{
	//assert(pkt);
	pthread_mutex_lock(&ring->lock);
	if(ring->count == ring->size-1)
	{
		printf("ring buffer is full!!\n");
		LOG_WRITE_ERR("ring buffer is full!!\n");
		pthread_mutex_unlock(&ring->lock);
		return -1;
	}
	ring->buffer[ring->read].len = buffer->len;
	memcpy(ring->buffer[ring->read].data, buffer->data, ring->buffer[ring->read].len);
	ring->write = (ring->write + 1) % ring->size;
	ring->count++;
	pthread_mutex_unlock(&ring->lock);
	
	return 0;
}

