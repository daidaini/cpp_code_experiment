#include "ThreadBase.h"

namespace pobo
{
    ThreadBase::ThreadBase(const std::string &name)
        : m_Name(name)
    {
    }

    ThreadBase::~ThreadBase()
    {
    }

    void ThreadBase::Start()
    {
        // m_ShouldRun为True，说明已经启动了，不能再启动了
        if (m_ShouldRun)
            return;

        m_ShouldRun = true;
        m_Thread = std::thread(&ThreadBase::ThreadProc, this);
    }

    void ThreadBase::Stop()
    {
        m_ShouldRun = false;
    }

    void ThreadBase::Join()
    {
        if (m_Thread.joinable())
            m_Thread.join();
    }

    void ThreadBase::ThreadProc()
    {
#ifdef WIN32
        m_ThreadID = GetCurrentThreadId();
#else
        m_ThreadID = ::syscall(SYS_gettid);
#endif

        printf("[ThreadBase]创建线程[%d]:%s\n", m_ThreadID, m_Name.c_str());

        InitThread();

        while (m_ShouldRun)
        {
            this->Run();
        }
    }
}