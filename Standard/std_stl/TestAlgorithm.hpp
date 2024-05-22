#pragma once

#include "CommInc.h"

namespace ForEachTest
{
	struct Student
	{
		int Number;
		char Sex;
		std::string Name;
		double Score;

		void operator()(const Student &s)
		{
			fmt::print("{}[{}][{}] get score {}.\n", s.Name, s.Sex, s.Number, s.Score);
		}
	};

	void Test()
	{
		std::vector<Student> vec;
		vec.emplace_back(Student{1, 'f', "mary", 90.2});
		vec.emplace_back(Student{2, 'm', "dube", 94.2});
		vec.emplace_back(Student{3, 'm', "kevim", 80.6});
		vec.emplace_back(Student{4, 'f', "kelly", 50.1});

		std::for_each(
			vec.begin(), vec.end(), Student());

		cout << "------------------------------------------\n";
		std::for_each(vec.begin(), vec.end(), [](const Student &s)
					  { fmt::print("{}[{}][{}] get score {}.\n", s.Name, s.Sex, s.Number, s.Score); });
	}

	void Test_1()
	{
		std::vector<std::string> vec{
			"abcdef", "123456", "abcdef123456", "123456abcdef"};

		std::for_each(vec.begin(), vec.end(),
					  [](const std::string &str)
					  {
						  if (str.length() > 10)
						  {
							  std::cout << str << '\n';
						  }
					  });
	}
}

void TestAllOf()
{
	// map<int, int> my_map;
	vector<int> my_vec;

	for (size_t i = 0; i < 100; i++)
	{
		my_vec.push_back(i + 10);
	}

	for_each(my_vec.begin(), my_vec.end(), [](int elem)
			 { cout << elem << " "; });

	cout << endl;

	int toComp = 0;

	while (true)
	{
		cout << "??:\n";
		cin >> toComp;

		if (toComp == 0)
			break;

		cout << any_of(my_vec.begin(), my_vec.end(), [toComp](int elem)
					   { return elem == toComp; })
			 << endl;
	}
}

void TestStdMin()
{
	map<int, vector<int>> mivi{
		{3, {1, 2, 3}},
		{1, {1, 2, 3, 4, 5}},
		{4, {1}},
		{5, {1}},
		{2, {
				1,
				2,
				3,
				4,
			}}};

	/*
	auto it = std::min_element(mivi.begin(), mivi.end(),
		[](const auto& lhs, const auto& rhs) {return lhs.second.size() < rhs.second.size(); }
	);

	cout << "minimun element is " << it->first << endl;
	*/
}

inline namespace Algorithm
{
	struct CoveredQuota
	{
		std::string StockCode;
		// 已锁定数量
		int LockedVolume;
		// 可解锁数量
		int EnableVolume;
	};

	void test_std_find()
	{
		vector<CoveredQuota> cover_vec;
		CoveredQuota rec_2{
			"510050", 200, 100};
		CoveredQuota rec_1{
			"510300", 500, 300};
		CoveredQuota rec_3{
			"119300", 400, 200};
		CoveredQuota rec_4{
			"113300", 2000, 1000};

		cover_vec.emplace_back(rec_1);
		cover_vec.emplace_back(rec_2);
		cover_vec.emplace_back(rec_3);
		cover_vec.emplace_back(rec_4);

		string code = "510400";
		auto it = std::find_if(cover_vec.begin(), cover_vec.end(),
							   [&code](const CoveredQuota &elem)
							   { return elem.StockCode == code; });

		if (it != cover_vec.end())
		{
			fmt::print("find it\n");
		}
		else
		{
			fmt::print("No find\n");
		}
	}

	void test_partial_sum()
	{
		std::vector<int> v{2, 2, 2, 2, 2, 2, 2, 2, 2, 2};
		std::cout << "The first 10 even numbers are: ";
		std::partial_sum(v.begin() + 4, v.end(),
						 std::ostream_iterator<int>(std::cout, " "));
		std::cout << '\n';

		std::partial_sum(v.begin() + 4, v.end(), v.begin() + 4, std::multiplies<int>());
		std::cout << "The first 10 powers of 2 are: ";
		for (auto n : v)
		{
			std::cout << n << " ";
		}
		std::cout << '\n';
	}

	struct AdItem
	{
		std::string ad_id;
		int priority;
		int score;

		bool operator<(const AdItem &rhs) const
		{
			return ad_id.compare(rhs.ad_id) < 0;
		}

		bool operator==(const AdItem &rhs) const
		{
			return ad_id.compare(rhs.ad_id) == 0;
		}
	};

	void TestSort() // priority升序，score降序
	{
		vector<AdItem> vec{
			{"one", 2, 20},
			{"two", 4, 40},
			{"three", 1, 100},
			{"four", 5, 30},
			{"five", 1, 60},
			{"six", 6, 30},
			{"seven", 3, 40},
			{"eight", 3, 40},
			{"nine", 5, 50},
			{"nine", 4, 80}};

		/*std::sort(vec.begin(), vec.end(), [](const AdItem& lhs, const AdItem& rhs) {
			if (lhs.priority < rhs.priority)
				return true;
			else if (lhs.priority > rhs.priority)
				return false;
			return lhs.score > rhs.score;
			}
		);*/

		std::sort(vec.begin(), vec.end());

		AdItem toFind{"eight", 4, 4444};
		auto it = std::find(vec.begin(), vec.end(), toFind);
		if (it != vec.end())
			cout << "Find elem = " << it->ad_id << " " << it->score << endl;

		for_each(vec.begin(), vec.end(),
				 [](const AdItem &item)
				 {
					 cout << item.ad_id << ", " << item.priority << ", " << item.score << endl;
				 });
	}

	// 使用折半查找的算法测试
	void TestBinarySearch()
	{
		vector<int> vec{
			3, 13, 2, 11, 4, 9, 11, 8};
		std::sort(vec.begin(), vec.end());

		auto it_u = std::upper_bound(vec.begin(), vec.end(), 4);
		if (it_u != vec.end())
			cout << "指针指向比4大的最小的数,在数组中第" << it_u - vec.begin() << "个" << endl;
		else
			cout << "指定元素比数组中的所有数都大" << endl;

		cout << boolalpha;
		cout << std::binary_search(vec.begin(), vec.end(), 13) << '\n';

		auto its = std::equal_range(vec.begin(), vec.end(), 11);
		for (auto it = its.first; it != its.second; ++it)
		{
			cout << *it << '\n';
		}

		auto it_l = std::lower_bound(vec.begin(), vec.end(), 10);
		cout << *it_l << endl; // 不存在则返回第一个不小于要查询的值的位置
	}

	void TestBinarySearchSelf()
	{
		vector<AdItem> vec{
			{"one", 1, 100},
			{"two", 2, 20},
			{"three", 3, 30},
			{"four", 4, 40},
			{"five", 5, 50},
			{"six", 6, 60}};

		std::sort(vec.begin(), vec.end());

		cout << boolalpha;

		AdItem src1;
		src1.ad_id = "four";
		cout << std::binary_search(vec.begin(), vec.end(), src1) << '\n';

		src1.ad_id = "seven";
		cout << std::binary_search(vec.begin(), vec.end(), src1) << '\n';

		src1.ad_id = "three";
		cout << std::binary_search(vec.begin(), vec.end(), src1) << '\n';

		vector<AdItem> vec_new{
			{"seven", 7, 70},
			{"eight", 8, 80},
			{"nine", 9, 90}};

		vec.insert(vec.end(), vec_new.begin(), vec_new.end());
		for_each(vec.begin(), vec.end(), [](const AdItem &elem)
				 { cout << elem.ad_id << ", " << elem.score << ", " << elem.priority << "\n"; });

		// vector<AdItem> dst;
		// std::sort(vec_new.begin(), vec_new.end());
		////必须排序，否则会出错
		// std::merge(vec.begin(), vec.end(), vec_new.begin(), vec_new.end(), back_inserter(dst));
		// for_each(dst.begin(), dst.end(), [](const AdItem& elem) {
		//	cout << elem.ad_id << ", " << elem.score << ", " << elem.priority << "\n";
		//	});
	}
}