
#ifndef _Driver_GLWrapper_h_
#define _Driver_GLWrapper_h_


#ifdef _WIN32
#pragma warning(disable : 4068)
#endif


// Library includes
#include <cassert>
#include <stdlib.h>
#include <string>

#ifdef _WIN32
#include <windows.h>

#include <OpenGL/include/gl.h>										// Header File For The OpenGL32 Library
#include <OpenGL/include/glext.h>
#include <OpenGL/include/glu.h>										// Header File For The GLu32 Library
//#include <OpenGL/include/glut.h>									// Header file for the GL utility library
#include <OpenGL/include/glaux.h>									// Header File For The GLaux Library
#endif

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


/*
// These are for our multi-texture defines
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB                     0x84C2
#define GL_TEXTURE3_ARB                     0x84C3
#define GL_TEXTURE4_ARB                     0x84C4

// These are used to increase the detail texture for our terrain
#define GL_COMBINE_ARB						0x8570
#define GL_RGB_SCALE_ARB					0x8573

// We need to define this for glTexParameteri()
#define GL_CLAMP_TO_EDGE					0x812F								// This is for our skybox textures

// This is a define that we use for our function pointers
#define APIENTRYP APIENTRY *

// Here we include the vertex and fragment shader defines
#define GL_VERTEX_SHADER_ARB              0x8B31
#define GL_FRAGMENT_SHADER_ARB            0x8B30

#define GL_VERTEX_PROGRAM_ARB             0x8620
#define GL_FRAGMENT_PROGRAM_ARB           0x8804

// This is what GL uses for handles when using extensions
typedef unsigned int GLhandleARB;
typedef char GLcharARB;


// Here are the multitexture function prototypes
typedef void (APIENTRY *PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY *PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
typedef int (*PFNWGLEXTGETSWAPINTERVALPROC) (void);
// Here we extern our function pointer for volumetric fog positioning
typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
// Below are all of our function pointer typedefs for all the extensions we need
typedef GLhandleARB (APIENTRYP PFNGLCREATESHADEROBJECTARBPROC)	(GLenum shaderType);
typedef void (APIENTRYP PFNGLSHADERSOURCEARBPROC)			(GLhandleARB shaderObj, GLsizei count, const GLcharARB* *string, const GLint *length);
typedef void (APIENTRYP PFNGLCOMPILESHADERARBPROC)			(GLhandleARB shaderObj);
typedef GLhandleARB (APIENTRYP PFNGLCREATEPROGRAMOBJECTARBPROC)	(void);
typedef void (APIENTRYP PFNGLATTACHOBJECTARBPROC)			(GLhandleARB containerObj, GLhandleARB obj);
typedef void (APIENTRYP PFNGLLINKPROGRAMARBPROC)			(GLhandleARB programObj);
typedef void (APIENTRYP PFNGLUSEPROGRAMOBJECTARBPROC)		(GLhandleARB programObj);
typedef void (APIENTRYP PFNGLVALIDATEPROGRAMARBPROC)		(GLhandleARB programObj);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONARBPROC)	(GLhandleARB programObj, const GLcharARB *name);
typedef void (APIENTRYP PFNGLDETACHOBJECTARBPROC)			(GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (APIENTRYP PFNGLDELETEOBJECTARBPROC)			(GLhandleARB obj);
typedef void (APIENTRYP PFNGLPROGRAMLOCALPARAMETER4FARBPROC)	(GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (APIENTRYP PFNGLBINDPROGRAMARBPROC)			(GLenum target, GLuint program);
typedef void (APIENTRYP PFNGLUNIFORM1FARBPROC)			(GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM1FVARBPROC)			(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM1IARBPROC)			(GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM1IVARBPROC)			(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM2FARBPROC)			(GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM2FVARBPROC)			(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM2IARBPROC)			(GLint location, GLint v0, GLint v1);
typedef void (APIENTRYP PFNGLUNIFORM2IVARBPROC)			(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM3FARBPROC)			(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM3FVARBPROC)			(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM3IARBPROC)			(GLint location, GLint v0, GLint v1, GLint v2);
typedef void (APIENTRYP PFNGLUNIFORM3IVARBPROC)			(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORM4FARBPROC)			(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORM4FVARBPROC)			(GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4IARBPROC)			(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (APIENTRYP PFNGLUNIFORM4IVARBPROC)			(GLint location, GLsizei count, const GLint* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX2FVARBPROC)	(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX3FVARBPROC)	(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVARBPROC)	(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

//typedef void (APIENTRYP * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
//typedef void (APIENTRYP * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
//typedef void (APIENTRYP * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
//typedef void (APIENTRYP * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
//typedef void (APIENTRYP * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
//typedef void (APIENTRYP * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
//typedef void (APIENTRYP * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid** params);
//typedef void (APIENTRYP * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
//typedef GLboolean (APIENTRYP * PFNGLISBUFFERARBPROC) (GLuint buffer);
//typedef GLvoid * (APIENTRYP * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
//typedef GLboolean (APIENTRYP * PFNGLUNMAPBUFFERARBPROC) (GLenum target);

// Vertex Buffer objects - begin
extern PFNGLBINDBUFFERPROC		glBindBuffer;
extern PFNGLBUFFERDATAPROC		glBufferData;
extern PFNGLBUFFERSUBDATAPROC	glBufferSubData;
extern PFNGLGENBUFFERSPROC		glGenBuffers;
extern PFNGLDELETEBUFFERSPROC	glDeleteBuffers;
// Vertex Buffer objects - end

// Here we extern our function pointers for OpenGL
extern PFNGLACTIVETEXTUREPROC			glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC		glClientActiveTexture;

extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;

extern PFNWGLEXTSWAPCONTROLPROC			glSwapIntervalEXT;
extern PFNWGLEXTGETSWAPINTERVALPROC		glGetSwapIntervalEXT;

// Here we extern our function pointer for volumetric fog positioning
extern PFNGLFOGCOORDFEXTPROC				glFogCoordfEXT;

// This declares our point sprite extension
extern PFNGLPOINTPARAMETERFARBPROC			glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC			glPointParameterfvARB;

// GL_ARB_occlusion_query - Begin
extern PFNGLGENQUERIESARBPROC				glGenQueriesARB;
extern PFNGLDELETEQUERIESARBPROC			glDeleteQueriesARB;
extern PFNGLISQUERYARBPROC					glIsQueryARB;
extern PFNGLBEGINQUERYARBPROC				glBeginQueryARB;
extern PFNGLENDQUERYARBPROC					glEndQueryARB;
extern PFNGLGETQUERYIVARBPROC				glGetQueryivARB;
extern PFNGLGETQUERYOBJECTIVARBPROC			glGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC		glGetQueryObjectuivARB;
// GL_ARB_occlusion_query - End

// Here we extern our functions to be used elsewhere
extern PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
extern PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
extern PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
extern PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
extern PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
extern PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
extern PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
extern PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB;
extern PFNGLUNIFORM1IARBPROC glUniform1iARB;
extern PFNGLUNIFORM2IARBPROC glUniform2iARB;
extern PFNGLUNIFORM3IARBPROC glUniform3iARB;
extern PFNGLUNIFORM4IARBPROC glUniform4iARB;
extern PFNGLUNIFORM1FARBPROC glUniform1fARB;
extern PFNGLUNIFORM2FARBPROC glUniform2fARB;
extern PFNGLUNIFORM3FARBPROC glUniform3fARB;
extern PFNGLUNIFORM4FARBPROC glUniform4fARB;
extern PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
extern PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
extern PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
extern PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
extern PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
extern PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
extern PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
extern PFNGLDETACHOBJECTARBPROC glDetachObjectARB;
extern PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
extern PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB;
extern PFNGLBINDPROGRAMARBPROC glBindProgramARB;
extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
*/



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
