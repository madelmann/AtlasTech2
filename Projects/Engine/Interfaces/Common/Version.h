
#ifndef Interfaces_Common_Version_h
#define Interfaces_Common_Version_h


// Library includes

// Project includes

// Forward declarations

// Namespace declarations


namespace Common {


class Version
{
public:
	virtual ~Version() { }

public:
	unsigned int getMajorVersion() const {
		return mMajor;
	}
	unsigned int getMinorVersion() const {
		return mMinor;
	}
	unsigned int getRevision() const {
		return mRevision;
	}

protected:
	Version(unsigned int major = -1, unsigned int minor = -1, unsigned int revision = -1)
	: mMajor(major),
	  mMinor(minor),
	  mRevision(revision)
	{ }

protected:
	void setMajorVersion(unsigned int value) {
		mMajor = value;
	}
	void setMinorVersion(unsigned int value) {
		mMinor = value;
	}
	void setRevision(unsigned int value) {
		mRevision = value;
	}

private:
	unsigned int mMajor;
	unsigned int mMinor;
	unsigned int mRevision;
};


}


#endif
