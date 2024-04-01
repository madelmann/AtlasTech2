
#ifndef _Font_Font_h_
#define _Font_Font_h_


// Library includes
#include <string>
// #include <windows.h>				// The windows header file.

// Project includes
#include "Settings.h"
#include <Driver/GLWrapper.h>
#include <Interfaces/Common/AComponent.h>

// Forward declarations
namespace TextureSpace {
	class IManager;
	class Texture;
}

// Namespace declarations
using TextureSpace::Texture;


namespace FontSpace {

typedef struct Font_s
{
	float		mCharHeight;
	float		mCharKerning;
	float		mCharWidth;
	char*		mName;
	Texture*	mTexture;
} Font_t;


class Font : public Common::AComponent
{
public:
	Font();
	~Font();

	// Common::AComponent implementation
	//{
	void configurationComplete();
	void shutdown();
	//}

	// Connectors
	//{
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	//{
	Settings& provideSettings();
	//}

	void clear();
	float getCharHeight() const;
	float getCharWidth() const;

	int getFontSize();															// Returns font's size
	bool Initialize(HDC hDC, int fontSize, const std::string& fontName);		// Initialize font.
	bool PrintText_cstr(const char *text, float x, float y);
	bool PrintText(const std::string& text, float x, float y);					// Print the text.
	void setWidthHeight(int w, int h);											// Set the window size.

	void drawString(const std::string& str, float x, float y);
	void selectFont(char* font, float size = 1.f);
	void selectFont(Texture* texture, float size = 1.f);

protected:
	unsigned int InitFont(const std::string& fontName, int fontSize, HDC hDC);	// Initialize list of text.

	unsigned int listBase;														// Store font.
	int Width;
	int Size;
	int Height;																	// Window width and height.

private:
	static void drawChar(char ch, float x, float y);

	Settings				mSettings;
	TextureSpace::IManager	*mTextureManager;

	float				mCharHeight_;
	float				mCharWidth_;
	HFONT				hFont;
	Font_t				mFont;
};

}


#endif
