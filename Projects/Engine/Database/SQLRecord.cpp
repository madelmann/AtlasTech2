
// Header
#include "SQLRecord.h"

// Library includes

// Project includes

// Namespace declarations


namespace Database {


SQLRecord::SQLRecord()
{
	mIterator = mEntries.begin();
}

SQLRecord::~SQLRecord()
{
	mEntries.clear();

	mIterator = mEntries.begin();
}

SQLEntry SQLRecord::operator[] (unsigned int idx)
{
	if ( idx >= size() ) {
		return SQLEntry();
	}

	return mEntries[idx];
}

SQLEntry SQLRecord::operator[] (const std::string& column)
{
	for ( std::vector<SQLEntry>::iterator it = mEntries.begin(); it != mEntries.end(); ++it ) {
		if ( strcmp((*it).Name.c_str(), column.c_str()) == 0 ) {
			return (*it);
		}
	}

	return SQLEntry();
}

void SQLRecord::add(const SQLEntry& entry)
{
	mEntries.push_back(entry);

	mIterator = mEntries.begin();
}

bool SQLRecord::empty() const
{
	return mEntries.empty();
}

bool SQLRecord::hasNext() const
{
	if ( mIterator != mEntries.end() ) {
		return true;
	}

	return false;
}

void SQLRecord::next()
{
	mIterator++;
}

size_t SQLRecord::size() const
{
	return mEntries.size();
}

SQLEntry SQLRecord::value()
{
	return (*mIterator);
}


}
