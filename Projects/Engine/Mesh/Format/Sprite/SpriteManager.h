
#ifndef Mesh_Format_Sprite_SpriteManager_h
#define Mesh_Format_Sprite_SpriteManager_h


// Library includes

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class Sprite;

class SpriteManager : public AMeshManager
{
public:
	SpriteManager(const Common::Logger *p);
	~SpriteManager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	//}

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& filename);
	Core::RenderObject* find(const std::string& filename);
	//}

protected:

private:
	typedef std::list<Sprite*> SpriteList;

private:
	SpriteList	mMeshes;
};


}


#endif
