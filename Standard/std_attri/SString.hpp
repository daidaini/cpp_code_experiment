#pragma once

#include "CommInc.h"

class SString
{
public:
	SString() = default;

	explicit SString(const char* elem) : data_(elem)
	{
		//fmt::print("using const char* construct\n");
	}

	explicit SString(std::string&& elem) : data_(std::forward<std::string>(elem))
	{
		//fmt::print("using rref construct\n");
	}

	explicit SString(const std::string& elem) : data_(elem)
	{
		//fmt::print("using const ref construct\n");
	}

	//移动构造
	SString(SString&& rhs) noexcept
	{
		data_ = std::move(rhs.data_);
		//data_.swap(rhs.data_);

		//fmt::print("SString move construct\n");
	}

	//拷贝构造
	SString(const SString& rhs)
	{
		data_ = rhs.data_;

		//fmt::print("SString copy construct\n");
	}

	//拷贝赋值
	SString& operator=(const SString& rhs)
	{
		data_ = rhs.data_;
		//fmt::print("SString copy assign\n");
		return *this;
	}

	//移动赋值
	SString& operator=(SString&& rhs) noexcept
	{
		data_ = std::move(rhs.data_);
		//fmt::print("SString move assign\n");
		return *this;
	}

	void Print()
	{
		if (data_.empty())
		{
			fmt::print("empty!\n");
		}
		else
		{
			fmt::print("{}\n", data_);
		}
	}

	bool Empty() const
	{
		return data_.empty();
	}

	void Assign(std::string&& src)
	{
		data_ = std::forward<std::string>(src);
	}

	std::string GetStr() const
	{
		return data_;
	}

private:
	std::string data_;
};

SString operator+(const SString& lhs, const SString& rhs)
{
	SString result;
	result.Assign(lhs.GetStr() + rhs.GetStr());
	return result;
}

class StringOnlyMove final
{
public:
	StringOnlyMove() {
		data_ = new char[1024];
	}

	explicit StringOnlyMove(const char* s) {
		data_ = new char[strlen(s) + 1];
		strcpy(data_, s);
	}

	~StringOnlyMove() {
		if (data_ != nullptr)
		{
			delete data_;
			data_ = nullptr;
		}
	}

	/*
	StringOnlyMove(const StringOnlyMove& rhs) {
		if (rhs.data_ == nullptr)
			return;

		cout << "copy constructing..\n";
		data_ = new char[1024];
		::memcpy(this->data_, rhs.data_, 1024);
	}
	*/

	StringOnlyMove(StringOnlyMove&& rhs) noexcept {
		if (rhs.data_ == nullptr)
			return;

		cout << "move constructing..\n";
		data_ = rhs.data_;
		rhs.data_ = nullptr;
	}

	/*
	StringOnlyMove& operator=(const StringOnlyMove& rhs)
	{
		if (rhs.data_ == nullptr || data_ == nullptr)
			return *this;

		cout << "copy assigning..\n";
		::memcpy(this->data_, rhs.data_, 1024);
		return *this;
	}
	*/

	StringOnlyMove& operator=(StringOnlyMove&& rhs) noexcept {
		if (rhs.data_ == nullptr || data_ == nullptr)
			return *this;

		cout << "move assigning..\n";
		data_ = rhs.data_;
		rhs.data_ = nullptr;
		return *this;
	}

	bool Empty() const
	{
		return data_ == nullptr;
	}

	std::string to_string() const
	{
		std::string output;
		if (data_ != nullptr)
		{
			output.assign(data_);
		}
		return output;
	}
private:
	char* data_ = nullptr;
};