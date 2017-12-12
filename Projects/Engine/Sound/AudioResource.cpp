
// Header
#include "AudioResource.h"

// Library includes

// Project includes

// Namespace declarations


namespace SoundSpace {


AudioResource::AudioResource(Core::AEid id, const std::string& name)
: Core::Object(id, name)
{
}

AudioData& AudioResource::data()
{
	return mAudioData;
}

void AudioResource::data(SoundSpace::AudioData &data)
{
	mAudioData = data;
}


}
