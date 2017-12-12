
#ifndef Mesh_Format_Sprite_Sprite_h
#define Mesh_Format_Sprite_Sprite_h


// Library includes

// Project includes
#include <Math/Vector2.h>
#include <Mesh/AMesh.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


class Sprite : public AMesh
{
public:
	Sprite();
	virtual ~Sprite();

public:
	// Core::RenderObject implementation
	//{
	void init();
	void render();
	void update(float elapsedTime);
	//}

	// AMesh implementation
	// {
	void setAnimation(const std::string& animation);
	void setTextureCoordAnim(const vector2f& anim);
	void setTextureCoords(const vector2f& coords);
	// }

protected:

private:
	// Core::RenderObject implementation
	//{
	void unload();
	//}

	void renderWithoutDisplayList();

private:
	float		mLastAnimTime;
	aeuint		mMeshId;
	bool		mTextureAnimation;
	vector2f	mTextureCoords;
	vector2f	mTextureCoordAnim;
};


}


#endif
