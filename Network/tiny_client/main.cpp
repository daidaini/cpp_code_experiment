#ifdef Win32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>

#endif

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <thread>
using namespace std;

#include <fmt/format.h>
#include <fmt/color.h>

#pragma pack(1)
struct PB_FrameHead
{
	// 协议标识，目前定义为66
	unsigned char sign;
	// 0表示未加密，1表示数据加密了
	unsigned char crypt : 2;
	// 未使用
	unsigned char unused : 6;
	// 通讯包总数
	unsigned char PackageNum;
	// 当前通讯包序号，从0开始，当为PackageNum-1时表示是最后一个通讯包
	unsigned char PackageNo;
	// 校验码（对通讯包体进行CRC16校验，在加密压缩后）
	unsigned short CheckCode;
	// 包体的长度，加密压缩后
	unsigned short PackageSize;
};
#pragma pack()

// constexpr char server_addr[24] = "106.14.183.12";
// constexpr char server_addr[24] = "127.0.0.1";
constexpr char server_addr[24] = "192.168.11.56";
int server_port = 20002;

// string login_request = "5=1&58=4bYVnA+AF3IIBYBqWkb1dg\a\a&11=0&3=6011&2061=0&6=1&7=1&\n"
//"249=17368674639&67=6&53=0&62=123&70=12365544&72=9U99SEF4GBFG&71=127.0.0.1&349=&350=&351=BFEBFBFF000306F2\\nBFEBFBFF000306F2"
//"&3=12&281=18516275549&73=HDV6.2.3.4-2.2.6.6.20200312&255=1&254=1&445=AXZQ2020&485=ypfq,sss,123&7034=DPanxtjbh&\n";

// \a 要改成 =
string login_request = "58=4bYVnA+AF3IIBYBqWkb1dg==&11=0&3=6011&2061=0&6=1&7=1&\n"
					   "249=17368674639&67=6&53=a&62=123&70=12365544&72=9U99SEF4GBFG&71=127.0.0.1&349=&350=&351=BFEBFBFF000306F2"
					   "&3=12&281=18516275549&73=HDV6.2.3.4-2.2.6.6.20200312&255=1&254=1&445=AXZQ2020&485=ypfq,sss,123&7034=DPanxtjbh&";

void FillHeadForRequest(const std::string &request, char *realRequest, int &reqSize)
{
	PB_FrameHead head{};
	head.sign = 69;
	head.crypt = 0;
	head.PackageNum = 1;
	head.PackageNo = 0;
	head.PackageSize = static_cast<unsigned short>(request.length() + sizeof(PB_FrameHead));

	::memcpy(realRequest, &head, sizeof(PB_FrameHead));
	::memcpy(realRequest + sizeof(PB_FrameHead), request.c_str(), request.length());
	reqSize = head.PackageSize;
}

string qry_contract_request = "3=6018&51=HDTest&58=123456&6=1&7=1\n";

string qry_order_request = "3=6019&51=HDTest&58=123456&6=1&7=1\n";

string qry_trade_request = "3=6013&51=HDTest&58=123456&6=1&7=1\n";

string qry_position_request = "3=6014&51=HDTest&58=123456&6=1&7=1\n";

string qry_seat_request = "3=6040&51=HDTest&58=123456&6=1&7=1\n";
string qry_fund_request = "3=6012&51=HDTest&58=123456&6=1&7=1\n";

vector<string> g_Requests;
vector<string> prepare_requests()
{
	vector<string> requests;

	auto fill = [&requests](const string &one_req) -> string
	{
		string tmp = "5=" + to_string(requests.size() + 1) + "&" + one_req;
		return tmp;
	};

	requests.emplace_back(fill(login_request));

	requests.emplace_back(fill(qry_contract_request));
	requests.emplace_back(fill(qry_order_request));
	requests.emplace_back(fill(qry_trade_request));
	requests.emplace_back(fill(qry_seat_request));

	requests.emplace_back(fill(qry_position_request));
	requests.emplace_back(fill(qry_fund_request));

	return requests;
}

void HandleData(int sock)
{
	/*char realReq[2045]{};
	int reqLen = sizeof(realReq);
	FillHeadForRequest(requests[0], realReq, reqLen);
	int ret = send(sock, realReq, reqLen, 0);
	if (ret > 0)
	{
		fmt::print(fg(fmt::color::forest_green), "Send data success:\n{}\n", realReq);
	}
	return;*/

	thread t1([&sock]()
			  {
			do
			{
				for (const auto& req : g_Requests)
				{
					int ret = send(sock, req.c_str(), req.length(), 0);
					if (ret > 0)
					{
						//fmt::print(fg(fmt::color::forest_green), "Send data success:\n{}\n", req);
					}
					this_thread::sleep_for(chrono::milliseconds(10));
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			} while (true); });

	thread t2([&sock]()
			  {
			timeval timeout{ 0, 1000 };
			fd_set fs_read;
			do
			{
				//this_thread::sleep_for(10ms);
				FD_ZERO(&fs_read);
				FD_SET(sock, &fs_read);

				int ret = select(sock + 1, &fs_read, nullptr, nullptr, &timeout);
				if (ret == 0)
					continue;
				if (ret < 0)
				{
					cout << "error happend\n";
					return false;
				}

				if (FD_ISSET(sock, &fs_read))
				{
					char buf[512]{};
					int ret = recv(sock, buf, sizeof(buf), 0);
					if (ret == -1)
					{
						cout << "Something error happended\n";
						return false;
					}
					else
					{
						fmt::print(fg(fmt::color::yellow_green), "Recieve data success:\n{}\n", buf);
					}
				}
			} while (true); });

	t1.detach();
	t2.detach();
}

int main(int argc, char *argv[])
{
#ifdef Win32
	WSADATA wsaData;
	int ret = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (ret != 0)
	{
		cout << "WSAStratup 网络库初始化失败\n";
		exit(-1);
	}
#endif

	if (argc > 1)
	{
		server_port = atoi(argv[1]);
	}

	sockaddr_in addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server_port);

	addr.sin_addr.s_addr = inet_addr(server_addr);
	// addr.sin_addr.S_un.S_addr = inet_addr(server_addr);

	int cli_sock = ::socket(AF_INET, SOCK_STREAM, 0);
	if (cli_sock < 0)
	{
		cout << "Create socket failed..\n";
		exit(-1);
	}

	int conn_ret = connect(cli_sock, (sockaddr *)(&addr), sizeof(sockaddr));
	if (conn_ret < 0)
	{
		cout << "Connect failed..\n";
		exit(-1);
	}

	g_Requests = prepare_requests();

	HandleData(cli_sock);

	int d = getchar();
#ifdef Win32
	WSACleanup();
#endif
	return 0;
}