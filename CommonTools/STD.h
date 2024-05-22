#ifndef _STD_HEADER_
#define _STD_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <string>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#endif

enum FT_TEXTTYPE
{
	FT_ANSI = 0,
	FT_UTF8 = 1
};

class STD
{
public:
	static char *ltrim(char *str);
	static char *rtrim(char *str);
	static char *alltrim(char *str);
	static char *TrimEnter(char *str);
	static char *TrimChar(char *str, char c);

	static char *DeleteChar(char *str, char c) { return TrimChar(str, c); }
	static void DeleteCtrlCharFromGBK(char *gbk, char ch);

	static char *TrimSpaceAndLine(char *str);

	static char *GetValue_KeepBlank(const char *src, char *dest, int size, int num, char ch);
	static char *GetValue(const char *src, char *dest, int size, int num, char ch);
	static char *GetValue_KeepReturn(const char *src, char *dest, int size, int num, char ch);
	static char GetValueChar(const char *src, int num, char ch)
	{
		char t[10];
		GetValue(src, t, sizeof(t), num, ch);
		return t[0];
	}
	static int GetValueInt(const char *src, int num, char ch);
	static bool GetValueBool(const char *src, int num, char ch);

	static char *strcpyr(char *dest, const char *src, int num);

	static char *my_strncpy(char *dst, const char *src, int num, char end = '\0');

	template <int N>
	static char *my_strncpy(char (&dst)[N], const char *src)
	{
		return STD::my_strncpy(dst, src, N);
	}

	static long GetDate(int &year, int &month, int &day, time_t timevalue = 0);
	static long GetDate(time_t timevalue = 0);
	static long GetTime(int &hour, int &minute, int &second, time_t timevalue = 0);
	static long GetTime(time_t timevalue = 0);
	static void GetTime(int &year, int &month, int &day, int &hour, int &minute, int &second, time_t timevalue = 0);
	static void GetTime(int &date, int &time) { GetDateTime(date, time); }
	static void GetDateTime(int &date, int &time, time_t timevalue = 0);
	static char *GetTimeString(time_t timevalue, char *out, int len);

	// 获取当前时间，精确到毫秒
	static long GetCurrtimeMs();

	static long DateMinus(long date1, long date2 = 0);

	static long rand(int max_rand = 0x7fff);

	static char *MobilePhoneCharReplace(const char *src, char *dst);
	// static char*	MobilePhoneEncrypt(const char* src, char* dst);
	// 计算两数值，oper可为'+','-','/'，分别代表加、减、除，结果存放在Result中,
	// Dec是存放的小数点位数，为0时没有小数点，否则可为1,2,3
	static int Float(char *Float1, char *Float2, char oper, char *Result, int Dec);

	// 格式化浮点字符串，将其中的','去掉
	//  ex :999,999,999.9999->999999999.9999
	static double FormatMoneyDouble(const char *money);
	static char *FormatMoneyString(char *out, const char *money, int dec);

	static int FormatInt(const char *money);
	// 将value转换成string，dot位小数点
	static int DoubleToString(char *out, int out_size, double value, int dot);

	// 将value转换成放大 multi倍 的整型
	static long long DoubleToll(double value, int multi);

	static int days_of_year(int year, int mon, int day);
	// 两个日期之间相差的天数
	static long days_num(long startrq, long endrq);

	static int64_t FloatToInt(double f);
	static void GenerateDesKey(unsigned char *pszDes, unsigned int pszDesLen, unsigned int uLen);

	// 得到某日是第几个星期week*7+weekday(0-6)
	// 除以7的数为第几个星期
	// 除以7的余数为一个星期的星期几，0：为星期天，1：星期一,...
	// 得到某日是第几个星期week*7+weekday(0-6)
	static long getweek(long date);

	static long getnextdate(long rq);
	static long getpreviousrq(long rq);

	// check if it is a leap year
	static bool IsRunnian(long date);

	// 由date 经过 nSpan 天 返回计算后的日期YYYYMMDD
	//  nSpan>=0 向前增加 nSpan<0 向后减 -364<nSpan<365
	static int GetOtherDay(int date, int nSpan);

	// format : yyyymmdd-hhmmss
	static time_t GetSeconds(const char *times);

	// 区分大小写，假设在'|'或'='之前没有空格，如“JY = fjjj | ”是错误的
	// 从一个字符串中查找参数，格式：name1=1111|name2=2222|...
	static char *GetPara(const char *src, char *dst, int size, const char *para);
	static int GetPara(const char *src, const char *para);
	static void to_zhuangyi_string(const char *value, char *out, int size);
	static void Get_zhuanyi_string(const char *value, char *out, int size);
	static void TranslateData(const char *value, char *out, int size);

	// first = '\1' second = '='
	static char *GetPara(const char *src, char *dst, int size, const char *para, char first, char second);
	static int GetPara(const char *src, const char *para, char first, char second);

	static int32_t GetDateTimeFromStringByDiv(const char *str, char div)
	{
		if (strchr(str, div))
		{
			return STD::GetValueInt(str, 1, div) * 10000 + STD::GetValueInt(str, 2, div) * 100 + STD::GetValueInt(str, 3, div);
		}
		return 0;
	}

	static int32_t GetDateTimeFromString(const char *str)
	{
		if (strchr(str, '/'))
		{
			return STD::GetValueInt(str, 1, '/') * 10000 + STD::GetValueInt(str, 2, '/') * 100 + STD::GetValueInt(str, 3, '/');
		}
		else if (strchr(str, '\\'))
		{
			return STD::GetValueInt(str, 1, '\\') * 10000 + STD::GetValueInt(str, 2, '\\') * 100 + STD::GetValueInt(str, 3, '\\');
		}
		else if (strchr(str, '-'))
		{
			return STD::GetValueInt(str, 1, '-') * 10000 + STD::GetValueInt(str, 2, '-') * 100 + STD::GetValueInt(str, 3, '-');
		}
		else if (strchr(str, ':'))
		{
			return STD::GetValueInt(str, 1, ':') * 10000 + STD::GetValueInt(str, 2, ':') * 100 + STD::GetValueInt(str, 3, ':');
		}
		else if (strchr(str, '.'))
		{
			return STD::GetValueInt(str, 1, '.') * 10000 + STD::GetValueInt(str, 2, '.') * 100 + STD::GetValueInt(str, 3, '.');
		}
		else
		{
			return atoi(str);
		}
	}

	static char *FormatTime(char *out, int size, int32_t time)
	{
		FormatTimeAndReturnSize(out, size, time);
		return out;
	}

	static int FormatTimeAndReturnSize(char *out, int size, int32_t time)
	{
		if (time > 10000000)
		{
			return snprintf(out, size, "%02d:%02d:%02d", time / 10000000, time / 100000 % 100, time / 1000 % 100);
		}
		else if (time > 1000000 && time < 10000000)
		{
			return snprintf(out, size, "%02d:%02d:%02d", time / 100000, time / 10000 % 100, time / 100 % 100);
		}
		else if (time > 1000000)
		{
			return snprintf(out, size, "%02d:%02d:%02d", time / 100000, time / 1000 % 100, time / 10 % 100);
		}
		else
		{
			return snprintf(out, size, "%02d:%02d:%02d", time / 10000 % 100, time / 100 % 100, time % 100);
		}
	}

	static char *FormatTimeINT(char *out, int size, int32_t time)
	{
		if (time > 10000000)
		{
			snprintf(out, size, "%02d%02d%02d", time / 10000000, time / 100000 % 100, time / 1000 % 100);
		}
		else if (time > 1000000 && time < 10000000)
		{
			snprintf(out, size, "%02d%02d%02d", time / 100000, time / 10000 % 100, time / 100 % 100);
		}
		else if (time > 1000000)
		{
			snprintf(out, size, "%02d%02d%02d", time / 100000, time / 1000 % 100, time / 10 % 100);
		}
		else
		{
			snprintf(out, size, "%02d%02d%02d", time / 10000 % 100, time / 100 % 100, time % 100);
		}
		return out;
	}

	// 日期格式带符号的
	static char *FormatTime(char *out, int size, const char *str)
	{
		int32_t time = GetDateTimeFromString(str);
		return FormatTime(out, size, time);
	}

	// 时间格式化不带符号的
	static char *FormatTimeToString(char *out, int size, const char *str)
	{
		int32_t time = GetDateTimeFromString(str);
		return FormatTimeINT(out, size, time);
	}

	// 带GBK编码的字符串拷贝，防止截取半个中文字符的情况发生
	// 形参表的含义同strncpy函数
	// 不要使用本函数，参考CTP处理结算单的方式
	static int StrncpyGBK(char *pDst, int nDstLen, char *pSrc, int nSrcLen);

	static char *FormatDate(char *out, int size, int32_t date)
	{
		if (date != 0)
		{
			snprintf(out, size, "%04d-%02d-%02d", date / 10000, date / 100 % 100, date % 100);
		}
		else if (out && size > 0)
		{
			*out = 0;
		}
		return out;
	}

	static int FormateDateTimeAndReturnSize(char *out, int size, int32_t date, int32_t time = 0)
	{
		char time_str[32];
		FormatTimeAndReturnSize(time_str, sizeof(time_str), time);
		if (date != 0)
		{
			return snprintf(out, size, "%04d-%02d-%02d %s", date / 10000, date / 100 % 100, date % 100, time_str);
		}
		else if (out && size > 0)
		{
			*out = 0;
			return 0;
		}
		return 0;
	}

	static int FormatDateAndReturnSize(char *out, int size, int32_t date)
	{
		if (date != 0)
		{
			return snprintf(out, size, "%04d-%02d-%02d", date / 10000, date / 100 % 100, date % 100);
		}
		else if (out && size > 0)
		{
			*out = 0;
			return 0;
		}
		return 0;
	}

	static char *FormatDate(char *out, int size, const char *str)
	{
		int32_t date = GetDateTimeFromString(str);
		return FormatDate(out, size, date);
	}

	static int FormatDateAndReturnSize(char *out, int size, const char *str)
	{
		int32_t date = GetDateTimeFromString(str);
		return FormatDateAndReturnSize(out, size, date);
	}

	static void GetUTCDateTime(int &n_date, int &n_time);

	/**
	 * @brief URLEncode 对字符串URL编码
	 *
	 * @param str 原字符串(不限制一定是Ansi编码)
	 * @param strSize 原字符串长度(不包括最后的\0)
	 * @param result 结果缓冲区的地址
	 * @param resultSize 结果缓冲区的大小(包括最后的\0)
	 *
	 * @return: >0:resultstring 里实际有效的长度
	 *            0: 解码失败.
	 */
	static int AnsiToURLEncode(char *result, const int resultSize, const char *str, const int strSize);

	/// @brief 对输入串做 urlencode编码
	/// @param srcstr 输入串（utf-8编码）
	/// @return 输出urlencode的字符串
	static std::string UrlEncode(const std::string &srcstr);
	// BASE64
	//  Base64编码
	static char *Base64Filter(char *str); // 过滤掉错误字符
	static int Base2Chr(unsigned char n, char *out);
	static unsigned char Chr2Base(char c);
	static long Base64Decode(char *in, char *out, int out_len);

	static char *Base64Encode_WithAdd(char *out, int len, const char *str, int str_len);

	static int Base2Chr_WithAdd(unsigned char n, char *out);

	static bool IsGBK(unsigned char *p);
	static const char *ReadColumn(const char *psrc, char cCol, char *pdst, int idstlen);
	static const char *ChangeStingBy(char *dst, const char *src, int dst_len);

	static unsigned long GetTickCount();

	// 判断对应内容是否是UTF8
	static FT_TEXTTYPE CheckTextType(const char *data, int size);
	// 判断是否包含UTF8的BOM
	static bool isHaveBOM(const char *data, int size);

	/**delete一个指针，并给指针赋值为nullptr
	 *@param p 需要析构的指针
	 */
	template <class T>
	static void Free(T *&p);

	/**获取指定长度的utf8字符串，不能截断utf8字符
	 *@param src utf8源字符串
	 *@param src_len 源字符串的长度
	 *@param dest 目标缓冲区
	 *@param dest_size 目标缓冲区的最大长度
	 *
	 *@return  实际返回字符串长度，应该小于等于dest_size-1。dest有一个字节用来保存字符串结束符。
	 */
	static int get_utf8_words(const char *src, int src_len, char *dest, int dest_size);
};

template <class T>
void STD::Free(T *&p)
{
	delete p;
	p = nullptr;
}

#endif
