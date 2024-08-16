#pragma once

#include "SString.hpp"

namespace TestRValue
{
	/*vector<SString> s_Vec;

	void InsertElem(SString&& elem)
	{
		s_Vec.emplace_back(std::forward<SString>(elem));
	}

	void TestInsertRVal()
	{
		s_Vec.reserve(16);
		SString s{ "first" };
		InsertElem(std::move(s));
		InsertElem(std::move(s));
		InsertElem("second"s);
	}*/

	SString &&r_take(SString &&msg)
	{
		cout << "begin r_take\n";
		return std::forward<SString>(msg);
	}

	SString &l_take(SString &msg)
	{
		cout << "begin l_take\n";
		return msg;
	}

	SString NewStr(string &str)
	{
		SString s(std::move(str));

		cout << "分割线\n";
		return s;
	}

	void TestUseRreturn()
	{
		static SString msgs{
			"first"};
		cout << "-------------------------\n";
		l_take(msgs).Print();
		cout << "-------------------------\n";
		r_take(std::move(msgs)).Print();

		/*vector<SString> msgs;

		string src = "12345";
		msgs.push_back(NewStr(src));*/
	}

	// map<int, SString> mapMsgs;
	// void put(int id, SString&& msg)
	//{
	//	fmt::print("put(int id, SString&& msg)\n");
	//	mapMsgs.emplace(id, std::forward<SString>(msg));
	// }

	// void put(int id, const SString& msg)
	//{
	//	fmt::print("put(int id, const SString& msg)\n");
	//	//mapMsgs.emplace(id, msg);
	//	mapMsgs.insert({ id, msg });
	// }

	// void Test()
	//{
	//	cout << "-----------------------\n";
	//	SString s("first"s);
	//	cout << "-----------------------\n";
	//	put(1, s);
	//	cout << "-----------------------\n";
	//	s = SString("second"s);
	//	cout << "-----------------------\n";
	//	put(2, std::move(s));
	//	cout << "-----------------------\n";
	// }
	/*
	void TestUniquePtr()
	{
		unique_ptr<string> ptr = make_unique<string>("123456");
		unique_ptr<string> ptr2 = make_unique<string>("654321");
		unique_ptr<string> ptr3 = make_unique<string>("abcdefghhijk");

		vector<unique_ptr<string>> mvec;
		mvec.emplace_back(std::move(ptr));
		mvec.emplace_back(std::move(ptr2));
		mvec.emplace_back(std::move(ptr3));

		for (const auto& ptr : mvec)
		{
			cout << *ptr << endl;
		}
	}

	void TestSTDEmplace()
	{
		vector<map<int, string>> itemsVec;

		map<int, string> items{
			{1, "first"s},
			{2, "second"s},
			{3, "third"s}
		};

		itemsVec.push_back(items);
		cout << itemsVec[0][2] << endl;
		itemsVec.emplace_back(items);
		cout << itemsVec[1][2] << endl;
		itemsVec.emplace_back(std::move(items));
		cout << items.size() << endl;

		itemsVec.emplace_back(items);
		itemsVec.push_back(items);

		cout << itemsVec.size() << endl;
	}

	vector<map<int, string>> s_answer;

	vector<map<int, string>>&& GetAnswer()
	{
		map<int, string> one{
			{1,"first"},
			{11,"eleven"}
		};
		map<int, string> two{
			{2,"second"},
			{22,"twenty-two"},
			{12,"twelve"}
		};

		s_answer.emplace_back(std::move(one));
		s_answer.emplace_back(std::move(two));

		assert(one.size() == 0);
		assert(two.empty());

		return std::move(s_answer);
	}

	void TestReturnRvalue()
	{
		auto inner_print = [](const auto& answer)
		{
			for_each(answer.begin(), answer.end(), [](auto smap) {
				for (auto& elem : smap)
				{
					fmt::print("{}={},", elem.first, elem.second);
				}
				fmt::print("\n");
				});
		};

		auto answer = GetAnswer();

		assert(s_answer.empty());

		fmt::print("dst answer:\n");
		inner_print(answer);
	}*/

	using FuncType = std::function<void()>;

	/*void TestFunctionRVal()
	{
		vector<FuncType> funcVec;

		funcVec.emplace_back(std::move(TestReturnRvalue));
		funcVec.emplace_back(std::move(TestSTDEmplace));
		funcVec.emplace_back([]() {cout << "I am the third function\n"; });

		for (const auto& func : funcVec)
		{
			func();
		}

		TestSTDEmplace();
	}*/

	void TestLiteralType()
	{
		auto self_move = [](std::string &&str) -> std::string
		{
			return std::move(str);
		};

		for (int i = 0; i < 10; ++i)
			fmt::print("{}\n", self_move("move string"));
	}

	void UserRvalueReturn(int &&row)
	{
		auto func = [](int &row)
		{
			row = (int)100;
		};

		func(row);
	}

	void TestForwardUse()
	{
		int row = 0;
		UserRvalueReturn(std::forward<int>(row));
		printf("row = %d\n", row);
	}
}

namespace MoveSemanticsTest
{
	class A
	{
	public:
		A() = default;
		A(A &&rhs) noexcept
		{
			cout << "using move construct.\n";
		}

		A &operator=(A &&rhs) noexcept
		{
			cout << "using move assign.\n";
			return *this;
		}
	};

	void func(A param)
	{
		cout << "using func...\n";
		A a1;
		a1 = std::move(param);
	}

	void test()
	{
		A a;
		// func(a); //error
		func(std::move(a));
	}
}

namespace StringOnlyMove_Test
{
	void Test()
	{
		StringOnlyMove s1;
		StringOnlyMove s2;
		// s1 = s2; //error!
		// assert(!s2.Empty());
		s1 = std::move(s2); // use move assgin
		assert(s2.Empty());
		assert(!s1.Empty());

		// StringOnlyMove s3 = s1; //error!
		StringOnlyMove s4 = std::move(s1);
	}
}

namespace TaskQueueTest
{
	template <typename T>
	class TaskQueue
	{
	public:
		void Insert(T &&task)
		{
			tasks_.push(std::forward<T>(task));
		}

		T Get()
		{
			T t = std::move(tasks_.front());
			tasks_.pop();
			return t;
		}

	private:
		std::queue<T> tasks_;
	};

	void Test()
	{
		TaskQueue<std::string> q1;
		q1.Insert("first"s);
		q1.Insert("second"s); // 不需要自动推导，所以不再是universal reference
		q1.Insert("third"s);

		assert(q1.Get() == "first");
		assert(q1.Get() == "second");

		TaskQueue<StringOnlyMove> q2;
		q2.Insert(StringOnlyMove("one"));
		q2.Insert(StringOnlyMove("two"));
		q2.Insert(StringOnlyMove("three"));

		assert(q2.Get().to_string() == "one");
		assert(q2.Get().to_string() == "two");
	}
}

namespace TestUniveralRef
{
	template <typename T>
	auto Merge(T &&a, T &&b)
	{
		if (std::is_lvalue_reference<T &&>::value)
		{
			cout << "左值\n";
		}
		else if (std::is_rvalue_reference<T &&>::value)
		{
			cout << "右值\n";
		}
		return std::forward<T>(a) + std::forward<T>(b);
	}

	void Test()
	{
		int a = 10, b = 20;
		cout << Merge(a, b) << endl;

		std::string s1{"one "};
		std::string s2{"two "};
		cout << Merge(std::move(s1), std::move(s2)) << endl;

		SString ss1("first");
		SString ss2("second");
		Merge(ss1, ss2).Print();
	}
}