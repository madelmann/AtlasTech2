
#ifndef _UIKit_UIFrame_h_
#define _UIKit_UIFrame_h_


// Library includes
#include <list>

// Project includes
#include "UIBaseObject.h"
#include <Interfaces/Common/Event.h>

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declarations
using TextureSpace::Texture;


namespace UIKit {


class UIFrame : public UIBaseObject
{
public:
	typedef std::list<UIFrame*>	UIFrameList;

public:
	UIFrame();
	virtual ~UIFrame();

public:
	// Event handling
	// {
	virtual LRESULT handleEvent(const Common::Event& e);
	// }

	virtual void render();


	void add(UIFrame *frame);
	void remove(UIFrame *frame);

	void setBackground(Texture *background);

	float getHeight() const;
	void setHeight(float height);

	float getWidth() const;
	void setWidth(float width);

protected:
	Texture		*mBackground;
	float		mHeight;
	bool		mIsContainer;
	UIFrameList	mObjects;
	float		mWidth;

private:

};


}


#endif
