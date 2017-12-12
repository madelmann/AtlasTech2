
#ifndef _JSON_JsonValue_h_
#define _JSON_JsonValue_h_


// Library includes

// Project includes
#include <Parsers/Value.h>

// Forward declarations

// Namespace declarations


namespace JSON {


class JsonValue : public Value
{
public:
	JsonValue(const std::string& name, const std::string& type, const std::string& value);
	virtual ~JsonValue() { }

public:
	virtual std::string toString() const;
	virtual std::string toStyledString();

protected:
	bool	mStyledString;

private:

};


}


#endif
