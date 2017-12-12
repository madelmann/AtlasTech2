
#ifndef Mesh_Format_PreparedObject_PreparedObject_h
#define Mesh_Format_PreparedObject_PreparedObject_h


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


class PreparedObject : public AMesh
{
public:
	class Leaf : public Core::Name
	{
	public:
		Leaf(const std::string& name)
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
		vector3f			mScale;
	};

public:
	PreparedObject(Core::AEid id = Core::invalidID, const std::string& name = "");
	~PreparedObject();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	//}

	void addLeaf(Leaf l);

protected:

private:
	typedef std::list<Leaf> Leaves;

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

	void addVolumeToSize(IVolume *v, const vector3f& position);

private:
	vector3f	mChangedSize;
	Leaves		mLeaves;
	bool		mResizeBoundingVolume;
};


}


#endif
