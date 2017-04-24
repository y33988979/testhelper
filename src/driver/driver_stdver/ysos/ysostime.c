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

/*----------------------------------------------------------------------------
 *	Includes
 *----------------------------------------------------------------------------*/
#include "ysostime.h"
#include "drv_os.h"
#include "ys_os.h"
/*����tm �ṹ�� ��*/
#include <time.h>
/*----------------------------------------------------------------------------
 *	Structures & Enums
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Private Constants
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Private Variables
 *----------------------------------------------------------------------------*/
static S32_YS m_LeapYearDays[] = 
{
  -1, 30, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

static S32_YS m_NormalYearDays[] = 
{
  -1, 30, 58, 89, 119, 150, 180, 211, 242, 272, 303, 333, 364
};
static StbTm_t stbtm = {0};	/* time block */
static StbTm_t m_stbtm = {0}; //zzjiang add for hefei
static StbTmSpan_t stbtmspan = {0}; /* time span block */
/*----------------------------------------------------------------------------
 *	External variables
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
 *	Global variables
 *----------------------------------------------------------------------------*/
DRVOS_TimeMS_t g_clkStart = 0;  
S32_YS g_nTimeStart = 0;
S32_YS g_nTimeCur = 0;

static signed char m_scTimeZone = 8;

/*----------------------------------------------------------------------------
 *	External Function Prototypes
 *----------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *	Private Function Prototypes
 *----------------------------------------------------------------------------*/
#if 1
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
#endif
 
/*----------------------------------------------------------------------------
 *	Functions
 *----------------------------------------------------------------------------*/
 
 
/*----------------------------------------------------------------------------
 *	Functions
 *----------------------------------------------------------------------------*/
 
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
void STBSetTimeZone(signed char scTimeZone)
{
	if( scTimeZone > 12)
		scTimeZone = 0;
	else if( scTimeZone < -12)
		scTimeZone = 0;
	m_scTimeZone = scTimeZone;
}

void STBSaveTimeZone(void)
{
	if(m_scTimeZone < 0)
		DRV_WriteTimeZone(12-m_scTimeZone);
	else
	DRV_WriteTimeZone( m_scTimeZone);
}

/*----------------------------------------------------------------------------
 *	Functions
 *----------------------------------------------------------------------------*/
 
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
signed char STBGetTimeZone( void)
{	
	return m_scTimeZone;
}

void STBLoadTimeZone(void)
{
	m_scTimeZone = DRV_ReadTimeZone( );
	if( m_scTimeZone > 12)
	{
		if(m_scTimeZone > 24)
		m_scTimeZone = 0;
		else
			m_scTimeZone = -(m_scTimeZone-12);
	}
	else
	{
		if(m_scTimeZone < 0)
		m_scTimeZone = 0;
}
}


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
char * STBAscTime(const StbTm_t *pStbTm)
{
	struct tm TmTemp = {0};

	TmTemp.tm_sec = pStbTm->Second;
	TmTemp.tm_min = pStbTm->Minute;
	TmTemp.tm_hour = pStbTm->Hour;
	TmTemp.tm_mday = pStbTm->Day;
	TmTemp.tm_mon = pStbTm->Month;
	TmTemp.tm_year = pStbTm->Year;
	TmTemp.tm_wday = pStbTm->DayOfWeek;
	TmTemp.tm_yday = pStbTm->DayOfYear;
	TmTemp.tm_isdst = pStbTm->IsDst;

	return (char *)(asctime(&TmTemp));
}


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
/* �ݲ���ʹ�ã���ʹ��GetLocalTime()����*/
#if 0
StbTm_t *STBGmTime(const StbTime_t *pTimer)
{
	struct tm *pTmTemp = NULL;
	time_t tmptm1;
	
	// �������Ϸ���
	if (*pTimer < 0)
		return NULL;

	//����OS_BASETIMEУ׼ʱ�䣬����gmtime����
	tmptm1 = (*pTimer) + OS_BASETIME;
	pTmTemp = gmtime(&tmptm1);
	
	if (pTmTemp)
	{
		stbtm.Second = pTmTemp->tm_sec;
		stbtm.Minute = pTmTemp->tm_min;
		stbtm.Hour = pTmTemp->tm_hour;
		stbtm.Day = pTmTemp->tm_mday;
		stbtm.Month = pTmTemp->tm_mon;
		stbtm.Year = pTmTemp->tm_year;
		stbtm.DayOfWeek = pTmTemp->tm_wday;
		stbtm.DayOfYear = pTmTemp->tm_yday;
		stbtm.IsDst = pTmTemp->tm_isdst;

		return &stbtm;
	}

	return NULL;
	
}
#endif

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
StbTm_t *STBGmTime (const StbTime_t *timp)
{
	S32_YS caltim = (S32_YS)(*timp);            /* calendar time to convert */
       S32_YS islpyr = 0;                 /* is-current-year-a-leap-year flag */
        S32_YS tmptim;
        S32_YS *mdays;                /* pointer to days or m_LeapYearDays */
        StbTm_t *ptb = &stbtm;            /* point to gmtime buffer */

	if ( caltim < 0 )
                return(NULL);
	/*����OS_BASETIMEУ׼ʱ��*/
	caltim += OS_BASETIME;
 	/*
         * Determine years since 1970. First, identify the four-year interval
         * since this makes handling leap-years easy (note that 2000 IS a
         * leap year and 2100 is out-of-range).
         */
        tmptim = (S32_YS)(caltim / OS_FOUROS_YEARSEC);
        caltim -= ((S32_YS)tmptim * OS_FOUROS_YEARSEC);

        /*
         * Determine which year of the interval
         */
        /*
         * Note, it takes 366 days-worth of seconds to get past a leap
         * year.
         */
        tmptim = (tmptim * 4) + 70;         /* 1970, 1974, 1978,...,etc. */

        if ( caltim >= OS_YEARSEC ) {

            tmptim++;                       /* 1971, 1975, 1979,...,etc. */
            caltim -= OS_YEARSEC;

            if ( caltim >= OS_YEARSEC ) {

                tmptim++;                   /* 1972, 1976, 1980,...,etc. */
                caltim -= OS_YEARSEC;

                /*
                 * Note, it takes 366 days-worth of seconds to get past a leap
                 * year.
                 */
                if ( caltim >= (OS_YEARSEC + OS_DAYSEC) ) {

                        tmptim++;           /* 1973, 1977, 1981,...,etc. */
                        caltim -= (OS_YEARSEC + OS_DAYSEC);
                }
                else {
                        /*
                         * In a leap year after all, set the flag.
                         */
                        islpyr++;
                }
            }
        }
        

        /*
         * tmptim now holds the value for tm_year. caltim now holds the
         * number of elapsed seconds since the beginning of that year.
         */
        ptb->Year = (short)tmptim;

        /*
         * Determine days since January 1 (0 - 365). This is the tm_yday value.
         * Leave caltim with number of elapsed seconds in that day.
         */
        ptb->DayOfYear = (short)(caltim / OS_DAYSEC);
        caltim -= (S32_YS)(ptb->DayOfYear) * OS_DAYSEC;

        /*
         * Determine months since January (0 - 11) and day of month (1 - 31)
         */
        if ( islpyr )
            mdays = m_LeapYearDays;
        else
            mdays = m_NormalYearDays;


        for ( tmptim = 1 ; mdays[tmptim] < ptb->DayOfYear; tmptim++ ) ;

        ptb->Month = (short)(--tmptim);

        ptb->Day = (short)(ptb->DayOfYear - mdays[tmptim]);

        /*
         * Determine days since Sunday (0 - 6)
         */
        ptb->DayOfWeek= ((short)(*timp / OS_DAYSEC) + OS_BASEDOW) % 7;

        /*
         *  Determine hours since midnight (0 - 23), minutes after the hour
         *  (0 - 59), and seconds after the minute (0 - 59).
         */
        ptb->Hour = (short)(caltim / 3600);
        caltim -= (S32_YS)(ptb->Hour) * 3600;

        ptb->Minute= (short)(caltim / 60);
        ptb->Second= (short)(caltim - (ptb->Minute) * 60);

        ptb->IsDst= 0;
        return ptb;
}

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
/*
StbTm_t *STBLocalTime(const StbTime_t *pTimer)
{
	struct tm *pTmTemp = NULL;
	time_t tmptm1;

	/*�������Ϸ���
	if (*pTimer < 0)
		return NULL;

	/*����OS_BASETIMEУ׼ʱ�䣬����gmtime����
	tmptm1 = (time_t)(*pTimer) + OS_BASETIME;
	pTmTemp = localtime(&tmptm1);	

	if (pTmTemp)
	{
		stbtm.Second = pTmTemp->tm_sec;
		stbtm.Minute = pTmTemp->tm_min;
		stbtm.Hour = pTmTemp->tm_hour;
		stbtm.Day = pTmTemp->tm_mday;
		stbtm.Month = pTmTemp->tm_mon;
		stbtm.Year = pTmTemp->tm_year;
		stbtm.DayOfWeek = pTmTemp->tm_wday;
		stbtm.DayOfYear = pTmTemp->tm_yday;
		stbtm.IsDst = pTmTemp->tm_isdst;

		return &stbtm;
	}

	return NULL;
	
}
*/

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
StbTm_t *STBLocalTime(const StbTime_t *ptime)
{
	register StbTm_t *ptm = NULL;
        S32_YS ltime;

  /*�����ⲿʹ�ÿ���Ϊ�з����������ý�Сֵ*/
  U32_YS MaxInt = 0x7fffffff;//0xffffffff - 1;

        /*
         * Check for illegal time_t value
         */
        if ( (S32_YS)(*ptime) < 0 )
                return( NULL );

        if ( (*ptime > 3 * OS_DAYSEC) && (*ptime < MaxInt - 3 * OS_DAYSEC) )
        {
                /*
                 * The date does not fall within the first three, or last
                 * three, representable days of the Epoch. Therefore, there
                 * is no possibility of overflowing or underflowing the
                 * time_t representation as we compensate for timezone and
                 * Daylight Savings Time.
                 */

                //ltime = (S32_YS)(*ptime) + OS_TIMEZONE;���¾��Ч����һ����
                ltime = (long)(*ptime) + m_scTimeZone*3600;

                ptm = STBGmTime( (StbTime_t *)(&ltime) );

                /*
                 * Check and adjust for Daylight Saving Time.
                 */
                /*DST not applied
                if ( _daylight && _isindst( ptm ) ) {
                        ltime -= _dstbias;
                        ptm = gmtime( (time_t *)&ltime );
                        ptm->tm_isdst = 1;
                }
                */
        }
        else {
                ptm = STBGmTime( ptime );

                /*
                 * The date falls with the first three, or last three days
                 * of the Epoch. It is possible the time_t representation
                 * would overflow or underflow while compensating for
                 * timezone and Daylight Savings Time. Therefore, make the
                 * timezone and Daylight Savings Time adjustments directly
                 * in the tm structure. The beginning of the Epoch is
                 * 00:00:00, 01-01-70 (UTC) and the last representable second
                 * in the Epoch is 03:14:07, 01-19-2038 (UTC). This will be
                 * used in the calculations below.
                 *
                 * First, adjust for the timezone.
                 */
                 /*
                if ( _isindst(ptm) )
                        ltime = (S32_YS)ptm->tm_sec - (_timezone + _dstbias);
                else
                        ltime = (S32_YS)ptm->tm_sec - _timezone;
                */
                //ltime = (S32_YS)(ptm->Second) + OS_TIMEZONE;
                ltime = (long)(ptm->Second) + m_scTimeZone*3600;

                ptm->Second = (short)(ltime % 60);
                if ( ptm->Second < 0 ) 
                {
                        ptm->Second += 60;
                        ltime -= 60;
                }

                ltime = (S32_YS)(ptm->Minute)+ ltime/60;
                ptm->Minute = (short)(ltime % 60);
                if ( ptm->Minute < 0 ) 
                {
                        ptm->Minute += 60;
                        ltime -= 60;
                }

                ltime = (S32_YS)(ptm->Hour)+ ltime/60;
                ptm->Hour = (short)(ltime % 24);
                if ( ptm->Hour < 0 ) 
                {
                        ptm->Hour += 24;
                        ltime -=24;
                }

                ltime /= 24;

                if ( ltime > 0 ) 
                {
                        /*
                         * There is no possibility of overflowing the tm_mday
                         * and tm_yday fields since the date can be no later
                         * than January 19.
                         */
                        ptm->DayOfWeek = (short)((ptm->DayOfWeek+ ltime) % 7);
                        ptm->Day += (short)ltime;
                        ptm->DayOfYear += (short)ltime;
                }
                else if ( ltime < 0 ) 
                {
                        /*
                         * It is possible to underflow the tm_mday and tm_yday
                         * fields. If this happens, then adjusted date must
                         * lie in December 1969.
                         */
                        ptm->DayOfWeek =  (short)((ptm->DayOfWeek + 7 + ltime) % 7);
                        if ( (ptm->Day +=  (short)ltime) <= 0 ) 
                        {
                                ptm->Day += 31;
                                ptm->DayOfYear = 364;
                                ptm->Month = 11;
                                ptm->DayOfYear--;
                        }
                        else {
                                ptm->DayOfYear +=  (short)ltime;
                        }
                }


        }


        return(ptm);
}


/*******************************************************************************
  Function:       STBMkTime
  Description:    ��StbTime_t�ṹת����StbTm_t������StbTm_t�е�ֵ"�淶��"
  Called By:      
  Input:          StbTm_t *pStbTm  - ָ��Ҫ��ת����StbTm_t
  Output:         
  Return:         StbTime_t
  Others:         
*******************************************************************************/
StbTime_t STBMkTime(StbTm_t* pStbTm)
{
    /* C �������ڳ�ʼ���ṹʱ������ memset���õ���ʹ�� task_lock ʧЧ */
	struct tm TmTemp/* = {0}*/;
	time_t tmptm1;
	
	/*�������Ϸ���*/
	if (pStbTm == NULL)
		return (StbTime_t)(-1);

    /*  ��Ϊ��ʽ��ֵ */
	TmTemp.tm_wday = 0;
	TmTemp.tm_yday = 0;
	TmTemp.tm_isdst = 0;

	TmTemp.tm_sec = pStbTm->Second;
	TmTemp.tm_min = pStbTm->Minute;
	TmTemp.tm_hour = pStbTm->Hour;
	TmTemp.tm_mday = pStbTm->Day;
	TmTemp.tm_mon = pStbTm->Month;
	TmTemp.tm_year = pStbTm->Year;
	TmTemp.tm_wday = pStbTm->DayOfWeek;
	TmTemp.tm_yday = pStbTm->DayOfYear;
	TmTemp.tm_isdst = pStbTm->IsDst;

	/*����mktime����*/
	tmptm1 = mktime(&TmTemp);

	if (tmptm1 == -1)
		return (StbTime_t)(-1);

	/*��"�淶��"���ֵ����pStbTm*/
	pStbTm->Second = TmTemp.tm_sec;
	pStbTm->Minute = TmTemp.tm_min;
	pStbTm->Hour = TmTemp.tm_hour;
	pStbTm->Day = TmTemp.tm_mday;
	pStbTm->Month = TmTemp.tm_mon;
	pStbTm->Year = TmTemp.tm_year;
	pStbTm->DayOfWeek = TmTemp.tm_wday;
	pStbTm->DayOfYear = TmTemp.tm_yday;
	pStbTm->IsDst = TmTemp.tm_isdst;
	
	/*����OS_BASETIMEУ׼ʱ��*/
	{
		StbTime_t ptmTime;
		ptmTime = (StbTime_t)(tmptm1 - OS_BASETIME);
		if (ptmTime < 0)
			return (StbTime_t)(-1);

		return ptmTime;
	}

}

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
StbTime_t GetSTBTimeFromMJDUTC(const unsigned char *ucMJDUTC)
{
	return STBMkTime(GetSTBTmFromMJDUTC(ucMJDUTC));
}
/*******************************************************************************
  Function:      GetSTBTmFromMJDUTC
  Description:   ��MJD+UTC�����ʽ��ʱ��ת��ΪStbTm_t��ʽ
  Calls:          
  Called By:     GetSTBTimeFromMJDUTC 
  Input:         unsigned char *ucMJDUTC - MJD+UTC���� 
  Output:         
  Return:        ����ָ��StbTm_t��ָ��
				 ����NULL���������ʧ��
  Others:        ע�⣬���ﷵ�ص�StbTm_t *ָ��һ����̬StbTm_t�ṹ���˺�������һ
  				 �ε��ý���������ε�ֵ         
*******************************************************************************/
StbTm_t *GetSTBTmFromMJDUTC(const unsigned char *ucMJDUTC)
{
	StbOldDate_t StbOldDate/* = {0}*/;
	StbOldTime_t StbOldTime/* = {0}*/;
	
	/*������*/
	if (ucMJDUTC == NULL)
		return NULL;

	/*��MJD+UTCʱ��ת��*/
	GetStbDate(ucMJDUTC, &StbOldDate);
	GetStbTime(ucMJDUTC, &StbOldTime);

//zzjiang add for hefei    
	m_stbtm.Second = StbOldTime.ucSecond;
	m_stbtm.Minute = StbOldTime.ucMinute;
	m_stbtm.Hour = StbOldTime.ucHour;
	m_stbtm.Day = StbOldDate.ucDay;
	m_stbtm.Month = StbOldDate.ucMonth;
	m_stbtm.Year = StbOldDate.usYear;

/*
	stbtm.Second = StbOldTime.ucSecond;
	stbtm.Minute = StbOldTime.ucMinute;
	stbtm.Hour = StbOldTime.ucHour;
	stbtm.Day = StbOldDate.ucDay;
	stbtm.Month = StbOldDate.ucMonth;
	stbtm.Year = StbOldDate.usYear;

	return &stbtm;
*/

    return &m_stbtm;
}

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
StbTime_t GetCurrentTime(void)
{
    YS_OS_TaskLock();
    
	  g_nTimeCur = g_nTimeStart + (DRV_OS_TimeMinus_MS(DRV_OS_TimeNow_MS(),g_clkStart) / 1000);

	  YS_OS_TaskUnlock();
    
	return g_nTimeCur;
}

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
StbTm_t *GetGmtTm(StbTm_t *pStbTm)
{
	StbTime_t TimerTmp = GetCurrentTime();
	
	if (pStbTm != NULL)
	{
		STBTmCopy(pStbTm, STBGmTime(&TimerTmp));
		return pStbTm;
	}
	else
		return STBGmTime(&TimerTmp);
}

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
StbTm_t *GetLocalTm(StbTm_t *pStbTm)
{
	StbTime_t TimerTmp = GetCurrentTime();
	
	if (pStbTm != NULL)
	{
		STBTmCopy(pStbTm, STBLocalTime(&TimerTmp));
		return pStbTm;
	}
	else
		return STBLocalTime(&TimerTmp);
}

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
StbTm_t *STBTmAdd(StbTm_t *pStbTmSummand, StbTmSpan_t *pStbTmSpanAddend)
{
	STBTmCopy(&stbtm, pStbTmSummand);

	stbtm.Second += GetTotalSeconds(pStbTmSpanAddend);

	/* ����stbtm */
	STBMkTime(&stbtm);
	
	return &stbtm;
}

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
StbTmSpan_t *STBTmMinus(StbTm_t *pStbTmMinuend, StbTm_t *pStbTmSubtrahend )
{
	stbtmspan.Seconds = STBMkTime(pStbTmMinuend) - STBMkTime(pStbTmSubtrahend);

	/* ����stbtmspan */
	StbTmSpanNormal(&stbtmspan);
	
	return &stbtmspan;
}

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
S32_YS STBTmCompare(StbTm_t *pStbTm1, StbTm_t *pStbTm2)
{
	StbTime_t TimerTmp1 = STBMkTime(pStbTm1);
	StbTime_t TimerTmp2 = STBMkTime(pStbTm2);
	return STBTimeCompare(&TimerTmp1, &TimerTmp2);
}

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
S32_YS STBTimeCompare(const StbTime_t *pStbTime1, const StbTime_t *pStbTime2)
{
	return (*pStbTime1) - (*pStbTime2);
}

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
void StbTmSpanNormal(StbTmSpan_t *pStbTmSpan)
{
	pStbTmSpan->Days += (pStbTmSpan->Hours + (pStbTmSpan->Minutes + pStbTmSpan->Seconds / 60) / 60) / 24;
	pStbTmSpan->Hours = (pStbTmSpan->Hours + (pStbTmSpan->Minutes + pStbTmSpan->Seconds / 60) / 60) % 24;
	pStbTmSpan->Minutes = (pStbTmSpan->Minutes + pStbTmSpan->Seconds / 60) % 60;
	pStbTmSpan->Seconds %= 60;

	pStbTmSpan->TotalHours = GetTotalHours(pStbTmSpan);
	pStbTmSpan->TotalMinutes = GetTotalMinutes(pStbTmSpan);
	pStbTmSpan->TotalSeconds = GetTotalSeconds(pStbTmSpan);

	/* ������һ�ּ��㷽�� */
	/*
	pStbTmSpan->Days = pStbTmSpan->TotalHours / 24;
	pStbTmSpan->Hours = pStbTmSpan->TotalHours % 24;
	pStbTmSpan->Minutes = pStbTmSpan->TotalMinutes % 60;
	pStbTmSpan->Seconds = pStbTmSpan->TotalSeconds % 60;
	*/ 
	return;
}

/*******************************************************************************
  Function:       
  Function:       GetMJD
  Description:   �ӽ��յ���MJD+UTCʱ��ֵ�л��MJD��
  Calls:        
  Called By:      
              a.    GetStbDate()
              b.    GetDayOfWeek()
  Table Accessed: 
  Table Updated:  
  Input:
  		a.	ucMJDUTC	:	unsigned char *  //�洢MJD+UTCʱ��ֵ�������ַ
  Output: 
  Return:        ����ֵ��MJD��ʽ��ʱ��
  Others:      
*******************************************************************************/
unsigned short GetMJD(const unsigned char *ucMJDUTC)
{
    unsigned short usMJD;

    if (ucMJDUTC == NULL)
    	return 0;

    //ȡ��MJDֵ��16λ��ucMJDUTC��ǰ���ֽ�
    usMJD = ucMJDUTC[0];
	usMJD <<= 8;
    //usMJD <<= sizeof(ucMJDUTC[0]);
    usMJD += ucMJDUTC[1];

    return usMJD;
}

/*******************************************************************************
  Function:       GetStbTime
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
void GetStbTime(const unsigned char *ucMJDUTC, StbOldTime_t *pStbTime)
{
    unsigned char ucUTCTmp;  //���UTCֵ
    unsigned char ucHour, ucMinute, ucSecond;

    if ((ucMJDUTC == NULL) || (pStbTime == NULL))
    	return ;
    
    //ȡ��UTCֵ��24λ��ucMJDUTC�ĺ�3�ֽ�
    ucUTCTmp = ucMJDUTC[2];    
    ucHour = ConvBCD(ucUTCTmp) + OS_BASEHOUR;    
    ucUTCTmp = ucMJDUTC[3];
    ucMinute = ConvBCD(ucUTCTmp) + OS_BASEMINUTE;
    ucUTCTmp = ucMJDUTC[4];
    ucSecond = ConvBCD(ucUTCTmp) + OS_BASESECOND;

    //����ʱ��
    if (ucSecond > 60)
    {
        ucSecond -= 60;
        ucMinute++;
    }
    if (ucMinute > 60 )
    {
        ucMinute -=60;
        ucHour++;
    }
    if (ucHour > 24)
    {
        ucHour -=24;
    }

	/*��䷵��ֵ*/
    pStbTime->ucHour = ucHour;
    pStbTime->ucMinute = ucMinute;
    pStbTime->ucSecond = ucSecond;
    	
    return;
}

/*******************************************************************************
  Function:       GetStbDate
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
void GetStbDate(const unsigned char *ucMJDUTC, StbOldDate_t *pStbDate)
{
    S32_YS k;
    unsigned short usYear, usYearTmp;
    unsigned char ucMonth, ucDay;
    unsigned char ucMonthTmp;
    unsigned short usMJDTmp;  //���MJDֵ

    if ((ucMJDUTC == NULL) || (pStbDate == NULL))
    {
    	return;
    }
    
    //ȡ��MJDֵ
    usMJDTmp = GetMJD(ucMJDUTC);
    
    //��MJD��������
    /* �����MJD��������
         Y' = S32_YS [ (MJD - 15 078,2) / 365,25 ]
         M' = S32_YS { [ MJD - 14 956,1 - S32_YS (Y' * 365,25) ] / 30,6001 }
         If M' = 14 or M' = 15, then K = 1; else K = 0
         Y = Y' + K
         M = M' - 1 - K * 12
         D = MJD - 14 956 - S32_YS (Y' * 365,25) - S32_YS (M' �� 30,6001 )
    */        
    usYearTmp = (unsigned short)((usMJDTmp - 15078.2) / 365.25);
    ucMonthTmp = (unsigned char)((usMJDTmp - 14956.1 - (S32_YS)(usYearTmp * 365.25)) / 30.6001);

    if (ucMonthTmp == 14 || ucMonthTmp == 15)    	
        k = 1;
    else
        k = 0;

    usYear = usYearTmp + k;
    ucMonth = ucMonthTmp - 1 - k * 12;
    ucDay = (unsigned char)(usMJDTmp - 14956 - (S32_YS)(usYearTmp * 365.25) - (S32_YS)(ucMonthTmp * 30.6001));

	
    //���ϻ�׼���ڣ������׼����
    usYear = usYear + OS_BASEYEAR;    

    ucMonth = ucMonth + OS_BASEMONTH;
    if (ucMonth > 12)
    {
        ucMonth -= 12;
        usYear++;
    }    

    ucDay += OS_BASEDAY;//�������Ϊ31+31=62��   
	
    switch (ucMonth)
    {
        case 1:  //��ǰΪ���·ݣ���������
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        	        if (ucDay > 31)
        	        {
        	            ucDay -= 31;
        	            ucMonth++;
        	        
	        	        if (ucMonth == 2)  //����1�¼�1Ϊ2��ʱ��������������
	        	        {
	        	            if(IsLeapYear(usYear))
	        	            {
	        	                if(ucDay > 29)
	        	            {
	        	                ucDay -= 29;
	        	                ucMonth++;
	        	            }
	        	            }
	        	            else 
	        	            {
	        	                if(ucDay > 28)
	        	            {
	        	                ucDay -= 28;
	        	                ucMonth++;
	        	            }
	        	        }
	        	        }
	        	        //������7�»�12�¼�1Ϊ8�»�13�£�������Ϊ31�죩ʱ��������������
	        	        if ((ucMonth != 8) && (ucMonth != 13) && (ucDay > 30))
	        	        {
	        	            ucDay -= 30;
	        	            ucMonth++;
	        	        }
    			}
        	        if (ucMonth > 12)
                      {
                          ucMonth -= 12;
                          usYear++;
                      }
        	        break;
        case 4:  //��ǰΪС�·ݣ���������
        case 6:
        case 9:
        case 11:
        	        if (ucDay > 30)
        	        {
        	            ucDay -= 30;
        	            ucMonth++;
        	        
	        	        if (ucDay > 31)
	        	        {
	        	            ucDay -= 31;
	        	            ucMonth++;
	        	        }
        	        }
        	        if (ucMonth > 12)
                      {
                          ucMonth -= 12;
                          usYear++;
                      }
        	        break;
        case 2:  //��Ϊ2��ʱ����������
        	        if(IsLeapYear(usYear)) 
        	        {
        	            if(ucDay > 29)
        	        {
        	            ucDay -= 29;
        	            ucMonth++;
        	        }
        	        }  
        	        else
                    {
                        if (ucDay > 28)
        	        {
        	            ucDay -= 28;
        	            ucMonth++;
        	        }
        	        }
        	        if (ucDay > 31)
        	        {
        	            ucDay -= 31;
        	            ucMonth++;
        	        }
        	        break;
        default:
        	        break;
    }  //end switch

    //�жϹ�ʽ����Ч�ԣ�1900-3-1��2100-2-28��
    if ((usYear < 1900) || (usYear > 2100) ||((usYear == 1900) && (ucMonth < 3))
    	 || ((usYear == 2100) && (ucMonth > 2)))
    {
        return;
    }

	/*��䷵��ֵ*/
    pStbDate->usYear = usYear - OS_BASEYEAR;
    pStbDate->ucMonth = ucMonth-1;
    pStbDate->ucDay = ucDay;
    
    return;
}

/*******************************************************************************
  Function:       IsLeapYear
  Description:    �ж��Ƿ�Ϊ����
  Calls:          // �����������õĺ����嵥
  Called By:      // ���ñ������ĺ����嵥
              a.  GetStbDate()
              b.  GetDateSecond()
  Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
  Table Updated:  // ���޸ĵı����������ǣ�������ݿ�����ĳ���
  Input:          
  		a.  usYear:   UI16
  Output: 	
  Return:   TRUEΪ��ǰ�����꣬FALSEΪ����
  Others:         // ����˵��        
*******************************************************************************/
S32_YS IsLeapYear(unsigned short usYear)
{
    unsigned short usYearTmp = usYear;
    
    if (usYearTmp % 400 == 0 || (usYearTmp % 4 == 0 && usYearTmp % 100 != 0))
    {
    	return TRUE_YS;
    }
    else
    {
        return FALSE_YS;
   	}
}
/*************************************************
  Function:       ConvBCD
  Description:    ��BCD��ת��Ϊ����
  Calls:          // �����������õĺ����嵥
  Called By:      // ���ñ������ĺ����嵥
                     a.  GetStbTime()
  Table Accessed: // �����ʵı����������ǣ�������ݿ�����ĳ���
  Table Updated:  // ���޸ĵı����������ǣ�������ݿ�����ĳ���
  Input:         
  			a.  ucBCDCode:	unsigned char 
  Output:        			
  Return:         ת���������
  Others:         // ����˵��
*************************************************/
unsigned char ConvBCD(unsigned char ucBCDCode)
{
    unsigned char ucCode, ucCodeTmp;

    ucCodeTmp = ucBCDCode;
    ucCode = ((ucCodeTmp >> 4) & 0x0F) * 10 + (ucCodeTmp & 0x0F);

    return ucCode;
}

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
void STBTmCopy(StbTm_t *pStbTmDest, StbTm_t *pStbTmSrc)
{
    if ((pStbTmDest == NULL) || (pStbTmSrc == NULL))
    	return;
    
    pStbTmDest->Second = pStbTmSrc->Second;
    pStbTmDest->Minute = pStbTmSrc->Minute;
    pStbTmDest->Hour = pStbTmSrc->Hour;
    pStbTmDest->Day = pStbTmSrc->Day;
    pStbTmDest->Month = pStbTmSrc->Month;
    pStbTmDest->Year = pStbTmSrc->Year;
    pStbTmDest->DayOfWeek = pStbTmSrc->DayOfWeek;
    pStbTmDest->DayOfYear = pStbTmSrc->DayOfYear;
    pStbTmDest->IsDst = pStbTmSrc->IsDst;
    
    return;
}

/*******************************************************************************
  Function:       GetTotalHours
  Description:    ��StbTmSpan_t�еõ���Сʱ��
  Called By:      
  Input:          StbTmSpan_t *pStbTmSpan
  Output:         
  Return:         S32_YS	��Сʱ��
  Others:         
*******************************************************************************/
S32_YS GetTotalHours(StbTmSpan_t *pStbTmSpan)
{
	pStbTmSpan->TotalHours = GetTotalSeconds(pStbTmSpan) / 3600;
	return pStbTmSpan->TotalHours;
}

/*******************************************************************************
  Function:       GetTotalHours
  Description:    ��StbTmSpan_t�еõ��ܷ�����
  Called By:      
  Input:          StbTmSpan_t *pStbTmSpan
  Output:         
  Return:         S32_YS	�ܷ�����
  Others:      
*******************************************************************************/
S32_YS GetTotalMinutes(StbTmSpan_t *pStbTmSpan)
{
	pStbTmSpan->TotalMinutes = GetTotalSeconds(pStbTmSpan) / 60;
	return pStbTmSpan->TotalMinutes;
}

/*******************************************************************************
  Function:       GetTotalHours
  Description:    ��StbTmSpan_t�еõ�������
  Called By:      
  Input:          StbTmSpan_t *pStbTmSpan
  Output:         
  Return:         S32_YS	������	
  Others:           
*******************************************************************************/
S32_YS GetTotalSeconds(StbTmSpan_t *pStbTmSpan)
{
	pStbTmSpan->TotalSeconds = (pStbTmSpan->Days * 3600 * 24 
								+ pStbTmSpan->Hours * 3600
								+ pStbTmSpan->Minutes * 60
								+ pStbTmSpan->Seconds);
	return pStbTmSpan->TotalSeconds;
}

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
S32_YS GetStbLocalTime(StbOldTime_t *pStbTime)
{
	StbTime_t TimerTmp;	

	TimerTmp = GetCurrentTime();
	STBLocalTime(&TimerTmp);
	
	if (pStbTime != NULL)
	{		
		pStbTime->ucHour = stbtm.Hour;
		pStbTime->ucMinute = stbtm.Minute;
		pStbTime->ucSecond = stbtm.Second;
		return TRUE_YS;
	}
	else
		return FALSE_YS;
}

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
S32_YS GetStbLocalDate(StbOldDate_t *pStbDate)
{
	StbTime_t TimerTmp;	

	TimerTmp = GetCurrentTime();
	STBLocalTime(&TimerTmp);
	
	if (pStbDate != NULL)
	{		
		pStbDate->usYear = stbtm.Year;
		pStbDate->ucMonth = stbtm.Month + 1;
		pStbDate->ucDay = stbtm.Day;
		return TRUE_YS;
	}
	else
		return FALSE_YS;
}

