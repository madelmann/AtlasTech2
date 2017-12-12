
#ifndef Mesh_AMesh_h
#define Mesh_AMesh_h


// Library includes

// Project includes
#include <Interfaces/Common/Containers.h>
#include <Renderer/RenderObject.h>

// Forward declarations

// Namespace declarations


namespace Mesh {


class AMesh : public Core::RenderObject
{
public:
	AMesh(Core::AEid id = Core::invalidID, const std::string& name = "");
	virtual ~AMesh();

public:
	// Core::RenderObject implementation
	// {
	virtual void update(float elapsedTime);
	// }

	// Animation specific
	// {
	bool animated() const;
	void animated(bool flag);

	void addAnimation(const std::string& animation);
	bool hasAnimation(const std::string& animation);

	virtual void setAnimation(const std::string& /*animation*/) { }
	// }

protected:
	virtual void setBoundingVolume();

protected:
	StringList	mAnimationList;
	bool		mIsAnimated;
	float		mTimeSinceLastAnimation;

private:

};


}


#endif
