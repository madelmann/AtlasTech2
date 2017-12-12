
#ifndef _Tools_Files_h_
#define _Tools_Files_h_


// Library includes
#include <string>
#include <vector>

// Project includes

// Forward declaration

// Namespace declarations


namespace Tools {
namespace Files {


bool exists(const std::string& filename);

std::string ChangeFileExt(const std::string& filename, const std::string& newExt);
std::string _ExtractFileExt(const std::string& filename);
std::string ExtractFileExt(const std::string& filename);
std::string ExtractFilename(const std::string& filename);
std::string ExtractPathname(const std::string& pathname);
std::string _RemoveFileExt(const std::string& filename);
std::string RemoveFileExt(const std::string& filename);


class FileBrowser
{
public:
	FileBrowser(const std::string& path, const std::string& ext);

public:
	void find(const std::string& path, const std::string& ext);
	void reset();

	std::string activeFile();
	std::string nextFile();
	std::string previousFile();

private:
	typedef std::vector<std::string> StringVector;

private:
	int				mFileIndex;
	StringVector	mFiles;
};


}
}


#endif
