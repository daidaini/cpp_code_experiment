#pragma once

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <fmt/format.h>
#include <fmt/color.h>

#include <map>

#include <syscall.h>

using namespace muduo::net;
using namespace muduo;

class Service
{
public:
    Service(EventLoop *loop, const InetAddress *listenAddr)
        : m_Server(loop, *listenAddr, "Service")
    {
        m_Server.setConnectionCallback([this](const TcpConnectionPtr &conn)
                                       { this->OnConnection(conn); });

        m_Server.setMessageCallback([this](const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
                                    { this->OnMessage(conn, buf, time); });
    }

    void Start()
    {
        m_Server.setThreadNum(1);
        m_Server.start();
        fmt::print("Server [{}] starts..\n", m_Server.name());
    }

private:
    void OnConnection(const TcpConnectionPtr &conn)
    {
        auto connName = conn->name();
        auto it = m_Conns.find(connName);
        if (it == m_Conns.end())
        {
            fmt::print(fg(fmt::color::sea_green), "New connection : {}\n", connName);
            m_Conns.emplace(connName, conn);

            // 添加proxy protocl 报文
        }
        else
        {
            fmt::print(fg(fmt::color::orange_red), "Remove connection : {}\n", connName);
            m_Conns.erase(it);
        }
    }

    void OnMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
    {
        std::string msg = buf->retrieveAllAsString();
        fmt::print(fg(fmt::color::light_yellow), "Recv message : {}\n", msg);

        if (msg.find("|6011|") != msg.npos) // 登录
        {
            return OnLogin(conn, msg);
        }
        else if (msg.find("|6031|") != msg.npos) // 心跳
        {
            return OnHeart(conn, msg);
        }
        else
        {
            return OnCommonRsp(conn, msg);
        }

        /*
        static long long requestId = 0;
        //[time][rsp][conn][tid][reqid]
        std::string msgBack = fmt::format("[{}][Response][{}][{}][{}]", time.toString(), conn->name(), ::syscall(SYS_gettid), ++requestId);
        // 返回应答
        conn->send(msgBack.data(), msgBack.size());
        */
    }

private:
    TcpServer m_Server;

    std::map<std::string, TcpConnectionPtr> m_Conns;

private:
    void OnCommonRsp(const TcpConnectionPtr &conn, const std::string &msg)
    {
        std::string errorRsp = "A|PC;IIP=172.25.2.43;HDV6.5.7.12;|1|N|1|This is error msg|1|";

        if (msg.find("|6014|") != msg.npos)
        {
            // 模拟产生Segmentation Fault
            int *ptr = nullptr;
            // 试图通过空指针访问内存
            *ptr = 42; // 这行代码将导致段错误
        }
        conn->send(errorRsp.data(), errorRsp.size());
    }

    void OnLogin(const TcpConnectionPtr &conn, const std::string &msg)
    {
        std::string loginRsp = "A|PC;IIP=172.25.2.43;HDV6.5.7.12;|1|Y|"
                               "9002050018|0.0000|0.00| |20240109|172.25.2.43,172.190.112.36|20240108|15:53:33|7|2|0|1| |1000100000|"
                               "2.1.2.0r| | |20240109| | | | |2.1.2.0r| |V8.2.1.2.0|1100000000| | | | | | |1|1|1| | |6785913|"
                               "0|1;20240109;08:52:28|09:41:16| |0| ";

        conn->send(loginRsp.data(), loginRsp.size());
    }

    void OnHeart(const TcpConnectionPtr &conn, const std::string &msg)
    {
        std::string rsp = "A|PC;IIP=172.25.2.43;HDV6.5.7.12;|1|Y|";
        conn->send(rsp.data(), rsp.size());
    }

    void PrepareProxyProtocolHead()
    {
        uint8_t protocol_version;
        uint8_t command;
        uint32_t source_address;
        uint16_t source_port;
        uint32_t destination_address;
        uint16_t destination_port;
        uint16_t data_length;
    }
};