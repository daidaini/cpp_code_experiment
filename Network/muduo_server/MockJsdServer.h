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

/**
 * @brief 模拟两种情况
 * 1. 响应ping请求
 * 2. 定时推送数据
 *
 */

class MockServer
{
public:
    MockServer(EventLoop *loop, const InetAddress *listenAddr)
        : m_Server(loop, *listenAddr, "MockJsdServer")
    {
        m_Server.setConnectionCallback([this](const TcpConnectionPtr &conn)
                                       { this->OnConnection(conn); });

        m_Server.setMessageCallback([this](const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
                                    { this->OnMessage(conn, buf, time); });

        loop->runEvery(30.0, [this]()
                       { this->OnTimer(); });
    }

    void Start()
    {
        m_Server.setThreadNum(1);
        m_Server.start();
        fmt::print("Mock server [{}] starts..\n", m_Server.name());
    }

    void OnConnection(const TcpConnectionPtr &conn)
    {
        auto connName = conn->name();
        auto it = m_Conns.find(connName);
        if (it == m_Conns.end())
        {
            fmt::print(fg(fmt::color::sea_green), "[{}]New connection : {}\n", ::syscall(SYS_gettid), connName);
            m_Conns.emplace(connName, conn);
        }
        else
        {
            fmt::print(fg(fmt::color::orange_red), "Remove connection : {}\n", connName);
            m_Conns.erase(it);
        }
    }

    void OnMessage(const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
    {
        // std::string msg = buf->retrieveAllAsString();
        std::string msg = std::string(buf->peek(), buf->readableBytes());
        if (msg.find("ping") != msg.npos)
        {
            fmt::print(fg(fmt::color::cornflower_blue), "[{}]This is ontime ping request\n", ::syscall(SYS_gettid));
            buf->retrieve(2);
            fmt::print("total = {}, left = {}\n", msg.size(), buf->readableBytes());
            buf->retrieve(buf->readableBytes());
            fmt::print("empty = {}\n", buf->readableBytes());
            conn->send(msg.data(), msg.size());
        }
        else
        {
            fmt::print(fg(fmt::color::orange_red), "[{}]Recv message : {}\n", ::syscall(SYS_gettid), msg);
            buf->retrieveAll();
        }
    }

    void OnTimer()
    {
        for (auto &item : m_Conns)
        {
            auto msg = fmt::format("This is push data {}", Timestamp::now().toFormattedString());
            fmt::print(fg(fmt::color::medium_sea_green), "[{}]OnTimer[{}]\n", ::syscall(SYS_gettid), msg, item.second->getConnId());
            item.second->send(msg.c_str(), msg.size());
        }
    }

private:
    TcpServer m_Server;

    std::map<std::string, TcpConnectionPtr> m_Conns;
};
