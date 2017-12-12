
#ifndef Interfaces_Common_ADependency_h
#define Interfaces_Common_ADependency_h


// Library includes
#include <cassert>
#include <list>

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class ADependency
{
public:
	virtual ~ADependency() {
		mDependencies.clear();
	}

	void addDependency(ADependency *dependency) {
		if ( dependency == this ) {
			assert(!"already in dependency list!");
			return;
		}

		mDependencies.push_back(dependency);
	}

	bool dependsOn(ADependency *target) {
		for ( DependencyList::iterator it = mDependencies.begin(); it != mDependencies.end(); ++it ) {
			if ( (*it) == target ) {
				return true;
			}
		}

		return false;
	}

protected:

private:
	typedef std::list<ADependency*> DependencyList;

private:
	DependencyList	mDependencies;
};


}


#endif
