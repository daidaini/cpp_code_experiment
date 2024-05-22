#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>

template<class T>
class Singleton
{
private:
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
public:
	static T& Instance()
	{
		static std::once_flag flag;
		std::call_once(flag, [&]()
		{
			static T instance;
			m_Instance = &instance;
		});
		return *m_Instance;
	}
protected:
	Singleton() = default;
	~Singleton() = default;

private:
	static T* m_Instance;
};

template<class T>
T* Singleton<T>::m_Instance;

#endif/*SINGLETON_H*/