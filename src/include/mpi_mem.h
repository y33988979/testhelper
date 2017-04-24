/******************************************************************************

Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : mpi_mem.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2006-07-18
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/

#ifndef __MPI_MEM_H__
#define __MPI_MEM_H__

#include "hi_type.h"
#include "hi_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */



/******************************* MPI declaration *****************************/

/*malloc memory for mmz and map to user-state address, bufname and bufsize used to input,
	physic address and user-state used to output*/
/*CNcomment: ����mmz�ڴ棬��ӳ���û�̬��ַ,bufname��bufsize��Ϊ����, �����ַ���û�̬�����ַ��Ϊ��� */
HI_S32 HI_MPI_MMZ_Malloc(HI_MMZ_BUF_S *pBuf);

/*free user-state map, release mmz memory, make sure that physic address, user-state address and lengh is right*/
/*CNcomment: ����û�̬��ַ��ӳ�䣬���ͷ�mmz�ڴ�,��֤����������ַ���û�̬�����ַ�ͳ�����ȷ*/
HI_S32 HI_MPI_MMZ_Free(HI_MMZ_BUF_S *pBuf);

/*malloc mmz memory for appointed mmz name, return physic address*/
/*CNcomment: ָ��mmz����������mmz�ڴ棬���������ַ*/
HI_VOID *HI_MPI_MMZ_New(HI_U32 size , HI_U32 align, HI_CHAR *mmz_name, HI_CHAR *mmb_name);

/*delete mmz memory*/
/*CNcomment: �ͷ�mmz�ڴ� */
HI_S32 HI_MPI_MMZ_Delete(HI_U32 phys_addr);

/*get physic address accordint to virtual address*/
/**CNcomment:  ���������ַ����ȡ��Ӧ�������ַ */
HI_S32 HI_MPI_MMZ_GetPhyAddr(HI_VOID *refaddr, HI_U32 *phyaddr, HI_U32 *size);

/*map physic address of mmz memory to user-state virtual address, can appoint whether cached*/
/**CNcomment:  ��mmz����������ַӳ����û�̬�����ַ������ָ���Ƿ�cached*/
HI_VOID *HI_MPI_MMZ_Map(HI_U32 phys_addr, HI_U32 cached);

/*unmap user-state address of mmz memory*/
/**CNcomment:  ���mmz�ڴ��û�̬��ַ��ӳ�� */
HI_S32 HI_MPI_MMZ_Unmap(HI_U32 phys_addr);

/*for cached memory, flush cache to memory*/
/**CNcomment:  ����cached�ڴ棬ˢDcache���ڴ� */
HI_S32 HI_MPI_MMZ_Flush(HI_U32 phys_addr);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __MPI_MEM_H__ */
