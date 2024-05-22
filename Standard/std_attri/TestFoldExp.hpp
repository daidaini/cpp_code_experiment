#pragma once

#include "CommInc.h"

inline namespace FoldExp
{
	auto FormatStr = [](auto &&...args)
	{
		((cout << args << endl), ...);
	};

	auto PlusElem = [](auto &&...args)
	{
		return (args + ...);
	};

	template <typename... T>
	void print(T... t)
	{
		//((cout << t << endl), ...);
		// cout << ((t), ...) << endl;	//123

		((cout << t << ' '), ...) << endl; // 3 4.3 123
	}

	template <typename... T>
	size_t multipy(T... t)
	{
		return (t * ...);
	}

	template <typename Tuple, size_t N>
	struct TupleReader
	{
		static void Read(const Tuple &t, string &logstr)
		{
			TupleReader<Tuple, N - 1>::Read(t, logstr);
			logstr += fmt::format(",{}", std::get<N - 1>(t));
		}
	};

	template <typename Tuple>
	struct TupleReader<Tuple, 1>
	{
		static void Read(const Tuple &t, string &logstr)
		{
			logstr += fmt::format("{}", std::get<0>(t));
		}
	};

	void Test()
	{
		// std::apply(FormatStr, std::make_tuple("1234"s, 9.92, "abcd"));
		// cout << PlusElem(13, 50.1, 44) << endl;

		// print(3, 4.3, "123");
		// cout << multipy(10, 10.0, 20.1, 4) << endl;

		auto params = make_tuple(3.3, "anything"s, 1001, 'a', "good");

		string logstr;
		TupleReader<decltype(params), 5>::Read(params, logstr);
		cout << logstr << endl;
	}

	void TestFmt()
	{
		char msg[1024]{};
		sprintf(msg, "\r\n  备兑不足预估提醒 \r\n\r\n  %8s\t%20s\t%20s\t%20s\t%20s\r\n",
				"证券代码", "应备兑总量", "不足数量", "通知日期", "备注信息");

		cout << msg;

		char msg2[1024]{};
		sprintf(msg2, "\r\n  %8s\t%20d\t%20d\t%20d\t%20s\r\n",
				"510050", 100, 99, 20210304, "extrar");
		cout << msg2;
		// fmt::print("\r\n  备兑不足预估提醒 \r\n\r\n  {:8s}\t{:20s}\t{:20s}\t{:20s}\t{:20s}\r\n",
		//	"证券代码", "应备兑总量", "不足数量", "通知日期", "备注信息");
		// cout << fmt::format("\r\n  {:8s}\t{:20d}\t{:20d}\t{:20s}\t{:20d}\r\n", "Mondy", 143051, 20201, "Janury", 99999); //可行
	}
}

namespace FizzBizz
{
	template <int N>
	constexpr void print()
	{
		if constexpr (N % 3 == 0 && N % 5 == 0)
			printf("FizzBizz\n");
		else if constexpr (N % 3 == 0)
			printf("Fizz\n");
		else if constexpr (N % 5 == 0)
			printf("Bizz\n");
	}
}