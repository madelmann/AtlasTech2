
#ifndef _Parsers_Tokenizer_h_
#define _Parsers_Tokenizer_h_


// Library includes
#include <string>

// Project includes
#include <Parsers/Value.h>

// Forward declarations

// Namespace declarations


class Tokenizer 
{
public:
    static const std::string DELIMITERS;

public:
    Tokenizer(const std::string& str, const std::string& delimiters = "");

public:
    const std::string getToken() const;
    bool hasNext();
    bool nextToken(const std::string& delimiters);
    void reset();

protected:
    std::string	mDelimiters;
    size_t		mOffset;
    std::string	mString;
    std::string	mToken;

private:

};


#endif
