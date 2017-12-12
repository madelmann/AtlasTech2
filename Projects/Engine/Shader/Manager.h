
#ifndef _Shader_ShaderManager_h_
#define _Shader_ShaderManager_h_


// Library includes
#include <list>

// Project includes
#include <Common/Logger.h>
#include <Interfaces/Shader/IManager.h>

// Forwards declarations
namespace Core {
	class EngineSettings;
}
class IMediaPathManager;

// Namespace declarations


namespace ShaderSpace {


class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	virtual ~Manager();

	// Common::AManager implementation
	//{
	void clear();
	void configurationComplete();
	void init();
	void manage();
	void shutdown();
	void update(float elapsedTime);
	//}

	// IManager implementation
	//{
	Shader* create(const std::string& filename, const std::string& path);
	Shader* find(const std::string& shader);
	//}

	// Connectors
	//{
	void connectEngineSettings(Core::EngineSettings *s);
	void connectMediaPathManager(IMediaPathManager *m);
	//}

protected:

private:
	typedef std::list<Shader*> ShaderList;

private:
	ShaderList	mShaders;

	Core::EngineSettings	*mEngineSettings;
	IMediaPathManager		*mMediaPathManager;
};


}

#endif
