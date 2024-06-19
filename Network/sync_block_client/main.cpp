#include "SyncBlockedClient.h"

#include <fmt/format.h>
#include <fmt/color.h>

int main(int argc, char *argv[])
{
    SyncBlockedClinet client(
        "127.0.0.1", 8888, 5);

    if (!client.CreateConnect())
    {
        fmt::print(fg(fmt::color::indian_red), "Failed. {}\n", client.GetLastErrmsg());
        exit(1);
    }

    std::string msg = "abcdefg";
    auto result = client.SendAndRecv(msg.data(), msg.size());
    if (!result.first)
    {
        fmt::print(fg(fmt::color::indian_red), "Failed. {}\n", client.GetLastErrmsg());
    }
    else
    {
        fmt::print(fg(fmt::color::green_yellow), "Recv: {}\n", result.second);
    }

    msg = "R|anyy|6012|";
    result = client.SendAndRecv(msg.data(), msg.size());
    if (!result.first)
    {
        fmt::print(fg(fmt::color::indian_red), "Failed. {}\n", client.GetLastErrmsg());
    }
    else
    {
        fmt::print(fg(fmt::color::green_yellow), "Recv: {}\n", result.second);
    }

    printf("ends\n");

    return 0;
}