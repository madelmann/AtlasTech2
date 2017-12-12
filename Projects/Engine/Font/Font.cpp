
// Header
#include "Font.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Interfaces/Texture/IManager.h>
#include <Texture/Texture.h>

// Namespace declarations


namespace FontSpace {


Font::Font()
: hFont(0),
  mTextureManager(0)
{
	Height = 600;
	Width = 800;

	mFont.mCharHeight = 30.f;
	mFont.mCharKerning = 0.f;
	mFont.mCharWidth = 25.f;
	mFont.mName = "";
	mFont.mTexture = NULL;
}

Font::~Font()
{
	clear();
}

void Font::clear()
{
	mSettings.clear();

	DeleteObject(hFont);

	if ( listBase ) {
		aeDeleteLists(listBase, 255);
	}
}

void Font::configurationComplete()
{
	assert(!mConfigurationComplete);

	assert(mTextureManager);

	mConfigurationComplete = true;
}

void Font::connectTextureManager(TextureSpace::IManager *t)
{
	mTextureManager = t;
}

void Font::drawChar(char ch, float x, float y)
{
	float s = (ch % 16) * 25.f;//mFont.mCharWidth;
	float t = (ch / 16) * 30.f;//mFont.mCharHeight;

	aeTexCoord2f(s, t);
	aeVertex2f(x, y);

	aeTexCoord2f(s, t + 30.f);
	aeVertex2f(x, y + 30.f);

	aeTexCoord2f(s + 25.f, t + 30.f);
	aeVertex2f(x + 25.f, y + 30.f);

	aeTexCoord2f(s + 25.f, t);
	aeVertex2f(x + 25.f, y);
}

void Font::drawString(const std::string& str, float x, float y)
{
	aePushAttrib(GL_ALL_ATTRIB_BITS);

	glDepthFunc(GL_ALWAYS);
	aeDepthMask(FALSE);

	bool lighting = aeGetState(GL_LIGHTING);
	if ( lighting )
		aeDisable(GL_LIGHTING);

	bool texture = aeGetState(GL_TEXTURE_2D);
	if ( !texture )
		aeEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	aePushMatrix();
		aeLoadIdentity();

		glMatrixMode(GL_PROJECTION);
		aePushMatrix();
			aeLoadIdentity();

			// make (0, 0) the upper left corner and
			// (width, height) the lower right corner
			glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

			// set the texture matrix so that texture coordinates
			// s and t range from 0 to width/height
			glMatrixMode(GL_TEXTURE);
			aePushMatrix();
				float texMatrix[16];
				memset(texMatrix, 0, sizeof(texMatrix));
				texMatrix[0 + 0 * 4] = 1.0;// / tex->getTexelWidth();
				texMatrix[1 + 1 * 4] = 1.0;// / tex->getTexelHeight();
				texMatrix[2 + 2 * 4] = 1.0;
				texMatrix[3 + 3 * 4] = 1.0;
				glLoadMatrixf(texMatrix);

				// setup GL so that we can render transparent textures
				aeEnable(GL_BLEND);
				aeBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				mFont.mTexture->bind();

				// render letters as small 2D quads using the font texture
				// room for improvement: make one quad instead of many small
				aeBegin(GL_QUADS);
					for ( unsigned int i = 0; i < str.length(); i++ ) {
						drawChar(str[i], x + i * (mFont.mCharWidth + mFont.mCharKerning), y);
					}
				aeEnd();

			aePopMatrix();
		glMatrixMode(GL_PROJECTION);
		aePopMatrix();
	glMatrixMode(GL_MODELVIEW);
	aePopMatrix();

	aePopAttrib();
}

float Font::getCharHeight() const
{
	return mCharHeight_;
}

float Font::getCharWidth() const
{
	return mCharWidth_;
}

int Font::getFontSize()
{
	return Size;
}

// Initialize font.
bool Font::Initialize(HDC hDC, int fontSize, const std::string& fontName)
{
	// Create the font list.
	listBase = InitFont(fontName, fontSize, hDC);

	mCharHeight_ = Size / (float)Height;
	mCharWidth_  = Size / (float)Width;

	if ( listBase == 0 ) {
		return false;
	}

	return true;
}

unsigned int Font::InitFont(const std::string& fontName, int fontSize, HDC hDC)
{
	unsigned int base;

	// Create space for 255 characters.
	base = aeGenLists(255);

	Size = fontSize;

	hFont = CreateFont(fontSize, 0, 0, 0, FW_BLACK, FALSE, FALSE, FALSE,
					   DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
					   CLEARTYPE_QUALITY, FF_DONTCARE | DEFAULT_PITCH, fontName.c_str());
						//SYMBOL_CHARSET or ANSI_CHARSET or DEFAULT_CHARSET
						//ANTIALIASED_QUALITY or CLEARTYPE_QUALITY

	assert(hFont);
	// Error checking
	if ( hFont == NULL ) {
		return 0;
	}

	// Set up font.
	SelectObject(hDC, hFont);
	wglUseFontBitmaps(hDC, 0, 255, base);

	return base;
}

// Print the text at the desired position.
bool Font::PrintText_cstr(const char *text, float x, float y)
{
	// Error checking...
	if ( listBase == 0 || text == NULL ) {
		return false;
	}

	glMatrixMode(GL_PROJECTION);
	aePushMatrix();
		aeLoadIdentity();

		glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

		glMatrixMode(GL_MODELVIEW);
		aePushMatrix();
			aeLoadIdentity();

			// And finally in order to get this to work correctly we must disable depth testing.
			// Since this is not 3D we can't do any depth testing.
			aeDisable(GL_DEPTH_TEST);

			// Move to screen position sent in to this function.
			glRasterPos2f(x, y);

			// Print the string to the screen.
			aePushAttrib(GL_LIST_BIT);
				glListBase(listBase);
				glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
			aePopAttrib();

			// Set everything back to perpective below, don't forget to re-enable depth testing.
			aeEnable(GL_DEPTH_TEST);
		aePopMatrix();
		glMatrixMode(GL_PROJECTION);										
	aePopMatrix();

	glMatrixMode(GL_MODELVIEW);

	return true;
}

bool Font::PrintText(const std::string& text, float x, float y)
{
	return PrintText_cstr(text.c_str(), x, y);
}

Settings& Font::provideSettings()
{
	return mSettings;
}

void Font::setWidthHeight(int w, int h)
{
   Width = w;
   Height = h;
}

void Font::selectFont(char* font, float size)
{
	mFont.mTexture = mTextureManager->create(font);
	mFont.mCharHeight *= size;
	mFont.mCharWidth *= size;
}

void Font::selectFont(Texture* texture, float size)
{
	mFont.mTexture = texture;
	mFont.mCharHeight *= size;
	mFont.mCharWidth *= size;
}

void Font::shutdown()
{
}


}
