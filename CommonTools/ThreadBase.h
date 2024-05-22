#pragma once

#include <thread>
#include <string>
#include <unistd.h>
#include <sys/syscall.h>

namespace pobo
{
	class ThreadBase
	{
	public:
		explicit ThreadBase(const std::string &name);
		virtual ~ThreadBase();

		void Start();
		void Stop();
		void Join();

	protected:
		void ThreadProc();
		virtual void InitThread() {}
		virtual void Run() = 0;

	protected:
		int m_ThreadID = -1;
		std::thread m_Thread;
		std::string m_Name;

		bool m_ShouldRun = false;
	};
}