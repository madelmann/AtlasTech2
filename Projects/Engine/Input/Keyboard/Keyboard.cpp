
// Header
#include "Keyboard.h"

// Library includes

// Project includes
#include <Tools/Strings.h>

// Namespace declarations


namespace Input {


Keyboard::Keyboard(const Common::Logger *p)
: Common::Logger(p, "Keyboard")
{
}

void Keyboard::addKey(const Key& key)
{
	mKeys.insert(std::pair<size_t, Key>(key.getKeyCode(), key));
}

void Keyboard::addKey(size_t keyCode, const std::string& function)
{
	mKeys.insert(std::pair<size_t, Key>(keyCode, Key(keyCode, function)));
}

void Keyboard::bind(const std::string& function, size_t keycode)
{
	KeyMap::iterator it;
	for ( it = mKeys.begin(); it != mKeys.end(); ++it ) {
		if ( (*it).second.getFunction() == function ) {
			(*it).second = Key(keycode, function);
			return;
		}
	}

	addKey(keycode, function);
}

Key* Keyboard::getKey(size_t keycode)
{
	KeyMap::iterator it;
	for ( it = mKeys.begin(); it != mKeys.end(); ++it ) {
		if ( (*it).second.getKeyCode() == keycode ) {
			return &(*it).second;
		}
	}

	return 0;
}

Key* Keyboard::getKey(const std::string& function)
{
	KeyMap::iterator it;
	for ( it = mKeys.begin(); it != mKeys.end(); ++it ) {
		if ( (*it).second.getFunction() == function ) {
			return &(*it).second;
		}
	}

	return 0;
}

Key::State::E Keyboard::getKeyState(size_t keyCode) const
{
	if ( mKeys.find(keyCode) == mKeys.begin() ) {
		//warn("Key[" + Tools::toString(keyCode) + "] not found!");
		return Key::State::None;
	}

	return (*mKeys.find(keyCode)).second.getState();
}

void Keyboard::setKeyState(size_t keyCode, Key::State::E state)
{
	if ( mKeys.find(keyCode) == mKeys.begin() ) {
		//warn("Key[" + Tools::toString(keyCode) + "] not found!");
		return;
	}

	mKeys[keyCode].setState(state);
}


}
