
// Header
#include "SceneParser.h"

// Library includes
#include <fstream>
#include <iostream>

// Project includes
#include <Tools/Strings.h>


namespace Scene {


SceneParser::SceneParser(const Common::Logger *p)
: Common::Logger(p, "SceneParser")
{
}

SceneParser::~SceneParser()
{
}

bool SceneParser::load(const std::string& filename)
{
	debug("load('" + filename + "')");

	if ( filename.empty() ) {
		//warn("invalid file name provided!");
		return false;
	}

	std::ifstream file(filename.c_str());

	if ( !file.is_open() ) {
		warn("could not load configuration file '" + filename + "'!");
		return false;
	}

	if ( file.is_open() ) {
		while ( file.good() ) {
			std::string line;
			getline(file, line);

			Tools::stringTrim(line);
			mData.push_back(line);
		}

		file.close();
	}

	parse();

	return true;
}

void SceneParser::parse()
{
	for ( std::list<std::string>::iterator it = mData.begin(); it != mData.end(); it++ ) {
		char* object = "";
		char* type = "";
		char* name = "";

		sscanf_s((*it).c_str(), "%s %s %s {", object, type, name);

		if ( Tools::StringCompare(object, "sceneobject") ) {
		}
	}
}

}
