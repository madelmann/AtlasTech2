
#ifndef Material_Material_h
#define Material_Material_h


// Library includes

// Project includes
#include <Appearence/AlphaAttributes.h>
#include <Appearence/BlendAttributes.h>
#include <Appearence/Environmental.h>
#include <Appearence/LightAttributes.h>
#include <Driver/GLWrapper.h>
#include <Interfaces/Appearence/IAttribute.h>
#include <Interfaces/Core/Object.h>
#include <Math/Vector4.h>

// Forward declarations
namespace TextureSpace {
	class Texture;
}

// Namespace declarations
using namespace TextureSpace;


#define aeBind( p ) \
	if ( p ) p->bind();
#define aeUnbind( p ) \
	if ( p ) p->unbind();


namespace MaterialSpace {


class Material : public Core::Object
				 //public IAttribute
{
public:
	Material(Core::AEid id, const std::string name);

public:
	// IAttribute implementation
	//{
	void bind();
	void unbind();
	//}

	AlphaAttributes& provideAlphaAttributes() {
		return mAlphaAttributes;
	}
	BlendAttributes& provideBlendAttributes() {
		return mBlendAttributes;
	}
	Environmental& provideEnvironmentalAttributes() {
		return mEnvironmentalAttributes;
	}
	LightAttributes& provideLightAttributes() {
		return mLightAttributes;
	}

	Texture* colorMap() const { return mColorMap; }
	void colorMap(Texture *colormap) { mColorMap = colormap; }

	vector4f emissive() const { return mEmissive; }
	void emissive(const vector4f& emissive) { mEmissive = emissive; }

	float shininess() const { return mShininess; }
	void shininess(float shininess) { mShininess = shininess; }

protected:

private:
	AlphaAttributes	mAlphaAttributes;
	BlendAttributes mBlendAttributes;
	Texture			*mColorMap;
	vector4f		mEmissive;
	Environmental	mEnvironmentalAttributes;
	LightAttributes mLightAttributes;
	float			mShininess;
};


}


#endif
