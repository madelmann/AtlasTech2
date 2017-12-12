
#ifndef Mesh_Format_ParticleSystem_ParticleSystemManager_h
#define Mesh_Format_ParticleSystem_ParticleSystemManager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class IObjectManager;
class ParticleSystem;

class ParticleSystemManager : public Mesh::AMeshManager
{
public:
	ParticleSystemManager(const Common::Logger *p);
	~ParticleSystemManager();

public:
	// IManager implementation
	//{
	void clear();
	void configurationComplete();
	void update(float elapsedTime);
	//}

	// IMeshManager implementation
	//{
	Core::RenderObject* create(const std::string& filename);
	Core::RenderObject* find(const std::string& filename);
	//}

	void connectMeshManager(IObjectManager *m);

protected:

private:
	typedef std::list<ParticleSystem*> ParticleSystemList;

private:
	ParticleSystemList	mParticleSystems;

	IObjectManager	*mMeshManager;
};


}


#endif
