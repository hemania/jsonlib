#ifndef JSON_H
#define JSON_H

#include <string>
#include <map>
#include <vector>

namespace JSON {
	class Value;
	class String;
	class Boolean;
	class Number;
	class Array;
	class Object;

	enum ValueType {
		VALUE_TYPE_STRING,
		VALUE_TYPE_ARRAY,
		VALUE_TYPE_OBJECT,
		VALUE_TYPE_BOOLEAN,
		VALUE_TYPE_NULL,
		VALUE_TYPE_NUMBER
	};

	class Value {
	protected:
		ValueType _type;

	public:
		Value() {
			_type = VALUE_TYPE_NULL;
		}

		virtual ~Value() {

		}

		String *asString() const {
			if (_type == VALUE_TYPE_NULL) {
				return 0;
			}

			if (_type != VALUE_TYPE_STRING) {
				throw std::exception();
			}

			return (String *)this;
		}

		Object *asObject() const {
			if (_type == VALUE_TYPE_NULL) {
				return 0;
			}

			if (_type != VALUE_TYPE_OBJECT) {
				throw std::exception();
			}

			return (Object *)this;
		}

		Array *asArray() const {
			if (_type == VALUE_TYPE_NULL) {
				return 0;
			}

			if (_type != VALUE_TYPE_ARRAY) {
				throw std::exception();
			}

			return (Array *)this;
		}

		Boolean *asBoolean() const {
			if (_type == VALUE_TYPE_NULL) {
				return 0;
			}

			if (_type != VALUE_TYPE_BOOLEAN) {
				throw std::exception();
			}

			return (Boolean *)this;
		}

		Number *asNumber() const {
			if (_type == VALUE_TYPE_NULL) {
				return 0;
			}

			if (_type != VALUE_TYPE_NUMBER) {
				throw std::exception();
			}

			return (Number *)this;
		}
	};

	class String : public Value, public std::string {
	public:
		String(const char *str) : std::string(str) {
			_type = VALUE_TYPE_STRING;
		}

		String(const std::string &str) : std::string(str) {
			_type = VALUE_TYPE_STRING;
		}

		virtual ~String() {

		}

		const std::string &asString() {
			return (*this);
		}
	};

	class Array : public Value, public std::vector < Value * > {
	public:
		Array() {
			_type = VALUE_TYPE_ARRAY;
		}

		virtual ~Array() {
			for (size_t i = 0; i < size(); i++) {
				delete (*this)[i];
			}
			(*this).clear();
		}

		const std::vector<Value *> &asVector() {
			return (*this);
		}
	};

	class Object : public Value, public std::map < std::string, Value * > {
	public:
		Object() {
			_type = VALUE_TYPE_OBJECT;
		}

		virtual ~Object() {
			Object::iterator it;
			for (it = (*this).begin(); it != (*this).end(); it++) {
				delete it->second;
			}
			(*this).clear();
		}

		const std::map<std::string, Value *> &asMap() {
			return (*this);
		}
	};

	class Boolean : public Value {
	private:
		bool _value;

	public:
		Boolean(bool v) {
			_type = VALUE_TYPE_BOOLEAN;
			_value = v;
		}

		virtual ~Boolean() {

		}

		bool asBool() {
			return _value;
		}
	};

	class Number : public Value {
	private:
		char _number[4];

	public:
		Number(int number) {
			_type = VALUE_TYPE_NUMBER;

			*((int *)_number) = number;
		}

		Number(float number) {
			_type = VALUE_TYPE_NUMBER;

			*((float *)_number) = number;
		}

		virtual ~Number() {

		}

		int asInt() const {
			return *((int *)_number);
		}

		float asFloat() const {
			return *((float *)_number);
		}
	};

}

#endif