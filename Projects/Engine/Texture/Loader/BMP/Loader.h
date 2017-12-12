
#ifndef _Texture_Loader_BMP_Loader_h_
#define _Texture_Loader_BMP_Loader_h_


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Texture/ITextureLoader.h>

// Forward declarations

// Namespace decalarations


namespace TextureSpace {


class BmpLoader : public ATextureLoader,
				  private Common::Logger
{
public:
	BmpLoader(const Common::Logger *p);

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
