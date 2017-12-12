
#ifndef _Parsers_Valueizer_h_
#define _Parsers_Valueizer_h_


// Library includes

// Project includes
#include "Tokenizer.h"

// Forward declarations

// Namespace declarations


class Valueizer : public Tokenizer 
{
public:
    Valueizer(const std::string& str);
    Valueizer(const std::string& str, const std::string& delimiters);

public:
	const Value getTokenAsValue() const;

protected:

private:

};


#endif
