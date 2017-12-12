
#ifndef _Terrain_ALayer_h_
#define _Terrain_ALayer_h_


// Library includes
#include <string>
#include <vector>

// Project includes
#include "Settings.h"
#include <Core/EngineSettings.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Renderer/Common.h>
#include <Renderer/RenderObject.h>

// Forward declarations
class IMediaPathManager;
namespace Scene {
	class IManager;
}
namespace TextureSpace {
	class Texture;
}

// Namespace declarations
using TextureSpace::Texture;


namespace Terrain {


class ALayer : public Core::RenderObject
{
public:
	class TextureSettings
	{
	public:
		Texture *ColorMap;
		bool Enabled;
		vector2f Scale;
	};

public:
	ALayer();
	virtual ~ALayer();

public:
	virtual bool build() = 0;
	virtual void clear() = 0;

	virtual vector3f heightAt(const vector3f& pos) = 0;
	virtual vector3f normalAt(const vector3f& pos) = 0;

public:
	// Connectors
	// {
	void connectEngineSettings(Core::EngineSettings *s);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectSceneManager(Scene::IManager *m);
	void connectSettings(Settings *s);
	// }

	bool enabled() const;
	void enabled(bool state);

	int height() const;
	int width() const;

	void addTexture(const TextureSettings &texture);
	void setAdditionalTexture(Texture *additional);
	void setAlphaTexture(Texture *alpha);
	void setBaseTexture(Texture *base);
	void setHeightMap(const std::string &heightmap);
	void setOffset(float offset);
	void setSubdivisions(const vector2f& sub);

protected:
	typedef std::vector<TextureSettings> TextureVector;

protected:
	Core::EngineSettings	*mEngineSettings;
	IMediaPathManager		*mMediaPathManager;
	Scene::IManager			*mSceneManager;
	Settings				*mSettings;

protected:
	Texture			*mAdditionalTexture;
	Texture			*mAlphaTexture;
	Texture			*mBaseTexture;
	bool			mEnabled;
	std::string		mHeightMap;
	vector2i		mMapSize;
	float			mOffset;
	bool			mRenderNormals;
	vector2f		mSubdivisions;
	TextureVector	mTextures;

private:

};


}


#endif
