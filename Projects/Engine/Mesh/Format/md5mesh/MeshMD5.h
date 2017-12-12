
#ifndef Mesh_Format_MD5_MeshMD5_h
#define Mesh_Format_MD5_MeshMD5_h


// Library incudes

// Project includes
#include <Mesh/AMesh.h>

// Forward declarations
class MD5Model;

// Namespace declarations


namespace Mesh {


class MeshMD5 : public AMesh
{
public:
	MeshMD5();
	virtual ~MeshMD5();

public:
	// Core::RenderObject implentation
	//{
	void init();
	void render();
	void update(float elapsedTime);
	//}

	// AMesh implementation
	// {
	void setAnimation(const std::string& animation);
	// }

	void setModel(MD5Model *model);

protected:

private:
	// Core::RenderObject implementation
	//{
	void setBoundingVolume();
	void unload();
	//}

	void renderWithoutDisplayList();
	//float returnCurrentTime(int nextFrame);

private:
	float		mLastAnimTime;
	MD5Model	*mModel;
};


}


#endif
