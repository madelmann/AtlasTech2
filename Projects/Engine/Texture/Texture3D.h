
#ifndef _Texture_Texture3D_h_
#define _Texture_Texture3D_h_


// Library includes

// Project includes
#include "BaseTexture.h"

// Forward declarations

// Namespace declarations


namespace TextureSpace {


class Texture3D : public ABaseTexture
{
public:
	Texture3D();
	~Texture3D();

	static void unbind();

	void uploadData(AEubyte *data);

	AEint getDepth() const;
	void setDepth(AEint depth);

	AEint getHeight() const;
	void setHeight(AEint height);

	AEint getWidth() const;
	void setWidth(AEint width);

protected:

private:
	// Copy operations are not allowed for textures, because
	// when the source texture is destroyed, it releases
	// its texture handle and so dest texture's handle is
	// not valid anymore.
	Texture3D (const Texture3D &);
	Texture3D &operator= (const Texture3D &);

	AEint	mDepth;
	AEuint	mWrapModeT;
};


}


#endif
