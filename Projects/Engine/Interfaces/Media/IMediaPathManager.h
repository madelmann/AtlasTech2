
#ifndef _Interface_IMediaPathManager_h_
#define _Interface_IMediaPathManager_h_


// Library includes
#include <string>

// Project includes

// Forward declarations

// Namespace declarations


namespace Media {

}


class IMediaPathManager
{
public:
	virtual ~IMediaPathManager() { }

public:
	virtual std::string buildAudioPath(const std::string& filename) const = 0;
	virtual std::string buildConfigPath(const std::string& filename) const = 0;
	virtual std::string buildMaterialPath(const std::string& filename) const = 0;
	virtual std::string buildMeshPath(const std::string& filename) const = 0;
	virtual std::string buildObjectPath(const std::string& filename) const = 0;
	virtual std::string buildPath(const std::string& filename) const = 0;
	virtual std::string buildShaderPath(const std::string& filename) const = 0;
	virtual std::string buildScriptPath(const std::string& filename) const = 0;
	virtual std::string buildTerrainPath(const std::string& filename) const = 0;
	virtual std::string buildTexturePath(const std::string& filename) const = 0;

	virtual std::string getAudio() const = 0;
	virtual std::string getConfig() const = 0;
	virtual std::string getConsole() const = 0;
	virtual std::string getGame() const = 0;
	virtual std::string getMap() const = 0;
	virtual std::string getMaps() const = 0;
	virtual std::string getMaterials() const = 0;
	virtual std::string getMeshes() const = 0;
	virtual std::string getObjects() const = 0;
	virtual std::string getParticles() const = 0;
	virtual std::string getScene() const = 0;
	virtual std::string getShaders() const = 0;
	virtual std::string getScripts() const = 0;
	virtual std::string getSystem() const = 0;
	virtual std::string getTerrain() const = 0;
	virtual std::string getTextures() const = 0;

	virtual std::string getBasePath() = 0;
	virtual std::string getMapPath() = 0;
	virtual std::string getMeshPath(const std::string& mesh) = 0;
	virtual std::string getScenePath() = 0;
	virtual std::string getScriptPath() = 0;
	virtual std::string getTerrainPath() = 0;

	virtual void setGame(const std::string& game) = 0;
	virtual void setMap(const std::string& map) = 0;
	virtual void setTextures(const std::string& textures) = 0;
};


#endif
