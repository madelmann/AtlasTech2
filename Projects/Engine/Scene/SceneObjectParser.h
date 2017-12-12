
#ifndef _Scene_SceneObjectParser_h_
#define _Scene_SceneObjectParser_h_


// Library includes
#include <string>

// Project includes
#include <Common/Logger.h>

// Forward declarations

// Namespace declarations
using namespace std;


namespace Scene {


class SceneObjectParser : private Common::Logger
{
public:
	SceneObjectParser(const Common::Logger *p);
	~SceneObjectParser();

public:
	void setData(const string& data);

protected:

private:
	string	mData;
};


}


#endif
