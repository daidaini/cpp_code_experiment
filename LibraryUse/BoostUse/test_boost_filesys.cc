#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <fmt/format.h>

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

bool isFileExistsInDirectory(const std::string &directory, const std::string &filename)
{
    fs::path dirPath(directory);
    fs::directory_iterator endItr;

    for (fs::directory_iterator itr(dirPath); itr != endItr; ++itr)
    {
        if (is_regular_file(itr->status()))
        {
            // 比较文件名，不区分大小写
            if (ba::iequals(itr->path().filename().string(), filename))
            {
                return true;
            }
        }
    }

    return false;
}

void traverseDirectory(const fs::path &directoryPath)
{
    if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
    {
        std::cerr << "Invalid directory: " << directoryPath << std::endl;
        return;
    }

    // recursive_directory_iterator
    for (const auto &entry : fs::directory_iterator(directoryPath))
    {
        if (fs::is_regular_file(entry))
        {
            std::cout << entry.path().filename().string() << std::endl;
        }
    }
}

std::string getAbsolutePath(const std::string &directory)
{
    fs::path absolutePath = fs::absolute(directory);
    return absolutePath.string();
}

bool isFileModifiedSinceLastRead(const std::string &path, time_t &last_operate_time)
{
    if (!fs::exists(path))
    {
        return true; // 被删除？
    }

    std::time_t last_write_time = fs::last_write_time(path);
    fmt::print("[{}] last_write_time = {}\n", path, last_write_time);

    if (last_write_time > last_operate_time)
    {
        last_operate_time = last_write_time;

        return true;
    }

    return false;
}

int main(int argc, char *argv[])
{
    std::string path = "tmp.txt";

    time_t curr_time = time(nullptr);

    for (int i = 0; i < 100; ++i)
    {
        sleep(1);

        if (isFileModifiedSinceLastRead(path, curr_time))
        {
            fmt::print("[{}] changed\n", path);
        }
    }

    return 0;
}
