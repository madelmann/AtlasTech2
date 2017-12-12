
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "Shader.h"
#include <Core/EngineSettings.h>
#include <Driver/Driver.h>
#include <Driver/GLWrapper.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Tools/Files.h>

// Namespace declarations


namespace ShaderSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "ShaderSpace::Manager"),
  mEngineSettings(0),
  mMediaPathManager(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::clear()
{
	debug("Clearing...");

	for ( ShaderList::iterator it = mShaders.begin(); it != mShaders.end(); it++ ) {
		delete (*it);
	}

	mShaders.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mEngineSettings);
	assert(mMediaPathManager);

	mConfigurationComplete = true;
}

void Manager::connectEngineSettings(Core::EngineSettings *s)
{
	mEngineSettings = s;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

Shader* Manager::create(const std::string& filename, const std::string& path)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		return 0;
	}
	if ( !mEngineSettings->useShaders() ) {
		warn("shaders disabled");
		return 0;
	}

	Shader *shader = find(filename);
	if ( shader ) {
		return shader;
	}

	std::string file = path;
	if ( file.empty() ) {
		//buffer = mMediaPathManager->buildShaderPath(filename) + "/";
		file = mMediaPathManager->getGame()
			 + mMediaPathManager->getMaps()
			 + mMediaPathManager->getMap()
			 + mMediaPathManager->getShaders()
			 + filename + "/";
	}

	if ( !Tools::Files::exists(file + FragmentShader)
		//|| !Tools::File::exists(file + GeometryShader)
		|| !Tools::Files::exists(file + VertexShader)
		) {
		file = mMediaPathManager->getGame()
		     + mMediaPathManager->getShaders()
			 + filename + "/";

		if ( !Tools::Files::exists(file + FragmentShader) ) {
			error("Fragment shader '" + file + FragmentShader + "' not found!");
			return 0;
		}
		if ( !Tools::Files::exists(file + GeometryShader) ) {
			warn("Geometry shader '" + file + GeometryShader + "' not found!");
			//error("Geometry shader '" + file + GeometryShader + "' not found!");
			//return 0;
		}
		if ( !Tools::Files::exists(file + VertexShader) ) {
			error("Vertex shader '" + file + VertexShader + "' not found!");
			return 0;
		}
	}


	shader = new Shader(this, filename, file);

	mShaders.push_back(shader);

	return shader;
}

Shader* Manager::find(const std::string& shader)
{
	if ( shader.empty() ) {
		return 0;
	}

	for ( ShaderList::iterator it = mShaders.begin(); it != mShaders.end(); it++ ) {
		if ( (*it)->name() == shader ) {
			return (*it);
		}
	}

	return 0;
}

void Manager::init()
{
	debug("Initializing...");

	mIsReady = true;
}

void Manager::manage()
{
	// nothing to do here
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();
}

void Manager::update(float /*elapsedTime*/)
{
	// nothing to do here
}


}
