
// Header
#include "DriverInformation.h"

// Library includes
#include <algorithm>
#include <sstream>

// Project includes
#include "GLWrapper.h"

// Namespace declarations


DriverInformation::DriverInformation()
: redBits(0), greenBits(0), blueBits(0), alphaBits(0), depthBits(0),
  stencilBits(0), maxTextureSize(0), maxLights(0), maxAttribStacks(0),
  maxModelViewStacks(0), maxClipPlanes(0), maxTextureStacks(0),
  mCompactDriverInfo(true),
  mStack_Attribute(0),
  mStack_Matrix_ModelView(0),
  mStack_Matrix_Projection(0),
  mStack_Texture(0)
{
}

std::string DriverInformation::getInfo()
{
	readInfo();

    std::stringstream ss;

    ss << std::endl; // blank line
    ss << "==================" << std::endl;
    ss << "OpenGL Driver Info" << std::endl;
    ss << "==================" << std::endl;
    ss << "Vendor: " << this->vendor << std::endl;
    ss << "Version: " << this->version << std::endl;
    ss << "Renderer: " << this->renderer << std::endl;

    ss << std::endl;
    ss << "Color Bits(R,G,B,A): (" << this->redBits << ", " << this->greenBits
       << ", " << this->blueBits << ", " << this->alphaBits << ")\n";
	ss << "Depth Bits: " << this->depthBits << std::endl;
	ss << "Stencil Bits: " << this->stencilBits << std::endl;

	ss << std::endl;
	ss << "Max Clip Planes: " << this->maxClipPlanes << std::endl;
	ss << "Max Lights: " << this->maxLights << std::endl;
	ss << "Max Texture Size: " << this->maxTextureSize << "x" << this->maxTextureSize << std::endl;

	ss << std::endl;
	ss << "Max Attribute Stacks: " << this->maxAttribStacks << std::endl;
	ss << "Max Modelview Matrix Stacks: " << this->maxModelViewStacks << std::endl;
	ss << "Max Projection Matrix Stacks: " << this->maxProjectionStacks << std::endl;
	ss << "Max Texture Stacks: " << this->maxTextureStacks << std::endl;

	ss << std::endl;
	if ( !mCompactDriverInfo ) {
		ss << "Total Number of Extensions:" << this->extensions.size() << std::endl;
		ss << "===========================" << std::endl;
		for(unsigned int i = 0; i < this->extensions.size(); ++i) {
			ss << this->extensions.at(i) << std::endl;
		}
	}

    ss << "==================";/* << std::endl;*/

	return ss.str();
}

bool DriverInformation::isSupported(const std::string& ext)
{
    // search corresponding extension
    std::vector<std::string>::const_iterator iter = this->extensions.begin();
    std::vector<std::string>::const_iterator endIter = this->extensions.end();

    while ( iter != endIter ) {
		if ( ext == *iter ) {
            return true;
		}
		else {
            ++iter;
		}
    }

    return false;
}

void DriverInformation::pop_AttributeStack()
{
	mStack_Attribute--;
	if ( !(mStack_Attribute >= 0) ) {
		assert(mStack_Attribute >= 0);
	}
}

void DriverInformation::pop_Matrix_ModelViewStack()
{
	mStack_Matrix_ModelView--;
	if ( !(mStack_Matrix_ModelView >= 0) ) {
		assert(mStack_Matrix_ModelView >= 0);
	}
}

void DriverInformation::pop_Matrix_ProjectionStack()
{
	mStack_Matrix_Projection--;
	if ( !(mStack_Matrix_Projection >= 0) ) {
		assert(mStack_Matrix_Projection >= 0);
	}
}

void DriverInformation::pop_TextureStack()
{
	mStack_Texture--;
	if ( !(mStack_Texture >= 0) ) {
		assert(mStack_Texture >= 0);
	}
}

void DriverInformation::push_AttributeStack()
{
	mStack_Attribute++;
	if ( !(mStack_Attribute <= maxAttribStacks) ) {
		assert(mStack_Attribute <= maxAttribStacks);
	}
}

void DriverInformation::push_Matrix_ModelViewStack()
{
	mStack_Matrix_ModelView++;
	if ( !(mStack_Matrix_ModelView <= maxModelViewStacks) ) {
		assert(mStack_Matrix_ModelView <= maxModelViewStacks);
	}
}

void DriverInformation::push_Matrix_ProjectionStack()
{
	mStack_Matrix_Projection++;
	if ( !(mStack_Matrix_Projection <= maxProjectionStacks) ) {
		assert(mStack_Matrix_Projection <= maxProjectionStacks);
	}
}

void DriverInformation::push_TextureStack()
{
	mStack_Texture++;
	if ( !(mStack_Texture <= maxTextureStacks) ) {
		assert(mStack_Texture <= maxTextureStacks);
	}
}

void DriverInformation::readInfo()
{
    char* str = 0;
    char* tok = 0;

    // get vendor string
    str = (char*)glGetString(GL_VENDOR);
    if ( str )
		this->vendor = str;                  // check NULL return value
	else
		return;

    // get renderer string
    str = (char*)glGetString(GL_RENDERER);
    if ( str )
		this->renderer = str;                // check NULL return value
    else
		return;

    // get version string
    str = (char*)glGetString(GL_VERSION);
    if ( str )
		this->version = str;                 // check NULL return value
    else
		return;

	// get all extensions as a string
	str = (char*)glGetString(GL_EXTENSIONS);

	// split extensions
	if ( str ) {
		char seps[] = " ";
		char *next_token = NULL;

		tok = strtok_s(str, seps, &next_token);
		while ( tok ) {
			this->extensions.push_back(tok);									// put a extension into struct
			tok = strtok_s(NULL, seps, &next_token);							// next token
		}
	}

	// sort extension by alphabetical order
	std::sort(this->extensions.begin(), this->extensions.end());

    // get number of color bits
    glGetIntegerv(GL_RED_BITS, &this->redBits);
    glGetIntegerv(GL_GREEN_BITS, &this->greenBits);
    glGetIntegerv(GL_BLUE_BITS, &this->blueBits);
    glGetIntegerv(GL_ALPHA_BITS, &this->alphaBits);

    // get depth bits
    glGetIntegerv(GL_DEPTH_BITS, &this->depthBits);

    // get stecil bits
    glGetIntegerv(GL_STENCIL_BITS, &this->stencilBits);

    // get max number of lights allowed
    glGetIntegerv(GL_MAX_LIGHTS, &this->maxLights);

    // get max texture resolution
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &this->maxTextureSize);

    // get max number of clipping planes
    glGetIntegerv(GL_MAX_CLIP_PLANES, &this->maxClipPlanes);

    // get max attribute, modelview matrix and projection matrix stacks
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &this->maxAttribStacks);
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &this->maxModelViewStacks);
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &this->maxProjectionStacks);

    // get max texture stacks
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &this->maxTextureStacks);
}

void DriverInformation::setCompactDriverInfo(bool state)
{
	mCompactDriverInfo = state;
}
