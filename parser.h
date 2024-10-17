#pragma once

#include"json.h"
#include<string>


namespace gyj {
namespace json {

class Parser {

public:
	typedef std::string::size_type size_type;

	Parser() :m_idx(0) {}
	Parser& load(const std::string&);
	Parser& load(std::string&&);

	Json parse();

private:
	Json parse_null();
	Json parse_bool();
	Json parse_number();
	std::string parse_string();
	Json parse_array();
	Json parse_object();

private:
	std::string m_str;
	size_type m_idx;

};


}//namespace json
}//namespace gyj