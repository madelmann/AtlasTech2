
#ifndef Mesh_Format_Particle_ParticleManager_h
#define Mesh_Format_Particle_ParticleManager_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMeshManager.h>

// Forward declarations

// Namespace declarations


namespace Mesh {

// Forward declarations
class IObjectManager;
class Particle;

class ParticleManager : public AMeshManager
{
public:
	ParticleManager(const Common::Logger *p);
	~ParticleManager();

public:
	// Common::AManager implementation
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
	typedef std::list<Particle*> ParticleList;

private:
	ParticleList	mParticles;

	IObjectManager	*mMeshManager;
};


}


#endif
