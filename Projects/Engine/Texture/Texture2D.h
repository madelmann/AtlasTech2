
#ifndef _Textures_Texture2D_h_
#define _Textures_Texture2D_h_


// Library includes

// Project includes
#include "BaseTexture.h"
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


namespace TextureSpace {


class Texture2D : public ABaseTexture
{
public:
	Texture2D();
	virtual ~Texture2D();

	static void unbind();

	void uploadData(AEubyte *data);

	AEint getHeight() const;
	void setHeight(int AEheight);

	AEint getWidth() const;
	void setWidth(int AEwidth);

protected:

private:
	// Copy operations are not allowed for textures, because
	// when the source texture is destroyed, it releases
	// its texture handle and so dest texture's handle is
	// not valid anymore.
	Texture2D (const Texture2D &);
	Texture2D &operator= (const Texture2D &);
};


}


#endif
