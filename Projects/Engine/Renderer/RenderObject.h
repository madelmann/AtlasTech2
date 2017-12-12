
#ifndef _Renderer_RenderObject_h_
#define _Renderer_RenderObject_h_


// Library includes
#include <string>

// Project includes
#include <Interfaces/Core/Object.h>
#include <Interfaces/Mesh/IVolume.h>
#include <Material/Material.h>
#include <Math/Vector3.h>
#include <Renderer/Common.h>
#include <Shader/Shader.h>
#include <Texture/Pack.h>
#include <Texture/Texture.h>

// Forward declarations
class IDriver;
class IOcclusionQuery;

// Namespace declarations


namespace Core {


class RenderObject : public Object
{
public:
	RenderObject(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~RenderObject();

public:
	virtual bool collide(vector3f vector, float radius);

	virtual bool boxCollision(vector3f vector, float radius, bool force = false);
	virtual bool cylinderCollision(vector3f vector, float radius, bool force = false);
	virtual int  deepCollision(vector3f vector, float radius, bool force = false);
	virtual bool sphereCollision(vector3f vector, float radius, bool force = false);

	virtual IOcclusionQuery* getOcclusionResult();
	virtual void render();
	virtual void renderBoundingVolume(bool solid = false);
	virtual void updateOcclusionQuery(IDriver *driver);


	virtual float animationSpeed() const;
	virtual void animationSpeed(float speed);

	IVolume* getBoundingVolume() const;
	void setBoundingVolume(IVolume *volume);

	const vector3f& getColor() const;
	void setColor(const vector3f& color);

	TextureSpace::Texture *getColorMap() const;
	void setColorMap(TextureSpace::Texture *colormap);

	Facing::E getFacing() const;
	void setFacing(Facing::E facing);

	TextureSpace::Texture* getHeightMap() const;
	void setHeightMap(TextureSpace::Texture *heightmap);

	MaterialSpace::Material *getMaterial() const;
	void setMaterial(MaterialSpace::Material *material);

	TextureSpace::Texture *getNormalMap() const;
	void setNormalMap(TextureSpace::Texture *normalmap);

	const vector3f getRotation();
	void setRotation(const vector3f& rot);

	const vector3f& getScale() const;
	void setScale(const vector3f& scale);

	const vector3f& getSize() const;
	void setSize(const vector3f& size);

	ShaderSpace::Shader *getShader() const;
	void setShader(ShaderSpace::Shader *shader);

	TextureSpace::TexturePack* getTexturePack() const;
	void setTexturePack(TextureSpace::TexturePack *pack);

	const vector3f& getTranslation() const;
	void setTranslation(const vector3f& trans);

	IVolume::Type::E getVolumeType() const;
	void setVolumeType(IVolume::Type::E type);

protected:
	float						mAnimationSpeed;
	IVolume						*mBoundingVolume;
	vector3f					mColor;
	TextureSpace::Texture		*mColorMap;
	Facing::E					mFacing;
	TextureSpace::Texture		*mHeightMap;
	MaterialSpace::Material		*mMaterial;
	TextureSpace::Texture		*mNormalMap;
	IOcclusionQuery				*mOcclusionQuery;
	vector3f					mRotation;
	vector3f					mScale;
	vector3f					mSize;
	ShaderSpace::Shader			*mShader;
	TextureSpace::TexturePack	*mTexturePack;
	vector3f					mTranslation;
	IVolume::Type::E			mVolumeType;

private:
	virtual void setBoundingVolume() { }
};


}

#endif
