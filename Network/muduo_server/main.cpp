#include "MockJsdServer.h"
#include "Service.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int port = 8888;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    EventLoop mainLoop;

    InetAddress addr(port);
    Service mockServ(&mainLoop, &addr);

    mockServ.Start();

    mainLoop.loop();

    return 0;
}
