
#ifndef _Object_Object_h_
#define _Object_Object_h_


class Object
{
public:
	Object()
	: mId(0)
	{ }
	Object(unsigned int id)
	: mId(id)
	{ }
	virtual ~Object() { }

	unsigned int getId() const {
		return mId;
	}
	void setId(unsigned int id) {
		mId = id;
	}

	bool operator == (const Object equal) { return (mId == equal.mId); }
	bool operator < (const Object equal) { return (mId < equal.mId); }
	bool operator > (const Object equal) { return (mId > equal.mId); }

protected:

private:
	unsigned int	mId;
};


#endif
