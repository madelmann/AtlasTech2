
#ifndef _Texture_Loader_TGALoader_h_
#define _Texture_Loader_TGALoader_h_


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Texture/ITextureLoader.h>

// Forward declarations

// Namespace decalarations


namespace TextureSpace {


class TgaLoader : public ATextureLoader,
				  private Common::Logger
{
public:
	TgaLoader(const Common::Logger *p);

public:
	// ATextureLoader implementation
	//{
	Texture* create(const std::string &filename);
	//}

protected:

private:

};


}


#endif
