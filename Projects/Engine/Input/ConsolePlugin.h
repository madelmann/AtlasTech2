
#ifndef _Input_ConsolePlugin_h_
#define _Input_ConsolePlugin_h_


// Library includes

// Project includes
#include "KeyGrabber.h"
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Interfaces/Input/IKeyboard.h>
#include <Interfaces/Input/IMouse.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations
using Tools::toString;


namespace Input {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("input"),
	  mKeyboard(0),
	  mKeyGrabber(0),
	  mMouse(0)
	{ }

	void connectKeyboard(IKeyboard *k) {
		mKeyboard = k;
	}
	void connectKeyGrabber(KeyGrabber *g) {
		mKeyGrabber = g;
	}
	void connectMouse(IMouse *m) {
		mMouse = m;
	}

	void execute(Valueizer *t) {
		assert(mKeyboard);
		assert(mKeyGrabber);
		assert(mMouse);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("bind") ) {
					value = t->getTokenAsValue();

					std::string function = value.getValue();
					Key *key = mKeyboard->getKey(function);

					mKeyGrabber->bind(key);
					mKeyGrabber->restart();
				}
				else if ( value.ofValue("bindkey") ) {
					value = t->getTokenAsValue();

					int code = value.toInt();
					if ( code > 0 && t->hasNext() ) {
						value = t->getTokenAsValue();
						std::string function = value.getValue();

						if ( function.length() ) {
							mKeyboard->bind(function, code);
						}
					}
				}
			}
			else {
				//if ( value.ofValue("bind") ) {
					//mPrinter->print("bind: " + Tools::toString(mEngine->isLightModelTwoSided()));
				//}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: input <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("   bind <function> <key press>");
		mPrinter->print("   bindkey <key> <function>");
	}

private:
	IKeyboard	*mKeyboard;
	KeyGrabber	*mKeyGrabber;
	IMouse		*mMouse;
};


}


#endif
