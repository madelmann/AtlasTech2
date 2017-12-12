
#ifndef Interfaces_Textures_IManager_h
#define Interfaces_Textures_IManager_h


// Library includes
#include <string>

// Project includes
#include <Interfaces/Common/AManager.h>

// Forward declarations

// Namespace declarations


namespace TextureSpace {

// Forward declarations
class ATextureLoader;
class Texture;
class TexturePack;
//class TexturePack1D;
//class TexturePack2D;
//class TexturePack3D;

class IManager : public Common::AManager
{
public:
	virtual ~IManager() { }

public:
	virtual void addPlugin(ATextureLoader *) = 0;
	virtual Texture* create(const std::string& , const std::string& path = "") = 0;
	virtual TexturePack* createPack(const std::string& , const std::string& path = "") = 0;
	//virtual TexturePack1D* createPack1D(const std::string& filename, const std::string& path = "") = 0;
	//virtual TexturePack2D* createPack2D(const std::string& filename, const std::string& path = "") = 0;
	//virtual TexturePack3D* createPack3D(const std::string& filename, const std::string& path = "") = 0;
	virtual Texture* find(const std::string& ) = 0;
	virtual const std::string& getError() const = 0;
};


}


#endif
