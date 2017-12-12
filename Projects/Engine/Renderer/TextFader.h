
#ifndef Renderer_TextFader_h
#define Renderer_TextFader_h


// Library includes
#include <list>
#include <string>

// Project includes
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


namespace Core {

// Forward declarations
class IScreenWriter;

class TextFader
{
public:
	class Settings
	{
	public:
		Settings()
			: mDuration(0.f),
			  mFadeInAfter(0.f),
			  mFadeOutAfter(0.f),
			  mLineWidth(-1)
		{ }

	public:
		vector4f	mColor;
		float		mDuration;
		float		mFadeInAfter;
		float		mFadeOutAfter;
		int			mLineWidth;
	};

public:
	TextFader();

public:
	// Initialisation
	// {
	void connectScreenWriter(IScreenWriter *w);
	// }

	void addLine(const std::string& line);
	void clear();

	const Settings& settings() const;
	void settings(const Settings& s);

	void pause();
	void start();
	void stop();

	void render();

protected:

private:
	typedef std::list<std::string> StringList;

private:
	IScreenWriter	*mScreenWriter;

	StringList	mLines;
	bool		mPaused;
	float		mProgress;
	Settings	mSettings;
};


}


#endif
