/*
 * @Descripttion: 
 * @Author: yubo
 * @Date: 2022-10-13 15:54:38
 * @LastEditTime: 2022-10-21 14:00:00
 */

#include <fmt/format.h>

#include <deque>
#include <algorithm>
#include <vector>
#include <boost/algorithm/string.hpp>
using namespace boost;

//ends_with  to_upper_copy  replace_first
void Test_1()
{
    std::string str("readme.txt");

    if(ends_with(str, "txt"))
    {
        fmt::print("{} UPPER\n", to_upper_copy(str));
        assert(ends_with(str, "txt"));
    }

    replace_first(str, "readme", "followme");
    fmt::print("{}\n", str);
}

//split
void Test_Split()
{
    std::string src("1=a&2=aa&3=aaa");

    std::vector<std::string> result;
    algorithm::split(result, std::move(src), [](char elem)
                     { return elem == '&'; });

    for(auto& item: result)
    {
        fmt::print("{}\n", item);
    }
    fmt::print("----------------------------------\n");
    src = "Samus, Link. Zelda:: Mario-Luigi+zelda";
    split(result, src, is_any_of(",.:+-")); //任意符号分割
    for(auto& item: result)
    {
        trim(item);
        fmt::print("{}\n", item);
    }
}

//find_all
void Test_FindAll()
{
    std::string src{"Samus, Link. Zelda:: Mario-Luigi+zelda"};

    std::deque<std::string> d;
    find_all(d, src, "ZelDa");
    if(d.empty())
    {
        fmt::print("find_all fialed... use ifind\n");
        ifind_all(d, src, "ZelDa"); //无视大小写
        if(!d.empty())
        {
            fmt::print("output: \n");
            for (auto &item : d)
            {
                fmt::print("{}\n", item);
            }
        }
    }
}

//replace erase
void Test_ReplaceAndErase()
{
    std::string src = "Samus beat the monster. Is it really for Samus?\n";

    //replace_first_copy 不改变原值
    fmt::print("After replace_first_copy : {}\n", replace_first_copy(src, "beat", "beats"));

    //replace_first 改变原值
    algorithm::replace_first(src, "beat", "beated");
    fmt::print("After replace : {}\n", src);

    fmt::print("After replace_last_copy : {}\n", algorithm::replace_last_copy(src, "Samus", "he"));

    fmt::print("After ierase_all_copy : {}\n", ierase_all_copy(src, "SamUs"));

    erase_last(src, "for");
    fmt::print("After erase_last: {}\n", src);
}

void Test_Join()
{
    std::vector<std::string> src{
        "I", "am", "a", "rap", "Star", "Thanks"};

    fmt::print("After Jonin : {}\n", algorithm::join(src, " "));

    fmt::print("After Jonin_if : {}\n", join_if(src, "--", [](const std::string &s)
                                                { return contains(s, "a"); }));
}

void Test_Trim()
{
    std::string src = "  123 456  ";
    std::string dst = trim_copy(std::move(src));
    fmt::print("after : {} , {}\n", src, dst);
}

int main()
{
    Test_Trim();

    return 0;
}
