#include <vector>
#include <list>
#include <iostream>

// #include <boost/test/auto_unit_test.hpp>

using namespace std;

void test_resize()
{
    std::vector<int> src{2, 4, 6, 8, 10, 12}; // 6

    src.resize(3);

    // cout << src.size() << endl;
    try
    {
        cout << src.at(2) << '\n';
        cout << src.at(3) << '\n';
    }
    catch (const std::exception &e)
    {
        cout << "exception : " << e.what() << '\n';
    }
}

int main(int argc, char *argv[])
{
    test_resize();

    return 0;
}