
#ifndef Core_GeneralManager_h
#define Core_GeneralManager_h


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Core/IGeneralManager.h>

// Forward declarations

// Namespace declarations


namespace Core {


class GeneralManager :	public IGeneralManager,
						private Common::Logger
{
public:
	GeneralManager(const Common::Logger *p);
	~GeneralManager();

public:
	// IGeneralManager implementation
	//{
	void add(AManager *m);
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void reset();
	void shutdown();
	void update(float elapsedTime);
	//}

protected:

private:
	typedef std::list<AManager*> ManagerList;

private:
	void sort();

private:
	ManagerList	mManagers;
};


}


#endif
