
// Header
#include "Parser.h"

// Library includes
#include <assert.h>

// Project includes
#include <Parsers/Tokenizer.h>
#include <Tools/Strings.h>

// Namespace declartations


namespace JSON {


JsonPhrase Parser::getString(Tokenizer *t)
{
	JsonPhrase result;

	if ( !t ) {
		return result;
	}

	std::string name;
	std::string type = "unknown";
	std::string value;

	while ( t->hasNext() && (t->getToken() != "}" /*&& t->getToken() != "]"*/) ) {
		name = t->getToken();
		if ( name[0] == '"' ) {
			name = Tools::extract(name);
		//	type = "string";
		}

		result.add(JsonString(name, type, ""));

		if ( t->hasNext() ) {
			if ( t->getToken() == "{" ) {
				result[name].setType("object");
				result[name].setValue(getString(t).getValue());
			}
			//else if ( t->getToken() == "[" ) {
			//	result[name].setType("array");
			//	result[name].setValue(getString(t).getValue());
			//}
			else {
				if ( t->getToken() == "\"" ) {
					// skip '"'
					t->hasNext();
				}

				result[name].setValue(t->getToken());

				if ( t->getToken() == "\"" ) {
					// skip '"'
					t->hasNext();
				}
			}
		}
	}

	return result;
}

JsonPhrase Parser::parse(const std::string& msg)
{
	JsonPhrase phrase;
	Tokenizer token(msg, " :,\t\n\r");

	// start it
	if ( token.hasNext() ) {
		// has to be a '{'
		if ( token.getToken() == "{" ) {
			// start the real parsing
			phrase = getString(&token);

			assert(token.getToken() == "}");
		}
	}

	return phrase;
}


}
