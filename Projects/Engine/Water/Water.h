
#ifndef _Water_Water_h_
#define _Water_Water_h_


// Library includes

// Project include
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Renderer/RenderObject.h>

// Forward declarations
namespace CameraSpace {
	class IViewer;
}
namespace LightSpace {
	class Light;
}
namespace MaterialSpace {
	class IManager;
}
class IMediaPathManager;
class Material;
namespace ShaderSpace {
	class IManager;
}
namespace TextureSpace {
	class IManager;
	class Texture2D;
}

// Namespace declarations
using CameraSpace::IViewer;
using LightSpace::Light;
using TextureSpace::Texture2D;


namespace WaterSpace {

// Forward declarations
class Settings;

class Water : public Core::RenderObject
{
public:
	Water();
	~Water();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	void update(float elapsedTime);
	//}

	// Connectors
	//{
	void connectLight(Light *l);
	void connectMaterialManager(MaterialSpace::IManager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectSettings(Settings *s);
	void connectShaderManager(ShaderSpace::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	void connectViewer(IViewer *v);
	//}

	void setDepthMap(Texture2D *map);
	void setDuDvMap(Texture2D *map);
	void setRefractionMap(Texture2D *map);

	Texture2D* getReflectionMap();
	void setReflectionMap(Texture2D *map);

protected:

private:
	// Core::RenderObject implementation
	//{
	void unload();
	//}

	void processWave();

private:
	MaterialSpace::IManager	*mMaterialManager;
	IMediaPathManager		*mMediaPathManager;
	Settings				*mSettings;
	ShaderSpace::IManager	*mShaderManager;
	TextureSpace::IManager	*mTextureManager;

	Texture2D	*mDepthMap;
	Texture2D	*mDuDvMap;
	Light		*mLight;
	Texture2D	*mReflectionMap;
	Texture2D	*mRefractionMap;
	float		mTime;
	vector2f	mTexCoords;
	IViewer		*mViewer;
	bool		mVisible;
	vector2f	mWaterFlow;
	vector3f	mWavePosition;
};


}


#endif
