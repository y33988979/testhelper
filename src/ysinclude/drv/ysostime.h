/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : 

    Description : 

    Others:      

    History:     
    ======================================================================
    ʱ    �䣺2004-03-12
    ��    �ߣ�Cao Songtao
    ˵    ����1. ����
    ======================================================================
    ʱ    �䣺2004-03-01
    ��    �ߣ�XXX
    ˵    ����1. �������
              2. �޸����ݼ���
              3. ����

*******************************************************************************/
#ifndef _YSOSTIME_H_
#define _YSOSTIME_H_

/* C++ support */
/* ----------- */
#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 *	Includes
 *----------------------------------------------------------------------------*/
#include "ys_typedef.h"
#include "drv_os.h"
/*----------------------------------------------------------------------------
 *	variables
 *----------------------------------------------------------------------------*/


#define ST20_CLOCKS_PER_SEC_LOW  DRV_OS_TimeClockPerSecond()

#define OS_BASETIME    0     /* 01-01-1900 is base time */
#define OS_BASEYEAR    1900  /* base time subtract one is the logic basetime */
#define OS_BASEMONTH   0     /* so 01-01-1900 - 1 = 00-00-1900*/
#define OS_BASEDAY     0
#define OS_BASEHOUR    0
#define OS_BASEMINUTE  0
#define OS_BASESECOND  0

#define OS_TIMEZONE  28800  /* China Time Zone */ //8 * 3600
#define OS_DAYSEC           86400   /* secs in a day */ //(24 * 60 * 60) 
#define OS_YEARSEC          31536000   /* secs in a year */ //(365 * OS_DAYSEC) 
#define OS_FOUROS_YEARSEC    126230400  /* secs in a 4 year interval */  //(1461 * OS_DAYSEC) 
#define OS_BASEDOW         4          /* 01-01-1970 was a Thursday */
#define OS_MAXYEAR         138        /* 2038 is the max year */

/*----------------------------------------------------------------------------
 *	Structures & Enums
 *----------------------------------------------------------------------------*/
typedef struct StbTm
{
    short Second;     /* seconds after the minute - [0,59] */
    short Minute;     /* minutes after the hour - [0,59]   */
    short Hour;       /* hours since midnight - [0,23]     */
    short Day;        /* day of the month - [1,31]         */
    short Month;      /* months since January - [0,11]     */
    short Year;       /* years since 1900                  */
    short DayOfWeek;  /* days since Sunday - [0,6]         */
    short DayOfYear;  /* days since January 1 - [0,365]    */
    short IsDst;      /* daylight savings time flag        */
} StbTm_t;

typedef  int StbTime_t;

typedef struct StbTmSpan
{
    int Days;
    short Hours;
    short Minutes;
    short Seconds;
    int TotalHours;
    int TotalMinutes;
    int TotalSeconds;
} StbTmSpan_t;

typedef struct StbOldDate
{
    unsigned short usYear;    /* years since 1900 */
    unsigned char  ucMonth;   /* months since January - [1,12] */
    unsigned char  ucDay;     /* day of the month - [1,31] */
} StbOldDate_t;

typedef struct StbOldTime
{
    unsigned char   ucHour;    /* hours since midnight - [0,23]     */
    unsigned char   ucMinute;  /* minutes after the hour - [0,59]   */
    unsigned char   ucSecond;  /* seconds after the minute - [0,59] */
} StbOldTime_t;
/*----------------------------------------------------------------------------
 *	Global functions
 *----------------------------------------------------------------------------*/  
unsigned char ConvBCD(unsigned char ucBCDCode);
StbTm_t *GetSTBTmFromMJDUTC(const unsigned char *ucMJDUTC);
unsigned short GetMJD(const unsigned char *ucMJDUTC);
void GetStbTime(const unsigned char *ucMJDUTC, StbOldTime_t *pStbTime);
void GetStbDate(const unsigned char *ucMJDUTC, StbOldDate_t *pStbDate);
S32_YS IsLeapYear(unsigned short usYear);
void STBTmCopy(StbTm_t *pStbTmDest, StbTm_t *pStbTmSrc);

S32_YS GetTotalHours(StbTmSpan_t *pStbTmSpan);
S32_YS GetTotalMinutes(StbTmSpan_t *pStbTmSpan);
S32_YS GetTotalSeconds(StbTmSpan_t *pStbTmSpan);

/*******************************************************************************
  Function:      STBGmTime 
  Description:   ��StbTime_tת����StbTm_t�ṹ(Gmtʱ��)
  Calls:          
  Called By:      
  Input:         const StbTime_t *pTimer - ָ��Ҫ��ת����StbTime_t
  Output:         
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ
*******************************************************************************/
StbTm_t *STBGmTime(const StbTime_t *pTimer);
/*******************************************************************************
  Function:       STBMkTime
  Description:    ��StbTime_t�ṹת����StbTm_t������StbTm_t�е�ֵ"�淶��"
  Called By:      
  Input:          StbTm_t *pStbTm  - ָ��Ҫ��ת����StbTm_t
  Output:         
  Return:         StbTime_t
  Others:         
*******************************************************************************/
StbTime_t STBMkTime(StbTm_t* pStbTm);
/*******************************************************************************
  Function:      STBLocalTime 
  Description:   ��StbTime_tת����StbTm_t�ṹ(����ʱ��)
  Calls:          
  Called By:      
  Input:         const StbTime_t *pTimer - ָ��Ҫ��ת����StbTime_t
  Output:         
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ
*******************************************************************************/
StbTm_t *STBLocalTime(const StbTime_t *pTimer);
/*******************************************************************************
  Function:       GetSTBTimeFromMJDUTC
  Description:    ��MJD+UTC�����ʽ��ʱ��ת��ΪStbTime_t
  Calls:          GetSTBTmFromMJDUTC
  Called By:      
  Input:          unsigned char *ucMJDUTC - MJD+UTC����
  Output:         
  Return:         StbTime_t
  Others:         
*******************************************************************************/
StbTime_t GetSTBTimeFromMJDUTC(const unsigned char *ucMJDUTC);
/*******************************************************************************
  Function:      GetCurrentTime
  Description:   �õ���ǰʱ��(StbTime_t��ʽ)
  Calls:          
  Called By:      
  Input:          
  Output:         
  Return:        StbTime_t
  Others:                
*******************************************************************************/
StbTime_t GetCurrentTime(void);
/*******************************************************************************
  Function:      GetGmtTm
  Description:   �õ���ǰGMTʱ��(StbTm_t��ʽ)
  Calls:         GetCurrentTime
  Called By:      
  Input:          
  Output:        StbTm_t *pStbTm  - ָ���Ž����StbTm_t�����Ϊ�գ�������Ϊ
  									����ֵ����
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ         
*******************************************************************************/
StbTm_t *GetGmtTm(StbTm_t *pStbTm);
/*******************************************************************************
  Function:      GetLocalTm
  Description:   �õ���ǰLocalʱ��(StbTm_t��ʽ)
  Calls:         GetCurrentTime
  Called By:      
  Input:          
  Output:        StbTm_t *pStbTm  - ָ���Ž����StbTm_t�����Ϊ�գ�������Ϊ
  									����ֵ����
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ         
*******************************************************************************/
StbTm_t *GetLocalTm(StbTm_t *pStbTm);
/*******************************************************************************
  Function:		STBAscTime
  Description:  Converts a time stored in a struct StbTm_t to a charcater string.
  				The string is always exactly 26 characters of the form
               			Tue May 01 02:34:55 1984\n\0
  Calls:          
  Called By:      
  Input:		StbTm_t *pStbTm  - ptr to time structure
  Output:         
  Return:		char*  - returns pointer to static string with time string.
  Others:         
*******************************************************************************/
char *STBAscTime(const StbTm_t *pStbTm);
/*******************************************************************************
  Function:      STBTmAdd
  Description:   ����StbTm_t����StbTmSpan_t�Ľ��
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTmSummand - ָ�򱻼���StbTm_t�Ľṹָ��
  				 StbTmSpan_t *pStbTmSpanAddend - ָ�����StbTmSpan_t�Ľṹָ��
  Output:        
  Return:        ����ָ��ӷ����StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ         
*******************************************************************************/
StbTm_t *STBTmAdd(StbTm_t *pStbTmSummand, 
                   StbTmSpan_t *pStbTmSpanAddend);
/*******************************************************************************
  Function:      STBTmMinus
  Description:   ��������StbTm_t֮��ļ��
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTmMinuend  - ָ�򱻼��� 
  				 StbTm_t *pStbTmSubtrahend - ָ�����
  Output:        
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTmSpan_t *ָ��һ����̬StbTmSpan_t�ṹ���˺�
  				 ������һ�ε��ý���������ε�ֵ  
*******************************************************************************/
StbTmSpan_t *STBTmMinus(StbTm_t *pStbTmMinuend,
                        StbTm_t *pStbTmSubtrahend );
/*******************************************************************************
  Function:      STBTmMinus
  Description:   ��������StbTm_t֮��ļ��
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTm1  - ָ���һ�����Ƚ��� 
  				 StbTm_t *pStbTm2 - ָ��ڶ������Ƚ���
  Output:        
  Return:        ���1 > 2 ����ֵ����0
  				 ���1 = 2 ����ֵ����0
  				 ���1 < 2 ����ֵС��0
  Others:          
*******************************************************************************/
int STBTmCompare(StbTm_t *pStbTm1, StbTm_t *pStbTm2);
/*******************************************************************************
  Function:      STBTimeCompare
  Description:   �Ƚ�����StbTime_t֮��Ĵ�С
  Calls:         
  Called By:      
  Input:         const StbTime_t *pStbTime1  - ָ���һ�����Ƚ��� 
  				 const StbTime_t *pStbTime2 - ָ��ڶ������Ƚ���
  Output:        
  Return:        ���1 > 2 ����ֵ����0
  				 ���1 = 2 ����ֵ����0
  				 ���1 < 2 ����ֵС��0
  Others:          
*******************************************************************************/
int STBTimeCompare(const StbTime_t *pStbTime1, const StbTime_t *pStbTime2);
/*************************************************
  Function:       StbTmCopy
  Description:    ����StbTm�ṹ
  Calls:          // �����������õĺ����嵥
  Called By:      // ���ñ������ĺ����嵥
  Input:         
  Output:        			
  Return:         
  Others:         // ����˵��
*************************************************/
void StbTmCopy(StbTm_t *pStbTmDest, StbTm_t *pStbTmSrc);
/*******************************************************************************
  Function:      StbTmSpanNormal
  Description:   ����StbTmSpan_t�ĸ�����
  Calls:         
  Called By:      
  Input:         StbTmSpan_t *pStbTmSpan  - ��Ҫ�����򻯵�StbTmSpan_t
  Output:        
  Return:       
  Others:        
*******************************************************************************/
void StbTmSpanNormal(StbTmSpan_t *pStbTmSpan);
/*******************************************************************************
  Function:       GetStbLocalTime
  Description:    �ѽ��յ���MJD+UTCʱ���е�ʱ��ֵת��Ϊ��׼ʱ���ʽ��
  Calls:          // �����������õĺ����嵥
              a.    ConvBCD()
  Called By:      // ���ñ������ĺ����嵥
  Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
  Table Updated:  // ���޸ĵı����������ǣ�������ݿ�����ĳ���
  Input:
  		a.	ucMJDUTC	:	unsigned char *  //�洢MJD+UTCʱ��ֵ�������ַ
  Output: 
  Return:         �洢��׼ʱ��Ľṹ��ַ
  Others:         // ����˵��       
*******************************************************************************/
int GetStbLocalTime(StbOldTime_t *pStbTime);
/*******************************************************************************
  Function:       GetStbLocalDate
  Description:    �ѽ��յ���MJD+UTCʱ���е�����ֵת��Ϊ��׼���ڸ�ʽ��
  Calls:         
              a.    GetMJD()  
              b.    IsLeapYear()
  Called By:      // ���ñ������ĺ����嵥
  Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
  Table Updated:  // ���޸ĵı����������ǣ�������ݿ�����ĳ���
  Input:
  		a.	ucMJDUTC	:	unsigned char *  //�洢MJD+UTCʱ��ֵ�������ַ
  Output: 
  Return:         �洢��׼���ڽṹ�ĵ�ַ
  Others:         // ����˵��     
*******************************************************************************/
int GetStbLocalDate(StbOldDate_t *pStbDate);
/*******************************************************************************
  Function:		STBAscTime
  Description:  Converts a time stored in a struct StbTm_t to a charcater string.
  				The string is always exactly 26 characters of the form
               			Tue May 01 02:34:55 1984\n\0
  Calls:          
  Called By:      
  Input:		StbTm_t *pStbTm  - ptr to time structure
  Output:         
  Return:		char*  - returns pointer to static string with time string.
  Others:         
*******************************************************************************/
void STBSetTimeZone(signed char scTimeZone);

void STBSaveTimeZone(void);
/*******************************************************************************
  Function:		STBAscTime
  Description:  Converts a time stored in a struct StbTm_t to a charcater string.
  				The string is always exactly 26 characters of the form
               			Tue May 01 02:34:55 1984\n\0
  Calls:          
  Called By:      
  Input:		StbTm_t *pStbTm  - ptr to time structure
  Output:         
  Return:		char*  - returns pointer to static string with time string.
  Others:         
*******************************************************************************/
signed char STBGetTimeZone( void);

void STBLoadTimeZone(void);

/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif
#endif
