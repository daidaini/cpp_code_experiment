#include "Client.h"

#include "Logging.h"
#include "EventLoopThread.h"

#include <vector>
#include <thread>

/**
 * @brief 模拟一个EventLoop对应多个TcpClient的情况
 *  需要做到可以安全断开连接，删除TcpClient
 */

// std::string server_addr = "192.168.11.56";
std::string server_addr = "127.0.0.1";
int default_port = 8888;
int port_2 = 8889;

EventLoop *g_clientLoop = nullptr;
EventLoop *g_clientLoop_2 = nullptr;
InetAddress g_addr(server_addr, default_port);
InetAddress g_addr_2(server_addr, port_2);

int Test_1(int argc, char *argv[])
{
    assert(EventLoop::getEventLoopOfCurrentThread() == NULL);

    std::thread runthread = std::thread(
        []()
        {
            g_clientLoop = new EventLoop;
            assert(EventLoop::getEventLoopOfCurrentThread() == g_clientLoop);

            g_clientLoop->loop();

            delete g_clientLoop;
        });

    std::thread runthread_2 = std::thread(
        []()
        {
            g_clientLoop_2 = new EventLoop;
            assert(EventLoop::getEventLoopOfCurrentThread() == g_clientLoop_2);

            g_clientLoop_2->loop();

            delete g_clientLoop_2;
        });

    assert(EventLoop::getEventLoopOfCurrentThread() == NULL);

    ::sleep(1);
    {
        std::vector<Client *> clients;

        for (int i = 0; i < 10; ++i)
        {
            Client *cli = new Client(g_clientLoop, &g_addr, fmt::format("loop_1_client_{}", i + 1));
            Client *cli_2 = new Client(g_clientLoop, &g_addr_2, fmt::format("loop_2_client_{}", i + 1));

            cli->Start();
            cli_2->Start();

            clients.push_back(cli);
            clients.push_back(cli_2);
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            static int randdata = 1;
            client->SendMsg(fmt::format("msg : {}", randdata));
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            client->Close();
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            delete client;
        }

        /*
                Client cli_1(g_clientLoop_2, &g_addr, "client_1");
                Client cli_2(g_clientLoop_2, &g_addr_2, "client_2");
                Client cli_3(g_clientLoop, &g_addr_2, "client_3");
                Client cli_4(g_clientLoop, &g_addr, "client_4");

                cli_1.Start();
                cli_2.Start();
                cli_3.Start();
                cli_4.Start();

                ::sleep(2);

                cli_1.SendMsg("123456");
                cli_2.SendMsg("abcdefg");
                cli_1.SendMsg("牛逼");
                cli_2.SendMsg("hello word");
                cli_3.SendMsg("Is it true");
                cli_4.SendMsg("I am in one or two");

                ::sleep(2);

                cli_1.Close();
                cli_2.Close();
                cli_3.Close();
                cli_4.Close();

                ::sleep(2);
                */
    }

    printf("clients delete success..\n");

    g_clientLoop->quit();
    g_clientLoop_2->quit();

    if (runthread.joinable())
    {
        runthread.join();
    }
    if (runthread_2.joinable())
    {
        runthread_2.join();
    }

    getchar();

    return 0;
}

void quit(EventLoop *p)
{
    // print(p);
    p->quit();
}

int main(int argc, char *argv[])
{
    EventLoopThread evth_1;
    EventLoopThread evth_2;

    EventLoop *loop_1 = evth_1.startLoop();
    EventLoop *loop_2 = evth_2.startLoop();

    {
        std::vector<Client *> clients;

        for (int i = 0; i < 10; ++i)
        {
            Client *cli = new Client(loop_1, &g_addr, fmt::format("loop_1_client_{}", i + 1));
            Client *cli_2 = new Client(loop_2, &g_addr_2, fmt::format("loop_2_client_{}", i + 1));

            cli->Start();
            cli_2->Start();

            clients.push_back(cli);
            clients.push_back(cli_2);
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            static int randdata = 1;
            client->SendMsg(fmt::format("msg : {}", randdata));
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            client->Close();
        }

        ::sleep(2);

        for (Client *client : clients)
        {
            delete client;
        }
    }

    printf("clients delete success..\n");

    loop_1->runInLoop(std::bind(quit, loop_1));
    loop_2->runInLoop(std::bind(quit, loop_2));

    printf("loop delete success..\n");
    return 0;
}
