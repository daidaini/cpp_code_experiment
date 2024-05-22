#include <boost/regex.hpp>
#include <iostream>
#include <string>

int main()
{
	const std::string srcstr = "MI;IIP=192.168.6.25;IPORT=12636;LIP=192.168.6.22;MAC=ABCDEFABCDEF;IDFV=BFBFF000306F2;";
	boost::regex regex_pattern(".*IIP=(.*?);.*");
	boost::cmatch results;
    	bool issuccess = boost::regex_match(srcstr.c_str(), results, regex_pattern);

	std::cout << issuccess <<'\n';

	return 0;
}
