#pragma once

#include "boost/asio.hpp"

#include <iostream>
#include <string>
#include <deque>
#include <mutex>

constexpr int max_length = 1024;
class AsioClient
{
public:
	AsioClient(boost::asio::io_context &io_context, const std::string &host, const std::string &port)
		: io_context_(io_context),
		  socket_(io_context),
		  resolver_(io_context)
	{
		connect(host, port);
	}

	void start_repeating_timer(int seconds, std::function<void()> callback)
	{
		timer_ = std::make_unique<boost::asio::deadline_timer>(io_context_);
		repeat_timer(seconds, callback);
	}

	void stop_timer()
	{
		if (timer_)
		{
			timer_->cancel();
		}
	}

	void send(const std::string &message)
	{
		io_context_.post(
			[this, message]()
			{
				bool write_in_progress = !write_msgs_.empty();
				write_msgs_.push_back(message + "\n");
				if (!write_in_progress)
				{
					do_write();
				}
			});
	}

	void close()
	{
		io_context_.post([this]()
						 { socket_.close(); });
	}

private:
	void connect(const std::string &host, const std::string &port)
	{
		/*
		auto endpoints = resolver_.resolve(host, port);

		boost::asio::async_connect(socket_, endpoints,
								   [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
								   {
									   if (!ec)
									   {
										   do_read();
									   }
									   else
									   {
										   std::cerr << "Connect error: " << ec.message() << std::endl;
									   }
								   });
		*/

		resolver_.async_resolve(host, port,
								[this](boost::system::error_code ec, boost::asio::ip::tcp::resolver::results_type endpoints)
								{
									std::cout << "async_resolve cb thread = " << ::syscall(SYS_gettid) << '\n';
									if (!ec)
									{
										boost::asio::async_connect(socket_, endpoints,
																   [this](boost::system::error_code ec, boost::asio::ip::tcp::endpoint)
																   {
																	   if (!ec)
																	   {
																		   do_read();
																	   }
																	   else
																	   {
																		   std::cerr << "Connect error: " << ec.message() << std::endl;
																	   }
																   });
									}
								});
	}

	void do_read()
	{
		socket_.async_read_some(boost::asio::buffer(read_buffer_, max_length),
								[this](boost::system::error_code ec, std::size_t length)
								{
									std::cout << "async_read_some cb thread = " << ::syscall(SYS_gettid) << '\n';
									if (!ec)
									{
										std::cout.write(read_buffer_, length);
										std::cout << std::endl; // 添加换行符以分隔消息
										do_read();				// 继续读取下一条消息
									}
									else
									{
										std::cerr << "Read error: " << ec.message() << std::endl;
										socket_.close();
									}
								});
	}

	void do_write()
	{
		boost::asio::async_write(socket_,
								 boost::asio::buffer(write_msgs_.front()),
								 [this](boost::system::error_code ec, std::size_t /*length*/)
								 {
									 std::cout << "async_write cb thread = " << ::syscall(SYS_gettid) << '\n';
									 if (!ec)
									 {
										 write_msgs_.pop_front();
										 if (!write_msgs_.empty())
										 {
											 do_write();
										 }
										 else
										 {
											 do_read();
										 }
									 }
									 else
									 {
										 socket_.close();
									 }
								 });
	}

	void repeat_timer(int seconds, std::function<void()> callback)
	{
		timer_->expires_from_now(boost::posix_time::seconds(seconds));
		timer_->async_wait([this, seconds, callback](const boost::system::error_code &ec)
						   {
							std::cout << "timer async_wait cb thread = " << ::syscall(SYS_gettid) << '\n';
			if (!ec) {
				callback();
				repeat_timer(seconds, callback);
			} });
	}

	std::unique_ptr<boost::asio::deadline_timer> timer_;

	boost::asio::io_context &io_context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::resolver resolver_;
	char read_buffer_[max_length];
	std::deque<std::string> write_msgs_;
};
