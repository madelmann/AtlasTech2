
#ifndef _Sky_SkyBox_h_
#define _Sky_SkyBox_h_


// Library includes
#include <string>

// Project includes
#include "ASky.h"

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declartations
using TextureSpace::Texture;


namespace SkySpace {


class SkyBox : public ASky
{
public:
	SkyBox(Core::AEid id);
	~SkyBox();

public:
	// Core::RenderObject implementation
	//{
	void render();
	void update(float elapsedTime);
	//}

	void setBack(Texture *t) { mBack = t; }
	void setBottom(Texture *t) { mBottom = t; }
	void setFront(Texture *t) { mFront = t; }
	void setLeft(Texture *t) { mLeft = t; }
	void setRight(Texture *t) { mRight = t; }
	void setTop(Texture *t) { mTop = t; }

protected:

private:
	// Core::RenderObject implementation
	//{
	void unload();
	//}

	// ASky implementation
	// {
	void preprocess();
	// }

private:
	Texture	*mBack;
	Texture	*mBottom;
	Texture	*mFront;
	Texture	*mLeft;
	Texture	*mRight;
	Texture	*mTop;
};


}


#endif
