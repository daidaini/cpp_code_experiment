
#include "STD.h"
#include <sys/time.h>
#include <cstdarg>
#include <algorithm>
#include <fmt/format.h>

char *STD::ltrim(char *str)
{
	char *s = str;
	while (*s == ' ')
		s++;
	if (s != str)
		strcpy(str, s);
	return str;
}

char *STD::rtrim(char *str)
{
	char *s = str + strlen(str) - 1;
	while (s - str >= 0 && (*s == ' ' || *s == '\n'))
		s--;
	s++;
	*s = 0;
	return str;
}

char *STD::alltrim(char *str)
{
	ltrim(str);
	rtrim(str);
	return str;
}

char *STD::TrimSpaceAndLine(char *str)
{
	TrimEnter(str);
	return alltrim(str);
}

char *STD::strcpyr(char *dest, const char *src, int num)
{
	size_t len = strlen(src);
	int movelen = static_cast<int>(len - num);

	if (movelen > 0)
	{
		memcpy(dest, src + movelen, num);
		dest[num] = 0;
	}
	else
		strcpy(dest, src);

	return dest;
}

char *STD::my_strncpy(char *dst, const char *src, int num, char end)
{
	if (!src)
	{
		if (dst)
			*dst = 0;
		return dst;
	}
	const char *psrc = src;
	char *pdst = dst;

	num--;
	while (num > 0)
	{
		char c = *psrc;
		if (c == '\0' || c == end)
		{
			break;
		}
		*pdst = c;
		psrc++;
		pdst++;
		num--;
	}

	*pdst = 0;

	return dst;
}

unsigned long STD::GetTickCount()
{
#ifndef _WIN32
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#else
	return ::GetTickCount();
#endif
}

void STD::GenerateDesKey(unsigned char *pszDes, unsigned int pszDesLen, unsigned int uLen)
{
	const char pszDesKey[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	srand(GetTickCount() + static_cast<unsigned int>(time(NULL)));
	for (auto i = 0U; i < uLen; ++i)
	{
		if (i > pszDesLen - 1)
		{
			break;
		}

		int m = rand();
		int nTmp = (m) % 62;

		pszDes[i] = pszDesKey[nTmp];
	}

	pszDes[uLen] = 0;
}

char *STD::GetValue_KeepBlank(const char *src, char *dest, int size, int num, char ch) // 保存空格
{
	char *s0 = dest;

	if (ch == '\n')
	{
		while (*src != 0 && num != 1)
		{
			if (*src == ch)
				num--;
			src++;
		}

		while (*src != 0 && *src != ch && size > 1)
		{
			size--;
			*s0++ = *src++;
		}
	}
	else
	{
		while (*src != 0 && *src != '\n' && num != 1)
		{
			if (*src == ch)
				num--;
			src++;
		}

		while (*src != 0 && *src != '\n' && *src != ch && size > 1)
		{
			size--;
			*s0++ = *src++;
		}
	}
	*s0 = 0;

	return dest;
}

char *STD::GetValue_KeepReturn(const char *src, char *dest, int size, int num, char ch)
{
	char *s0 = dest;

	if (ch == '\n')
	{
		while (*src != 0 && num != 1)
		{
			if (*src == ch)
				num--;
			src++;
		}

		while (*src != 0 && *src != ch && size > 1)
		{
			size--;
			*s0++ = *src++;
		}
	}
	else
	{
		while (*src != 0 && *src && num != 1)
		{
			if (*src == ch)
				num--;
			src++;
		}

		while (*src != 0 && *src != ch && size > 1)
		{
			size--;
			*s0++ = *src++;
		}
	}
	*s0 = 0;

	return dest;
}

char *STD::GetValue(const char *src, char *dest, int size, int num, char ch)
{
	GetValue_KeepBlank(src, dest, size, num, ch);
	alltrim(dest);

	return dest;
}

int STD::GetValueInt(const char *src, int num, char ch)
{
	char dest[30];
	return atoi(STD::GetValue(src, dest, sizeof(dest), num, ch));
}

bool STD::GetValueBool(const char *src, int num, char ch)
{
	char dest[30];

	STD::GetValue(src, dest, sizeof(dest), num, ch);
	if (dest[0] == 0)
	{
		return false;
	}
	else if (strcmp(dest, "yes") == 0 || strcmp(dest, "true") == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

long STD::GetDate(int &year, int &month, int &day, time_t timevalue)
{
	if (timevalue == 0)
		timevalue = time(NULL);

	struct tm *newtime = localtime(&timevalue);

	year = newtime->tm_year + 1900;
	month = newtime->tm_mon + 1;
	day = newtime->tm_mday;
	return 10000L * year + 100L * month + day;
}

long STD::GetDate(time_t timevalue)
{
	int year, month, day;

	return GetDate(year, month, day, timevalue);
}

long STD::GetTime(int &hour, int &minute, int &second, time_t timevalue)
{
	if (timevalue == 0)
		timevalue = time(NULL);

	struct tm *newtime = localtime(&timevalue);

	if (newtime != NULL)
	{
		hour = newtime->tm_hour;
		minute = newtime->tm_min;
		second = newtime->tm_sec;
	}
	else
	{
		hour = 0;
		minute = 0;
		second = 0;
	}

	return 10000L * hour + 100L * minute + second;
}

long STD::GetTime(time_t timevalue)
{
	int hour, minute, second;

	return GetTime(hour, minute, second, timevalue);
}

void STD::GetTime(int &year, int &month, int &day, int &hour, int &minute, int &second, time_t timevalue)
{
	if (timevalue == 0)
		timevalue = time(NULL);

	struct tm *newtime = localtime(&timevalue);

	if (newtime != NULL)
	{
		year = newtime->tm_year + 1900;
		month = newtime->tm_mon + 1;
		day = newtime->tm_mday;
		hour = newtime->tm_hour;
		minute = newtime->tm_min;
		second = newtime->tm_sec;
	}
	else
	{
		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		second = 0;
	}
}

void STD::GetUTCDateTime(int &n_date, int &n_time)
{
	time_t timevalue = time(NULL);
	tm *newtime = gmtime(&timevalue);

	n_date = (newtime->tm_year + 1900) * 10000 + (newtime->tm_mon + 1) * 100 + newtime->tm_mday;
	n_time = newtime->tm_hour * 10000 + newtime->tm_min * 100 + newtime->tm_sec;
}

void STD::GetDateTime(int &p_date, int &p_time, time_t timevalue)
{
	if (timevalue == 0)
		timevalue = time(NULL);

	struct tm *newtime = localtime(&timevalue);

	if (newtime != NULL)
	{
		p_date = (newtime->tm_year + 1900) * 10000 + (newtime->tm_mon + 1) * 100 + newtime->tm_mday;
		p_time = newtime->tm_hour * 10000 + newtime->tm_min * 100 + newtime->tm_sec;
	}
	else
	{
		p_date = 0;
		p_time = 0;
	}
}

bool STD::IsRunnian(long date) // YYYYMMDD 判断是不是闰年
{
	if (((date % 4 == 0) && (date % 100 != 0)) || (date % 400 == 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int STD::GetOtherDay(int date, int nSpan)
{
	static int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	static int months1[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	int nPyear = date / 10000;
	int nPmonth = (date - nPyear * 10000) / 100;
	int nPday = date - nPyear * 10000 - nPmonth * 100;

	int nDyear = 0;
	int nDmonth = 0;
	int nDday = 0;

	int nstartdays = 0;
	int nenddays = 0;
	int i = 0;

	int nTemp = 0;

	if (IsRunnian(nPyear))
	{
		for (i = 0; i < nPmonth - 1; ++i)
		{
			nTemp += months1[i];
		}
	}
	else
	{
		for (i = 0; i < nPmonth - 1; ++i)
		{
			nTemp += months[i];
		}
	}

	// 这一年中此时的总天数
	nstartdays = nTemp + nPday;
	// 跨度后的天数
	nenddays = nstartdays + nSpan;

	i = 0;
	nTemp = 0;
	int ntempyear = nPyear;
	int *p = NULL;

	if (nenddays >= 0)
	{
		do
		{
			if (IsRunnian(ntempyear))
			{
				p = months1;
			}
			else
			{
				p = months;
			}
			if (nenddays < 31)
			{
				ntempyear -= 1;
				i = 11;
				break;
			}
			nTemp += *(p + i);
			i++;
			if (i == 12)
			{
				i = 0;
				ntempyear++;
			}

		} while (nenddays - nTemp > *(p + i));

		nDyear = ntempyear;
		nDmonth = i + 1;
		nDday = nenddays - nTemp;
	}
	else
	{
		ntempyear -= 1;
		i = 12;

		do
		{
			if (IsRunnian(ntempyear))
			{
				p = months1;
			}
			else
			{
				p = months;
			}
			i--;
			nTemp += *(p + i);
			if (i == 0)
			{
				i = 12;
				ntempyear -= 1;
			}

		} while (nTemp + nenddays < 0);

		nDyear = ntempyear;
		nDmonth = i + 1;
		nDday = nenddays + nTemp;
	}

	return nDyear * 10000 + nDmonth * 100 + nDday;
}

static int days_of_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
long getpreviousrq(long rq) // 取前一日期
{
	short year = (short)(rq / 10000l);
	short mon = (short)(rq % 10000);
	short day = (short)(mon % 100);
	mon = mon / 100;
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		days_of_mon[1] = 29;
	else
		days_of_mon[1] = 28;
	if (day > 1)
		return rq - 1l;
	if (mon > 1)
		return (long)year * 10000l + 100l * (mon - 1) + days_of_mon[mon - 2];
	return (long)(year - 1l) * 10000l + 1231l;
}

short days_of_year(short year, short mon, short day) // 某日是该年的第几天
{
	short i, days;
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		days_of_mon[1] = 29;
	else
		days_of_mon[1] = 28;
	days = 0;
	for (i = 0; i < mon - 1; i++)
		days += days_of_mon[i];
	days += day;
	return days;
}

long days_num(long startrq, long endrq)
{
	int sign = 1;
	if (startrq > endrq)
	{
		sign = -1;
		long temp = startrq;
		startrq = endrq;
		endrq = temp;
	}
	short startyear = (short)(startrq / 10000);
	short endyear = (short)(endrq / 10000);
	short mon, day;
	long days = 0L;
	mon = (short)((endrq % 10000) / 100);
	day = (short)((endrq % 10000) % 100);
	days += days_of_year(endyear, mon, day);

	mon = (short)((startrq % 10000) / 100);
	day = (short)((startrq % 10000) % 100);
	if (startyear == endyear)
	{
		days -= days_of_year(startyear, mon, day);
		days += 1;
		return days * sign;
	}
	else
	{
		if (startyear % 400 == 0 || (startyear % 4 == 0 && startyear % 100 != 0))
			days += 366 - days_of_year(startyear, mon, day);
		else
			days += 365 - days_of_year(startyear, mon, day);
		days += 1;
	}

	for (short i = startyear + 1; i < endyear; i++)
	{
		if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
			days += 366;
		else
			days += 365;
	}

	return days * sign;
}
long STD::DateMinus(long date1, long date2) // date1-date2，两个日期相减，格式为20061103，date2为0时取当前日期
{
	if (date2 == 0)
		date2 = GetDate(time(NULL));

	return days_num(date1, date2);
}

long STD::rand(int max_rand)
{
	if (max_rand <= 0)
	{
		return 0;
	}
	static long s = static_cast<long>(time(NULL));

	s = abs(s * 29 + 37 + clock()) % max_rand;

	return s;
}

char *STD::GetTimeString(time_t timevalue, char *out, int len)
{
	if (!out || len <= 0)
	{
		return NULL;
	}

	char buffer[100];

	if (timevalue > 0)
	{
		int year, month, day, hour, minute, second;

		STD::GetTime(year, month, day, hour, minute, second, timevalue);
		sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

		my_strncpy(out, buffer, len);
	}
	else
		*out = 0;

	return out;
}

long STD::GetCurrtimeMs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_usec * 1000;
}

// 计算两数值，oper可为'+','-','/'，分别代表加、减、除，结果存放在Result中，Dec是存放的小数点位数，为0时没有小数点，否则可为1,2,3
int STD::Float(char *Float1, char *Float2, char oper, char *Result, int Dec)
{
	double f1, f2;
	double f3;

	f1 = atof(Float1);
	f2 = atof(Float2);

	switch (oper)
	{
	case '+':
		f3 = f1 + f2;
		break;
	case '-':
		f3 = f1 - f2;
		break;
	case '/':
		if (f2 < 0.0005)
			f3 = 0;
		else
			f3 = f1 / f2;
		break;
	case '*':
		f3 = f1 * f2;
		break;
	default:
		f3 = 0;
		break;
	}

	sprintf(Result, "%.*f", Dec, f3);

	if (f3 < -0.0005)
		return -1;
	else if (f3 > 0.0005)
		return 1;
	else
		return 0;
}

char *STD::FormatMoneyString(char *out, const char *money, int dec)
{
	const char *in = money;
	char *result = out;

	while (1)
	{
		char c = *in;

		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result = c;
			}

			result++;
		}

		in++;
	}

	double f = atof(out);
	sprintf(out, "%.*f", dec, f);

	return out;
}

double STD::FormatMoneyDouble(const char *money)
{
	char out[100];
	const char *in = money;
	char *result = out;

	while (1)
	{
		char c = *in;

		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result = c;
			}

			result++;
		}

		in++;
	}

	return atof(out);
}

int STD::StrncpyGBK(char *pDst, int nDstLen, char *pSrc, int nSrcLen)
{
	int ret = nSrcLen;
	// pDst缓存足够容纳pSrc中的数据
	if (nSrcLen < nDstLen)
	{
		strncpy(pDst, pSrc, nSrcLen);
		*(pDst + nSrcLen) = 0;
		return ret;
	}

	ret = 0;
	for (nDstLen--; nDstLen > 0 && nSrcLen > 0;)
	{
		char c = *pSrc;
		if (c & 0x80)
		{
			if (nDstLen <= 1 || nSrcLen <= 1)
				break;

			*pDst = c;

			pSrc++;
			nSrcLen--;
			pDst++;
			nDstLen--;
			c = *pSrc;

			ret++;
		}

		*pDst = c;

		pSrc++;
		nSrcLen--;
		pDst++;
		nDstLen--;

		ret++;
	}

	*pDst = 0;

	return ret;
}

int STD::FormatInt(const char *money)
{
	char out[100];
	const char *in = money;
	char *result = out;

	while (1)
	{
		char c = *in;

		if (c == 0)
		{
			*result = 0;
			break;
		}
		else if (c != ',')
		{
			if (result != in)
			{
				*result = c;
			}

			result++;
		}

		in++;
	}

	return atoi(out);
}

int64_t STD::FloatToInt(double f)
{
	if (f < -0.00001)
	{
		return int64_t(f - 0.5);
	}
	else
	{
		return int64_t(f + 0.5);
	}
}

void STD::TranslateData(const char *value, char *out, int size)
{
	int len = 0;
	size--;
	for (; (*value) && len < size; value++)
	{
		char c = *value;
		switch (c)
		{
		case ':':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = ':';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		case ',':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = ',';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		case ';':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = ';';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
			continue;
		}
		out[len] = c;
		len++;
	}

	out[len] = 0;
}

void STD::to_zhuangyi_string(const char *value, char *out, int size)
{
	int len = 0;
	size--;
	for (; (*value) && len < size; value++)
	{
		char c = *value;
		switch (c)
		{
		case '\\':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = '\\';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		case '=':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = 'a';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		case '&':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = 'b';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		case '\n':
			if (len + 1 < size)
			{
				out[len] = '\\';
				len++;
				out[len] = 'n';
				len++;
			}
			else
			{
				out[len] = 0;
				return;
			}
			continue;
		}
		out[len] = c;
		len++;
	}

	out[len] = 0;
}

void STD::Get_zhuanyi_string(const char *value, char *out, int size)
{
	for (size--; size > 0; value++, size--, out++)
	{
		char c = *value;
		if (c == 0)
			break;
		if (c == '\\')
		{
			if (*(value + 1))
			{
				value++;
				char c1 = *value;
				if (c1 == '\\')
					c = '\\';
				else if (c1 == 'a')
					c = '=';
				else if (c1 == 'b')
					c = '&';
				else if (c1 == 'n')
					c = '\n';
			}
		}

		*out = c;
	}

	*out = 0;
}

long STD::getnextdate(long rq)
{
	char days_of_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int year = (rq / 10000l);
	int mon = (rq / 100 % 100);
	int day = (rq % 100);

	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
	{
		days_of_mon[1] = 29;
	}
	else
	{
		days_of_mon[1] = 28;
	}

	if (day < days_of_mon[mon - 1])
	{
		return rq + 1l;
	}
	else if (mon < 12)
	{
		return (long)year * 10000l + 100l * (mon + 1) + 1;
	}

	return (long)(year + 1) * 10000l + 101;
}

int STD::days_of_year(int year, int mon, int day) //  某日是该年的第几天
{
	char days_of_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	int i, days;
	if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
		days_of_mon[1] = 29;
	else
		days_of_mon[1] = 28;
	days = 0;
	for (i = 0; i < mon - 1; i++)
		days += days_of_mon[i];
	days += day;
	return days;
}

long STD::days_num(long startrq, long endrq)
{
	int sign = 1;
	if (startrq > endrq)
	{
		sign = -1;
		long temp = startrq;
		startrq = endrq;
		endrq = temp;
	}
	short startyear = (short)(startrq / 10000);
	short endyear = (short)(endrq / 10000);
	short mon, day;
	long days = 0L;
	mon = (short)((endrq % 10000) / 100);
	day = (short)((endrq % 10000) % 100);
	days += days_of_year(endyear, mon, day);

	mon = (short)((startrq % 10000) / 100);
	day = (short)((startrq % 10000) % 100);
	if (startyear == endyear)
	{
		days -= days_of_year(startyear, mon, day);
		days += 1;
		return days * sign;
	}
	else
	{
		if (startyear % 400 == 0 || (startyear % 4 == 0 && startyear % 100 != 0))
			days += 366 - days_of_year(startyear, mon, day);
		else
			days += 365 - days_of_year(startyear, mon, day);
		days += 1;
	}

	for (short i = startyear + 1; i < endyear; i++)
	{
		if (i % 400 == 0 || (i % 4 == 0 && i % 100 != 0))
			days += 366;
		else
			days += 365;
	}

	return days * sign;
}

long STD::getweek(long date) // 得到某日是第几个星期week*10+weekday(0-6)
{
	if (date < 19800101l)
		return 2; // 不能在1980年之前
	return days_num(19800101l, date) + 1;
}

// 格式yyyymmdd-hhmmss
time_t STD::GetSeconds(const char *times)
{
	int days = GetValueInt(times, 1, '-');
	int seconds = GetValueInt(times, 2, '-');
	struct tm t = {0};
	t.tm_year = days / 10000 - 1900;
	if (t.tm_year < 0)
	{
		t.tm_year = 0;
	}
	t.tm_mon = (days / 100) % 100 - 1;
	t.tm_mday = days % 100;

	t.tm_hour = seconds / 10000;
	t.tm_min = (seconds / 100) % 100;
	t.tm_sec = seconds % 100;

	time_t ttime = mktime(&t);
	return ttime;
}

char *STD::GetPara(const char *src, char *dst, int size, const char *para)
{
	return GetPara(src, dst, size, para, '|', '=');
}

int STD::GetPara(const char *src, const char *para)
{
	char data[50];
	GetPara(src, data, sizeof(data), para);
	return atoi(data);
}

// first = '\1' second = '='
// 从一个字符串中查找参数，格式：name1=1111|name2=2222|...
char *STD::GetPara(const char *src, char *dst, int size, const char *para, char first, char second)
{
	*dst = 0;

	int len = static_cast<int>(strlen(para));
	int destlen = size;
	const char *p = NULL;
	if (strncmp(src, para, len) == 0 && src[len] == second)
	{
		p = src;
		len++;
	}
	else
	{
		char key[100];
		len = snprintf(key, sizeof(key), "%c%s%c", first, para, second);

		p = strstr(src, key); // 假设全为小写
	}
	if (p)
	{
		p += len;

		char *pdst = dst;
		size--;
		while (size > 0)
		{
			char c = *p;
			if (c == 0 || c == first)
			{
				break;
			}
			*pdst = *p;
			pdst++;
			p++;
			size--;
		}
		*pdst = 0;
	}

	// 转义
	Get_zhuanyi_string(dst, dst, destlen);

	return dst;
}

int STD::GetPara(const char *src, const char *para, char first, char second)
{
	char data[50];
	GetPara(src, data, sizeof(data), para, first, second);
	return atoi(data);
}

char *STD::TrimEnter(char *str)
{
	if (str == NULL)
		return NULL;
	char *ptr = strchr(str, '\xa');
	if (ptr)
		*ptr = 0;
	ptr = strchr(str, '\xd');
	if (ptr)
		*ptr = 0;
	return ptr;
}

char *STD::TrimChar(char *str, char c)
{
	char *dst = str;

	for (char *p = str; *p; p++)
	{
		if (*p == c)
		{
			continue;
		}

		if (dst != p)
		{
			*dst = *p;
		}

		dst++;
	}

	*dst = 0;

	return str;
}

void STD::DeleteCtrlCharFromGBK(char *gbk, char ch)
{
	if (nullptr == gbk)
	{
		return;
	}

	char *target = gbk;
	char *source = gbk;
	while (*source)
	{
		if (*source & 0x80)
		{
			// 汉字保留，双字节
			*target++ = *source++;
			if (*source == '\0')
			{
				break;
			}
			*target++ = *source++;
		}
		else if (*source == ch)
		{
			// 跳过ch字符
			source++;
		}
		else
		{
			// 其他ASCII码保留
			*target++ = *source++;
		}
	}
	*target = '\0';
}

char *STD::MobilePhoneCharReplace(const char *src, char *dst)
{
	strcpy(dst, src);
	if (*src == '1' && strlen(src) >= 7)
	{
		memset(dst + 3, '*', 4);
	}
	return dst;
}

bool STD::IsGBK(unsigned char *p) // 有些汉字的半个字符为'|'
{
	return (p[0] >= 0x81U && p[0] <= 0xFEU && p[1] >= 0x40U && p[1] <= 0xFEU);
}

const char *STD::ReadColumn(const char *psrc, char cCol, char *pdst, int idstlen)
{
	for (int i = 0;; ++i)
	{
		if (psrc[i] == cCol || psrc[i] == 0)
		{
			int j = std::min(i, idstlen - 1) - 1;
			for (; j >= 0 && pdst[j] == ' '; --j)
			{
			}
			pdst[j + 1] = 0;
			int k = 0;
			for (; pdst[k] == ' '; ++k)
			{
			}
			if (k != 0)
			{
				memmove(pdst, pdst + k, j + 1 - k + 1);
			}
			return psrc + i + (psrc[i] == 0 ? 0 : 1);
		}
		if (i < idstlen - 1)
			pdst[i] = psrc[i];
		if (IsGBK((unsigned char *)psrc + i))
		{
			++i;
			if (i < idstlen - 1)
				pdst[i] = psrc[i];
		}
	}
	return NULL;
}

// BASE64
int STD::Base2Chr(unsigned char n, char *out)
{
	n &= 0x3F;
	if (n < 26)
	{
		*out = (char)(n + 'A');
		return 1;
	}
	else if (n < 52)
	{
		*out = (char)(n - 26 + 'a');
		return 1;
	}
	else if (n < 62)
	{
		*out = (char)(n - 52 + '0');
		return 1;
	}
	else if (n == 62)
	{
		*out++ = '%';
		*out++ = '2';
		*out = 'B';
		return 3;
	}
	else
	{
		*out = '/';
		return 1;
	}
}

char *STD::Base64Filter(char *str) // 过滤掉错误字符
{
	if (str == NULL)
	{
		return NULL;
	}
	char *src = str;
	char *dst = str;
	for (char c = *src; c != 0;)
	{
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '%') || (c == '/') || (c == '+') || (c == '='))
		{
			*dst = c;
			dst++;
		}
		src++;
		c = *src;
	}
	return str;
}

//---------------------------------------------------------------------------
unsigned char STD::Chr2Base(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (unsigned char)(c - 'A');
	else if (c >= 'a' && c <= 'z')
		return (unsigned char)(c - 'a' + 26);
	else if (c >= '0' && c <= '9')
		return (unsigned char)(c - '0' + 52);
	else if (c == '+')
		return 62;
	else
		return 63;
}

long STD::Base64Decode(char *in, char *out, int out_len)
{
	Base64Filter(in);

	*out = 0;
	unsigned long x, y, z;
	unsigned long i, j;
	unsigned char bufa[4];
	unsigned char bufb[3];

	out_len--;

	unsigned long len = static_cast<unsigned long>(strlen(in));

	if (len < 4)
	{
		*out = 0;
		return 0;
	}

	x = (len - 4) / 4;
	i = 0;
	j = 0;
	for (z = 0; z < x; z++)
	{
		for (y = 0; y < 4; y++)
		{
			bufa[y] = Chr2Base(in[j + y]);
		} /* end of for */

		if (int(i + 3) > out_len)
		{
			break;
		}
		out[i] = bufa[0] << 2 | (bufa[1] & 0x30) >> 4;
		out[i + 1] = (bufa[1] & 0x0F) << 4 | (bufa[2] & 0x3C) >> 2;
		out[i + 2] = (bufa[2] & 0x03) << 6 | (bufa[3] & 0x3F);
		i += 3;
		j += 4;
	} /* end of for */
	for (z = 0; z < 4; z++)
	{
		bufa[z] = Chr2Base(in[j + z]);
	} /* end of for */

	// 编码算法确保了结尾最多有两个'='
	if ('=' == in[len - 2])
	{
		y = 2;
	}
	else if ('=' == in[len - 1])
	{
		y = 1;
	}
	else
	{
		y = 0;
	}
	/*
	 * BASE64算法所需填充字节个数是自识别的
	 */
	for (z = 0; z < y; z++)
	{
		bufa[4 - z - 1] = 0x00;
	} /* end of for */
	bufb[0] = bufa[0] << 2 | (bufa[1] & 0x30) >> 4;
	bufb[1] = (bufa[1] & 0x0F) << 4 | (bufa[2] & 0x3C) >> 2;
	bufb[2] = (bufa[2] & 0x03) << 6 | (bufa[3] & 0x3F);
	/*
	 * y必然小于3
	 */
	if (int(i + 3 - y) <= out_len)
	{
		for (z = 0; z < 3 - y; z++)
		{
			out[i + z] = bufb[z];
		} /* end of for */
		/*
		 * 离开for循环的时候已经z++了
		 */
		i += z;
	}
	out[i] = 0;
	return (i);
}

int STD::Base2Chr_WithAdd(unsigned char n, char *out)
{
	n &= 0x3F;
	if (n < 26)
	{
		*out = (char)(n + 'A');
		return 1;
	}
	else if (n < 52)
	{
		*out = (char)(n - 26 + 'a');
		return 1;
	}
	else if (n < 62)
	{
		*out = (char)(n - 52 + '0');
		return 1;
	}
	else if (n == 62)
	{
		*out = '+';
		return 1;
	}
	else
	{
		*out = '/';
		return 1;
	}
}

char *STD::Base64Encode_WithAdd(char *out, int len, const char *str, int str_len)
{
	unsigned char c, t = 0;

	int pos = 0;
	int n = str_len;
	len--;

	for (int i = 0; i < n; i++)
	{
		c = str[i];
		if (i % 3 == 0)
		{
			if (pos + 4 >= len)
			{
				break;
			}
			pos += Base2Chr_WithAdd(c >> 2, out + pos);
			t = (c << 4) & 0x3F;
		}
		else if (i % 3 == 1)
		{
			if (pos + 4 >= len)
			{
				break;
			}
			pos += Base2Chr_WithAdd(t | (c >> 4), out + pos);
			t = (c << 2) & 0x3F;
		}
		else
		{
			if (pos + 6 >= len)
			{
				break;
			}
			pos += Base2Chr_WithAdd(t | (c >> 6), out + pos);
			pos += Base2Chr_WithAdd(c & 0x3F, out + pos);
		}
	}
	if (n % 3 != 0 && pos + 5 < len)
	{
		pos += Base2Chr_WithAdd(t, out + pos);
		out[pos] = '=';
		pos++;
		if (n % 3 == 1)
		{
			out[pos] = '=';
			pos++;
		}
	}
	out[pos] = 0;
	return out;
}

// 将value转换成string，dot位小数点
int STD::DoubleToString(char *out, int out_size, double value, int dot)
{
	int size;
	if (value < -0.000000000000001)
		size = snprintf(out, out_size, "%.*f", dot, value - 0.000000000000001);
	else if (value > -0.000000000000001)
		size = snprintf(out, out_size, "%.*f", dot, value + 0.000000000000001);
	else
		size = snprintf(out, out_size, "%.*f", dot, value);

	return size;
}

long long STD::DoubleToll(double value, int multi)
{
	if (value < -0.000000000000001)
		return static_cast<long long>((value - 0.000000000000001) * multi);
	else if (value > -0.000000000000001)
		return static_cast<long long>((value + 0.000000000000001) * multi);
	else
		return static_cast<long long>((value)*multi);
}

FT_TEXTTYPE STD::CheckTextType(const char *data, int size)
{
	while (size > 0)
	{
		char c = *data;
		if (!(c & 0x80)) // 0xxx xxxx
		{
			data++;
			size--;
		}
		else if ((c & 0xe0) == 0xc0) // 110x xxxx
		{
			// 表示两个字节
			if (size >= 2 && (data[1] & 0xc0) == 0x80) // 10xx xxxx
			{
				data += 2;
				size -= 2;
			}
			else
			{
				return FT_ANSI;
			}
		}
		else if ((c & 0xf0) == 0xe0) // 1110 xxxx
		{
			// 表示三个字节
			if (size >= 3 && ((data[1] & 0xc0) == 0x80) // 10xx xxxx
				&& ((data[2] & 0xc0) == 0x80))
			{
				data += 3;
				size -= 3;
			}
			else
			{
				return FT_ANSI;
			}
		}
		else if ((c & 0xf8) == 0xf0) // 1111 0xxx
		{
			// 表示四个字节
			if (size >= 4 && ((data[1] & 0xc0) == 0x80) // 10xx xxxx
				&& ((data[2] & 0xc0) == 0x80) && ((data[3] & 0xc0) == 0x80))
			{
				data += 4;
				size -= 4;
			}
			else
			{
				return FT_ANSI;
			}
		}
		else if ((c & 0xfc) == 0xf8) // 1111 10xx
		{
			// 表示五个字节
			if (size >= 5 && ((data[1] & 0xc0) == 0x80) // 10xx xxxx
				&& ((data[2] & 0xc0) == 0x80) && ((data[3] & 0xc0) == 0x80) && ((data[4] & 0xc0) == 0x80))
			{
				data += 5;
				size -= 5;
			}
			else
			{
				return FT_ANSI;
			}
		}
		else if ((c & 0xfe) == 0xfc) // 1111 110x
		{
			// 表示六个字节
			if (size >= 6 && ((data[1] & 0xc0) == 0x80) // 10xx xxxx
				&& ((data[2] & 0xc0) == 0x80) && ((data[3] & 0xc0) == 0x80) && ((data[4] & 0xc0) == 0x80) && ((data[5] & 0xc0) == 0x80))
			{
				data += 6;
				size -= 6;
			}
			else
			{
				return FT_ANSI;
			}
		}
		else
		{
			return FT_ANSI;
		}
	}

	return FT_UTF8;
}

bool STD::isHaveBOM(const char *data, int size)
{
	if (size >= 3)
	{
		const int value = *(const int *)data;
		if ((value & 0x00ffffff) == 0xbfbbef)
		{
			return true;
		}
	}

	return false;
}

int STD::AnsiToURLEncode(char *result, const int resultSize, const char *str, const int strSize)
{
	int i;
	int j = 0; // for result index
	char ch;

	if ((str == NULL) || (result == NULL) || (strSize <= 0) || (resultSize <= 0))
	{
		return 0;
	}

	for (i = 0; (i < strSize) && (j < resultSize); ++i)
	{
		ch = str[i];
		if (((ch >= 'A') && (ch <= 'Z')) ||
			((ch >= 'a') && (ch <= 'z')) ||
			((ch >= '0') && (ch <= '9')))
		{
			result[j++] = ch;
		}
		else if (ch == ' ')
		{
			result[j++] = '+';
		}
		else if (ch == '.' || ch == '-' || ch == '_' || ch == '*')
		{
			result[j++] = ch;
		}
		else
		{
			if (j + 3 < resultSize)
			{
				sprintf(result + j, "%%%02X", (unsigned char)ch);
				j += 3;
			}
			else
			{
				return 0;
			}
		}
	}
	result[j] = '\0';
	return j;
}

std::string STD::UrlEncode(const std::string &srcstr)
{
	std::string encoded;

	for (char c : srcstr)
	{
		if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			encoded.push_back(c);
		}
		else if (c == ' ')
		{
			encoded.push_back('+');
		}
		else
		{
			fmt::format_to(std::back_inserter(encoded), "%{:02X}", static_cast<unsigned int>(static_cast<unsigned char>(c)));
		}
	}

	return encoded;
}

const char *STD::ChangeStingBy(char *dst, const char *src, int dst_len)
{
	const char *psrc = src;
	char *pdst = dst;
	int num = static_cast<int>(strlen(psrc)) + 1;
	num--;
	int dst_num = 0;
	while (num > 0)
	{
		char c = *psrc;
		if (c == '\0')
		{
			//*pdst	= '\0';
			break;
		}
		if (c == '|')
		{
			dst_num += 3;
			if (dst_num >= dst_len)
			{
				break;
			}
			*pdst = '&';
			pdst++;
			*pdst = '1';
			pdst++;
			*pdst = ';';
			psrc++;
			pdst++;
			num--;
		}
		else if (c == '&')
		{
			dst_num += 3;
			if (dst_num >= dst_len)
			{
				break;
			}
			*pdst = '&';
			pdst++;
			*pdst = '2';
			pdst++;
			*pdst = ';';
			psrc++;
			pdst++;
			num--;
		}
		else
		{
			dst_num++;
			if (dst_num >= dst_len)
			{
				break;
			}
			*pdst = c;
			psrc++;
			pdst++;
			num--;
		}
	}
	*pdst = 0;
	return dst;
}

int STD::get_utf8_words(const char *src, int src_len, char *dest, int dest_size)
{
	if (src == nullptr || dest == nullptr)
		return 0;

	auto utf8_word_size = [](char first) -> int
	{
		if ((first & 0x80) == 0)
			return 1;

		if ((first & 0xF8) == 0xF0)
			return 4;

		if ((first & 0xF0) == 0xE0)
			return 3;

		if ((first & 0xE0) == 0xC0)
			return 2;

		// 正常情况下，是1-4个字符。如果不是1-4个字符，属于异常情况，就认为是一个字符。
		return 1;
	};

	int len = 0;
	while (len < src_len)
	{
		auto word_size = utf8_word_size(src[len]);
		if (len + word_size >= dest_size)
			break;

		::memcpy(dest + len, src + len, word_size);
		len += word_size;
	}

	dest[len] = '\0';
	return len;
}
