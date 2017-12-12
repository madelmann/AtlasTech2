
// Header
#include "Compiler.h"

// Library includes

// Project includes
#include <Driver/GLWrapper.h>
#include <Shader/Shader.h>
#include <Tools/Strings.h>

// Namespace declarations
using namespace ShaderSpace;


Compiler::Compiler(const Common::Logger *p)
: Common::Logger(p, "Compiler")
{
}

void Compiler::init()
{
	// Now let's set all of our function pointers for our extension functions
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgramObjectARB");
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
}

int Compiler::process(const std::string& filename)
{
	Shader shader(this, filename , "");

	return 0;
}
