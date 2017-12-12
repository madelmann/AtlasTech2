
#ifndef JsonNG_Parser_h
#define JsonNG_Parser_h


// Library includes

// Project includes
#include "Value.h"

// Forward declarations
class Tokenizer;

// Namespace declarations


namespace Json {


class Parser
{
public:
	static Value parse(const std::string& msg);

protected:

private:
	static Value getString(Tokenizer *t);
};


}


#endif
