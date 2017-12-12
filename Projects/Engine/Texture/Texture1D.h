
#ifndef _Textures_Texture1D_h_
#define _Textures_Texture1D_h_


// Library includes

// Project includes
#include "BaseTexture.h"

// Forward declarations

// Namespace declarations


namespace TextureSpace {


class Texture1D : public ABaseTexture
{
public:
	Texture1D();
	virtual ~Texture1D();

	static void unbind();

	void uploadData(AEubyte *data);

	AEint getWidth() const;
	void setWidth(AEint width);

protected:

private:
	// Copy operations are not allowed for textures, because
	// when the source texture is destroyed, it releases
	// its texture handle and so dest texture's handle is
	// not valid anymore.
	Texture1D (const Texture1D &);
	Texture1D &operator= (const Texture1D &);
};


}


#endif
