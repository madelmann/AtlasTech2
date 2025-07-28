#ifndef _Driver_GLWrapper_h_
#define _Driver_GLWrapper_h_


// Library includes
#ifdef _WIN32
#include <windows.h>
#include <OpenGL/include/gl.h>
#include <OpenGL/include/glext.h>
#include <OpenGL/include/glu.h>
#include <OpenGL/include/glaux.h>
#else
// Linux/SDL implementation
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
// Note: We use SDL's built-in OpenGL function loading
#endif

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


// OpenGL extension function pointers that may not be available in all drivers
// Only declare these if not already defined by SDL/system headers

#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1
#endif

#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1
#endif

// Function pointers for extensions that might not be core
#ifdef _WIN32
// Windows-specific extension loading
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;
#endif

// Fog extension - not always available
#ifndef glFogCoordfEXT
extern PFNGLFOGCOORDFEXTPROC glFogCoordfEXT;
#endif

// Point parameter extensions
extern PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB;
extern PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB;

// Shader extensions (ARB versions for compatibility)
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

// Occlusion query extensions
extern PFNGLGENQUERIESARBPROC glGenQueriesARB;
extern PFNGLDELETEQUERIESARBPROC glDeleteQueriesARB;
extern PFNGLISQUERYARBPROC glIsQueryARB;
extern PFNGLBEGINQUERYARBPROC glBeginQueryARB;
extern PFNGLENDQUERYARBPROC glEndQueryARB;
extern PFNGLGETQUERYIVARBPROC glGetQueryivARB;
extern PFNGLGETQUERYOBJECTIVARBPROC glGetQueryObjectivARB;
extern PFNGLGETQUERYOBJECTUIVARBPROC glGetQueryObjectuivARB;

// Initialize OpenGL extensions
bool initializeGLExtensions();

// GL state management namespace
namespace GL {

class CGLDriverStates
{
public:
	enum CullMode_t {
		CCW = 0,
		CW
	};

public:
	void setCullMode(CullMode_t cullMode);
	CullMode_t getCullMode() const;

private:
	CullMode_t mCullMode;
};

}

// AtlasEngine OpenGL wrapper functions
// These provide a consistent interface across platforms

void aeActiveTexture(unsigned int target);
void aeAlphaFunc(unsigned int func, GLclampf ref);
void aeBegin(unsigned int mode);
void aeBeginQuery(unsigned int id);
void aeBindTexture(unsigned int target, unsigned int texture);
void aeBlendFunc(unsigned int sfactor, unsigned int dfactor);
void aeCallList(unsigned int list);
void aeClear(unsigned int mask);
void aeClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void aeClearDepth(GLclampd depth);
void aeColor3f(float red, float green, float blue);
void aeColor3v(const vector3f& color);
void aeColor4f(float red, float green, float blue, float alpha);
void aeColor4v(const vector4f& color);
void aeColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void aeCullFace(unsigned int mode);
void aeDeleteLists(unsigned int list, AEsizei range);
void aeDeleteQueries(AEsizei n, const unsigned int *ids);
void aeDeleteTextures(AEsizei n, const unsigned int *textures);
void aeDepthFunc(unsigned int func);
void aeDepthMask(GLboolean flag);
void aeDisable(unsigned int cap);
void aeDrawArrays(unsigned int mode, int first, AEsizei count);
void aeEnable(unsigned int cap);
void aeEnd();
void aeEndQuery(unsigned int target);
GLboolean aeIsEnabled(unsigned int cap);
void aeForceEnable(unsigned int cap);
void aeFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
unsigned int aeGenLists(AEsizei range);
void aeGenQueries(AEsizei n, unsigned int *ids);
void aeGenTextures(AEsizei n, unsigned int *textures);
void aeGetBooleanv(unsigned int pname, GLboolean *params);
void aeGetDoublev(unsigned int pname, GLdouble *params);
void aeGetFloatv(unsigned int pname, GLfloat *params);
void aeGetIntegerv(unsigned int pname, int *params);
void aeGetQueryObjectuiv(unsigned int id, unsigned int pname, unsigned int *params);
const GLubyte* aeGetString(unsigned int name);
void aeHint(unsigned int target, unsigned int mode);
void aeLightf(unsigned int light, unsigned int pname, GLfloat param);
void aeLightfv(unsigned int light, unsigned int pname, const GLfloat *params);
void aeLightModelf(unsigned int pname, GLfloat param);
void aeLightModelfv(unsigned int pname, const GLfloat *params);
void aeLoadIdentity();
void aeLoadMatrixf(const GLfloat *m);
void aeMaterialf(unsigned int face, unsigned int pname, GLfloat param);
void aeMaterialfv(unsigned int face, unsigned int pname, const GLfloat *params);
void aeMatrixMode(unsigned int mode);
void aeMultMatrixf(const GLfloat *m);
void aeNewList(unsigned int list, unsigned int mode);
void aeNormal3f(float nx, float ny, float nz);
void aeNormal3v(const vector3f& n);
void aeOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
void aePerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
void aePointParameterf(unsigned int pname, GLfloat param);
void aePointParameterfv(unsigned int pname, const GLfloat *params);
void aePointSize(GLfloat size);
void aePolygonMode(unsigned int face, unsigned int mode);
void aePopMatrix();
void aePushMatrix();
void aeRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
void aeScalef(GLfloat x, GLfloat y, GLfloat z);
void aeTexCoord2f(GLfloat s, GLfloat t);
void aeTexCoord2v(const vector2f& t);
void aeTexEnvf(unsigned int target, unsigned int pname, GLfloat param);
void aeTexEnvi(unsigned int target, unsigned int pname, int param);
void aeTexImage2D(unsigned int target, int level, int internalformat, AEsizei width, AEsizei height, int border, unsigned int format, unsigned int type, const GLvoid *pixels);
void aeTexParameterf(unsigned int target, unsigned int pname, GLfloat param);
void aeTexParameteri(unsigned int target, unsigned int pname, int param);
void aeTranslatef(GLfloat x, GLfloat y, GLfloat z);
void aeVertex2f(float x, float y);
void aeVertex2v(const vector2f& v);
void aeVertex3f(float x, float y, float z);
void aeVertex3v(const vector3f& v);
void aeViewport(int x, int y, AEsizei width, AEsizei height);
void aeViewport(float x, float y, float width, float height);

// Debug functions
unsigned int aeGetAttributesLeft();
unsigned int aeGetImmediateModeLeft();
unsigned int aeGetMatricesLeft();


#endif