#include <boost/utility/string_ref.hpp>
#include <boost/utility/string_view.hpp>

#include "../../CommonTools/TimerLog.h"

#include <fmt/format.h>

#include <string>
#include <cassert>
#include <vector>
using namespace boost;

typedef boost::string_ref str_ref;
typedef boost::string_view str_view;

std::vector<std::string> Split(const std::string &src, char separator)
{
    std::vector<std::string> result;

    size_t pos = 0;
    do
    {
        auto separatePos = src.find_first_of(separator, pos);
        if (separatePos == src.npos)
        {
            auto last = src.substr(pos);
            if (!last.empty())
                result.emplace_back(std::move(last));

            break;
        }

        result.emplace_back(src.substr(pos, separatePos - pos));
        pos = separatePos + 1;
    } while (true);

    return result;
}

std::vector<std::string> Split_Ref(str_ref src, char separator)
{
    std::vector<std::string> result;
    str_ref iter(src);
    do
    {
        auto separatePos = iter.find_first_of(separator);
        if (separatePos == iter.npos)
        {
            auto last = iter.substr(0);
            if (!last.empty())
                result.emplace_back(last);

            break;
        }

        if (separatePos != 0)
        {
            result.emplace_back(std::string(iter.data(), separatePos));
        }

        iter = iter.substr(separatePos + 1);

    } while (true);

    return result;
}

std::vector<std::string> Split_View(str_view src, char separator)
{
    std::vector<std::string> result;
    do
    {
        auto separatePos = src.find_first_of(separator);
        if (separatePos == src.npos)
        {
            auto last = src.substr(0);
            if (!last.empty())
                result.emplace_back(last);

            break;
        }

        if (separatePos != 0)
        {
            result.emplace_back(std::string(src.data(), separatePos));
        }

        src = src.substr(separatePos + 1);

    } while (true);

    return result;
}

void TestFunction()
{
    std::string src{
        "abcdef|1234567|ghijkim|987654|.,.,.,.|opqrst"};

    size_t index = src.find_first_of('|');

    str_ref sref(src.data() + index, src.size() - index);

    fmt::print("string ref = {}\n", sref.data());

    // std::string str(sref.data(), sref.size());

    auto spliteds = Split_Ref(src.data(), '|');
    for (auto &item : spliteds)
    {
        fmt::print("{}\n", item.data());
    }
}

std::string g_src = "abcdef|1234567|ghijkim|987654|.,.,.,.|opqrst";

void TestEfficiency_string_ref()
{
    TimerLog t("string ref");

    for (int i = 0; i < 5000; ++i)
    {
        std::string src = g_src;

        auto results = Split_Ref(src, '|');

        assert(results.size() == 6);
    }
}

void TestEfficiency_string()
{
    TimerLog t("string");

    for (int i = 0; i < 5000; ++i)
    {
        std::string src = g_src;
        // std::string src = "abcdef|1234567|ghijkim|987654|.,.,.,.|opqrst";

        auto results = Split(src, '|');

        assert(results.size() == 6);
    }
}

void TestEfficiency_string_view()
{
    TimerLog t("string view");

    for (int i = 0; i < 5000; ++i)
    {
        std::string src = g_src;
        // std::string src = "abcdef|1234567|ghijkim|987654|.,.,.,.|opqrst";

        auto results = Split_View(src, '|');

        assert(results.size() == 6);

        /*
        for (auto &item : results)
        {
            fmt::print("{}\n", item.data());
        }
        */
    }
}

int main()
{
    TestEfficiency_string_view();
    TestEfficiency_string();

    TestEfficiency_string_ref();

    return 0;
}