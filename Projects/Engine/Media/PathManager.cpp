
// Header file
#include "PathManager.h"

// Library includes
#include <io.h>

// Project includes
#include <Tools/Tools.h>

// Namespace declarations


MediaPathManager::MediaPathManager(const Common::Logger* p, const std::string& game)
: Common::Logger(p, "MediaPathManager"),
  mAudio("audio"),
  mConfig("config"),
  mConsole("console"),
  mGame(game),
  mMaps("maps"),
  mMaterials("materials"),
  mMeshes("meshes"),
  mObjects("objects"),
  mParticles("particles"),
  mScene("scene"),
  mShaders("shaders"),
  mScripts("scripts"),
  mSystem("system"),
  mTerrain("terrain"),
  mTextures("textures")
{
	debug("Starting...");
}

MediaPathManager::~MediaPathManager()
{
}

std::string MediaPathManager::buildAudioPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into maps folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getAudio()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getAudio()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildConfigPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into maps folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getConfig()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getConfig()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildMaterialPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into maps folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getMaterials()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getMaterials()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildMeshPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into maps folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getMeshes()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getMeshes()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildObjectPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into objects folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getObjects()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getObjects()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into base folder
	file = getGame()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		return "";
	}

	return file;
}

std::string MediaPathManager::buildShaderPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into shaders folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getShaders()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getShaders()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildScriptPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into scripts folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getScripts()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getScripts()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildTerrainPath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into terrain folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getTerrain()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getTerrain()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::buildTexturePath(const std::string& filename) const
{
	std::string file;

	// build complete path pointing into textures folder
	file = getGame()
		 + getMaps()
		 + getMap()
		 + getTextures()
		 + filename;

	if ( !Tools::Files::exists(file) ) {
		file = getGame()
			 + getTextures()
			 + filename;

		if ( !Tools::Files::exists(file) ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::getAudio() const
{
	return mAudio + "/";
}

std::string MediaPathManager::getBasePath()
{
	return mGame + "/";
}

std::string MediaPathManager::getConfig() const
{
	return mConfig + "/";
}

std::string MediaPathManager::getConsole() const
{
	return mConsole + "/";
}

std::string MediaPathManager::getGame() const
{
	return mGame + "/";
}

std::string MediaPathManager::getMap() const
{
	return mMap + "/";
}

std::string MediaPathManager::getMapPath()
{
	std::string file;
	file = getBasePath()
		 + getMaps()
		 + getMap();

	return file;
}

std::string MediaPathManager::getMaps() const
{
	return mMaps + "/";
}

std::string MediaPathManager::getMaterials() const
{
	return mMaterials + "/";
}

std::string MediaPathManager::getMeshPath(const std::string& mesh)
{
	std::string file;
	file = getMapPath()
		 + getMeshes()
		 + mesh + "/";

	if ( access(file.c_str(), 0) == -1 ) {
		file = getBasePath()
			 + getMeshes()
			 + mesh + "/";

		if ( access(file.c_str(), 0) == -1 ) {
			return "";
		}
	}

	return file;
}

std::string MediaPathManager::getScene() const
{
	return mScene + "/";
}

std::string MediaPathManager::getScenePath()
{
	std::string file;
	file = getMapPath()
		 + getScene();

	if ( access(file.c_str(), 0) == -1 ) {
		return "";
	}

	return file;
}

std::string MediaPathManager::getScriptPath()
{
	std::string file;
	file = getScriptPath()
		 + getScripts();

	if ( access(file.c_str(), 0) == -1 ) {
		return "";
	}

	return file;
}

std::string MediaPathManager::getTerrainPath()
{
	std::string file;
	file = getMapPath()
		 + getTerrain();

	if ( access(file.c_str(), 0) == -1 ) {
		return "";
	}

	return file;
}

std::string MediaPathManager::getMeshes() const
{
	return mMeshes + "/";
}

std::string MediaPathManager::getObjects() const
{
	return mObjects + "/";
}

std::string MediaPathManager::getParticles() const
{
	return mParticles + "/";
}

std::string MediaPathManager::getShaders() const
{
	return mShaders + "/";
}

std::string MediaPathManager::getScripts() const
{
	return mScripts + "/";
}

std::string MediaPathManager::getSystem() const
{
	return mSystem + "/";
}

std::string MediaPathManager::getTerrain() const
{
	return mTerrain + "/";
}

std::string MediaPathManager::getTextures() const
{
	return mTextures + "/";
}

void MediaPathManager::setGame(const std::string& game)
{
	mGame = game;
}

void MediaPathManager::setMap(const std::string& map)
{
	mMap = map;
}

void MediaPathManager::setTextures(const std::string& textures)
{
assert(!"is this in use?");
	mTextures = textures;
}
