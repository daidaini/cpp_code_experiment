//#include "format.h"

#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#include <time.h>
#include <thread>
#include <iostream>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

#define SERVER_PORT 8090
#define LISTEN_NUM 20

void print()
{
	time_t starttime = time(nullptr);
	int n = 0;
	while (1)
	{
		if (time(nullptr) - starttime > 1)
		{
			printf("当前线程\n");
			starttime = time(nullptr);
			if (n++ > 5)
				break;
		}

		::Sleep(50);
	}
}

vector<SOCKET> all_cli_sockets;

bool  HandleData(SOCKET cli_sock)
{
	char recv_buf[1024] = "";
	int recv_len = recv(cli_sock, recv_buf, sizeof(recv_buf), 0);
	if (recv_len == SOCKET_ERROR)
	{
		cout << "recv error\n";
		return false;
	}
	cout << "What i am recved is\n" << recv_buf << "\nNow it is back for you\n";
	cout << "*********************************\n";

	send(cli_sock, recv_buf, strlen(recv_buf), 0);
	return true;
}

void PrintSocektInfo(SOCKET sock)
{
	sockaddr_in addr{};
	int addrlen = sizeof(addr);
	::getsockname(sock, (sockaddr*)&addr, &addrlen);
	cout << "[getsockname]: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;

	::getpeername(sock, (sockaddr*)&addr, &addrlen);
	cout << "[getpeername]: " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
}

void DoLoopIO(SOCKET serv_sock, sockaddr* addr)
{
	fd_set read_fds;
	fd_set except_fd;
	FD_ZERO(&read_fds);
	FD_ZERO(&except_fd);

	FD_SET(serv_sock, &read_fds);
	FD_SET(serv_sock, &except_fd);

	timeval timeout{ 100,0 };

	do
	{
		if (serv_sock == INVALID_SOCKET)
		{
			cout << "socket error\n";
			exit(-1);
		}
		int ret = ::select(serv_sock + 1, &read_fds, nullptr, &except_fd, &timeout);
		if (ret < 0)
		{
			cout << "select error\n";
			FD_ZERO(&read_fds);
			FD_SET(serv_sock, &read_fds);
			continue;
		}
		else if (ret == 0)
		{
			cout << "timeout happended\n";
			continue;
		}

		if (FD_ISSET(serv_sock, &read_fds))
		{
			int addr_len = sizeof(sockaddr);
			SOCKET cli_sock = accept(serv_sock, addr, &addr_len);
			cout << "New connection socketid = " << cli_sock << endl;
			PrintSocektInfo(cli_sock);

			FD_SET(cli_sock, &read_fds);
			all_cli_sockets.push_back(cli_sock);
		}
		else
		{
			for (int i = 0; i < all_cli_sockets.size(); ++i)
			{
				auto& cs = all_cli_sockets[i];
				if (FD_ISSET(cs, &read_fds))
				{
					bool ret = HandleData(cs);
					if (!ret)
					{
						FD_CLR(cs, &read_fds);
						closesocket(cs);
						cs = INVALID_SOCKET;
						all_cli_sockets.erase(all_cli_sockets.begin() + i);
					}
				}
				else
				{
				}
			}
		}
	} while (true);
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStratup 网络库初始化失败\n";
		exit(-1);
	}

	sockaddr_in addr;
	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (server_sock == INVALID_SOCKET)
	{
		printf("socket create fail\n");
		return 0;
	}
	int ret = ::bind(server_sock, (sockaddr*)&addr, sizeof(sockaddr));
	if (ret == SOCKET_ERROR)
	{
		printf("bind failed\n");
		return 0;
	}

	if (listen(server_sock, LISTEN_NUM) == SOCKET_ERROR)
	{
		printf("listen failed\n");
		return 0;
	}

	cout << "服务器开启监听，准备接受数据\n";
	DoLoopIO(server_sock, (sockaddr*)&addr);

	WSACleanup();

	return 0;
}