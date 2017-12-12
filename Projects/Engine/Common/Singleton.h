
#ifndef _Common_Singleton_h_
#define _Common_Singleton_h_


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


template <class T_DERIVED>
class Singleton
{
public:
	static T_DERIVED& GetInstance() {
		static T_DERIVED oInstance;
		return oInstance;
	}

protected:
	Singleton() { }

private:
	Singleton( const Singleton& );
	Singleton& operator=( const Singleton& ) { return *this; }
};


#endif
