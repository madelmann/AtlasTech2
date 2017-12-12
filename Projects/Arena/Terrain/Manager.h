
#ifndef WorldEditor_Terrain_Manager_h
#define WorldEditor_Terrain_Manager_h


// Library includes

// Project includes
#include <Interfaces/Terrain/IManager.h>

// Forward declarations
namespace TextureSpace {
	class IManager;
	class Texture;
}

// Namespace declarations


namespace ApplicationSpace {
namespace Terrain {


class Manager : public ::Terrain::IManager
{
public:
	Manager();
	~Manager();

public:
	// Common::AComponent implementation
	// {
	virtual void configurationComplete();
	virtual void shutdown();
	// }

	// Common::AManager implementation
	// {
	virtual void clear();
	virtual void init();
	virtual void manage();
	virtual void update(float /*elapsedTime*/);
	// }

	// Terrain::IManager implementation
	// {
	vector3f heightAt(const vector3f& pos);
	vector3f normalAt(const vector3f& pos);

	void render();
	// }

	void connectTextureManager(TextureSpace::IManager *m);

protected:

private:
	TextureSpace::Texture *mColorMap;
	TextureSpace::IManager *mTextureManager;
};


}
}


#endif
