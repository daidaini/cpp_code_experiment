#pragma once

#include "CommInc.h"

#include "TestRvalue.hpp"

/*fold expression
 */
inline namespace folding_expr
{
	template <typename... T>
	auto my_sum(T... t)
	{
		return (... + t); // 右折叠
		// return (t + ...); //左折叠
	}

	void TestSum()
	{
		std::cout << my_sum(1, 0.2, 3, 4, 5, 6, 7, 8, 9, 10) << std::endl;
	}

	template <typename... Args>
	auto average(Args... t)
	{
		return (t + ...) / sizeof...(t); // 这个sizeof的用法？
	}

	void test_average()
	{
		cout << average(1, 2, 3, 4, 5, 6, 7, 8, 9, 10.0) << endl;
	}
}

/*variadic templates
 */
void print_out()
{
	cout << endl;
}

template <typename Head, typename... T>
void print_out(Head h, T... data)
{
	cout << h << " , ";
	print_out(data...);
}

// void test_print_out()
//{
//	TradePushData data;
//
//	data.BSType = '1';
//
//	strcpy_s(data.BSTypeName, sizeof(data.BSTypeName), "卖");
//	//strcpy_s(data.ContractID, sizeof(data.ContractID), "600000");
//	data.Volume = 1000;
//
//	print_out(data.BSType, data.BSTypeName, data.ContractID, data.Volume);
// }

/*if/switch 变量声明强化
 */
void test_if_assign()
{
	vector<int> vec{1, 2, 3, 4, 6};

	if (const auto it = std::find(vec.begin(), vec.end(), 3);
		it != vec.end())
	{
		cout << *it << endl;
	}
}

void test_switch_assign()
{
	vector<int> vec{1, 2, 3, 4, 6};

	switch (auto it = vec[1]; it)
	{
	case 1:
		cout << "first\n";
		break;
	case 2:
		cout << "second\n";
		break;
	default:
		break;
	}
}

/*结构化绑定，简化tuple的使用
 */
tuple<int, double, string> get_results()
{
	return make_tuple(100, 3.1415926, "I am waiting for the reuslts");
}

void test_get_tuple_return()
{
	auto [one, two, three] = get_results();

	cout << one << endl;
	cout << two << endl; // double 的精度 只有5位？
	cout << three << endl;
}

/*if constexpr
 */
template <typename T>
auto print_type_info(const T &t)
{
	if constexpr (std::is_integral<T>::value)
	{
		return t + 1;
	}
	else if (std::is_floating_point<T>::value)
	{
		return t + 3.14;
	}
	else
		return t;
}

/*structured binding
 */
template <typename Key, typename Value, typename F>
void update(std::map<Key, Value> &m, F fool)
{
	for (auto &[key, value] : m)
		value = fool(key);

	// for (auto& elem : m)
	//	elem.second = fool(elem.first);
}

void test_update()
{
	map<string, long long int> m{
		{"a"s, 1},
		{"b"s, 2},
		{"c"s, 3}};
	update(m, [](string key)
		   { return hash<string>{}(key); });

	for (auto &&[key, value] : m)
		cout << key << " : " << value << endl;
}

void StructedBindTest()
{
	tuple my_tup{1, 3.0, "five"};

	auto &[f, s, t] = my_tup;

	t = "Friday";

	cout << f << " " << s << " " << t << "\n";

	cout << std::get<2>(my_tup) << endl;
}

struct Point
{
	int x;
	int y;
};

Point func()
{
	return {1, 2};
}

void StructedBindTest_1()
{
	int arr[3] = {1, 3, 5};
	auto [a, b, c] = arr;
	cout << a << "," << c << "," << b << "\n";

	const auto [x, y] = func();
	cout << x << " " << y << endl;
}

void VariantTest()
{
	variant<int, string> var("hello");

	variant<int, string> var_2(2021);

	var = 20210301;
	var_2 = "world";

	cout << std::get<int>(var) << endl;

	cout << std::get<1>(var_2) << endl;
}

void ApplyTest()
{
	auto add_lambda = [](auto first, auto second, auto third)
	{ return first + second + third; };

	cout << std::apply(add_lambda, std::make_tuple(10.3, 11, 12.45)) << endl;

	cout << std::apply(add_lambda, std::make_tuple(10, 11, 12)) << endl;
}

void ApplyTest2()
{
	auto print = [](auto src1, auto src2)
	{ cout << "reuslt = " << src1 << endl
		   << src2 << endl; };

	std::apply(print, std::make_tuple(12, "Saturday"));

	std::invoke(print, 11, "anything");
}

struct Foo
{
	Foo(int first, float second, string third)
	{
		cout << first << ", " << second << ", " << third << endl;
	}
};

int mul(int x)
{
	return x * x;
}

void InvokeTest()
{
	cout << std::invoke(mul, 2) << endl;
}

void SpliceMapTest()
{
	map<int, string> m{{1, "one"}, {2, "second"}, {3, "three"}};

	/*从这个例子可以看出来，extract 返回的是一个右值引用
	 */
	auto e = m.extract(2);

	e.key() = 4;
	e.mapped() = "four";

	m.insert(std::move(e));

	for (auto &[key, val] : m)
	{
		cout << key << " = " << val << endl;
	}
}

void DoOneAction(int x)
{
	int sum = 0;
	for (int i = 0; i < x; ++i)
	{
		sum += i;
	}
	cout << sum << endl;
}

void ExcutionTest()
{
	using namespace std::chrono;

	vector<int> my_vec;
	for (int i = 1; i <= 10000; i++)
	{
		my_vec.emplace_back(i);
	}

	cout << "begin\n";
	auto begin = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	std::for_each(my_vec.begin(), my_vec.end(), [](auto x)
				  { DoOneAction(x); });
	// std::for_each(std::execution::par, my_vec.begin(), my_vec.end(), [](int x) {DoOneAction(x); });
	// std::sort(std::execution::par, my_vec.begin(), my_vec.end(), [](auto a, auto b) {return a > b; });

	cout << "end: " << duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - begin << endl;
}

struct paper_question_info
{
	int question_no;		  // 试卷编号
	int choice_no;			  // 选项序号
	char choice_content[512]; // 选项内容
};

std::optional<std::vector<paper_question_info>> GetQuestionChoices(int flag)
{
	std::vector<paper_question_info> qeustions;

	if (flag == 0)
		return nullopt;

	if (flag == 1)
	{
		;
	}

	if (flag == 2)
	{
		paper_question_info data;
		strncpy(data.choice_content, "first", 5);
		data.choice_no = 1;
		data.question_no = 11;
		qeustions.emplace_back(data);

		strncpy(data.choice_content, "second", 6);
		data.choice_no = 2;
		data.question_no = 22;
		qeustions.emplace_back(data);
	}

	return qeustions;
}

void OptionalTest()
{
	auto result = GetQuestionChoices(2);

	cout << typeid(decltype(result)).name() << endl;

	if (result.has_value())
	{
		for (const paper_question_info &rec : result.value())
		{
			cout << rec.choice_content << " " << rec.question_no << " " << rec.choice_no << endl;
		}
	}
}

std::vector<std::string> SplitV(std::string_view src, char separator)
{
	std::vector<std::string> result;

	size_t pos = 0;
	do
	{
		auto sep_pos = src.find_first_of(separator, pos);
		if (sep_pos == src.npos)
			break;

		result.emplace_back(src.substr(pos, sep_pos - pos));
		pos = sep_pos + 1;
	} while (true);

	return result;
}

std::vector<std::string> Split(const std::string &src, char separator)
{
	std::vector<std::string> result;

	size_t pos = 0;
	do
	{
		auto sep_pos = src.find_first_of(separator, pos);
		if (sep_pos == src.npos)
			break;

		result.emplace_back(src.substr(pos, sep_pos - pos));
		pos = sep_pos + 1;
	} while (true);

	return result;
}

std::vector<std::string_view> SplitVW(std::string_view src, char separator)
{
	std::vector<std::string_view> result;

	size_t pos = 0;
	do
	{
		auto sep_pos = src.find_first_of(separator, pos);
		if (sep_pos == src.npos)
			break;

		result.emplace_back(src.substr(pos, sep_pos - pos));
		pos = sep_pos + 1;
	} while (true);

	return result;
}

void TestStringView()
{
	string src = "1234567890abcdefghijklmn"s;
	string_view src_view = src;
	cout << src_view.at(10) << endl;
	cout << typeid(src_view.data()).name() << endl;
}

optional<tuple<char, int, string>> GetOpResult(int flag)
{
	if (flag == 0)
		return std::nullopt;

	else if (flag == 1)
		return std::optional<tuple<char, int, string>>{{'1', 1, "first"}};

	else if (flag == 2)
		return std::optional<tuple<char, int, string>>{{'2', 2, "second"}};

	return nullopt;
}

void test_use_optinal(int flag)
{
	auto result = GetOpResult(flag);
	if (result == std::nullopt)
		cout << "get no result\n";
	else
	{
		auto &[a, b, c] = result.value();
		cout << a << " " << b << " " << c << endl;
	}
}

inline namespace test_filesystem
{
	namespace fs = std::filesystem;
	std::uintmax_t ComputeFileSize(const fs::path &pathToCheck)
	{
		if (fs::exists(pathToCheck) && fs::is_regular_file(pathToCheck))
		{
			auto err = std::error_code{};
			auto filesize = fs::file_size(pathToCheck, err);
			if (filesize != static_cast<uintmax_t>(-1))
				return filesize;
		}

		return static_cast<uintmax_t>(-1);
	}

	void DisplayFileInfo(const filesystem::directory_entry &entry, std::string &lead,
						 filesystem::path &filename)
	{
		// time_t cftime = chrono::system_clock::to_time_t(fs::last_write_time(entry));
		cout << lead << " " << filename << '\n';
		//<< ComputeFileSize(entry);
		//<< ", time: " << std::asctime(std::localtime(&cftime));
	}

	// 递归遍历目录
	void DisplayDirTree(const filesystem::path &pathToShow, int level)
	{
		if (filesystem::exists(pathToShow) && filesystem::is_directory(pathToShow))
		{
			auto lead = string(level * 3, ' ');
			for (const auto &entry : filesystem::directory_iterator(pathToShow))
			{
				auto filename = entry.path().filename();
				if (filesystem::is_directory(entry.status()))
				{
					cout << lead << "[+] " << filename << endl;
					DisplayDirTree(entry, level + 1);
					cout << '\n';
				}
				else if (filesystem::is_regular_file(entry.status()))
				{
					DisplayFileInfo(entry, lead, filename);
				}
				else
				{
					cout << lead << " [?]" << filename << '\n';
				}
			}
		}
	}

	// tuple的set
	void TestTupleSet()
	{
		set<tuple<int, string, double>> mySet{
			{4, "4", 4.0},
			{2, "20", 20.0},
			{9, "9", 9.0},
			{2, "2", 2.0},
			{8, "8", 8.0}};
		/*mySet.insert(make_tuple(4, "4", 4.0));
		mySet.insert(make_tuple(2, "20", 20.0));
		mySet.insert(make_tuple(9, "9", 9.0));
		mySet.insert(make_tuple(2, "2", 2.0));
		mySet.insert(make_tuple(8, "8", 8.0));*/

		for_each(mySet.begin(), mySet.end(), [](const auto &elem)
				 { fmt::print("{},{},{}\n", std::get<0>(elem), std::get<1>(elem), std::get<2>(elem)); });

		auto it = mySet.find(make_tuple(8, "8", 8.0));
		if (it != mySet.end())
			fmt::print("\n{},{},{}\n", std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
		else
			fmt::print("\nNot Find\n");
	}
}

// void test_form_chars()
//{
//	for (std::string_view const str : {"1.5 foo", "20 bar22", "42 ", "5000000000"}) {
//		std::cout << "String: " << std::quoted(str) << ". ";
//
//		float result{};
//
//		auto [ptr, ec] { std::from_chars(str.data(), str.data() + str.size(), result) };
//
//		if (ec == std::errc())
//		{
//			std::cout << "Result: " << result << ", ptr -> " << std::quoted(ptr) << '\n';
//		}
//		else if (ec == std::errc::invalid_argument)
//		{
//			std::cout << "That isn't a number.\n";
//		}
//		else if (ec == std::errc::result_out_of_range)
//		{
//			std::cout << "This number is larger than an int.\n";
//		}
//	}
// }

void test_quoted()
{
	string_view src = " ab|c ";

	cout << std::quoted(src, '|') << endl;
}

void test_lambda_alias()
{
	using namespace TestRValue;
	vector<SString> ss;
	ss.reserve(16);
	ss.emplace_back("First");
	ss.emplace_back("Second");
	ss.emplace_back("Third");

	SString toCmp{"othre"s};

	auto comp = [&lhs = toCmp](const SString &rhs)
	{
		return lhs._data == rhs._data;
	};

	cout << "********************************\n";
	cout << (std::find_if(ss.begin(), ss.end(), comp) != ss.end()) << endl;
}