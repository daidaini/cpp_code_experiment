#include "PoboTool.h"

#include <fstream>
#include <sstream>

#ifdef BOOST_REGEX_SUPPORT
#include <boost/regex.hpp>
#else
#include <regex>
#endif

#include <boost/filesystem.hpp>

namespace dai
{
    namespace fs = boost::filesystem;
    unsigned short GetCheckCode(const char *src, unsigned short size)
    {
        volatile unsigned short checknum = 0;

        const unsigned short *p = (const unsigned short *)src;
        unsigned short num = size / 2;
        for (unsigned short i = 0; i < num; ++i, p++)
        {
            checknum += *p;
        }
        if (size & 1)
        {
            unsigned short t = 0;
            ::memcpy(&t, src + size - 1, 1);
            checknum += t;
        }
        return checknum;
    }

    int GB2312ToUTF8(char *szSrc, size_t iSrcLen, char *szDst, size_t iDstLen)
    {
        iconv_t cd = iconv_open("utf-8//IGNORE", "gb2312//IGNORE");
        if (0 == cd)
            return -2;
        ::memset(szDst, 0, iDstLen);
        char **src = &szSrc;
        char **dst = &szDst;
        if (-1 == (int)iconv(cd, src, &iSrcLen, dst, &iDstLen))
            return -1;
        iconv_close(cd);
        return 0;
    }

    int UTF8ToGB2312(char *szSrc, size_t iSrcLen, char *szDst, size_t iDstLen)
    {
        iconv_t cd = iconv_open("gb2312//IGNORE", "utf-8//IGNORE"); // take care of "//IGNORE", it will ignore those invalid code
        if (0 == cd)
            return -2;
        ::memset(szDst, 0, iDstLen);
        char **src = &szSrc;
        char **dst = &szDst;
        if (-1 == (int)iconv(cd, src, &iSrcLen, dst, &iDstLen))
            return -1;
        iconv_close(cd);
        return 0;
    }

    std::vector<std::string> ReadLinesFromFile(const std::string &filename)
    {
        std::ifstream fs;
        fs.open(filename);

        std::vector<std::string> lines;
        if (!fs.is_open())
        {
            return lines;
        }

        std::string oneline;
        while (getline(fs, oneline))
        {
            if (oneline.empty())
            {
                // 空行不要
                continue;
            }

            lines.emplace_back(oneline);
        }
        fs.close();

        return lines;
    }

    std::string ReadContentsFromFile(const std::string &filename)
    {
        std::fstream fs;
        fs.open(filename);
        if (!fs.is_open())
            return "";

        std::ostringstream oss;
        oss << fs.rdbuf();
        fs.close();
        return oss.str();
    }

    std::vector<std::string> GetFilenamesInDir(const std::string &dirPath)
    {
        std::vector<std::string> results;

        if (!fs::exists(dirPath) || !fs::is_directory(dirPath))
        {
            return results;
        }

        for (const auto &entry : fs::directory_iterator(dirPath))
        {
            if (fs::is_regular_file(entry))
            {
                results.emplace_back(entry.path().filename().string());
            }
        }

        return results;
    }

    bool IsFileModifiedAfterSpecifiedTime(const std::string &file, time_t compareTime)
    {
        if (!fs::exists(file))
        {
            return true;
        }

        time_t lastWriteTime = fs::last_write_time(file);
        if (lastWriteTime > compareTime)
        {
            return true;
        }
        return false;
    }

    std::string Md5HexToString(const unsigned char *md)
    {
        std::string result;
        for (size_t i = 0; i < 16; ++i)
        {
            char tmp[3]{};
            sprintf(tmp, "%02x", md[i]);
            result.append(tmp);
        }
        return result;
    }

    std::string SubStringByRegex(const std::string &src, const std::string &regex_pattern)
    {
#ifdef BOOST_REGEX_SUPPORT
        using namespace boost;
#else // 使用标准库regex (优先使用boost库，标准库里的regex某个版本有bug)
        using namespace std;
#endif
        regex rule(regex_pattern);
        cmatch matchResult;
        bool mathced = regex_match(src.c_str(), matchResult, rule);
        if (mathced)
        {
            size_t size = matchResult.size();
            if (size > 1)
            {
                return matchResult[size - 1].str();
            }
        }

        return {};
    }

}