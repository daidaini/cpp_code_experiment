/*
 * @Descripttion:
 * @Author: yubo
 * @Date: 2022-10-12 13:24:31
 * @LastEditTime: 2022-10-12 17:10:03
 */

#include <fmt/format.h>

#include <thread>
#include <string>

#include <boost/pool/pool.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/singleton_pool.hpp>
using namespace boost;

using UnitType = char[1024];

void Test_Pool()
{
    boost::pool<> pool1(sizeof(UnitType));

    for (int i = 0; i < 50000; ++i)
    {
        usleep(1000);
        char *data1 = static_cast<char *>(pool1.malloc());
        assert(pool1.is_from(data1));
        // fmt::print("ptr = {}\n", fmt::ptr(data1));

        strncpy(data1, "first", sizeof(UnitType));
        // fmt::print("output_1 = {}\n", data1);

        pool1.free(data1);

        char *data2 = static_cast<char *>(pool1.malloc());
        assert(pool1.is_from(data2));
        // fmt::print("ptr = {}\n", fmt::ptr(data2));
        strncpy(data2, "second try", sizeof(UnitType));
        // fmt::print("output_2 = {}, max = {}\n", data2, pool1.get_requested_size());
        pool1.free(data2);
    }
    fmt::print("finished\n");
    getchar();
}

void Test_Pool_Ordered()
{
    boost::pool<> pool1(sizeof(UnitType));

    for (int i = 0; i < 20000; ++i)
    {
        usleep(1000);

        //4096 -- 1.6
        //1024 -- 1.6
        char *data1 = static_cast<char *>(pool1.ordered_malloc(1 + i / sizeof(UnitType)));
        strncpy(data1, "second try", sizeof(UnitType));

        pool1.ordered_free(data1);
    }
    fmt::print("finished\n");
    getchar();
}

// result: pool not thread safe
void Test_Pool_MultiThread()
{
    boost::pool<> pool1(sizeof(UnitType));

    for (int times = 0; times < 10; ++times)
    {
        std::thread([&pool1]()
                    { 
                for (int i = 0; i < 100; ++i)
                {
                    usleep(1000);  //10 ms
                    char *data1 = static_cast<char*>(pool1.malloc());
                    strncpy(data1, "try first", sizeof(UnitType));

                    pool1.free(data1);
                } 
                fmt::print("finish task1\n"); })
            .detach();
    }
    getchar();
}

class TestData
{
public:
    std::string m_Data;
    int m_Key = 0;
};

struct TestDataStr
{
    std::string Data;

    TestDataStr()
    {
        Data.reserve(4096);
    }
};

void Test_ObjectPool_String()
{
    object_pool<TestDataStr> pool;

    for (int i = 0; i < 20000; ++i)
    {
        usleep(1000);
        auto data = pool.construct();

        data->Data = std::string(10*(i+1), 'c');

        pool.free(data);

        //pool.destroy(data);
    }

    fmt::print("finished\n");
    getchar();
}

void Test_ObjectPool()
{
    object_pool<TestData> pool;
    std::string srcstr(1024 * 64, 'a');

    for (int i = 0; i < 20000; ++i)
    {
        usleep(1000);

        auto data = pool.construct();
        data->m_Data = "123456789";
        data->m_Key = 1;
        // fmt::print("ptr = {}\n", fmt::ptr(data));
        //fmt::print("output = {}\n", data->m_Data);
        pool.free(data);

        // fmt::print("output = {}\n", data->m_Data.empty() ? "null" : data->m_Data);

        data = pool.construct();
        data->m_Data = std::move(srcstr);
        data->m_Key = 2;
        // fmt::print("output = {}\n", data->m_Data);
        //  fmt::print("ptr = {}\n", fmt::ptr(data));
        srcstr = std::move(data->m_Data);
        pool.free(data);
    }
    fmt::print("finished\n");
    getchar();
}

struct pool_tag
{
};
using spool = singleton_pool<pool_tag, sizeof(char)>;

void Test_SingletionPool()
{
    for (int i = 0; i < 2000; ++i)
    {
        usleep(10000);

        char *data = static_cast<char *>(spool::ordered_malloc((1 + i) * sizeof(UnitType)));

        strncpy(data, "try first", sizeof(UnitType));
        fmt::print("output = {}\n", data);

        spool::ordered_free(data);
    }

    fmt::print("finished\n");
    getchar();
}

// thread safe
void Test_SingletionPool_MultiThread()
{
    for (int times = 0; times < 10; ++times)
    {
        std::thread([]()
                    { 
                for (int i = 0; i < 100; ++i)
                {
                    usleep(1000);  //10 ms
                    char *data = static_cast<char*>(spool::malloc());
                    strncpy(data, "try first", sizeof(UnitType));

                    spool::free(data);
                } 
                fmt::print("finish task\n"); })
            .detach();
    }
    getchar();
}

int main(int argc, char *argv[])
{
    // Test_Pool();
    Test_Pool_Ordered();

    // Test_Pool_MultiThread();

    //Test_ObjectPool();
    //Test_ObjectPool_String();

    // Test_SingletionPool();

    return 0;
}