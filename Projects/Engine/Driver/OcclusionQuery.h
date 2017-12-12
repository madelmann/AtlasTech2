
#ifndef _Driver_OcclusionQuery_h_
#define _Driver_OcclusionQuery_h_


// Library includes

// Project includes
#include <Interfaces/Driver/IOcclusionQuery.h>

// Forward declarations
class Driver;

// Namespace declarations


class OcclusionQuery : public IOcclusionQuery
{
public:
	OcclusionQuery(uint id, Driver *driver);
	virtual ~OcclusionQuery();

public:
	void begin();
	void end();

	uint getId() const;
	OcclusionType::E getResult();

	uint getVisibleCount();

protected:

private:
	Driver				*mDriver;
	uint				mId;
	OcclusionType::E	mType;
	uint				mVisibleCount;
};


#endif
