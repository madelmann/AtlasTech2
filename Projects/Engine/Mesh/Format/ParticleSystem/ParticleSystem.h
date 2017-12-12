
#ifndef Mesh_Format_ParticleSystem_ParticleSystem_h
#define Mesh_Format_ParticleSystem_ParticleSystem_h


// Library includes
#include <list>

// Project includes
#include <Mesh/AMesh.h>

// Forward declarations
namespace Core {
	class RenderObject;
}

// Namespace declarations


namespace Mesh {


class ParticleSystem : public AMesh
{
public:
	class Emitter : public Core::Name
	{
	public:
		Emitter(const std::string& name)
		: Name(name),
		  mEnabled(true),
		  mMesh(0)
		{ }

	public:
		bool enabled() const {
			return mEnabled;
		}
		void enabled(bool state) {
			mEnabled = state;
		}

		Core::RenderObject* mesh() const {
			return mMesh;
		}
		void mesh(Core::RenderObject *mesh) {
			mMesh = mesh;
		}

		const vector3f& position() const {
			return mPosition;
		}
		void position(const vector3f& position) {
			mPosition = position;
		}

		void render() {
			if ( !mEnabled || !mMesh ) {
				return;
			}

			mMesh->render();
		}

	private:
		bool				mEnabled;
		Core::RenderObject	*mMesh;
		vector3f			mPosition;
	};

public:
	ParticleSystem(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~ParticleSystem();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	//}

	void addEmitter(Emitter e);

protected:

private:
	typedef std::list<Emitter> EmitterList;

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

private:
	EmitterList	mEmitters;
	bool		mResizeBoundingVolume;
};


}


#endif
