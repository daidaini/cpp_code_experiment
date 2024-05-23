#include "TestAlgorithm.hpp"

using namespace std;

// std::transform
void test_transform()
{
    vector<int> src{1, 2, 3, 4, 5};

    // 转换到原位置
    transform(src.begin(), src.end(), src.begin(), [](int elem)
              { return elem * 2; });

    for_each(src.begin(), src.end(), [](int elem)
             { cout << elem << ","; });
    cout << endl;

    // 转换到新位置
    vector<int> dst;
    transform(src.begin(), src.end(), std::back_inserter(dst),
              [](int elem)
              {
                  return elem + 10;
              });

    for_each(dst.begin(), dst.end(), [](int elem)
             { cout << elem << ","; });
    cout << endl;
}

int main(int argc, char *argv[])
{
    // test_transform();

    Algorithm::test_std_find();

    return 0;
}