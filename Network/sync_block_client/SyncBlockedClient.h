#pragma once

/*
 * 阻塞客户端：
 * 发起请求后阻塞，同步等待应答数据，待应答数据收到后再确认是否发起后续同步阻塞请求
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <poll.h>
#include <errno.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <map>

class SyncBlockedClinet final
{
public:
    SyncBlockedClinet(const std::string &servAddr, uint16_t servPort, int timeout, int perPkgSizeLimit = 8192);
    ~SyncBlockedClinet();

    bool CreateConnect();

    std::pair<bool, std::string> SendAndRecv(const char *request, size_t reqlen);

    bool IsValid() const;

    time_t AwakedTime() const;

    // 关闭连接
    void CloseConnection();

    std::string GetLastErrmsg() const;

private:
    std::pair<bool, std::string> RecvData();

    bool SetOptions();

private:
    int m_PerPkgSizeLimit{0};

    int m_Socket{-1};
    std::string m_ServAddr;
    uint16_t m_ServPort{0};
    int m_Timeout{0}; // seconds

    sockaddr_in m_SockAddr{};

    bool m_IsConnected{false};

    char *m_CachedBuff = nullptr;

    // 被使用的唤醒时间
    time_t m_AwakedTime{0};

    std::string m_LastErrorMsg;
};