#pragma once

#include "../StdCommFiles.h"

class Widget //: public enable_shared_from_this<Widget>
{
public:
	Widget()
	{
		cout << "Widget()\n";
	}

	~Widget()
	{
		cout << "~Widget()\n";
	}

	Widget(const Widget &rhs)
	{
		cout << "Widget Copy\n";
	}

	shared_ptr<Widget> GetPtr()
	{
		return shared_ptr<Widget>(this);
		// return shared_from_this();
	}

	vector<int> m_members{4, 8, 16, 32};
};

void TestWeakPtr()
{
	auto spw = std::make_shared<Widget>();
	// after spw is constructed, the pointer-to Widget's ref count(RC) is 1.

	std::weak_ptr<Widget> wpw(spw);
	// wpw points to same Widget as spw. RC remains 1.

	spw = nullptr;
	// RC goes to 0, and the Widget is destoryed. wpw now dangles

	if (wpw.expired()) // if wpw desen't point to an object.
	{
		cout << "resource has already been destoryed\n";
	}
}

void AFunc(Widget &&one)
{
	cout << "Using a func\n";
}

void AFunc2(Widget &&two)
{
	// AFunc(two);//报错，无法将右值引用绑定到左值
	AFunc(std::forward<Widget>(two));
}

void TestForward()
{
	Widget src;
	// AFunc(move(src));
	AFunc2(move(src));
}

/*test using enable_shared_from_this
 */

// 比较推荐的写法
struct Good : std::enable_shared_from_this<Good> // note: public inheritance
{
	std::shared_ptr<Good> getptr()
	{
		return shared_from_this();
	}
};

// 错误的用法：用不安全的表达式试图获得 this 的 shared_ptr 对象
struct Bad
{
	std::shared_ptr<Bad> getptr()
	{
		return std::shared_ptr<Bad>(this);
	}
	~Bad() { std::cout << "Bad::~Bad() called\n"; }
};

void TestSharedPtrCp()
{
	Widget *w = new Widget;
	cout << w->m_members[2] << endl;

	Widget *w2 = w;
	cout << w2->m_members[2] << endl;

	auto w3 = w->GetPtr();
	cout << w3->m_members[2] << endl;

	delete w2;
}

void TestSharedFromThis()
{
	// 正确的用法: 两个 shared_ptr 共享同一个对象
	std::shared_ptr<Good> gp1 = std::make_shared<Good>();
	std::shared_ptr<Good> gp2 = gp1->getptr();
	std::cout << "gp2.use_count() = " << gp2.use_count() << '\n';

	// 错误的用法: 调用 shared_from_this 但其没有被 std::shared_ptr 占有
	try
	{
		Good not_so_good;
		std::shared_ptr<Good> gp1 = not_so_good.getptr();
	}
	catch (std::bad_weak_ptr &e)
	{
		// 在 C++17 之前，编译器不能捕获 enable_shared_from_this 抛出的std::bad_weak_ptr 异常
		// 这是在C++17之后才有的特性
		std::cout << e.what() << '\n';
	}

	// 错误的用法，每个 shared_ptr 都认为自己是对象的唯一拥有者
	// 调用错误的用法，会导致两次析构 Bad的对象，第二次析构时，指针指向的空间已经被析构，
	// 会导致程序出错
	// std::shared_ptr<Bad> bp1 = std::make_shared<Bad>();
	// std::shared_ptr<Bad> bp2 = bp1->getptr();
	// std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
}

namespace use_weak_ptr
{
	shared_ptr<vector<int>> container = make_shared<vector<int>>(vector<int>{4, 5, 6});

	void handle_func(int times)
	{
		static int mem = 100;
		weak_ptr<vector<int>> ptr = container;

		for (int i = 0; i < times; ++i)
		{
			// fmt::print("引用计数 = {}\n", ptr.use_count());
			if (!ptr.expired())
			{
				auto spr = ptr.lock();
				cout << this_thread::get_id() << " , use_count = " << spr.use_count() << endl;
			}
			// cout << this_thread::get_id() << " , use_count = " << ptr.use_count() << endl;

			this_thread::sleep_for(100ms);
		}
		// if (!ptr.expired())
		//	delete src;
	}

	class Res
	{
	public:
		Res()
		{
			spr = make_shared<vector<int>>();
			wptr = spr;
		}

		shared_ptr<vector<int>> GetRes()
		{
			return wptr.lock();
		}

		void Assign(int a)
		{
			if (!wptr.expired())
			{
				wptr.lock()->push_back(a);
			}
		}

		void Release()
		{
			spr = nullptr;
		}

	private:
		weak_ptr<vector<int>> wptr;
		shared_ptr<vector<int>> spr;
	};

	void DoTest()
	{
		Res r;
		r.Assign(10);
		r.Assign(20);

		{
			auto spr = r.GetRes();
			for (int elem : *spr)
			{
				cout << elem << endl;
			}
			cout << spr.use_count() << endl;
		}

		r.Release();

		{
			auto spr_2 = r.GetRes();
			if (spr_2 == nullptr)
			{
				cout << "resourse is clear\n";
				return;
			}

			cout << spr_2.use_count() << endl;
			for (int elem : *spr_2)
			{
				cout << elem << endl;
			}
		}
	}

	void DoTestWptr()
	{
		shared_ptr<vector<int>> spr = make_shared<vector<int>>(vector<int>{4, 5, 6});

		weak_ptr<vector<int>> wpr = spr;

		assert(!wpr.expired());

		cout << wpr.lock()->back() << endl;

		spr = nullptr;
		assert(wpr.expired());

		cout << (spr == nullptr) << endl;
	}
}

namespace use_unique_ptr
{
	class Widget
	{
	public:
		using ElemType = unique_ptr<string>;
		void push(ElemType &&elem)
		{
			_lists.emplace_back(std::forward<ElemType>(elem));
		}

		ElemType get()
		{
			ElemType elem = std::move(_lists.front());
			_lists.pop_front();
			return elem;
		}

		int size() const
		{
			return _lists.size();
		}

	private:
		list<ElemType> _lists;
	};

	void TestReturnUp()
	{
		Widget obj;
		obj.push(make_unique<string>("one"s));
		obj.push(make_unique<string>("two"s));
		obj.push(make_unique<string>("three"s));
		obj.push(make_unique<string>("four"s));

		auto test = [&obj]()
		{
			auto elem_1 = obj.get();
			auto elem_2 = obj.get();
			cout << typeid(elem_1).name() << endl;
			cout << *elem_1 << " , " << *elem_2 << endl;
			cout << obj.size() << endl;
			obj.push(std::move(elem_1));
			obj.push(std::move(elem_2));
			cout << obj.size() << endl;
		};

		test();
		test();
		test();
		test();
	}

	void up_deleter(string *ptr)
	{
		cout << "ready to delte :" << *ptr << endl;
		delete ptr;
	}

	void test_up_deleter()
	{
		function<void(string *)> myDeleter = up_deleter;
		unique_ptr<string, function<void(string *)>> up(new string("first"), myDeleter);

		cout << "end" << endl;
	}
}