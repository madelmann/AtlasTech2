
#ifndef Tools_SharedPtr_h
#define Tools_SharedPtr_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


class ReferenceCounter
{
public:
	ReferenceCounter()
	: mCount(0)
	{ }

	void _add_ref() {
		mCount++;
	}
	int _release_ref() {
		return --mCount;
	}

private:
	int mCount;
};

template <class T>
class SharedPtr
{
public:
	explicit SharedPtr(T* ptr = 0)
	{
		mPtr = ptr;
		mReference = new ReferenceCounter();
		mReference->_add_ref();
	}
	explicit SharedPtr(const SharedPtr<T>& ptr)
	{
		mPtr = ptr.get();
		mReference = ptr.mReference;
		mReference->_add_ref();
	}

	~SharedPtr() {
		if ( mReference->_release_ref() == 0 ) {
			if ( mPtr ) {
				delete mPtr;
				mPtr = 0;
			}
			delete mReference;
			mReference = 0;
		}
	}

public:
	T* get() {
		return mPtr;
	}
	T* get() const {
		return mPtr;
	}

	void reset(T* ptr) {
		mPtr = ptr;
	}

public:
	SharedPtr& operator= (const SharedPtr& other) {
		if ( this != &other ) {
			if ( mReference->_release_ref() == 0 ) {
				if ( mPtr ) {
					delete mPtr;
					mPtr = 0;
				}

				delete mReference;
				mReference = 0;
			}

			mPtr = other.get();
			mReference = other.mReference;
			mReference->_add_ref();
		}

		return *this;
	}

	inline bool operator! () const {
		return !mPtr;
	}
	inline bool operator() () const {
		return (mPtr != 0);
	}
	inline bool operator== (const SharedPtr& other) const {
		return (mPtr == other.mPtr);
	}

	inline T& operator*() {
		return *mPtr;
	}
	inline T& operator*() const {
		return *mPtr;
	}

	inline T* operator->() {
		return mPtr;
	}
	inline T* operator->() const {
		return mPtr;
	}

private:
	ReferenceCounter *mReference;
	T* mPtr;
};


#endif
