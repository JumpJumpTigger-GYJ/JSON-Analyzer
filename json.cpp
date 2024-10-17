#include"json.h"
#include "parser.h"
#include<stdexcept>
#include<sstream>
#include<stdexcept>

using namespace gyj::json;



//浅拷贝构造函数
Json::Json(const Json& other) :m_type(other.m_type) {
	switch (m_type)
	{
	case json_null:
		break;
	case json_bool:
		m_bool = other.m_bool;
		break;
	case json_int:
		m_int = other.m_int;
		break;
	case json_double:
		m_double = other.m_double;
		break;
	case json_string:
		new (&m_string) std::shared_ptr<std::string>(other.m_string);
		break;
	case json_array:
		new (&m_array) std::shared_ptr<std::vector<Json>>(other.m_array);
		break;
	case json_object:
		new (&m_object) std::shared_ptr<std::map<std::string, Json>>(other.m_object);
		break;
	default:
		throw std::invalid_argument("invalid argument of Json type!");
	}
}


//析构函数
Json::~Json() {
	switch (m_type) {
	case json_string:
		m_string.reset();
		break;
	case json_array:
		m_array.reset();
		break;
	case json_object:
		m_object.reset();
		break;
	default:
		break;
	}

	m_type = json_null;
}	


Json& Json::operator =(const Json& rhs) {
	if (m_type != rhs.m_type) {
		clear();
		m_type = rhs.m_type;
	}

	switch (m_type)
	{
	case json_null:
		break;
	case json_bool:
		m_bool = rhs.m_bool;
		break;
	case json_int:
		m_int = rhs.m_int;
		break;
	case json_double:
		m_double = rhs.m_double;
		break;
	case json_string:
		new (&m_string) std::shared_ptr<std::string>(rhs.m_string);
		break;
	case json_array:
		new (&m_array) std::shared_ptr<std::vector<Json>>(rhs.m_array);
		break;
	case json_object:
		new (&m_object) std::shared_ptr<std::map<std::string, Json>>(rhs.m_object);
		break;
	default:
		throw std::invalid_argument("invalid argument of Json type!");
	}

	return *this;
}


//转换函数
Json::operator bool() {
	if (m_type != json_bool){
		throw std::logic_error("type error, not bool value!");
	}
	return m_bool;
}


Json::operator int() {
	if (m_type != json_int) {
		throw std::logic_error("type error, not int value!");
	}
	return m_int;
}


Json::operator double() {
	if (m_type != json_double) {
		throw std::logic_error("type error, not double value!");
	}
	return m_double;
}


Json::operator std::string() {
	if (m_type != json_string) {
		throw new std::logic_error("type error, not string value!");
	}
	return *m_string;
}


Json& Json::operator[](size_t index) {
	if (m_type != json_array) {
		throw std::logic_error("Json object of non array can't be subscripted!");
	}

	if (index >= m_array->size()) {
		throw std::out_of_range("subscript out of range!");
	}

	return (*m_array)[index];
}


void Json::make_array(size_t sz) {
	if (m_type != json_array) {
		clear();
		m_type = json_array;
		m_array = std::make_shared<std::vector<Json>>(sz);
	}
	else m_array->resize(sz);
}


size_t Json::array_size() const {
	if (m_type != json_array) {
		throw std::logic_error("object is not a Json array type!");
	}
	return m_array->size();
}


void Json::push_back(const Json& other) {
	if (m_type != json_array) {
		throw std::logic_error("non array Json object has no member function 'push_back'!");
	}
	m_array->push_back(other);
}


void Json::remove(size_t index) {
	if (m_type != json_array) {
		throw std::logic_error("non array type Json can't be subscripted!");
	}

	if (!has(index)) {
		throw std::out_of_range("subscript out of range!");
	}
	m_array->erase(m_array->begin() + index);
}


bool Json::has (size_t index) const {
	if (m_type != json_array) {
		throw std::logic_error("non array type Json can't be subscripted!");
	}
	if (index >= array_size()) return false;
	return true;
}


std::string Json::str() const {
	std::stringstream ss;

	switch (m_type)
	{
	case json_null:
		ss << "null";
		break;
	case json_bool:
		ss << std::boolalpha << m_bool << std::noboolalpha;
		break;
	case json_int:
		ss << m_int;
		break;
	case json_double:
		ss << m_double;
		break;
	case json_string:
		ss << "\"" << *m_string << "\"";
		break;

	case json_array:
		ss << '[';
		for (const auto& j : *m_array) {
			ss << j.str() << ',';
		}
		ss.seekp(-1, std::stringstream::end);
		ss << ']';
		break;

	case json_object:
		ss << '{';
		for (const auto& pair : *m_object) {
			ss << '\"' << pair.first << "\":" << pair.second.str() << ',';
		}
		ss.seekp(-1, std::stringstream::end);
		ss << '}';
		break;

	default:
		throw std::invalid_argument("invalid argument of Json type!");
		break;
	}

	return ss.str();
}


Json& Json::operator[](const std::string& key) {
	if (m_type != json_object) {
		throw std::logic_error("non object Json can't be subscripted by 'const char*'!");
	}
	return (*m_object)[key];
}


bool Json::has(const std::string& s) const {
	if (m_type != json_object) {
		throw std::logic_error("non object type Json can't be subscripted by 'string'!");
	}
	return m_object->find(s) != m_object->end();
}


void Json::remove(const std::string& s) {
	if (m_type != json_object) {
		throw std::logic_error("non object type Json can't be subscripted by 'string'!");
	}
	m_object->erase(s);
}


void Json::make_object() {
	if (m_type != json_object) {
		clear();
		m_type = json_object;
		m_object = std::make_shared<std::map<std::string, Json>>();
	}
}


bool Json::operator ==(const Json& other) const {
	if (m_type != other.m_type) return false;

	switch (m_type)
	{
	case json_null:
		return true;
	case json_bool:
		return m_bool == other.m_bool;
	case json_int:
		return m_int == other.m_int;
	case json_double:
		return m_double == other.m_double;
	case json_string:
		return m_string == other.m_string;
	case json_array:
		return m_array == other.m_array;
	case json_object:
		return m_object == other.m_object;
	default:
		throw std::invalid_argument("invalid argument of Json type!");
	}
}


bool Json::operator !=(const Json& other) const {
	return !(*this == other);
}


bool Json::asBool() const {
	if (m_type != json_bool) {
		throw std::logic_error("not a bool Json object!");
	}
	return m_bool;
}


int Json::asInt() const {
	if (m_type != json_int) {
		throw std::logic_error("not a int Json object!");
	}
	return m_int;
}


double Json::asDouble() const {
	if (m_type != json_double) {
		throw std::logic_error("not a double Json object!");
	}
	return m_double;
}


std::string Json::asString() const {
	if (m_type != json_string) {
		throw std::logic_error("not a string Json object!");
	}
	return *m_string;
}


Json& Json::operator[](const char* ptr) {
	if (m_type != json_object) {
		throw std::logic_error("non object type Json can't be subscripted by 'const char*'!");
	}
	return (*m_object)[std::string(ptr)];
}


bool Json::has(const char* ptr) const {
	if (m_type != json_object) {
		throw std::logic_error("non object type Json can't be subscripted by 'const char*'!");
	}
	return has(std::string(ptr));
}


void Json::remove(const char* ptr) {
	if (m_type != json_object) {
		throw std::logic_error("non object type Json can't be subscripted by 'const char*'!");
	}
	remove(std::string(ptr));
}


void Json::remove(int index) {
	remove(static_cast<size_t>(index));
}


Json& Json::operator[](int index) {
	return (*m_array)[static_cast<size_t>(index)];
}


bool Json::has(int index) const {
	return has(static_cast<size_t>(index));
}


void Json::parse(const std::string& s) {
	Parser p;
	*this = p.load(s).parse();
}


void Json::parse(std::string&& s) {
	Parser p;
	*this = p.load(std::move(s)).parse();
}