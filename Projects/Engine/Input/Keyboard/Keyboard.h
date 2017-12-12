
#ifndef Input_Keyboard_Keyboard_h
#define Input_Keyboard_Keyboard_h


// Library includes
#include <map>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Input/IKeyboard.h>

// Forward declarations

// Namespace declarations


namespace Input {


class Keyboard : public IKeyboard,
				 private Common::Logger
{
public:
	Keyboard(const Common::Logger *p);

public:
	void addKey(const Key& key);
	void addKey(size_t keyCode, const std::string& function);

	void bind(const std::string& function, size_t keycode);

	Key* getKey(size_t keycode);
	Key* getKey(const std::string& function);

	Key::State::E getKeyState(size_t keyCode) const;
	void setKeyState(size_t keyCode, Key::State::E state);

protected:

private:
	typedef std::map<size_t, Key> KeyMap;

private:
	KeyMap	mKeys;
};


}


#endif
