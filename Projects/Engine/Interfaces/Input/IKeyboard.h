
#ifndef Interfaces_Input_IKeyboard_h
#define Interfaces_Input_IKeyboard_h


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarationd


namespace Input {


class Key
{
public:
	class State
	{
	public:
		enum E {
			None,
			Down,
			Up
		};
	};

public:
	Key() {}

	Key(size_t keyCode, const std::string& function)
	: mFunction(function),
	  mKeyCode(keyCode),
	  mState(State::None)
	{ }

public:
	bool isFunction(const std::string& function) {
		return (mFunction == function);
	}

	const std::string& getFunction() const {
		return mFunction;
	}

	size_t getKeyCode() const {
		return mKeyCode;
	}
	void setKeyCode(size_t keyCode) {
		mKeyCode = keyCode;
	}

	State::E getState() const {
		return mState;
	}
	void setState(State::E state) {
		mState = state;
	}

protected:

private:
	std::string	mFunction;
	size_t		mKeyCode;
	State::E	mState;
};


class IKeyboard
{
public:
	virtual ~IKeyboard() { }

public:
	virtual void addKey(const Key& key) = 0;
	virtual void addKey(size_t keyCode, const std::string& function) = 0;

	virtual void bind(const std::string& function, size_t keycode) = 0;

	virtual Key* getKey(size_t keycode) = 0;
	virtual Key* getKey(const std::string& function) = 0;

	virtual Key::State::E getKeyState(size_t keyCode) const = 0;
	virtual void setKeyState(size_t keyCode, Key::State::E state) = 0;
};


}


#endif
