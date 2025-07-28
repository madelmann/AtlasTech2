// Header
#include "GLWrapper.h"

// Library includes
#include <map>
#include <cmath>

// Project includes
#include "DriverInformation.h"
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Tools/Strings.h>

// Namespace declarations

static DriverInformation& DI = DriverInformation::getInstance();

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

void aeActiveTexture(unsigned int target)
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
	// For minimal version, skip occlusion queries
	// glBeginQueryARB(GL_SAMPLES_PASSED_ARB, id);
}

void aeBindTexture(unsigned int target, unsigned int texture)
{
	glBindTexture(target, texture);
}

void aeBlendFunc(unsigned int sfactor, unsigned int dfactor)
{
	glBlendFunc(sfactor, dfactor);
}

void aeCallList(unsigned int list)
{
	glCallList(list);
}

void aeClear(unsigned int mask)
{
	glClear(mask);
}

void aeClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
}

void aeClearDepth(GLclampd depth)
{
	glClearDepth(depth);
}

void aeColor3f(float red, float green, float blue)
{
	glColor3f(red, green, blue);
}

void aeColor3v(const vector3f& color)
{
	aeColor3f(color.x, color.y, color.z);
}

void aeColor4f(float red, float green, float blue, float alpha)
{
	glColor4f(red, green, blue, alpha);
}

void aeColor4v(const vector4f& color)
{
	aeColor4f(color.x, color.y, color.z, color.w);
}

void aeColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	glColorMask(red, green, blue, alpha);
}

void aeCullFace(unsigned int mode)
{
	if ( g_CullFace == mode ) {
		return;
	}

	g_CullFace = mode;
	glCullFace(mode);
}

void aeDeleteLists(unsigned int list, AEsizei range)
{
	glDeleteLists(list, range);
}

void aeDeleteQueries(AEsizei n, const unsigned int *ids)
{
	// For minimal version, skip occlusion queries
	// glDeleteQueriesARB(n, ids);
}

void aeDeleteTextures(AEsizei n, const unsigned int *textures)
{
	glDeleteTextures(n, textures);
}

void aeDepthFunc(unsigned int func)
{
	glDepthFunc(func);
}

void aeDepthMask(GLboolean flag)
{
	if ( g_DepthMaskEnabled == flag ) {
		return;
	}

	g_DepthMaskEnabled = flag;
	glDepthMask(flag);
}

void aeDisable(unsigned int cap)
{
	std::map<GLenum, GLboolean>::iterator it = g_DriverStates.find(cap);
	if ( it != g_DriverStates.end() ) {
		if ( (*it).second == false ) {
			return;
		}
	}

	g_DriverStates[cap] = false;
	glDisable(cap);
}

void aeDrawArrays(unsigned int mode, int first, AEsizei count)
{
	glDrawArrays(mode, first, count);
}

void aeEnable(unsigned int cap)
{
	std::map<GLenum, GLboolean>::iterator it = g_DriverStates.find(cap);
	if ( it != g_DriverStates.end() ) {
		if ( (*it).second == true ) {
			return;
		}
	}

	g_DriverStates[cap] = true;
	glEnable(cap);
}

void aeEnd()
{
	// Decrement counter...
	g_ImmediateModeLeft--;
	// ... and leave immediate mode
	glEnd();
}

void aeEndQuery(unsigned int target)
{
	// For minimal version, skip occlusion queries
	// glEndQueryARB(target);
}

GLboolean aeIsEnabled(unsigned int cap)
{
	return glIsEnabled(cap);
}

void aeForceEnable(unsigned int cap)
{
	g_DriverStates[cap] = true;
	glEnable(cap);
}

void aeFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glFrustum(left, right, bottom, top, zNear, zFar);
}

unsigned int aeGenLists(AEsizei range)
{
	return glGenLists(range);
}

void aeGenQueries(AEsizei n, unsigned int *ids)
{
	// For minimal version, skip occlusion queries
	// glGenQueriesARB(n, ids);
	for (int i = 0; i < n; i++) {
		ids[i] = 0;
	}
}

void aeGenTextures(AEsizei n, unsigned int *textures)
{
	glGenTextures(n, textures);
}

void aeGetBooleanv(unsigned int pname, GLboolean *params)
{
	glGetBooleanv(pname, params);
}

void aeGetDoublev(unsigned int pname, GLdouble *params)
{
	glGetDoublev(pname, params);
}

void aeGetFloatv(unsigned int pname, GLfloat *params)
{
	glGetFloatv(pname, params);
}

void aeGetIntegerv(unsigned int pname, int *params)
{
	glGetIntegerv(pname, params);
}

void aeGetQueryObjectuiv(unsigned int id, unsigned int pname, unsigned int *params)
{
	// For minimal version, skip occlusion queries
	// glGetQueryObjectuivARB(id, pname, params);
	*params = 0;
}

const GLubyte* aeGetString(unsigned int name)
{
	return glGetString(name);
}

void aeHint(unsigned int target, unsigned int mode)
{
	glHint(target, mode);
}

void aeLightf(unsigned int light, unsigned int pname, GLfloat param)
{
	glLightf(light, pname, param);
}

void aeLightfv(unsigned int light, unsigned int pname, const GLfloat *params)
{
	glLightfv(light, pname, params);
}

void aeLightModelf(unsigned int pname, GLfloat param)
{
	glLightModelf(pname, param);
}

void aeLightModelfv(unsigned int pname, const GLfloat *params)
{
	glLightModelfv(pname, params);
}

void aeLoadIdentity()
{
	glLoadIdentity();
}

void aeLoadMatrixf(const GLfloat *m)
{
	glLoadMatrixf(m);
}

void aeMaterialf(unsigned int face, unsigned int pname, GLfloat param)
{
	glMaterialf(face, pname, param);
}

void aeMaterialfv(unsigned int face, unsigned int pname, const GLfloat *params)
{
	glMaterialfv(face, pname, params);
}

void aeMatrixMode(unsigned int mode)
{
	glMatrixMode(mode);
}

void aeMultMatrixf(const GLfloat *m)
{
	glMultMatrixf(m);
}

void aeNewList(unsigned int list, unsigned int mode)
{
	glNewList(list, mode);
}

void aeNormal3f(float nx, float ny, float nz)
{
	glNormal3f(nx, ny, nz);
}

void aeNormal3v(const vector3f& n)
{
	aeNormal3f(n.x, n.y, n.z);
}

void aeOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glOrtho(left, right, bottom, top, zNear, zFar);
}

void aePerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
#ifdef HAVE_GLU
	gluPerspective(fovy, aspect, zNear, zFar);
#else
	// Simple perspective implementation without GLU
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
	GLdouble fH = tan(fovy / 360 * M_PI) * zNear;
	GLdouble fW = fH * aspect;
	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
#endif
}

void aePointParameterf(unsigned int pname, GLfloat param)
{
	// For minimal version, skip point parameters
	// glPointParameterfARB(pname, param);
}

void aePointParameterfv(unsigned int pname, const GLfloat *params)
{
	// For minimal version, skip point parameters
	// glPointParameterfvARB(pname, params);
}

void aePointSize(GLfloat size)
{
	glPointSize(size);
}

void aePolygonMode(unsigned int face, unsigned int mode)
{
	glPolygonMode(face, mode);
}

void aePopMatrix()
{
	g_MaxtrixesLeft--;
	glPopMatrix();
}

void aePushMatrix()
{
	g_MaxtrixesLeft++;
	glPushMatrix();
}

void aeRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
	glRotatef(angle, x, y, z);
}

void aeScalef(GLfloat x, GLfloat y, GLfloat z)
{
	glScalef(x, y, z);
}

void aeTexCoord2f(GLfloat s, GLfloat t)
{
	glTexCoord2f(s, t);
}

void aeTexCoord2v(const vector2f& t)
{
	aeTexCoord2f(t.x, t.y);
}

void aeTexEnvf(unsigned int target, unsigned int pname, GLfloat param)
{
	glTexEnvf(target, pname, param);
}

void aeTexEnvi(unsigned int target, unsigned int pname, int param)
{
	glTexEnvi(target, pname, param);
}

void aeTexImage2D(unsigned int target, int level, int internalformat, AEsizei width, AEsizei height, int border, unsigned int format, unsigned int type, const GLvoid *pixels)
{
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void aeTexParameterf(unsigned int target, unsigned int pname, GLfloat param)
{
	glTexParameterf(target, pname, param);
}

void aeTexParameteri(unsigned int target, unsigned int pname, int param)
{
	glTexParameteri(target, pname, param);
}

void aeTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
	glTranslatef(x, y, z);
}

void aeVertex2f(float x, float y)
{
	glVertex2f(x, y);
}

void aeVertex2v(const vector2f& v)
{
	aeVertex2f(v.x, v.y);
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

// Debug functions
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