#include "../ClientDataDefine.h"

#include <dlfcn.h>
#include <cassert>
#include <cstring>
#include <string>
#include <thread>
#include <unistd.h>

using CreateClientFuncType = void *(*)(const char *, OnConnectionFuncType, OnMsgFuncType);
using DeleteCleintFuncType = void (*)(void *);
using SendMsgFuncType = void (*)(void *, const char *, size_t);

bool g_IsConnected = false;

void OnMsg(ErrorCode code, const char *msg, size_t msglen)
{
    printf("[%d] %s\n", code, std::string(msg, msglen).c_str());
}

void OnConnection(ErrorCode code)
{
    if (code == ErrorCode::Succcess)
    {
        g_IsConnected = true;
        printf("Connect success\n");
    }
    else
    {
        g_IsConnected = false;
        printf("Connect failed\n");
    }
}

void Test()
{
    void *handle = dlopen("libAsyncNetMsg.so", RTLD_LAZY);

    assert(handle != nullptr);

    CreateClientFuncType create_client = (CreateClientFuncType)dlsym(handle, "HDCreateClient");
    DeleteCleintFuncType delete_client = (DeleteCleintFuncType)dlsym(handle, "HDDeleteCleint");
    SendMsgFuncType send_msg = (SendMsgFuncType)dlsym(handle, "HDSendMsg");

    assert(create_client != nullptr);

    void *client = create_client("172.30.223.114:8888", OnConnection, OnMsg);
    if (client == nullptr)
    {
        printf("create client failed\n");
        exit(1);
    }

    while (!g_IsConnected)
    {
        ::sleep(1);
    }

    for (int i = 0; i < 10; ++i)
    {
        char buf[64]{};
        sprintf(buf, "This is |%d| msg", 6011 + i);
        send_msg(client, buf, strlen(buf));
        ::usleep(50000);
    }

    // getchar();
    std::this_thread::sleep_for(std::chrono::seconds(25));

    send_msg(client, "prepare to end", 14);
    ::sleep(1);

    delete_client(client);
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 1; ++i)
    {
        std::thread([]()
                    { Test(); })
            .detach();
    }
    getchar();
    return 0;
}