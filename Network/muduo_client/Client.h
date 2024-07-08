#pragma once

#include "muduo/net/EventLoop.h"
#include "muduo/net/InetAddress.h"
#include "muduo/net/TcpClient.h"

#include <fmt/format.h>
#include <fmt/color.h>

#include <map>

#include <syscall.h>

using namespace muduo::net;
using namespace muduo;
class Client
{
public:
    Client(EventLoop *loop, const InetAddress *serverAddr, std::string name)
        : m_Client(loop, *serverAddr, name)
    {
        m_Client.setConnectionCallback([this](const TcpConnectionPtr &conn)
                                       { this->OnConnect(conn); });

        m_Client.setMessageCallback([this](const TcpConnectionPtr &conn, Buffer *buf, Timestamp time)
                                    { this->OnResponse(conn, buf, time); });

        loop->runEvery(10.0, [this]()
                       { this->OnTimer(); });

        m_ClientName = std::move(name);
    }

    void Start()
    {
        m_Client.connect();
    }

    void OnConnect(const TcpConnectionPtr &tcpConn)
    {
        auto connName = tcpConn->name();
        if (tcpConn->connected())
        {
            fmt::print(fg(fmt::color::sea_green), "[{}]New connection : {}\n", ::syscall(SYS_gettid), connName);
            std::string req = "First hello";
            tcpConn->send(req.data(), req.size());
        }
        else
        {
            fmt::print(fg(fmt::color::orange_red), "Remove connection : {}\n", connName);
        }
    }

    void OnResponse(const TcpConnectionPtr &conn, Buffer *buf, muduo::Timestamp time)
    {
        std::string msg = buf->retrieveAllAsString();

        if (msg.find("push") != msg.npos)
        {
            fmt::print(fg(fmt::color::medium_sea_green), "[{}]Push msg : {}\n", ::syscall(SYS_gettid), msg);
        }
        else
        {
            fmt::print(fg(fmt::color::light_yellow), "[{}]Recv msg : {}\n", ::syscall(SYS_gettid), msg);
        }

        // static long long requestId = 0;
        //[time][req][conn][tid][reqid]
        //  std::string msgBack = fmt::format("[{}][Request][{}][{}][{}]", time.toString(), conn->name(), ::syscall(SYS_gettid), ++requestId);
        //   返回应答
        //  conn->send(msgBack.data(), msgBack.size());
    }

    void SendMsg(std::string msg)
    {
        fmt::print(fg(fmt::color::light_sky_blue), "[{}]Send message : {}\n", ::syscall(SYS_gettid), msg);
        m_Client.connection()->send(msg.data(), msg.size());
    }

    void OnTimer()
    {
        fmt::print(fg(fmt::color::cornflower_blue), "[{}]OnTimer Send ping message\n", ::syscall(SYS_gettid));
        std::string msg = "This is ping msg";
        m_Client.connection()->send(msg.data(), msg.size());
    }

    void Close()
    {
        // m_Client.stop();
        m_Client.disconnect();
    }

private:
    TcpClient m_Client;

    std::string m_ClientName;
};