
#ifndef Menu_MenuItem_h
#define Menu_MenuItem_h


// Library includes

// Project includes
#include <Interfaces/Core/Id.h>
#include <Interfaces/Core/Name.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <UIKit/UIBaseObject.h>

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declarations
using TextureSpace::Texture;


namespace MenuSpace {


class MenuItem : public Core::Id,
				 public Core::Name,
				 public UIKit::UIBaseObject
{
public:
	MenuItem(const std::string& name);
	virtual ~MenuItem();

public:
	virtual void render();
	virtual void renderDebug();

public:
	void colorMap(Texture *colormap);

	float height() const;
	void height(float value);

	float width() const;
	void width(float value);

protected:

private:
	Texture	*mColorMap;
	float	mHeight;
	float	mWidth;
};


}


#endif
