
// Header
#include "Manager.h"

// Library includes

// Project includes
#include "Material.h"
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Interfaces/Media/IMediaPathManager.h>
#include <Interfaces/Texture/IManager.h>
#include <Tools/Files.h>
#include <Tools/Strings.h>

// Namespace declarations


namespace MaterialSpace {


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "MaterialSpace::Manager"),
  mIdGenerator(0),
  mMediaPathManager(0),
  mTextureManager(0)
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

	for ( MaterialList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++ ) {
		delete (*it);
		(*it) = 0;				// in case this pointer is used anywhere else
	}

	mMaterials.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mMediaPathManager);
	assert(mTextureManager);

	mConfigurationComplete = true;
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mIdGenerator = g;
}

void Manager::connectMediaPathManager(IMediaPathManager *m)
{
	mMediaPathManager = m;
}

void Manager::connectTextureManager(TextureSpace::IManager *m)
{
	mTextureManager = m;
}

Material* Manager::create(const std::string& filename, const std::string& path)
{
	debug("create('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	std::string file = path + Tools::Files::ExtractFilename(filename);
	if ( !Tools::Files::exists(file) ) {
		file = mMediaPathManager->buildMaterialPath(filename);
	}
	file = Tools::toLowerCase(file);

	Material *material = find(file);
	if ( material ) {
		debug("getting '" + file + "' from repository");
		return material;
	}

	material = loadFromFile(file);
	if ( !material ) {
		error("error while loading material from file '" + file + "'!");
		return 0;
	}

	mMaterials.push_back(material);

	return material;
}

Material* Manager::find(const std::string& material)
{
	if ( material.empty() ) {
		return 0;
	}

	for ( MaterialList::iterator it = mMaterials.begin(); it != mMaterials.end(); it++ ) {
		if ( Tools::StringCompare((*it)->name(), material) ) {
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

Material* Manager::loadFromFile(const std::string& filename)
{
	Core::Configuration::Configurator config(this, filename);
	if ( !config.success() ) {
		return 0;
	}

	Material *m = new Material(mIdGenerator->generate(), config.getValue("name").toString());

	m->colorMap(mTextureManager->create(config.getValue("texture").toString()));
	m->provideAlphaAttributes().enabled(config.getValue("alpha.enabled").toBool());
	//m->provideAlphaAttributes().function(AlphaAttributes::Function::convert(config.getValue("alpha.function").toInt()));
	m->provideAlphaAttributes().function(AlphaAttributes::Function::convert(config.getValue("alpha.function").toString()));
	m->provideAlphaAttributes().reference(config.getValue("alpha.reference").toFloat());
	//m->provideBlendAttributes().destination(BlendAttributes::BlendingFactorDestination::convert(config.getValue("blending.destination").toInt()));
	m->provideBlendAttributes().destination(BlendAttributes::BlendingFactorDestination::convert(config.getValue("blending.destination").toString()));
	m->provideBlendAttributes().enabled(config.getValue("blending.enabled").toBool());
	//m->provideBlendAttributes().source(BlendAttributes::BlendingFactorSource::convert(config.getValue("blending.source").toInt()));
	m->provideBlendAttributes().source(BlendAttributes::BlendingFactorSource::convert(config.getValue("blending.source").toString()));
	m->provideEnvironmentalAttributes().enabled(config.getValue("environmental.enabled").toBool());
	m->provideLightAttributes().ambient(config.getValue("light.ambient").toVector4f());
	m->provideLightAttributes().diffuse(config.getValue("light.diffuse").toVector4f());
	m->provideLightAttributes().drawbuffermode(config.getValue("light.drawbuffermode").toInt());
	m->provideLightAttributes().enabled(config.getValue("light.enabled").toBool());
	m->provideLightAttributes().specular(config.getValue("light.specular").toVector4f());
	m->emissive(config.getValue("light.specular").toVector4f());
	m->shininess(config.getValue("shininess").toFloat());

	return m;
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
	// nothing to do
}


}
