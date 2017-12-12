
// Header
#include "Manager.h"

// Library includes

// Project includes
#include <Core/Configuration/Configurator.h>
#include <Core/IdGenerator.h>
#include <Driver/GLWrapper.h>
#include <Interfaces/Texture/ITextureLoader.h>
#include <Parsers/ConfigParser.h>
#include <Texture/Loader/BMP/Loader.h>
#include <Texture/Loader/JPG/Loader.h>
#include <Texture/Loader/PCX/Loader.h>
#include <Texture/Loader/PNG/Loader.h>
#include <Texture/Loader/TGA/Loader.h>
#include <Tools/Files.h>
#include <Tools/Strings.h>

// Namespace declarations


namespace TextureSpace {


static const std::string FailureTexture = "TextureFailed.tga";


Manager::Manager(const Common::Logger *p)
: Common::Logger(p, "TextureSpace::Manager"),
  mEngineSettings(0),
  mBmpLoader(0),
  mEmptyTexture(0),
  mFailureTexture(0),
  mGenerator(0),
  mJpgLoader(0),
  mMediaPathManager(0),
  mPcxLoader(0),
  mPngLoader(0),
  mTextureId(0),
  mTgaLoader(0)
{
	debug("Starting...");
}

Manager::~Manager()
{
	shutdown();
}

void Manager::addManual(Texture *texture)
{
	if ( !texture ) {
		return;
	}

	mTextures.push_back(texture);
}

void Manager::addPlugin(ATextureLoader *loader)
{
	mLoaders.push_back(loader);
}

void Manager::clear()
{
	debug("Clearing...");

	for ( TexturePackList::iterator it = mTexturePacks.begin(); it != mTexturePacks.end(); ++it ) {
		delete (*it);
	}
	mTexturePacks.clear();

	for ( TextureList::iterator it = mTextures.begin(); it != mTextures.end(); ++it ) {
		if ( !(*it) ) {
			continue;
		}

		glDeleteTextures(1, &(*it)->mDriverId);
		delete (*it);
	}
	mTextures.clear();
}

void Manager::configurationComplete()
{
	debug("configurationComplete()");

	assert(!mConfigurationComplete);

	assert(mEngineSettings);
	assert(mGenerator);
	assert(mMediaPathManager);

	mBmpLoader = new BmpLoader(this);
	mBmpLoader->connectEngineSettings(mEngineSettings);
	addPlugin(mBmpLoader);

	mJpgLoader = new JPGLoader(this);
	mJpgLoader->connectEngineSettings(mEngineSettings);
	addPlugin(mJpgLoader);

	mPcxLoader = new PCXLoader(this);
	mPcxLoader->connectEngineSettings(mEngineSettings);
	addPlugin(mPcxLoader);

	mPngLoader = new PngLoader(this);
	mPngLoader->connectEngineSettings(mEngineSettings);
	addPlugin(mPngLoader);

	mTgaLoader = new TgaLoader(this);
	mTgaLoader->connectEngineSettings(mEngineSettings);
	addPlugin(mTgaLoader);

	mConfigurationComplete = true;

	mEmptyTexture = new Texture();
	mFailureTexture = create(FailureTexture);
}

void Manager::connectEngineSettings(Core::EngineSettings *s)
{
	mEngineSettings = s;
}

void Manager::connectIdGenerator(Core::IdGenerator *g)
{
	mGenerator = g;
}

void Manager::connectMediaPathManager(MediaPathManager *m)
{
	mMediaPathManager = m;
}

Texture* Manager::create(const std::string& filename, const std::string& path)
{
	debug("create('" + filename + "')");

	assert(mConfigurationComplete);

	if ( filename.empty() ) {
		return mEmptyTexture;
	}
	if ( !mEngineSettings->useTextures() ) {
		return createDummyTexture();
	}

	Texture *texture = find(filename);
	if ( texture ) {
		return texture;
	}

	std::string file = path + Tools::Files::ExtractFilename(filename);
	if ( !Tools::Files::exists(file) ) {
		file = mMediaPathManager->buildTexturePath(filename);

		if ( !Tools::Files::exists(file) ) {
			warn("file '" + filename + "' not found!");
			return mFailureTexture;
		}
	}


	std::string ext = Tools::Files::ExtractFileExt(file);

 	glErrorCheck();

	try {
		texture = createFromPlugins(file);
	}
	catch ( std::exception &e ) {
		error("Exception: " + std::string(e.what()));
	}

	if ( !texture ) {
		error("could not load '" + filename + "', using failure texture instead!");
		return mFailureTexture;
	}

	glErrorCheck();

	Core::AEid id = mGenerator->generate();
	texture->setId(id);

	return texture;
}

Texture* Manager::createDummyTexture()
{
	Texture *texture = new Texture();

	Core::AEid id = mGenerator->generate();
	texture->setId(id);

	mTextures.push_back(texture);

	return texture;
}

Texture* Manager::createFromPlugins(const std::string& filename)
{
	debug("createFromPlugins('" + filename + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	std::string ext = Tools::Files::ExtractFileExt(filename);
	if ( ext.empty() ) {
		warn("invalid file extension found!");
		return 0;
	}

	Texture *texture = 0;

	for ( TextureLoaderList::iterator it = mLoaders.begin(); it != mLoaders.end(); it++ ) {
		if ( (*it)->handleFormat(ext) ) {
			texture = (*it)->create(filename);
			if ( texture ) {
				mTextures.push_back(texture);
				return texture;
			}
		}
	}

	if ( !texture ) {
		warn("there was a problem loading texture '" + filename + "'");
	}

	return texture;
}

TexturePack* Manager::createPack(const std::string& filename, const std::string& path)
{
	debug("createPack('" + filename + "', '" + path + "')");

	if ( filename.empty() ) {
		warn("invalid filename provided!");
		return 0;
	}

	TexturePack *pack = findPack(filename);
	if ( pack ) {
		return pack;
	}

	PackConfig config = loadPackConfig(filename, path);
	if ( config.mName.empty() ) {
		warn("could not load '" + filename + "' pack!");
		return 0;
	}

	Texture *color = create("packs/" + config.mName + "/" + config.mColor, path);
	Texture *height = create("packs/" + config.mName + "/" + config.mHeight, path);
	Texture *normal = create("packs/" + config.mName + "/" + config.mNormal, path);

	pack = new TexturePack(config, color, height, normal);
	mTexturePacks.push_back(pack);

	return pack;
}

//TexturePack1D Manager::createPack1D(const std::string& filename, const std::string& path)
//{
//	debug("createPack1D('" + filename + "', '" + path + "')");
//
//	if ( filename.empty() ) {
//		warn("invalid filename provided!");
//		return TexturePack1D();
//	}
//
//	PackConfig config = loadPackConfig(filename, path);
//
//	Texture1D *color = create("packs/" + config.mName + "/" + config.mColor, path);
//	Texture1D *height = create("packs/" + config.mName + "/" + config.mHeight, path);
//	Texture1D *normal = create("packs/" + config.mName + "/" + config.mNormal, path);
//
//	return TexturePack1D(config, color, height, normal);
//}

//TexturePack2D* Manager::createPack2D(const std::string& filename, const std::string& path)
//{
//	debug("createPack2D('" + filename + "', '" + path + "')");
//
//	if ( filename.empty() ) {
//		warn("invalid filename provided!");
//		return 0;
//	}
//
//	TexturePack2D *pack = findPack(filename);
//	if ( pack ) {
//		return pack;
//	}
//
//	PackConfig config = loadPackConfig(filename, path);
//	if ( config.mName.empty() ) {
//		warn("could not load '" + filename + "' pack configuration!");
//		return 0;
//	}
//
//	Texture *color = create("packs/" + config.mName + "/" + config.mColor, path);
//	Texture *height = create("packs/" + config.mName + "/" + config.mHeight, path);
//	Texture *normal = create("packs/" + config.mName + "/" + config.mNormal, path);
//
//	pack = new TexturePack2D(config, color, height, normal);
//	mTexturePacks.push_back(pack);
//
//	return pack;
//}

//TexturePack3D Manager::createPack3D(const std::string& filename, const std::string& path)
//{
//	debug("createPack3D('" + filename + "', '" + path + "')");
//
//	if ( filename.empty() ) {
//		warn("invalid filename provided!");
//		return TexturePack3D();
//	}
//
//	PackConfig config = loadPackConfig(filename, path);
//
//	Texture3D *color = create("packs/" + config.mName + "/" + config.mColor, path);
//	Texture3D *height = create("packs/" + config.mName + "/" + config.mHeight, path);
//	Texture3D *normal = create("packs/" + config.mName + "/" + config.mNormal, path);
//
//	return TexturePack3D(config, color, height, normal);
//}

Texture* Manager::createRenderTexture(Texture *replace, int width, int height, int channels, int format)
{
	Texture *result = new Texture();
	result->setBPP(channels * 8);
	result->mDataFormat = format;
	result->setHeight(height);
	result->mInternalFormat = channels;
	result->setWidth(width);

	if ( replace ) {
		result->name(replace->name());
	}

	// Create a pointer to store the blank image data
	unsigned int *pTexture = 0;

	// Allocate and init memory for the image array and point to it from pTexture
	pTexture = new unsigned int [width * height * channels];
	memset(pTexture, 0, width * height * channels * sizeof(unsigned int));

	// Register the texture with OpenGL and bind it to the texture ID
	glGenTextures(1, &result->mDriverId);
	result->bind();

	// Create the texture and store it on the video card
	glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, format, GL_UNSIGNED_BYTE, pTexture);

	// Set the texture quality
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Since we stored the texture space with OpenGL, we can delete the image data
	delete [] pTexture;

	mTextures.push_back(result);
	remove(replace);

	return result;
}

Texture* Manager::find(const std::string& name)
{
	std::string textureName = Tools::Files::ExtractFilename(name);

	for ( TextureList::iterator it = mTextures.begin(); it != mTextures.end(); it++ ) {
		if ( (*it)->name() == textureName ) {
			return (*it);
		}
	}

	return 0;
}

TexturePack* Manager::findPack(const std::string& name)
{
	if ( name.empty() ) {
		return 0;
	}

	for ( TexturePackList::iterator it = mTexturePacks.begin(); it != mTexturePacks.end(); ++it ) {
		if ( (*it)->name() == name ) {
			return (*it);
		}
	}

	return 0;
}

const std::string& Manager::getError() const
{
	return mError;
}

AEuint Manager::getNextId()
{
	return ++mTextureId;
}

bool Manager::glErrorCheck()
{
	GLenum error;
	char buffer[512] = "";

	if ( (error = glGetError()) != GL_NO_ERROR ) {
		sprintf(buffer, "%s", gluErrorString(error));
		mError = buffer;
		error("Driver failed with: " + mError);
		return true;
	}

	return false;
}

void Manager::init()
{
	debug("Initializing...");

	// select modulate to mix texture with color for shading
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	mIsReady = true;
}

PackConfig Manager::loadPackConfig(const std::string& filename, const std::string& path)
{
	debug("loadPackConfig('" + filename + "', '" + path + "')");

	std::string file = path + "packs/" + Tools::Files::ExtractFilename(filename) + "/pack.cfg";
	if ( !Tools::Files::exists(file) ) {
		file = mMediaPathManager->buildTexturePath("packs/" + filename + "/pack.cfg");
	}

	PackConfig config;

	Core::Configuration::Configurator parser(this, file);
	if ( !parser.success() ) {
		warn("could not load '" + filename + "' pack configuration!");
		return config;
	}

	config.mAnimated = parser.getValue("animated").toBool();
	config.mColor = parser.getValue("color").toString();
	config.mFormat = parser.getValue("format").toString();
	config.mHeight = parser.getValue("height").toString();
	config.mName = filename;
	config.mNormal = parser.getValue("normal").toString();
	config.mSize = parser.getValue("size").toVector2f();

	return config;
}

void Manager::manage()
{
	// nothing to do
}

void Manager::remove(Texture *texture)
{
	if ( !texture ) {
		return;
	}

	bool contains = false;

	for ( TextureList::iterator it = mTextures.begin(); it != mTextures.end(); it++ ) {
		if ( (*it) == texture ) {
			contains = true;
			break;
		}
	}

	// Delete OpenGL data and remove from list
	if ( contains ) {
		glDeleteTextures(1, &texture->mDriverId);

		mTextures.remove(texture);
	}

	// Delete texture object
	delete texture;
}

void Manager::shutdown()
{
	debug("Stopping...");

	clear();

	delete mEmptyTexture;

	for ( TextureLoaderList::iterator it = mLoaders.begin(); it != mLoaders.end(); ++it ) {
		delete (*it);
	}
	mLoaders.clear();
}

void Manager::update(float /*elapsedTime*/)
{
	// nothing to do
}


}
