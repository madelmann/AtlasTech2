
#ifndef _Interfaces_Driver_IOcclusionQuery_h_
#define _Interfaces_Driver_IOcclusionQuery_h_


// Library includes

// Project includes
#include <Interfaces/Common/Types.h>

// Forward declarations

// Namespace declarations


class IOcclusionQuery
{
public:
	class OcclusionType
	{
	public:
		enum E {
			NotAvailable = 0,
			NotOccluded,
			Occluded
		};
	};

	virtual void begin() = 0;
	virtual void end() = 0;

	virtual uint getId() const = 0;
	virtual OcclusionType::E getResult() = 0;

	virtual uint getVisibleCount() = 0;

	virtual ~IOcclusionQuery() { }
};


#endif
