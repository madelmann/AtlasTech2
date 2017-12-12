
#ifndef _Sound_Manager_h_
#define _Sound_Manager_h_


// Library includes
#include <list>

// Project includes
#include "Sound.h"
#include <Common/Logger.h>
#include <Interfaces/Sound/IManager.h>

// Forward declarations
namespace Core {
	class EngineSettings;
}
class IMediaPathManager;

// Namespace declarations


namespace SoundSpace {

// Forward declarations
class Sound;

class Manager : public IManager,
				private Common::Logger
{
public:
	Manager(const Common::Logger *p);
	~Manager();

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
	// {
	Sound* create(const std::string& filename);
	Sound* create2D(const std::string& filename);
	Sound* create3D(const std::string& filename);
	// }

	// Connectors
	// {
	void connectEngineSettings(Core::EngineSettings *s);
	void connectMediaPathManager(IMediaPathManager *m);
	// }

protected:

private:
	bool createAudioResource(const std::string& filename, Sound *sound);
	Sound* find(const std::string& name) const;

private:
	typedef std::list<AudioResource*> ResourceList;
	typedef std::list<Sound*> SoundList;

private:
	ResourceList	mAudioResources;
	ALCcontext		*mContext;
	ALCdevice		*mDevice;
	SoundList		mSounds;

	Core::EngineSettings	*mEngineSettings;
	IMediaPathManager		*mMediaPathManager;
};


}


#endif
