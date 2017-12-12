
#ifndef _JSON_Parser_h_
#define _JSON_Parser_h_


// Library includes

// Project includes
#include "JsonPhrase.h"

// Forward declarations
class Tokenizer;

// Namespace declarations


namespace JSON {


class Parser
{
public:
	static JsonPhrase parse(const std::string& msg);

protected:

private:
	static JsonPhrase getString(Tokenizer *t);
};


}


#endif
