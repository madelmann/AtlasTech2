
// Header
#include "GLWrapper.h"

// Library includes
#include <map>

// Project includes
#include "DriverInformation.h"
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Tools/Strings.h>

// Namespace declarations


static DriverInformation& DI = DriverInformation::getInstance();


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


namespace GL {


void CGLDriverStates::setCullMode(CGLDriverStates::CullMode_t cullMode)
{
	aeCullFace(cullMode == CCW ? GL_BACK : GL_FRONT);

	mCullMode = cullMode;
}

CGLDriverStates::CullMode_t CGLDriverStates::getCullMode() const
{
	return mCullMode;
}


}


// AtlasEngine driver wrappers

static unsigned int 				g_ActiveTexture		= 0;
static unsigned int					g_AttributesLeft	= 0;
static unsigned int					g_CullFace			= 0;
static GLboolean					g_DepthMaskEnabled	= false;
static std::map<GLenum, GLboolean>	g_DriverStates;
static unsigned int					g_ImmediateModeLeft	= 0;
static unsigned int 				g_MaxtrixesLeft		= 0;


void aeActiveTexture (unsigned int target)
{
	if ( g_ActiveTexture == target ) {
		return;
	}

	g_ActiveTexture = target;
	glActiveTexture(target);
}

void aeAlphaFunc(unsigned int func, GLclampf ref)
{
	glAlphaFunc(func, ref);
}

void aeBegin(unsigned int mode)
{
	// Increment counter...
	g_ImmediateModeLeft++;
	// ... and enter immediate mode
	glBegin(mode);
}

void aeBeginQuery(unsigned int id)
{
	glBeginQueryARB(GL_SAMPLES_PASSED_ARB, id);
}

void aeBlendFunc(unsigned int sfactor, unsigned int dfactor)
{
	glBlendFunc(sfactor, dfactor);
}

void aeCallList(unsigned int list)
{
	glCallList(list);
}

void aeColor(const vector3f& color)
{
	//glColor3f(color.x, color.y, color.z);
	glColor4f(color.x, color.y, color.z, 1.f);
}

void aeColor(const vector4f& color)
{
	glColor4f(color.x, color.y, color.z, color.w);
}

void aeColor3f(float c1, float c2, float c3)
{
	glColor3f(c1, c2, c3);
}

void aeColor4f(float c1, float c2, float c3, float c4)
{
	glColor4f(c1, c2, c3, c4);
}

void aeCreateBuffer (unsigned int &bufferId)
{
    // Make sure we don't loose the reference to the previous buffer if there is one
    aeDeleteBuffer(bufferId);
    glGenBuffers(1, &bufferId);
}

void aeCullFace (unsigned int mode)
{
	if ( g_CullFace == mode ) {
		return;
	}

	g_CullFace = mode;
	glCullFace(mode);
}

void aeDeleteBuffer (unsigned int &bufferId)
{
    if ( bufferId != 0 ) {
        glDeleteBuffers(1, &bufferId);
        bufferId = 0;
    }
}

void aeDeleteLists(unsigned int list, AEsizei range)
{
	if ( list != 0 ) {
		glDeleteLists(list, range);
		list = 0;
	}
}

void aeDepthFunc(unsigned int func)
{
	glDepthFunc(func);
}

void aeDepthMask(unsigned char flag, bool force)
{
	if ( force || g_DepthMaskEnabled != flag) {
		glDepthMask(flag);
	}

	g_DepthMaskEnabled = flag;
}

void aeDisable(unsigned int cap, bool force)
{
	if ( force || g_DriverStates[cap] != GL_FALSE ) {
		g_DriverStates[cap] = GL_FALSE;
		glDisable(cap);
	}
}

void aeEnable(unsigned int cap, bool force)
{
	if ( force || g_DriverStates[cap] != GL_TRUE ) {
		g_DriverStates[cap] = GL_TRUE;
		glEnable(cap);
	}
}

void aeEnd()
{
	// Exit immediate mode...
	glEnd();
	// ... and decrement counter
	g_ImmediateModeLeft--;
}

void aeEndList()
{
	glEndList();
}

void aeEndQuery()
{
	glEndQueryARB(GL_SAMPLES_PASSED_ARB);
}

void aeForceDisable ( unsigned int cap )
{
	g_DriverStates[cap] = GL_FALSE;
	glDisable(cap);	
}

void aeForceEnable ( unsigned int cap )
{
	g_DriverStates[cap] = GL_TRUE;
	glEnable(cap);
}

unsigned int aeGenLists (int range)
{
	return glGenLists(range);
}

unsigned int aeGetAttributesLeft()
{
	return g_AttributesLeft;
}

unsigned int aeGetImmediateModeLeft()
{
	return g_ImmediateModeLeft;
}

unsigned int aeGetMatricesLeft()
{
	return g_MaxtrixesLeft;
}

bool aeGetState (unsigned int cap)
{
	GLboolean state;

	glGetBooleanv(cap, &state);

	return (state > 0);
}

void aeHint(unsigned int target, unsigned int mode)
{
	glHint(target, mode);
}

unsigned char aeIsEnabled (unsigned int cap)
{
	return glIsEnabled(cap);
}

unsigned char aeIsList (unsigned int list)
{
	return glIsList(list);
}

void aeLoadIdentity ()
{
	glLoadIdentity();
}

void aeMaterialf(unsigned int face, unsigned int pname, float param)
{
	glMaterialf(face, pname, param);
}

void aeMaterialfv(unsigned int face, unsigned int pname, float *params)
{
	glMaterialfv(face, pname, params);
}

void aeMaterialfv(unsigned int face, unsigned int pname, vector4f *params)
{
	float p[4];

	p[0] = params->x;
	p[1] = params->y;
	p[2] = params->z;
	p[3] = params->w;

	glMaterialfv(face, pname, p);
}

void aeMaterialfv(unsigned int face, unsigned int pname, vector4f& params)
{
	float p[4];

	p[0] = params.x;
	p[1] = params.y;
	p[2] = params.z;
	p[3] = params.w;

	glMaterialfv(face, pname, p);
}

void aeNewList(unsigned int list, unsigned int mode)
{
	glNewList(list, mode);
}

void aeNormal3f(float nx, float ny, float nz)
{
	glNormal3f(nx, ny, nz);
}

void aePopAttrib()
{
	DI.pop_AttributeStack();

	// Pop attributes...
	glPopAttrib();
	// ... and decrement counter
	g_AttributesLeft--;
}

void aePopMatrix()
{
	DI.pop_Matrix_ModelViewStack();

	// Pop matrix...
	glPopMatrix();
	// ... and decrement counter
	g_MaxtrixesLeft--;
}

void aePushAttrib(GLbitfield mask)
{
	DI.push_AttributeStack();

	// Increment counter...
	g_AttributesLeft++;
	// ... and push attributes
	glPushAttrib(mask);
}

void aePushMatrix()
{
	DI.push_Matrix_ModelViewStack();

	// Increment counter...
	g_MaxtrixesLeft++;
	// ... and push matrix
	glPushMatrix();
}

void aeRotate(const vector3f& v)
{
	glRotatef(v.x, 1.f, 0.f, 0.f);
	glRotatef(v.y, 0.f, 1.f, 0.f);
	glRotatef(v.z, 0.f, 0.f, 1.f);
}

void aeRotatef (float angle, float x, float y, float z)
{
	glRotatef(angle, x, y, z);
}

void aeRotatev3 (float angle, const vector3f& v)
{
	glRotatef(angle, v.x, v.y, v.z);
}

void aeSetState (unsigned int cap, bool state, bool force)
{
	if ( state ) {
		aeEnable(cap, force);
	}
	else {
		aeDisable(cap, force);
	}
}

std::string aeStackInfo()
{
	//g_CullFace, g_DriverStates

	std::string result;
	result += "\n";
	result += "=== aeStackInfo ===\n";
	//result += "Active Texture Unit: " + Tools::toString(g_ActiveTexture) + "\n";
	//result += "Depth Mask Enabled: " + Tools::toString(g_DepthMaskEnabled) + "\n";
	result += "Immediate Mode Active: " + Tools::toString(g_ImmediateModeLeft) + "\n";
	result += "Pushed Attributes: " + Tools::toString(g_AttributesLeft) + "\n";
	result += "Pushed Matrices: " + Tools::toString(g_MaxtrixesLeft) + "\n";
	result += "===================\n";

	return result;
}

void aeTexCoord2f(float s, float t)
{
	glTexCoord2f(s, t);
}

void aeTranslate(const vector3f& v)
{
	glTranslatef(v.x, v.y, v.z);
}

void aeTranslatef(float x, float y, float z)
{
	glTranslatef(x, y, z);
}

void aeVertex(const vector2f& v)
{
	aeVertex2f(v.x, v.y);
}

void aeVertex(const vector3f& v)
{
	aeVertex3f(v.x, v.y, v.z);
}

void aeVertex2f(float x, float y)
{
	glVertex2f(x, y);
}

void aeVertex3f(float x, float y, float z)
{
	glVertex3f(x, y, z);
}

void aeVertex3v(const vector3f& v)
{
	aeVertex3f(v.x, v.y, v.z);
}

void aeViewport(int x, int y, AEsizei width, AEsizei height)
{
	glViewport(x, y, width, height);
}

void aeViewport(float x, float y, float width, float height)
{
	glViewport((int)x, (int)y, (AEsizei)width, (AEsizei)height);
}
