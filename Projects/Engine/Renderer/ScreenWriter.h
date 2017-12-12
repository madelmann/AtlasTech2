
#ifndef _Renderer_ScreenWriter_h_
#define _Renderer_ScreenWriter_h_


// Library includes
#include <list>

// Project includes
#include <Interfaces/Common/AComponent.h>
#include <Interfaces/Renderer/IScreenWriter.h>

// Forward declarations
namespace FontSpace {
	class Font;
}

// Namespace declarations


namespace Core {


class ScreenWriter : public Common::AComponent,
					 public IScreenWriter
{
public:
	class ScreenEntry
	{
	public:
		ScreenEntry(float x, float y, const std::string& text, aeint time, const vector3f& color)
		: mColor(color),
		  mText(text),
		  mTime(time),
		  mX(x),
		  mY(y)
		{ }

		vector3f	mColor;
		std::string	mText;
		aeint		mTime;
		float		mX;
		float		mY;
	};

	ScreenWriter();
	~ScreenWriter();

	// Common::AComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// IScreenWriter implementations
	//{
	void write(float x, float y, const std::string& text, aeint time, const vector3f& color = vector3f(1.f, 1.f, 1.f));
	//}

	// Connectors
	//{
	void connectFont(FontSpace::Font *f);
	//}

	void render();
	void update(float elapsedTime);

protected:

private:
	FontSpace::Font	*mFont;

	std::list<ScreenEntry*>	mEntries;
};


}


#endif
