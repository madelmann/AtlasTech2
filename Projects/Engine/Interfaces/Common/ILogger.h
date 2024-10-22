
#ifndef _Interfaces_Common_ILogger_h_
#define _Interfaces_Common_ILogger_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class ILogger
{
public:
	virtual ~ILogger() { }

public:
	static const int LoudnessError = 0;
	static const int LoudnessWarning = 1;
	static const int LoudnessDebug = 2;
	static const int LoudnessInfo = 3;
	static const int LoudnessDeprecated = 4;
	static const int LoudnessMethod = 5;

public:
	virtual const std::string& getClassName() const = 0;
	virtual ILogger* getLogger() = 0;

	virtual void LogDebug(const std::string& message, char* file, unsigned int line) = 0;
	virtual void LogDeprecate(const std::string& message, char* file, unsigned int line) = 0;
	virtual void LogError(const std::string& message, char* file, unsigned int line) = 0;
	virtual void LogFatal(const std::string& message, char* file, unsigned int line) = 0;
	virtual void LogInfo(const std::string& message, char* file, unsigned int line) = 0;
	virtual void LogWarn(const std::string& message, char* file, unsigned int line) = 0;

	virtual void setKey(const std::string& key) = 0;
	virtual void setLoudness(int loudness) = 0;
};


}


#endif
