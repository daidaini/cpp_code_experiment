#pragma once

#include "SString.hpp"

namespace FunctionTest
{
	class Demo
	{
	public:
		static int Func1(int no)
		{
			cout << no << endl;
			return no + 1;
		}

		int Func2(int no)
		{
			cout << no << endl;
			return no + 2;
		}
	};

	auto Func3 = [](int no) -> int
	{
		cout << no << endl;
		return no + 3;
	};

	int Func4(int no)
	{
		cout << no << endl;
		return no + 4;
	}

	using FuncType = std::function<int(int)>;

	void Test()
	{
		map<int, FuncType> funcs;
		funcs.emplace(1, Demo::Func1);

		Demo demo;
		funcs.emplace(2, std::bind(&Demo::Func2, &demo, placeholders::_1));

		funcs.emplace(3, Func3);
		funcs.emplace(4, Func4);

		assert(funcs[1](3) == 4);
		assert(funcs[2](3) == 5);
		assert(funcs[3](3) == 6);
		assert(funcs[4](3) == 7);
	}
}

/*测试场景1：
 * 使用std::function，SString作为参数，线程一赋值，线程二获取。测试构造函数等调用情况
 * 结论:
 *
 */

namespace FunctionWithThread
{
	using FuncType = std::function<void(SString)>;

	void DoFunc(SString s)
	{
		// fmt::print("Recv: {}\n", fmt::ptr(s.data_.data()));
		s.Print();

		this_thread::sleep_for(std::chrono::seconds(1));
	}

	void ThreadFunc_1()
	{
		SString str("abcdefg");
		// fmt::print("Start: {}\n", fmt::ptr(str.data_.data()));

		// 3次 move construct 1次 copy construct
		//有点多...
		// thread t = std::thread(std::bind(DoFunc, std::move(str)));
		// t.join();

		// 3次 move construct
		std::thread(DoFunc, std::move(str)).detach();

		// std::thread([&str](){ DoFunc(std::move(str)); }).detach();
		// assert(str.Empty()); //error，捕获的时候，该函数已经运行结束

		// std::thread([s = std::move(str)](){ DoFunc(std::move(s)); }).detach();
		// printf("after thread\n");
		// assert(str.Empty());

		// this_thread::sleep_for(std::chrono::seconds(5));
	}

	void Test()
	{
		ThreadFunc_1();

		getchar();
	}
}

/*测试 std::bind 函数参数时的消耗，
 * 只会调用 move construct
 * 所以上例中的copy construct，只会在std::thread的场景下出现？
 */
namespace FunctionParam
{
	void PrintString(SString str)
	{
		printf("(3)..................\n");
		str.Print();
	}

	using FuncType = std::function<void(SString)>;

	void DoWithFuncParam(FuncType func, SString src)
	{
		printf("(2)..................\n");
		func(std::move(src));
	}

	void Test()
	{
		SString src("This is test for copy or move with bind");
		printf("(1)..................\n");
		DoWithFuncParam(
			std::bind(PrintString, placeholders::_1), std::move(src));
	}
}

namespace BindFunctionTest
{
	class Obj
	{
	public:
		Obj(int mem) : member_(mem)
		{
		}

		void MemFunc_Raw()
		{
			cout << member_ << endl;
		}

		void MemFunc_Square(int a)
		{
			cout << a * a + member_ << endl;
		}

		void MemFunc_Multiply(int a, int b)
		{
			cout << member_ - a * b << endl;
		}

	private:
		int member_;
	};


	using FuncType = std::function<void()>;
	map<int, FuncType> functions;

	/*测试总结：
	* 当使用std::bind来绑定成员函数到std::function的时候，结合测试结果，有以下结论：
	* 1）bind 必须第一个参数为成员的指针，第二个参数为类对象的指针，后续跟上对应实际的入参
	* 2）可以确定，bind操作是运行期的操作，只是将相关的参数传入到std::function内部，因为除了成员函数的指针是静态的，剩下的参数都是动态可变的。
	*/
	void Test_BindMemberFunction()
	{
		Obj one(1);
		Obj two(20);
		Obj three(300);
		
		functions.emplace(1, std::bind(&Obj::MemFunc_Raw, &one));
		functions.emplace(2, [&two]()
						  { two.MemFunc_Square(5); });
		functions.emplace(3, std::bind(&Obj::MemFunc_Multiply, &three, 10, 5));

		functions[1]();	//1
		
		functions[2]();	//45
		functions[3](); //250
	}
}