
#ifndef _Material_MaterialManager_h_
#define _Material_MaterialManager_h_


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Material/IManager.h>

// Forward declarations
namespace Core {
	class IdGenerator;
}
class IMediaPathManager;
namespace TextureSpace {
	class IManager;
}

// Namespace declarations


namespace MaterialSpace {


class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

public:
	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	//}

	// IManager implementation
	// {
	Material* create(const std::string& filename, const std::string& path = "");
	Material* find(const std::string& material);
	// }

	// Connectors
	//{
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

	// Providers
	// {
	// }

protected:

private:
	typedef std::list<Material*> MaterialList;

private:
	Material* loadFromFile(const std::string& filename);

private:
	MaterialList	mMaterials;

	Core::IdGenerator		*mIdGenerator;
	IMediaPathManager		*mMediaPathManager;
	TextureSpace::IManager	*mTextureManager;
};


}


#endif
