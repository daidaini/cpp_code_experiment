#pragma once

#include "CommInc.h"
#include "TimerLog.h"

using namespace std;

auto get_student(int id)
{
	// 返回类型被推断为 std::tuple<double, char, std::string>

	if (id == 0)
		return std::make_tuple(3.8, 'A', "张三");
	if (id == 1)
		return std::make_tuple(2.9, 'C', "李四");
	if (id == 2)
		return std::make_tuple(1.7, 'D', "王五");
	return std::make_tuple(0.0, 'D', "null");
	// 如果只写 0 会出现推断错误, 编译失败
}

void test_tuple()
{
	auto student = get_student(0);
	std::cout << "ID: 0, "
		<< "GPA: " << std::get<0>(student) << ", "
		<< "成绩: " << std::get<1>(student) << ", "
		<< "姓名: " << std::get<2>(student) << '\n';

	double gpa;
	char grade;
	std::string name;

	// 元组进行拆包
	std::tie(gpa, grade, name) = get_student(1);
	std::cout << "ID: 1, "
		<< "GPA: " << gpa << ", "
		<< "成绩: " << grade << ", "
		<< "姓名: " << name << '\n';
}

void test_tuple_new()
{
	auto [gpa, grade, name] = get_student(2);
	std::cout << "ID: 2, "
		<< "GPA: " << gpa << ", "
		<< "成绩: " << grade << ", "
		<< "姓名: " << name << '\n';
}

/*std::variant
*/
template <size_t n, typename... T>
constexpr std::variant<T...> _tuple_index(const std::tuple<T...>& tpl, size_t i)
{
	if constexpr (n >= sizeof...(T))
		throw std::out_of_range("越界.");
	if (i == n)
		return std::variant<T...>{ std::in_place_index<n>, std::get<n>(tpl) };
	return _tuple_index<(n < sizeof...(T) - 1 ? n + 1 : 0)>(tpl, i);
}

template <typename... T>
constexpr std::variant<T...> tuple_index(const tuple<T...>& tpl, size_t i)
{
	return _tuple_index<0>(tpl, i);
}

template<typename T0, typename ... Ts>
std::ostream& operator<< (std::ostream& s, std::variant<T0, Ts...> const& v)
{
	std::visit([&](auto&& x) {s << x; }, v);
	return s;
}

void test_tuple_index()
{
	auto student = get_student(2);

	int i = 2;
	cout << tuple_index(student, 2) << endl;;
}

template <typename T>
auto tuple_len(T& tpl) {
	return std::tuple_size<T>::value;
}

void test_tuple_cat()
{
	auto stu1 = get_student(0);

	auto new_tuple = std::tuple_cat(get_student(0), get_student(1), get_student(2));

	cout << tuple_len(new_tuple) << endl;

	// 迭代
	for (int i = 0; i != tuple_len(new_tuple); ++i)
		// 运行期索引
		std::cout << tuple_index(new_tuple, i) << std::endl;
}

string test_use_array(int type)
{
	if (type == 10)
		return {};

	static std::array<std::string, 6> ProductRisks{
		"", "R1低风险", "R2中低风险", "R3中等风险", "R4中高风险", "R5高风险"
	};

	static string delim(8, ' ');

	if (type > 5)
		type = 5;

	string result = ProductRisks[0];
	for (auto i = 1; i <= type; ++i)
	{
		result.append(delim);
		result.append(ProductRisks[i]);
	}

	return result;
}

inline namespace variant_test
{
	/*一元二次方程的求根公式，根可能是一个，也可能是两个，也可能没有根
	*/
	using Two = std::pair<double, double>;
	using Roots = std::variant<Two, double, void*>;
	/*
	Roots FindRoots(double a, double b, double c)
	{
		auto d = b * b - 4 * a * c;

		if (d > 0.0)
		{
			auto p = sqrt(d);
			return std::make_pair((-b + p) / 2 * a, (-b - p) / 2 * a);
		}
		else if (d == 0.0)
			return (-1 * b) / (2 * a);

		return nullptr;
	}
	*/
	struct RootPrinterVisitor
	{
		void operator()(const Two& roots) const
		{
			std::cout << "2 roots: " << roots.first << " " << roots.second << '\n';
		}
		void operator()(double root) const
		{
			std::cout << "1 root: " << root << '\n';
		}
		void operator()(void*) const
		{
			std::cout << "No real roots found.\n";
		}
	};

	auto VisitElem = [](const auto& result)
	{
		if (result.index() == 0)
			cout << "2 roots: " << std::get<Two>(result).first << " " << std::get<Two>(result).second << '\n';

		else if (result.index() == 1)
			cout << "1 root: " << std::get<double>(result) << '\n';

		else if (result.index() == 2)
			cout << "No real roots found.\n";
	};

	/*
	void TestVariant()
	{
		VisitElem(FindRoots(1, -2, 1));
		VisitElem(FindRoots(1, -3, 2));
		VisitElem(FindRoots(1, 0, 2));
	}
	*/
	struct Triangle
	{
		void Draw()const {
			//cout << "△" << endl;
		}
	};

	struct Circle
	{
		void Draw() const {
			cout << "O" << endl;
		}
	};

	void TestMulti()
	{
		using Draw = std::variant<Triangle, Circle>;
		Draw draw;
		vector<Draw> draw_list{ Triangle{},Circle{},Triangle{} };
		auto DrawVistor = [](const auto& t)
		{
			t.Draw();
		};
		for (const auto& item : draw_list)
		{
			std::visit(DrawVistor, item);
		}
	}
}

inline namespace SelfTypeMap
{
	struct SelfKey
	{
		string first_name;
		string second_name;
	};

	struct SelfLess
	{
		bool operator()(const SelfKey& lhs, const SelfKey& rhs) const
		{
			if (lhs.first_name < rhs.first_name)
				return true;
			else if (lhs.first_name > rhs.first_name)
				return false;
			return lhs.second_name < rhs.second_name;
		}
	};

	void test()
	{
		map<SelfKey, string, SelfLess> self_map;
		self_map.insert_or_assign(SelfKey{ "1","first" }, "one");
		self_map.insert_or_assign(SelfKey{ "1","second" }, "two");
		self_map.insert_or_assign(SelfKey{ "1","third" }, "three");
		self_map.insert_or_assign(SelfKey{ "1","first" }, "一");
		self_map.insert_or_assign(SelfKey{ "2","second" }, "two");

		for (const auto& [key, val] : self_map)
		{
			cout << key.first_name << " " << key.second_name << " = " << val << endl;
		}
	}
}

namespace MapTest
{
	using SellElemType = priority_queue<long long, std::deque<long long>, std::greater<long long>>;
	using BuyElemType = priority_queue<long long>;

	map<double, SellElemType, greater<double>> map_;
	void SelfCompare()
	{
		map_[3.0].push(100);
		map_[3.0].push(200);
		map_[3.0].push(300);
		map_[2.0].push(50);
		map_[4.4].push(30);
		map_[1.5].push(90);
		map_[1.5].push(22);
		map_[2.2].push(44);
		map_[3.6].push(100);

		/*for (auto& [key, val] : map_)
		{
			while (!val.empty())
			{
				fmt::print("{} = {}; ", key, val.top());
				val.pop();
			}
			fmt::print("\n");
		}*/
	}

	void TestEraseInLoop()
	{
		auto it = map_.begin();
		while (it != map_.end())
		{
			if (it->first > 3)
				it = map_.erase(it);
			else
				++it;
		}

		for (auto& [key, val] : map_)
		{
			while (!val.empty())
			{
				fmt::print("{} = {}; ", key, val.top());
				val.pop();
			}
			fmt::print("\n");
		}
	}

	/*
	//测试下来，当插入删除的频率高时，使用 deque的效率更好
	void UseVectorTest()
	{
		TimerLog t("UseVectorTest");
		for (int i = 0; i < 1000; ++i)
		{
			SelfCompare_b();
		}
	}

	void UseDequeTest()
	{
		TimerLog t("UseDequeTest");
		for (int i = 0; i < 1000; ++i)
			SelfCompare();
	}
	*/
}

namespace VectorTest
{
	void test_erase()
	{
		vector<int> src {1, 4, 9, 12, 3, 44, 556, 123, 13, 25, 9};

		auto it = std::find(src.begin(), src.end(), 126);
		if (it != src.end())
			src.erase(it);

		for (int e : src)
		{
			cout << e << ',';
		}
		cout << '\n';
	}

}