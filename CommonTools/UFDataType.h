/*
@history 2016年5月27日 created by Frank
@brief	定义系统内各种基本类型
*/
#ifndef FRANK_UFDATATYPE_H
#define FRANK_UFDATATYPE_H

#include <string>
#include <string.h>

/////////////////////////////////////////////////////////////////////////
///CUFStringType<int length>是一个数据字符串模版类，通过指定长度，
///可以得到真正可以使用的类。CUFStringType<int length>实质上就是根据长度，
///直接保存字符串的值，左对齐，最后加'\0'。
/////////////////////////////////////////////////////////////////////////
template <int length> class UFStringType
{
public:
	///构造函数
	UFStringType(void)
	{
		m_buffer[0] = '\0';
	}

	///复制构造子
	///@param	s	构造的源对象
	UFStringType(const UFStringType<length>& s)
	{
		SetValue(s.Data());
	}

	///复制构造子
	///@param	p	构造的源对象
	UFStringType(const char *p)
	{
		SetValue(p);
	}

	///等于操作符
	///@param	s	等于的值
	///@return	self
	UFStringType & operator =(const UFStringType<length>& s)
	{
		SetValue(s.Data());
		return *this;
	}

	///等于操作符，从字符串中获取
	///@param	p	等于的字符串
	///@return	self
	UFStringType& operator =(const char *p)
	{
		SetValue(p);
		return *this;
	}
	

	///清除字符串内容
	void Clear(void)
	{
		m_buffer[0]='\0';
	}

	///获取字符串的值
	///@return	字符串的值
	const char *GetValue(void) const
	{
		return m_buffer;
	}

	//返回数据指针
	char *Data()
	{
		return m_buffer;
	}

	///判断是否为空
	///@return	true表示为空，false表示不为空
	bool Empty(void) const
	{
		return m_buffer[0] == '\0';
	}

	///判断是否等于某个字符串
	///@param	r	右边项
	///@return	true表示是，false表示否
	bool operator == (const UFStringType<length> &r) const
	{
		return strcmp(m_buffer, r.Data()) == 0;
	}

	///判断是否等于某个字符串
	///@param	r	右边项
	///@return	true表示是，false表示否
	bool operator == (const char *r) const
	{
		return strcmp(m_buffer, r)==0;
	}

	///判断是否不等于某个字符串
	///@param	r	右边项
	///@return	true表示是，false表示否
	bool operator != (const UFStringType<length> &r) const
	{
		return strcmp(m_buffer, r.Data()) != 0;
	}

	///判断是否不等于某个字符串
	///@param	r	右边项
	///@return	true表示是，false表示否
	bool operator != (const char *r) const
	{
		return strcmp(m_buffer, r)!=0;
	}

	int Legth()
	{
		return length;
	}
private:
	///设置字符串的值
	///@param	s	要设置的字符串
	void SetValue(const char *s)
	{
		if (s == nullptr)
			m_buffer[0] = '\0';
		else
		{
			strncpy(m_buffer, s, length);
			m_buffer[length] = '\0';
		}
	}

protected:
	///存储字符串的值
	char m_buffer[length+1];
};

#endif
