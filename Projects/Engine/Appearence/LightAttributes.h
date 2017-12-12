
#ifndef Appearence_LightAttributes_h
#define Appearence_LightAttributes_h


// Library includes

// Project includes
#include <Interfaces/Appearence/IAttribute.h>
#include <Interfaces/Common/Types.h>
#include <Math/Vector4.h>

// Forward declarations

// Namespace declarations


class LightAttributes : public IAttribute
{
public:
	class DrawBufferMode
	{
	public:
		enum E {
			BACK = 0,
			FRONT = 1,
			FRONT_AND_BACK = 2
		};

		static AEenum convert(int value);
	};

public:
	LightAttributes();

public:
	// IAttribute implementation
	//{
	void bind();
	void unbind();
	//}

	vector4f ambient() const;
	void ambient(const vector4f& ambient);

	vector4f diffuse() const;
	void diffuse(const vector4f& diffuse);

	AEenum drawbuffermode() const;
	void drawbuffermode(AEenum m);

	void set(AEenum mode, const vector4f& a, const vector4f& d, const vector4f& s);

	vector4f specular() const;
	void specular(const vector4f& specular);

protected:

private:
	vector4f	mAmbient;
	AEenum		mDrawBufferMode;
	vector4f	mDiffuse;
	vector4f	mSpecular;
};


#endif
