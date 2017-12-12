
// Header
#include "DriverUtils.h"

// Library includes

// Project includes
#include "GLWrapper.h"

// Namespace declarations


vector3f glPosition3d(const vector2l& pos)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)pos.x;
	winY = (float)viewport[3] - (float)pos.y;
	glReadPixels( int(winX), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	return vector3f( (float)posX, (float)posY, (float)posZ );
}

void glRenderText3v(const vector3f& pos, const std::string& text)
{
	glRasterPos3f(pos.x, pos.y, pos.z);
	for ( unsigned int c = 0; c < text.size(); ++c ) {
		//gluBitmapCharacter(((void *)0x0004), text[c]);
	}
}
