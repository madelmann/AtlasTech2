
// Header
#include "Driver.h"

// Library includes
#include <assert.h>

// Project includes
#include "DriverInformation.h"
#include "OcclusionQuery.h"
#include <OpenGL/include/glext.h>

// Namespace declarations


// EXT_framebuffer_object function pointers - begin
// extension info can be found at http://oss.sgi.com/projects/ogl-sample/registry/EXT/framebuffer_object.txt
extern PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = NULL;
extern PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
extern PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
extern PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
extern PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = NULL;
extern PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
extern PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = NULL;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = NULL;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
extern PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;


const int RENDERBUFFER_WIDTH  = 256;
const int RENDERBUFFER_HEIGHT = 256;
// EXT_framebuffer_object function pointers - end


Driver::Driver(const Common::Logger *parent)
: Logger(parent, "Driver"),
  mCurrentOcclusionQuery(0),
  mDepthRenderBuffer(0),
  mError(0),
  mFrameBuffer(0)
{
	debug("Starting...");
}

Driver::~Driver()
{
	shutdown();
}

void Driver::bindFrameBufferEXT(AEuint targetId)
{
	//
	// Bind the frame-buffer object and attach to it a render-buffer object 
	// set up as a depth-buffer.
	//

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, mFrameBuffer );
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mDepthRenderBuffer );
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, targetId, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, mDepthRenderBuffer );
}

IOcclusionQuery* Driver::createOcclusionQuery()
{
	GLuint id;

	glGenQueriesARB(1, &id);

	if ( id == 0 ) {
		assert(!"could not create occlusion query!");
		return 0;
	}

	OcclusionQuery *query = new OcclusionQuery(id, this);

	mOcclusionQueries.push_front(query);

	return query;
}

void Driver::deleteOcclusionQuery(IOcclusionQuery *query)
{
	if ( !query ) {
		return;
	}

	GLuint id = query->getId();

	glDeleteQueriesARB(1, &id);

	for ( std::list<IOcclusionQuery*>::iterator it = mOcclusionQueries.begin(); it != mOcclusionQueries.end(); ++it ) {
		if ( (*it) == query ) {
			mOcclusionQueries.erase(it);
			delete query;
			return;
		}
	}

	assert(!"OcclusionQuery not deleted!");
}

void Driver::destroyFrameBufferEXT()
{
	if ( mDepthRenderBuffer ) {
		glDeleteRenderbuffersEXT( 1, &mDepthRenderBuffer );
	}
	if ( mFrameBuffer ) {
		glDeleteFramebuffersEXT ( 1, &mFrameBuffer );
	}
}

IOcclusionQuery* Driver::getCurrentOcclusionQuery() const
{
	return mCurrentOcclusionQuery;
}

char* Driver::getError() const
{
	return mError;
}

void Driver::initFrameBufferEXT()
{
	//
	// Create a frame-buffer object and a render-buffer object...
	//
	glGenRenderbuffersEXT( 1, &mDepthRenderBuffer );
	glGenFramebuffersEXT ( 1, &mFrameBuffer );

	// Initialize the render-buffer for usage as a depth buffer.
	// We don't really need this to render things into the frame-buffer object,
	// but without it the geometry will not be sorted properly.
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, mDepthRenderBuffer );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, RENDERBUFFER_WIDTH, RENDERBUFFER_HEIGHT );
}

void Driver::initVertexBufferObjects()
{
	//if ( !checkDriverExtension("GL_vertex_buffer_objects") ) {
	//	warn("GL_vertex_buffer_objects not supported");
	//	return;
	//}
	//else {
		// Now let's set all of our function pointers for our extension functions
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
		glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
		glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	//}

	mDriverExtensions.mVertexBufferObjects = true;
}

bool Driver::initialize()
{
	debug("Initializing...");

	DriverInformation &di = DriverInformation::getInstance();
	debug(di.getInfo());

	mError = "";

	// OpenGL Light
	{
		// Get maximum opengl lights
		glGetIntegerv(GL_MAX_LIGHTS, &mDriverExtensions.mMaxLights);
	}

	// Texture compression
	{
		glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &mDriverExtensions.mTextureCompression.mNumFormats);
		mDriverExtensions.mTextureCompression.mFormats = (AEint*)malloc(mDriverExtensions.mTextureCompression.mNumFormats * sizeof(AEint));
		glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, mDriverExtensions.mTextureCompression.mFormats);
	}

	// Check for anisotropic filter extension
	if ( di.isSupported("GL_EXT_texture_filter_anisotropic") ) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &mDriverExtensions.mAnsitropicFilter);
	}
	else {
		mError = "GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT";
		warn("GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT not supported");
	}

	// Find the correct function pointer that houses the fog coordinate function
	if ( di.isSupported("GL_EXT_fog_coord") ) {
		glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC)wglGetProcAddress("glFogCoordfEXT");

		mDriverExtensions.mglFogCoordfEXT = (glFogCoordfEXT != 0);
	}
	else {
		mError = "glFogCoordfEXT";
		warn("glFogCoordfEXT not supported");
	} 

	// Here we initialize our multitexturing functions
	if ( di.isSupported("GL_ARB_multitexture") ) {
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
		glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)wglGetProcAddress("glClientActiveTexture");

		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

		mDriverExtensions.mglActiveTextureARB = (aeActiveTexture != 0);
		mDriverExtensions.mglMultiTexCoord2fARB = (glMultiTexCoord2fARB != 0);
	}
	else {
		mError = "glActiveTextureARB";
		warn("glActiveTextureARB not supported");
    }

	if ( di.isSupported("GL_ARB_point_parameters") ) {
		glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
		glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

		mDriverExtensions.mGL_ARB_point_parameters = (glPointParameterfARB != 0) && (glPointParameterfvARB != 0);
	}
	else {
		mError = "GL_ARB_point_parameters";
		warn("GL_ARB_point_parameters not supported");
	}

 	if ( !di.isSupported("GL_ARB_shader_objects") ) {
		warn("GL_ARB_shader_objects not supported");
	}
	else {
		if ( !di.isSupported("GL_ARB_shading_language_100") ) {
			warn("GL_ARB_shading_language_100 not supported");
		}
		else {
			// Now let's set all of our function pointers for our extension functions
			glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
			glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
			glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
			glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
			glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
			glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
			glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
			glValidateProgramARB = (PFNGLVALIDATEPROGRAMARBPROC)wglGetProcAddress("glValidateProgramARB");
			glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
			glUniform2iARB = (PFNGLUNIFORM2IARBPROC)wglGetProcAddress("glUniform2iARB");
			glUniform3iARB = (PFNGLUNIFORM3IARBPROC)wglGetProcAddress("glUniform3iARB");
			glUniform4iARB = (PFNGLUNIFORM4IARBPROC)wglGetProcAddress("glUniform4iARB");
			glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
			glUniform2fARB = (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2fARB");
			glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
			glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
			glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC)wglGetProcAddress("glUniform1fvARB");
			glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC)wglGetProcAddress("glUniform2fvARB");
			glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fvARB");
			glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fvARB");
			glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC)wglGetProcAddress("glUniformMatrix3fvARB");
			glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress("glUniformMatrix4fvARB");
			glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocationARB");
			glDetachObjectARB = (PFNGLDETACHOBJECTARBPROC)wglGetProcAddress("glDetachObjectARB");
			glDeleteObjectARB  = (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteObjectARB");
			glProgramLocalParameter4fARB = (PFNGLPROGRAMLOCALPARAMETER4FARBPROC)wglGetProcAddress("glProgramLocalParameter4fARB");
			glBindProgramARB = (PFNGLBINDPROGRAMARBPROC)wglGetProcAddress("glBindProgramARB");
			glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetInfoLogARB");
			glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetObjectParameterivARB");

			mDriverExtensions.mglShaders = (glCreateShaderObjectARB != 0);
		}
	}

	//
	// EXT_framebuffer_object
	//
#ifdef _WIN32
    // check if FBO is supported by your video card
	if ( di.isSupported("GL_EXT_framebuffer_object") ) {
		glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		mDriverExtensions.mFrameBufferEXT = true;

		if( !glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT || 
			!glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT || 
			!glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT || 
			!glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT || 
			!glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT||  
			!glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT )
		{
			mError = "EXT_framebuffer_object";
			warn("EXT_framebuffer_object not supported");
			mDriverExtensions.mFrameBufferEXT = false;
		}
	}
#endif

	if ( mDriverExtensions.mFrameBufferEXT ) {
		initFrameBufferEXT();
	}

	initVertexBufferObjects();

/* Does not work in VM
	// Check for VSync extension
	if ( di.isSupported("WGL_EXT_swap_control") ) {
warn("swap control");
		//get address's of both functions and save them
		glSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)
		wglGetProcAddress("wglSwapIntervalEXT");
		glGetSwapIntervalEXT = (PFNWGLEXTGETSWAPINTERVALPROC)
		wglGetProcAddress("wglGetSwapIntervalEXT");
	}
	else {
warn("no swap control!");
		mError = "WGL_EXT_swap_control";
		return false;
	}
*/
	// Check for occlusion query extension
	if ( di.isSupported("GL_ARB_occlusion_query") ) {
		glGenQueriesARB        = (PFNGLGENQUERIESARBPROC)wglGetProcAddress("glGenQueriesARB");
        glDeleteQueriesARB     = (PFNGLDELETEQUERIESARBPROC)wglGetProcAddress("glDeleteQueriesARB");
        glIsQueryARB           = (PFNGLISQUERYARBPROC)wglGetProcAddress("glIsQueryARB");
        glBeginQueryARB        = (PFNGLBEGINQUERYARBPROC)wglGetProcAddress("glBeginQueryARB");
        glEndQueryARB          = (PFNGLENDQUERYARBPROC)wglGetProcAddress("glEndQueryARB");
        glGetQueryivARB        = (PFNGLGETQUERYIVARBPROC)wglGetProcAddress("glGetQueryivARB");
        glGetQueryObjectivARB  = (PFNGLGETQUERYOBJECTIVARBPROC)wglGetProcAddress("glGetQueryObjectivARB");
        glGetQueryObjectuivARB = (PFNGLGETQUERYOBJECTUIVARBPROC)wglGetProcAddress("glGetQueryObjectuivARB");

		mDriverExtensions.mOcclusionQuery = true;

        if( !glGenQueriesARB  || !glDeleteQueriesARB || !glIsQueryARB || 
            !glBeginQueryARB || !glEndQueryARB || !glGetQueryivARB ||
            !glGetQueryObjectivARB || !glGetQueryObjectuivARB )
		{
			mError = "GL_ARB_occlusion_query";
			warn("GL_ARB_occlusion_query not supported");
			mDriverExtensions.mOcclusionQuery = false;
        }
	}

	return true;
}

DriverExtensions* Driver::provideDriverExtensions()
{
	return &mDriverExtensions;
}

void Driver::setCurrentOcclusionQuery(IOcclusionQuery *query)
{
	mCurrentOcclusionQuery = query;
}

void Driver::shutdown()
{
	debug("Stopping...");

	destroyFrameBufferEXT();

	for ( std::list<IOcclusionQuery*>::iterator it = mOcclusionQueries.begin(); it != mOcclusionQueries.end(); ++it ) {
		delete (*it);
	}

	free(mDriverExtensions.mTextureCompression.mFormats);
}

void Driver::unbindFrameBufferEXT()
{
	//
	// Unbind the frame-buffer and render-buffer objects.
	//

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	//glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 );
}


