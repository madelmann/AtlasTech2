
#ifndef _JSON_JsonPhrase_h_
#define _JSON_JsonPhrase_h_


// Library includes

// Project includes
#include "JsonString.h"

// Forward declarations

// Namespace declarations


namespace JSON {


class JsonPhrase : public JsonString
{
public:
	std::string toString();
	std::string toStyledString();

protected:

private:

};


}


#endif
