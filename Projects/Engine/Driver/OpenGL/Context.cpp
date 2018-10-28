
// Header
#include "Context.h"

// Library includes

// Project includes

// Namespace declarations



// fog extension function pointer - begin
PFNGLFOGCOORDFEXTPROC			glFogCoordfEXT = NULL;
// fog extension function pointer - end

// ARB multitexturing function pointers - begin
PFNGLACTIVETEXTUREPROC			glActiveTexture = NULL;
PFNGLCLIENTACTIVETEXTUREPROC	glClientActiveTexture = NULL;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB = NULL;
// ARB multitexturing function pointers - end

// glPointParameter - begin
PFNGLPOINTPARAMETERFARBPROC		glPointParameterfARB = NULL;
PFNGLPOINTPARAMETERFVARBPROC	glPointParameterfvARB = NULL;
// glPointParameter - end

// The function pointers for shaders
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;
PFNGLVALIDATEPROGRAMARBPROC glValidateProgramARB = NULL;
PFNGLUNIFORM1IARBPROC glUniform1iARB = NULL;
PFNGLUNIFORM2IARBPROC glUniform2iARB = NULL;
PFNGLUNIFORM3IARBPROC glUniform3iARB = NULL;
PFNGLUNIFORM4IARBPROC glUniform4iARB = NULL;
PFNGLUNIFORM1FARBPROC glUniform1fARB = NULL;
PFNGLUNIFORM2FARBPROC glUniform2fARB = NULL;
PFNGLUNIFORM3FARBPROC glUniform3fARB = NULL;
PFNGLUNIFORM4FARBPROC glUniform4fARB = NULL;
PFNGLUNIFORM1FVARBPROC glUniform1fvARB = NULL;
PFNGLUNIFORM2FVARBPROC glUniform2fvARB = NULL;
PFNGLUNIFORM3FVARBPROC glUniform3fvARB = NULL;
PFNGLUNIFORM4FVARBPROC glUniform4fvARB = NULL;
PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB = NULL;
PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = NULL;
PFNGLDETACHOBJECTARBPROC glDetachObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = NULL;
PFNGLPROGRAMLOCALPARAMETER4FARBPROC glProgramLocalParameter4fARB = NULL;
PFNGLBINDPROGRAMARBPROC glBindProgramARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = NULL;

// WGL_EXT_swap_control - begin
PFNWGLEXTSWAPCONTROLPROC		glSwapIntervalEXT = NULL;
PFNWGLEXTGETSWAPINTERVALPROC	glGetSwapIntervalEXT = NULL;
// WGL_EXT_swap_control - end

// Vertex Buffer objects - begin
PFNGLBINDBUFFERPROC		glBindBuffer = NULL;
PFNGLBUFFERDATAPROC		glBufferData = NULL;
PFNGLBUFFERSUBDATAPROC	glBufferSubData = NULL;
PFNGLGENBUFFERSPROC		glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers = NULL;
// Vertex Buffer objects - end

// GL_ARB_occlusion_query function pointers - begin
PFNGLGENQUERIESARBPROC			glGenQueriesARB = NULL;
PFNGLDELETEQUERIESARBPROC		glDeleteQueriesARB = NULL;
PFNGLISQUERYARBPROC				glIsQueryARB = NULL;
PFNGLBEGINQUERYARBPROC			glBeginQueryARB = NULL;
PFNGLENDQUERYARBPROC			glEndQueryARB = NULL;
PFNGLGETQUERYIVARBPROC			glGetQueryivARB = NULL;
PFNGLGETQUERYOBJECTIVARBPROC	glGetQueryObjectivARB = NULL;
PFNGLGETQUERYOBJECTUIVARBPROC	glGetQueryObjectuivARB = NULL;
// GL_ARB_occlusion_query function pointers - end



namespace OpenGL {


Context::Context(const Common::Logger *p)
: Common::Logger(p, "OpenGL::Context"),
  mDeviceContext(0),
  mRenderingContext(0),
  mWindowHandle(0)
{
}

Context::~Context()
{
	shutdown();
}

bool Context::change(const Settings& s)
{
	// validate & execute changes ...

	// ... and update our settings
	mSettings = s;

	return true;
}

bool Context::createOpenGLContext()
{
	mDeviceContext = GetDC(mWindowHandle);
	if ( !mDeviceContext ) {
		return false;
	}

	return true;
}

void Context::destroyOpenGLContext()
{
	wglMakeCurrent(mDeviceContext, 0);
	wglDeleteContext(mRenderingContext);

	ReleaseDC(mWindowHandle, mDeviceContext);
}

const Context::Settings& Context::provideSettings() const
{
	return mSettings;
}

bool Context::setup(HWND hwnd, const Settings& s)
{
	// set window handle
	mWindowHandle = hwnd;

	// set our opengl context settings
	mSettings = s;

	// initialize
	startup();

	return true;
}

void Context::shutdown()
{
	destroyOpenGLContext();
}

void Context::startup()
{
	createOpenGLContext();
}


}
