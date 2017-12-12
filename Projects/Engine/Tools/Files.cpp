
// Header
#include "Files.h"

// Library includes
#include <fstream>
#include <io.h>

// Project includes

// Namespace declarations


namespace Tools {
namespace Files {


bool exists(const std::string& filename)
{
	if ( filename.empty() ) {
		return false;
	}

	std::ifstream file( filename.c_str() );
	return (file != 0);
}


// Deprecated - begin
std::string _ExtractFileExt(const std::string& filename)
{
	if ( filename.empty() ) {
		return std::string();
	}

	unsigned int d = filename.length();
    while ( filename[d] != '.' && d > 0 ) {
	    d--;
    }

	if ( d <= 0 ) {
	    return std::string();
	}

	d++;

    char buffer[255] = "";
	sprintf_s(buffer, "%s", filename.c_str());

	std::string result;

    for(unsigned int i = d, j = 0; i < filename.length(); i += 1, j += 1) {
		result += buffer[i];
    }

    result[filename.length() - d] = '\0';

	return std::string(result);
}

std::string _RemoveFileExt(const std::string& filename)
{
	if ( filename.empty() ) {
		return std::string();
	}

    unsigned int d = filename.length();
    while ( filename[d] != '.' && d > 0 ) {
	    d--;
    }

	if ( d <= 0 ) {
	    return filename;
	}

    std::string buffer = filename;

    buffer[d] = '\0';

    return buffer;
}
// Deprecated - end


std::string ChangeFileExt(const std::string& filename, const std::string& newExt)
{
	std::string result = RemoveFileExt(filename);
	return result + "." + newExt;
}

std::string ExtractFileExt(const std::string& filename)
{
	return filename.substr(filename.find_last_of(".") + 1);
}

std::string ExtractFilename(const std::string& filename)
{
	if ( filename.empty() ) {
		return filename;
	}

	unsigned int d = filename.length();
	while ( (filename[d] != '\\' && d > 0) || ( filename[d] != '/' && d > 0) ) {
		d--;
	}

	if ( d <= 0 ) {
		return filename;
	}

	d++;
	//int len = filename.length() - d;

	std::string result;

	for ( unsigned int i = d; i < filename.length(); i += 1 ) {
		result += filename[i];
	}

	return result;
}

std::string ExtractPathname(const std::string& pathname)
{
	if ( pathname.empty() ) {
		return pathname;
	}

	unsigned int d = pathname.length();
	while ( (pathname[d] != '\\' && pathname[d] != '/') && d > 0 ) {
		d--;
	}

	if ( d <= 0 ) {
		return pathname;
	}

	d++;	// to get the (back)slash
	std::string result;

	for ( unsigned int i = 0; i < d; i += 1 ) {
		result += pathname[i];
	}

	return result;
}

std::string RemoveFileExt(const std::string& filename)
{
	if ( filename.empty() ) {
		return std::string();
	}

	std::string ext = ExtractFileExt(filename);

	if ( ext.empty() || ext == filename ) {
	    return filename;
	}

    std::string buffer = filename;
	buffer.resize(filename.length() - ext.length() - 1);

    return buffer;
}


FileBrowser::FileBrowser(const std::string& path, const std::string& ext)
{
	find(path, ext);
}

std::string FileBrowser::activeFile()
{
	if ( mFiles.empty() || mFileIndex < 0 || mFileIndex > (int)mFiles.size() - 1 ) {
		return "";
	}

	return mFiles[mFileIndex];
}

void FileBrowser::find(const std::string& path, const std::string& ext)
{
	reset();

	struct _finddata_t file;
	long hFile;

	/* Find first file in current directory */
	hFile = _findfirst(std::string(path + "/*." + ext).c_str(), &file);
	if ( hFile == -1L ) {
		hFile = _findfirst(std::string(path + "/*." + ext).c_str(), &file);

		if ( hFile == -1L ) {
			return;
		}
	}

	mFiles.push_back(file.name);

	/* Find the rest of the files */
	while ( _findnext( hFile, &file ) == 0 ) {
		mFiles.push_back(file.name);
	}

	_findclose( hFile );

	mFileIndex = -1;
}

std::string FileBrowser::nextFile()
{
	if ( mFileIndex > (int)mFiles.size() - 1 ) {
		mFileIndex = -1;
	}

	mFileIndex++;

	return activeFile();
}

std::string FileBrowser::previousFile()
{
	if ( mFileIndex < 0 ) {
		mFileIndex = mFiles.size();
	}

	mFileIndex--;

	return activeFile();
}

void FileBrowser::reset()
{
	mFiles.clear();
	mFileIndex = -1;
}


}
}
