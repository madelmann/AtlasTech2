
#ifndef _Physics_BulletDebugRenderer_h_
#define _Physics_BulletDebugRenderer_h_


// Library includes
#include "LinearMath/btIDebugDraw.h"


class BulletDebugRenderer : public btIDebugDraw
{
public:
	BulletDebugRenderer();

public:
	virtual void setDebugMode(int debugMode) { m_debugMode = debugMode; }
	virtual int getDebugMode() const { return m_debugMode; }

	virtual void draw3dText(const btVector3& location,const char* textString);
	virtual void drawBox (const btVector3& boxMin, const btVector3& boxMax, const btVector3& color, btScalar alpha);
	virtual void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& fromColor, const btVector3& toColor);
	virtual void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);
	virtual void drawSphere (const btVector3& p, btScalar radius, const btVector3& color);
	virtual void drawTriangle(const btVector3& a,const btVector3& b,const btVector3& c,const btVector3& color,btScalar alpha);

	virtual void reportErrorWarning(const char* warningString);

private:
	int	m_debugMode;
};


#endif
