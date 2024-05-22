#pragma once

#include "CommInc.h"

namespace my_lock_free
{
	/*使用TAS 实现下 spinlock
	 */

	class SpinLock final
	{
		using this_type = SpinLock;

	public:
		SpinLock() = default;
		~SpinLock() = default;

		SpinLock(const this_type &) = delete;
		SpinLock &operator=(const this_type &) = delete;

	public:
		void lock() noexcept
		{
			while (m_lock.test_and_set())
			{
				;
			}
		}

		// 返回 false表示 获取失败，true表示获取成功
		bool try_lock() noexcept
		{
			return !m_lock.test_and_set();
		}

		void unlock() noexcept
		{
			m_lock.clear();
		}

	private:
		std::atomic_flag m_lock; // = ATOMIC_FLAG_INIT;
	};

	SpinLock lock;
	int geneartor = 0;
	void myFunc()
	{
		lock.lock();
		cout << this_thread::get_id() << " index = " << ++geneartor << endl;
		lock.unlock();
	}

	void test_use_spinlock()
	{
		for (int i = 0; i < 20; ++i)
			thread(myFunc).detach();
	}
}