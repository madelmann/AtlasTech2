
#ifndef _Driver_GLWrapper_h_
#define _Driver_GLWrapper_h_


#ifdef _WIN32
#pragma warning(disable : 4068)
#endif


// Library includes
#include <cassert>
#include <stdlib.h>
#include <string>

// Project includes
#include <Driver/OpenGL/Context.h>
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


#define BUFFER_OFFSET(i) ((char*)NULL + (i))



namespace GL {


class Hint
{
public:
	enum E {
		gl_dont_care = 0x1100,
		gl_fastest = 0x1101,
		gl_nicest = 0x1102
	};
};


class CGLDriverStates
{
public:
	typedef enum {
		CCW = 0,
		CW
	} CullMode_t;

	void		setCullMode(CullMode_t cullMode);
	CullMode_t	getCullMode() const;

protected:

private:
	CullMode_t	mCullMode;
};


}


// Type definitions
typedef GLfloat	aefloat;
typedef GLint	aeint;
typedef GLuint	aeuint;


// Forward declarations
class vector2f;
class vector3f;
class vector4f;


// AtlasEngine driver wrappers
void aeActiveTexture (unsigned int target);
void aeAlphaFunc (unsigned int func, GLclampf ref);
void aeBegin (unsigned int mode);
void aeBeginQuery (unsigned int id);
void aeBlendFunc (unsigned int sfactor, unsigned int dfactor);
void aeCallList (unsigned int list);
void aeColor (const vector3f& color);
void aeColor (const vector4f& color);
void aeColor3f (float c1, float c2, float c3);
void aeColor4f (float c1, float c2, float c3, float c4);
void aeCreateBuffer (unsigned int &bufferId);
void aeCullFace (unsigned int mode);
void aeDeleteBuffer (unsigned int &bufferId);
void aeDeleteLists (unsigned int list, AEsizei range);
void aeDepthFunc (unsigned int func);
void aeDepthMask (unsigned char flag, bool force = false);
void aeDisable (unsigned int cap, bool force = false);
void aeEnable (unsigned int cap, bool force = false);
void aeEnd ();
void aeEndList ();
void aeEndQuery ();
void aeForceDisable ( unsigned int cap );
void aeForceEnable ( unsigned int cap );
unsigned int aeGetAttributesLeft ();
unsigned int aeGetImmediateModeLeft ();
unsigned int aeGetMatricesLeft ();
unsigned int aeGenLists (int range);
bool aeGetState (unsigned int cap);
void aeHint (unsigned int target, unsigned int mode);
unsigned char aeIsEnabled (unsigned int cap);
unsigned char aeIsList (unsigned int list);
void aeLoadIdentity ();
void aeMaterialf (unsigned int face, unsigned int pname, float param);
void aeMaterialfv (unsigned int face, unsigned int pname, float *params);
void aeMaterialfv (unsigned int face, unsigned int pname, vector4f& params);
void aeMaterialfv (unsigned int face, unsigned int pname, vector4f *params);
void aeNewList (unsigned int list, unsigned int mode);
void aeNormal3f (float nx, float ny, float nz);
void aePopAttrib ();
void aePopMatrix ();
void aePushAttrib (GLbitfield mask);
void aePushMatrix ();
void aeRotate (const vector3f& v);
void aeRotatef (float angle, float x, float y, float z);
void aeRotatev3 (float angle, const vector3f& v);
void aeSetState (unsigned int cap, bool state, bool force = false);
void aeTexCoord2f (float s, float t);
void aeTranslate (const vector3f& v);
void aeTranslatef (float x, float y, float z);
void aeVertex (const vector2f& v);
void aeVertex (const vector3f& v);
void aeVertex2f (float x, float y);
void aeVertex3f (float x, float y, float z);
void aeVertex3v (const vector3f& v);
void aeViewport (int x, int y, AEsizei width, AEsizei height);
void aeViewport (float x, float y, float width, float height);

// AtlasEngine driver info provider
std::string aeStackInfo();


#endif
