#include "SyncBlockedClient.h"
#include <thread>
#include <chrono>

#include <fmt/format.h>

SyncBlockedClinet::SyncBlockedClinet(const std::string &servAddr, uint16_t servPort, int timeout, int perPkgSizeLimit)
    : m_ServAddr(servAddr), m_ServPort(servPort), m_Timeout(timeout)
{
    m_CachedBuff = new char[perPkgSizeLimit]{};
    m_PerPkgSizeLimit = perPkgSizeLimit;
}

SyncBlockedClinet::~SyncBlockedClinet()
{
    // 析构的时候，需要关闭连接
    if (m_Socket > 0)
    {
        ::close(m_Socket);
    }
    delete[] m_CachedBuff;
}

bool SyncBlockedClinet::CreateConnect()
{
    m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 明确创建tcp套接字
    if (m_Socket == -1)
    {
        m_LastErrorMsg = "Create socket failed";

        return false;
    }

    m_SockAddr.sin_family = AF_INET;
    m_SockAddr.sin_port = htons(m_ServPort); // 转换网络字节序
    if (inet_pton(AF_INET, m_ServAddr.c_str(), &m_SockAddr.sin_addr) <= 0)
    {
        m_LastErrorMsg = "Set ip address of JSD system failed";
        return false;
    }

    SetOptions();

    auto addrLen = sizeof(sockaddr_in);
    // linux下，connect的阻塞情况默认超时时间为75秒
    int connectRet = ::connect(m_Socket, (sockaddr *)&m_SockAddr, addrLen);

    if (connectRet < 0)
    {
        m_LastErrorMsg = fmt::format("Connect address [{}:{}] fail", m_ServAddr, m_ServPort);
        return false;
    }

    m_IsConnected = true;

    m_AwakedTime = time(nullptr);

    return true;
}

bool SyncBlockedClinet::SetOptions()
{
    struct timeval timeout;
    timeout.tv_usec = 0;
    timeout.tv_sec = m_Timeout;

    // 发送超时设置
    if (setsockopt(m_Socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        m_LastErrorMsg = "设置发送超时失败";
        return false;
    }
    // 接收超时设置
    if (setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        m_LastErrorMsg = "设置接收超时失败";
        return false;
    }

    // 以下为针对性新增优化
    // 1. 禁用Negle算法
    int flag = 1;
    if (setsockopt(m_Socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int)) < 0)
    {
        m_LastErrorMsg = "禁用Negle算法失败";
        return false;
    }
    // 2. 启用快速重传
    flag = 1;
    if (setsockopt(m_Socket, IPPROTO_TCP, TCP_QUICKACK, &flag, sizeof(flag)) < 0)
    {
        m_LastErrorMsg = "启用快速重传失败";
        return false;
    }
    return true;
}

std::pair<bool, std::string> SyncBlockedClinet::SendAndRecv(const char *request, size_t reqlen)
{
    if (!m_IsConnected)
    {
        if (!CreateConnect())
        {
            return {false, "Create connection to server failed.."};
        }
    }

    // MSG_NOSIGNAL - 发送时断开连接不会发起 SGPIPE信号，而是返回错误
    ssize_t bytesSend = ::send(m_Socket, request, reqlen, MSG_NOSIGNAL);
    if (bytesSend == -1)
    {
        int errcode = errno;
        m_LastErrorMsg = fmt::format("Send data failed. [{}][{}]", errcode, strerror(errcode));
        // CloseConnection();
        return {false, "Send data to server failed.."};
    }

    auto ret = RecvData();
    m_AwakedTime = time(nullptr);

    if (!ret.first)
    {
        // CloseConnection();
        return ret;
    }

    return std::make_pair(true, std::string(m_CachedBuff));
}

std::pair<bool, std::string> SyncBlockedClinet::RecvData()
{
    const int tryTimes = 3;
    for (int i = 0; i < tryTimes; ++i)
    {
        ssize_t bytesRecv = ::recv(m_Socket, m_CachedBuff, m_PerPkgSizeLimit, 0);
        if (bytesRecv == -1)
        {
            int errcode = errno;
            m_LastErrorMsg = fmt::format("Recv data failed. [{}][{}]", errcode, strerror(errcode));
            if (errcode == EAGAIN || errcode == EWOULDBLOCK)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 等50ms
                continue;
            }

            return {false, "Recv data from server happend error.."};
        }
        else if (bytesRecv == 0)
        {
            m_LastErrorMsg = "Connection closed by the peer";
            return {false, m_LastErrorMsg};
        }
        else
        {
            if (bytesRecv < 0)
            {
                return {false, "Data recv failed"};
            }
            m_CachedBuff[bytesRecv] = '\0'; // 加结束符 ::memset(m_CachedBuff, 0, byteRecv)
            return {true, ""};
        }
    }
    return {false, "接收后台应答超时!"};
}

void SyncBlockedClinet::CloseConnection()
{
    // 返回超时错误时，需要关闭套接字
    if (m_IsConnected)
    {
        m_IsConnected = false;
    }

    ::close(m_Socket);
    m_Socket = -1;
}

bool SyncBlockedClinet::IsValid() const
{
    return m_IsConnected;
}

time_t SyncBlockedClinet::AwakedTime() const
{
    return m_AwakedTime;
}

std::string SyncBlockedClinet::GetLastErrmsg() const
{
    return m_LastErrorMsg;
}