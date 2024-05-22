#include "CommInc.h"

using namespace std::chrono;

namespace ThreadTest
{
	condition_variable my_cv;
	mutex my_mt;

	void tFunc()
	{
		cout << this_thread::get_id() << " -- This is tFunc\n";
		for (int i = 0; i < 10; ++i)
		{
			my_cv.notify_one();
			cout << this_thread::get_id() << " -- Notify once\n";
			this_thread::sleep_for(chrono::microseconds(200));
		}
	}

	void tFuncW()
	{
		cout << this_thread::get_id() << " -- This is tFuncW\n";

		for (int i = 0; i < 10; ++i)
		{
			unique_lock<mutex> lock(my_mt);
			my_cv.wait_for(lock, chrono::seconds(5));
			cout << this_thread::get_id() << " -- I Got it once\n";
			this_thread::sleep_for(chrono::seconds(1));
		}
	}

	void TestCv()
	{
		thread t1(tFunc);
		thread t2(tFuncW);

		t1.join();
		t2.join();
	}

	void Test()
	{
		cout << "current thread count = " << std::thread::hardware_concurrency() << endl;
	}
}

namespace ThreadLocalTest
{
	static int g_cnt = 1;

	//测试结果：对于thread_local变量，无论加不加static修饰，它在一个线程中都只会被初始化一次。
	void ThreadFunc()
	{
		//static thread_local int t_cnt = ++g_cnt;
		thread_local int t_cnt = ++g_cnt;
		for(int i = 0; i<10; ++i)
		{
			this_thread::sleep_for(chrono::seconds(t_cnt));
			cout << this_thread::get_id() << " : " << t_cnt << '\n';
		}
		if(t_cnt > 10)
		{
			return;
		}
		ThreadFunc();
	}

	void Test()
	{
		for(int i= 0; i< 3; ++i)
		{
			thread(ThreadFunc).detach();
		}
		getchar();
	}
}