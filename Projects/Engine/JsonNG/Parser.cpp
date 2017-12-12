
// Header
#include "Parser.h"

// Library includes
#include <assert.h>

// Project includes
#include <Parsers/Tokenizer.h>
#include <Tools/Strings.h>

// Namespace declartations


namespace Json {

bool _is_array_ = false;


Value::Type::E guessType(const std::string& value)
{
	if ( value.empty() )
		return Value::Type::NIL;
	if ( value == "true" || value == "false" )
		return Value::Type::BOOL;
	if ( value[0] == '\"' )
		return Value::Type::STRING;

	return Value::Type::UNDEFINED;
}


Value Parser::getString(Tokenizer *t)
{
	Value result;

	if ( !t ) {
		return result;
	}

	std::string name;
	std::string original;

	bool hasNext = t->hasNext();
	while ( hasNext && (t->getToken() != "}" && t->getToken() != "]") ) {
		name = t->getToken();
		original = name;
		if ( name[0] == '"' ) {
			name = Tools::extract(name);
		}

		result.addMember(name, Value());
		if ( _is_array_ ) {
			result.type(Value::Type::ARRAY);
		}

		if ( t->hasNext() ) {
			if ( t->getToken() == "{" ) {
				result[name].type(Value::Type::OBJECT);
				result[name] = getString(t);
			}
			else if ( t->getToken() == "[" ) {
				_is_array_ = true;
				result[name].type(Value::Type::ARRAY);
				result[name] = getString(t);
				_is_array_ = false;
			}
			else if ( !_is_array_ && t->getToken()[0] == '\"' ) {
				result[name].type(Value::Type::STRING);
				result[name].value(Tools::extract(t->getToken()));
			}
			else {
				result[name].isArrayElement(_is_array_);

				std::string token = t->getToken();
				if ( token == "null") {
					hasNext = t->hasNext();
					continue;
				}

				if ( _is_array_ ) {
					result[name].type(guessType(original));
					result[name].value(name);
					continue;
				}

				result[name].type(guessType(token));
				result[name].value(token);
			}
		}

		hasNext = t->hasNext();
	}

	return result;
}

Value Parser::parse(const std::string& msg)
{
	Value value;
	Tokenizer token(msg, " :,\t\n\r");

	// start it
	if ( token.hasNext() ) {
		// has to be a '{'
		if ( token.getToken() == "{" ) {
			// start the real parsing
			value = getString(&token);

			assert(token.getToken() == "}");
		}
	}

	return value;
}


}
