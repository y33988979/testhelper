/*****************************************************************************
*
* Description: 
*    椹卞姩灞侰AS妯″潡瀹炵幇鏂囦欢
*
* Revision History:
*    Date          Author         Modification
* ---------------------------------------------------------------------
* 1. 2005-10-12    Qiujordan      鍒涘缓
*
*****************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <stddefs.h>
#include "hi_unf_ecs.h"
#include "drv_cas.h"
#include "drv_os.h"

HI_CHAR rcv_buf[512];

#define TestNegotiation	0
#define TestSetETU		0	
#define TestOther		0
//#define T0_transfer     0


#if 1
/******************t0_irdeto_zeta*************************************/

char t0_irdeto_zeta_send[][22] = {
    {0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00, 0x19},
    {0xd2, 0x40, 0x00, 0x00, 0x11, 
     0x03, 0x06, 0x14, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00, 0x09},
    {0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22},
    {0xd2, 0xfe, 0x00, 0x00, 0x15},
    {0xd2, 0x3e, 0x00, 0x00, 0x01, 0x23},
    {0xd2, 0xfe, 0x00, 0x00, 0x0b},
    {0xd2, 0x00, 0x00, 0x00, 0x01, 0x3c},
    {0xd2, 0xfe, 0x00, 0x00, 0x1d},
};
char t0_irdeto_zeta_rcv[][32] = {
    {0x90,0x19},
    {0x01, 0x02, 0x00, 0x00,
     0x21, 0x00, 0x00, 0x10, 0x03,
     0x06, 0x14, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x1c, 0x90, 0x00},
    {0x90,0x09},
    {0x01, 0x02, 0x00, 0x00,
     0x20, 0x00, 0x00, 0x00, 0x1c,
     0x90, 0x00},
    {0x90,0x15},
    {0x01, 0x02, 0x00, 0x00,
     0x1e, 0x00, 0x00, 0x0c, 0x53,
     0x36, 0x01, 0x0e, 0x00, 0x08,
     0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x4c, 0x90, 0x00},
    {0x90,0x0b},
    {0x01, 0x02, 0x00, 0x00,
     0x1f, 0x00, 0x00, 0x02, 0x00,
     0x02, 0x23, 0x90, 0x00},
    {0x90,0x1d},
    {0x01, 0x02, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x14, 0x34,
     0x30, 0x32, 0x30, 0x33, 0x32,
     0x35, 0x31, 0x35, 0x32, 0x54,
     0x36, 0x33, 0x38, 0x35, 0x30,
     0x30, 0x30, 0x41, 0x00, 0x01,
     0x90, 0x00},
};

int t0_irdeto_zeta_send_len[10] = {6, 5, 22, 5, 6, 5, 6, 5, 6, 5,};
int t0_irdeto_zeta_rcv_len[10] = { 2, 27, 2, 11, 2, 23, 2, 13, 2, 31};

#endif

#if 0
/******************t0_irdeto_ICE*************************************/

char t0_irdeto_zeta_send[][17] = {
    {0xd2, 0x42, 0x00, 0x00, 0x01, 0x1d},
    {0xd2, 0xfe, 0x00, 0x00, 0x19},
    {0xd2, 0x3c, 0x00, 0x00, 0x01, 0x22},
    {0xd2, 0xfe, 0x00, 0x00, 0x15},
};

char t0_irdeto_zeta_rcv[][32] = {
    {0x90, 0x019},
    {0x1   ,0x2   ,0   ,0,
    0x21   ,0   ,0   ,0x10   ,0x3,
    0x6   ,0x14   ,0   ,0   ,0,
    0   ,0   ,0   ,0   ,0,
    0,   0   ,0   ,0   ,0,
    0x1c, 0x90,0x00},
    {0x90, 0x15},
    { 0x1, 0x2   ,0   ,0,
    0x1e   ,0   ,0, 0xc   ,0x57,
    0x12   ,0x1   ,0x93   ,0   ,0xd,
    0   ,0   ,0   ,0   ,0,
    0, 0xf4 , 0x90, 0x00},
};


int t0_irdeto_zeta_send_len[4] = { 6,5,6,5};
int t0_irdeto_zeta_rcv_len[4] = { 2,27,2, 23};

#endif 

#if 0

/**************************T1 suantong******************************************/
char atr_suantong_t1[18] = {0x3b, 0xe9, 0x00, 0x00, 0x81,
                            0x31, 0xc3, 0x45, 0x99, 0x63,
                            0x74, 0x69, 0x19, 0x99, 0x12,
                            0x56, 0x10, 0xec};
int atr_suantong_t1_cnt = 18;

char t1_suantong_send[][18] = {
    {0x00, 0x00, 0x05, 0x00, 0x25, 0xa0, 0x21, 0x20, 0x81},
    {0x00, 0x00, 0x05, 0x80, 0xb5, 0x30, 0x01, 0x20, 0x21},
    {0x00, 0x00, 0x05, 0x81, 0xdd, 0x00, 0x10, 0x04, 0x4d},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x05, 0x54},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b, 0x5a},
    {0x00, 0x00, 0x05, 0x81, 0xd0, 0x00, 0x01, 0x08, 0x5d},
    {0x00, 0x00, 0x05, 0x81, 0xc0, 0x00, 0x01, 0x0a, 0x4f},
    {0x00, 0x00, 0x05, 0x00, 0x31, 0x05, 0x00, 0x08, 0x39},
    {0x00, 0x00, 0x0a, 0x33, 0x2e, 0x30, 0x34, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x83},
    {0x00, 0x00, 0x05, 0x81, 0xd1, 0x00, 0x01, 0x10, 0x44},
    {0x00, 0x00, 0x05, 0x81, 0xd2, 0x00, 0x01, 0x10, 0x47},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x00, 0x05, 0x22},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x01, 0x05, 0x23},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x00, 0x02, 0x01, 0x23},
    {0x00, 0x00, 0x0e, 0x02, 0x68, 0x00, 0x00, 0x09, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x02, 0x50, 0x03, 0xbf},
};
char t1_suantong_rcv[][39] = {
    {0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00, 0x9f},
    {0x00, 0x00, 0x23, 0xeb, 0x70, 0x11, 0x27, 0xc7, 0x72, 0xfc,
     0x84, 0xa4, 0x53, 0x39, 0x66, 0xe3, 0xc5, 0x5d, 0xa2, 0x34,
     0x27, 0x2b, 0xac, 0x1b, 0xf6, 0xf6, 0x5f, 0x49, 0xa7, 0x51,
     0x5e, 0x08, 0x00, 0xe5, 0xba, 0x0c, 0x90, 0x00, 0xc8},
    {0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x01, 0x90, 0x00,0x96},
    {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00,
     0x96},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x0a, 0x28, 0x4b, 0x3c, 0x2a, 0x04, 0x99, 0x71,
     0xcf, 0x90, 0x00, 0xcc},
    {0x00, 0x00, 0x04, 0x30, 0x00, 0x90, 0x00,0xa4},
    {0x00, 0x00, 0x0a, 0x33, 0x2e, 0x30, 0x34, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x83},
    {0x00, 0x00, 0x12, 0x00, 0x1c, 0xdc, 0x33, 0x3e, 0x9f, 0xc8,
     0x0a, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x90,
     0x00, 0x12},
    {0x00, 0x00, 0x12, 0x50, 0x61, 0x79, 0x5f, 0x54, 0x56, 0x5f,
     0x43, 0x61, 0x72, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90,
     0x00, 0xfc},
    {0x00, 0x00, 0x07, 0x1c, 0x21, 0x0e, 0x15, 0x00, 0x90, 0x00,
     0xb1},
    {0x00, 0x00, 0x07, 0x2a, 0x87, 0x0e, 0x15, 0x00, 0x90, 0x00,
     0x21},
    {0x00, 0x00, 0x08, 0x00, 0x25, 0x00, 0x02, 0x01, 0x09, 0x90,
     0x00, 0xb7},
    {0x00, 0x00, 0x20, 0x50, 0x09, 0x1b, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x90, 0x00, 0xc2},
};
int t1_suantong_send_len[15] = {9, 9, 9, 9, 9, 9, 9, 9, 14, 9, 9, 9, 9, 9, 18};
int t1_suantong_rcv_len[15] = {18, 39, 10, 11, 17, 14, 8, 14, 22, 22, 11, 11, 12, 36};

#else

/***************************T1 suantong*******************************************/
char atr_suantong_t1[18] = {0x3b, 0xe9, 0x00, 0x00, 0x81,
                            0x31, 0xc3, 0x45, 0x99, 0x63,
                            0x74, 0x69, 0x19, 0x99, 0x12,
                            0x56, 0x10, 0xec};
int atr_suantong_t1_cnt = 18;
char t1_suantong_send[][23] = { //[][6]
    {0x00, 0x00, 0x06, 0x00, 0x32, 0x10, 0x01, 0x01, 0x01, 0x25},

    //{0x00, 0x00, 0x05, 0x00, 0x84, 0x00, 0x00, 0x10, 0x91},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0xa0, 0x21, 0x20,0x81},
    {0x00, 0x00, 0x05, 0x81, 0xdd, 0x00, 0x10, 0x04,0x4d},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x05,0x54},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b,0x5a},
    {0x00, 0x00, 0x05, 0x81, 0xd0, 0x00, 0x01, 0x08,0x5d},
    {0x00, 0x00, 0x05, 0x81, 0xc0, 0x00, 0x01, 0x0a,0x4f},
    {0x00, 0x00, 0x05, 0x00, 0x31, 0x05, 0x00, 0x08,0x39},
    {0x00, 0x00, 0x05, 0x81, 0xd1, 0x00, 0x01, 0x10,0x44},
    {0x00, 0x00, 0x05, 0x81, 0xd2, 0x00, 0x01, 0x10,0x47},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x00, 0x05,0x22},
    {0x00, 0x00, 0x05, 0x81, 0xa3, 0x00, 0x01, 0x05,0x23},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x00, 0x02, 0x01,0x23},
    {0x00, 0x00, 0x0e, 0x02, 0x68, 0x00, 0x00, 0x09, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x02, 0x50, 0x03, 0xbf},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x10,
     0xa7, 0x10, 0xc7},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x20,
     0xa7, 0x10, 0xf7},
    {0x00, 0x00, 0x13, 0x02, 0x68, 0x00, 0x00, 0x0e, 0x00, 0x00,
     0x00, 0x00, 0x01, 0x82, 0x07, 0x90, 0x03, 0x00, 0x00, 0x30,
     0xa7, 0x10, 0xe7},
    {0x00, 0x00, 0x05, 0x81, 0xd4, 0x00, 0x01, 0x0b,0x5a},
    {0x00, 0x00, 0x05, 0x00, 0x25, 0x80, 0x14, 0x01,0xb5},
};
char t1_suantong_rcv[][36] = { //[][13]
    {0x00, 0x00, 0x02, 0x90, 0x00, 0x92},

    //{0x00, 0x00, 0x12, 0xd0, 0x9d, 0x75, 0xb4, 0xcf, 0x06, 0xd4, 0xa9, 0x5c, 0x02, 0x5f, 0x56, 0xb1, 0xa9,
    //0xf1, 0xf3, 0x90, 0x00, 0xf7},
    {0x00, 0x00, 0x02, 0x6a, 0x86, 0xee},
    {0x00, 0x00, 0x06, 0x00, 0x01,0x00,0x01,0x90,0x00,0x96},
    {0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x01, 0x90, 0x00,
     0x96},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x0a, 0x33, 0xc5, 0xcf, 0xc0, 0x02, 0xed, 0x02,
     0x7b, 0x90, 0x00, 0xf5},
    {0x00, 0x00, 0x04, 0x30, 0x00,0x90,0x00,0xa4},
    {0x00, 0x00, 0x0a, 0x32, 0x2e, 0x35, 0x31, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x82},
    {0x00, 0x00, 0x12, 0x00, 0x1c, 0x6b, 0xf6, 0x14, 0x9f, 0x39,
     0xe9, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x90,
     0x00, 0x58},
    {0x00, 0x00, 0x12, 0x43, 0x54, 0x49, 0x5f, 0xd3, 0xc3, 0xbb,
     0xa7, 0xbf, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x90,
     0x00, 0x98},
    {0x00, 0x00, 0x07, 0x1c, 0x29, 0x0b, 0x20, 0x00, 0x90, 0x00,
     0x89},
    {0x00, 0x00, 0x07, 0x20, 0xa9, 0x0b, 0x20, 0x00, 0x90, 0x00,
     0x35},
    {0x00, 0x00, 0x08, 0x00, 0x25, 0x00, 0x02, 0x01, 0x12, 0x90,
     0x00, 0xac},
    {0x00, 0x00, 0x20, 0x50, 0x09, 0x1b, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x10, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x90, 0x00, 0xf2},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x53, 0x54, 0x42, 0x20, 0x54,
     0x65, 0x61, 0x6d, 0x28, 0x43, 0x41, 0x29, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x68},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x32, 0xb1, 0xb1, 0xbe, 0xa9,
     0xb6, 0xfe, 0xcc, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x3a},
    {0x00, 0x00, 0x14, 0xa7, 0x10, 0x33, 0xb1, 0xb1, 0xbe, 0xa9,
     0xc8, 0xfd, 0xcc, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x90, 0x00, 0x46},
    {0x00, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x01, 0x86, 0xf1, 0x90, 0x00, 0xeb},
    {0x00, 0x00, 0x02, 0x6a, 0x86, 0xee},
};
int t1_suantong_send_len[19] = {10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 18, 23, 23, 23, 9, 9}; //{6,5,5,5};
int t1_suantong_rcv_len[19] = {6, 6, 10, 11, 17, 14, 8, 14, 22, 22, 11, 11, 12, 36, 24, 24, 24, 17, 6};

#endif

/******************t14_irdeto_access*************************************/
char atr_irdeto_access_t14[20] = {0x3b, 0x9f, 0x21,  0xe, 0x49, 0x52, 0x44, 0x45,
                                  0x54, 0x4f, 0x20, 0x41, 0x43, 0x53, 0x20, 0x56,
                                  0x32, 0x2e, 0x32, 0x98};
int atr_irdeto_access_t14_cnt = 20;
char t14_irdeto_access_send[][7] = {
    {0x1, 0x2, 0x0, 0x0, 0x0, 0x0, 0x3c},
    {0x1, 0x2, 0x1, 0x0, 0x0, 0x0, 0x3d},
    {0x1, 0x2, 0x2, 0x0, 0x0, 0x0, 0x3e},
    {0x1, 0x2, 0x8, 0x0, 0x0, 0x0, 0x34},
    {0x1, 0x2, 0x3, 0x0, 0x0, 0x0, 0x3f},
};
char t14_irdeto_access_rcv[][73] = {
    {0x1, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x14,
     0x34, 0x30, 0x30, 0x32, 0x33, 0x33, 0x30, 0x34,
     0x39, 0x34, 0x54, 0x33, 0x32, 0x34, 0x30, 0x31,
     0x41, 0x0, 0x0, 0x0, 0x6},
    {0x1, 0x2, 0x0, 0x0, 0x1, 0x0, 0x0, 0x10,
     0xff, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x4, 0x7, 0x37, 0x94, 0xe6, 0x18,
     0x8c},
    {0x1, 0x2, 0x0, 0x0, 0x2, 0x0, 0x0, 0x10,
     0x4, 0x6, 0x12, 0x6, 0x23, 0x6, 0x24, 0x6,
     0x25, 0x6, 0x26, 0x0, 0x0, 0x43, 0x48, 0x4e,
     0x7f},
    {0x1, 0x2, 0x0, 0x0, 0x8, 0x0, 0x0, 0x40,
     0x0, 0x1, 0x8, 0x20, 0x0, 0x15, 0x60, 0x2,
     0x60, 0x0, 0x0, 0x3, 0x81, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0xc8},
    {0x1, 0x2, 0x0, 0x0, 0x3, 0x0, 0x0, 0x18,
     0x0, 0x3, 0xe9, 0x97, 0x0, 0x0, 0x0, 0x0,
     0x0, 0x0, 0x2, 0x11, 0x1, 0x1, 0x0, 0x0,
     0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
     0x49},
};
int t14_irdeto_access_send_len[5] = {7, 7, 7, 7, 7};
int t14_irdeto_access_rcv_len[5] = {29, 25, 25, 73, 33};

/********************t0_crypto_send*******************************/
HI_CHAR t0_crypto_send[][5] = {
    {0xa4, 0xc0, 0x00, 0x00, 0x11},
    {0xa4, 0xa4, 0x00, 0x00, 0x02},
    {0x3f,0x00},
    {0xa4, 0xa4, 0x00, 0x00, 0x02},
    {0x2f,0x01},
    {0xa4, 0xa2, 0x00, 0x00, 0x01},
    {0xd1},
};
HI_S32 t0_crypto_send_len[7] = {5, 5, 2, 5, 2, 5, 1};

HI_CHAR t0_crypto_rcv[][20] = {
    {0xc0, 0xdf, 0x0f, 0x05, 0x04, 0x00, 0x09, 0x3f, 0x00, 0x01,
     0x00, 0xf0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x90, 0x00},
    {0xa4},
    {0x9f,0x11},
    {0xa4},
    {0x9f,0x11},
    {0xa2},
    {0x9f,0x04},
};
HI_S32 t0_crypto_rcv_len[7] = {20, 1, 2, 1, 2, 1, 2};

/********************************************************************/

static HI_S32 g_s32MaxSndLen;
static HI_S32 g_s32SndTimes;
static HI_S32 *g_pSndLen;
static HI_CHAR *g_pSndBuf;

static HI_S32 g_s32MaxRcvLen;
static HI_S32 g_s32RcvNum;
static HI_S32 *g_pRcvLen;
static HI_CHAR *g_pRcvBuf;

static HI_BOOL g_bDropWord;
static HI_U8 g_u8SCIPort;
static HI_S32 protrol =HI_UNF_SCI_PROTOCOL_T0;

#define SMC_VCC_LEVEL HI_UNF_SCI_LEVEL_LOW
#define SMC_DETECT_LEVEL HI_UNF_SCI_LEVEL_HIGH
static void CasSCEvtNotify(HI_U8 CardAction);
HI_S32 Smc_T0_transfer(HI_U8 *request,HI_U32 reqlen, HI_U8 *reply, HI_U32 *replen);
HI_S32 Smc_T14_transfer(HI_U8 *request,HI_U32 reqlen, HI_U8 *reply, HI_U32 *replen);
HI_S32 drv_smartcard_transfer_data(HI_U8 *reqdata,
                                         HI_S32 reqlen, 
										 HI_U8 *repdata, 
                                         HI_S32 *replen, 
                                         HI_S16 *statusword);

HI_S32 drv_CasSelectCard(HI_S32 CardNo)
{
    if (0 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t0_irdeto_zeta_send;
        g_pSndLen = t0_irdeto_zeta_send_len;
        g_s32MaxSndLen = sizeof(t0_irdeto_zeta_send[0]);
        g_s32SndTimes = sizeof(t0_irdeto_zeta_send) / sizeof(t0_irdeto_zeta_send[0]);

        g_pRcvBuf = (HI_CHAR*)t0_irdeto_zeta_rcv;
        g_pRcvLen = t0_irdeto_zeta_rcv_len;
        g_s32MaxRcvLen = sizeof(t0_irdeto_zeta_rcv[0]);
        g_s32RcvNum = sizeof(t0_irdeto_zeta_rcv) / sizeof(t0_irdeto_zeta_rcv[0]);
        g_bDropWord = HI_TRUE;
    }
    else if (1 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t1_suantong_send;
        g_pSndLen = t1_suantong_send_len;
        g_s32MaxSndLen = sizeof(t1_suantong_send[0]);
        g_s32SndTimes = sizeof(t1_suantong_send) / sizeof(t1_suantong_send[0]);

        g_pRcvBuf = (HI_CHAR*)t1_suantong_rcv;
        g_pRcvLen = t1_suantong_rcv_len;
        g_s32MaxRcvLen = sizeof(t1_suantong_rcv[0]);
        g_s32RcvNum = sizeof(t1_suantong_rcv) / sizeof(t1_suantong_rcv[0]);
        g_bDropWord = HI_FALSE;
    }
    else if (14 == CardNo)
    {
        g_pSndBuf = (HI_CHAR*)t14_irdeto_access_send;
        g_pSndLen = t14_irdeto_access_send_len;
        g_s32MaxSndLen = sizeof(t14_irdeto_access_send[0]);
        g_s32SndTimes = sizeof(t14_irdeto_access_send) / sizeof(t14_irdeto_access_send[0]);

        g_pRcvBuf = (HI_CHAR*)t14_irdeto_access_rcv;
        g_pRcvLen = t14_irdeto_access_rcv_len;
        g_s32MaxRcvLen = sizeof(t14_irdeto_access_rcv[0]);
        g_s32RcvNum = sizeof(t14_irdeto_access_rcv) / sizeof(t14_irdeto_access_rcv[0]);
        g_bDropWord = HI_FALSE;
    }
    else
    {
        printf("not supported in this sample now\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


HI_S32 CardOutProcess()
{
    return HI_SUCCESS;
}

HI_S32 CardInProcess()
{
    return HI_SUCCESS;
}

#define ACTION_CARDIN 0     	  /* card inset flag*/
#define ACTION_CARDOUT 1      /* card pull out flag*/
#define ACTION_NONE 2      	  /* card immobile */

HI_BOOL g_bCardStatus = HI_FALSE; /*g_bCardStatus = HI_TRUE indicate card in ;g_bCardStatus = HI_FALSE  indicate no card */
HI_U8 g_CardAction = ACTION_NONE;

HI_S32 drv_CasCheckCa(HI_U8 *pAction)
{
    HI_S32 ErrCode;
    HI_U8 SciPort = 0;
    HI_UNF_SCI_STATUS_E enStatus;
    HI_BOOL bStatus;

    ErrCode = HI_UNF_SCI_GetCardStatus(SciPort, &enStatus);
    if (HI_SUCCESS != ErrCode)
    {
        return HI_FAILURE;
    }

    if (enStatus <= HI_UNF_SCI_STATUS_NOCARD)
    {
        bStatus = HI_FALSE;  /*no card  */
    }
    else
    {
        bStatus = HI_TRUE;   /*have card*/
    }

    if (g_bCardStatus != bStatus)       /* if it is ture indicate have pull out or push in card operation*/
    {
        g_bCardStatus = bStatus;
        if (HI_TRUE == bStatus)
        {
            *pAction = ACTION_CARDIN;   	/*card in  */
        }
        else
        {
            *pAction = ACTION_CARDOUT;    /*card out */
        }
        CasSCEvtNotify(*pAction);
    }
    else
    {
        *pAction = ACTION_NONE;        /*no operation*/
    }

    return HI_SUCCESS;
}

static HI_BOOL g_RunFlag1, g_RunFlag2;
static DRV_MutexHandle g_SciMutex;

HI_VOID* drv_CasCheckSci(HI_VOID* args)
{
    HI_S32 Ret;

    while (g_RunFlag1)
    {
        DRV_OS_MutexLock(g_SciMutex);
        Ret = drv_CasCheckCa(&g_CardAction);
        if (HI_SUCCESS != Ret)
        {
            printf("drv_CasCheckCa failed\n");
        }

        DRV_OS_MutexRelease(g_SciMutex);
        usleep(50 * 1000);
    }

    return (HI_VOID*)HI_SUCCESS;
}

HI_VOID* drv_CasRunSci(HI_VOID *args)
{
    HI_U8 CardAction;

    while (g_RunFlag2)
    {
        DRV_OS_MutexLock(g_SciMutex);
        CardAction = g_CardAction;
        DRV_OS_MutexRelease(g_SciMutex);
        if (ACTION_CARDIN == CardAction)
        {
            printf("CARD IN\n");
            CardInProcess();
        }
        else if (ACTION_CARDOUT == CardAction)
        {
            printf("CARD OUT\n");
            CardOutProcess();
        }
        else
        {
            //do nothing
            
        }
        usleep(50 * 1000);
    }

    return (HI_VOID*)HI_SUCCESS;
}


#define CURRENT_MODULE DRV_MODULE_CAS
/* 浣跨敤鏂扮殑SMARTCARD椹卞姩缁撴瀯锛屽湪鎵撳紑椹卞姩鏃惰缃洖璋冨嚱鏁? */
//#define CAS_NEW_ARCHI


static SC_StatusCallbackFun CasSCCallback = NULL;

//////Add.By.Lqjun.20080702.

/*****************************************************************************
* Function    : DRV_CasInit
* Description :
*                   初始化cas相关的模块如：智能卡.
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasInit(DRV_SMART_CA_Type_t ca_type)
{
	DRV_ErrCode ErrCode = HI_FAILURE;
    HI_U32 u32ClkMode;
    HI_U32 u32Freq;
    HI_U32 u32Level, u32Detect;
    HI_UNF_SCI_CLK_MODE_E enClkMode;
    HI_U32 u8ProtocolType;
    HI_UNF_SCI_PROTOCOL_E enProtocolType;
    HI_UNF_SCI_LEVEL_E enSciLevel;
	switch(ca_type)
    {
        case DRV_CAS_NORMAL:
	 break;
        case DRV_CAS_CTI225:
	 break;
        case DRV_CAS_CTI300:
	 break;
        case DRV_CAS_NAGRA:
	 break;
        case DRV_CAS_TF21PRO:
	 break;
        case DRV_CAS_TF21PROEX:
	 break;
        case DRV_CAS_DVN:
	 break;
       case DRV_CAS_IRDETO:
	 break;
	 default:
	 break;
    }

    g_u8SCIPort = 0;
    u8ProtocolType = 0;
    //printf("please select protocol type: 0-T0 1-T1 14-T14 others-T0\n");
    //printf("for most card, this should be use  0-T0\n");
    //scanf("%s", a);
    //u8ProtocolType = (HI_U32)strtoul(a, 0, 0);
    //if ((0 != u8ProtocolType) && (1 != u8ProtocolType) && (14 != u8ProtocolType))
    //{
    //    u8ProtocolType = 0;
    //}

    printf("u8ProtocolType = %d\n", u8ProtocolType);
    ErrCode = drv_CasSelectCard(u8ProtocolType);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasInit drv_CasSelectCard ErrCode 0x%08x\n",ErrCode); 
        return HI_FAILURE;
    }

    if (14 == u8ProtocolType)
    {
        enProtocolType = HI_UNF_SCI_PROTOCOL_T14;
        u32Freq = 6000;
    }
    else
    {
        enProtocolType = u8ProtocolType;
        u32Freq = 3570;
    }

    ErrCode = HI_UNF_SCI_Init();
	if(ErrCode != HI_SUCCESS)
	{
        printf("DRV_CasInit HI_UNF_SCI_Init 0x%08x\n",ErrCode); 
		return ErrCode;
	}
    /*open SCI device */
    ErrCode = HI_UNF_SCI_Open(g_u8SCIPort, enProtocolType, u32Freq);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasInit HI_UNF_SCI_Open 0x%08x\n",ErrCode); 
        HI_UNF_SCI_DeInit();
        return ErrCode;
    }

    //printf("please config clk mode: 0-OD 1-CMOS\n");
    //printf("for hisilicon demo board, this should be 0-OD\n");
    //scanf("%s", a);
    //u32ClkMode = (HI_U32)strtoul(a, 0, 0);
//#if (defined(CHIP_HI3716MV300) || defined(CHIP_HI3716MV300TFADV) || defined(CHIP_HI3716MV300SMADV))
    //u32ClkMode = 1;
//#else
    u32ClkMode = 0;
//#endif
    printf("u32ClkMode = %d\n", u32ClkMode);
    if (0 == u32ClkMode)
    {
        enClkMode = HI_UNF_SCI_CLK_MODE_OD;
    }
    else
    {
        enClkMode = HI_UNF_SCI_CLK_MODE_CMOS;
    }

    /*set SCI clk mode:  0-OD 1-CMOS . it reference the hardware design*/
    ErrCode = HI_UNF_SCI_ConfigClkMode(g_u8SCIPort, enClkMode);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasInit HI_UNF_SCI_ConfigClkMode 0x%08x\n",ErrCode); 
		return ErrCode;
    }
//#if (defined(CHIP_HI3716MV300) || defined(CHIP_HI3716MV300TFADV) || defined(CHIP_HI3716MV300SMADV))
	//u32Level = FALSE;//此选项会将vcc拉低为4v
//#else
    u32Level = TRUE;
//#endif
	
    //printf("please config VccEn: 0-low level active 1-high level active\n");
    //printf("for hisilicon demo board, this should be 0-low level active\n");
    //scanf("%s", a);
    //u32Level = (HI_U32)strtoul(a, 0, 0);
    if (!u32Level)
    {
        enSciLevel = HI_UNF_SCI_LEVEL_LOW;
    }
    else
    {
        enSciLevel = HI_UNF_SCI_LEVEL_HIGH;
    }

    printf("u32Level = %d\n", enSciLevel);

    /*set sci card power enable level.it reference the hardware design*/
    ErrCode = HI_UNF_SCI_ConfigVccEn(g_u8SCIPort, enSciLevel);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasInit HI_UNF_SCI_ConfigVccEn 0x%08x\n",ErrCode); 
		return ErrCode;
    }
    u32Detect = TRUE;
    //printf("please config Detect: 0-low level active; 1-high level active\n");
    //printf("for hisilicon demo board, this should be 1-high level active\n");
    //scanf("%s", a);
    //u32Detect = (HI_U32)strtoul(a, 0, 0);
    if (!u32Detect)
    {
        enSciLevel = HI_UNF_SCI_LEVEL_LOW;
    }
    else
    {
        enSciLevel = HI_UNF_SCI_LEVEL_HIGH;
    }

    printf("u32Detect = %d\n", enSciLevel);

    /*set sci card detect level.it reference the hardware design*/
    ErrCode = HI_UNF_SCI_ConfigDetect(g_u8SCIPort, enSciLevel);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasInit HI_UNF_SCI_ConfigDetect 0x%08x\n",ErrCode); 
		return ErrCode;
    }

	return (DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasEnableCallback
* Description :
*                  注册智能卡状态回调函数
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasEnableCallback(SC_StatusCallbackFun Callback_f)
{
    CasSCCallback = Callback_f;
    return(DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasDisableCallback
* Description :
*                  注消智能卡状态回调函数
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasDisableCallback(void)
{
    CasSCCallback = NULL;
    return(DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : CasSCEvtNotify
* Description :
*                  智能卡状态上报，通知函数
* Note        :
******************************************************************************/
static void CasSCEvtNotify(HI_U8 CardAction)
{
    DRV_CasScStatus Status = DRV_CAS_SMARTCARD_ERROR;
    switch(CardAction)
    {
    	case ACTION_CARDIN:
    	Status = DRV_CAS_SMARTCARD_IN;
    	break;
    	case ACTION_CARDOUT:
    	Status = DRV_CAS_SMARTCARD_OUT;
    	break;
    }
    if(CasSCCallback != NULL)
    {
        CasSCCallback(Status);	
    }
}

/*****************************************************************************
* Function    : DRV_CasOpen
* Description :
*                   打开智能卡设备，准备工作
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasOpen(void)
{
    DRVOS_Task_t *task1, *task2;
	int errCode = 0;
    DRV_OS_MutexCreate(NULL,&g_SciMutex);

    /** create thread to drv_CasCheckCa */
    g_RunFlag1 = 1;
	task1 = DRV_OS_TaskCreate("drv_CasCheckSci",drv_CasCheckSci,3,8*1024,NULL,0);
	if(task1 == NULL)
	{
        printf("DRV_CasOpen DRV_OS_TaskCreate drv_CasCheckSci Faild\n"); 
		return -1;
	}

    /** Create thread to send and rcv data */
    g_RunFlag2 = 1;
	task2 = DRV_OS_TaskCreate("drv_CasRunSci",drv_CasRunSci,3,8*1024,NULL,0);
	if(task1 == NULL)
	{
        printf("DRV_CasOpen DRV_OS_TaskCreate drv_CasRunSci Faild\n"); 
		return -1;
	}
	return (DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasGetScStatus
* Description :
*                   获取智能卡是否插入、拔出的状态
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasGetScStatus(DRV_CasScStatus *Status_p)
{
	HI_S32 errCode = HI_SUCCESS;
    HI_UNF_SCI_STATUS_E u32SCIStatus;


	errCode = HI_UNF_SCI_GetCardStatus(g_u8SCIPort, &u32SCIStatus);
	if(errCode != HI_SUCCESS)
	{
        printf("DRV_CasGetScStatus HI_UNF_SCI_GetCardStatus 0x%08x\n",errCode); 
		*Status_p = DRV_CAS_SMARTCARD_ERROR;
	}

	if(u32SCIStatus < 3)
	{
		*Status_p = DRV_CAS_SMARTCARD_OUT;
	}
	else
	{
		*Status_p = DRV_CAS_SMARTCARD_IN;
	}

	return (DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasResetSc
* Description :
*                   重新复位智能卡
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasResetSc(U8_YS *Answer_p, U8_YS* Answer_Len, U8_YS ResetType)
{
    HI_S32 ErrCode;
    HI_UNF_SCI_STATUS_E u32SCIStatus;
    HI_S32 s32ResetTime;
    HI_U8 u8ATRCount;
    HI_U8 ATRBuf[255];
    HI_U32  u32ReadLen;
    HI_S32 i, j;
    HI_U8 u8Result = 0;
    struct timeval tv_1,tv_2;
	struct timezone tz;
	HI_U32 usd_us;
	HI_BOOL bWarmResetValid = 0;

    if(ResetType == SMART_COLD_RESET )
    {
    	bWarmResetValid = 1;
    }
	gettimeofday(&tv_1, &tz);
    ErrCode = HI_UNF_SCI_ResetCard(g_u8SCIPort, 0);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasResetSc HI_UNF_SCI_ResetCard 0x%08x,bWarmResetValid %d\n",ErrCode,bWarmResetValid); 
        return DRV_CAS_ERROR_SMART_RESET;
    }
    else
    {
        printf("DRV_CasResetSc HI_UNF_SCI_ResetCard ok\n");
    }
    s32ResetTime = 0;
    while (1)
    {
        /* will exit reset when reseting out of 2S*/
        if (s32ResetTime >= 100)
        {
            printf("Reset Card Failure\n");
            return HI_FAILURE;
        }

        /*get SCI card status */
        ErrCode = HI_UNF_SCI_GetCardStatus(g_u8SCIPort, &u32SCIStatus);
        if (HI_SUCCESS != ErrCode)
        {
            printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
            return ErrCode;
        }
        else
        {
            if (u32SCIStatus >= HI_UNF_SCI_STATUS_READY)
            {
            	gettimeofday(&tv_2, &tz);
				usd_us = (tv_2.tv_sec * 1000 + tv_2.tv_usec/1000 ) - (tv_1.tv_sec * 1000 + tv_1.tv_usec/1000 );
				printf("Reset:%d ms	\n", usd_us);
                /*reset Success*/
                printf("Reset Card Success\n");
                break;
            }
            else
            {
                printf("Reset Card Waiting...\n");
                usleep( 50000);
                s32ResetTime += 1;
            }
        }
    }

    /*get and print ATR message*/
    ErrCode = HI_UNF_SCI_GetATR(g_u8SCIPort, ATRBuf, 255, &u8ATRCount);
    if (HI_SUCCESS != ErrCode)
    {
        printf("DRV_CasResetSc HI_UNF_SCI_GetATR 0x%08x\n",ErrCode); 
        return DRV_CAS_ERROR_SMART_RESET;
    }
#if 0
    printf("GetATR Count:%d\n", u8ATRCount);

    for (i = 0; i < u8ATRCount; i++)
    {
        printf("ATRBuf[%d]:%#x\n", i, ATRBuf[i]);
    }
#endif
#if 0 //对应天柏邋CA卡通信，需要重新设下波特率  wrwei
    ErrCode = HI_UNF_SCI_SetEtuFactor(g_u8SCIPort, 372, 2);
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }

    ErrCode = HI_UNF_SCI_SetGuardTime(g_u8SCIPort, 2);
    if (HI_SUCCESS != ErrCode)
    {
        printf("%s: %d ErrorCode=0x%x\n", __FILE__, __LINE__, ErrCode);
        return ErrCode;
    }
#endif

	if(u8ATRCount < 33 )
	{
	    memcpy(Answer_p,ATRBuf,u8ATRCount);
	    *Answer_Len=(U8_YS)u8ATRCount;
	}

	return (DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasScSend
* Description :
*			给智能卡发送数据，有超时设置
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScSend(const U8_YS *Data_p,
                              U32_YS  NumberToSend,
                              U32_YS *NumberSent_p,
                              U32_YS  Timeout)
{
    HI_S32 ErrCode = DRV_NO_ERROR;
    U32_YS uiNumberSended = 0;

	ErrCode = HI_UNF_SCI_Send(g_u8SCIPort, Data_p, NumberToSend, &uiNumberSended, Timeout);
    if( DRV_NO_ERROR != ErrCode )
    {
            STTBX_Print(("DRV_CasScSend() HI_UNF_SCI_Send ErrCode is 0x%08x\n",ErrCode));
    }
    else
    {
        *NumberSent_p = uiNumberSended;
    }

    return(ErrCode);
}

/*****************************************************************************
* Function    : DRV_CasScReceive
* Description :
*			接收智能卡数据，有超时设置			
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScReceive(U8_YS  *Data_p,
                                 U32_YS  NumberToReceive,
                                 U32_YS *NumberReceived_p,
                                 U32_YS  Timeout)
{
    HI_S32 ErrCode = DRV_NO_ERROR;
    U32_YS uiNuberReceived = 0;
	ErrCode = HI_UNF_SCI_Receive(g_u8SCIPort,Data_p,NumberToReceive, &uiNuberReceived,Timeout);
    if( DRV_NO_ERROR != ErrCode )
    {
            STTBX_Print(("DRV_CasScReceive() HI_UNF_SCI_Receive ErrCode is 0x%08x\n",ErrCode));
    }
    else
    {
        *NumberReceived_p = uiNuberReceived;
    }

    return(ErrCode);
}

/*****************************************************************************
* Function    : DRV_CasScTransfer
* Description :
*			单一接口中实现智能卡读写
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScTransfer(U8_YS  *Command_p,
                                 U32_YS  NumberToWrite,
                                 U32_YS *NumberToWrite_p,
                                 U8_YS   *Response_p,
                                 U32_YS  NumberToRead,
                                 U32_YS *NumberToRead_p,                                 
                                 DRV_SMART_Status_t  *Status_p)
{
    DRV_ErrCode ErrCode = DRV_NO_ERROR;
    U32_YS replyLen   = 0;
    U32_YS numWritten = 0;
    U8 StatusWord[2];
	Status_p->ErrorCode = -1;
	ErrCode = drv_smartcard_transfer_data(Command_p,NumberToWrite,Response_p,&replyLen,(U16*)&StatusWord);
    if( ErrCode != DRV_NO_ERROR )
    {
        STTBX_Print(("DRV_CasScTransfer() T1_Communication ErrCode is 0x%x\n",ErrCode));
        return ErrCode;
    }
    else
    {
        *NumberToWrite_p = NumberToWrite;
        *NumberToRead_p  = replyLen;
		Status_p->ErrorCode = DRV_NO_ERROR;
 		Status_p->StatusBlock.T0.PB[0] = StatusWord[1];
		Status_p->StatusBlock.T0.PB[1] = StatusWord[0];
    }	
   return(ErrCode);
}

/*****************************************************************************
* Function    : DRV_CasScTerm
* Description :
*                   停止智能卡工作
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScTerm(void)
{
	return (DRV_NO_ERROR);
}

/*****************************************************************************
* Function    : DRV_CasScClose
* Description :
*                   关闭智能卡模块
* Note        :
******************************************************************************/
DRV_ErrCode DRV_CasScClose(void)
{
	return (DRV_NO_ERROR);
}
#define ATR_LEN_MAX 256
#define CMD_LEN_MAX 256

HI_S32 drv_smartcard_transfer_data(HI_U8 *reqdata,
                                         HI_S32 reqlen, 
										 HI_U8 *repdata, 
                                         HI_S32 *replen, 
                                         HI_S16 *statusword)
{
	HI_S32 ret = 0;
	HI_S32 cnt = 1;
	HI_U32 card_status;

	while(cnt--) 
    {
		ret = HI_UNF_SCI_GetCardStatus(g_u8SCIPort, &card_status);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_SCI_GetCardStatus] ret = %d,status = %d\n",ret,card_status));
#endif
		if(ret != HI_SUCCESS)
        {
			return -2;
		}
		if(card_status==HI_UNF_SCI_STATUS_READY) break;
		usleep(1000*20);
	}
	if(card_status==HI_UNF_SCI_STATUS_NOCARD) return -1;
	if(protrol == HI_UNF_SCI_PROTOCOL_T0)
    {
		ret = Smc_T0_transfer((HI_U8 *)reqdata, (HI_U32 )reqlen, repdata, (HI_U32 *)replen);
		if(ret != DRV_NO_ERROR) 
        {
			return -3;
		}
		*statusword = (repdata[*replen-2]<<8) | repdata[*replen-1];
		*replen-=2;
	}
	else if(protrol == HI_UNF_SCI_PROTOCOL_T14)
    {
		ret = Smc_T14_transfer((HI_U8 *)reqdata, (HI_U32)reqlen, repdata, (HI_U32 *)replen);
		*statusword = (repdata[*replen] << 8 | repdata[*replen + 1]);
	}
	return ret;
}

HI_S32 Smc_T0_transfer(HI_U8 *request,HI_U32 reqlen, HI_U8 *reply, HI_U32 *replen)
{
	HI_S32 ret = 0;
	HI_U8 INS;
	HI_U8 buf[CMD_LEN_MAX+2] ={0};
	HI_U32 actuallen = 0;
	HI_U32 recvlen = 0,sendlen;

	*replen = 0;
	
	/* cmd length */
	if (reqlen < 5) 
    {/*  fix me!! sometimes length < 5*/
		STTBX_Print(("reqlen<5"));
		ret = -1;
		goto EXIT;
	}
	/* header check */
	if (*request == 0xFF)
    {/* CLA check */
		STTBX_Print(("PTS cmd not support!"));
		ret = -1;
		goto EXIT;
	}
	INS = *(request+1);
 	if (((INS&0x1) == 1)||((INS&0xF0) == 0x60)||((INS&0xF0) == 0x90)) {
		STTBX_Print(("INS invalid!\n"));
		ret = -1;
		goto EXIT;
	}
	/* send header data */

	ret = HI_UNF_SCI_Send(g_u8SCIPort, request, 5, &actuallen, 500);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_Send] ret = %d\n",ret));
#endif
	if(ret != HI_SUCCESS) 
    {
		ret = -1;
		goto EXIT;
	}
	if(actuallen != 5) 
    {
		ret = -1;
		goto EXIT;
	}

	if (5 == reqlen) 
    { /* no procedure byte */
		do {
			/* receive procedure byte */
			ret = HI_UNF_SCI_Receive(g_u8SCIPort, buf, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
			STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d\n",ret));
#endif
			if(ret != HI_SUCCESS) {
				ret = -1;
				goto EXIT;
			}
			if(actuallen != 1) {
				ret = -1;
				goto EXIT;
			}
			if(buf[0] == 0x60) {
				usleep(1000*10);
			}
		}while(buf[0] == 0x60);
		
		/* procedure process */
		if(((buf[0]&0xF0) == 0x60)||((buf[0]&0xF0) == 0x90))
        {
			*reply = buf[0];
			ret = HI_UNF_SCI_Receive(g_u8SCIPort, reply+1, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
			STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d\n",ret));
#endif
			if(ret != HI_SUCCESS) {
				ret = -1;
				goto EXIT;
			}
			if(actuallen != 1) {
				ret = -1;
				goto EXIT;
			}
			ret = DRV_NO_ERROR;
			*replen = 2;
			goto EXIT;
		}
		
		/* receive data from card */
		if (0 == *(request+4))
			recvlen = 256;
		else
			recvlen = *(request+4);

		/*这里会超时*/
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, buf, recvlen, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != recvlen) {
			ret = -1;
			goto EXIT;
		}
		//SW
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, buf+recvlen, 2, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 2) 
        {
			ret = -1;
			goto EXIT;
		}

		*replen = recvlen+2;
		memcpy(reply, buf, *replen);
		ret = DRV_NO_ERROR;
		goto EXIT;
	}

	/* receive procedure byte */
	do {
		/* receive procedure byte */
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, buf, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data=%x\n",ret,buf[0]));
#endif
		if(ret != HI_SUCCESS)
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 1)
        {
			ret = -1;
			goto EXIT;
		}
		if(buf[0] == 0x60) 
        {
			usleep(1000*10);
		}
	}while(buf[0] == 0x60);

	/* procedure process */
	if(((buf[0]&0xF0) == 0x60)||((buf[0]&0xF0) == 0x90))
    {
		*reply = buf[0];
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, reply+1, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data=%x/%x\n",ret,reply[0],reply[1]));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 1) 
        {
			ret = -1;
			goto EXIT;
		}
		ret = DRV_NO_ERROR;
		*replen = 2;
		goto EXIT;
	}

	if((buf[0]&0xFE) == INS) 
    {
		if (0 == *(request+4))
			sendlen = 256;
		else
			sendlen = *(request+4);
#if defined(CHIP_HI3716C)
        usleep(1000*10);
#else	
	    
#endif
		ret = HI_UNF_SCI_Send(g_u8SCIPort, request+5, sendlen, &actuallen, 500);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Send] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != sendlen) 
        {
			ret = -1;
			goto EXIT;
		}
	}
	else if((buf[0]&0xFE) == (INS^0xFE))
    {
#if defined(CHIP_HI3716C)
        usleep(1000*10);
#else	
	    
#endif
		ret = HI_UNF_SCI_Send(g_u8SCIPort, request+5, 1, &actuallen, 500);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Send] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 1)
        {
			ret = -1;
			goto EXIT;
		}
	}
	else 
    {
		ret = -1;
		goto EXIT;
	}
	
	/* receive procedure byte 2*/
	do {
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, buf, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data = %x\n",ret,buf[0]));
#endif
		if(ret != HI_SUCCESS)
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 1) 
        {
			ret = -1;
			goto EXIT;
		}
		if(buf[0] == 0x60)
        {
			usleep(1000*10);
		}
	}while(buf[0] == 0x60);

	/* procedure process */
	if(((buf[0]&0xF0) == 0x60)||((buf[0]&0xF0) == 0x90)) 
    {
		*reply = buf[0];
		ret = HI_UNF_SCI_Receive(g_u8SCIPort, reply+1, 1, &actuallen, 2000);//原为500，但是超时等待时间应大于1500ms
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data = %x/%x\n",ret,reply[0],reply[1]));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
		if(actuallen != 1) 
        {
			ret = -1;
			goto EXIT;
		}
		ret = DRV_NO_ERROR;
		*replen = 2;
		goto EXIT;
	}
	
EXIT:
	return ret;	
}

HI_S32 Smc_T14_transfer(HI_U8 *request,HI_U32 reqlen, HI_U8 *reply, HI_U32 *replen)
{
	HI_S32 ret = 0;
	HI_U32 readlen = 0,writelen = 0,tmplen = 0;
	ret = HI_UNF_SCI_Send(g_u8SCIPort, request, reqlen, &writelen, 2000);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_Send] ret = %d\n",ret));
#endif
	if(ret != HI_SUCCESS) 
    {
		ret = -1;
		goto EXIT;
	}
	ret = HI_UNF_SCI_Receive(g_u8SCIPort, reply, 8,&tmplen, 2000);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data = %x\n",ret,reply[0]));
#endif
	if(ret != HI_SUCCESS)
    {
		ret = -1;
		goto EXIT;
	}
	readlen = reply[7] + 9 - 8;
	ret = HI_UNF_SCI_Receive(g_u8SCIPort, &reply[8], readlen, &tmplen, 2000);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_Receive] ret = %d,data = %x\n",ret,reply[0]));
#endif
	if(ret != HI_SUCCESS) 
    {
		ret = -1;
		goto EXIT;
	}
	*replen = tmplen + 8;
	ret = DRV_NO_ERROR;
EXIT:
	if(ret != DRV_NO_ERROR) *replen = 0;
	return ret;	
}


HI_S32 Smc_Open(HI_S32 protrol)
{
	HI_S32 ret = 0;

	if(HI_UNF_SCI_PROTOCOL_T0 == protrol)
    {
		ret = HI_UNF_SCI_Open(g_u8SCIPort, HI_UNF_SCI_PROTOCOL_T0, 4500);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Open] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
	}
	else if(HI_UNF_SCI_PROTOCOL_T1 == protrol) 
    {
		ret = HI_UNF_SCI_Open(g_u8SCIPort, HI_UNF_SCI_PROTOCOL_T1, 4500);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Open] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS)
        {
			ret = -1;
			goto EXIT;
		}
	}
	else if(HI_UNF_SCI_PROTOCOL_T14 == protrol)
    {
		ret = HI_UNF_SCI_Open(g_u8SCIPort, HI_UNF_SCI_PROTOCOL_T14, 6000);
#ifdef HAVE_SMC_DEBUG
		STTBX_Print(("[HI_UNF_SCI_Open] ret = %d\n",ret));
#endif
		if(ret != HI_SUCCESS) 
        {
			ret = -1;
			goto EXIT;
		}
	}
	
	ret = HI_UNF_SCI_ConfigClkMode(g_u8SCIPort, HI_UNF_SCI_CLK_MODE_OD);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_ConfigClkMode] ret = %d\n",ret));
#endif
	if(ret != HI_SUCCESS)
    {
		ret = -1;
		goto EXIT;
	}
	
	ret = HI_UNF_SCI_ConfigVccEn(g_u8SCIPort, SMC_VCC_LEVEL);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_ConfigVccEn] ret = %d\n",ret));
#endif
	if(ret != HI_SUCCESS)
    {
		ret = -1;
		goto EXIT;
	}
	ret = HI_UNF_SCI_ConfigDetect(g_u8SCIPort,SMC_DETECT_LEVEL);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_ConfigDetect] ret = %d\n",ret));
#endif
	if(ret != HI_SUCCESS) 
    {
		ret = -1;
		goto EXIT;
	}

EXIT:
	STTBX_Print(("ret = %d.\n",ret));
	return ret;
}


int Smc_Close()
{
	int ret = 0;
	ret = HI_UNF_SCI_DeactiveCard(g_u8SCIPort);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_DeactiveCard] ret = %d\n",ret));
#endif
	ret = HI_UNF_SCI_Close(g_u8SCIPort);
#ifdef HAVE_SMC_DEBUG
	STTBX_Print(("[HI_UNF_SCI_Close] ret = %d\n",ret));
#endif
	return ret;
}


