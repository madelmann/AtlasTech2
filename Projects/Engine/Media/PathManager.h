
#ifndef _Media_MediaPathManager_h_
#define _Media_MediaPathManager_h_


// Library includes

// Poject includes
#include <Common/Logger.h>
#include <Interfaces/Media/IMediaPathManager.h>

// Forward declarations

// Namespace declarations


class MediaPathManager : public IMediaPathManager,
						 private Common::Logger
{
public:
	MediaPathManager(const Common::Logger* p, const std::string& gameName = "base");
	virtual ~MediaPathManager();

public:
	// IMediaPathManager implementation
	//{
	std::string buildAudioPath(const std::string& filename) const;
	std::string buildConfigPath(const std::string& filename) const;
	std::string buildMaterialPath(const std::string& filename) const;
	std::string buildMeshPath(const std::string& filename) const;
	std::string buildObjectPath(const std::string& filename) const;
	std::string buildPath(const std::string& filename) const;
	std::string buildShaderPath(const std::string& filename) const;
	std::string buildScriptPath(const std::string& filename) const;
	std::string buildTerrainPath(const std::string& filename) const;
	std::string buildTexturePath(const std::string& filename) const;

	std::string getAudio() const;
	std::string getConfig() const;
	std::string getConsole() const;
	std::string getGame() const;
	std::string getMap() const;
	std::string getMaps() const;
	std::string getMaterials() const;
	std::string getMeshes() const;
	std::string getObjects() const;
	std::string getParticles() const;
	std::string getScene() const;
	std::string getShaders() const;
	std::string getScripts() const;
	std::string getSystem() const;
	std::string getTerrain() const;
	std::string getTextures() const;

	std::string getBasePath();
	std::string getMapPath();
	std::string getMeshPath(const std::string& mesh);
	std::string getScenePath();
	std::string getScriptPath();
	std::string getTerrainPath();
	//}

	// Basic setup
	// {
	void setGame(const std::string& game);
	void setMap(const std::string& map);
	void setTextures(const std::string& textures);
	// }

protected:

private:
	std::string	mAudio;
	std::string	mConfig;
	std::string mConsole;
	std::string	mGame;
	std::string	mMap;
	std::string	mMaps;
	std::string	mMaterials;
	std::string	mMeshes;
	std::string	mObjects;
	std::string	mParticles;
	std::string	mScene;
	std::string	mShaders;
	std::string	mScripts;
	std::string	mSystem;
	std::string	mTerrain;
	std::string	mTextures;
};


#endif
