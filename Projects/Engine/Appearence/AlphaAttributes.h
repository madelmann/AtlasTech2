
#ifndef _Appearence_AlphaAttributes_h_
#define _Appearence_AlphaAttributes_h_


// Library includes

// Project includes
#include <Interfaces/Appearence/IAttribute.h>
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


class AlphaAttributes : public IAttribute
{
public:
	class Function
	{
	public:
		enum E {
			Never = 0,
			Less = 1,
			Equal = 2,
			LEqual = 3,
			Greater = 4,
			NotEqual = 5,
			GEqual = 6,
			Always = 7
		};

		static AEenum convert(int value);
		static AEenum convert(const std::string& value);
	};

public:
	AlphaAttributes();

public:
	// IAttribute implementation
	//{
	void bind();
	void unbind();
	//}

	AEenum function() const;
	void function(AEenum f);

	float reference() const;
	void reference(float r);

	void set(AEenum function, float reference);

protected:

private:
	AEenum	mFunction;
	float	mReference;
};


#endif
