/** @file
 *  @brief 时间操作集
 *  @author jinhailiao
 *  @date 2011/11/07
 *  @version 0.1
 *  1、时间操作类-符合国网规约格式ssmmhhDDMMYYYY
 *  2、时间操作类-符合南网规约格式YYYYMMDDhhmmss
 */
#include "haitime.h"
#include <time.h>
#include <sys/timeb.h>

S_TIME_UNIT C_TIME::GetTimeUnit(const char *pUnitName)
{
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_SECOND) == 0)
		return TIME_UNIT_SECOND;
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_MINUTE) == 0)
		return TIME_UNIT_MINUTE;
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_HOUR) == 0)
		return TIME_UNIT_HOUR;
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_DAY) == 0)
		return TIME_UNIT_DAY;
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_MONTH) == 0)
		return TIME_UNIT_MONTH;
	if (strcasecmp(pUnitName, TIME_UNIT_NAME_YEAR) == 0)
		return TIME_UNIT_YEAR;
	return TIME_UNIT_UNKNOW;
}

const char* C_TIME::GetTimeUnit(S_TIME_UNIT TimeUnit)
{
	switch (TimeUnit)
	{
	case TIME_UNIT_SECOND:return TIME_UNIT_NAME_SECOND;
	case TIME_UNIT_MINUTE:return TIME_UNIT_NAME_MINUTE;
	case TIME_UNIT_HOUR:return TIME_UNIT_NAME_HOUR;
	case TIME_UNIT_DAY:return TIME_UNIT_NAME_DAY;
	case TIME_UNIT_MONTH:return TIME_UNIT_NAME_MONTH;
	case TIME_UNIT_YEAR:return TIME_UNIT_NAME_YEAR;
	default:break;
	}
	return TIME_UNIT_NAME_UNKNOW;
}

/** 取当前时钟(封装time(NULL)) */
S_TIME C_TIME::TimeNow(void)
{
	return (S_TIME)time(NULL);
}

S_TIMEX *C_TIME::TimeNow(S_TIMEX &timeX)
{
#ifdef WIN32
	timeX.m_MS = 0;
	timeX.m_Second = TimeNow();
#else
	struct timeb tb;

	ftime(&tb);
	timeX.m_MS = (S_TIME)tb.millitm;
	timeX.m_Second = (S_TIME)tb.time;
#endif
	return &timeX;
}

S_TIME C_TIME::MakeTime(const S_DATE &date)
{
	struct tm tim;
	memset(&tim, 0x00, sizeof(tim));
	tim.tm_year = date.m_Year - 1900;
	tim.tm_mon  = date.m_Mon - 1;
	tim.tm_mday = date.m_Day;
	tim.tm_hour = date.m_Hour;
	tim.tm_min  = date.m_Min;
	tim.tm_sec  = date.m_Sec;
	return (S_TIME)mktime(&tim);
}

S_DATE C_TIME::LocalTime(S_TIME mytime)
{
	S_DATE mydate;

	LocalTime(mytime, mydate);
	return mydate;
}

S_DATE *C_TIME::LocalTime(S_TIME mytime, S_DATE &date)
{
	time_t needtime = (time_t)mytime;
	struct tm *tim = localtime(&needtime);
	
	date.m_Year = tim->tm_year + 1900;
	date.m_Mon  = tim->tm_mon + 1;
	date.m_Day  = tim->tm_mday;
	date.m_Hour = tim->tm_hour;
	date.m_Min  = tim->tm_min;
	date.m_Sec  = tim->tm_sec;
	date.m_WeekDay = tim->tm_wday;
	date.m_YearDay = tim->tm_yday + 1;
	
	return &date;
}

/** BCD串的格式为:ssmmhhDDMMYYYY 符合国网规约*/
char * C_TIME::Time2BCDStr(S_TIME time, char *bcdStr7B)
{
	time_t mytime = (S_TIME)time;
	struct tm *curtime = localtime(&mytime);

	if (curtime != NULL)
	{
		bcdStr7B[0] = hai_cbcd(curtime->tm_sec);
		bcdStr7B[1] = hai_cbcd(curtime->tm_min);
		bcdStr7B[2] = hai_cbcd(curtime->tm_hour);
		bcdStr7B[3] = hai_cbcd(curtime->tm_mday);	
		bcdStr7B[4] = hai_cbcd(curtime->tm_mon+1);
		bcdStr7B[5] = hai_cbcd((curtime->tm_year+1900)%100);
		bcdStr7B[6] = hai_cbcd((curtime->tm_year+1900)/100);
	}
	return bcdStr7B;
}

/** BCD串的格式为:ssmmhhDDMMYYYY 符合国网规约*/
S_TIME C_TIME::BcdStr2Time(const char *bcdStr7B)
{
	struct tm curtime;
	memset(&curtime,0x00,sizeof(struct tm));

	curtime.tm_year = hai_bcdc(bcdStr7B[5]) + hai_bcdc(bcdStr7B[6])*100 - 1900;
	curtime.tm_mon  = hai_bcdc(bcdStr7B[4]) - 1;
	curtime.tm_mday = hai_bcdc(bcdStr7B[3]);
	curtime.tm_hour = hai_bcdc(bcdStr7B[2]);
	curtime.tm_min = hai_bcdc(bcdStr7B[1]);
	curtime.tm_sec = hai_bcdc(bcdStr7B[0]);
	curtime.tm_wday = 0;
	curtime.tm_yday = 0;
	curtime.tm_isdst = -1;

	return (S_TIME)mktime(&curtime);
}

S_TIME C_TIME::BcddateAdd(S_TIME_UNIT interUnit, long offset, const char *srcBcd7B, char *dstBcd7B)
{
	S_TIME date = DateAdd(interUnit, offset, BcdStr2Time(srcBcd7B));
	Time2BCDStr(date, dstBcd7B);
	return date;
}

/** BCD串的格式为:ssmmhhDDMMYYYY 符合国网规约*/
int C_TIME::SetHWClock(const char *bcdStr7B)
{
	int err = 0;
	char cmd[64];
	memset(cmd, 0x00, sizeof(cmd));
	sprintf(cmd, "date %02x%02x%02x%02x%02x%02x.%02x", bcdStr7B[4], bcdStr7B[3], bcdStr7B[2], bcdStr7B[1], bcdStr7B[6], bcdStr7B[5], bcdStr7B[0]);

#ifdef WIN32
//	cout<<"Set Hardware Time to "<<cmd<<endl;
	return err;
#else
	if ((err = system(cmd)) == 0)
		err = system("hwclock -w");
	return err;
#endif
}
int C_TIME::SyncHWClock(void)
{
#ifdef WIN32
	return 0;
#else
	return system("hwclock -s");
#endif
}

//bcdStr5B:mmhhDDMMYY
S_TIME C_TIME::BcdStr2Time5B(const char *bcdStr5B)
{
	char bcdStr7B[7];
	copy(bcdStr5B, bcdStr5B+5, bcdStr7B+1);
	bcdStr7B[6] = 0x20;//YY
	bcdStr7B[0] = 0x00;//SS
	return BcdStr2Time(bcdStr7B);
}
//bcdStr5B:mmhhDDMMYY
char* C_TIME::Time2BcdStr5B(S_TIME time, char *bcdStr5B)
{
	char bcdStr7B[7];
	Time2BCDStr(time, bcdStr7B);
	copy(bcdStr7B+1, bcdStr7B+1+5, bcdStr5B);
	return bcdStr5B;
}


/** BCD串的格式为:YYYYMMDDhhmmss 符合南网规约*/
char *C_TIME::Time2BCDStr_Big(S_TIME time, char *bcdStr7B)
{
	Time2BCDStr(time, bcdStr7B);
	reverse(bcdStr7B, bcdStr7B+7);
	return bcdStr7B;
}
/** BCD串的格式为:YYYYMMDDhhmmss 符合南网规约*/
S_TIME C_TIME::BcdStr2Time_Big(const char *bcdStr7B)
{
	char mybcdStr7B[7];
	reverse_copy(bcdStr7B, bcdStr7B+7, mybcdStr7B);
	return BcdStr2Time(mybcdStr7B);
}
/** BCD串的格式为:YYYYMMDDhhmmss 符合南网规约*/
int C_TIME::SetHWClock_Big(const char *bcdStr7B)
{
	char mybcdStr7B[7];
	reverse_copy(bcdStr7B, bcdStr7B+7, mybcdStr7B);
	return SetHWClock(mybcdStr7B);
}

//bcdStr5B:YYMMDDHHMM
S_TIME C_TIME::BcdStr2Time5B_Big(const char *bcdStr5B)
{
	char bcdStr7B[7];
	reverse_copy(bcdStr5B, bcdStr5B+5, bcdStr7B+1);
	bcdStr7B[6] = 0x20;//YY
	bcdStr7B[0] = 0x00;//SS
	return BcdStr2Time(bcdStr7B);
}

char* C_TIME::Time2BcdStr5B_Big(S_TIME time, char *bcdStr5B)
{
	char bcdStr7B[7];
	Time2BCDStr(time, bcdStr7B);
	reverse_copy(bcdStr7B+1, bcdStr7B+1+5, bcdStr5B);
	return bcdStr5B;
}

//bcdStr6B:YYYYMMDDHHMM
S_TIME C_TIME::BcdStr2Time6B_Big(const char *bcdStr6B)
{
	char bcdStr7B[7];
	reverse_copy(bcdStr6B, bcdStr6B+6, bcdStr7B+1);
	bcdStr7B[0] = 0x00;//SS
	return BcdStr2Time(bcdStr7B);
}

char* C_TIME::Time2BcdStr6B_Big(S_TIME time, char *bcdStr6B)
{
	char bcdStr7B[7];
	Time2BCDStr(time, bcdStr7B);
	reverse_copy(bcdStr7B+1, bcdStr7B+1+6, bcdStr6B);
	return bcdStr6B;
}

S_TIME C_TIME::BcddateAdd_Big(S_TIME_UNIT interUnit, long offset, const char *srcBcd7B, char *dstBcd7B)
{
	S_TIME date = DateAdd(interUnit, offset, BcdStr2Time_Big(srcBcd7B));
	Time2BCDStr_Big(date, dstBcd7B);
	return date;
}


/** @brief 日期对齐
 *  @param[in] interUnit 时间单位
 *  @param[in] date 日期
 *  @return 规整后的日期
 */
S_TIME C_TIME::DateTrim(S_TIME_UNIT interUnit, S_TIME date)
{
	time_t mydate = (time_t)date;
	tm* t = localtime(&mydate);

	if (interUnit >= TIME_UNIT_MINUTE) t->tm_sec=0;
	if (interUnit >= TIME_UNIT_HOUR) t->tm_min=0;
	if (interUnit >= TIME_UNIT_DAY) t->tm_hour=0;
	if (interUnit >= TIME_UNIT_MONTH) t->tm_mday=1;
	if (interUnit >= TIME_UNIT_YEAR) t->tm_mon=0;
	t->tm_wday=0;
	t->tm_yday=0;
	t->tm_isdst=-1;

	return (S_TIME)mktime(t);
}

/** @brief 日期对齐
 *  @param[in] interUnit 时间单位
 *  @param[in] offset 对齐的周期，如5，10
 *  @param[in] date 日期
 *  @return 规整后的日期
 */
S_TIME C_TIME::DateTrim(S_TIME_UNIT interUnit, S_WORD offset, S_TIME timeDate)
{
	S_DATE Date;
	LocalTime(DateTrim(interUnit, timeDate), Date);

	switch (interUnit)
	{
	case TIME_UNIT_SECOND:
		Date.m_Sec = Date.m_Sec - (Date.m_Sec%offset);
		break;
	case TIME_UNIT_MINUTE:
		Date.m_Min = Date.m_Min - (Date.m_Min%offset);
		break;
	case TIME_UNIT_HOUR:
		Date.m_Hour = Date.m_Hour - (Date.m_Hour%offset);
		break;
	case TIME_UNIT_DAY:
		Date.m_Day -= 1; // 从1开始
		Date.m_Day = Date.m_Day - (Date.m_Day%offset);
		Date.m_Day += 1;
		break;
	case TIME_UNIT_MONTH:
		Date.m_Mon -= 1; // 从1开始
		Date.m_Mon = Date.m_Mon - (Date.m_Mon%offset);
		Date.m_Mon += 1;
		break;
	case TIME_UNIT_YEAR:
		Date.m_Year = Date.m_Year - (Date.m_Year%offset);
		break;
	default:
		break;
	}
	return MakeTime(Date);
}

/** @breif 日期单位时间增减
 *  @param[in] interUnit 日期单位
 *  @param[in] offset 时间偏移
 *  @param[in] date 日期
 *  @return 日期
 */
S_TIME C_TIME::DateAdd(S_TIME_UNIT interUnit, long offset, S_TIME date)
{
	time_t mydate = (time_t)date;
	if(mydate>0 && offset!=0)
	{
		tm* t=localtime(&mydate);
		switch(interUnit)
		{
		case TIME_UNIT_SECOND: t->tm_sec  += offset; break;
		case TIME_UNIT_MINUTE: t->tm_min  += offset; break;
		case TIME_UNIT_HOUR:   t->tm_hour += offset; break;
		case TIME_UNIT_DAY:    t->tm_mday += offset; break;
		case TIME_UNIT_MONTH:  t->tm_mon  += offset; break;
		case TIME_UNIT_YEAR:   t->tm_year += offset; break;
		default:break;
		}
		t->tm_wday  = 0;
		t->tm_yday  = 0;
		t->tm_isdst = -1;
		return (S_TIME)mktime(t);
	}
	return date;
}

/** @brief 计算日期间的单位差
 *  @param[in] interUnit 时间单位
 *  @param[in] begin 开始时间
 *  @param[in] end 结束时间
 *  @return >= 0 (end>=begin)
 *  @return <0 (end<begin)
 */
long C_TIME::DateDiff(S_TIME_UNIT interUnit, S_TIME begin, S_TIME end)
{
	time_t d1 = (time_t)DateTrim(interUnit, begin);
	time_t d2 = (time_t)DateTrim(interUnit, end);

	if (interUnit < TIME_UNIT_MONTH)//sec,min,hour,day
	{
		if (interUnit == TIME_UNIT_SECOND)
			return (long)difftime(d2,d1);
		else if (interUnit == TIME_UNIT_MINUTE)
			 return (long)difftime(d2,d1)/((long)60);
		else if(interUnit == TIME_UNIT_HOUR)
			return (long)difftime(d2,d1)/((long)3600);
		else if(interUnit == TIME_UNIT_DAY)
			return (long)difftime(d2,d1)/((long)86400);
		else
			return 0;
	}
	else
	{
		struct tm t1 = *localtime(&d1);
		struct tm t2 = *localtime(&d2);
		if (interUnit == TIME_UNIT_MONTH)
			return (t2.tm_year*12+t2.tm_mon)-(t1.tm_year*12+t1.tm_mon);
		else
			return t2.tm_year-t1.tm_year;
	}
	return 0;
}

/** @brief 取时间串，格式:YYYY-MM-DD hh:mm:ss
 *  @param[out] strAscii20B 格式串输出缓冲
 *  @param[in] time 时针
 *  @return strAscii20B
 */
char *C_TIME::Time2YMDhms(S_TIME time, char *strAscii20B)
{
	S_DATE date;
	LocalTime(time, date);

	strAscii20B[0] = hai_Hex2Char(date.m_Year/1000);
	strAscii20B[1] = hai_Hex2Char(date.m_Year/100%10);
	strAscii20B[2] = hai_Hex2Char(date.m_Year%100/10);
	strAscii20B[3] = hai_Hex2Char(date.m_Year%10);
	strAscii20B[4] = '-';
	strAscii20B[5] = hai_Hex2Char(date.m_Mon/10);
	strAscii20B[6] = hai_Hex2Char(date.m_Mon%10);
	strAscii20B[7] = '-';
	strAscii20B[8] = hai_Hex2Char(date.m_Day/10);
	strAscii20B[9] = hai_Hex2Char(date.m_Day%10);
	strAscii20B[10] = ' ';
	strAscii20B[11] = hai_Hex2Char(date.m_Hour/10);
	strAscii20B[12] = hai_Hex2Char(date.m_Hour%10);
	strAscii20B[13] = ':';
	strAscii20B[14] = hai_Hex2Char(date.m_Min/10);
	strAscii20B[15] = hai_Hex2Char(date.m_Min%10);
	strAscii20B[16] = ':';
	strAscii20B[17] = hai_Hex2Char(date.m_Sec/10);
	strAscii20B[18] = hai_Hex2Char(date.m_Sec%10);
	strAscii20B[19] = '\0';

	return strAscii20B;
}

/** @brief 取时间串，格式:MM-DD hh:mm:ss
 *  @param[out] strAscii14B 格式串输出缓冲
 *  @return strAscii14B
 */
char *C_TIME::Now2Stamp(char *strAscii14B)
{
	char strAscii20B[20];
	Time2YMDhms(TimeNow(), strAscii20B);
	memcpy(strAscii14B, strAscii20B+5, 14);

	return strAscii14B;
}

/** @brief 取时间串，格式:YYYYMMDDhhmm
 *  @param[out] strAscii12B 格式串输出缓冲
 *  @return strAscii12B
 */
char *C_TIME::Now2Ascii12(char *strAscii12B)
{
	char strAscii16B[16];
	
	C_TIME::Time2Ascii14(TimeNow(), strAscii16B);
	memcpy(strAscii12B, strAscii16B, 12);

	return strAscii12B;
}

/** @brief 取时间串，格式:YYYYMMDDhhmmss
 *  @param[out] strAscii16B 格式串输出缓冲
 *  @param[in] time 时针
 *  @return strAscii16B
 */
char* C_TIME::Time2Ascii14(S_TIME time, char *strAscii16B)
{
	int i;
	char bcdStr7B[8];
	Time2BCDStr(time, bcdStr7B);

	for (i = 0; i < 7; i++)
	{
		strAscii16B[i*2+0] = ((bcdStr7B[6-i]>>4)&0x0F)+'0';
		strAscii16B[i*2+1] = ((bcdStr7B[6-i]>>0)&0x0F)+'0';
	}
	strAscii16B[i*2+0] = '\0';
	return strAscii16B;
}

/** @brief 检查时间格式是否符合正常格式，如13月是不对的
 *  @param[in] date 时间参数
 *  @return true 正常; false 异常
 */
bool C_TIME::CheckTime(const S_DATE &date)
{
	S_DATE myDate;
	LocalTime(MakeTime(date), myDate);

	if (date.m_Year != myDate.m_Year)
		return false;
	if (date.m_Mon != myDate.m_Mon)
		return false;
	if (date.m_Day != myDate.m_Day)
		return false;
	if (date.m_Hour != myDate.m_Hour)
		return false;
	if (date.m_Min != myDate.m_Min)
		return false;
	if (date.m_Sec != myDate.m_Sec)
		return false;
	return true;
}

/** @brief 超时类定义 */
C_MSTIMOUT::C_MSTIMOUT(S_DWORD ms)
{
	m_timeStart = C_TIME::TimeNow();
	m_timout = ms/1000 + ((ms%1000)? 1:0);
}

C_MSTIMOUT::~C_MSTIMOUT()
{
}

/** @brief 超时判断
 *  @return true 超时；false 未超时
 */
S_BOOL C_MSTIMOUT::timeout(void)
{
	S_TIME CurTime = C_TIME::TimeNow();

	if (CurTime < m_timeStart)
		m_timeStart = CurTime;
	return (CurTime - m_timeStart) > m_timout;
}


/* @brief 时钟节拍对象
 */
C_TimeTick::C_TimeTick(S_WORD span)
{
	m_Span = span;
	m_timePrev = 0x00;
}

C_TimeTick::C_TimeTick(void)
{
	m_Span = 0x00;
	m_timePrev = 0x00;
}

bool C_TimeTick::TickNow(void)
{
	S_TIME timeNow = C_TIME::TimeNow();
	if (m_timePrev == 0x00)
		;
	else if (timeNow - m_timePrev < m_Span)
		return false;

	m_timePrev = timeNow;
	return true;
}



