
#ifndef _Texture_Loader_JPG_Loader_h_
#define _Texture_Loader_JPG_Loader_h_


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Texture/ITextureLoader.h>

// Forward declarations
struct jpeg_decompress_struct;
struct tImage;

// Namespace decalarations


namespace TextureSpace {


class JPGLoader : public ATextureLoader,
				  private Common::Logger
{
public:
	JPGLoader(const Common::Logger *p);

public:
	// ATextureLoader implementation
	//{
	Texture* create(const std::string &filename);
	//}

protected:

private:
	void decode(jpeg_decompress_struct* cinfo, tImage *pImageData);
};


}


#endif
