
// Header
#include "Value.h"

// Library includes

// Project includes
#include "Exceptions.h"

// Namespace declarations


namespace Json {


Value::Value()
: mIsArrayElement(false),
  mType(Type::NIL)
{
}

Value::Value(bool value)
: mIsArrayElement(false),
  mType(Type::BOOL)
{
	if ( value ) {
		mValue = "true";
	}
	else {
		mValue = "false";
	}
}

Value::Value(double value)
: mIsArrayElement(false),
  mType(Type::DOUBLE)
{
	mValue = Tools::toString(value);
}

Value::Value(float value)
: mIsArrayElement(false),
  mType(Type::FLOAT)
{
	mValue = Tools::toString(value);
}

Value::Value(int value)
: mIsArrayElement(false),
  mType(Type::INT)
{
	mValue = Tools::toString(value);
}

Value::Value(const char* value)
: mIsArrayElement(false),
  mType(Type::STRING)
{
	mValue = value;
}

Value::Value(const std::string& value)
: mIsArrayElement(false),
  mType(Type::STRING)
{
	mValue = value;
}

Value::Value(unsigned int value)
: mIsArrayElement(false),
  mType(Type::UINT)
{
	mValue = Tools::toString(value);
}

void Value::addMember(const std::string& key, const Value& member)
{
	if ( mMembers.find(key) != mMembers.end() ) {
		throw Exceptions::DuplicateKey(key);
	}

	if ( mType == Type::NIL ) {
		mType = Type::OBJECT;
	}

	mMembers[key] = member;
	mMembers[key].name(key);
}

bool Value::asBool() const
{
	if ( Tools::StringCompareI(mValue, "true") ) {
		return true;
	}

	return false;
}

double Value::asDouble() const
{
	std::stringstream ss(mValue);
	double v;
	ss >> v;

	return v;
}

float Value::asFloat() const
{
	std::stringstream ss(mValue);
	float v;
	ss >> v;

	return v;
}

int Value::asInt() const
{
	std::stringstream ss(mValue);
	int v;
	ss >> v;

	return v;
}

const std::string& Value::asString() const
{
	return mValue;
}

unsigned int Value::asUInt() const
{
	std::stringstream ss(mValue);
	unsigned int v;
	ss >> v;

	return v;
}

bool Value::isArray() const
{
	return (mType == Type::ARRAY);
}

bool Value::isArrayElement() const
{
	return mIsArrayElement;
}

void Value::isArrayElement(bool value)
{
	mIsArrayElement = value;
}

bool Value::isBool() const
{
	return (mType == Type::BOOL);
}

bool Value::isDouble() const
{
	return (mType == Type::DOUBLE);
}

bool Value::isFloat() const
{
	return (mType == Type::FLOAT);
}

bool Value::isInt() const
{
	return (mType == Type::INT);
}

bool Value::isMember(const std::string& member) const
{
	for ( Members::const_iterator it = mMembers.begin(); it != mMembers.end(); ++it ) {
		if ( (*it).first == member ) {
			return true;
		}
	}

	return false;
}

bool Value::isObject() const
{
	return (mType == Type::OBJECT);
}

bool Value::isString() const
{
	return (mType == Type::STRING);
}

bool Value::isUInt() const
{
	return (mType == Type::UINT);
}

const Value::Members& Value::members() const
{
	return mMembers;
}

const std::string& Value::name() const
{
	return mName;
}

void Value::name(const std::string& value)
{
	mName = value;
}

size_t Value::size() const
{
	switch ( mType ) {
		case Type::ARRAY:
		case Type::OBJECT: return mMembers.size();
		default:
			throw Exceptions::JSONException("Json::Value is not an array nor an object!");
	}
}

std::string Value::toString() const
{
	std::string result;
	if ( isObject() ) {
		result += "{ ";
	}

	Value::Members::const_iterator it = mMembers.begin();
	while ( it != mMembers.end() ) {
		if ( !(*it).second.isArrayElement() ) {
			result += "\"";
			result += (*it).first;
			result += "\": ";
		}

		Value v = (*it).second;
		switch ( v.type() ) {
			case Value::Type::NIL:
				result += "null";
				break;
			case Value::Type::BOOL:
			case Value::Type::DOUBLE:
			case Value::Type::FLOAT:
			case Value::Type::INT:
			case Value::Type::STRING:
			case Value::Type::UINT:
			case Value::Type::UNDEFINED:
				result += v.value();
				break;
			case Value::Type::ARRAY:
				if ( !v.size() ) {
					result += "[ ]";
				}
				else {
					result += "[ ";

					Value::Members members = v.members();
					Value::Members::const_iterator it = members.begin();

					while ( it != members.end() ) {
						result += (*it).second.value();
						it++;

						if ( it != members.end() ) {
							result += ", ";
						}
					}

					result += " ]";
				}
				break;
			case Value::Type::OBJECT:
				if ( !v.size() )
					result += "{ }";
				else
					result += v.toString();
				break;
		}

		it++;

		if ( it != mMembers.end() ) {
			result += ", ";
		}
	}

	if ( isObject() ) {
		result += " }";
	}
	return result;
}

Value::Type::E Value::type() const
{
	return mType;
}

void Value::type(Value::Type::E type)
{
	mType = type;
}

std::string Value::value() const
{
	std::string result;
	if ( mType == Type::STRING ) {
		result += "\"";
	}
	result += mValue;
	if ( mType == Type::STRING ) {
		result += "\"";
	}

	return result;
}

void Value::value(const std::string& value)
{
	mValue = value;
}

Value& Value::operator[] (size_t idx)
{
	// check if we have already a type set
	if ( mType != Type::NIL && mType != Type::ARRAY ) {
		// we can not redeclare a value's type => throw an exception
		throw Exceptions::JSONException("Json::Value is not an array!");
	}

	if ( mType == Type::NIL ) {
		mType = Type::ARRAY;
	}

	if ( idx >= size() ) {
		for ( size_t i = size(); i <= idx; i++ ) {
			mMembers[Tools::toString(idx)];
		}
	}

	return mMembers[Tools::toString(idx)];
}

Value& Value::operator[] (const std::string& key)
{
	for ( Members::iterator it = mMembers.begin(); it != mMembers.end(); ++it ) {
		if ( (*it).first == key ) {
			return (*it).second;
		}
	}

	if ( mType == Type::NIL ) {
		mType = Type::OBJECT;
	}

	mMembers[key] = Value();
	return mMembers[key];
}


}
