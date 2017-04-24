/*******************************************************************************
    Copyright (C), 2004-2007, YOUNGSUN. Co., Ltd.

    File name : 

    Description : 

    Others:      

    History:     
    ======================================================================
    时    间：2004-03-12
    作    者：Cao Songtao
    说    明：1. 创建
    ======================================================================
    时    间：2004-03-01
    作    者：XXX
    说    明：1. 问题简述
              2. 修改内容简述
              3. 其他

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
  Description:   将StbTime_t转换成StbTm_t结构(Gmt时间)
  Calls:          
  Called By:      
  Input:         const StbTime_t *pTimer - 指向将要被转换的StbTime_t
  Output:         
  Return:        返回指向StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTm_t *指向一个静态StbTm_t结构，此函数的下一
  				 次调用将会清除本次的值
*******************************************************************************/
StbTm_t *STBGmTime(const StbTime_t *pTimer);
/*******************************************************************************
  Function:       STBMkTime
  Description:    将StbTime_t结构转换成StbTm_t，并将StbTm_t中的值"规范化"
  Called By:      
  Input:          StbTm_t *pStbTm  - 指向将要被转换的StbTm_t
  Output:         
  Return:         StbTime_t
  Others:         
*******************************************************************************/
StbTime_t STBMkTime(StbTm_t* pStbTm);
/*******************************************************************************
  Function:      STBLocalTime 
  Description:   将StbTime_t转换成StbTm_t结构(本地时间)
  Calls:          
  Called By:      
  Input:         const StbTime_t *pTimer - 指向将要被转换的StbTime_t
  Output:         
  Return:        返回指向StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTm_t *指向一个静态StbTm_t结构，此函数的下一
  				 次调用将会清除本次的值
*******************************************************************************/
StbTm_t *STBLocalTime(const StbTime_t *pTimer);
/*******************************************************************************
  Function:       GetSTBTimeFromMJDUTC
  Description:    将MJD+UTC数组格式的时间转换为StbTime_t
  Calls:          GetSTBTmFromMJDUTC
  Called By:      
  Input:          unsigned char *ucMJDUTC - MJD+UTC数组
  Output:         
  Return:         StbTime_t
  Others:         
*******************************************************************************/
StbTime_t GetSTBTimeFromMJDUTC(const unsigned char *ucMJDUTC);
/*******************************************************************************
  Function:      GetCurrentTime
  Description:   得到当前时间(StbTime_t格式)
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
  Description:   得到当前GMT时间(StbTm_t格式)
  Calls:         GetCurrentTime
  Called By:      
  Input:          
  Output:        StbTm_t *pStbTm  - 指向存放结果的StbTm_t，如果为空，则结果作为
  									返回值返回
  Return:        返回指向StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTm_t *指向一个静态StbTm_t结构，此函数的下一
  				 次调用将会清除本次的值         
*******************************************************************************/
StbTm_t *GetGmtTm(StbTm_t *pStbTm);
/*******************************************************************************
  Function:      GetLocalTm
  Description:   得到当前Local时间(StbTm_t格式)
  Calls:         GetCurrentTime
  Called By:      
  Input:          
  Output:        StbTm_t *pStbTm  - 指向存放结果的StbTm_t，如果为空，则结果作为
  									返回值返回
  Return:        返回指向StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTm_t *指向一个静态StbTm_t结构，此函数的下一
  				 次调用将会清除本次的值         
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
  Description:   计算StbTm_t加上StbTmSpan_t的结果
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTmSummand - 指向被加数StbTm_t的结构指针
  				 StbTmSpan_t *pStbTmSpanAddend - 指向加数StbTmSpan_t的结构指针
  Output:        
  Return:        返回指向加法结果StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTm_t *指向一个静态StbTm_t结构，此函数的下一
  				 次调用将会清除本次的值         
*******************************************************************************/
StbTm_t *STBTmAdd(StbTm_t *pStbTmSummand, 
                   StbTmSpan_t *pStbTmSpanAddend);
/*******************************************************************************
  Function:      STBTmMinus
  Description:   计算俩个StbTm_t之间的间隔
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTmMinuend  - 指向被减数 
  				 StbTm_t *pStbTmSubtrahend - 指向减数
  Output:        
  Return:        返回指向StbTm_t的指针
				 返回NULL，如果操作失败
  Others:        注意，这里返回的StbTmSpan_t *指向一个静态StbTmSpan_t结构，此函
  				 数的下一次调用将会清除本次的值  
*******************************************************************************/
StbTmSpan_t *STBTmMinus(StbTm_t *pStbTmMinuend,
                        StbTm_t *pStbTmSubtrahend );
/*******************************************************************************
  Function:      STBTmMinus
  Description:   计算俩个StbTm_t之间的间隔
  Calls:         
  Called By:      
  Input:         StbTm_t *pStbTm1  - 指向第一个被比较数 
  				 StbTm_t *pStbTm2 - 指向第二个被比较数
  Output:        
  Return:        如果1 > 2 返回值大于0
  				 如果1 = 2 返回值等于0
  				 如果1 < 2 返回值小于0
  Others:          
*******************************************************************************/
int STBTmCompare(StbTm_t *pStbTm1, StbTm_t *pStbTm2);
/*******************************************************************************
  Function:      STBTimeCompare
  Description:   比较俩个StbTime_t之间的大小
  Calls:         
  Called By:      
  Input:         const StbTime_t *pStbTime1  - 指向第一个被比较数 
  				 const StbTime_t *pStbTime2 - 指向第二个被比较数
  Output:        
  Return:        如果1 > 2 返回值大于0
  				 如果1 = 2 返回值等于0
  				 如果1 < 2 返回值小于0
  Others:          
*******************************************************************************/
int STBTimeCompare(const StbTime_t *pStbTime1, const StbTime_t *pStbTime2);
/*************************************************
  Function:       StbTmCopy
  Description:    复制StbTm结构
  Calls:          // 被本函数调用的函数清单
  Called By:      // 调用本函数的函数清单
  Input:         
  Output:        			
  Return:         
  Others:         // 其它说明
*************************************************/
void StbTmCopy(StbTm_t *pStbTmDest, StbTm_t *pStbTmSrc);
/*******************************************************************************
  Function:      StbTmSpanNormal
  Description:   规则化StbTmSpan_t的各属性
  Calls:         
  Called By:      
  Input:         StbTmSpan_t *pStbTmSpan  - 需要被规则化的StbTmSpan_t
  Output:        
  Return:       
  Others:        
*******************************************************************************/
void StbTmSpanNormal(StbTmSpan_t *pStbTmSpan);
/*******************************************************************************
  Function:       GetStbLocalTime
  Description:    把接收到的MJD+UTC时间中的时间值转化为标准时间格式。
  Calls:          // 被本函数调用的函数清单
              a.    ConvBCD()
  Called By:      // 调用本函数的函数清单
  Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
  Table Updated:  // 被修改的表（此项仅对于牵扯到数据库操作的程序）
  Input:
  		a.	ucMJDUTC	:	unsigned char *  //存储MJD+UTC时间值的数组地址
  Output: 
  Return:         存储标准时间的结构地址
  Others:         // 其它说明       
*******************************************************************************/
int GetStbLocalTime(StbOldTime_t *pStbTime);
/*******************************************************************************
  Function:       GetStbLocalDate
  Description:    把接收到的MJD+UTC时间中的日期值转化为标准日期格式。
  Calls:         
              a.    GetMJD()  
              b.    IsLeapYear()
  Called By:      // 调用本函数的函数清单
  Table Accessed: // 被访问的表（此项仅对于牵扯到数据库操作的程序）
  Table Updated:  // 被修改的表（此项仅对于牵扯到数据库操作的程序）
  Input:
  		a.	ucMJDUTC	:	unsigned char *  //存储MJD+UTC时间值的数组地址
  Output: 
  Return:         存储标准日期结构的地址
  Others:         // 其它说明     
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
