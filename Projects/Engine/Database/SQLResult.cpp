
// Header
#include "SQLResult.h"

// Library includes

// Project includes

// Namespace declarations


namespace Database {


SQLResult::SQLResult()
{
	mIterator = mRecords.begin();
}

SQLResult::~SQLResult()
{
	mRecords.clear();

	mIterator = mRecords.begin();
}

SQLRecord SQLResult::operator[] (unsigned int idx)
{
	if ( idx >= size() ) {
		return SQLRecord();
	}

	return mRecords[idx];
}

void SQLResult::add(const SQLRecord& record)
{
	mRecords.push_back(record);

	mIterator = mRecords.begin();
}

bool SQLResult::empty() const
{
	return mRecords.empty();
}

bool SQLResult::next()
{
	if ( mIterator++ != mRecords.end() ) {
		return true;
	}

	return false;
}

size_t SQLResult::size() const
{
	return mRecords.size();
}

SQLRecord SQLResult::value() const
{
	return (*mIterator);
}


}
