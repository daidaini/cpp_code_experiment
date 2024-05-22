#include <iostream>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace boost::gregorian;

void test_date()
{
    date d1(2001, 1, 1);
    date d2(2002, Feb, 2);
    //date d3 = from_string("2003-3-3");
    boost::gregorian::date d3 = from_simple_string("2003-3-3");
    date d4 = from_string("2004/4/4");
    date d5 = from_undelimited_string("20050505");

    std::cout << "本地日期：" << day_clock::local_day() << std::endl
         << "UTC日期：" << day_clock::universal_day() << std::endl
         << std::endl;

    std::cout << "负无限日期：" << neg_infin << std::endl
         << "正无限日期：" << pos_infin << std::endl
         << "无效日期：" << not_a_date_time << std::endl
         << "最大可能日期：" << max_date_time << std::endl
         << "最小可能日期：" << min_date_time << std::endl
         << std::endl;

    std::cout << "对于2001-01-01：" << std::endl
         << "年：" << d1.year() << std::endl
         << "月：" << d1.month() << std::endl
         << "日：" << d1.day() << std::endl
         << std::endl;

    std::cout << "对于2002-02-02：" << std::endl
         << "星期数：（0表示星期天）" << d2.day_of_week() << std::endl
         << "当年的第几天：" << d2.day_of_year() << std::endl
         << "当年的第几周：" << d2.week_number() << std::endl
         << "当月的最后一天：" << d2.end_of_month() << std::endl
         << std::endl;

    std::cout << "对于2003-03-03：" << std::endl
         << "直接打印日期：" << d3 << std::endl
         << "使用to_simple_string打印日期：" << to_simple_string(d3) << std::endl
         << "使用to_iso_string打印日期：" << to_iso_string(d3) << std::endl
         << "使用to_iso_extended_string打印日期：" << to_iso_extended_string(d3) << std::endl
         << std::endl;
}

void test_date_trans()
{
    // 日期格式化输出
    date date_a = from_string("2021-01-25");

    std::cout << "转为英文格式: " << to_simple_string(date_a) << std::endl;
    std::cout << "转为纯数字格式: " << to_iso_string(date_a) << std::endl;
    std::cout << "转为通用格式: " << to_iso_extended_string(date_a) << std::endl;

    // 日期/时间/月份 差值计算
    days day_a(10), day_b(100), day_c(-50);
    std::cout << "day_b 与 day_a 相差: " << day_b - day_a << " 天" << std::endl;
    std::cout << "相加是否大于100: " << ((day_a + day_b).days() > 100) << std::endl;

    weeks week_a(3), week_b(4);
    std::cout << "三个星期: " << week_a << " 天" << std::endl;
    std::cout << "四个星期等于28: " << (week_b.days() == 28) << std::endl;

    // 日期混合相加
    years year(2);   // 2年
    months month(5); // 5个月

    months item = year + month;
    std::cout << "总计: " << item.number_of_months() << " 个月" << std::endl;
    std::cout << "总计: " << (year).number_of_years() << " 年 "
              << "零 " << month.number_of_months() << " 个月" << std::endl;

    // 日期的运算
    date date_b(2019, 01, 01), date_c(2020, 01, 01);
    std::cout << "日期相差: " << date_c - date_b << " 天" << std::endl;

    date_b += months(12);
    std::cout << "加12个月后: " << date_b << std::endl;

    date_b -= days(100);
    std::cout << "减100天后: " << date_b << std::endl;
}


int main(int argc, char const *argv[])
{
    test_date();

    getchar();
    return 0;
}