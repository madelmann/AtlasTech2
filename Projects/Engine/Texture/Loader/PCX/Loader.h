
#ifndef _Texture_Loader_PCX_Loader_h_
#define _Texture_Loader_PCX_Loader_h_


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Texture/ITextureLoader.h>

// Forward declarations

// Namespace decalarations


namespace TextureSpace {


class PCXLoader : public ATextureLoader,
				  private Common::Logger
{
public:
	PCXLoader(const Common::Logger *p);

public:
	// ATextureLoader implementation
	//{
	Texture* create(const std::string& filename);
	//}

protected:

private:

};


}


#endif
