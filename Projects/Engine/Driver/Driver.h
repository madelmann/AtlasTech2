
#ifndef _Driver_Driver_h_
#define _Driver_Driver_h_


// Library includes
#include <list>

// Project includes
#include "DriverExtensions.h"
#include "GLWrapper.h"
#include <Common/Logger.h>
#include <Interfaces/Common/Types.h>
#include <Interfaces/Driver/IDriver.h>

// Forward declarations
class IOcclusionQuery;

// Namespace declarations


class Driver : public IDriver,
			   private Common::Logger
{
public:
	Driver(const Common::Logger *parent);
	virtual ~Driver();

public:
	char* getError() const;
	bool initialize();
	void shutdown();

	void bindFrameBufferEXT(AEuint targetId);
	void unbindFrameBufferEXT();

	DriverExtensions* provideDriverExtensions();

	IOcclusionQuery* createOcclusionQuery();
	void deleteOcclusionQuery(IOcclusionQuery *query);
	IOcclusionQuery* getCurrentOcclusionQuery() const;
	void setCurrentOcclusionQuery(IOcclusionQuery *query);

protected:

private:
	void destroyFrameBufferEXT();
	void initFrameBufferEXT();
	void initVertexBufferObjects();

	DriverExtensions mDriverExtensions;

	char* mError;

	IOcclusionQuery*			mCurrentOcclusionQuery;
	std::list<IOcclusionQuery*>	mOcclusionQueries;

	// FrameBufferEXT - begin
	AEuint	mDepthRenderBuffer;
	AEuint	mFrameBuffer;
	// FrameBufferEXT - end
};


#endif
