
#ifndef _Common_Logger_h_
#define _Common_Logger_h_


#pragma warning(disable : 4996)


#define debug(msg) LogDebug(msg, __FILE__, __LINE__);
#define deprecate(msg) LogDeprecate(msg, __FILE__, __LINE__);
#define error(msg) LogError(msg, __FILE__, __LINE__);
#define fatal(msg) LogFatal(msg, __FILE__, __LINE__);
#define info(msg) LogInfo(msg, __FILE__, __LINE__);
#define method(msg) LogMethod(__FUNCTION__ msg, __FILE__, __LINE__);
#define warn(msg) LogWarn(msg, __FILE__, __LINE__);


// Library includes
#include <assert.h>
#include <iostream>
#include <fstream>

// Project includes
#include "Context.h"
#include <Interfaces/Common/ILogger.h>

// Forward declarations

// Namespace declarations


static const char* MessageDebug     = "[DBG ]";
static const char* MessageDeprecate = "[DEPR]";
static const char* MessageError     = "[ERR ]";
static const char* MessageFatal	    = "[FATE]";
static const char* MessageWarning   = "[WARN]";


namespace Common {


class Logger : public ILogger
{
public:
	Logger(const Logger *parent = 0);
	Logger(const Logger *parent, const std::string& className, const std::string& key = "");
	virtual ~Logger();

public:
	const std::string& getClassName() const;
	IContext* getContext() const;
	Logger* getLogger();

	void LogDebug(const std::string& message, char *file, unsigned int line);
	void LogDeprecate(const std::string& message, char *file, unsigned int line);
	void LogError(const std::string& message, char *file, unsigned int line);
	void LogFatal(const std::string& message, char *file, unsigned int line);
	void LogInfo(const std::string& message, char *file, unsigned int line);
	void LogMethod(const std::string& message, char *file, unsigned int line);
	void LogWarn(const std::string& message, char *file, unsigned int line);

	void setKey(const std::string& key);
	void setLoudness(int loudness);

protected:
	IContext	*mContext;

private:
	std::string getDateTime();
	void Log(const std::string& loglevel, const std::string& message, char *file, unsigned int line);

private:
	std::string	mClassName;
	bool		mHasParent;
	std::string	mKey;
	char		*mLogFile;
	int			mLoudness;
};


const int MaxInfoLength = 256;
const int MaxInfoDepth = 64;

static int g_infoStackDepth = 0;
static char g_infoStack[MaxInfoDepth][MaxInfoLength + 1];


void push_info_stack(const char * string);
void pop_info_stack();


class InfoPushPopHelper
{
    InfoPushPopHelper(const char * string) {
        push_info_stack( string );
    }

    ~InfoPushPopHelper() {
        pop_info_stack();
    }
};


#define INFO( format, ... )                                       \
    char buffer[MaxInfoLength + 1];                               \
    snprintf( buffer, MaxInfoLength + 1, format, __VA_ARGS__ );   \
    InfoPushPopHelper infoPushPop( buffer );

}


#endif
