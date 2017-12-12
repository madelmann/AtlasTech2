
#ifndef Core_ConsolePlugin_h
#define Core_ConsolePlugin_h


// Library includes

// Project includes
#include "EngineSettings.h"
#include <Interfaces/Common/Version.h>
#include <Interfaces/Console/IConsolePlugin.h>
#include <Interfaces/Console/IConsolePrinter.h>
#include <Parsers/Evaluator.h>
#include <Parsers/Valueizer.h>
#include <Tools/Strings.h>

// Forward declarations

// Namespace declarations


namespace Core {


class ConsolePlugin : public Console::IConsolePlugin
{
public:
	ConsolePlugin()
	: Console::IConsolePlugin("core"),
	  mEngineSettings(0),
	  mVersionInformation(0)
	{ }

public:
	void connectEngine(EngineSettings *s) {
		mEngineSettings = s;
	}
	void connectVersionInformation(Common::Version *vi) {
		mVersionInformation = vi;
	}

	void execute(Valueizer *t) {
		assert(mEngineSettings);
		assert(mVersionInformation);
		assert(mPrinter);

		if ( t->hasNext() ) {
			Value value = t->getTokenAsValue();

			if ( t->hasNext() ) {
				if ( value.ofValue("twosidedlight") ) {
					value = Value(t->getToken());

					mEngineSettings->setLightModelTwoSided(value.toBool());

					mPrinter->print("Core: light-model-two-sided set to " + value.toString() + ".");
				}
				else if ( value.ofValue("viewdepth") ) {
					value = Value(t->getToken());

					mEngineSettings->setMaxViewDepth(value.toFloat());

					mPrinter->print("Core: view depth set to " + value.toString() + ".");
				}
			}
			else {
				if ( value.ofValue("") ) {
					//mPrinter->print();
				}
				else if ( value.ofValue("twosidedlight") ) {
					mPrinter->print("twosidedlight: " + Tools::toString(mEngineSettings->isLightModelTwoSided()));
				}
				if ( value.ofValue("version") ) {
					mPrinter->print("Major: " + Tools::toString(mVersionInformation->getMajorVersion()));
					mPrinter->print("Minor: " + Tools::toString(mVersionInformation->getMinorVersion()));
					mPrinter->print("Revision: " + Tools::toString(mVersionInformation->getRevision()));
				}
				else if ( value.ofValue("viewdepth") ) {
					mPrinter->print("viewdepth: " + Tools::toString(mEngineSettings->getMaxViewDepth()));
				}
			}

			return;
		}

		help();
	}

protected:

private:
	void help() {
		mPrinter->print("Usage: core <setting> <value>");
		mPrinter->print("");
		mPrinter->print("Available settings:");
		mPrinter->print("	twosidedlight <bool>");
		mPrinter->print("	viewdepth <float>");
		mPrinter->print("Available functions:");
		mPrinter->print("	version");
		mPrinter->print("");
	}

private:
	EngineSettings	*mEngineSettings;
	Common::Version	*mVersionInformation;
};


}


#endif
