#include "parser.h"
#include<cctype>
#include<stdexcept>
#include<sstream>

using namespace gyj::json;


Parser& Parser::load(const std::string& s) {
	m_str = s;
	m_idx = 0;
	return *this;
}


Parser& Parser::load(std::string&& s) {
	m_str = std::move(s);
	m_idx = 0;
	return *this;
}


Json Parser::parse() {
	while (m_idx != m_str.size() && isspace(m_str[m_idx])) ++m_idx;
	
	if (m_idx == m_str.size()) throw std::logic_error("invalid Json object!");

	char ch = m_str[m_idx];

	if (isdigit(ch) || ch == '+' || ch == '-') return parse_number();

	switch (ch){
	case'n':
		return parse_null();
	case't':
	case'f':
		return parse_bool();
	case'"':
		return Json(parse_string());
	case'[':
		return parse_array();
	case'{':
		return parse_object();
	default:
		throw std::logic_error("invalid Json object!");
	}
}


Json Parser::parse_null() {
	if (m_str.compare(m_idx, 4, "null")) {
		throw std::logic_error("invalid Json object!");
	}
	m_idx += 4;
	return Json();
}


Json Parser::parse_bool() {
	if (!m_str.compare(m_idx, 4, "true")) {
		m_idx += 4;
		return Json(true);
	}
	if (!m_str.compare(m_idx, 5, "false")) {
		m_idx += 5;
		return Json(false);
	}
	throw std::logic_error("invalid Json object!");
}



Json Parser::parse_number() {
	size_type pos;
	char ch;

	for (pos = m_idx; pos != m_str.size();) {
		ch = m_str[pos++];
		if (isdigit(ch) || ch == '.' || ch == '+' || ch == '-') continue;
		--pos;
		break;
	} 
	
	Json res = std::stod(m_str.substr(m_idx, pos - m_idx));

	m_idx = pos;

	return res;
}


std::string Parser::parse_string() { 
	auto ed = m_idx + 1;

	while (ed != m_str.size() && m_str[ed] != '"') ++ed;

	if (ed == m_str.size()) throw std::logic_error("invalid Json object!");

	std::string s = m_str.substr(m_idx + 1, ed - m_idx - 1);
	m_idx = ed + 1;

	return s;
}


Json Parser::parse_array() { 
	Json arr;
	arr.make_array();
	bool is_end = false;
	++m_idx;

	while (!is_end) {
		arr.push_back(this->parse());

		while (m_idx != m_str.size() && isspace(m_str[m_idx])) ++m_idx;
		
		if (m_idx == m_str.size()) throw std::logic_error("invalid Json object!");

		if (m_str[m_idx] == ',') ++m_idx;
		else if (m_str[m_idx] == ']') is_end = true;
		else throw std::logic_error("invalid Json object!");
	}

	++m_idx;

	return arr;
}


Json Parser::parse_object() { 
	Json obj;
	obj.make_object();
	bool is_end = false;
	++m_idx;

	while (!is_end) {
		Json key = this->parse();

		while (m_idx != m_str.size() && isspace(m_str[m_idx])) ++m_idx;

		if (m_idx == m_str.size() || m_str[m_idx] != ':') {
			throw std::logic_error("invalid Json object!");
		} 

		++m_idx;

		Json val = this->parse();

		while (m_idx != m_str.size() && isspace(m_str[m_idx])) ++m_idx;

		if(m_idx==m_str.size()) throw std::logic_error("invalid Json object!");

		if (m_str[m_idx] == ',') ++m_idx;
		else if (m_str[m_idx] == '}') is_end = true;
		else throw std::logic_error("invalid Json object!");

		obj[key.asString()] = val;
	}

	++m_idx;

	return obj;
}