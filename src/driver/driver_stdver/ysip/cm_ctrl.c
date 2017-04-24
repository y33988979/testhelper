/*
    cablemodem control interface
    save ip addr
    ifconfig eth0 192.168.100.101
    cm_stop(restart)_scanning()
    restore ip addr
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <os21.h>

#include "cm_ctrl.h"


#if Linux_STB
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>


STB_CM_info_desc_t gCmInfoDesc;
int return_DS_Channel=0;
int cm_return_DS_Offset=0;
//unsigned char cm_MAC_Addr[CM_MAC_LENGTH]= {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char gCmMacAddr[CM_MAC_LENGTH]= {0x00,0x00,0x00,0x00,0x00,0x00};

int downstream_power_index=0;

char  store_downstream_power[28]=
{
    0xfa,0x60,0xf9,0xac,0xfa,0x10,0xf9,0xd4,
    0xf9,0xa2,0xf9,0xfc,0xfa,0x06,0xf9,0xa2,
    0xf9,0xfc,0xf9,0xfc,0xf9,0xfc,0xfa,0x06,
    0xfa,0x06,0xfa,0x24
};

short downstream_power_offset[]=
{
    -1440,-1620,-1520,-1580,
    -1550,-1540,-1530 ,-1550 ,
    -1540,-1540,-1540,-1530,
    -1530,-1500
};

char cm_snmp_password_pdu[]=
{
    0x30, 0x38,
    0x02, 0x01, 0x00,
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,

    0xa3, 0x2a,
    0x02, 0x02, 0x2c, 0x3b,
    0x02, 0x01, 0x00,
    0x02, 0x01, 0x00,
    0x30, 0x1e,
    0x30, 0x1c, 0x06, 0x10, 0x2b, 0x06, 0x01, 0x04, 0x01,
    0xa2,0x3d, 0x02, 0x63, 0x01, 0x01, 0x01, 0x02, 0x01, 0x02, 0x01,

    0x04, 0x08, 0x70, 0x61, 0x73, 0x73, 0x77, 0x6f, 0x72, 0x64
};

char cm_snmp_factory_pdu[]=
{
    0x30, 0x35,
    0x02, 0x01, 0x00,
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,

    0xa3, 0x27,
    0x02, 0x02, 0x35, 0xfa,
    0x02, 0x01, 0x00,
    0x02, 0x01, 0x00,
    0x30, 0x1b,
    0x30, 0x19, 0x06, 0x0e, 0x2b, 0x06, 0x01, 0x04, 0x01,
    0xa2, 0x3d, 0x02, 0x63, 0x01, 0x01, 0x01, 0x01, 0x00,

    0x04, 0x07, 0x66, 0x61, 0x63, 0x74, 0x6f, 0x72, 0x79
};

char cm_snmp_scanning_pdu[]=
{
    0x30, 0x30,
    0x02, 0x01, 0x00,
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,

    0xa3, 0x22,
    0x02, 0x02, 0x18, 0x10,
    0x02, 0x01,0x00,
    0x02, 0x01, 0x00,
    0x30, 0x16,
    0x30, 0x14, 0x06, 0x0f, 0x2b, 0x06, 0x01, 0x04, 0x01,
    0xa2,0x3d, 0x02, 0x02, 0x02, 0x01, 0x02, 0x01, 0x14, 0x00,

    0x02, 0x01, 0x00
};

char cm_snmp_frequency_pdu[]=
{
    //在正式版本中注意把这些注释去掉，此处仅为分析snmp数据包格式，供学习使用。
    0x30,0x34,//Sequence,Length
    0x02,0x01,0x00,//TLV,Value is SNMP Version 1 ,need -1
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,//TLV:0x04 is OCTET STRING,Value is "private"

    0xa3,0x26,//TL:PDU,SetRequest(A3)
    0x02,0x02,0x43,0x39,//Value:Request id ,it can random,but should keep only is seesion
    0x02,0x01,0x00, //error status: 在请求报文中，该字段为0
    0x02,0x01,0x00,//error index :在请求报文中，该字段为0
    0x30,0x1a,//varialble-bindings :指明一个或多个变量的名和对应的值，在请求报文中，变量的值应该忽略。
    0x30,0x18,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,//object identifier 类型:长度为16个字节,首两个字节1.3合并为2b(1*40+3=43(0x2b)).
    0xa2,0x3d,0x02,0x02,0x63,0xa2,0x3d,0x02,0x02,0x01,0x00,//MBI 全部字符为1.3.6.1.4.1.4413.2.2.99.4413.2.2.1.0 ,
    //4413 其二进制形式为0001 0001 0011 1101 ，大于127，故将其做一个变换，从最高位开始
    //取001 0001 0 作高位并将首位置为1，为101 0001 0，然后取低七位:011 1101 ，首位添0，为0011 1101.
    //两个字符串合并即为新的编码:1010 0010 0011 1101 ,十六进制为a23d.


    0x02,0x04,0x07,0x54,0xd4,0xc0 //TLV:为下行频点的具体值，此处设置为1230 0000 0，其十六进制数为0x754D4C0
};

//频点值为三字节的数据使用该数据包,其范围值为1M HZ~16MHZ
char cm_snmp_frequency_pdu3[]=
{
    0x30,0x33,
    0x02,0x01,0x00,
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,

    0xa3,0x25,
    0x02,0x02,0x40,0x5b,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x19,
    0x30,0x17,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,
    0xa2,0x3d,0x02,0x02,0x63,0xa2,0x3d,0x02,0x02,0x01,0x00,

    0x02,0x03,0x2d,0xc6,0xc0 //TLV:为下行频点的具体值，此处设置为300 0000(3M HZ)，其十六进制数为0x754D4C0

};


//频点值为四字节的数据使用该数据包,其范围值为17M HZ~1000MHZ
char cm_snmp_frequency_pdu4[]=
{
    //在正式版本中注意把这些注释去掉，此处仅为分析snmp数据包格式，供学习使用。
    0x30,0x34,//Sequence,Length
    0x02,0x01,0x00,//TLV,Value is SNMP Version 1 ,need -1
    0x04,0x07, 0x70, 0x72, 0x69, 0x76, 0x61, 0x74, 0x65,//TLV:0x04 is OCTET STRING,Value is "private"

    0xa3,0x26,//TL:PDU,SetRequest(A3)
    0x02,0x02,0x43,0x39,//Value:Request id ,it can random,but should keep only is seesion
    0x02,0x01,0x00, //error status: 在请求报文中，该字段为0
    0x02,0x01,0x00,//error index :在请求报文中，该字段为0
    0x30,0x1a,//varialble-bindings :指明一个或多个变量的名和对应的值，在请求报文中，变量的值应该忽略。
    0x30,0x18,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,//object identifier 类型:长度为16个字节,首两个字节1.3合并为2b(1*40+3=43(0x2b)).
    0xa2,0x3d,0x02,0x02,0x63,0xa2,0x3d,0x02,0x02,0x01,0x00,//MBI 全部字符为1.3.6.1.4.1.4413.2.2.99.4413.2.2.1.0 ,
    //4413 其二进制形式为0001 0001 0011 1101 ，大于127，故将其做一个变换，从最高位开始
    //取001 0001 0 作高位并将首位置为1，为101 0001 0，然后取低七位:011 1101 ，首位添0，为0011 1101.
    //两个字符串合并即为新的编码:1010 0010 0011 1101 ,十六进制为a23d.


    0x02,0x04,0x07,0x54,0xd4,0xc0 //TLV:为下行频点的具体值，此处设置为1230 0000 0，其十六进制数为0x754D4C0

};

char cm_snmp_Get_DSChannel[]=
{
    0x30,0x32,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,

    0xa0,0x24,
    0x02,0x02,0x18,0x72,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x18,
    0x30,0x16,
    0x06,0x12,
    0x2b,0x06,0x01,0x04,0x01,
    0xa2,0x3d,0x02,0x02,0x63,0xa2,0x3d,0x02,0x02,0x02,0x01,0x02,0x01,
    0x05,0x00   //意为NULL,其长度为0

};

/*以下数据包供对Cable Modem 进行校准时使用*/
/*--------------------------------------------------------------------------*/
char cm_snmp_diagnostic[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,

    0xa3, 0x23,
    0x02,0x02,0x46,0xa3,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,
    0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x01,0x01,0x00,
    0x02,0x01,0x02

};

char cm_snmp_private_enable_factory[]=
{
    0x30,0x2f,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72, 0x69,0x76,0x61,0x74,0x65,

    0xa3,0x21,
    0x02,0x02,0x1d,0xe3,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x15,
    0x30,0x13,
    0x06,0x0e,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x01,0x01,0x00,
    0x02,0x01,0x01
};

char cm_snmp_tunerindex_zero[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,

    0xa3,0x23,
    0x02,0x02,0x58,0x7f,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2 ,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x09,0x00,
    0x02,0x01,0x00
};

char cm_snmp_ds_offset_set_zero[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x23,
    0x02,0x02,0x7c,
    0x7d,0x02,0x01,
    0x00,0x02,0x01,0x00,0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x06,0x00,
    0x02,0x01,0x01
};

/*For Euro docsis,you need calibrate these 14 frequencies.

RowIndex=0; Freq=93000000 Hz; OffsetPower=0 dBmV

RowIndex=1; Freq=153000000 Hz; OffsetPower=0 dBmV

RowIndex=2; Freq=213000000 Hz; OffsetPower=0 dBmV

RowIndex=3; Freq=273000000 Hz; OffsetPower=0 dBmV

RowIndex=4; Freq=333000000 Hz; OffsetPower=0 dBmV

RowIndex=5; Freq=393000000 Hz; OffsetPower=0 dBmV

RowIndex=6; Freq=453000000 Hz; OffsetPower=0 dBmV

RowIndex=7; Freq=513000000 Hz; OffsetPower=0 dBmV

RowIndex=8; Freq=573000000 Hz; OffsetPower=0 dBmV

RowIndex=9; Freq=633000000 Hz; OffsetPower=0 dBmV

RowIndex=10; Freq=693000000 Hz; OffsetPower=0 dBmV

RowIndex=11; Freq=753000000 Hz; OffsetPower=0 dBmV

RowIndex=12; Freq=813000000 Hz; OffsetPower=0 dBmV

RowIndex=13; Freq=855000000 Hz; OffsetPower=0 dBmV
*/
/*
char cm_snmp_frequency[]=
{
    0x30,0x34,
    0x02,0x01,0x00,0x04,
    0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa3,0x26,
    0x02,0x02,0x62,0x4e,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x1a,
    0x30,0x18,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x01,0x00,
    0x02,0x04,0x05,0x8b,0x11,0x40


}; //可以使用以前的三字节四字节的方法进行判断，减少此部分的工作量

char cm_snmp_qam64[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x23,
    0x02,0x02,0x7b,0xbf,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x02,0x00,
    0x02,0x01,0x03
};

char cm_snmp_lock_qam[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x23,
    0x02,0x02,0x42,0x0c,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x03,0x00,
    0x02,0x01,0x01
};

char cm_snmp_verity_lockqam[]=
{
    0x30,0x30,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa0,0x22,
    0x02,0x02,0x5c,0x1d,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x16,
    0x30,0x14,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x04,0x00,
    0x05,0x00
};

char cm_snmp_verity_SNR[]=
{
    0x30,0x2d,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa0,0x1f,
    0x02,0x02,0x60,0x40,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x13,
    0x30,0x11,
    0x06,0x0d,
    0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x04,0x01,0x05,0x03,
    0x05,0x00
};

char cm_snmp_read_dspower[]=
{
    0x30,0x2d,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa0,0x1f,
    0x02,0x02,0x55,0xef,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x13,
    0x30,0x11,
    0x06,0x0d,
    0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x01,0x06,0x03,
    0x05,0x00
};
*/
/*对每一个频率都需要写进去，程序中注意对先前的获得的偏移值进行保存，或者保存一个就
写一个，方法无论采用哪种都行*/
char cm_snmp_write_ds_offset[]=
{
    0x30,0x34,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65
    ,0xa3,0x26
    ,0x02,0x02,0x74,0x9a,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x1a,
    0x30,0x18,
    0x06,0x12,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x08,0x01,0x03,0x01,
    0x02,0x02,0x06,0x72
};

char cm_snmp_disable_factory[]=
{
    0x30,0x2f,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x21,
    0x02,0x02,0x1b,0x5f,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x15,
    0x30,0x13,
    0x06,0x0e,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x01,0x01,0x00,
    0x02,0x01,0x00
};

char cm_snmp_set_DOCSIS[]=
{
    0x30,0x31,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x23,
    0x02,0x02,0x24,0xd1,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x10,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x01,0x01,0x00,
    0x02,0x01,0x01
};

char cm_snmp_commit_setting[]=
{
    0x30,0x30,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x22,
    0x02,0x02,0x07,0xd0,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x16,
    0x30,0x14,
    0x06,0x0f,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x01,0x01,0x00,
    0x02,0x01,0x01
};

char cm_snmp_get_vertity_setting[]=
{
    0x30,0x32,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa0,0x24,
    0x02,0x02,0x6e,0x57,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x18,
    0x30,0x16,
    0x06,0x12,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x02,0x03,0x08,0x01,0x03,
    0x01,0x05,0x00
};

char cm_snmp_get_MacAddr[]=
{
    0x30,0x31,
    0x02,0x01,0x01,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa0,0x23,
    0x02,0x02,0x09,0x80,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x17,
    0x30,0x15,
    0x06,0x11,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x01,0x04,0x01,0x02,0x01,
    0x05,0x00
};

char cm_snmp_cdPrivateMibEnable[]=
{
    0x30,0x2f,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa3,0x21,
    0x02,0x02,0x49,0x5f,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x15,
    0x30,0x13,
    0x06,0x0e,
    0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x01,0x01,0x00,
    0x02,0x01,0x00
};

/***************START  ***** 获取CM 下行信息数据包*****************************/

char cm_snmp_get_down_channel_id[]=
	{0x30,0x2a,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1c,
0x02,0x02,0x5e,0x00,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x10,
0x30,0x0e,0x06,0x0a,0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x05,0x00};

char cm_snmp_get_down_strength[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,0x02,0x02,0x1e,
0xed,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,0x2b,0x06,0x01,0x02,0x01,0x0a,
0x7f,0x01,0x01,0x01,0x01,0x05,0x03,0x05,0x00};



char cm_snmp_get_down_snr[]=
{
    0x30,0x2c,
    0x02,0x01,0x00,
    0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
    0xa1,0x1e,
    0x02,0x02,0x54,0xbc,
    0x02,0x01,0x00,
    0x02,0x01,0x00,
    0x30,0x12,
    0x30,0x10,
    0x06,0x0c,0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x04,0x01,0x05,
    0x05,0x00

};

/*BEGIN:modify by whkuan 20120919*/
char cm_snmp_get_down_modulation[]=
{
    0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
0xa1,0x1f,0x02,0x02,0x6d,0x59,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,
0x30,0x11,0x06,0x0d,0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x01,0x03,0x03,0x05,0x00
};

char cm_snmp_get_down_frequency[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,0x02,
0x02,0x08,0xd8,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,0x2b,
0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x01,0x01,0x03,0x05,0x00};

char cm_snmp_get_down_width[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x74,0x1b,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x01,0x02,0x03,0x05,0x00};

char cm_snmp_get_down_interleave[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x74,0x1d,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x01,0x01,0x04,0x03,0x05,0x00};


/*END:modify by whkuan 20120919*/

/***************END  ***** 获取CM 下行信息数据包*****************************/


/***************START  ***** 获取CM 上行信息数据包*****************************/

char cm_snmp_get_up_frequency[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,0x02,0x02,
0x41,0x8a,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,0x2b,0x06,
0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x01,0x04,0x05,0x00
};

char cm_snmp_get_up_channel_id[]=
	{0x30,0x2a,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1c,
0x02,0x02,0x4e,0xb1,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x10,
0x30,0x0e,0x06,0x0a,0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x05,0x00};

char cm_snmp_get_up_strength[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,0x02,0x02,0x4f,0xa9,
0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,0x2b,0x06,0x01,0x02,0x01,0x0a,
0x7f,0x01,0x02,0x02,0x01,0x02,0x02,0x05,0x00
	};

char cm_snmp_get_up_modulation[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,
0xa1,0x1f,0x02,0x02,0x2a,0xaf,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,
0x30,0x11,0x06,0x0d,0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x03,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelWidth[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xae,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x02,0x04,0x05,0x00};

char cm_snmp_get_up_SlotSize[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb0,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x04,0x04,0x05,0x00};

char cm_snmp_get_up_TxTimingOffset[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb1,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x05,0x04,0x05,0x00};

char cm_snmp_get_up_RangingBackoffStart[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb2,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x06,0x04,0x05,0x00};

char cm_snmp_get_up_RangingBackoffEnd[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb3,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x07,0x04,0x05,0x00};

char cm_snmp_get_up_TxBackoffStart[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb4,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x08,0x04,0x05,0x00};

char cm_snmp_get_up_TxBackoffEnd[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb5,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x09,0x04,0x05,0x00};

char cm_snmp_get_up_ScdmaActiveCodes[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb6,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0a,0x04,0x05,0x00};

char cm_snmp_get_up_ScdmaCodesPerSlot[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb7,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0b,0x04,0x05,0x00};

char cm_snmp_get_up_ScdmaFrameSize[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb8,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0c,0x04,0x05,0x00};

char cm_snmp_get_up_ScdmaHoppingSeed[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xb9,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0d,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelType[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xba,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0e,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelCloneFrom[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xbb,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x0f,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelUpdate[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xbc,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x10,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelStatus[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xbd,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x11,0x04,0x05,0x00};

char cm_snmp_get_up_ChannelPreEqEnable[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x2a,0xbe,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x01,0x02,0x01,0x12,0x04,0x05,0x00};

/***************END ***** 获取CM 上行信息数据包*****************************/

/***************START ***** 获取CM 状态信息数据包*****************************/
char cm_snmp_get_status_value[]=
{0x30,0x2a,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1c,
0x02,0x02,0x4f,0xa7,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x10,0x30,0x0e,0x06,0x0a,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x05,0x00};

char cm_snmp_get_status_code[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xa8,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x01,0x02,0x05,0x00};

char cm_snmp_get_status_reset[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xaa,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x03,0x02,0x05,0x00};

char cm_snmp_get_Status_LostSyncs[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xab,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x04,0x02,0x05,0x00};

char cm_snmp_get_Status_InvalidMaps[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xac,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x05,0x02,0x05,0x00};

char cm_snmp_get_Status_InvalidUcds[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xad,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x06,0x02,0x05,0x00};

char cm_snmp_get_Status_InvalidRangingResponses[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xae,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x07,0x02,0x05,0x00};

char cm_snmp_get_Status_InvalidRegistrationResponses[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xaf,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x08,0x02,0x05,0x00};

char cm_snmp_get_Status_T1Timeouts[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb0,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x09,0x02,0x05,0x00};

char cm_snmp_get_Status_T2Timeouts[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb1,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0a,0x02,0x05,0x00};

char cm_snmp_get_Status_T3Timeouts[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb2,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0b,0x02,0x05,0x00};

char cm_snmp_get_Status_T4Timeouts[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb3,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0c,0x02,0x05,0x00};

char cm_snmp_get_Status_RangingAborteds[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb4,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0d,0x02,0x05,0x00};

char cm_snmp_get_Status_DocsisOperMode[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb5,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0e,0x02,0x05,0x00};

char cm_snmp_get_Status_ModulationType[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x4f,0xb6,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x0f,0x02,0x05,0x00};

char cm_snmp_get_Status_EqualizationData[]=
{0x30,0x2d,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa1,0x1f,
0x02,0x02,0x5a,0x74,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x13,0x30,0x11,0x06,0x0d,
0x2b,0x06,0x01,0x02,0x01,0x0a,0x7f,0x01,0x02,0x02,0x01,0x10,0x02,0x05,0x00};

/***************END ***** 获取CM 状态信息数据包*****************************/

/***************START ***** 设置CM参数*****************************/
char cm_snmp_set_macaddr[]=
{0x30,0x37,0x02,0x01,0x00,0x04,0x07,0x70,0x72,0x69,0x76,0x61,0x74,0x65,0xa3,0x29,
0x02,0x02,0x69,0x6d,0x02,0x01,0x00,0x02,0x01,0x00,0x30,0x1d,0x30,0x1b,0x06,0x11,
0x2b,0x06,0x01,0x04,0x01,0xa2,0x3d,0x02,0x63,0x01,0x01,0x02,0x01,0x04,0x01,0x02,
0x01,0x04,0x06,0x00,0x23,0xb8,0x90,0x52,0x99};

unsigned int cm_generate_snmp_requestid()
{
    struct timeval tv;
    unsigned int ticks;
    unsigned int ret;

    gettimeofday(&tv, NULL);
    ticks = tv.tv_sec + tv.tv_usec;

    srand(ticks);

    ret = (rand() % 31765) + 1000;

    return ret;
}
/*
int cm_send_snmp_command(int command)
{
    int server_port;
    int ret;
    int len;
    int socketfd;
    int nbytes;
    fd_set rset;
    struct sockaddr_in remote_addr;
    struct sockaddr_in remote_addr1;
    struct timeval tmo;
    char recvbuf[128];
    unsigned int requestid;
    int retry_count=0;

    server_port = SNMP_PORT;

    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(0xc0a86401);
    remote_addr.sin_port = htons(server_port);

    len = sizeof(remote_addr);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0)
    {
        //socket error
        return -1;
    }

    requestid = cm_generate_snmp_requestid();
#if CM_SNMP_DEBUG
    printf("[%s] %s -- request id is %lx \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, requestid);
#endif

    retry_count = 0;
    while(retry_count < CM_CMD_RETRY_NUM)
    {
        // stop or start cmd 
        if(command == CM_CMD_PASSWORD)
        {
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_password_pdu, sizeof(cm_snmp_password_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_password_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_password_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
        else if(command == CM_CMD_FACTORY)
        {
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_factory_pdu, sizeof(cm_snmp_factory_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_factory_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_factory_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }

        else
        {
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET] = (requestid & 0x000000ff);
            cm_snmp_scanning_pdu[sizeof(cm_snmp_scanning_pdu)-1] = command;
            nbytes = sendto(socketfd, cm_snmp_scanning_pdu, sizeof(cm_snmp_scanning_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_scanning_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_scanning_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }

        if(nbytes <=0)
        {
            //send error;
#if CM_SNMP_DEBUG
            printf("[%s] %s -- send data return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }

        tmo.tv_sec = 2;
        // tmo.tv_usec = 500*1000; // 100 m-seconds
        tmo.tv_usec = 0;

        FD_ZERO(&rset);
        FD_SET(socketfd, &rset);
        ret = 0;
        ret = select(socketfd+1, &rset, NULL, NULL, &tmo);

        if(ret < 0)
        {
            //recv error or timeout
#if CM_SNMP_DEBUG
            printf("[%s] %s -- select return error [ret = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, ret);
#endif
            close(socketfd);
            return -1;
        }
        else if(ret == 0)
        {
            retry_count++;
            continue;
        }

        nbytes = recvfrom(socketfd, recvbuf, 128, 0,
                          (struct sockaddr *)&remote_addr1, &len);
        if(nbytes <= 0)
        {
            //recv error or no data
#if CM_SNMP_DEBUG
            printf("[%s] %s -- recv return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }
        else
        {
            //calculate response id
            if(((recvbuf[CM_PASSWORD_OFFSET-1] & 0x000000ff) == ((requestid >> 8) & 0x000000ff)) &&
               ((recvbuf[CM_PASSWORD_OFFSET] & 0x000000ff)== (requestid & 0x000000ff)))
            {
                //it's ok?
                break;
            }
            else
            {
                //data error
#if CM_SNMP_DEBUG
                printf("[%s] %s -- recv data error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
                retry_count++;
                continue;
            }
        }
    }

    //it's ok?
    if(retry_count >= CM_CMD_RETRY_NUM)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    close(socketfd);

    return ret;

}
*/

int cm_send_snmp_command(int command)
{
    int server_port;
    int ret;
    int len;
    int socketfd;
    int nbytes;
    fd_set rset;
    struct sockaddr_in remote_addr;
    struct sockaddr_in remote_addr1;
    struct timeval tmo;
    char recvbuf[256];
    unsigned int requestid;
    int retry_count=0;

    server_port = SNMP_PORT;

    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(0xc0a86401);
    remote_addr.sin_port = htons(server_port);

    len = sizeof(remote_addr);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0)
    {
        //socket error
        return -1;
    }

    requestid = cm_generate_snmp_requestid();
#if CM_SNMP_DEBUG
    printf("[%s] %s -- request id is %x \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, requestid);
#endif

    retry_count = 0;
    while(retry_count < CM_CMD_RETRY_NUM)
    {
        /* stop or start cmd */
        if(command == CM_CMD_PASSWORD)
        {
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_password_pdu, sizeof(cm_snmp_password_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_password_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_password_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
        else if(command == CM_CMD_FACTORY)
        {
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_factory_pdu, sizeof(cm_snmp_factory_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_factory_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_factory_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
        else if(command==CM_CMD_DOWNLINKFREQUENCY_FOURBYTES)
        {
            cm_snmp_frequency_pdu4[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_frequency_pdu4[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_frequency_pdu4, sizeof(cm_snmp_frequency_pdu4), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DOWNLINKFREQUENCY_THREEBYTES)
        {
            // NOTE:need change the pdu data
            cm_snmp_frequency_pdu3[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_frequency_pdu3[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_frequency_pdu3, sizeof(cm_snmp_frequency_pdu3), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DS_CHANNEL)
        {
            cm_snmp_Get_DSChannel[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_Get_DSChannel[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_Get_DSChannel, sizeof(cm_snmp_Get_DSChannel), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DIAGNOSTIC)
        {
            cm_snmp_diagnostic[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_diagnostic[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_diagnostic, sizeof(cm_snmp_diagnostic), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_PRIVATE_MIB_FACTORY)
        {
            cm_snmp_private_enable_factory[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_private_enable_factory[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_private_enable_factory, sizeof(cm_snmp_private_enable_factory), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_TUNNER_INDEX)
        {
            cm_snmp_tunerindex_zero[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_tunerindex_zero[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_tunerindex_zero, sizeof(cm_snmp_tunerindex_zero), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DS_OFFSET_ZERO)
        {
            cm_snmp_ds_offset_set_zero[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_ds_offset_set_zero[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_ds_offset_set_zero, sizeof(cm_snmp_ds_offset_set_zero), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_CAL_OFFSET_POWER)
        {
            cm_snmp_write_ds_offset[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_write_ds_offset[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_write_ds_offset, sizeof(cm_snmp_write_ds_offset), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DISABLE_FACTORY)
        {
            cm_snmp_disable_factory[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_disable_factory[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_disable_factory, sizeof(cm_snmp_disable_factory), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_OPER_MODE)
        {
            cm_snmp_set_DOCSIS[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_set_DOCSIS[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_set_DOCSIS, sizeof(cm_snmp_set_DOCSIS), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_COMMIT_SETTING)
        {
            cm_snmp_commit_setting[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_commit_setting[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_commit_setting, sizeof(cm_snmp_commit_setting), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_OFFSET_SETTING)
        {
            cm_snmp_get_vertity_setting[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_vertity_setting[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_vertity_setting, sizeof(cm_snmp_get_vertity_setting), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_MACADDR)
        {
            cm_snmp_get_MacAddr[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_MacAddr[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_MacAddr, sizeof(cm_snmp_get_MacAddr), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_PRIVATE_MIBENABLE)
        {
            cm_snmp_cdPrivateMibEnable[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_cdPrivateMibEnable[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_cdPrivateMibEnable, sizeof(cm_snmp_cdPrivateMibEnable), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        /***************START  ***** 获取CM 上行信息*****************************/
        else if(command==CM_CMD_GET_UP_FREQUENCY)
        {
            cm_snmp_get_up_frequency[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_frequency[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_frequency, sizeof(cm_snmp_get_up_frequency), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_CHANNEL_ID)
        {
            cm_snmp_get_up_channel_id[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_channel_id[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_channel_id, sizeof(cm_snmp_get_up_channel_id), 0,
                            (struct sockaddr *)&remote_addr, len);
        }

        else if(command==CM_CMD_GET_UP_STRENGTH)
        {
            cm_snmp_get_up_strength[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_strength[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_strength, sizeof(cm_snmp_get_up_strength), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_MODULATION)
        {
            cm_snmp_get_up_modulation[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_modulation[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_modulation, sizeof(cm_snmp_get_up_modulation), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        
        else if(command==CM_CMD_GET_UP_WIDTH)
        {
            cm_snmp_get_up_ChannelWidth[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelWidth[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelWidth, sizeof(cm_snmp_get_up_ChannelWidth), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_SLOT_SIZE)
        {
            cm_snmp_get_up_SlotSize[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_SlotSize[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_SlotSize, sizeof(cm_snmp_get_up_SlotSize), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_TX_TIM_OFFSET)
        {
            cm_snmp_get_up_TxTimingOffset[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_TxTimingOffset[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_TxTimingOffset, sizeof(cm_snmp_get_up_TxTimingOffset), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_RANG_BACK_START)
        {
            cm_snmp_get_up_RangingBackoffStart[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_RangingBackoffStart[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_RangingBackoffStart, sizeof(cm_snmp_get_up_RangingBackoffStart), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_RANG_BACK_END)
        {
            cm_snmp_get_up_RangingBackoffEnd[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_RangingBackoffEnd[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_RangingBackoffEnd, sizeof(cm_snmp_get_up_RangingBackoffEnd), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_TX_BACK_START)
        {
            cm_snmp_get_up_TxBackoffStart[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_TxBackoffStart[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_TxBackoffStart, sizeof(cm_snmp_get_up_TxBackoffStart), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_TX_BACK_END)
        {
            cm_snmp_get_up_TxBackoffEnd[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_TxBackoffEnd[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_TxBackoffEnd, sizeof(cm_snmp_get_up_TxBackoffEnd), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_SCDMA_ACT_CODE)
        {
            cm_snmp_get_up_ScdmaActiveCodes[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ScdmaActiveCodes[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ScdmaActiveCodes, sizeof(cm_snmp_get_up_ScdmaActiveCodes), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_SCDMA_CODE_PER_SLOT)
        {
            cm_snmp_get_up_ScdmaCodesPerSlot[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ScdmaCodesPerSlot[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ScdmaCodesPerSlot, sizeof(cm_snmp_get_up_ScdmaCodesPerSlot), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_SCDMA_FRAME_SIZE)
        {
            cm_snmp_get_up_ScdmaFrameSize[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ScdmaFrameSize[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ScdmaFrameSize, sizeof(cm_snmp_get_up_ScdmaFrameSize), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_SCDMA_HOP_SEED)
        {
            cm_snmp_get_up_ScdmaHoppingSeed[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ScdmaHoppingSeed[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ScdmaHoppingSeed, sizeof(cm_snmp_get_up_ScdmaHoppingSeed), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_CHANNEL_TYPE)
        {
            cm_snmp_get_up_ChannelType[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelType[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelType, sizeof(cm_snmp_get_up_ChannelType), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_CLONE_FROM)
        {
            cm_snmp_get_up_ChannelCloneFrom[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelCloneFrom[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelCloneFrom, sizeof(cm_snmp_get_up_ChannelCloneFrom), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_UPDATE)
        {
            cm_snmp_get_up_ChannelUpdate[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelUpdate[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelUpdate, sizeof(cm_snmp_get_up_ChannelUpdate), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_STATUS)
        {
            cm_snmp_get_up_ChannelStatus[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelStatus[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelStatus, sizeof(cm_snmp_get_up_ChannelStatus), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_UP_PRE_EQ_ENABLE)
        {
            cm_snmp_get_up_ChannelPreEqEnable[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_up_ChannelPreEqEnable[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_up_ChannelPreEqEnable, sizeof(cm_snmp_get_up_ChannelPreEqEnable), 0,
                            (struct sockaddr *)&remote_addr, len);
        } 
        
        /***************END  ***** 获取CM 上行信息*****************************/

        /***************START  ***** 获取CM 下行信息*****************************/

        else if(command==CM_CMD_GET_DOWN_SNR)
        {
            cm_snmp_get_down_snr[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_snr[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_snr, sizeof(cm_snmp_get_down_snr), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DOWN_MODULATION)
        {
            cm_snmp_get_down_modulation[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_modulation[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_modulation, sizeof(cm_snmp_get_down_modulation), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DOWN_CHANNEL_ID)
        {
            cm_snmp_get_down_channel_id[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_channel_id[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_channel_id, sizeof(cm_snmp_get_down_channel_id), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DOWN_STRENGTH)
        {
            cm_snmp_get_down_strength[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_strength[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_strength, sizeof(cm_snmp_get_down_strength), 0,
                            (struct sockaddr *)&remote_addr, len);
        }

        else if(command==CM_CMD_GET_DOWN_FREQUENCY)
        {
            cm_snmp_get_down_frequency[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_frequency[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_frequency, sizeof(cm_snmp_get_down_frequency), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DOWN_WIDTH)
        {
           cm_snmp_get_down_width[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_width[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_width, sizeof(cm_snmp_get_down_width), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DOWN_INTER_LEAVE)
        {
            cm_snmp_get_down_interleave[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_down_interleave[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_down_interleave, sizeof(cm_snmp_get_down_interleave), 0,
                            (struct sockaddr *)&remote_addr, len);
        }

        /*****************START*********获取CM 状态信息************************/
        else if(command==CM_CMD_GET_STAT_VALUE)
        {
            cm_snmp_get_status_value[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_status_value[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_status_value, sizeof(cm_snmp_get_status_value), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_CODE)
        {
             cm_snmp_get_status_code[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_status_code[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_status_code, sizeof(cm_snmp_get_status_code), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_RESET)
        {
            cm_snmp_get_status_reset[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_status_reset[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_status_reset, sizeof(cm_snmp_get_status_reset), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_LOST_SYNC)
        {
            cm_snmp_get_Status_LostSyncs[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_LostSyncs[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_LostSyncs, sizeof(cm_snmp_get_Status_LostSyncs), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_INVALID_MAP)
        {
            cm_snmp_get_Status_InvalidMaps[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_InvalidMaps[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_InvalidMaps, sizeof(cm_snmp_get_Status_InvalidMaps), 0,
                            (struct sockaddr *)&remote_addr, len); 
        }
        else if(command==CM_CMD_GET_STAT_INVALID_UCD)
        {
            cm_snmp_get_Status_InvalidUcds[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_InvalidUcds[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_InvalidUcds, sizeof(cm_snmp_get_Status_InvalidUcds), 0,
                            (struct sockaddr *)&remote_addr, len);  
        }
        else if(command==CM_CMD_GET_STAT_INVALID_RANG_RES)
        {
            cm_snmp_get_Status_InvalidRangingResponses[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_InvalidRangingResponses[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_InvalidRangingResponses, sizeof(cm_snmp_get_Status_InvalidRangingResponses), 0,
                            (struct sockaddr *)&remote_addr, len);  
        }
        else if(command==CM_CMD_GET_STAT_INVALID_REG_RES)
        {
            cm_snmp_get_Status_InvalidRegistrationResponses[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_InvalidRegistrationResponses[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_InvalidRegistrationResponses, sizeof(cm_snmp_get_Status_InvalidRegistrationResponses), 0,
                            (struct sockaddr *)&remote_addr, len);
        }

        else if(command==CM_CMD_GET_STAT_INVALID_REG_RES)
        {
            cm_snmp_get_Status_InvalidRegistrationResponses[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_InvalidRegistrationResponses[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_InvalidRegistrationResponses, sizeof(cm_snmp_get_Status_InvalidRegistrationResponses), 0,
                            (struct sockaddr *)&remote_addr, len);
        }

        else if(command==CM_CMD_GET_STAT_T1_TIMEOUT)
        {
            cm_snmp_get_Status_T1Timeouts[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_T1Timeouts[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_T1Timeouts, sizeof(cm_snmp_get_Status_T1Timeouts), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_T2_TIMEOUT)
        {
            cm_snmp_get_Status_T2Timeouts[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_T2Timeouts[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_T2Timeouts, sizeof(cm_snmp_get_Status_T2Timeouts), 0,
                            (struct sockaddr *)&remote_addr, len); 
        }
        else if(command==CM_CMD_GET_STAT_T3_TIMEOUT)
        {
            cm_snmp_get_Status_T3Timeouts[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_T3Timeouts[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_T3Timeouts, sizeof(cm_snmp_get_Status_T3Timeouts), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_T4_TIMEOUT)
        {
            cm_snmp_get_Status_T4Timeouts[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_T4Timeouts[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_T4Timeouts, sizeof(cm_snmp_get_Status_T4Timeouts), 0,
                            (struct sockaddr *)&remote_addr, len);    
        }
        else if(command==CM_CMD_GET_STAT_RANG_ABORT)
        {
            cm_snmp_get_Status_RangingAborteds[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_RangingAborteds[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_RangingAborteds, sizeof(cm_snmp_get_Status_RangingAborteds), 0,
                            (struct sockaddr *)&remote_addr, len);
            
        }
        else if(command==CM_CMD_GET_STAT_DOCSIS_OPER_MODE)
        { 
            cm_snmp_get_Status_DocsisOperMode[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_DocsisOperMode[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_DocsisOperMode, sizeof(cm_snmp_get_Status_DocsisOperMode), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_MUDO_TYPE)
        {
            cm_snmp_get_Status_ModulationType[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_ModulationType[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_ModulationType, sizeof(cm_snmp_get_Status_ModulationType), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_STAT_EQ_DATA)
        {
            cm_snmp_get_Status_EqualizationData[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_Status_EqualizationData[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_Status_EqualizationData, sizeof(cm_snmp_get_Status_EqualizationData), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        /*****************END*********获取CM状态信息*************************/


        /*****************START*********设置CM参数*************************/
        else if(command==CM_CMD_SET_MACADDR) //设置CM的MAC地址
        {
            cm_snmp_set_macaddr[CM_REQUEST_ID_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_set_macaddr[CM_REQUEST_ID_OFFSET] = (requestid & 0x000000ff);

            memmove(&cm_snmp_set_macaddr[CM_MACADDR_OFFSET-1], gCmMacAddr, 6);     
            nbytes = sendto(socketfd, cm_snmp_set_macaddr, sizeof(cm_snmp_set_macaddr), 0,
                            (struct sockaddr *)&remote_addr, len);
        }        
        else
        {
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET] = (requestid & 0x000000ff);
            cm_snmp_scanning_pdu[sizeof(cm_snmp_scanning_pdu)-1] = command;
            nbytes = sendto(socketfd, cm_snmp_scanning_pdu, sizeof(cm_snmp_scanning_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_scanning_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_scanning_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
		
NoCompare://如果在判断语句中添加直接跳转的功能，可以减少部分时间开销，暂时还不添加wlei
	
        if(nbytes <=0)
        {
            //send error;
#if CM_SNMP_DEBUG
            printf("[%s] %s -- send data return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }

        tmo.tv_sec = 2;
        // tmo.tv_usec = 500*1000; /* 100 m-seconds */
        tmo.tv_usec = 0;

        FD_ZERO(&rset);
        FD_SET(socketfd, &rset);

        ret = 0;
        ret = select(socketfd+1, &rset, NULL, NULL, &tmo);

        if(ret < 0)
        {
            //recv error or timeout
#if CM_SNMP_DEBUG
            printf("[%s] %s -- select return error [ret = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, ret);
#endif
            close(socketfd);
            return -1;
        }
        else if(ret == 0)
        {
            retry_count++;
            continue;
        }
        /*sendto 的最后一个参数是整数值，而 recvfrom 的最后一个参数是一个指向整数值的指针，注意一下*/
        nbytes = recvfrom(socketfd, recvbuf, 256, 0,
                          (struct sockaddr *)&remote_addr1, &len);

        if(nbytes <= 0)
        {
            //recv error or no data
#if CM_SNMP_DEBUG
            printf("[%s] %s -- recv return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }
        else
        {
            //calculate response id
            if(((recvbuf[CM_PASSWORD_OFFSET-1] & 0x000000ff) == ((requestid >> 8) & 0x000000ff)) &&
               ((recvbuf[CM_PASSWORD_OFFSET] & 0x000000ff)== (requestid & 0x000000ff)))
            {
                if(command==CM_CMD_GET_DS_CHANNEL)
                {
                    if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x36)
                    {
                        int i=0;
                        return_DS_Channel=0x00000000;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        int j=return_DS_Channel;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                    }
                    else if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x35)
                    {
                        int i=0;
                        return_DS_Channel=0x00000000;

                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        int j=return_DS_Channel;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);

                    }
                }
                else if(command==CM_CMD_GET_OFFSET_SETTING)
                {
                    int i=0;
                    cm_return_DS_Offset=0x00000000;
                    memcpy(&cm_return_DS_Offset,&recvbuf[CM_DSCHANNEL_OFFSET+i],1);
                    i++;
                    //cm_return_DS_Offset<<8;//????
                    cm_return_DS_Offset = cm_return_DS_Offset<<8;
                    memcpy(&cm_return_DS_Offset,&recvbuf[CM_DSCHANNEL_OFFSET+i],1);
                }
                else if(command==CM_CMD_GET_MACADDR)
                {
                    memcpy(gCmInfoDesc.cm_mac,&recvbuf[CM_DSCHANNEL_OFFSET-1],6);
                }
                # if 0
                else if(command==CM_CMD_GET_UP_FREQUENCY)
                {
                    if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x36)
                    {
                        int i=0;
                        cm_Up_Frequency=0x00000000;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        cm_Up_Frequency=cm_Up_Frequency<<8;
                        int j=cm_Up_Frequency;
                        i++;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        cm_Up_Frequency=cm_Up_Frequency<<8;
                        i++;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        cm_Up_Frequency=cm_Up_Frequency<<8;
                        i++;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                    }
                    else if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x35)
                    {
                        int i=0;
                        cm_Up_Frequency=0x00000000;

                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        cm_Up_Frequency=cm_Up_Frequency<<8;
                        int j=cm_Up_Frequency;
                        i++;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        cm_Up_Frequency=cm_Up_Frequency<<8;
                        i++;
                        memcpy(&cm_Up_Frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);

                    }

                }
                #endif

                else if(command==CM_CMD_GET_UP_FREQUENCY)
                {
                    if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x36)
                    {
                        int i=0;
                        gCmInfoDesc.cm_up.cur_frequency=0x00000000;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        gCmInfoDesc.cm_up.cur_frequency = gCmInfoDesc.cm_up.cur_frequency<<8;
                        i++;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        gCmInfoDesc.cm_up.cur_frequency = gCmInfoDesc.cm_up.cur_frequency<<8;
                        i++;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        gCmInfoDesc.cm_up.cur_frequency = gCmInfoDesc.cm_up.cur_frequency<<8;
                        i++;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                    }
                    else if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x35)
                    {
                        int i=0;
                        gCmInfoDesc.cm_up.cur_frequency=0x00000000;

                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        gCmInfoDesc.cm_up.cur_frequency=gCmInfoDesc.cm_up.cur_frequency<<8;
                        i++;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        gCmInfoDesc.cm_up.cur_frequency=gCmInfoDesc.cm_up.cur_frequency<<8;
                        i++;
                        memcpy(&gCmInfoDesc.cm_up.cur_frequency,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);               
                    }
                }

                /*BEGIN: ***get cm up info **** modify by whkuan 20120919 */
                else if(command==CM_CMD_GET_UP_CHANNEL_ID)
                {
                    gCmInfoDesc.cm_up.channel_id=0x00000000;
                    memcpy(&gCmInfoDesc.cm_up.channel_id,&recvbuf[CM_UP_CHANNEL_ID_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_UP_STRENGTH)
                {
                    gCmInfoDesc.cm_up.cm_signal_strength=0x00000000;
                    memcpy(&gCmInfoDesc.cm_up.cm_signal_strength,&recvbuf[CM_UP_STRENTH_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_UP_MODULATION)
                {
                    gCmInfoDesc.cm_up.cm_modulation=0x00000000;
                    memcpy(&gCmInfoDesc.cm_up.cm_modulation,&recvbuf[CM_UP_MODULATION_OFFSET-1] ,1);

                }
                /*---------------------*/
                else if(command==CM_CMD_GET_UP_WIDTH)
                {
                    gCmInfoDesc.cm_extend.UpWidth=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpWidth,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_SLOT_SIZE)
                {
                    gCmInfoDesc.cm_extend.UpSlotSize=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpSlotSize,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_TX_TIM_OFFSET)
                {
                    gCmInfoDesc.cm_extend.UpTxTimOffset=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpTxTimOffset,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_RANG_BACK_START)
                {
                    gCmInfoDesc.cm_extend.UpRangBackStart=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpRangBackStart,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_RANG_BACK_END)
                {
                    gCmInfoDesc.cm_extend.UpRangBackEnd=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpRangBackEnd,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_TX_BACK_START)
                {
                    gCmInfoDesc.cm_extend.UpTxBackStart=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpTxBackStart,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_TX_BACK_END)
                {
                    gCmInfoDesc.cm_extend.UpTxBackEnd=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpTxBackEnd,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_SCDMA_ACT_CODE)
                {
                    gCmInfoDesc.cm_extend.UpScdmaActCodes=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpScdmaActCodes,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_SCDMA_CODE_PER_SLOT)
                {
                    gCmInfoDesc.cm_extend.UpScdmaCodesPerSlot=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpScdmaCodesPerSlot,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_SCDMA_FRAME_SIZE)
                {
                    gCmInfoDesc.cm_extend.UpScdmaFraSize=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpScdmaFraSize,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_SCDMA_HOP_SEED)
                {
                    gCmInfoDesc.cm_extend.UpScdmaHopSeed=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpScdmaHopSeed,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_CHANNEL_TYPE)
                {
                    gCmInfoDesc.cm_extend.UpType=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpType,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_CLONE_FROM)
                {
                    gCmInfoDesc.cm_extend.UpCloFrom=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpCloFrom,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_UPDATE)
                {
                    gCmInfoDesc.cm_extend.UpUpdate=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpUpdate,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_STATUS)
                {
                    gCmInfoDesc.cm_extend.UpStatus=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpStatus,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_UP_PRE_EQ_ENABLE)
                {
                    gCmInfoDesc.cm_extend.UpPreEqEnable=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.UpPreEqEnable,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_DOWN_WIDTH)
                {
                    gCmInfoDesc.cm_extend.DownWidth=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.DownWidth,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_DOWN_INTER_LEAVE)
                {
                    gCmInfoDesc.cm_extend.DownInterleave=0x00000000;
                    memcpy(&gCmInfoDesc.cm_extend.DownInterleave,&recvbuf[CM_BIND_VALUE_OFFSET-1] ,1);

                }
                /*----------------------*/
                /*END: ***get cm up info **** modify by whkuan 20120919 */

                /*BEGIN: ***get cm down info **** modify by whkuan 20120919 */
                else if(command==CM_CMD_GET_DOWN_FREQUENCY)
                {
                    gCmInfoDesc.cm_down.cur_frequency=0x00000000;
                    memcpy(&gCmInfoDesc.cm_down.cur_frequency,&recvbuf[CM_DOWN_FREQUENCY_OFFSET-1] ,1);
                    gCmInfoDesc.cm_down.cur_frequency<<=8;
                    memcpy(&gCmInfoDesc.cm_down.cur_frequency,&recvbuf[CM_DOWN_FREQUENCY_OFFSET] ,1);
                    gCmInfoDesc.cm_down.cur_frequency<<=8;
                    memcpy(&gCmInfoDesc.cm_down.cur_frequency,&recvbuf[CM_DOWN_FREQUENCY_OFFSET+1] ,1);
                    gCmInfoDesc.cm_down.cur_frequency<<=8;
                    memcpy(&gCmInfoDesc.cm_down.cur_frequency,&recvbuf[CM_DOWN_FREQUENCY_OFFSET+2] ,1);
                }
                else if(command==CM_CMD_GET_DOWN_CHANNEL_ID)
                {
                    gCmInfoDesc.cm_down.channel_id=0x00000000;
                    memcpy(&gCmInfoDesc.cm_down.channel_id,&recvbuf[CM_DOWN_CHANNEL_ID_OFFSET-1] ,1);

                }
                else if(command==CM_CMD_GET_DOWN_SNR)
                {
                    gCmInfoDesc.cm_down.cm_snr = 0x00000000;
                    memcpy(&gCmInfoDesc.cm_down.cm_snr,&recvbuf[CM_DOWN_SNR_OFFSET-1] ,1);
                    gCmInfoDesc.cm_down.cm_snr<<=8;
                    memcpy(&gCmInfoDesc.cm_down.cm_snr,&recvbuf[CM_DOWN_SNR_OFFSET] ,1);
                }
                else if(command==CM_CMD_GET_DOWN_MODULATION)
                {
                    gCmInfoDesc.cm_down.cm_modulation = 0x00000000;
                    memcpy(&gCmInfoDesc.cm_down.cm_modulation,&recvbuf[CM_DOWN_CHANNEL_ID_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_DOWN_STRENGTH)
                {
                    if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x2f)
                    {
                        gCmInfoDesc.cm_down.cm_signal_strength= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_down.cm_signal_strength,&recvbuf[CM_DOWN_STRENTH_OFFSET-1] ,1);
                        gCmInfoDesc.cm_down.cm_signal_strength<<=8;
                        memcpy(&gCmInfoDesc.cm_down.cm_signal_strength,&recvbuf[CM_DOWN_STRENTH_OFFSET] ,1);
                    }
                    
                    else if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x2e)
                    {
                        gCmInfoDesc.cm_down.cm_signal_strength = 0x00000000;
                        memcpy(&gCmInfoDesc.cm_down.cm_signal_strength,(char *)&recvbuf[CM_DOWN_STRENTH_OFFSET-1] ,1);
                    }
                }
                /*END: ***get cm down info **** modify by whkuan 20120919 */

                /*** BEGIN : get status info **** add by whkuan 20120924**/

                else if(command==CM_CMD_GET_STAT_VALUE)
                {
                    gCmInfoDesc.cm_status.CmStatValue = 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatValue,(char *)&recvbuf[CM_STAT_VALUE_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_CODE)
                {
                    memset(gCmInfoDesc.cm_status.CmStatCode, 0, 10);
                        memcpy(gCmInfoDesc.cm_status.CmStatCode,(char *)&recvbuf[CM_STAT_CODE_OFFSET-1] ,4);
                }
                else if(command==CM_CMD_GET_STAT_RESET)
                {
                    gCmInfoDesc.cm_status.CmStatResets= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatResets,(char *)&recvbuf[CM_STAT_RESET_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_LOST_SYNC)
                {
                    gCmInfoDesc.cm_status.CmStatLostSyncs= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatLostSyncs,(char *)&recvbuf[CM_STAT_LOST_SYNC_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_INVALID_MAP)
                {
                    gCmInfoDesc.cm_status.CmStatInvMaps= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatLostSyncs,(char *)&recvbuf[CM_STAT_INVALID_MAP_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_INVALID_UCD)
                {
                    gCmInfoDesc.cm_status.CmStatInvUcds= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatInvUcds,(char *)&recvbuf[CM_STAT_INVALID_UCD_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_INVALID_RANG_RES)
                {
                    gCmInfoDesc.cm_status.CmStatInvRangRes= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatInvRangRes,(char *)&recvbuf[CM_STAT_INVALID_RING_RES_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_INVALID_REG_RES)
                {
                    gCmInfoDesc.cm_status.CmStatInvRegRes= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatInvRegRes,(char *)&recvbuf[CM_STAT_INVALID_REG_RES_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_T1_TIMEOUT)
                {
                    gCmInfoDesc.cm_status.CmStatT1Timeout= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatT1Timeout,(char *)&recvbuf[CM_STAT_T1TIMEOUT_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_T2_TIMEOUT)
                {
                    gCmInfoDesc.cm_status.CmStatT2Timeout= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatT2Timeout,(char *)&recvbuf[CM_STAT_T2TIMEOUT_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_T3_TIMEOUT)
                {
                    gCmInfoDesc.cm_status.CmStatT3Timeout= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatT3Timeout,(char *)&recvbuf[CM_STAT_T3TIMEOUT_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_T4_TIMEOUT)
                {
                    gCmInfoDesc.cm_status.CmStatT4Timeout= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatT4Timeout,(char *)&recvbuf[CM_STAT_T4TIMEOUT_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_RANG_ABORT)
                {
                    gCmInfoDesc.cm_status.CmStatRangAbr= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatRangAbr,(char *)&recvbuf[CM_STAT_RANG_ABORT_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_DOCSIS_OPER_MODE)
                {
                    gCmInfoDesc.cm_status.CmStatDocOperMode= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatDocOperMode,(char *)&recvbuf[CM_STAT_DOC_OPER_MODE_OFFSET-1] ,1);
                }
                else if(command==CM_CMD_GET_STAT_MUDO_TYPE)
                {
                    gCmInfoDesc.cm_status.CmStatModuType= 0x00000000;
                        memcpy(&gCmInfoDesc.cm_status.CmStatModuType,(char *)&recvbuf[CM_STAT_MODU_TYPE_OFFSET-1] ,1);
                }    
            //it's ok?
                break;
            }
            else if(((recvbuf[CM_REQUEST_ID_EQDATA_OFFSET-1] & 0x000000ff) == ((requestid >> 8) & 0x000000ff)) &&
               ((recvbuf[CM_REQUEST_ID_EQDATA_OFFSET] & 0x000000ff)== (requestid & 0x000000ff)))
            {
                if(command==CM_CMD_GET_STAT_EQ_DATA)
                {
                        memset(gCmInfoDesc.cm_status.CmStatusEqData, 0,101);
                        memcpy(gCmInfoDesc.cm_status.CmStatusEqData,(char *)&recvbuf[CM_STAT_EQ_DATA_OFFSET-1] ,100);                  
                }
                break;
            }
               
            else
            {
                //data error
#if CM_SNMP_DEBUG
                printf("[%s] %s -- recv data error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
                retry_count++;
                continue;
            }
        }
    }

    //it's ok?
    if(retry_count >= CM_CMD_RETRY_NUM)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    close(socketfd);

    return ret;

}

int cm_stop_scanning()
{
    int ret;
    ret = cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret < 0)
        return ret;

    ret = cm_send_snmp_command(CM_CMD_FACTORY);
    if(ret < 0)
        return ret;

    return cm_send_snmp_command(CM_CMD_STOP_SCANNING);
}

int cm_restart_scanning()
{
    int ret;
    ret = cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret < 0)
        return ret;

    ret = cm_send_snmp_command(CM_CMD_FACTORY);
    if(ret < 0)
        return ret;

    return cm_send_snmp_command(CM_CMD_START_SCANNING);
}

#if 0
int main(int argc, char *argv[])
{
    //test
    cm_stop_scanning();
    cm_restart_scanning();
    return 1;
}
#endif

int cm_set_DownlinkFrequency(int Bytes)
{
	/*
    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    */
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
//    sys_thread_add(pthread_id);
    int ret ;

    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
        return ret;

    /*
        ret=cm_send_snmp_command(CM_CMD_FACTORY);
        if(ret<0)
            return ret;
    */
    if(Bytes==4)
    {
        ret= cm_send_snmp_command(CM_CMD_DOWNLINKFREQUENCY_FOURBYTES);
    }
    else
    {
        ret= cm_send_snmp_command(CM_CMD_DOWNLINKFREQUENCY_THREEBYTES);
    }

 //   sys_thread_del(pthread_id);
    return ret;

}

#define ERROR  -1
#define RIGHT  0
int cm_set_DS_Channel(int DSFrequency)
{
    int ret=0;
    //首先判断下行频率值是否是正常范围:1MHZ~1000MHZ(1 00 0000~1000 00 0000)
    if(DSFrequency<1000000||DSFrequency>1000000000)
    {
        return ERROR;
    }
    if(DSFrequency<=16000000)
    {
        int i=2;

        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        DSFrequency=DSFrequency>>8;
        int j=DSFrequency;
        i--;
        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        ret=cm_set_DownlinkFrequency(3);
    }
    else
    {
        int i=3;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency= DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);
        ret=cm_set_DownlinkFrequency(4);
    }
    return ret;


}


/*--------------------------------used for CM Calibration----------------------------------------*/
/*
int cm_Configure_Selected_Frequency(int DSFrequency)
{
    int ret=0;
    int i=3;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency=DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency=DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency= DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);
    ret=cm_send_snmp_command(CM_CMD_CONFIGURE_SELECTED_FREQUENCY)
        return ret;


}
*/
int cm_Downstream_Calibration()
{
	/*
    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    */
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
//    sys_thread_add(pthread_id);
    int ret ;

    /*校准之前先要停止Cable Modem 的扫描，不让其上线，然后才能进行相应的操作
        最后完成后要把Cable Modem 的扫描开启*/
    ret=cm_stop_scanning();
    /*1*/
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
    {
//        sys_thread_del(pthread_id);
        return ret;
    }
    //Err_Print("PassWord 发送正确\n");

    ret=cm_send_snmp_command(CM_CMD_DIAGNOSTIC);
    ret=cm_send_snmp_command(CM_CMD_PRIVATE_MIB_FACTORY);

    /*2、对每一个downstream tunner 都需要进行，3349 只支持一个*/
    ret=cm_send_snmp_command(CM_CMD_TUNNER_INDEX);
    /*3*/
    ret=cm_send_snmp_command(CM_CMD_DS_OFFSET_ZERO);

#if 0
    /*4*/
    int i=0;
    int dsfrequency=93000000;
    for(i=0; i<14; i++)
    {
        /*5*/
        ret= cm_Configure_Selected_Frequency(dsfrequency+i*60000000);
        ret=cm_send_snmp_command(CM_CMD_CAL_MODULATION);
        /*6*/
        ret=cm_send_snmp_command(CM_CMD_LOCK_NOW);
        task_delay(time_ticks_per_sec()*5); /* delay 5 s */
        /*7*/
        int VerityQam=0;
        VerityQam=cm_send_snmp_command(CM_CMD_CAL_QAM_LOCKED);
        int VeritySNR=0;
        downstream_power_index=i;
        VeritySNR=cm_send_snmp_command(CM_CMD_SNR);
        /*对获得的值进行操作，上述两个均有数据返回，注意进行相应的判断，然后再往下继续进行*/
        /*8*/
        int downstream_power=0;
        downstream_power=cm_send_snmp_command(CM_CMD_DOWNCHANNEL_POWER);
        /*9*/
        /*10*/

    }
#endif
    /*11*/
    int j=0;
    for(j=1; j<=14; j++)
    {
        //需要修改14个频点的OFFSET，依靠修改MIB树下节点的序号即可，Note the Number is started by 1,not zero(0)
        //避免构造大量的数据包
        memcpy(cm_snmp_write_ds_offset+49,&j,1);//改变数据包所指向CM 对象的序列号
	memcpy(cm_snmp_write_ds_offset+52,&store_downstream_power[j*2-2],2);//改变对应的OFFSET 值
        ret=cm_send_snmp_command(CM_CMD_CAL_OFFSET_POWER);
    }
    /*12*/
    ret=cm_send_snmp_command(CM_CMD_DISABLE_FACTORY);
    ret=cm_send_snmp_command(CM_CMD_OPER_MODE);
    /*13*/
    ret=cm_send_snmp_command(CM_CMD_COMMIT_SETTING);

     ret=cm_restart_scanning(); 
//sys_thread_del(pthread_id);

    return ret;

}

/*设置Cable Modem的 MAC地址 */
void cm_Set_MAC_Address(uint8_t *mac)
{
    /*设置MAC地址的命令*/
    /*snmpset -v 1 -c private 192.168.100.1 cdMacAddress.1 x 0023b8905299*/
    
    int ret=0;

    memmove(gCmMacAddr, mac, 6);
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_SET_MACADDR);
    ret=cm_send_snmp_command(CM_CMD_PRIVATE_MIBENABLE);
}



/*获取Cable Modem 的MAC 地址 wlei*/
void cm_Get_MAC_Address(uint8_t *mac)
{
   // task_t *pthread;
//    u32_t pthread_id;

//    pthread = task_id();
//    pthread_id = (u32_t)pthread;
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
 //   sys_thread_add(pthread_id);
    /*获取MAC 地址的命令为:
    snmpset -v 1 -c private 192.168.100.1 cdPvtMibEnableKeyValue.1 s password
    snmpget -v2c -c private 192.168.100.1 cdMacAddress.1
    snmpset -v 1 -c private 192.168.100.1 cdPrivateMibEnable.0 i disabled

    */
    int ret=0;
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_MACADDR);
    ret=cm_send_snmp_command(CM_CMD_PRIVATE_MIBENABLE);

  //  sys_thread_del(pthread_id);
    memmove(mac, gCmInfoDesc.cm_mac, 6);
}


/*BEGIN: 获取Cable Modem 信息 add by whkuan */
/*
STB_CM_info_desc_t * cm_Get_Up_Channel_Info()
{
    STB_CM_info_desc_t *pinfo = &gCmInfoDesc;
   
    int ret=0;
    ret=cm_send_snmp_command(CM_CMD_GET_UP_CHANNEL_ID);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_FREQUENCY);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_MODULATION);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_STRENGTH);

    ret=cm_send_snmp_command(CM_CMD_GET_UP_WIDTH); 
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SLOT_SIZE);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_TIM_OFFSET);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_RANG_BACK_START);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_RANG_BACK_END);

    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_BACK_START); 
    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_BACK_END);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_ACT_CODE);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_CODE_PER_SLOT);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_FRAME_SIZE);

    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_HOP_SEED);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_CLONE_FROM);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_UPDATE);

    ret=cm_send_snmp_command(CM_CMD_GET_UP_STATUS);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_PRE_EQ_ENABLE);
    return pinfo;
}
*/
/*获取Cable Modem 的上行信道 id */
uint32_t cm_Get_Up_ChannelId()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_CHANNEL_ID);
    tmp = (uint32_t)gCmInfoDesc.cm_up.channel_id;
    return tmp;       
}
/*获取Cable Modem 的上行频率*/
uint32_t cm_Get_Up_ChannelFrequency()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_FREQUENCY);
    tmp = (uint32_t)gCmInfoDesc.cm_up.cur_frequency;
    return tmp;       
}
/*获取Cable Modem 的上行调制方式*/
uint16_t cm_Get_Up_ChannelModulation()
{
    int ret;
    uint16_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_MODULATION);
    tmp = (uint16_t)gCmInfoDesc.cm_up.cm_modulation;
    return tmp;       
}
/*获取Cable Modem 的上行信号强度*/
float cm_Get_Up_ChannelStrenth()
{
    int ret;
    float tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_STRENGTH);
    tmp = (float)gCmInfoDesc.cm_up.cm_signal_strength/10;
    return tmp;       
}

int32_t cm_Get_Up_ChannelWidth()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_WIDTH);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpWidth;
    return tmp;       
}

int32_t cm_Get_Up_ChannelSlotSize()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SLOT_SIZE);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpWidth;
    return tmp;       
}

uint32_t cm_Get_Up_ChannelTxTimOffset()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_TIM_OFFSET);
    tmp = (uint32_t)gCmInfoDesc.cm_extend.UpTxTimOffset;
    return tmp;       
}

int32_t cm_Get_Up_ChannelRangBackStart()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_RANG_BACK_START);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpRangBackStart;
    return tmp;       
}

int32_t cm_Get_Up_ChannelRangBackEnd()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_RANG_BACK_END);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpRangBackEnd;
    return tmp;       
}

int32_t cm_Get_Up_ChannelTxBackStart()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_BACK_START);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpTxBackStart;
    return tmp;       
}

int32_t cm_Get_Up_ChannelTxBackEnd()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_TX_BACK_END);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpTxBackEnd;
    return tmp;       
}

uint32_t cm_Get_Up_ChannelScdmaActCode()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_ACT_CODE);
    tmp = (uint32_t)gCmInfoDesc.cm_extend.UpScdmaActCodes;
    return tmp;       
}

int32_t cm_Get_Up_ChannelScdmaCodePerSlot()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_CODE_PER_SLOT);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpScdmaCodesPerSlot;
    return tmp;       
}

uint32_t cm_Get_Up_ChannelScdmaFrameSize()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_FRAME_SIZE);
    tmp = (uint32_t)gCmInfoDesc.cm_extend.UpScdmaFraSize;
    return tmp;       
}

uint32_t cm_Get_Up_ChannelScdmaHopSeed()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_SCDMA_HOP_SEED);
    tmp = (uint32_t)gCmInfoDesc.cm_extend.UpScdmaHopSeed;
    return tmp;       
}

int32_t cm_Get_Up_ChannelCloneFrom()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_CLONE_FROM);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpCloFrom;
    return tmp;       
}

int32_t cm_Get_Up_ChannelUpdate()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_UPDATE);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpUpdate;
    return tmp;       
}

int32_t cm_Get_Up_ChannelStatus()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_STATUS);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpStatus;
    return tmp;       
}

int32_t cm_Get_Up_ChannelPreEqEnable()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_UP_PRE_EQ_ENABLE);
    tmp = (int32_t)gCmInfoDesc.cm_extend.UpPreEqEnable;
    return tmp;       
}


/*
STB_CM_info_desc_t * cm_Get_Down_Channel_Info()
{
    STB_CM_info_desc_t *pinfo = &gCmInfoDesc;
   
    int ret=0;
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_CHANNEL_ID); 
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_FREQUENCY);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_MODULATION);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_STRENGTH);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_SNR);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_WIDTH);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_INTER_LEAVE);

    return pinfo;
}
*/

uint32_t cm_Get_DownChannelId()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_CHANNEL_ID);
    tmp = (uint32_t)gCmInfoDesc.cm_down.channel_id;
    return tmp;       
}

int32_t cm_Get_DownChannelFrequency()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_FREQUENCY);
    tmp = (int32_t)gCmInfoDesc.cm_down.cur_frequency;
    return tmp;        
}

int32_t cm_Get_DownChannelWidth()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_WIDTH);
    tmp = (int32_t)gCmInfoDesc.cm_extend.DownWidth;
    return tmp;        
}

int32_t cm_Get_DownChannelModulation()
{
    int ret;
    int16_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_MODULATION);
    tmp = (int16_t)gCmInfoDesc.cm_down.cm_modulation;
    return tmp;        
}

float cm_Get_DownChannelStrenth()
{
    int ret;
    float tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_STRENGTH);
    tmp = (float)gCmInfoDesc.cm_down.cm_signal_strength/10;
    return tmp;        
}

float cm_Get_DownChannelSNR()
{
    int ret;
    float tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_SNR);
    tmp = (float)gCmInfoDesc.cm_down.cm_snr/10;
    return tmp;        
}

int32_t cm_Get_DownChannelInterLeave()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_DOWN_INTER_LEAVE);
    tmp = (int32_t)gCmInfoDesc.cm_extend.DownInterleave;
    return tmp;        
}

STB_CM_info_desc_t * cm_Get_Status_Info()
{
    STB_CM_info_desc_t *pinfo = &gCmInfoDesc;
   
    int ret=0;
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_VALUE); 
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_CODE);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_RESET);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_LOST_SYNC);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_MAP);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_UCD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_RANG_RES);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_REG_RES);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T1_TIMEOUT);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T2_TIMEOUT);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T3_TIMEOUT);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T4_TIMEOUT);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_RANG_ABORT);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_DOCSIS_OPER_MODE);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_MUDO_TYPE);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_EQ_DATA);

    return pinfo;
}


int32_t cm_Get_StatusValue()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_VALUE);
    tmp = (int32_t)gCmInfoDesc.cm_status.CmStatValue;
    return tmp;        
}

void cm_Get_StatusCode(uint8_t * tmp)
{
    int ret;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_CODE);
    memmove(tmp, gCmInfoDesc.cm_status.CmStatCode,10);        
}

uint32_t cm_Get_StatusReset()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_RESET);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatResets;
    return tmp;        
}

uint32_t cm_Get_StatusLostSync()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_LOST_SYNC);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatLostSyncs;
    return tmp;        
}

uint32_t cm_Get_StatusInvalidMap()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_MAP);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatInvMaps;
    return tmp;        
}

uint32_t cm_Get_StatusInvalidUcd()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_UCD);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatInvUcds;
    return tmp;        
}

uint32_t cm_Get_StatusInvalidRangRes()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_RANG_RES);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatInvRangRes;
    return tmp;        
}

uint32_t cm_Get_StatusInvalidRegRes()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_INVALID_REG_RES);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatInvRegRes;
    return tmp;        
}

uint32_t cm_Get_StatusT1Timeout()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T1_TIMEOUT);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatT1Timeout;
    return tmp;        
}

uint32_t cm_Get_StatusT2Timeout()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T2_TIMEOUT);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatT2Timeout;
    return tmp;        
}

uint32_t cm_Get_StatusT3Timeout()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T3_TIMEOUT);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatT3Timeout;
    return tmp;        
}

uint32_t cm_Get_StatusT4Timeout()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_T4_TIMEOUT);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatT4Timeout;
    return tmp;        
}

uint32_t cm_Get_StatusRangAbort()
{
    int ret;
    uint32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_RANG_ABORT);
    tmp = (uint32_t)gCmInfoDesc.cm_status.CmStatRangAbr;
    return tmp;        
}

int32_t cm_Get_StatusDocsisOperMode()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_DOCSIS_OPER_MODE);
    tmp = (int32_t)gCmInfoDesc.cm_status.CmStatDocOperMode;
    return tmp;        
}

int32_t cm_Get_StatusModulationType()
{
    int ret;
    int32_t tmp;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_MUDO_TYPE);
    tmp = (int32_t)gCmInfoDesc.cm_status.CmStatModuType;
    return tmp;        
}

void cm_Get_StatusEqualizationData(uint8_t *tmp)
{
    int ret;
    
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    ret=cm_send_snmp_command(CM_CMD_GET_STAT_EQ_DATA);
    memmove(tmp, gCmInfoDesc.cm_status.CmStatusEqData,100);
    tmp[100] = '\0';
        
}


/*END: 获取Cable Modem 信息 add by whkuan */
#endif

#if  OS21_STB
#include "lwip/icmp.h"
#include "lwip/ip.h"
#include "lwip/opt.h"
#include "lwip/inet.h"
#include "lwip/stats.h"
#include "arch/lib.h"
#include "lwip/tcpip.h"
#include "lwip/sys.h"
#include "tcpip/sockets.h"
#include "tcpip/lwipsock.h"
#include "ares/ares_private.h"
#include "arch/sys_arch.h"
unsigned int cm_generate_snmp_requestid()
{
  /* struct timeval1 tv;
    unsigned int ticks;
    unsigned int ret;

    gettimeofday(&tv, NULL);
    ticks = tv.tv_sec + tv.tv_usec;
*/
 unsigned int ticks;
      unsigned int ret;

   ticks=time_now();
    srand(ticks);

    ret = (rand() % 31765) + 1000;

    return ret;
}

int cm_send_snmp_command(int command)
{
    int server_port;
    int ret;
    int len;
    int socketfd;
    int nbytes;
    fd_set rset;
    struct sockaddr_in remote_addr;
    struct sockaddr_in remote_addr1;
    struct timeval1 tmo;
    char recvbuf[128];
    unsigned int requestid;
    int retry_count=0;

    server_port = SNMP_PORT;

    //memset(&remote_addr, 0, sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = htonl(0xc0a86401);
    remote_addr.sin_port = htons(server_port);

    len = sizeof(remote_addr);

    socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketfd < 0)
    {
        //socket error
        return -1;
    }

    requestid = cm_generate_snmp_requestid();
#if CM_SNMP_DEBUG
    printf("[%s] %s -- request id is %x \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, requestid);
#endif

    retry_count = 0;
    while(retry_count < CM_CMD_RETRY_NUM)
    {
        /* stop or start cmd */
        if(command == CM_CMD_PASSWORD)
        {
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_password_pdu[CM_PASSWORD_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_password_pdu, sizeof(cm_snmp_password_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_password_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_password_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
        else if(command == CM_CMD_FACTORY)
        {
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_factory_pdu[CM_FACTORY_OFFSET] = (requestid & 0x000000ff);
            nbytes = sendto(socketfd, cm_snmp_factory_pdu, sizeof(cm_snmp_factory_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_factory_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_factory_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }
        else if(command==CM_CMD_DOWNLINKFREQUENCY_FOURBYTES)
        {
            cm_snmp_frequency_pdu4[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_frequency_pdu4[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_frequency_pdu4, sizeof(cm_snmp_frequency_pdu4), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DOWNLINKFREQUENCY_THREEBYTES)
        {
            // NOTE:need change the pdu data
            cm_snmp_frequency_pdu3[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_frequency_pdu3[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_frequency_pdu3, sizeof(cm_snmp_frequency_pdu3), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_DS_CHANNEL)
        {
            cm_snmp_Get_DSChannel[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_Get_DSChannel[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_Get_DSChannel, sizeof(cm_snmp_Get_DSChannel), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DIAGNOSTIC)
        {
            cm_snmp_diagnostic[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_diagnostic[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_diagnostic, sizeof(cm_snmp_diagnostic), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_PRIVATE_MIB_FACTORY)
        {
            cm_snmp_private_enable_factory[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_private_enable_factory[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_private_enable_factory, sizeof(cm_snmp_private_enable_factory), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_TUNNER_INDEX)
        {
            cm_snmp_tunerindex_zero[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_tunerindex_zero[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_tunerindex_zero, sizeof(cm_snmp_tunerindex_zero), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DS_OFFSET_ZERO)
        {
            cm_snmp_ds_offset_set_zero[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_ds_offset_set_zero[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_ds_offset_set_zero, sizeof(cm_snmp_ds_offset_set_zero), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_CAL_OFFSET_POWER)
        {
            cm_snmp_write_ds_offset[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_write_ds_offset[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_write_ds_offset, sizeof(cm_snmp_write_ds_offset), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_DISABLE_FACTORY)
        {
            cm_snmp_disable_factory[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_disable_factory[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_disable_factory, sizeof(cm_snmp_disable_factory), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_OPER_MODE)
        {
            cm_snmp_set_DOCSIS[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_set_DOCSIS[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_set_DOCSIS, sizeof(cm_snmp_set_DOCSIS), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_COMMIT_SETTING)
        {
            cm_snmp_commit_setting[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_commit_setting[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_commit_setting, sizeof(cm_snmp_commit_setting), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else if(command==CM_CMD_GET_OFFSET_SETTING)
        {
            cm_snmp_get_vertity_setting[CM_FREQUENCY_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_get_vertity_setting[CM_FREQUENCY_OFFSET] = (requestid & 0x000000ff);

            nbytes = sendto(socketfd, cm_snmp_get_vertity_setting, sizeof(cm_snmp_get_vertity_setting), 0,
                            (struct sockaddr *)&remote_addr, len);
        }
        else
        {
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET-1] = ((requestid>>8) & 0x000000ff);
            cm_snmp_scanning_pdu[CM_SCANNING_OFFSET] = (requestid & 0x000000ff);
            cm_snmp_scanning_pdu[sizeof(cm_snmp_scanning_pdu)-1] = command;
            nbytes = sendto(socketfd, cm_snmp_scanning_pdu, sizeof(cm_snmp_scanning_pdu), 0,
                            (struct sockaddr *)&remote_addr, len);

#if 0
            {
                int dgi;
                printf("[%s] %s -- send data : \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__);
                for(dgi=0; dgi < sizeof(cm_snmp_scanning_pdu); dgi++)
                {
                    printf("%x\t", cm_snmp_scanning_pdu[dgi]);
                    if((dgi != 0) && (dgi %16 == 0))
                    {
                        printf("\n");
                    }
                }
                printf("\n");
            }
#endif

        }

        if(nbytes <=0)
        {
            //send error;
#if CM_SNMP_DEBUG
            printf("[%s] %s -- send data return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }

        tmo.tv_sec = 2;
        // tmo.tv_usec = 500*1000; /* 100 m-seconds */
        tmo.tv_usec = 0;

        FD_ZERO(&rset);
        FD_SET(socketfd, &rset);

        ret = 0;
        ret = select(socketfd+1, &rset, NULL, NULL, &tmo);

        if(ret < 0)
        {
            //recv error or timeout
#if CM_SNMP_DEBUG
            printf("[%s] %s -- select return error [ret = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, ret);
#endif
            close(socketfd);
            return -1;
        }
        else if(ret == 0)
        {
            retry_count++;
            continue;
        }

        nbytes = recvfrom(socketfd, recvbuf, 128, 0,
                          (struct sockaddr *)&remote_addr1, &len);

        if(nbytes <= 0)
        {
            //recv error or no data
#if CM_SNMP_DEBUG
            printf("[%s] %s -- recv return error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
            close(socketfd);
            return -1;
        }
        else
        {
            //calculate response id
            if(((recvbuf[CM_PASSWORD_OFFSET-1] & 0x000000ff) == ((requestid >> 8) & 0x000000ff)) &&
               ((recvbuf[CM_PASSWORD_OFFSET] & 0x000000ff)== (requestid & 0x000000ff)))
            {
                if(command==CM_CMD_GET_DS_CHANNEL)
                {
                    if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x36)
                    {
                        int i=0;
                        return_DS_Channel=0x00000000;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        int j=return_DS_Channel;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                    }
                    else if((recvbuf[CM_LENGHT_OFFSET] & 0x000000ff)==0x35)
                    {
                        int i=0;
                        return_DS_Channel=0x00000000;

                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        int j=return_DS_Channel;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);
                        return_DS_Channel=return_DS_Channel<<8;
                        i++;
                        memcpy(&return_DS_Channel,&recvbuf[CM_DSCHANNEL_OFFSET+i] ,1);

                    }
                }
                else if(command==CM_CMD_GET_OFFSET_SETTING)
                {
                    int i=0;
                    cm_return_DS_Offset=0x00000000;
                    memcpy(&cm_return_DS_Offset,&recvbuf[CM_DSCHANNEL_OFFSET+i],1);
                    i++;
                    cm_return_DS_Offset<<8;
                    memcpy(&cm_return_DS_Offset,&recvbuf[CM_DSCHANNEL_OFFSET+i],1);
                }
                //it's ok?
                break;
            }
            else
            {
                //data error
#if CM_SNMP_DEBUG
                printf("[%s] %s -- recv data error [nbytes = %d] \n", CM_SNMP_DEBUG_PREFIX, __FUNCTION__, nbytes);
#endif
                retry_count++;
                continue;
            }
        }
    }

    //it's ok?
    if(retry_count >= CM_CMD_RETRY_NUM)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    close(socketfd);

    return ret;

}

int cm_stop_scanning()
{
    int ret;
    ret = cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret < 0)
        return ret;

    ret = cm_send_snmp_command(CM_CMD_FACTORY);
    if(ret < 0)
        return ret;

    return cm_send_snmp_command(CM_CMD_STOP_SCANNING);
}

int cm_restart_scanning()
{
    int ret;
    ret = cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret < 0)
        return ret;

    ret = cm_send_snmp_command(CM_CMD_FACTORY);
    if(ret < 0)
        return ret;

    return cm_send_snmp_command(CM_CMD_START_SCANNING);
}

int cm_set_DownlinkFrequency(int Bytes)
{
    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
    sys_thread_add(pthread_id);
    int ret ;

    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
        return ret;

    /*
        ret=cm_send_snmp_command(CM_CMD_FACTORY);
        if(ret<0)
            return ret;
    */
    if(Bytes==4)
    {
        ret= cm_send_snmp_command(CM_CMD_DOWNLINKFREQUENCY_FOURBYTES);
    }
    else
    {
        ret= cm_send_snmp_command(CM_CMD_DOWNLINKFREQUENCY_THREEBYTES);
    }

    sys_thread_del(pthread_id);
    return ret;

}

#define ERROR  -1
#define RIGHT  0
int cm_set_DS_Channel(int DSFrequency)
{
    int ret=0;
    //首先判断下行频率值是否是正常范围:1MHZ~1000MHZ(1 00 0000~1000 00 0000)
    if(DSFrequency<1000000||DSFrequency>1000000000)
    {
        return ERROR;
    }
    if(DSFrequency<=16000000)
    {
        int i=2;

        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        DSFrequency=DSFrequency>>8;
        int j=DSFrequency;
        i--;
        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu3+50+i,&DSFrequency,1);
        ret=cm_set_DownlinkFrequency(3);
    }
    else
    {
        int i=3;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency=DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);

        DSFrequency= DSFrequency>>8;
        i--;
        memcpy(cm_snmp_frequency_pdu4+50+i,&DSFrequency,1);
        ret=cm_set_DownlinkFrequency(4);
    }
    return ret;


}

int cm_get_DS_Channel()
{
    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
    sys_thread_add(pthread_id);
    int ret ;
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
		{
		 sys_thread_del(pthread_id);
		return ret;
	}

    ret= cm_send_snmp_command(CM_CMD_GET_DS_CHANNEL);

    sys_thread_del(pthread_id);

    return return_DS_Channel;
    // return ret;
}


/*--------------------------------used for CM Calibration----------------------------------------*/
/*
int cm_Configure_Selected_Frequency(int DSFrequency)
{
    int ret=0;
    int i=3;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency=DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency=DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);

    DSFrequency= DSFrequency>>8;
    i--;
    memcpy(cm_snmp_frequency+50+i,&DSFrequency,1);
    ret=cm_send_snmp_command(CM_CMD_CONFIGURE_SELECTED_FREQUENCY)
        return ret;


}
*/
int cm_Downstream_Calibration()
{
    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
    sys_thread_add(pthread_id);
    int ret ;

    /*校准之前先要停止Cable Modem 的扫描，不让其上线，然后才能进行相应的操作
        最后完成后要把Cable Modem 的扫描开启*/
    ret=cm_stop_scanning();
    /*1*/
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
    {
        sys_thread_del(pthread_id);
        return ret;
    }
    //Err_Print("PassWord 发送正确\n");

    ret=cm_send_snmp_command(CM_CMD_DIAGNOSTIC);
    ret=cm_send_snmp_command(CM_CMD_PRIVATE_MIB_FACTORY);

    /*2、对每一个downstream tunner 都需要进行，3349 只支持一个*/
    ret=cm_send_snmp_command(CM_CMD_TUNNER_INDEX);
    /*3*/
    ret=cm_send_snmp_command(CM_CMD_DS_OFFSET_ZERO);

#if 0
    /*4*/
    int i=0;
    int dsfrequency=93000000;
    for(i=0; i<14; i++)
    {
        /*5*/
        ret= cm_Configure_Selected_Frequency(dsfrequency+i*60000000);
        ret=cm_send_snmp_command(CM_CMD_CAL_MODULATION);
        /*6*/
        ret=cm_send_snmp_command(CM_CMD_LOCK_NOW);
        task_delay(time_ticks_per_sec()*5); /* delay 5 s */
        /*7*/
        int VerityQam=0;
        VerityQam=cm_send_snmp_command(CM_CMD_CAL_QAM_LOCKED);
        int VeritySNR=0;
        downstream_power_index=i;
        VeritySNR=cm_send_snmp_command(CM_CMD_SNR);
        /*对获得的值进行操作，上述两个均有数据返回，注意进行相应的判断，然后再往下继续进行*/
        /*8*/
        int downstream_power=0;
        downstream_power=cm_send_snmp_command(CM_CMD_DOWNCHANNEL_POWER);
        /*9*/
        /*10*/

    }
#endif
    /*11*/
    int j=0;
    for(j=1; j<=14; j++)
    {
        //需要修改14个频点的OFFSET，依靠修改MIB树下节点的序号即可，Note the Number is started by 1,not zero(0)
        //避免构造大量的数据包
        memcpy(cm_snmp_write_ds_offset+49,&j,1);//改变数据包所指向CM 对象的序列号
	memcpy(cm_snmp_write_ds_offset+52,&store_downstream_power[j*2-2],2);//改变对应的OFFSET 值
        ret=cm_send_snmp_command(CM_CMD_CAL_OFFSET_POWER);
    }
    /*12*/
    ret=cm_send_snmp_command(CM_CMD_DISABLE_FACTORY);
    ret=cm_send_snmp_command(CM_CMD_OPER_MODE);
    /*13*/
    ret=cm_send_snmp_command(CM_CMD_COMMIT_SETTING);

     ret=cm_restart_scanning(); 
sys_thread_del(pthread_id);

    return ret;

}



int cm_Get_DownStream_Offset()
{

    task_t *pthread;
    u32_t pthread_id;
    pthread = task_id();
    pthread_id = (u32_t)pthread;
    /* add current thread to lwip */
    //sys_sem_wait(sys_thread_sem);
    sys_thread_add(pthread_id);
    int ret ;
    ret=cm_send_snmp_command(CM_CMD_PASSWORD);
    if(ret<0)
    {
        sys_thread_del(pthread_id);
    }
    ret=cm_send_snmp_command(CM_CMD_GET_OFFSET_SETTING);
    sys_thread_del(pthread_id);

    return cm_return_DS_Offset;
}

#if 0
int main(int argc, char *argv[])
{
    //test
    cm_stop_scanning();
    cm_restart_scanning();
    return 1;
}
#endif

#endif
