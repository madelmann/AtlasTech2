
#ifndef _Appearence_BlendAttributes_h_
#define _Appearence_BlendAttributes_h_


// Library includes

// Project includes
#include <Interfaces/Appearence/IAttribute.h>
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


class BlendAttributes : public IAttribute
{
public:
	class BlendingFactorDestination
	{
	public:
		enum E {
			ZERO = 0,
			ONE = 1,
			SRC_COLOR = 2,
			ONE_MINUS_SRC_COLOR = 3,
			SRC_ALPHA = 4,
			ONE_MINUS_SRC_ALPHA = 5,
			DST_ALPHA = 6,
			ONE_MINUS_DST_ALPHA = 7
		};

		static AEenum convert(int value);
		static AEenum convert(const std::string& value);
	};

	class BlendingFactorSource
	{
	public:
		enum E {
			ZERO = 0,
			ONE = 1,
			DST_COLOR = 2,
			ONE_MINUS_DST_COLOR = 3,
			SRC_ALPHA_SATURATE = 4,
			SRC_ALPHA = 5,
			ONE_MINUS_SRC_ALPHA = 6,
			DST_ALPHA = 7,
			ONE_MINUS_DST_ALPHA = 8
		};

		static AEenum convert(int value);
		static AEenum convert(const std::string& value);
	};

public:
	BlendAttributes();

public:
	// IAttribute implementation
	//{
	void bind();
	void unbind();
	//}

	AEenum destination() const;
	void destination(AEenum d);

	void set(AEenum src, AEenum dest);

	AEenum source() const;
	void source(AEenum s);

protected:

private:
	AEenum	mDestination;
	AEenum	mSource;
};


#endif
