
#ifndef Json_Value_h
#define Json_Value_h


// Library includes
#include <map>
#include <string>
#include <vector>

// Project includes
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace Json {


class Value
{
public:
	typedef std::map<std::string, Value> Members;

	class Type
	{
	public:
		enum E {
			NIL,
			ARRAY,
			BOOL,
			DOUBLE,
			FLOAT,
			INT,
			OBJECT,
			STRING,
			UINT,
			UNDEFINED
		};
	};

public:
	Value();
	Value(bool value);
	Value(double value);
	Value(float value);
	Value(int value);
	Value(const char* value);
	Value(const std::string& value);
	Value(unsigned int value);

public:
	bool asBool() const;
	double asDouble() const;
	float asFloat() const;
	int asInt() const;
	const std::string& asString() const;
	unsigned int asUInt() const;

	bool isArray() const;
	bool isBool() const;
	bool isDouble() const;
	bool isFloat() const;
	bool isInt() const;
	bool isNull() const;
	bool isObject() const;
	bool isString() const;
	bool isUInt() const;

	void addMember(const std::string& key, const Value& member);

	bool isArrayElement() const;
	void isArrayElement(bool value);

	bool isMember(const std::string& member) const;

	const Members& members() const;

	size_t size() const;

	const std::string& name() const;

	Type::E type() const;
	void type(Type::E type);

	std::string value() const;
	void value(const std::string& value);

public:
	std::string toString() const;

public:
	Value& operator[] (size_t idx);
	Value& operator[] (const std::string& key);

protected:
	void name(const std::string& value);

private:
	bool		mIsArrayElement;
	Members		mMembers;
	std::string	mName;
	Type::E		mType;
	std::string mValue;
};


}


#endif
