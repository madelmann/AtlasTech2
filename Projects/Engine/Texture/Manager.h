
#ifndef _Textures_Manager_h_
#define _Textures_Manager_h_


// Library includes
#include <list>
#include <string>
#include <vector>

// Project includes
#include "Pack.h"
#include "Texture.h"
#include <Common/Logger.h>
#include <Core/EngineSettings.h>
#include <Interfaces/Texture/IManager.h>
#include <Media/PathManager.h>

// Forward declarations
namespace Core {
	class IdGenerator;
}

// Namespace declarations


namespace TextureSpace {

// Forward declarations
class BmpLoader;
class JPGLoader;
class PCXLoader;
class PngLoader;
class TgaLoader;

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	virtual ~Manager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	//}

	// IManager implementation
	//{
	void addPlugin(ATextureLoader *loader);
	Texture* create(const std::string& filename, const std::string& path = "");
	TexturePack* createPack(const std::string& filename, const std::string& path = "");
	Texture* find(const std::string& name);
	const std::string& getError() const;
	//}

	// Connectors
	//{
	void connectEngineSettings(Core::EngineSettings *s);
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(MediaPathManager *m);
	//}

	// Providers
	// {
	// }

	void addManual(Texture *texture);
	//TexturePack1D* createPack1D(const std::string& filename, const std::string& path = "");
	//TexturePack2D* createPack2D(const std::string& filename, const std::string& path = "");
	//TexturePack3D* createPack3D(const std::string& filename, const std::string& path = "");
	Texture* createRenderTexture(Texture *replace, int width, int height, int channels, int format);

protected:

private:
typedef std::list<Texture*> TextureList;
	typedef std::list<ATextureLoader*> TextureLoaderList;
	typedef std::list<TexturePack*> TexturePackList;

private:
	Texture* createDummyTexture();
	Texture* createFromPlugins(const std::string& filename);
	void remove(Texture *texture);

	// Texture packs
	// {
	TexturePack* findPack(const std::string& name);
	PackConfig loadPackConfig(const std::string& filename, const std::string& path);
	// }

	AEuint getNextId();
	bool glErrorCheck();

private:
	Texture*			mEmptyTexture;
	std::string			mError;
	Texture*			mFailureTexture;
	TextureLoaderList	mLoaders;
	GLuint				mTextureId;
	TextureList			mTextures;
	TexturePackList		mTexturePacks;

	Core::EngineSettings	*mEngineSettings;
	Core::IdGenerator		*mGenerator;
	MediaPathManager		*mMediaPathManager;

	// Plugins
	//{
	BmpLoader	*mBmpLoader;
	JPGLoader	*mJpgLoader;
	PCXLoader	*mPcxLoader;
	PngLoader	*mPngLoader;
	TgaLoader	*mTgaLoader;
	//}
};


}


#endif
