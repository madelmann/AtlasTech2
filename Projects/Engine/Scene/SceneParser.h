
#ifndef _Scene_SceneParser_h_
#define _Scene_SceneParser_h_


// Library includes
#include <list>
#include <string>

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations


namespace Scene {


class SceneParser : private Common::Logger
{
public:
	SceneParser(const Common::Logger *p);
	~SceneParser();

public:
	bool load(const std::string& filename);

protected:

private:
	void parse();

private:
	std::list<std::string>	mData;
	int						mError;
};


}


#endif
