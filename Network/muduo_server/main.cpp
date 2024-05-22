#include "Service.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

/*可不可以一个loop,两个server, 两个端口？
 */
/*
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Lack of param\n");
        exit(-1);
    }
    pid_t pid, sid;
    char cwd[1024]{};
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("failed .. %s\n", cwd);
        strcpy(cwd, "/"); // 根目录
    }
    // 创建子进程
    pid = fork();
    // 如果fork失败
    if (pid < 0)
    {
        perror("Fork failed");
        return 1;
    }
    // 如果是父进程，退出
    if (pid > 0)
    {
        printf("Parent process exiting\n");
        return 0;
    }

    // 创建新会话
    sid = setsid();
    if (sid < 0)
    {
        perror("Failed to create new session");
        return 1;
    }

    // 切换工作目录

    printf("Running directory = %s\n", cwd);
    if (chdir(cwd) < 0)
    {
        perror("Failed to change directory");
        return 1;
    }

    // 重设文件权限掩码
    umask(0);

    // 关闭文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 执行守护进程的核心逻辑
    while (true)
    {
        // Your daemon's code goes here
        EventLoop mainLoop;

        InetAddress addr_1(8888);
        Service serv_1(&mainLoop, &addr_1);
        // InetAddress addr_2(20002);
        // Service serv_2(&mainLoop, &addr_2);

        serv_1.Start();
        // serv_2.Start();

        mainLoop.loop();

        // Sleep for a while to avoid high CPU usage
        sleep(1);
    }

    return 0;
}
*/

int main(int argc, char *argv[])
{
    int port = 8888;
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    EventLoop mainLoop;

    InetAddress addr_1(port);
    Service serv_1(&mainLoop, &addr_1);
    // InetAddress addr_2(20002);
    // Service serv_2(&mainLoop, &addr_2);

    serv_1.Start();
    // serv_2.Start();

    mainLoop.loop();

    return 0;
}