/*
 * @Descripttion: 
 * @Author: yubo
 * @Date: 2022-09-27 16:15:50
 * @LastEditTime: 2022-10-24 14:08:10
 */
#include "EchoClient.h"

#include <vector>

int main(int argc, char* argv[]) {
    // connect several clients
	//ip::tcp::endpoint ep(ip::address::from_string("106.14.183.12"), 8090);
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),20002);
    std::vector<std::string> names { "John", "James", "Lucy", "Tracy", "Frank", "Abby" };
    for (std::string& name : names) {
        talk_to_svr::start(ep, name);
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }

    service.run();
}
