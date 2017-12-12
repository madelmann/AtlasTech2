
#ifndef Arena_Editor_Editor_h
#define Arena_Editor_Editor_h


// Project includes
#include <Math/Vector2.h>

// Forward declarations
namespace Scene {
	class SceneGraphObject;
}


class Editor
{
public:
	typedef vector2<int> Position;

public:
	Editor();

public:
	void buildUnit();
	void init();

	void setMesh(Scene::SceneGraphObject *sgo) {
		mMesh = sgo;
	}

public:
	bool enabled() const {
		return mEnabled;
	}
	void enabled(bool state);

	const Position& position() const {
		return mPosition;
	}
	void position(const Position& position);

protected:

private:
	bool mEnabled;
	Scene::SceneGraphObject *mMesh;
	vector2f mOffset;
	Position mPosition;
	vector2f mRasterSize;
};


#endif
