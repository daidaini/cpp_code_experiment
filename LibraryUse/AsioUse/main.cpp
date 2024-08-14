#include <iostream>
#include <thread>
#include "asio_client.h"

int main()
{
	try
	{
		boost::asio::io_context io_context;
		AsioClient client(io_context, "localhost", "8888");

		std::thread t([&io_context]()
					  { io_context.run(); });

		int seconds = 3;
		client.start_repeating_timer(seconds, [seconds]()
									 { std::cout << "timer running within " << seconds << " seconds..\n"; });

		std::string message;
		while (std::getline(std::cin, message))
		{
			if (message == "quit")
			{
				break;
			}
			client.send(message + "\n");

			client.stop_timer();
		}

		client.close();
		t.join();
	}
	catch (std::exception &e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}