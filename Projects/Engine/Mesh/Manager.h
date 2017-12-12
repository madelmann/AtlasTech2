
#ifndef Mesh_Manager_h
#define Mesh_Manager_h


// Library includes

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Mesh/IObjectManager.h>

// Forward declarations
namespace MaterialSpace {
	class IManager;
}
class IMediaPathManager;
namespace Core {
	class IdGenerator;
	class RenderObject;
}
namespace ShaderSpace {
	class IManager;
}
namespace TextureSpace {
	class IManager;
}

// Namespace declarations


namespace Mesh {


class ObjectManager : public IObjectManager,
					  private Common::Logger
{
public:
	ObjectManager(const Common::Logger *p);
	~ObjectManager();

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

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& name);
	Core::RenderObject* find(const std::string& filename);
	bool handleFormat(const std::string& format) const;
	//}

	// IObjectManager implementation
	//{
	void addPlugin(IMeshManager *m);
	//}

	// Connectors
	//{
	void connectIdGenerator(Core::IdGenerator *g);
	void connectMaterialManager(MaterialSpace::IManager *m);
	void connectMediaPathManager(IMediaPathManager *m);
	void connectShaderManager(ShaderSpace::IManager *m);
	void connectTextureManager(TextureSpace::IManager *m);
	//}

protected:

private:
	Core::RenderObject* createFromPlugin(const std::string& filename, const std::string& extension);

private:
	typedef std::list<Mesh::IMeshManager*> MeshManagerList;
	typedef std::list<Core::RenderObject*> RenderObjectList;

private:
	std::string			mFormat;
	MeshManagerList		mManagers;
	RenderObjectList	mObjects;

	Core::IdGenerator		*mIdGenerator;
	MaterialSpace::IManager	*mMaterialManager;
	IMediaPathManager		*mMediaPathManager;
	ShaderSpace::IManager	*mShaderManager;
	TextureSpace::IManager	*mTextureManager;
};


}


#endif
