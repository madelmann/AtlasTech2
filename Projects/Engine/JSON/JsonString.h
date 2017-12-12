
#ifndef _JSON_JsonString_h_
#define _JSON_JsonString_h_


// Library includes
#include <list>
#include <map>

// Project includes
#include "JsonValue.h"

// Forward declarations

// Namespace declarations


namespace JSON {


class JsonString : public JsonValue
{
public:
	typedef std::list<JsonString> JsonStrings;
	typedef std::map<std::string, JsonString> JsonMap;

public:
	JsonString();
	JsonString(const std::string& name, const std::string& type, const std::string& value);
	virtual ~JsonString() { }

public:
	void add(JsonString jstring);
	void clear();

	JsonStrings getValue() const;

	bool isValid() const;

	void setType(const std::string& type);
	void setValue(const std::string& value);
	void setValue(const JsonStrings &value);

	virtual std::string toString() const;
	virtual std::string toStyledString();

	JsonString& operator[] (const std::string& value);

protected:
	JsonStrings	mStrings;

private:

};


}


#endif
