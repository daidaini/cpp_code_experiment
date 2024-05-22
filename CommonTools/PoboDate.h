#pragma once

#include <string>
#include <cstring>
#include <time.h>

namespace pobo
{
    struct Date
    {
        //年
        int m_year;
        //月
        int m_month;
        //日
        int m_day;

    private:
        //当前日期的天数
        int m_days;

    public:
        Date();
        /**构造函数
         *@param date 字符串日期，格式：YYYYMMDD，不对输入参数做任何检查
         */
        Date(const char *date);

        /**构造函数，不对输入参数做任何检查
         *@param year 年
         *@param month 月
         *@param day 日
         */
        Date(int year, int month, int day);

        /**复制构造函数
         */
        Date(const Date &date);

        /**赋值构造函数
         */
        Date &operator=(const Date &date);

        /**转化为字符串
         *@param str 输出参数，格式：YYYYMMDD
         */
        char *to_string(char *str) const;

        std::string to_string() const;

        /**判断是否闰年
         */
        bool is_leap_year() const;

        /**当年天数
         */
        int this_year_days() const;

        /**大于比较符
         */
        bool operator>(const Date &other) const;

        /**大于等于比较符
         */
        bool operator>=(const Date &other) const;

        /**小于比较符
         */
        bool operator<(const Date &other) const;

        /**小于等于比较符
         */
        bool operator<=(const Date &other) const;

        /**等于比较符
         */
        bool operator==(const Date &other) const;

        /**获取两个日期之间的相差天数
         */
        int operator-(const Date &other) const;

        /**减去天数操作
         */
        Date operator-(int days) const;

        /**减赋值天数操作
         */
        Date &operator-=(int days);

        /**加上天数的操作
         */
        Date operator+(int days) const;

        /**加赋值天数的操作
         */
        Date &operator+=(int days);

    private:
        /*转化为天数
         */
        int to_days() const;

        /**从天数获取
         */
        void from_days(int days);
    };

    inline Date::Date()
    {
        time_t now = time(NULL);
        struct tm *newtime = localtime(&now);

        m_year = newtime->tm_year + 1900;
        m_month = newtime->tm_mon + 1;
        m_day = newtime->tm_mday;

        m_days = to_days();
    }

    inline Date::Date(const char *date)
    {
        char temp[9];
        strncpy(temp, date, 9);

        temp[8] = '\0';
        m_day = atoi(temp + 6);

        temp[6] = '\0';
        m_month = atoi(temp + 4);

        temp[4] = '\0';
        m_year = atoi(temp);

        m_days = to_days();
    }

    inline Date::Date(int year, int month, int day)
        : m_year(year), m_month(month), m_day(day)
    {
        m_days = to_days();
    }

    inline Date::Date(const Date &date)
    {
        m_year = date.m_year;
        m_month = date.m_month;
        m_day = date.m_day;
        m_days = date.m_days;
    }

    inline Date &Date::operator=(const Date &date)
    {
        m_year = date.m_year;
        m_month = date.m_month;
        m_day = date.m_day;
        m_days = to_days();

        return *this;
    }

    inline char *Date::to_string(char *str) const
    {
        if (str)
            sprintf(str, "%04d%02d%02d", m_year, m_month, m_day);

        return str;
    }

    std::string Date::to_string() const
    {
        char str[16]{};
        snprintf(str, sizeof(str), "%04d%02d%02d", m_year, m_month, m_day);
        return str;
    }

    inline bool Date::is_leap_year() const
    {
        return m_year % 4 == 0 && (m_year % 100 != 0 || m_year % 400 == 0);
    }

    inline int Date::this_year_days() const
    {
        return 365 + (int)this->is_leap_year();
    }

    inline bool Date::operator>(const Date &other) const
    {
        return m_days > other.m_days;
    }

    inline bool Date::operator>=(const Date &other) const
    {
        return m_days >= other.m_days;
    }

    inline bool Date::operator<(const Date &other) const
    {
        return m_days < other.m_days;
    }

    inline bool Date::operator<=(const Date &other) const
    {
        return m_days <= other.m_days;
    }

    inline bool Date::operator==(const Date &other) const
    {
        return m_days == other.m_days;
    }

    inline int Date::operator-(const Date &other) const
    {
        return m_days - other.m_days;
    }

    inline Date Date::operator-(int days) const
    {
        Date tmp(*this);
        tmp.from_days(tmp.m_days - days);
        return tmp;
    }

    inline Date &Date::operator-=(int days)
    {
        from_days(m_days - days);
        return *this;
    }

    inline Date Date::operator+(int days) const
    {
        Date tmp(*this);
        tmp.from_days(tmp.m_days + days);
        return tmp;
    }

    inline Date &Date::operator+=(int days)
    {
        from_days(m_days + days);
        return *this;
    }

    inline int Date::to_days() const
    {
        int y, m;

        m = (m_month + 9) % 12; /* mar=0, feb=11 */
        y = m_year - m / 10;    /* if Jan/Feb, year-- */
        return y * 365 + y / 4 - y / 100 + y / 400 + (m * 306 + 5) / 10 + (m_day - 1);
    }

    inline void Date::from_days(int days)
    {
        m_days = days;

        long y, ddd, mi;

        //使用longlong来计算，否则会溢出
        y = ((long long)10000 * days + 14780) / 3652425;
        ddd = days - (y * 365 + y / 4 - y / 100 + y / 400);
        if (ddd < 0)
        {
            y--;
            ddd = days - (y * 365 + y / 4 - y / 100 + y / 400);
        }
        mi = (52 + 100 * ddd) / 3060;
        m_year = y + (mi + 2) / 12;
        m_month = (mi + 2) % 12 + 1;
        m_day = ddd - (mi * 306 + 5) / 10 + 1;
    }
}