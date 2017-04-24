#ifndef _DVTCAS_STBDataPublic_H_2004_12_31_
#define _DVTCAS_STBDataPublic_H_2004_12_31_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef  __cplusplus
extern "C"
{
#endif

        /*��DVTCA_VERSION_NEW���������¡��ϼ��ɵĻ����С���������Ѷ��Ա���ơ�2009-12-02
          �¼��ɵ�SDVTCAIpp��SDVTCAViewedIpp�ṹ�ж��˳�Ա������DVTSTBCA_ShowOSDMsg�����б仯.
        */
#define DVTCA_VERSION_NEW       //������¼��ɵĻ�����,��Ҫ���壻�����ԭ�����ɵĻ����У�ע�͵�����??????????????????????

        /*��DVTCA_SECURITYCHIP���������ǲ��ǿ�ͨ�˰�ȫ���ܵİ�ȫоƬ.��������Ѷ��Ա���ơ�2009-12-14
        */
#define DVTCA_SECURITYCHIP   (BYTE)0    //1:��ȫоƬ,���Ҵ��˰�ȫ����; 0:�ǰ�ȫоƬ�����ǰ�ȫоƬ��û��оƬ�İ�ȫ����???????????

#ifndef BYTE //8bit
#define BYTE unsigned char
#endif
#ifndef WORD //16bit
#define WORD unsigned short
#endif
#ifndef HRESULT
#define HRESULT long
#endif
#ifndef DWORD
#define DWORD unsigned long
#endif
#ifndef bool //8bit
#define bool unsigned char
#endif
#ifndef NULL
#define NULL 0
#endif
#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif

//�ź�������
        typedef long  DVTCA_Semaphore ;

//�̺߳�����ַ����
        typedef void (*pThreadFunc) (void);

        /*----------------------------------------------------------------------*/
        /*                       �����ǳ���ֵ����                               */
        /*----------------------------------------------------------------------*/

#define DVTCA_MAXLEN_PINCODE                    8       //���PIN�볤��
#define DVTCA_LEN_SMARTCARD_SN                  64      //�ֻ�ܿ����кų��ȣ��ⲿ���ţ�

#define DVTCA_MAXLEN_SERVICEPERECM              10      //ÿ��ECM֧�ֵ����Service��Ŀ

#define DVTCA_MAXLEN_TVSNAME                    21      //���Ӫ�����Ƴ���+\0
#define DVTCA_MAXLEN_MANUFACTURERNAME           21      //���CAS�������ֳ���+\0 
#define DVTCA_MAXLEN_PRODUCT_NAME               21      //��Ĳ�Ʒ����,���20���ַ�+\0
#define DVTCA_MAXLEN_SERVICE_NAME               21      //���ҵ������,���20���ַ�+\0
#define DVTCA_MAXLEN_EMAIL_SENDERNAME           11      //E-mail���������������11���ַ�+\0
#define DVTCA_MAXLEN_EMAIL_CONTENT              1025    //E-mail��󳤶ȣ����1024���ַ�+\0
#define DVTCA_MAXLEN_EMAIL_SHORT_CONTENT        21      //E-mail���ⳤ�ȣ����20���ַ�+\0

#define DVTCA_MAXNUMBER_TVSID                   5       //֧�ֵ���Ӫ��������

#define DVTCA_MAX_IPP_COUNT                     250     //����IPP��Ʒ�������ݶ�Ϊ250

//������ȡ˽�����ݵ������
#define DVTCA_STB_ECM_SLOT                      1
#define DVTCA_STB_EMM_Private_SLOT              2
#define DVTCA_STB_EMM_Public_SLOT               3
#define DVTCA_STB_EMM_Group_SLOT                4  //for Group Address 

//���ܿ�״̬����
#define DVTCAS_SC_OUT                           0       //���ܿ����ڶ�������
#define DVTCAS_SC_INIT                          1       //���ܿ��ڶ������У����ڳ�ʼ��
#define DVTCAS_SC_IN                            2       //���ܿ��ڶ������У���������

//�������״̬����
#define DVTCAS_SC_PAIR_OK                       0       //���ܿ��뵱ǰ���������
#define DVTCAS_SC_NO_Pairing                    1       //���ܿ�û�����κλ��������
#define DVTCAS_SC_PAIR_OTHER                    2       //���ܿ��뵱ǰ������û�����

//�տ�������
#define DVTCAS_Rating_3                         3       //3��
#define DVTCAS_Rating_4                         4       //4��
#define DVTCAS_Rating_5                         5       //5��
#define DVTCAS_Rating_6                         6       //6��
#define DVTCAS_Rating_7                         7       //7��
#define DVTCAS_Rating_8                         8       //8��
#define DVTCAS_Rating_9                         9       //9��
#define DVTCAS_Rating_10                        10      //10��
#define DVTCAS_Rating_11                        11      //11��
#define DVTCAS_Rating_12                        12      //12��
#define DVTCAS_Rating_13                        13      //13��
#define DVTCAS_Rating_14                        14      //14��
#define DVTCAS_Rating_15                        15      //15��
#define DVTCAS_Rating_16                        16      //16��
#define DVTCAS_Rating_17                        17      //17��
#define DVTCAS_Rating_18                        18      //18��

//email ��ʾ��Ϣ��
#define DVTCAS_EMAIL_NEW                        0       //���ʼ�֪ͨ
#define DVTCAS_EMAIL_NEW_NO_ROOM                1       //�����ʼ�����Email�ռ䲻��
#define DVTCAS_EMAIL_NONE                       0xFF    //���ر�־

//email ״̬��
#define DVTCAS_EMAIL_STATUS_INI                 0       //��ʼֵ
#define DVTCAS_EMAIL_STATUS_READ                2       //��������

//�۸����Ͷ��塣
#define TPP_TAP_PRICE                           0       //���ش���¼��
#define TPP_NOTAP_PRICE                         1       //���ش�����¼��
#define CPP_TAP_PRICE                           2       //�ش���¼��
#define CPP_NOTAP_PRICE                         3       //�ش�����¼��


        /*----------------------------------------------------------------------*/
        /*                       �����Ƿ���ֵ����                               */
        /*----------------------------------------------------------------------*/

//�����з���ֵ��ֵ����
#define DVTCAERR_BASE_STB                       0xF1000000

//����ֵ����
#define DVTCA_OK                                0

#define DVTCAERR_STB_POINTER_INVALID            (DVTCAERR_BASE_STB + 1)     //�����ָ����Ч
#define DVTCAERR_STB_DATA_LEN_ERROR             (DVTCAERR_BASE_STB + 2)     //��������ݳ��Ȳ��Ϸ�
#define DVTCAERR_STB_TVS_NOT_FOUND              (DVTCAERR_BASE_STB + 3)     //û���ҵ�����Ҫ�����Ӫ��
#define DVTCAERR_STB_EXCEED_MAX_COUNT           (DVTCAERR_BASE_STB + 4)     //��������������ĸ���
#define DVTCAERR_STB_KEY_NOT_FOUND              (DVTCAERR_BASE_STB + 5)     //û���ҵ�������Կ
#define DVTCAERR_STB_PIN_INVALID                (DVTCAERR_BASE_STB + 6)     //�����PIN����Ч
#define DVTCAERR_STB_PIN_LOCKED                 (DVTCAERR_BASE_STB + 7)     //PIN�뱻����
#define DVTCAERR_STB_CMD_INVALID                (DVTCAERR_BASE_STB + 8)     //MACУ�����
#define DVTCAERR_STB_VER_EXPIRED                (DVTCAERR_BASE_STB + 9)     //����İ汾û��IC���еĴ󣬼������Ѿ�����
#define DVTCAERR_STB_EXCEED_MAX_LEN             (DVTCAERR_BASE_STB + 10)    //��Ӫ�̵����Ƴ������ĳ���
#define DVTCAERR_STB_NO_SPACE                   (DVTCAERR_BASE_STB + 11)    //�Ѿ�û�пռ�
//#define DVTCAERR_STB_IS_PAIRED                    (DVTCAERR_BASE_STB + 12)    //���ܿ��뵱ǰ�����ж�Ӧ
#define DVTCAERR_STB_NOT_PAIRED                 (DVTCAERR_BASE_STB + 13)    //���ܿ��뵱ǰ������û�ж�Ӧ
//#define DVTCAERR_STB_COUNT_ISZERO             (DVTCAERR_BASE_STB + 14)    //�����CountΪ0.
#define DVTCAERR_STB_WORKTIME_INVALID           (DVTCAERR_BASE_STB + 15)    //����ʱ�����ò��Ϸ���������ʱ�䲻��00:00:00~23:59:59֮�䣬���߿�ʼʱ����ڽ���ʱ�䡣
//#define   DVTCAERR_STB_OVERFLOW                   (DVTCAERR_BASE_STB + 16)
#define DVTCAERR_STB_CARD_INVALID               (DVTCAERR_BASE_STB + 17)    //���ܿ���Ч
#define DVTCAERR_STB_Rating_LOW                 (DVTCAERR_BASE_STB + 18)    //���ܿ����õ��տ�����ȵ�ǰ�տ��Ľ�Ŀ��
#define DVTCAERR_STB_UNKNOWN                    (DVTCAERR_BASE_STB + 19)    //δ֪����STB�����κ���ʾ����ͨ������λ�ָ���

//#define   DVTCAERR_STB_EMAILBOX_EMPTY             (DVTCAERR_BASE_STB + 20)    //E-mail����Ϊ��
#define DVTCAERR_STB_NO_EMAIL                   (DVTCAERR_BASE_STB + 21)    //û�д�E-mail
#define DVTCAERR_STB_EMAILBOX_FULL              (DVTCAERR_BASE_STB + 22)    //E-mail������
//#define   DVTCAERR_STB_NO_SECTION                 (DVTCAERR_BASE_STB + 23)    //û���յ�����Section

#define DVTCAERR_STB_MONEY_LACK                 (DVTCAERR_BASE_STB + 24)    //���ֻ࣬��Ԥ��ʱ�᷵�ظô�����Ҫ��ʾ�û�"Ԥ���ɹ��������࣬�뼰ʱ��ֵ"��
#define DVTCAERR_STB_PROD_NOT_FOUND             (DVTCAERR_BASE_STB + 25)    //��Ʒδ�ҵ�
#define DVTCAERR_STB_OPER_INVALID               (DVTCAERR_BASE_STB + 26)    //��Ʒ�Ѵ��ڣ����ܲ���
#define DVTCAERR_STB_NEED_PIN_PASS              (DVTCAERR_BASE_STB + 27)    //��ҪPIN��֤ͨ��
#define DVTCAERR_STB_IC_COMMUNICATE             (DVTCAERR_BASE_STB + 28)    //��IC��ͨѶ����
#define DVTCAERR_STB_SLOT_NOT_FOUND             (DVTCAERR_BASE_STB + 29)    //��Ӫ��Ǯ��û�з���
#define DVTCAERR_STB_PRICE_INVALID              (DVTCAERR_BASE_STB + 30)    //�۸���Ч
#define DVTCAERR_STB_PRODUCT_EXPIRED            (DVTCAERR_BASE_STB + 31)    //��Ʒ�Ѿ�����

//xb:20050617 ++
#define DVTCAERR_STB_EXPIRED                    (DVTCAERR_BASE_STB + 33)    //��Ʒ�Ѿ�����
#define DVTCAERR_IPPREC_NOT_FOUND               (DVTCAERR_BASE_STB + 34)    //IPP��¼������
#define DVTCAERR_TIME_INVALID                   (DVTCAERR_BASE_STB + 35)    //��ǰʱ����Ч
#define DVTCAERR_NOT_BOOKED                     (DVTCAERR_BASE_STB + 37)    //��ƷδԤ��
#define DVTCAERR_CONFIRMED                      (DVTCAERR_BASE_STB + 38)    //��Ʒ�Ѿ�ȷ��
#define DVTCAERR_UNCONFIRMED                    (DVTCAERR_BASE_STB + 39)    //��Ʒ�Ѿ�ȡ��ȷ��
#define DVTCAERR_INPUT_DATA_INVALID             (DVTCAERR_BASE_STB + 40)    //�����������Ч

//xb:20050617 --

//������ң��������ȡ������ʾ��Ϣ�б�
#define DVTCA_RATING_TOO_LOW                    0           //�տ����𲻹�
#define DVTCA_NOT_IN_WATCH_TIME                 1           //�����տ�ʱ����
#define DVTCA_NOT_PAIRED                        2           //û�л�����Ӧ
#define DVTCA_PLEASE_INSERT_CARD                4           //��忨
#define DVTCA_NO_ENTITLE                        5           //û�й���˽�Ŀ
#define DVTCA_PRODUCT_RESTRICT                  6           //��Ӫ�����ƹۿ��ý�Ŀ
#define DVTCA_AREA_RESTRICT                     7           //��Ӫ����������ۿ�
//V2.1��������ʾ��Ϣ
#define DVTCA_MOTHER_RESTRICT                   8           //�˿�Ϊ�ӿ����Ѿ��������տ�������ĸ�����
#define DVTCA_NO_MONEY                          9           //���㣬���ܹۿ��˽�Ŀ���뼰ʱ��ֵ
#define DVTCA_IPPV_NO_CONFIRM                   10          //�˽�ĿΪIPPV��Ŀ���뵽IPPV��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define DVTCA_IPPV_NO_BOOK                      11          //�˽�ĿΪIPPV��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
#define DVTCA_IPPT_NO_CONFIRM                   12          //�˽�ĿΪIPPT��Ŀ���뵽IPPT��Ŀȷ��/ȡ������˵���ȷ�Ϲ���˽�Ŀ
#define DVTCA_IPPT_NO_BOOK                      13          //�˽�ĿΪIPPT��Ŀ����û��Ԥ����ȷ�Ϲ��򣬲��ܹۿ��˽�Ŀ
//xb:20050617
#define DVTCA_DATA_INVALID                      16          //������Ч��STB�����κ���ʾ������Կ���⡣
#define DVTCA_SC_NOT_SERVER                     18          //IC������ֹ����
#define DVTCA_KEY_NOT_FOUND                     20          //�˿�δ���������ϵ��Ӫ��
#define DVTCA_IPPNEED_CALLBACK                  21          //����ϵ��Ӫ�̻ش�IPP��Ŀ��Ϣ
#define DVTCA_FREE_PREVIEWING                   22          //�û����ã��˽�Ŀ����δ�����������Ԥ����
//wy:20071022���û��Զ�����������
#define DVTCA_BOUQUETID                         254         //�Զ�����������-Bouquet_id

//kfd:2007-11-16---�ַ�����������
#define DVTCA_LANG_CHN_SIM                      1           //��������(Ĭ��)
#define DVTCA_LANG_ENG                          2           //Ӣ��

        /*----------------------------------------------------------------------*/
        /*                       ������һЩ���ݽṹ����                         */
        /*----------------------------------------------------------------------*/

        typedef struct _SDVTCAPin
        {
                BYTE    m_byLen;
                BYTE    m_byszPin[DVTCA_MAXLEN_PINCODE];
        } SDVTCAPin;

        typedef struct _SDVTCAManuInfo
        {
                DWORD   m_dwCardID;                                         //���ڲ�ID
                BYTE    m_byszSerialNO[DVTCA_LEN_SMARTCARD_SN];             //�ⲿ����
                DWORD   m_dwSTBCASVer;                                      //������CASģ��İ汾��
                char    m_szSCCASManuName[DVTCA_MAXLEN_MANUFACTURERNAME];   //CAS��Ӧ�̵�����
                DWORD   m_dwSCCASVer;                                       //���ܿ�CAS�İ汾��
                DWORD   m_dwSCCOSVer;                                       //SC�İ汾�ţ�Ҳ����SC��COS�İ汾��
                DWORD   m_tSCExpireDate;                                    //���ܿ�����Ч�ڣ���STB��ʼ��ʱ�жϣ�������ڣ����û�һ����ʾ�������Լ���ʹ�á�time_tʱ���ʽ��
        } SDVTCAManuInfo;

        typedef struct _SDVTCATvsInfo
        {
                WORD    m_wTVSID;                                           //��Ӫ�̵��ⲿ���
                char    m_szTVSName[DVTCA_MAXLEN_TVSNAME];                  //��Ӫ������
        } SDVTCATvsInfo;

        typedef struct _SDVTCAServiceEntitle
        {
                WORD    m_wProductID;                                       //��ƷID
                DWORD   m_tEntitleTime;                                     //��Ȩʱ�䣬time_t��ʽ��
                DWORD   m_tStartTime;                                       //��ʼʱ�䣬time_t��ʽ��
                DWORD   m_tEndTime;                                         //����ʱ�䣬time_t��ʽ��
                char    m_szProductName[DVTCA_MAXLEN_PRODUCT_NAME];         //��Ʒ����
                bool    m_bTapingFlag;                                      //¼���ʶ��
        } SDVTCAServiceEntitle;                                         //��ͨ��Ʒ��Ȩ

        typedef struct _SDVTCAServiceInfo
        {
                WORD    m_wEcmPid;                                          //���ŵ�ECMPID
                WORD    m_wServiceID;                                       //����Ƶ����ServiceID
        } SDVTCAServiceInfo;

        typedef struct _SDVTCAEmailInfo
        {
                DWORD   m_dwVersion;                                        //����ʱ��
                char    m_szSenderName[DVTCA_MAXLEN_EMAIL_SENDERNAME];      //����������
                BYTE    m_Status;                                           //�ʼ���״̬��ΪDVTCAS_EMAIL_STATUS_���е�һ��
                WORD    m_wEmailLength;                                     //�ʼ����ݳ���
                char    m_szTitle[DVTCA_MAXLEN_EMAIL_SHORT_CONTENT];        //�ʼ�������
        } SDVTCAEmailInfo;

        typedef struct _SDVTCAEmailContent
        {
                DWORD   m_dwVersion;                                        //����ʱ��
                WORD    m_wEmailLength;                                     //�ʼ����ݳ���
                char    m_szEmail[DVTCA_MAXLEN_EMAIL_CONTENT];              //�ʼ�����
        } SDVTCAEmailContent;

        typedef struct _SDVTCAIpp
        {
                WORD    m_wTVSID;                   //��Ӫ�̱��
                WORD    m_wProdID;                  //��ƷID
                BYTE    m_bySlotID;                 //Ǯ��ID
                char    m_szProdName[DVTCA_MAXLEN_PRODUCT_NAME];    //��Ʒ����
                DWORD   m_tStartTime;               //��ʼʱ�䣬time_t��ʽ��
                DWORD   m_dwDuration;               //��������
                char    m_szServiceName[DVTCA_MAXLEN_SERVICE_NAME]; //ҵ������
                WORD    m_wCurTppTapPrice;          //��ǰ�Ĳ��ش�����¼��۸�(��)���۸�����ֵΪ0
                WORD    m_wCurTppNoTapPrice;        //��ǰ�Ĳ��ش�������¼��۸�(��)���۸�����ֵΪ1
                WORD    m_wCurCppTapPrice;          //��ǰ��Ҫ�ش�����¼��۸�(��)���۸�����ֵΪ2
                WORD    m_wCurCppNoTapPrice;        //��ǰ��Ҫ�ش�������¼��۸�(��)���۸�����ֵΪ3
                WORD    m_wBookedPrice;         //�Ѿ�Ԥ���ļ۸�(��)
                BYTE    m_byBookedPriceType;        //�Ѿ�Ԥ���ļ۸����ͣ�ȡֵ��Χ0~3
                BYTE    m_byBookedInterval;     //Ԥ���շѼ��
                BYTE    m_byCurInterval;            //��ǰ�շѼ��
                BYTE    m_byIppStatus;              //Ipp��Ʒ״̬
#ifdef DVTCA_VERSION_NEW
                BYTE    m_byUnit;                   //�շѼ���ĵ�λ0 -����1-Сʱ2-��3-��4-��
                WORD    m_wIpptPeriod;          //�û�����IPPT�Ĺۿ�������,for Philippines LongIPPT��
#endif
        } SDVTCAIpp;

        typedef struct _SDVTCAViewedIpp
        {
                WORD    m_wTVSID;                                           //��Ӫ�̱��
                char    m_szProdName[DVTCA_MAXLEN_PRODUCT_NAME];        //��Ʒ����
                DWORD   m_tStartTime;                       //��ʼʱ�䣬time_t��ʽ��
                DWORD   m_dwDuration;                       //��������
                WORD    m_wBookedPrice;                     //Ԥ���۸�(��)
                BYTE    m_byBookedPriceType;                    //Ԥ���۸����ͣ�0:TppTap;1:TppNoTap;2:CppTap;3:CppNoTap;
                BYTE    m_byBookedInterval;                 //Ԥ���շѼ��
                char    m_szOtherInfo[44];                  //ippvʱΪ���˲�ƷΪippv��Ʒ����ipptʱΪ���ۿ���ʱ�䣺�����ӣ���Ǯ���������֡�
#ifdef DVTCA_VERSION_NEW
                BYTE    m_byUnit;                       //�շѵ�λ��0 -����1-Сʱ2-��3-��4-��
#endif
        } SDVTCAViewedIpp;

        typedef struct _SDVTCAAreaInfo
        {
                DWORD       m_dwCardArea;           //����������
                DWORD       m_tSetCardAreaTime;     //�趨����ʱ�䡣
                BYTE        m_byStartFlag;          //������־��1:������0:û�С�
                DWORD       m_tSetFlagTime;         //���ñ�־ʱ�䡣
                DWORD       m_tSetStreamTime;       //��������������ʱ�䡣(�յ���������ʱ��)
                DWORD       m_dwIntervalTime;       //���е�ǰʱ���ȥ������ʱ������ֵ��
                DWORD       m_dwStreamMinArea;      //��������С������
                DWORD       m_dwStreamMaxArea;      //���������������
        } SDVTCAAreaInfo;

#ifdef  __cplusplus
}
#endif

#endif
