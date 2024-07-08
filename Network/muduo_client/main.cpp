#include "Client.h"
#include "Logging.h"
#include "EventLoopThread.h"
#include <vector>
#include <thread>

// std::string server_addr = "192.168.11.56";
std::string server_addr = "127.0.0.1";
int default_port = 8888;

void quit(EventLoop *p)
{
    // print(p);
    p->quit();
}

int main(int argc, char *argv[])
{
    EventLoop mainloop;
    InetAddress tcpAddr(server_addr, default_port);

    Client cli(&mainloop, &tcpAddr, "main_client");

    cli.Start();

    ::sleep(1); // 等1秒连接成功

    std::thread(
        [&cli]()
        {
            for (int i = 0; i < 100; ++i)
            {
                ::sleep(2);
                cli.SendMsg(fmt::format("This is request {}", Timestamp::now().toFormattedString()));
            }
        })
        .detach();

    mainloop.loop();

    return 0;
}
