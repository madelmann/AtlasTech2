
// Header
#include "Logger.h"

// Library includes
#include <stdarg.h>
#include <time.h>

// Project include
#include <Tools/Strings.h>

// Namespace declarations
using Tools::toString;


#pragma warning(disable : 4996)


namespace Common {


void push_info_stack( const char * string )
{
    assert( g_infoStackDepth < MaxInfoDepth );
    strncpy( &g_infoStack[g_infoStackDepth][0], string, MaxInfoLength );
    g_infoStack[g_infoStackDepth][MaxInfoLength] = '\0';
    g_infoStackDepth++;
}

void pop_info_stack()
{
    assert( g_infoStackDepth > 0 );
    g_infoStack[g_infoStackDepth][0] = '\0';
    g_infoStackDepth--;
}



Logger::Logger(const Logger *parent)
: mContext(0),
  mHasParent(false),
  mLogFile("AtlasEngine.log"),
  mLoudness(Logger::LoudnessMethod)
{
	if ( parent ) {
		mContext = parent->getContext();
		if ( !parent->getClassName().empty() ) {
			mClassName = parent->getClassName() + "::";
		}
	}

	if ( !mContext ) {
		mContext = new Context(mLogFile);
	}
}

Logger::Logger(const Logger *parent, const std::string& className, const std::string& key)
: mContext(0),
  mHasParent(false),
  mKey(key),
  mLogFile("AtlasEngine.log"),
  mLoudness(Logger::LoudnessMethod)
{
	if ( parent ) {
		mContext = parent->getContext();
		mHasParent = true;

		if ( !parent->getClassName().empty() ) {
			mClassName = parent->getClassName() + "::";
		}
	}

	mClassName += className;

	if ( !mContext ) {
		mContext = new Context(mLogFile);
	}
}

Logger::~Logger()
{
	if ( !mHasParent ) {
		delete mContext;
	}
}

const std::string& Logger::getClassName() const
{
	return mClassName;
}

IContext* Logger::getContext() const
{
	return mContext;
}

std::string Logger::getDateTime()
{
	time_t t = time(0);
	tm *lt = localtime(&t);

	char dateStr[11];
	sprintf(dateStr, "%04d/%02d/%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday);

	char timeStr[9];
	_strtime( timeStr );

	return std::string(dateStr) + " " + std::string(timeStr);
}

Logger* Logger::getLogger()
{
	return this;
}

void Logger::Log(const std::string& loglevel, const std::string& message, char *file, unsigned int line)
{
	std::string msg; //= getDateTime()
	msg += "[" + loglevel + "] ";
	if ( !mClassName.empty() ) {
	msg += mClassName;
	if ( !mKey.empty() ) {
	msg += "[" + mKey + "]";
	}
	msg += ": ";
	}
	msg += message;
	msg += "   [" + toString(file) + ":" + toString(line) + "]";

	mContext->write(msg.c_str());
}

void Logger::LogDebug(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessDebug ) {
		return;
	}

	Log("DEBUG", message, file, line);
}

void Logger::LogDeprecate(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessDeprecated ) {
		return;
	}

	Log("DEPRECATED", message, file, line);
}

void Logger::LogError(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessError ) {
		return;
	}

	Log("ERROR", message, file, line);
}

void Logger::LogFatal(const std::string& message, char *file, unsigned int line)
{
	Log("FATAL", message, file, line);

	assert(!"Fatal error occured: ");
	std::exit(1);
}

void Logger::LogInfo(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessMethod ) {
		return;
	}

	Log("INFO ", message, file, line);
}

void Logger::LogMethod(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessMethod ) {
		return;
	}

	Log("METHOD", message, file, line);
}

void Logger::LogWarn(const std::string& message, char *file, unsigned int line)
{
	if ( mLoudness < LoudnessWarning ) {
		return;
	}

	Log("WARN ", message, file, line);
}

void Logger::setKey(const std::string& key)
{
	mKey = key;
}

void Logger::setLoudness(int loudness)
{
	mLoudness = loudness;
}


}
