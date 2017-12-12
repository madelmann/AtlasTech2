
// Header
#include "SceneObjectParser.h"

// Library includes

// Project includes


namespace Scene {


SceneObjectParser::SceneObjectParser(const Common::Logger *p)
: Logger(p, "SceneObjectParser")
{
}

SceneObjectParser::~SceneObjectParser()
{
}

void SceneObjectParser::setData(const string& data)
{
	mData = data;
}


}
