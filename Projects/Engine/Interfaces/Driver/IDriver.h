
#ifndef _Interfaces_Driver_IDriver_h_
#define _Interfaces_Driver_IDriver_h_


// Library includes

// Project includes

// Forward declarations
class DriverExtensions;
class IOcclusionQuery;

// Namespace declarations


class IDriver
{
public:
	virtual ~IDriver() { }

public:
	virtual DriverExtensions* provideDriverExtensions() = 0;

	virtual IOcclusionQuery* createOcclusionQuery() = 0;
	virtual void deleteOcclusionQuery(IOcclusionQuery *query) = 0;
	virtual IOcclusionQuery* getCurrentOcclusionQuery() const = 0;
	virtual void setCurrentOcclusionQuery(IOcclusionQuery *query) = 0;
};


#endif
