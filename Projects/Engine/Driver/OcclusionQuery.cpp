
// Header
#include "OcclusionQuery.h"

// Library includes

// Project includes
#include "Driver.h"
#include "GLWrapper.h"

// Namespace declarations


OcclusionQuery::OcclusionQuery(uint id, Driver *driver)
: mDriver(driver),
  mId(id),
  mType(OcclusionType::NotAvailable),
  mVisibleCount(0)
{
}

OcclusionQuery::~OcclusionQuery()
{
}

void OcclusionQuery::begin()
{
	assert(mDriver);
	assert(mDriver->getCurrentOcclusionQuery() == 0);

	aeBeginQuery(mId);

	mType = OcclusionType::NotAvailable;
	mVisibleCount = 0;

	mDriver->setCurrentOcclusionQuery(this);
}

void OcclusionQuery::end()
{
	assert(mDriver);
	assert(mDriver->getCurrentOcclusionQuery() == this);

	aeEndQuery();

	mDriver->setCurrentOcclusionQuery(0);
}

uint OcclusionQuery::getId() const
{
	return mId;
}

IOcclusionQuery::OcclusionType::E OcclusionQuery::getResult()
{
	assert(mDriver);
	assert(mDriver->getCurrentOcclusionQuery() != this);		// Not allowed during begin/end pair!

	if ( mType == OcclusionType::NotAvailable ) {
		GLuint result;
		glGetQueryObjectuivARB(mId, GL_QUERY_RESULT_ARB, &result);

		if ( result != GL_FALSE ) {
			glGetQueryObjectuivARB(mId, GL_PIXEL_COUNT_NV, &result);
			mType = result != 0 ? OcclusionType::NotOccluded : OcclusionType::Occluded;
			mVisibleCount = (uint)result;
		}
	}

	return mType;
}

uint OcclusionQuery::getVisibleCount()
{
	if ( mType == OcclusionType::NotAvailable ) {
		return 0;
	}

	return mVisibleCount;
}
