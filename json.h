#pragma once

#include<string>
#include<vector>
#include<map>
#include<memory>



namespace gyj {
namespace json {


class Json {

public:

	Json() :m_type(json_null) {};
	Json(bool val) :m_type(json_bool), m_bool(val) {}
	Json(int val) :m_type(json_int), m_int(val) {}
	Json(double val) :m_type(json_double), m_double(val) {}
	Json(const std::string& s) :m_type(json_string), m_string(std::make_shared<std::string>(s)) {}
	Json(const char* ptr) :m_type(json_string), m_string(std::make_shared<std::string>(ptr)) {}
	Json(const Json&);
	~Json();

	Json& operator =(const Json&);

	//转换运算符
	operator bool();
	operator int();
	operator double();
	operator std::string();
	
	
	bool operator ==(const Json&) const;
	bool operator !=(const Json&) const;


	bool isNull() const { return m_type == json_null; }
	bool isBool() const { return m_type == json_bool; }
	bool isInt() const { return m_type == json_int; }
	bool isDoubel() const { return m_type == json_double; }
	bool isString() const { return m_type == json_string; }
	bool isArray() const { return m_type == json_array; }
	bool isObject() const { return m_type == json_object; }


	bool asBool() const;
	int asInt() const;
	double asDouble() const;
	std::string asString() const; 



	//对象操作
	Json& operator[](const char*);
	Json& operator[](const std::string&);
	bool has(const char*) const;
	bool has(const std::string&) const;
	void remove(const char*);
	void remove(const std::string&);
	void make_object();


	//数组操作
	void remove(size_t);
	void remove(int);
	bool has(size_t) const;
	bool has(int) const;
	Json& operator[](size_t);
	Json& operator[](int);
	void push_back(const Json&);
	void make_array(size_t = 0);
	size_t array_size() const;


	void clear() { this->~Json(); }

	std::string str() const;

	void parse(const std::string&);
	void parse(std::string&&);

private:

	enum Type {
		json_null = 0,
		json_bool,
		json_int,
		json_double,
		json_string,
		json_array,
		json_object
	} m_type;


	union {
		bool m_bool;
		int m_int;
		double m_double;
		std::shared_ptr<std::string> m_string;
		std::shared_ptr<std::vector<Json>> m_array;
		std::shared_ptr<std::map<std::string, Json>> m_object;
	};

};



}//namespace json
}//namespace gyj