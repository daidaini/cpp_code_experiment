/*
 * @Descripttion: 
 * @Author: yubo
 * @Date: 2022-10-13 17:33:35
 * @LastEditTime: 2022-10-14 09:35:04
 */
#include <fmt/format.h>

#include <boost/circular_buffer.hpp>
#include <boost/assign.hpp>
using namespace boost;
using namespace boost::assign;

template<typename T>
void print(T& cb)
{
    for(auto& x : cb)
    {
        fmt::print("{}, ", x);
    }
    fmt::print("\n");
}

void Test_1()
{
    circular_buffer<int> cbuffer(5);

    cbuffer.push_back(1);
    cbuffer.push_front(2);
    assert(cbuffer.front() == 2);

    cbuffer.insert(cbuffer.begin(), 3);

    print(cbuffer);

    cbuffer.pop_front();
    assert(cbuffer.size() == 2);
    cbuffer.pop_back();
    assert(cbuffer[0] == 2);
}

void Test_Split()
{
    //circular_buffer 可以使用 assign库初始化
    circular_buffer<int> cb = (list_of(1), 2, 3);
    print(cb);

    cb.push_back(4);
    print(cb); //覆盖1

    cb.push_back(5);
    print(cb); //覆盖2
}

//using UnitType = char[4096];
using UnitType = std::string;
void Test_3()
{
    circular_buffer<UnitType> cb(3);
    UnitType data = "123456";
    cb.push_back(data);
    //strcpy(data, "12345678");
    data = "12345678";
    cb.push_back(data);
    data = "abcdefg";
    //strcpy(data, "abcdefg");
    cb.push_back(data);
    cb.push_front(data);

    print(cb);
}

int main()
{
    Test_3();
    return 0;
}