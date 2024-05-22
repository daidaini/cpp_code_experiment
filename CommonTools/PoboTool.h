/*
 * @Descripttion: 一些通用的工具方法
 * @Author: yubo
 * @Date: 2022-07-13 09:02:21
 * @LastEditTime: 2022-08-26 13:52:13
 */
#pragma once

#include <locale>
#include <iconv.h>
#include <cstring>
#include <vector>
#include <string>

namespace dai
{
   // 生成校验码
   unsigned short GetCheckCode(const char *src, unsigned short size);

   // 编码转换 (UTF8 和 GB2312)
   int GB2312ToUTF8(char *szSrc, size_t iSrcLen, char *szDst, size_t iDstLen);
   int UTF8ToGB2312(char *szSrc, size_t iSrcLen, char *szDst, size_t iDstLen);

   // 文件内容读写
   std::vector<std::string> ReadLinesFromFile(const std::string &filename);
   std::string ReadContentsFromFile(const std::string &filename);

   // 获取某个路径下的所有文件名
   std::vector<std::string> GetFilenamesInDir(const std::string &dirPath);

   // 判断某个文件在某个时间点之后是否被修改过
   bool IsFileModifiedAfterSpecifiedTime(const std::string &file, time_t compareTime);

   // md5码转字符串
   std::string Md5HexToString(const unsigned char *md);

   // 正则解析子串
   std::string SubStringByRegex(const std::string &src, const std::string &regex_pattern);
}
