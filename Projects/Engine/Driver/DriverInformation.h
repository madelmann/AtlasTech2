
#ifndef _Driver_DriverInformation_h_
#define _Driver_DriverInformation_h_


// Library includes
#include <cassert>
#include <string>
#include <vector>

// Project includes

// Forward declarations

// Namespace declarations


class DriverInformation
{
public:
	static DriverInformation& getInstance() {
		static DriverInformation oInstance;
		return oInstance;
	}

public:
	std::string getInfo();
    bool isSupported(const std::string& ext);
	void readInfo();
	void pop_AttributeStack();
	void pop_Matrix_ModelViewStack();
	void pop_Matrix_ProjectionStack();
	void pop_TextureStack();
	void push_AttributeStack();
	void push_Matrix_ModelViewStack();
	void push_Matrix_ProjectionStack();
	void push_TextureStack();
	void setCompactDriverInfo(bool state);

private:
	DriverInformation();
	DriverInformation( const DriverInformation& );
	DriverInformation& operator=( const DriverInformation& ) { return *this; }

private:
	std::string vendor;
    std::string renderer;
    std::string version;
    std::vector<std::string> extensions;
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    int stencilBits;
    int maxTextureSize;
    int maxLights;
    int maxAttribStacks;
    int maxModelViewStacks;
    int maxProjectionStacks;
    int maxClipPlanes;
    int maxTextureStacks;

private:
	bool	mCompactDriverInfo;
	int		mStack_Attribute;
	int		mStack_Matrix_ModelView;
	int		mStack_Matrix_Projection;
	int		mStack_Texture;
};


#endif
