#pragma once

#include "CommInc.h"
#include "TimerLog.h"

template <typename T>
inline void hash_combine(std::size_t &seed, const T &val)
{
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <typename T>
inline void hash_val(std::size_t &seed, const T &val)
{
	hash_combine(seed, val);
}

template <typename T, typename... Types>
inline void hash_val(std::size_t &seed, const T &val, const Types &...args)
{
	hash_combine(seed, val);
	hash_val(seed, args...);
}

template <typename... Types>
inline std::size_t hash_val(const Types &...args)
{
	std::size_t seed = 0;
	hash_val(seed, args...);
	return seed;
}

class Elem
{
public:
	Elem()
	{
		cout << "Elem()" << endl;
	}

	Elem(const Elem &rhs)
	{
		cout << "Copy Elem()" << endl;
	}

	Elem operator=(const Elem &rhs)
	{
		cout << "Assignment Elem()" << endl;
		a = rhs.a;
		return *this;
	}

	int a = 10;
};

/*测试queue元素读取时，= 是否有拷贝行为
 */
void TestQueueRead()
{
	queue<Elem> my_q;
	Elem e1;
	Elem e2;
	Elem e3;
	Elem e4;

	my_q.push(e1);
	my_q.push(e2);
	my_q.push(e3);
	my_q.push(e4);

	Elem get = my_q.back();
	my_q.pop();

	cout << get.a << endl;
}

void TestArray()
{
	array<char, 10> my_arr{};
	char data[] = "12a3";
	memcpy(my_arr.data(), data, strlen(data));
	cout << my_arr.size() << " , " << my_arr.max_size() << endl;
	cout << my_arr.data() << endl;
}

void test_shrink_to_fit()
{
	/*不排序的vector 也能查找到
	 */
	vector<int> my_vec{4, 2, 18, 5, 10, 6, 319, 44};

	cout << my_vec.size() << "\n";
	my_vec.shrink_to_fit();
	cout << my_vec.size() << "\n";
}

void TestMultimap()
{
	multimap<string, int> accountUsers;
	accountUsers.emplace("test", 1);
	accountUsers.emplace("test", 2);
	accountUsers.emplace("test", 3);
	accountUsers.emplace("test", 4);

	for (const auto &[account, user] : accountUsers)
	{
		cout << account << " = " << user << endl;
	}
}

void TestUnorderedMap()
{
	unordered_map<int, string> umap{{1, "one"}, {2, "two"}, {3, "three"}};

	auto it = umap.find(1);
	umap.erase(it);

	umap.emplace(1, "first");
	for (auto &[key, val] : umap)
	{
		cout << key << " = " << val << endl;
	}
}

void TestUnorderedMap_hash()
{
	struct Student
	{
		int Number = 0;
		string Name;
		double score = 0.0;

		bool operator==(const Student &rhs) const
		{
			return Name == rhs.Name && Number == rhs.Number;
		}
	};

	struct StudentHash
	{
		auto operator()(const Student &s) const
		{
			return hash_val(s.Name, s.Number, s.score);
		}
	};

	struct StudentEqual
	{
		auto operator()(const Student &lhs, const Student &rhs) const
		{
			return lhs.Name == rhs.Name && lhs.Number == rhs.Number;
		}
	};

	unordered_map<Student, string, StudentHash, StudentEqual> s;

	// Student 重载 operator==
	// unordered_map<Student, string, StudentHash> s;

	s.emplace(Student{11, "yubo"s, 100.0}, "Computer"s);
	s.emplace(Student{23, "James"s, 99.9}, "SF"s);
	s.emplace(Student{35, "Durent"s, 88.0}, "SF"s);
	s.emplace(Student{30, "Curry"s, 99.0}, "PG"s);

	for (const auto &[stu, info] : s)
	{
		cout << stu.Name << " : " << info << endl;
	}
}

map<int, string> record{{1, "first"}, {2, "second"}, {3, "third"}};

vector<map<int, string>> GetRec(map<int, queue<vector<map<int, string>>>> &records)
{
	return std::move(records[5].front());
}

void test_sub_map()
{
	map<int, queue<vector<map<int, string>>>> records;

	records.insert({3, {}});
	records.insert({5, {}});
	records.insert({8, {}});

	auto &user_rec = records[5];

	// map<int, string> one{ {1, "first"},{2, "second"}, {3,"third"} };
	user_rec.emplace(vector<map<int, string>>{record});
	record.emplace(5, "five");

	user_rec.emplace(vector<map<int, string>>{record});

	cout << records[5].size() << endl;
	cout << records[5].front().size() << endl;

	auto rec_get = GetRec(records);
	cout << rec_get[0][3] << endl;

	if (records[5].front().empty())
	{
		cout << "move success\n";
		records[5].pop();
	}

	cout << records[5].size() << endl;
}

void TestMapOfSharedPtr()
{
	using keyType = shared_ptr<string>;
	using valType = string;
	keyType ptr1 = make_unique<string>("123456");
	keyType ptr2 = make_unique<string>("abcdef");

	map<keyType, valType> my_map;

	my_map.emplace(ptr1, "123456"s);
	my_map.emplace(ptr2, "abcdef"s);

	for (const auto &[key, val] : my_map)
	{
		fmt::print("key={},val={}\n", *key, val);
	}
}

void test_insert_or_assign()
{
	map<int, string> mps{
		{1, "first"s},
		{2, "second"s},
		{3, "third"s},
		{4, "fourth"s},
	};

	mps.insert_or_assign(3, "three"s);
	mps.insert_or_assign(5, "fifth"s);

	cout << mps[3] << endl;
	cout << mps[5] << endl;
}

template <typename T>
void AddParam(int id, T &&val)
{
	if constexpr (std::is_arithmetic_v<T>)
	{
		record.emplace(id, std::to_string(val));
	}
	else
	{
		record.emplace(id, std::forward<T>(val));
	}
}

template <typename T>
void AddParam(int id, T &val)
{
	if constexpr (std::is_arithmetic_v<T>)
	{
		record.emplace(id, std::to_string(val));
	}
	else
	{
		record.emplace(id, std::forward<T>(val));
	}
}

constexpr int namev = 123456;

void test_add_elem_generic()
{
	AddParam(5, 5.0);
	AddParam(6, 60000);
	AddParam(7, namev); // 左值参数
	AddParam(8, "eight");
	AddParam(3, "three"s);
	const char *val_1 = "nineth";
	AddParam(9, val_1);
	char val_2[12] = "twelve";
	AddParam(12, val_2);
}

void use_add_elem_generic()
{
	for (const auto &[id, val] : record)
	{
		cout << id << " = " << val << '\n';
	}
}

namespace vector_test
{
	struct Data_Copy
	{
		Data_Copy(const string &rhs) : Elem(rhs) {}
		Data_Copy(const Data_Copy &rhs)
		{
			cout << "Data_Copy(const Data_Copy& rhs)\n";
			Elem = rhs.Elem;
		}
		std::string Elem;
	};

	struct Data_Move
	{
		Data_Move(const string &rhs) : Elem(rhs) {}
		Data_Move(Data_Move &&rhs) noexcept
		{
			cout << "Data_Move(Data_Move&& rhs)\n";
			Elem.swap(rhs.Elem);
		}

		Data_Move(const Data_Move &) = default;

		/*Data_Move(const Data_Move& rhs)
		{
			cout << "Data_Move(const Data_Move& rhs)\n";
			Elem = rhs.Elem;
		}*/

		std::string Elem;
	};

	void Test()
	{
		Data_Move data_mv("12345"s);

		// Data_Copy data_cp("12345"s);

		{
			cout << "Data_Move ...\n";
			vector<Data_Move> mv_vec;
			mv_vec.reserve(2);
			mv_vec.emplace_back(data_mv);
			mv_vec.emplace_back(data_mv);
			mv_vec.emplace_back(data_mv);
		}
		/*	{
				cout << "Data_Copy ...\n";
				vector<Data_Copy> cp_vec;
				cp_vec.emplace_back(data_cp);
				cp_vec.emplace_back(data_cp);
			}*/
	}

	void TestRemove()
	{
		vector<int> vec{
			10, 20, 5, 14, 3, 25, 9, 99, 204, 0};

		size_t front = 0, back = vec.size() - 1;
		do
		{
			if (vec[front] <= 10)
				std::swap(vec[front], vec[back--]);
			else
				++front;
		} while (front < back);

		int nowsize = vec.size();
		for (size_t i = back + 1; i < nowsize; ++i)
		{
			vec.pop_back();
		}

		std::sort(vec.begin(), vec.end());
		auto its = std::equal_range(vec.begin(), vec.end(), 99);
		if (its.first != its.second)
			cout << "find .. " << *(its.first) << endl;

		for_each(vec.begin(), vec.end(), [](int elem)
				 { cout << elem << ", "; });
	}

	void MoveInRange()
	{
		vector<string> one{"1", "2", "3", "4", "5", "6"};

		vector<vector<string>> two;
		two.push_back(one);
		two.push_back(one);
		two.push_back(one);
		two.push_back(one);
		two.push_back(one);

		vector<string> splt;
		for (auto &elem : two)
		{
			for (auto &item : elem)
			{
				splt.emplace_back(std::move(item));
			}
		}

		for_each(splt.begin(), splt.end(), [](string elem)
				 { cout << elem << ", "; });
	}
}